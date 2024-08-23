/*==============================================================================
Copyright 2003 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by 
copyright. The reproduction, in whole or in part, by anyone without the written 
approval of Xantrex is prohibited.

FILE NAME:  
    xbpgfns.h

PURPOSE:
   This file provides some function prototypes for PGN handlers that
    are handled exclusively by XanBus.

FUNCTION(S):
    XBADDR_fnWantClaim        - Send an address claim message
    XBADDR_fnRecvClaim        - Handler for the ISO Address Claim message
    XBADDR_fnRecvCmdAddr      - Handle Commanded Address message
    XBADDR_fnRecvNAMECfg      - Handle NAME Configuration message
    XBADDR_fnWantNAMECfg      - Send NAME Config message
    XBMSG_fnRecvISOReq        - Handle Received ISO Request message
    XBMSG_fnWantPgnList       - Send NMEA Rx and Tx PGN lists
    XBMSG_fnWantCommStats     - Send communications statistics PGN
    XBTIMERS_fnRecvPerXmitCfg - Receive Period Transmit Configuration
    XBTIMERS_fnWantPerXmitCfg - Send Periodic Transmit Configuration
    XBTIMERS_fnRecvRxToCfg    - Receive an Rx Timeout Configuration
    XBTIMERS_fnWantRxToCfg    - Send Receive Timeout Configuration
    
    local:

NOTES:
    
    
CHANGE HISTORY:
$Log: xbpgfns.h $

    ***********************************************
    Revision: XanBus_HollyZ/4
    User: HollyZ     Date: 05/25/06  Time: 07:12PM
    Revert Xb stack to V1.2.7

    ***********************************************
    Revision: XanBus_HenryW/2
    User: HenryW     Date: 08/19/05  Time: 08:07PM
    - cleaned up history list.

    ***********************************************
    Revision: XanBus_HollyZ/3
    User: HollyZ     Date: 06/08/05  Time: 10:57PM
    function prototype changed for XBMSG_fnCheckRx() and XBMSG_fnRouteRx()
    
    ***********************************************
    Revision: XanBus_HollyZ/2
    User: HollyZ     Date: 06/07/05  Time: 07:08PM
    Added XBMSG_fnCheckRx() and XBMSG_fnRouteRx()

    ***********************************************
    Revision: XanBus_HollyZ/1
    User: HollyZ     Date: 06/03/05  Time: 10:37PM
    Removed XBMSG_fnRawReceive()

    ***********************************************
    Revision: GateWayPfrmANMEA_DaleM/1
    User: DaleM     Date: 02/10/05  Time: 07:20PM
    Cleaned up old history list
    

    ***********************************************
    Revision: XanBus_DaleM/1
    User: DaleM     Date: 01/12/05  Time: 07:18PM
    Multiport XanBus Library
    
    
==============================================================================*/
#ifndef XBPGFNS_INCL
#define XBPGFNS_INCL

#include "pgnudefs.h"

/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/

#ifndef XBDLL_API
#ifdef WIN32
#ifdef XBDLL_EXPORTS
#define XBDLL_API __declspec(dllexport)
#else
#define XBDLL_API __declspec(dllimport)
#endif
#else // !win32
#define XBDLL_API extern
#endif
#endif // ndef XBDLL_API

#ifdef __cplusplus
extern "C" {
#endif


// From xbaddr.c
XBDLL_API void XBADDR_fnWantClaim( PGN_tzWANT_DATA *ptzWant );

XBDLL_API void XBADDR_fnRecvClaim( PGN_tzRECV_DATA *ptzRecv );

XBDLL_API void XBADDR_fnRecvCmdAddr( PGN_tzRECV_DATA *ptzRecv );

XBDLL_API void XBADDR_fnRecvNAMECfg( PGN_tzRECV_DATA *ptzRecv );

XBDLL_API void XBADDR_fnWantNAMECfg( PGN_tzWANT_DATA *ptzWant );

XBDLL_API void XBADDR_fnRecvDiagMsg1( PGN_tzRECV_DATA *ptzRecv );

// From xbinit.c

// From xbltmpkt.c

// From xbmsg.c
XBDLL_API void XBMSG_fnRecvISOReq( PGN_tzRECV_DATA *ptzRecv );

XBDLL_API void XBMSG_fnWantPgnList( PGN_tzWANT_DATA *ptzWant );

XBDLL_API void XBMSG_fnRecvConnMgmt( PGN_tzRECV_DATA *ptzRecv );

XBDLL_API void XBMSG_fnWantCommStats( PGN_tzWANT_DATA *ptzWant );

// From xbpack.c

// From xbplatform.c

// From xbtfx.c

// From xbtimers.c
XBDLL_API void XBTIMERS_fnRecvPerXmitCfg( PGN_tzRECV_DATA *ptzRecv );

XBDLL_API void XBTIMERS_fnWantPerXmitCfg( PGN_tzWANT_DATA *ptzWant );

XBDLL_API void XBTIMERS_fnRecvRxToCfg( PGN_tzRECV_DATA *ptzRecv );

XBDLL_API void XBTIMERS_fnWantRxToCfg( PGN_tzWANT_DATA *ptzWant );

#ifdef __cplusplus
}
#endif   // _cplusplus

#endif
