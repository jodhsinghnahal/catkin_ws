/*=============================================================================
Copyright 2003 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by 
copyright. The reproduction, in whole or in part, by anyone without the written 
approval of Xantrex is prohibited.

FILE NAME:  CDrvAPI.h

PURPOSE:
    This header defines the API for the XanBus CAN driver which is the
    intermediate layer between the hardware HAL driver and the rest of the
    driver and stack.

NOTES:
    
    None

HISTORY:
$Log: CDrvAPI.h $
    
    
=============================================================================*/

#ifndef CDRVAPI_H
#define CDRVAPI_H

#include "canbuf.h"         // Interface to CAN frame buffering

/*=============================================================================
                                    Defines
=============================================================================*/

/*=============================================================================
                                   Data Types
=============================================================================*/

/*=============================================================================
                                   Prototypes
=============================================================================*/

INT16 DRVAPI_i16InitController( CANPORT Channel,
                                uchar8 teBitRate );
                                
BOOL DRVAPI_bOK_to_send( CANPORT Channel );

INT16 DRVAPI_iXmit_object( CANPORT Channel,
                           CANBUF_tzTX_BUF *ptzTxBuf );
uchar8 DRVAPI_fnBusState( CANPORT Channel );
void DRVAPI_fnStatusTick( CANPORT Channel );
tucBOOL DRVAPI_fnIsBlocked( CANPORT Channel );
void DRVAPI_fnCountTxLost( uchar8 Channel );
uint16 DRVAPI_fnGetCommStats( uchar8 Channel, CAN_tzCOMM_STATS *ptzStats );
uint16 DRVAPI_fnClearCommStats( uchar8 Channel );

void DRVAPI_vDisable_int( CANPORT Channel );
void DRVAPI_vEnable_int( CANPORT Channel );


void DRVAPI_fnTxTimeOut( CANPORT Channel );
void DRVAPI_fnBufferToCanData( CANDRV_tzCAN_FRAME *pzFrame,
                               CANDATA *pzCanData );
void DRVAPI_fnCanDataToBuffer( CANDATA *pzCanData,
                               CANDRV_tzCAN_FRAME *pzFrame );

#endif

