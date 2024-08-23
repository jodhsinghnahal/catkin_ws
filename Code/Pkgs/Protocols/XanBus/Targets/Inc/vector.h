/*==============================================================================
Copyright © 2009 Xantrex Technology Inc.

This file is the property of Xantrex Technology Inc. and shall not be
reproduced, copied, or used as the basis for the manufacture or sale of
equipment without the express written permission of Xantrex Technology Inc.

FILE NAME:  
    vector.h            - Interface to vector.c: Interface for Vector CANCard

PURPOSE:
    This file provides the external interface to vector.c which provides
    an interface to the Vector CANCard interface

FUNCTION(S):
    VECTOR_fnInit         - Initialize the interface
    VECTOR_fnClose        - Close the interface
    VECTOR_fnStatus       - Get the status of the interface
    VECTOR_fnReceiveFrame - Receive a CAN frame
    VECTOR_fnSendFrame    - Send a CAN frame

NOTES:

CHANGE HISTORY:
$Log: vector.h $

    ***********************************************
    Revision: XanBus4_TomL/1
    User: TomL     Date: 01/20/10  Time: 10:26PM
    Matched case of include statements to files

    ***********************************************
    Revision: XanBus3_TomL/2
    User: TomL     Date: 02/05/09  Time: 10:13PM
    Added file header
    
==============================================================================*/

#ifndef VECTOR_H
#define VECTOR_H

/*==============================================================================
                              Includes
==============================================================================*/

#include "LIB_stdtype.h"
#include "cdrive.h"

/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/

tucBOOL VECTOR_fnInit( uchar8 ucBitRate );
void VECTOR_fnClose( void );
TFXCAN_RETURNS VECTOR_fnStatus( void );
TFXCAN_RETURNS VECTOR_fnReceiveFrame( CANPORT u8PortNumber,
				     CANFRAME *pFrame );
TFXCAN_RETURNS VECTOR_fnSendFrame( CANPORT u8PortNumber,
				  CANFRAME *pFrame,
				  INT16 *pi16MessageID );



#endif  // ifndef VECTOR_H
