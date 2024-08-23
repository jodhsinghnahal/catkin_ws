/*==============================================================================
Copyright © 2009 Xantrex Technology Inc.

This file is the property of Xantrex Technology Inc. and shall not be
reproduced, copied, or used as the basis for the manufacture or sale of
equipment without the express written permission of Xantrex Technology Inc.

FILE NAME:  
    apoxdrv.h            - Interface to apoxdrv.c: Interface to Apox

PURPOSE:
    This file provides the external interface to apoxdrv.c which provides
    an interface to the Apox USB to CAN interface

FUNCTION(S):
    APOXDRV_fnInit         - Initialize the interface
    APOXDRV_fnClose        - Close the interface
    APOXDRV_fnStatus       - Get the status of the interface
    APOXDRV_fnReceiveFrame - Receive a CAN frame
    APOXDRV_fnSendFrame    - Send a CAN frame

NOTES:

CHANGE HISTORY:
$Log: apoxdrv.h $

    ***********************************************
    Revision: XanBus4_TomL/2
    User: TomL     Date: 01/20/10  Time: 09:55PM
    Matched case of include statements to files

    ***********************************************
    Revision: XanBus4_TomL/1
    User: TomL     Date: 01/16/10  Time: 02:00AM
    Corrected case of some include files

    ***********************************************
    Revision: XanBus3_TomL/2
    User: TomL     Date: 02/05/09  Time: 10:13PM
    Added file header
    
==============================================================================*/

#ifndef APOXDRV_H
#define APOXDRV_H

/*==============================================================================
                              Includes
==============================================================================*/

#include "LIB_stdtype.h"
#include "cdrive.h"
#include "libenv.h"

/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/

tucBOOL APOXDRV_fnInit( uchar8 ucBitRate );
void APOXDRV_fnClose( void );
TFXCAN_RETURNS APOXDRV_fnStatus( void );
TFXCAN_RETURNS APOXDRV_fnReceiveFrame( CANPORT u8PortNumber,
				     CANFRAME *pFrame );
TFXCAN_RETURNS APOXDRV_fnSendFrame( CANPORT u8PortNumber,
				  CANFRAME *pFrame,
				  INT16 *pi16MessageID );



#endif  // ifndef APOXDRV_H
