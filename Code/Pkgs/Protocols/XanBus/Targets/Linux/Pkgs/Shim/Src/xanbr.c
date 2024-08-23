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
    This code works for Linux only

CHANGE HISTORY :
$Log: xanbr.c $

    ***********************************************
    Revision: XanBus4_TomL/1
    User: TomL     Date: 01/16/10  Time: 02:00AM
    Corrected case of some include files
==============================================================================*/

/*==============================================================================
                              Includes
==============================================================================*/


#include <stdio.h>          // For printf
#include "xanbr.h"

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


/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/


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
    return FALSE;
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
    return TFXCR_MSG_NOT_HANDLED;
}

TFXCAN_RETURNS XANBR_fnReceiveFrame( CANPORT u8PortNumber,
				     CANFRAME *pFrame )
{
    return TFXCR_NO_DATA;
}

// Close connection to XanBridge
void XANBR_fnClose( void )
{
}

// Status of XanBridge connection
TFXCAN_RETURNS XANBR_fnStatus( void )
{
    return TFXCR_OK;
}

