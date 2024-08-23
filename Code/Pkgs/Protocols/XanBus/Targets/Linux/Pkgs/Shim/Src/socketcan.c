/******************************************************************************

  (c) 2020 Xantrex Canada ULC. All rights reserved.
  All trademarks are owned or licensed by Xantrex Canada ULC,
  its subsidiaries or affiliated companies.

****************************************************************************//**

  @addtogroup socketcan SOCKETCAN
  @{
  @brief  Interface to Linux's socketcan

****************************************************************************//**

  @page socket_design Interface to Linux's socketcan

    This file provides the interface bewteen the XanBus shim and the
    socketcan interface provided by standard Linux CAN drivers

  @note
    This file connects to can0 by default

*******************************************************************************/

/*==============================================================================
                              Includes
==============================================================================*/


#include <stdio.h>          // For printf
#include <string.h>
#include <stdlib.h>         // For strtol
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include "socketcan.h"         // API for this module

/*==============================================================================
                              Defines
==============================================================================*/

#define CAN_DEVICE       "can0"


// Number of times socketcan_fnGetStatus is called before requesting
// the latest from the hardware
#define STATUS_SKIPS       32




/*==============================================================================
                           Local/Private Variables
==============================================================================*/

// Socket for CAN interface
static int CanSock;

// Latest received bus status
static TFXCAN_RETURNS teBusStatus;

/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/

/*==============================================================================
                           Function Definitions
==============================================================================*/


//! Initialize the socketcan interface module
tucBOOL SOCKETCAN_fnInit( uchar8 ucBitRate )
{
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_filter filter;
    int result;
    
    // Bus status is OK until proven otherwise
    teBusStatus = TFXCR_OK;

    // Open the socket for the CAN interface
    CanSock = socket( PF_CAN, SOCK_RAW, CAN_RAW );
    if ( CanSock < 0 )
    {
        perror( "Could not open raw CAN socket" );
        return FALSE;
    }

    // Set up filter to allow only extended frames
    filter.can_id = CAN_EFF_FLAG;
    filter.can_mask = CAN_EFF_FLAG | CAN_RTR_FLAG;
    result = setsockopt( CanSock, SOL_CAN_RAW, CAN_RAW_FILTER, &filter, sizeof( filter ) );
    if ( result != 0 )
    {
        perror( "SOCKETCAN_fnInit - setsockopt" );
    }

    // Make this a non-blocking socket
    fcntl(CanSock, F_SETFL, O_NONBLOCK);

    // Connect to the named CAN interface
    strcpy( ifr.ifr_name, CAN_DEVICE );
    ioctl( CanSock, SIOCGIFINDEX, &ifr);

    memset( &addr, 0, sizeof(addr) );
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if ( bind( CanSock, (struct sockaddr *)&addr, sizeof(addr)) < 0 )
    {
        perror( "SocketCAN Bind" );
        return FALSE;
    }

    // Success!
    return TRUE;
}

//! Close the CANUSB module
void SOCKETCAN_fnClose( void )
{
    close( CanSock );
}

//! Return the status of the CANUSB module
TFXCAN_RETURNS SOCKETCAN_fnStatus( void )
{
    return teBusStatus;
}

//! Receive a frame from the CANUSB module
TFXCAN_RETURNS SOCKETCAN_fnReceiveFrame( CANPORT u8PortNumber,
                                         CANFRAME *pFrame )
{
    ssize_t BytesRead;
    struct can_frame SockFrame;
    int i;
    TFXCAN_RETURNS RetVal = TFXCR_NO_DATA;

    BytesRead = read( CanSock, &SockFrame, sizeof( SockFrame ) );
    if ( BytesRead < 0 )
    {
        // Read from socket failed - expected reason would be nothing received
        if ( errno != EWOULDBLOCK )
        {
            perror( "Unexpected error on CAN read" );
        }
    }
    else if ( BytesRead == sizeof( SockFrame ) )
    {
        // Make sure that pFrame points to something to write to
        // and that this is an extended frame
        if (( pFrame != NULL )
            && (( SockFrame.can_id & CAN_EFF_FLAG ) != 0 ))
        {
      
            // Copy contents of SockFrame into the CANFRAME
            pFrame->m_CanData.m_u8Priority = (uchar8)(( SockFrame.can_id >> 26 ) & 0x07 );
            pFrame->m_CanData.m_u8DataPage = (uchar8)(( SockFrame.can_id >> 24 ) & 0x03 );
            pFrame->m_CanData.m_u8PF = (uchar8)(( SockFrame.can_id >> 16 ) & 0xff );
            pFrame->m_CanData.m_u8PS = (uchar8)(( SockFrame.can_id >> 8 ) & 0xff );
            pFrame->m_CanData.m_u8SA = (uchar8)( SockFrame.can_id & 0xff );
            
            pFrame->m_CanData.m_u8DataByteCount = SockFrame.can_dlc;
            
            for ( i = 0; i < SockFrame.can_dlc; i++ )
            {
                pFrame->m_CanData.m_u8Data[ i ] = SockFrame.data[ i ];
            }
            
            RetVal = TFXCR_NEW_DATA;
        }
    }
    else
    {
        // That's weird!  Read should always return a full frame
        RetVal = TFXCR_MSG_NOT_HANDLED;
    }

    // Indicate whether or not an extended frame was received
    return RetVal;
}

//! Send a frame to the socket module
TFXCAN_RETURNS SOCKETCAN_fnSendFrame( CANPORT u8PortNumber,
                                      CANFRAME *pFrame,
                                      INT16 *pi16MessageID )
{
    struct can_frame SockFrame;
    ssize_t BytesWritten;
    int i;
    uchar8 ucByte1;
    static tucBOOL ucIsXmitWorking = TRUE;

    //---  Convert CANFRAME to can_frame ---

    // First ID byte has the three bit priority, a zero and then
    // the data page
    ucByte1 = pFrame->m_CanData.m_u8DataPage;
    ucByte1 |= ( pFrame->m_CanData.m_u8Priority << 2 ) & 0x1c;

    SockFrame.can_id = ucByte1 << 24;
    SockFrame.can_id |= pFrame->m_CanData.m_u8PF << 16;
    SockFrame.can_id |= pFrame->m_CanData.m_u8PS << 8;
    SockFrame.can_id |= pFrame->m_CanData.m_u8SA;
  
    SockFrame.can_dlc = pFrame->m_CanData.m_u8DataByteCount;

    for ( i = 0; i < SockFrame.can_dlc; i++ )
    {
        SockFrame.data[ i ] = pFrame->m_CanData.m_u8Data[ i ];
    }

    // Mark as an extended frame
    SockFrame.can_id |= CAN_EFF_FLAG;
  
    // Send to the socket
    BytesWritten = write( CanSock, &SockFrame, sizeof( SockFrame ) );
    if ( BytesWritten < 0 )
    {
	// Only print a transmit error the first time
	if ( ucIsXmitWorking != FALSE )
	{
	    // This is where we get to if the cable is unplugged or there are
	    // no other devices on the network
	    perror( "Writing frame to socket" );
	    ucIsXmitWorking = FALSE;
	}

	// Sure, the frame was not sent, but reporting a failure will just
	// result in another failed retry.  Best to just tell a white lie and move on.
        return TFXCR_OK; // TFXCR_MSG_NOT_HANDLED;
    }

    if ( BytesWritten != sizeof( SockFrame ) )
    {
	// Only print a transmit error the first time
	if ( ucIsXmitWorking != FALSE )
	{
	    fprintf( stderr,
		     "Tried to send %d characters but actually wrote %d\n",
		     sizeof( SockFrame ),
		     BytesWritten );
	    ucIsXmitWorking = FALSE;
	}

        return TFXCR_OK; // TFXCR_MSG_NOT_HANDLED;
    }

    // Report transmit working again if it was broken
    if ( ucIsXmitWorking == FALSE )
    {
	fprintf( stderr, "CAN transmit working again\n" );
	ucIsXmitWorking = TRUE;
    }
    return TFXCR_OK;
}

//*****************************************************************************
// End of file.
//! @}
//*****************************************************************************
