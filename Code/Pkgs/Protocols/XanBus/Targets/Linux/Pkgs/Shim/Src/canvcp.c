/******************************************************************************

  (c) 2020 Xantrex Canada ULC. All rights reserved.
  All trademarks are owned or licensed by Xantrex Canada ULC,
  its subsidiaries or affiliated companies.

****************************************************************************//**

  @addtogroup canvcp CANVCP
  @{
  @brief  CAN over Virtual Comm Port for Lawicel CANUSB

****************************************************************************//**

  @page canvcp_design CAN over Virtual Comm Port for Lawicel CANUSB

    This file provides the interface bewteen the XanBus shim and the
    virtual com port connected to the Lawicel CANUSB module

  @note
    This is the Linux version
    This file connects to the CANUSB on device /dev/ttyUSB0

*******************************************************************************/

/*==============================================================================
                              Includes
==============================================================================*/


#include <stdio.h>          // For printf
#include <string.h>
#include <stdlib.h>         // For strtol
#include <unistd.h>         // For usleep
#include <fcntl.h>
#include <termios.h>
#include "canvcp.h"         // API for this module
#include "candrv.h"

/*==============================================================================
                              Defines
==============================================================================*/

#define CAN_DEVICE       "/dev/ttyUSB0"
#define CAN_BAUD         B230400

#define FRAME_ID_LENGTH  8
#define FRAME_MIN_LENGTH ( FRAME_ID_LENGTH + 2 )

// Timing for getting version string
#define VERSION_TRY_MS      100
#define VERSION_TIMEOUT_MS 2000
#define VERSION_TRIES      ( VERSION_TIMEOUT_MS / VERSION_TRY_MS )

// Number of times canvcp_fnGetStatus is called before requesting
// the latest from the hardware
#define STATUS_SKIPS       32


// Linux timing in microseconds
#define VERSION_SLEEP      ( VERSION_TRY_MS * 1000 )
#define CANVCP_mSLEEP(n)   usleep(n)


// Size of circular input buffer
#define CIRC_BUFFER_SIZE 4096

// Size of the linear message buffer
#define MSG_BUFFER_SIZE  100

/*==============================================================================
                           Local/Private Variables
==============================================================================*/

// File handle for virtual serial port
static int CanFd;

// Circular input buffer
static char acInBuffer[ CIRC_BUFFER_SIZE ];
static sint32 slInBufStart;
static sint32 slInBufEnd;

// Linear message buffer
static char acMsgBuffer[ MSG_BUFFER_SIZE ];

// Latest received bus status
static TFXCAN_RETURNS teBusStatus;

/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/

/*==============================================================================
                           Function Definitions
==============================================================================*/

//! Determine the amount of space remaining in the input buffer
static sint32 canvcp_fnBufRemaining( void )
{
    sint32 slUsed;

    slUsed = slInBufEnd - slInBufStart;
    if ( slUsed < 0 )
    {
        slUsed += CIRC_BUFFER_SIZE;
    }

    return CIRC_BUFFER_SIZE - slUsed;
}

//! Read the contents of the CANUSB buffer into the circular input buffer
static tucBOOL canvcp_fnReadToBuf( void )
{
    size_t ReadBytes;
    ssize_t BytesRead;
    
    // Feel free to read as many bytes left until the wrap-around point
    ReadBytes = CIRC_BUFFER_SIZE - slInBufEnd - 1;
    BytesRead = read( CanFd, &acInBuffer[ slInBufEnd ], ReadBytes );
    if ( BytesRead < 0 )
    {
        perror( "CANVCP read failed" );
        return FALSE;
    }

    if ( BytesRead < ReadBytes )
    {
        // Update pointer with number of bytes actually read and be happy with that
        slInBufEnd += BytesRead;
    }
    else
    {
        // If we were able to read right to the wrap-around then try reading into the other side
        slInBufEnd = 0;
        ReadBytes = canvcp_fnBufRemaining();
        if ( ReadBytes > 0 )
        {             
            BytesRead = read( CanFd, &acInBuffer[ slInBufEnd ], ReadBytes );
            if ( BytesRead < 0 )
            {
                perror( "CANVCP wrap read failed" );
            }
            else
            {
                // Update the counter
                slInBufEnd += BytesRead;
            }
        }
    }
        
    // Success!
    // Return TRUE if something is in the buffer, regardless of
    // whether or not it came in the latest read cycle
    return ( slInBufStart != slInBufEnd ) ? TRUE : FALSE;
}

//! Read a byte from the circular input buffer
static char canvcp_fnGetFromBuf( void )
{
    char RetVal;

    // Return a null character if the buffer is empty
    if ( slInBufEnd == slInBufStart )
    {
        RetVal = '\0';
    }
    else
    {
        // Grab the next character
        RetVal = acInBuffer[ slInBufStart ];

        // And adjust the head pointer
        slInBufStart++;
        if ( slInBufStart >= CIRC_BUFFER_SIZE )
        {
            slInBufStart = 0;
        }
    }

    return RetVal;
}

//! Read the latest message from the circular input buffer
static tucBOOL canvcp_fnParse( void )
{
    char InChar;
    tucBOOL Eom = FALSE;
    tucBOOL Success = TRUE;
    static sint32 slMsgBufEnd = 0;

    // Read characters until a CR, BELL or end is encountered
    do
    {
        InChar = canvcp_fnGetFromBuf();
        switch ( InChar )
        {
        case '\0': // Unexpected end of message
            // Suspend until more data available from module
            return FALSE;

        case '\r': // Normal end of message
        case 7:    // BELL character, meaning an error
            Eom = TRUE;
            // Allow processing to pass through to default
            
        default:
            // Add to end of buffer
            acMsgBuffer[ slMsgBufEnd ] = InChar;
            slMsgBufEnd++;
        }

        // Check limits
        if ( slMsgBufEnd >= ( MSG_BUFFER_SIZE - 1 ) )
        {
            Eom = TRUE;
            Success = FALSE;
            fprintf( stderr, "CANUSB string too big for buffer\n" );
        }
    } while ( !Eom );

    // Null terminate for good measure
    acMsgBuffer[ slMsgBufEnd ] = '\0';
    slMsgBufEnd = 0;

    return Success;
}

//! Process a Status message from the CANUSB
static TFXCAN_RETURNS canvcp_fnParseStatus( char *Buffer )
{
    uint16 StsCode = 0;
    uint16 i;
    char   Digit;
    TFXCAN_RETURNS RetVal = TFXCR_OK;

    // The status number is in two ASCII digits after the F
    for ( i = 1; i < 3; i++ )
    {
        StsCode *= 10;
        Digit = Buffer[ i ];
        if (( Digit >= '0' ) && ( Digit <= '9' ))
        {
            StsCode += Digit - '0';
        }
    }
    
    // Interpret the stscode number
    if ( StsCode & CANVCP_STS_RXQ_FULL )
    {
        fprintf( stderr, "CANUSB Receive queue full\n" );
    }

    if ( StsCode & CANVCP_STS_TXQ_FULL )
    {
        fprintf( stderr, "CANUSB Transmit queue full\n" );
    }

    if ( StsCode & CANVCP_STS_ERR_WARNING )
    {
        fprintf( stderr, "Bus warning\n" );
        RetVal = TFXCR_BUS_WARNING;
    }

    if ( StsCode & CANVCP_STS_OVERRUN )
    {
        fprintf( stderr, "CANUSB bus overrun\n" );
        RetVal = TFXCR_BUS_WARNING;
    }

    if ( StsCode & CANVCP_STS_ERR_PASSIVE )
    {
        fprintf( stderr, "Error passive\n" );
        RetVal = TFXCR_BUS_WARNING;
    }

    if ( StsCode & CANVCP_STS_BUS_ERROR )
    {
        fprintf( stderr, "Bus Error\n" );
    }

    if ( StsCode & CANVCP_STS_ERR_BUSOFF )
    {
        fprintf( stderr, "Bus off\n" );
        RetVal = TFXCR_BUS_OFF;
    }

    return RetVal;
}

//! Read the version from the input buffer
static tucBOOL canvcp_fnParseForVersion( void )
{
    tucBOOL Found = FALSE;

    while ( canvcp_fnParse() )
    {
        if ( acMsgBuffer[ 0 ] == 'V' )
        {
            //printf( "CANUSB Version: %s\n", acMsgBuffer );
            Found = TRUE;
            break;
        }
    }

    return Found;
}

//! Read the latest CAN frame from the input buffer
static tucBOOL canvcp_fnParseForFrame( void )
{
    tucBOOL Found = FALSE;
    
    // Keep reading messages until a frame is found or no message left
    while ( canvcp_fnParse() )
    {
        // The first character of the received message indicates the meaning
        switch ( acMsgBuffer[ 0 ] )
        {
        case 'T':
            // Transmitted extended CAN frame that we're looking for
            Found = TRUE;
            break;

        case 'F':
            // Status information from an F command
            teBusStatus = canvcp_fnParseStatus( acMsgBuffer );
            break;

        case '\r':
            // Bare carriage return, positive reply from transmitted frame command
            break;

        case 'Z':
            // Z messages seem to happen pretty regularly.  Don't know why
            break;

        case 7:
            // BELL character, negative reply from transmitted frame command
            fprintf( stderr, "CANUSB transmit error\n" );
            break;

        default:
            fprintf( stderr, "Unexpected CANUSB message: %s\n", acMsgBuffer );
            break;
        }


        // If a frame was found then break out of the loop without trying
        // to parse another message
        if ( Found )
        {
            break;
        }
    }

    return Found;
}

//! Convert a CANUSB frame string into a binary frame
static tucBOOL canvcp_fnParseFrame( char *pcBuf, CANFRAME *pFrame )
{
    uint32 ulFrameId;
    uchar8 ucDlc;
    uint32 i;
    char c;
    char *pcEnd;
    char acDataBuf[ 20 ];

    // Make sure that the string is long enough to hold the whole frame
    if ( strlen( pcBuf ) < FRAME_MIN_LENGTH )
    {
        fprintf( stderr, "Frame too short: [%s]\n", pcBuf );
        return FALSE;
    }

    // Get the data length code
    c = *( pcBuf + ( FRAME_MIN_LENGTH - 1 ) );
    if (( c < '0' ) || ( c > '8' ))
    {
        fprintf( stderr, "Data length out of range: %c\n", c );
        return FALSE;
    }

    ucDlc = c - '0';
    pFrame->m_CanData.m_u8DataByteCount = ucDlc;

    // Grab the data if available
    if ( ucDlc > 0 )
    {
        if ( strlen( pcBuf ) < ( size_t )( FRAME_MIN_LENGTH + ( 2 * ucDlc )) )
        {
            fprintf( stderr, "Frame not long enough to accommodate data\n" );
            return FALSE;
        }

        memset( acDataBuf, 0, sizeof( acDataBuf ) );

        for ( i = 0; i < ucDlc; i++ )
        {
            strncpy( acDataBuf,
                     pcBuf + ( FRAME_MIN_LENGTH + ( 2 * i )),
                     2 );
            pFrame->m_CanData.m_u8Data[ i ] = ( UINT8 )strtol( acDataBuf, &pcEnd, 16 );
        }
    }

    // Grab the ID
    memset( acDataBuf, 0, sizeof( acDataBuf ) );
    strncpy( acDataBuf, pcBuf + 1, FRAME_ID_LENGTH );
    ulFrameId = strtol( acDataBuf, &pcEnd, 16 );

    // Get the Priority
    pFrame->m_CanData.m_u8Priority = ( UINT8 )(( ulFrameId >> 26 ) & 0x07 );

    // Datapage bit
    pFrame->m_CanData.m_u8DataPage = ( UINT8 )(( ulFrameId >> 24 ) & 0x01 );

    // PF
    pFrame->m_CanData.m_u8PF = ( UINT8 )(( ulFrameId >> 16 ) & 0xff );

    // PS
    pFrame->m_CanData.m_u8PS = ( UINT8 )(( ulFrameId >> 8 ) & 0xff );

    // Source address
    pFrame->m_CanData.m_u8SA = ( UINT8 )( ulFrameId & 0xff );

    return TRUE;
}

//! Initialize the CANUSB module
tucBOOL CANVCP_fnInit( uchar8 ucBitRate )
{
    char cRateNum;
    char acCmdBuf[ 8 ];
    size_t ToWrite;
    ssize_t BytesWritten;
    sint16 i;
    struct termios zTermOpts;

    // Initialize the input buffer
    slInBufStart = 0;
    slInBufEnd = 0;


    // Bus status is OK until proven otherwise
    teBusStatus = TFXCR_OK;

    // Open the serial port for the CANUSB
    CanFd = open( CAN_DEVICE, O_RDWR | O_NONBLOCK | O_NOCTTY );
    if ( CanFd < 0 )
    {
        perror( "Could not open CANUSB at " CAN_DEVICE );
        return FALSE;
    }

    // Configure for raw input
    cfmakeraw( &zTermOpts );

    // Make non-blocking (return right away from read() whether characters received or not)
    zTermOpts.c_cc[ VMIN ] = 0;
    zTermOpts.c_cc[ VTIME ] = 0;
    
    // Configure the serial port as well
    cfsetspeed( &zTermOpts, CAN_BAUD );

    // Flush the input and start communications
    tcflush( CanFd, TCIFLUSH );
    if ( tcsetattr( CanFd, TCSANOW, &zTermOpts ) < 0 )
    {
        // Sucessful return is not a sure-fire indicator of success.
        // The most rigorous check would be to verify with tcgetattr
        perror( "Setting serial port attributes" );
        return FALSE;
    }
    
    // Send three carriage returns to purge any input and output
    strcpy( acCmdBuf, "\r\r\r" );
    ToWrite = strlen( acCmdBuf );
    BytesWritten = write( CanFd, acCmdBuf, ToWrite );
    if ( BytesWritten < ToWrite )
    {
        perror( "Three carriage returns" );
    }

    // Turn off timestamps
    strcpy( acCmdBuf, "Z0\r" );
    ToWrite = strlen( acCmdBuf );
    BytesWritten = write( CanFd, acCmdBuf, ToWrite );
    if ( BytesWritten < ToWrite )
    {
        perror( "Zero timestamps" );
        return FALSE;
    }

    /* FT_Purge( ftHandle, FT_PURGE_RX ); */
    tcflush( CanFd, TCIFLUSH );

    // Get the version string to make sure the communication is right
    strcpy( acCmdBuf, "V\r" );
    ToWrite = strlen( acCmdBuf );
    BytesWritten = write( CanFd, acCmdBuf, ToWrite );
    if ( BytesWritten < ToWrite )
    {
        perror( "Version command" );
        return FALSE;
    }

    for ( i = 0; i < VERSION_TRIES; i++ )
    {
        if ( canvcp_fnReadToBuf() )
        {
            if ( canvcp_fnParseForVersion() )
            {
                break;
            }
        }

        CANVCP_mSLEEP( VERSION_SLEEP );
    }

    if ( i >= VERSION_TRIES )
    {
        fprintf( stderr, "No version string\n" );
        return FALSE;
    }
    
    // Make sure CAN connection is closed before setting bit rate
    strcpy( acCmdBuf, "C\r" );
    ToWrite = strlen( acCmdBuf );
    BytesWritten = write( CanFd, acCmdBuf, ToWrite );
    if ( BytesWritten < ToWrite )
    {
        perror( "Close command" );
    }

    CANVCP_mSLEEP( VERSION_SLEEP );

    // Determine the bit rate
    switch ( ucBitRate )
    {
    case CANDRV_eBIT_RATE_20KBPS:
        cRateNum = '1';
        break;

    case CANDRV_eBIT_RATE_125KBPS:
        cRateNum = '4';
        break;

    case CANDRV_eBIT_RATE_250KBPS:
        cRateNum = '5';
        break;

    case CANDRV_eBIT_RATE_500KBPS:
        cRateNum = '6';
        break;

    case CANDRV_eBIT_RATE_1MBPS:
        cRateNum = '8';
        break;

    default:
        // Error
        return FALSE;
    }

    // Set the bit rate
    /* FT_Purge( ftHandle, FT_PURGE_RX | FT_PURGE_TX ); */
    tcflush( CanFd, TCIFLUSH );
    
    sprintf( acCmdBuf, "S%c\r", cRateNum );
    ToWrite = strlen( acCmdBuf );
    BytesWritten = write( CanFd, acCmdBuf, ToWrite );
    if ( BytesWritten < ToWrite )
    {
        perror( "Speed command" );
        return FALSE;
    }

    // Open the device
    tcflush( CanFd, TCIFLUSH );

    strcpy( acCmdBuf, "O\r" );
    ToWrite = strlen( acCmdBuf );
    BytesWritten = write( CanFd, acCmdBuf, ToWrite );
    if ( BytesWritten < ToWrite )
    {
        perror( "Open command" );
        return FALSE;
    }

    tcflush( CanFd, TCIFLUSH );

    // Success!
    return TRUE;
}

//! Close the CANUSB module
void CANVCP_fnClose( void )
{
    char acCmdBuf[ 8 ];
    ssize_t BytesWritten;
    size_t ToWrite;
    /* FT_STATUS ftStatus; */

    // Close the CAN connection
    strcpy( acCmdBuf, "C\r" );
    ToWrite = strlen( acCmdBuf );
    BytesWritten = write( CanFd, acCmdBuf, ToWrite );
    if ( BytesWritten < ToWrite )
    {
        perror( "Close command" );
    }

    close( CanFd );
}

//! Return the status of the CANUSB module
TFXCAN_RETURNS CANVCP_fnStatus( void )
{
    static uint32 IgnoreCount = 0;
    /* FT_STATUS ftStatus; */
    ssize_t BytesWritten;

    // This function gets called lots of times so don't send the
    // status request to the device everytime
    IgnoreCount++;
    if ( IgnoreCount > STATUS_SKIPS )
    {
        IgnoreCount = 0;

        // Send an F command to get the bus status
        BytesWritten = write( CanFd, "F\r", 2 );
        if ( BytesWritten < 2 )
        {
            return TFXCR_DRV_BUSY;
            perror( "Close command" );
        }
        /* ftStatus = FT_Write( ftHandle, "F\r", 2, &BytesWritten ); */
        /* if ( ftStatus != FT_OK ) */
        /* { */
        /*     return TFXCR_DRV_BUSY; */
        /* } */
    }
        
    return teBusStatus;
}

//! Receive a frame from the CANUSB module
TFXCAN_RETURNS CANVCP_fnReceiveFrame( CANPORT u8PortNumber,
                                      CANFRAME *pFrame )
{
    // Get the latest from the module
    if ( canvcp_fnReadToBuf() )
    {
        // Scan through the buffer for a frame string
        if ( canvcp_fnParseForFrame() )
        {
            // Convert the frame string into a binary frame for the stack
            if ( !canvcp_fnParseFrame( acMsgBuffer, pFrame ) )
            {
                return TFXCR_MSG_NOT_HANDLED;
            }
            else
            {
                return TFXCR_NEW_DATA;
            }
        }
        else
        {
            // None of the received messages were frames
            return TFXCR_NO_DATA;
        }
    }
    else
    {
        // Nothing to report
        return TFXCR_NO_DATA;
    }
}

//! Send a frame to the CANUSB module
TFXCAN_RETURNS CANVCP_fnSendFrame( CANPORT u8PortNumber,
                                      CANFRAME *pFrame,
                                      INT16 *pi16MessageID )
{
    char acBuf[ MSG_BUFFER_SIZE ];   // Outgoing buffer for frame message
    sint32 slBufPos;     // Current position within the buffer
    ssize_t BytesWritten;
    uchar8 ucByte1;
    sint32 i;

    // Message buffers not used
    *pi16MessageID = 1000;

    //-- Convert the binary frame to a hex string --

    // First byte has the three bit priority, a zero and then
    // the data page
    ucByte1 = pFrame->m_CanData.m_u8DataPage;
    ucByte1 |= ( pFrame->m_CanData.m_u8Priority << 2 ) & 0x1c;

    // Write the command, the ID and the data length
    slBufPos = sprintf( acBuf,
                        "T%2.2X%2.2X%2.2X%2.2X%X",
                        ucByte1,
                        pFrame->m_CanData.m_u8PF,
                        pFrame->m_CanData.m_u8PS,
                        pFrame->m_CanData.m_u8SA,
                        pFrame->m_CanData.m_u8DataByteCount );
    
    // Write in the data bytes
    i = 0;
    while ( i < pFrame->m_CanData.m_u8DataByteCount )
    {
        slBufPos += sprintf( acBuf + slBufPos,
                             "%2.2X",
                             pFrame->m_CanData.m_u8Data[ i ] );
        i++;
    }

    // Terminate with a carriage return
    slBufPos += sprintf( acBuf + slBufPos, "\r" );

    acBuf[ slBufPos ] = '\0';

    // -- Send to the CANUSB --
    BytesWritten = write( CanFd, acBuf, slBufPos );
    if ( BytesWritten < 0 )
    {
        perror( "Writing frame to CANUSB" );
        return TFXCR_MSG_NOT_HANDLED;
    }

    if ( BytesWritten != slBufPos )
    {
        fprintf( stderr,
                 "Tried to send %ld characters but actually wrote %d\n",
                 slBufPos,
                 BytesWritten );
        return TFXCR_MSG_NOT_HANDLED;
    }

    return TFXCR_OK;
}

//*****************************************************************************
// End of file.
//! @}
//*****************************************************************************
