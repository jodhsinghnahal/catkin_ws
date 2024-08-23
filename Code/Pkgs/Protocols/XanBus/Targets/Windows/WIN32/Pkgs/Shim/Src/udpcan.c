/******************************************************************************

  (c) 2020 Xantrex Canada ULC. All rights reserved.
  All trademarks are owned or licensed by Xantrex Canada ULC,
  its subsidiaries or affiliated companies.

****************************************************************************//**

  @addtogroup udpcan UDPCAN
  @{
  @brief  Interface to udpcan.c: virtual CAN over UDP

****************************************************************************//**

  @page udpcan_design CAN over UDP Virtual Interface (UDPCAN)

    This file provides the interface bewteen the XanBus shim and the
    virtual CAN over UDP interface

  @note
    This code works for Windows only as it uses the winsock2 library

*******************************************************************************/

/*==============================================================================
                              Includes
==============================================================================*/


#include <stdio.h>          // For printf
#include <string.h>
#include <winsock2.h>
#include "pgnudefs.h"
#include "udpcan.h"
#include "candrv.h"

/*==============================================================================
                              Defines
==============================================================================*/

#define CAN_UDP_BCST_ADDR       "192.168.0.255"
#define CAN_UDP_PORT            22594


/*==============================================================================
                           Local/Private Variables
==============================================================================*/

// UDP socket variables
static SOCKET gCanListenSocket = INVALID_SOCKET;
static SOCKET gCanSocket = INVALID_SOCKET;
static WSADATA wsaData;
static struct sockaddr_in gSend_addr;
static UINT8 ucMySrcAddr = PGN_ADDR_NULL;


/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/


/*==============================================================================
                           Function Definitions
==============================================================================*/


tucBOOL UDPCAN_fnInit( void )
{
    unsigned long nonblocking = 1;
    int err;
    
    // Initialize the socket variables
    gCanSocket = INVALID_SOCKET;
    gCanListenSocket = INVALID_SOCKET;
    
    // initialize windows sockets
    if( WSAStartup( MAKEWORD(2,2), &wsaData ) == 0 )
    {
        gCanListenSocket = socket( AF_INET, SOCK_DGRAM, 0 );
                
        if( gCanListenSocket != INVALID_SOCKET )
        {
            // make the socket non-blocking
            if( ioctlsocket( gCanListenSocket, FIONBIO, &nonblocking) != 0 )
            {
                printf("ioctlsocket failed\n");
                return FALSE;
            }
                    
            // This option is needed on the socket in order to be able to receive broadcast messages
            // If not set the receiver will not receive broadcast messages in the local network.
            char broadcast = '1';
            if ( setsockopt( gCanListenSocket,
                             SOL_SOCKET,
                             SO_BROADCAST,
                             &broadcast,
                             sizeof( broadcast )) < 0 )

            {
                closesocket( gCanListenSocket );
                printf("setsockopt SO_BROADCAST failed\n");
                return FALSE;
            }

            // More than one process on this machine may be running
            // CAN over UDP so we set the socket to non-exclusive use
            if ( setsockopt( gCanListenSocket,
                             SOL_SOCKET,
                             SO_REUSEADDR,
                             &broadcast,
                             sizeof( broadcast )) < 0 )

            {
                closesocket( gCanListenSocket );
                printf("setsockopt SO_REUSEADDR failed\n");
                return FALSE;
            }
            

            struct sockaddr_in Recv_addr;  
            struct sockaddr_in Sender_addr;
            int len = sizeof(struct sockaddr_in);
            char recvbuff[50];
            int recvbufflen = 50;

            Recv_addr.sin_family       = AF_INET;        
            Recv_addr.sin_port         = htons( CAN_UDP_PORT );   
            Recv_addr.sin_addr.s_addr  = INADDR_ANY;

            if ( bind( gCanListenSocket,
                       (struct sockaddr*)&Recv_addr,
                       sizeof( Recv_addr )) < 0)
            {
                err = WSAGetLastError();
                printf("bind failed with error %d\n", err);
                closesocket( gCanListenSocket );
                return FALSE;
            }

            // Now create the transmit socket
            gCanSocket = socket( AF_INET, SOCK_DGRAM, 0 );
            if( gCanSocket == INVALID_SOCKET )
            {
                // Could not create the transmit socket
                printf("Could not create the transmit socket\n");
                closesocket( gCanListenSocket );
                return FALSE;
            }
                
            gSend_addr.sin_family = AF_INET;
            gSend_addr.sin_port = htons( CAN_UDP_PORT);
            gSend_addr.sin_addr.s_addr = inet_addr( CAN_UDP_BCST_ADDR );
            
            // Indicate successful initialization
            printf("udpcan init success\n");
            return TRUE;
        }
        else
        {
            printf("Invalid receive socket created\n");
        }
    }
    else
    {
        printf("WSAStartup failed\n");
    }
    
    // indicate failure
    return FALSE;
}




TFXCAN_RETURNS UDPCAN_fnSendFrame( CANPORT u8PortNumber,
                                   CANFRAME *pFrame,
                                   INT16 *pi16MessageID )
{
    int retval;
    static UINT8 CanFrame[ 16 ];
    UINT16 checksum;
    int i;
    UINT8 *pData = (UINT8 *)pFrame;
    UINT8 cs0;
    UINT8 cs1;
    char *pCanFrame = (char *)CanFrame;
    int err;

    // send the CAN Frame to the remote client
    if (( gCanSocket != INVALID_SOCKET )
        && ( pFrame != NULL ))
    {
        // Catch the sender address so that we can distinguish
        // our broadcasts from everyone else's
        ucMySrcAddr = pFrame->m_CanData.m_u8SA;
        
        // populate the CAN frame and generate the checksum
        checksum = 0;
        
        for(i=0;i<14;i++)
        {
            checksum += pData[i];
            CanFrame[i] = pData[i];
        }
        
        // populate the checksum
        cs1 = ((checksum & 0xFF00) >> 8);
        cs0 = (checksum & 0x00FF);
        CanFrame[14] = cs1;
        CanFrame[15] = cs0;
        retval = sendto( gCanSocket,
                         pCanFrame,
                         16,
                         0,
                         (struct sockaddr*)&gSend_addr,
                         sizeof( gSend_addr ));
        
        if( retval == SOCKET_ERROR )
        {
            err = WSAGetLastError();
            
            switch(err)
            {
            case WSAEWOULDBLOCK:
                return TFXCR_DRV_BUSY;
                
            default:
                closesocket(gCanSocket);
                gCanSocket = INVALID_SOCKET;
            }
            
            return TFXCR_MSG_NOT_HANDLED;
        }
        
        // CAN frame sent successfully
        return TFXCR_OK;
    }
    
    return TFXCR_MSG_NOT_HANDLED;
}

TFXCAN_RETURNS UDPCAN_fnReceiveFrame( CANPORT u8PortNumber,
                                     CANFRAME *pFrame )
{
    int retval;
    static char FrameBuf[ sizeof(CANDATA)+2 ];
    int err;
    CANDATA *pCanData = (CANDATA *)FrameBuf;
    
    if( gCanListenSocket != INVALID_SOCKET )
    {
        // try to receive data on the CAN socket
        retval = recvfrom( gCanListenSocket,
                           FrameBuf,
                           sizeof( FrameBuf ),
                           0,
                           NULL,
                           NULL );
        
        if ( retval >= (int)(sizeof( CANDATA )) )
        {
            // TODO: Should verify that checksum is correct
            
            // Make sure this isn't just my own broadcast
            if ( pCanData->m_u8SA != ucMySrcAddr )
            {
                memcpy(pFrame, FrameBuf, sizeof(CANDATA));
                return TFXCR_NEW_DATA;
            }
        }
    }

    return TFXCR_NO_DATA;
}


// Close connection
void UDPCAN_fnClose( void )
{
    if ( gCanListenSocket != INVALID_SOCKET )
    {
        closesocket( gCanListenSocket );
        gCanListenSocket = INVALID_SOCKET;
    }
    
    if ( gCanSocket != INVALID_SOCKET )
    {
        closesocket( gCanSocket );
        gCanSocket = INVALID_SOCKET;
    }

    ucMySrcAddr = PGN_ADDR_NULL;
}

// Status of CAN connection
TFXCAN_RETURNS UDPCAN_fnStatus( void )
{
    return TFXCR_OK;
}

//*****************************************************************************
// End of file.
//! @}
//*****************************************************************************
