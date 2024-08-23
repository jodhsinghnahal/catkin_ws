/*==============================================================================
Copyright © 2009 Xantrex Technology, Inc. All rights reserved.

This source file and the information contained in it are confidential and 
proprietary to Xantrex Technology, Inc. The reproduction or disclosure, in 
whole or in part, to anyone outside of Xantrex without the written approval of 
a Xantrex officer under a Non-Disclosure Agreement, or to any employee of 
Xantrex who has not previously obtained written authorization for access from 
the individual responsible for the source code, will have a significant 
detrimental effect on Xantrex and is expressly prohibited.

FILE NAME:  xanbr.c

PURPOSE:
    This file provides the interface bewteen the XanBus shim and the TCP
    interface with the XanBridge board.

FUNCTION(S):
    local:
   
NOTES:
    This code works for Windows only

CHANGE HISTORY :
$Log: CDrive.c $
==============================================================================*/

/*==============================================================================
                              Includes
==============================================================================*/


#include <stdio.h>          // For printf
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "xanbr.h"
#include "candrv.h"

/*==============================================================================
                              Defines
==============================================================================*/

#define CAN_TCP_TARGET			"localhost"
#define CAN_TCP_PORT			"22594"
#define CAN_TCP_MODE            "CLIENT"

#define MAX_TCP_MODE_LEN		10
#define MAX_TCP_TARGET_LEN		128
#define MAX_TCP_PORT_LEN        6

#define TCP_MODE_SERVER			0
#define TCP_MODE_CLIENT			1

/*==============================================================================
                           Local/Private Variables
==============================================================================*/

// TCP socket variables
SOCKET gCanListenSocket = INVALID_SOCKET;
SOCKET gCanSocket = INVALID_SOCKET;
WSADATA wsaData;
static DWORD tcp_mode;
static char tcp_target[ MAX_TCP_TARGET_LEN ];
static char tcp_port[ MAX_TCP_PORT_LEN ];

FILE *dbgfp=NULL;

/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/

static void ReadTCPSettings( void );
static SOCKET MakeTcpConnection( void );

/*==============================================================================
                           Function Definitions
==============================================================================*/


/*******************************************************************************

FUNCTION NAME:
    XANBR_fnInit

PURPOSE:
    Create a TCP server for receiving CAN frames

INPUTS:
    Channel is the number of the CAN module to put to wakeup, usually zero.

OUTPUTS:
    1 = TCP Virtual CAN port created
	0 = TCP Virtual CAN port could not be created

NOTES:

VERSION HISTORY:

Version: 1.00  Date: Apr-13-2007  By: Trevor Monk
    - Creation

*******************************************************************************/
tucBOOL XANBR_fnInit( void )
{
    ADDRINFO Hints;
    struct addrinfo *pAddrInfo;
    char *port = tcp_port;
    unsigned long nonblocking = 1;
    
    // read the TCP settings from the registry
    ReadTCPSettings();
    
    // Initialize the socket variables
    gCanSocket = INVALID_SOCKET;
    gCanListenSocket = INVALID_SOCKET;
    
    // initialize windows sockets
    if(WSAStartup(MAKEWORD(2,2), &wsaData) == 0)
    {
	if( tcp_mode == TCP_MODE_SERVER )
	{
	    // set up a Hints structure to provide to the getaddrinfo function
	    memset(&Hints, 0, sizeof(Hints));
	    Hints.ai_family = AF_INET;
	    Hints.ai_socktype = SOCK_STREAM;
	    Hints.ai_flags = AI_PASSIVE;
	    
	    // get the local machine's IP address
	    if( getaddrinfo(NULL, port, &Hints, &pAddrInfo) == 0 )
	    {
		// create a TCP socket to listen on
		gCanListenSocket = socket(pAddrInfo->ai_family,pAddrInfo->ai_socktype, pAddrInfo->ai_protocol);
		
		if( gCanListenSocket != INVALID_SOCKET )
		{
		    // make the socket non-blocking
		    if( ioctlsocket(gCanListenSocket, FIONBIO, &nonblocking) != 0 )
		    {
			return FALSE;
		    }
		    
		    // bind the socket to the CAN TCP port
		    if( bind(gCanListenSocket, pAddrInfo->ai_addr, pAddrInfo->ai_addrlen) != SOCKET_ERROR )
		    {
			// set up the socket to listen for incoming connection requests
			if( listen( gCanListenSocket, 1) != SOCKET_ERROR)
			{
			    // indicate success
			    return TRUE;
			}
		    }
		}
		
		// close the socket
		closesocket(gCanListenSocket);
	    }
	}
	else
	{
	    // no further action for the client
	    return TRUE;
	}
    }
    
    // indicate failure
    return FALSE;
}

static void ReadTCPSettings( void )
{
	char tcp_mode_str[ MAX_TCP_MODE_LEN ];
	char szSubKey[] = "SOFTWARE\\Xantrex\\Xanbus";
	HKEY hKey;
	DWORD disposition;
	DWORD len;

	// set up the default settings
	strcpy(tcp_port, CAN_TCP_PORT);
	strcpy(tcp_target, CAN_TCP_TARGET);
	strcpy(tcp_mode_str, CAN_TCP_MODE);


	// open or create the subkey
	if( RegCreateKeyEx( HKEY_LOCAL_MACHINE, szSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, 
		                KEY_SET_VALUE | KEY_QUERY_VALUE, NULL, &hKey, &disposition ) == ERROR_SUCCESS )
	{
		if( disposition == REG_CREATED_NEW_KEY )
		{
			// write the default values into the registry
			RegSetValueEx(hKey, "MODE", 0, REG_SZ, (LPBYTE)tcp_mode_str, strlen(tcp_mode_str) );
			RegSetValueEx(hKey, "TARGET", 0, REG_SZ, (LPBYTE)tcp_target, strlen(tcp_target) );
			RegSetValueEx(hKey, "PORT", 0, REG_SZ, (LPBYTE)tcp_port, strlen(tcp_port) );
		}
		else if( disposition == REG_OPENED_EXISTING_KEY )
		{
			len = MAX_TCP_MODE_LEN;
			RegQueryValueEx(hKey, "MODE", NULL, NULL, (LPBYTE)tcp_mode_str, &len );
			len = MAX_TCP_TARGET_LEN;
			RegQueryValueEx(hKey, "TARGET", NULL, NULL, (LPBYTE)tcp_target, &len );
			len = MAX_TCP_PORT_LEN;
			RegQueryValueEx(hKey, "PORT", NULL, NULL, (LPBYTE)tcp_port, &len );
		}
	}
	RegCloseKey( hKey );

	if( toupper(tcp_mode_str[0]) == 'S' )
	{
		tcp_mode = TCP_MODE_SERVER;
	}
	else
	{
		tcp_mode = TCP_MODE_CLIENT;
	}
}



/*******************************************************************************

FUNCTION NAME: 
    XANBR_fnSendFrame
  
PURPOSE: 
    Transmit the specified CAN frame over TCP

INPUTS: 
    'u8PortNumber' presently unused, but included for consistency with all other "SendFrame" functions
    'pFrame' is a pointer to the CAN data be be sent
    'pi16MessageID' presently unused, but included for consistency with all other "SendFrame" functions

OUTPUTS:
    returns TFXCR_INVALID_PORT if port is invalid
    returns TFXCR_MSG_NOT_HANDLED if the TCP port is not open, or if the message transmission failed
    returns TFXCR_OK if message was sent ok

NOTES:

VERSION HISTORY:

Version: 1.00  Date: APR-20-2007  By: Trevor Monk
    - Created

*******************************************************************************/
TFXCAN_RETURNS XANBR_fnSendFrame( CANPORT u8PortNumber,
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
    if( gCanSocket != INVALID_SOCKET )
    {
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
	
	retval = send(gCanSocket, pCanFrame, 16, 0 );
	if( dbgfp != NULL && retval != 16)
	{
	    fprintf(dbgfp, "XANBR_fnSendFrame result = %d\r\n", retval);
	}
	
	if( retval == SOCKET_ERROR )
	{
	    err = WSAGetLastError();
	    if( dbgfp != NULL )
	    {
		fprintf(dbgfp, "XANBR_fnSendFrame error %d\r\n", err);
	    }
	    
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

TFXCAN_RETURNS XANBR_fnReceiveFrame( CANPORT u8PortNumber,
				     CANFRAME *pFrame )
{
    int retval;
    static char FrameBuf[ sizeof(CANDATA)+2 ];
    int len = sizeof(CANDATA)+2;
    static int remaining = sizeof(CANDATA)+2;
    int err;
    
    if( gCanSocket != INVALID_SOCKET )
    {
	// try to receive data on the CAN socket
	retval = recv( gCanSocket, &FrameBuf[len-remaining], remaining, 0 );
	if( retval == 0 )
	{
	    goto socket_reset;
	}
	else if( retval == SOCKET_ERROR )
	{
	    err = WSAGetLastError();
	    switch( err )
	    {
	    case WSAEWOULDBLOCK:
		// no data available on the socket yet
		return TFXCR_NO_DATA;
		
	    default:
		goto socket_reset;
	    }
	}
	else
	{
	    remaining -= retval;
	}
	
	// copy the received data into the CAN frame
	if( remaining == 0 )
	{
	    memcpy(pFrame, FrameBuf, sizeof(CANDATA));
	    remaining = len;
	    return TFXCR_NEW_DATA;
	}
	else
	{
	    // haven't received a full frame yet
	    return TFXCR_NO_DATA;
	}
    }
    else
    {
	// we have not received a valid connection yet.
	// try to accept an incoming connection
	gCanSocket = MakeTcpConnection();
	
	return TFXCR_NO_DATA;
    }
    
socket_reset:
    closesocket( gCanSocket );
    gCanSocket = INVALID_SOCKET;
    return TFXCR_NO_DATA;
}

static SOCKET MakeTcpConnection( void )
{
    SOCKET xanbus_socket = INVALID_SOCKET;
    ADDRINFO Hints;
    struct addrinfo *pAddrInfo;
    unsigned long nonblocking = 1;
    
    switch( tcp_mode )
    {
    case TCP_MODE_SERVER:
	if( gCanListenSocket != INVALID_SOCKET )
	{
	    xanbus_socket = accept(gCanListenSocket, NULL, NULL );
	    if( xanbus_socket != INVALID_SOCKET )
	    {
		// we have received a connection!!!
		// make the socket non-blocking
		if( ioctlsocket(xanbus_socket, FIONBIO, &nonblocking) == SOCKET_ERROR )
		{
		    closesocket( xanbus_socket );
		    return INVALID_SOCKET;
		}
	    }
	}
	break;
	
    case TCP_MODE_CLIENT:
	xanbus_socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	
	// set up a Hints structure to provide to the getaddrinfo function
	memset(&Hints, 0, sizeof(Hints));
	Hints.ai_family = AF_INET;
	Hints.ai_socktype = SOCK_STREAM;
	Hints.ai_flags = 0;
	
	// get the local machine's IP address
	if( getaddrinfo(tcp_target, tcp_port, &Hints, &pAddrInfo) == 0 )
	{
	    if( connect(xanbus_socket, pAddrInfo->ai_addr, pAddrInfo->ai_addrlen ) != 0 )
	    {
		return INVALID_SOCKET;
	    }
	    
	    // make the socket non-blocking
	    if( ioctlsocket(xanbus_socket, FIONBIO, &nonblocking) == SOCKET_ERROR )
	    {
		closesocket( xanbus_socket );
		return INVALID_SOCKET;
	    }
	}
	else
	{
	    return INVALID_SOCKET;
	}
	break;
	
    default:
	break;
    }
    
    return xanbus_socket;
}


// Close connection to XanBridge
void XANBR_fnClose( void )
{
    if( gCanListenSocket != INVALID_SOCKET )
    {
	closesocket(gCanListenSocket);
    }
    
    if( gCanSocket != INVALID_SOCKET )
    {
	closesocket(gCanSocket);
    }
}

// Status of XanBridge connection
TFXCAN_RETURNS XANBR_fnStatus( void )
{
    return TFXCR_OK;
}

