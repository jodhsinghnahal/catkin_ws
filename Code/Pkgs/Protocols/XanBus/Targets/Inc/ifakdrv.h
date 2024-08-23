/*==============================================================================
Copyright © 2009 Xantrex Technology Inc.

This file is the property of Xantrex Technology Inc. and shall not be
reproduced, copied, or used as the basis for the manufacture or sale of
equipment without the express written permission of Xantrex Technology Inc.

FILE NAME:  
    ifakdrv.h            - Interface to ifakdrv.c: Interface to Ifak

PURPOSE:
    This file provides the external interface to ifakdrv.c which provides
    an interface to the Ifak USB to CAN interface

FUNCTION(S):
    IFAKDRV_fnInit         - Initialize the interface
    IFAKDRV_fnClose        - Close the interface
    IFAKDRV_fnStatus       - Get the status of the interface
    IFAKDRV_fnReceiveFrame - Receive a CAN frame
    IFAKDRV_fnSendFrame    - Send a CAN frame

NOTES:

CHANGE HISTORY:
$Log: ifakdrv.h $

    ***********************************************
    Revision: XanBus4_TomL/1
    User: TomL     Date: 01/20/10  Time: 10:24PM
    Matched case of include statements to files

    ***********************************************
    Revision: XanBus3_TomL/2
    User: TomL     Date: 02/05/09  Time: 10:13PM
    Added file header
    
==============================================================================*/

#ifndef IFAKDRV_H
#define IFAKDRV_H

/*==============================================================================
                              Includes
==============================================================================*/

#include "LIB_stdtype.h"
#include "cdrive.h"

/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/

tucBOOL IFAKDRV_fnInit( uchar8 ucBitRate );
void IFAKDRV_fnClose( void );
TFXCAN_RETURNS IFAKDRV_fnStatus( void );
TFXCAN_RETURNS IFAKDRV_fnReceiveFrame( CANPORT u8PortNumber,
				     CANFRAME *pFrame );
TFXCAN_RETURNS IFAKDRV_fnSendFrame( CANPORT u8PortNumber,
				  CANFRAME *pFrame,
				  INT16 *pi16MessageID );



#endif  // ifndef IFAKDRV_H
