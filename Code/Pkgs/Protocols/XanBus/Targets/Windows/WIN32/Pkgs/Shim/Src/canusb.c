/*==============================================================================
Copyright © 2009 Xantrex Technology, Inc. All rights reserved.

This source file and the information contained in it are confidential and 
proprietary to Xantrex Technology, Inc. The reproduction or disclosure, in 
whole or in part, to anyone outside of Xantrex without the written approval of 
a Xantrex officer under a Non-Disclosure Agreement, or to any employee of 
Xantrex who has not previously obtained written authorization for access from 
the individual responsible for the source code, will have a significant 
detrimental effect on Xantrex and is expressly prohibited.

FILE NAME:  canusb.c

PURPOSE:
    This file provides the interface bewteen the XanBus shim and the
    'ftd2xx' drive library for the unit's FTDI USB chip.

FUNCTION(S):
    local:
   
NOTES:
    This file is meant to be portable to either Windows or Linux

CHANGE HISTORY :
$Log: canusb.c $

    ***********************************************
    Revision: XanBus3_TomL/4
    User: TomL     Date: 03/06/09  Time: 02:35AM
    Fixed calculation of remaining space in circular buffer.

    Increased size of circular buffer.

    ***********************************************
    Revision: XanBus3_TomL/3
    User: TomL     Date: 03/05/09  Time: 02:48AM
    Fixed uninitialized variable in canusb interface.

    ***********************************************
    Revision: XanBus3_TomL/2
    User: TomL     Date: 02/12/09  Time: 02:00AM
    Extra Close to allow changing of bit rate
==============================================================================*/

/*==============================================================================
                              Includes
==============================================================================*/


#include <stdio.h>          // For printf
#include <string.h>
#include <stdlib.h>         // For strtol
#ifdef WIN32
#include <windows.h>        // For sleep
#else
#include <unistd.h>         // For usleep
#endif
#include "ftd2xx.h"         // Interface to FTDI chip
#include "canusb.h"         // API for this module
#include "candrv.h"

/*==============================================================================
                              Defines
==============================================================================*/

#define CANFTD_DEVINDEX_MAX ( 10 )
#define FRAME_ID_LENGTH  8
#define FRAME_MIN_LENGTH ( FRAME_ID_LENGTH + 2 )

// Timing for getting version string
#define VERSION_TRY_MS      100
#define VERSION_TIMEOUT_MS 2000
#define VERSION_TRIES      ( VERSION_TIMEOUT_MS / VERSION_TRY_MS )

// Number of times canusb_fnGetStatus is called before requesting
// the latest from the hardware
#define STATUS_SKIPS       32

#ifdef WIN32

// Windows timing in milliseconds
#define VERSION_SLEEP      VERSION_TRY_MS
#define CANUSB_mSLEEP(n)   Sleep(n)

#else

// Linux timing in microseconds
#define VERSION_SLEEP      ( VERSION_TRY_MS * 1000 )
#define CANUSB_mSLEEP(n)   usleep(n)

#endif

// Size of circular input buffer
#define CIRC_BUFFER_SIZE 4096

// Size of the linear message buffer
#define MSG_BUFFER_SIZE  100

/*==============================================================================
                           Local/Private Variables
==============================================================================*/

// CANUSB variables
static FT_HANDLE ftHandle;

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

// Determine the amount of space remaining in the input buffer
static DWORD canusb_fnBufRemaining( void )
{
    sint32 slUsed;

    slUsed = slInBufEnd - slInBufStart;
    if ( slUsed < 0 )
    {
        slUsed += CIRC_BUFFER_SIZE;
    }

    return CIRC_BUFFER_SIZE - slUsed;
}

// Read the contents of the CANUSB buffer into the circular input buffer
static tucBOOL canusb_fnReadToBuf( void )
{
    FT_STATUS ftStatus;
    DWORD RxBytes;
    DWORD TxBytes;
    DWORD ReadBytes;
    DWORD BytesRead;
    DWORD EventStatus;
    DWORD Remaining;
    
    // Is there anything to read?
    ftStatus = FT_GetStatus( ftHandle, &RxBytes, &TxBytes, &EventStatus );
    if ( ftStatus != FT_OK )
    {
        fprintf( stderr, "Could not read status from CANUSB\n" );
        return FALSE;
    }

    if ( RxBytes > 0 )
    {
        // Make sure we don't try to bring in too much
        Remaining = canusb_fnBufRemaining();
        if ( RxBytes > Remaining )
        {
            fprintf( stderr, "More CANUSB bytes (%d) than space in buffer (%d)\n",
		     RxBytes,
		     Remaining );
            RxBytes = Remaining;
        }
        
        // Do we need to handle wrap around?
        ReadBytes = CIRC_BUFFER_SIZE - slInBufEnd - 1;
        if ( ReadBytes < RxBytes )
        {
            ftStatus = FT_Read( ftHandle, &acInBuffer[ slInBufEnd ], ReadBytes, &BytesRead );
            if ( ftStatus != FT_OK )
            {
                fprintf( stderr, "CANUSB read error\n" );
                return FALSE;
            }
            
            if ( BytesRead != ReadBytes )
            {
                fprintf( stderr, "CANUSB read timeout\n" );
                return FALSE;
            }
            
            slInBufEnd = 0;
            RxBytes -= ReadBytes;
        }
        
        ftStatus = FT_Read( ftHandle, &acInBuffer[ slInBufEnd ], RxBytes, &BytesRead );
        if ( ftStatus != FT_OK )
        {
            fprintf( stderr, "CANUSB read error\n" );
            return FALSE;
        }

        if ( BytesRead != RxBytes )
        {
            fprintf( stderr, "CSNUSB read timeout\n" );
            return FALSE;
        }
        
        slInBufEnd += RxBytes;
    }
        
    // Success!
    // Return TRUE if something is in the buffer, regardless of
    // whether or not it came in the latest read cycle
    return ( slInBufStart != slInBufEnd ) ? TRUE : FALSE;
}

// Read a byte from the circular input buffer
static char canusb_fnGetFromBuf( void )
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

// Read the latest message from the circular input buffer
static tucBOOL canusb_fnParse( void )
{
    char InChar;
    tucBOOL Eom = FALSE;
    tucBOOL Success = TRUE;
    static sint32 slMsgBufEnd = 0;

    // Read characters until a CR, BELL or end is encountered
    do
    {
        InChar = canusb_fnGetFromBuf();
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

// Process a Status message from the CANUSB
static TFXCAN_RETURNS canusb_fnParseStatus( char *Buffer )
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
    if ( StsCode & CANUSB_STS_RXQ_FULL )
    {
        fprintf( stderr, "CANUSB Receive queue full\n" );
    }

    if ( StsCode & CANUSB_STS_TXQ_FULL )
    {
        fprintf( stderr, "CANUSB Transmit queue full\n" );
    }

    if ( StsCode & CANUSB_STS_ERR_WARNING )
    {
        fprintf( stderr, "Bus warning\n" );
        RetVal = TFXCR_BUS_WARNING;
    }

    if ( StsCode & CANUSB_STS_OVERRUN )
    {
        fprintf( stderr, "CANUSB bus overrun\n" );
        RetVal = TFXCR_BUS_WARNING;
    }

    if ( StsCode & CANUSB_STS_ERR_PASSIVE )
    {
        fprintf( stderr, "Error passive\n" );
        RetVal = TFXCR_BUS_WARNING;
    }

    if ( StsCode & CANUSB_STS_BUS_ERROR )
    {
        fprintf( stderr, "Bus Error\n" );
    }

    if ( StsCode & CANUSB_STS_ERR_BUSOFF )
    {
        fprintf( stderr, "Bus off\n" );
        RetVal = TFXCR_BUS_OFF;
    }

    return RetVal;
}

// Read the version from the input buffer
static tucBOOL canusb_fnParseForVersion( void )
{
    tucBOOL Found = FALSE;

    while ( canusb_fnParse() )
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

// Read the latest CAN frame from the input buffer
static tucBOOL canusb_fnParseForFrame( void )
{
    tucBOOL Found = FALSE;
    
    // Keep reading messages until a frame is found or no message left
    while ( canusb_fnParse() )
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
            teBusStatus = canusb_fnParseStatus( acMsgBuffer );
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

// Convert a CANUSB frame string into a binary frame
static tucBOOL canusb_fnParseFrame( char *pcBuf, CANFRAME *pFrame )
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

// Initialize the CANUSB module
tucBOOL CANUSB_fnInit( uchar8 ucBitRate )
{
    char cRateNum;
    char acCmdBuf[ 8 ];
    DWORD BytesWritten;
    FT_STATUS ftStatus;
    sint16 i;
    DWORD devIndex;
    char SerNum[ 64 ];

    // Initialize the input buffer
    slInBufStart = 0;
    slInBufEnd = 0;

#ifndef WIN32
    // We want to make sure the library finds the CANUSB's USB identification
    ftStatus = FT_SetVIDPID( 0x0403, 0xffa8 );
    if ( ftStatus != FT_OK )
    {
        printf( "FT_SetVIDPID failed\n" );
    }
#endif
    
    // List the devices and pick the first likely suspect
    for ( devIndex = 0; devIndex < CANFTD_DEVINDEX_MAX; devIndex++ )
    {
        ftStatus = FT_ListDevices( (PVOID)devIndex, SerNum, FT_LIST_BY_INDEX | FT_OPEN_BY_SERIAL_NUMBER );
        if ( ftStatus == FT_OK )
        {
            printf( "Found %s at index %d\n", SerNum, devIndex );
            if (( SerNum[ 0 ] == 'L' )
                && ( SerNum[ 1 ] == 'W' ))
            {
                // Found a Lawicel CANUSB
                break;
            }
        }
    }

    if ( devIndex >= CANFTD_DEVINDEX_MAX )
    {
        fprintf( stderr, "Could not find CANUSB\n" );
        return FALSE;
    }

    // Bus status is OK until proven otherwise
    teBusStatus = TFXCR_OK;

    // Find the module
    ftStatus = FT_OpenEx( SerNum, FT_OPEN_BY_SERIAL_NUMBER, &ftHandle);
    //ftStatus = FT_Open( 0, &ftHandle );
    if ( ftStatus != FT_OK )
    {
        fprintf( stderr, "Could not open CANUSB at index%d\n", devIndex );
        return FALSE;
    }

    // Reset the module
    ftStatus = FT_ResetDevice( ftHandle );
    if ( ftStatus != FT_OK )
    {
        fprintf( stderr, "Could not reset CANUSB\n" );
        return FALSE;
    }

    FT_SetTimeouts( ftHandle, 3000, 3000 );       // 3 second read timeout

    // Send three carriage returns to purge any input and output
    strcpy( acCmdBuf, "O\r" );
    ftStatus = FT_Write( ftHandle, acCmdBuf, strlen( acCmdBuf ), &BytesWritten );
    if ( ftStatus != FT_OK )
    {
        fprintf( stderr, "Three carriage returns failed\n" );
    }

    // Turn off timestamps
    strcpy( acCmdBuf, "Z0\r" );
    ftStatus = FT_Write( ftHandle, acCmdBuf, strlen( acCmdBuf ), &BytesWritten );
    if ( ftStatus != FT_OK )
    {
        fprintf( stderr, "Could not turn off timestamps\n" );
    }

    FT_Purge( ftHandle, FT_PURGE_RX );

    // Get the version string to make sure the communication is right
    strcpy( acCmdBuf, "V\r" );
    ftStatus = FT_Write( ftHandle, acCmdBuf, strlen( acCmdBuf ), &BytesWritten );
    if ( ftStatus != FT_OK )
    {
        fprintf( stderr, "Asking for CANUSB version failed\n" );
        return FALSE;
    }

    for ( i = 0; i < VERSION_TRIES; i++ )
    {
        if ( canusb_fnReadToBuf() )
        {
            if ( canusb_fnParseForVersion() )
            {
                break;
            }
        }

        CANUSB_mSLEEP( VERSION_SLEEP );
    }

    if ( i >= VERSION_TRIES )
    {
        fprintf( stderr, "No version string\n" );
        return FALSE;
    }
    
    // Make sure CAN connection is closed before setting bit rate
    strcpy( acCmdBuf, "C\r" );
    ftStatus = FT_Write( ftHandle, acCmdBuf, strlen( acCmdBuf ), &BytesWritten );
    if ( ftStatus != FT_OK )
    {
        fprintf( stderr, "Could not close CANUSB\n" );
    }

    CANUSB_mSLEEP( VERSION_SLEEP );

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
    FT_Purge( ftHandle, FT_PURGE_RX | FT_PURGE_TX );
    
    sprintf( acCmdBuf, "S%c\r", cRateNum );
    ftStatus = FT_Write( ftHandle, acCmdBuf, strlen( acCmdBuf ), &BytesWritten );
    if ( ftStatus != FT_OK )
    {
        fprintf( stderr, "Failed to set bit rate\n" );
        return FALSE;
    }

    // Open the device
    FT_Purge( ftHandle, FT_PURGE_RX );

    strcpy( acCmdBuf, "O\r" );
    ftStatus = FT_Write( ftHandle, acCmdBuf, strlen( acCmdBuf ), &BytesWritten );
    if ( ftStatus != FT_OK )
    {
        fprintf( stderr, "Failed to open device\n" );
        return FALSE;
    }

    FT_Purge( ftHandle, FT_PURGE_RX );

    // Success!
    return TRUE;
}

// Close the CANUSB module
void CANUSB_fnClose( void )
{
    char acCmdBuf[ 8 ];
    DWORD BytesWritten;
    FT_STATUS ftStatus;

    // Close the CAN connection
    strcpy( acCmdBuf, "C\r" );
    ftStatus = FT_Write( ftHandle, acCmdBuf, strlen( acCmdBuf ), &BytesWritten );
    if ( ftStatus != FT_OK )
    {
        fprintf( stderr, "Could not close CANUSB\n" );
    }

    // Close the device connection
    ftStatus = FT_Close( ftHandle );
    if ( ftStatus != FT_OK )
    {
        fprintf( stderr, "Could not close connection to CANUSB\n" );
    }
}

// Return the status of the CANUSB module
TFXCAN_RETURNS CANUSB_fnStatus( void )
{
    static uint32 IgnoreCount = 0;
    FT_STATUS ftStatus;
    DWORD BytesWritten;

    // This function gets called lots of times so don't send the
    // status request to the device everytime
    IgnoreCount++;
    if ( IgnoreCount > STATUS_SKIPS )
    {
        IgnoreCount = 0;

        // Send an F command to get the bus status
        ftStatus = FT_Write( ftHandle, "F\r", 2, &BytesWritten );
        if ( ftStatus != FT_OK )
        {
            return TFXCR_DRV_BUSY;
        }
    }
        
    return teBusStatus;
}

// Receive a frame from the CANUSB module
TFXCAN_RETURNS CANUSB_fnReceiveFrame( CANPORT u8PortNumber,
                                      CANFRAME *pFrame )
{
    // Get the latest from the module
    if ( canusb_fnReadToBuf() )
    {
        // Scan through the buffer for a frame string
        if ( canusb_fnParseForFrame() )
        {
            // Convert the frame string into a binary frame for the stack
            if ( !canusb_fnParseFrame( acMsgBuffer, pFrame ) )
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

// Send a frame to the CANUSB module
TFXCAN_RETURNS CANUSB_fnSendFrame( CANPORT u8PortNumber,
                                   CANFRAME *pFrame,
                                   INT16 *pi16MessageID )
{
    char acBuf[ MSG_BUFFER_SIZE ];   // Outgoing buffer for frame message
    sint32 slBufPos;     // Current position within the buffer
    sint32 slBytesWritten;
    uchar8 ucByte1;
    sint32 i;
    FT_STATUS ftStatus;

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
    ftStatus = FT_Write( ftHandle, acBuf, slBufPos, &slBytesWritten );
    if ( ftStatus != FT_OK )
    {
        fprintf( stderr, "Writing frame to CANUSB failed\n" );
        return TFXCR_MSG_NOT_HANDLED;
    }

    if ( slBytesWritten != slBufPos )
    {
        fprintf( stderr,
                 "Tried to send %d characters but actually wrote %d\n",
                 slBufPos,
                 slBytesWritten );
        return TFXCR_MSG_NOT_HANDLED;
    }

    return TFXCR_OK;
}
