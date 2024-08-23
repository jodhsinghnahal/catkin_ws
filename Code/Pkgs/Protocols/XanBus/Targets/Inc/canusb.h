/*==============================================================================
Copyright © 2009 Xantrex Technology Inc.

This file is the property of Xantrex Technology Inc. and shall not be
reproduced, copied, or used as the basis for the manufacture or sale of
equipment without the express written permission of Xantrex Technology Inc.

FILE NAME:  
    canusb.h            - Interface to canusb.c: Interface to CANUSB

PURPOSE:
    This file provides the external interface to canusb.c which provides
    an interface to the CANUSB interface

FUNCTION(S):
    CANUSB_fnInit         - Initialize the interface
    CANUSB_fnClose        - Close the interface
    CANUSB_fnStatus       - Get the status of the interface
    CANUSB_fnReceiveFrame - Receive a CAN frame
    CANUSB_fnSendFrame    - Send a CAN frame

NOTES:

CHANGE HISTORY:
$Log: canusb.h $

    ***********************************************
    Revision: XanBus4_TomL/1
    User: TomL     Date: 01/20/10  Time: 10:22PM
    Matched case of include statements to files

    ***********************************************
    Revision: XanBus3_TomL/2
    User: TomL     Date: 02/05/09  Time: 10:13PM
    Added file header
    
==============================================================================*/

#ifndef CANUSB_H
#define CANUSB_H

/*==============================================================================
                              Includes
==============================================================================*/

#include "LIB_stdtype.h"
#include "cdrive.h"

/*==============================================================================
                              Defines
==============================================================================*/

// Bitmasks for status flags
#define CANUSB_STS_RXQ_FULL     0x01
#define CANUSB_STS_TXQ_FULL     0x02
#define CANUSB_STS_ERR_WARNING  0x04
#define CANUSB_STS_OVERRUN      0x08
#define CANUSB_STS_ERR_PASSIVE  0x20
#define CANUSB_STS_ERR_BUSOFF   0x40
#define CANUSB_STS_BUS_ERROR    0x80

/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/

tucBOOL CANUSB_fnInit( uchar8 ucBitRate );
void CANUSB_fnClose( void );
TFXCAN_RETURNS CANUSB_fnStatus( void );
TFXCAN_RETURNS CANUSB_fnReceiveFrame( CANPORT u8PortNumber,
				      CANFRAME *pFrame );
TFXCAN_RETURNS CANUSB_fnSendFrame( CANPORT u8PortNumber,
				   CANFRAME *pFrame,
				   INT16 *pi16MessageID );



#endif  // ifndef CANUSB_H
