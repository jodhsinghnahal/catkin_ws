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
    #$-VIEW:C Coding Standard-$#
    **If you do not see a button above this, either use CodeWright or ignore 
    **the text.
    
    There is also no use or assumption of any operating system services.

HISTORY:
$Log: CDrvAPI.h $

    ***********************************************
    Revision: XanBus_HollyZ/2
    User: HollyZ     Date: 06/17/05  Time: 05:44PM
    Changed function prototype

    ***********************************************
    Revision: XanBus_HollyZ/1
    User: HollyZ     Date: 06/17/05  Time: 04:39PM
    Added new parameter BitRate to DRVAPI_i16InitController()

    ***********************************************
    Revision: XanBus_DaleM/1
    User: DaleM     Date: 01/12/05  Time: 07:19PM
    Multiport XanBus Library
    
    
    *****************  Version 32  *****************
    User: Dalem        Date: 10/12/04   Time: 2:17p
    Updated in $/PD/Network_Extensions/Code/Gateway/CAN/N2K/XB/Inc
    Added check for transmitter being blocked
    
    *****************  Version 31  *****************
    User: Dalem        Date: 10/08/04   Time: 3:33p
    Updated in $/PD/Network_Extensions/Code/Gateway/CAN/N2K/XB/Inc
    Added time filtering to CAN tx status
    
    *****************  Version 30  *****************
    User: Dalem        Date: 10/01/04   Time: 6:15p
    Updated in $/PD/Network_Extensions/Code/Gateway/CAN/N2K/XB/Inc
    Changes for initial Gateway development
    
    *****************  Version 10  *****************
    User: Toml         Date: 12/16/03   Time: 11:41a
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/UIEx/XB/Inc
    Updated copyright notice
    
    *****************  Version 7  *****************
    User: Dalem        Date: 11/06/03   Time: 7:48a
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/AGSEx/XB/Inc
    Removed unimplemented frame pacing variables
    
    *****************  Version 6  *****************
    User: Dalem        Date: 9/17/03    Time: 3:28p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/AGSEx/XB/Inc
    Changed communications statistics
    
    *****************  Version 5  *****************
    User: Dalem        Date: 7/18/03    Time: 1:34p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/AGSEx/XB/Inc
    Interface names changes for candrv.c
    
    *****************  Version 4  *****************
    User: Toml         Date: 6/26/03    Time: 2:45p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/BattMonEx/XB/Inc
    New API
    
    *****************  Version 5  *****************
    User: Dalem        Date: 6/25/03    Time: 5:13p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DP256B/Test/BattMonEx/XB/Inc
    Changed structure name to CAN_tzCOMM_STATS
    
    *****************  Version 3  *****************
    User: Dalem        Date: 3/04/03    Time: 9:50a
    Updated in $/PD/Common/XanBus/Code/Common/CAN Driver/HCS12
    Added CAN communications statistics
    
    *****************  Version 2  *****************
    User: Toml         Date: 2/20/03    Time: 9:11p
    Updated in $/PD/Common/XanBus/Code/Common/CAN Driver/HCS12
    mscan.c replaced with cdrvapi.c and candrv.c
    
    
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

