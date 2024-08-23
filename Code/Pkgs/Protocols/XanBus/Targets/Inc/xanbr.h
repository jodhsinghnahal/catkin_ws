/*==============================================================================
Copyright © 2009 Xantrex Technology Inc.

This file is the property of Xantrex Technology Inc. and shall not be
reproduced, copied, or used as the basis for the manufacture or sale of
equipment without the express written permission of Xantrex Technology Inc.

FILE NAME:  
    xanbr.h            - Interface to xanbr.c: TCP interface to XanBridge

PURPOSE:
    This file provides the external interface to xanbr.c which provides
    an interface to the XanBridge CAN to TCP/IP interface

FUNCTION(S):
    XANBR_fnInit         - Initialize the interface
    XANBR_fnClose        - Close the interface
    XANBR_fnStatus       - Get the status of the interface
    XANBR_fnReceiveFrame - Receive a CAN frame
    XANBR_fnSendFrame    - Send a CAN frame

NOTES:

CHANGE HISTORY:
$Log: xanbr.h $

    ***********************************************
    Revision: XanBus4_TomL/1
    User: TomL     Date: 01/20/10  Time: 10:26PM
    Matched case of include statements to files

    ***********************************************
    Revision: XanBus3_TomL/2
    User: TomL     Date: 02/05/09  Time: 10:13PM
    Added file header
    
==============================================================================*/

#ifndef XANBR_H
#define XANBR_H

/*==============================================================================
                              Includes
==============================================================================*/

#include "LIB_stdtype.h"
#include "cdrive.h"

/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/

tucBOOL XANBR_fnInit( void );
void XANBR_fnClose( void );
TFXCAN_RETURNS XANBR_fnStatus( void );
TFXCAN_RETURNS XANBR_fnReceiveFrame( CANPORT u8PortNumber,
				     CANFRAME *pFrame );
TFXCAN_RETURNS XANBR_fnSendFrame( CANPORT u8PortNumber,
				  CANFRAME *pFrame,
				  INT16 *pi16MessageID );



#endif  // ifndef XANBR_H
