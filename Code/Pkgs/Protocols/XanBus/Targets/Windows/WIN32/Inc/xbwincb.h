/*=============================================================================
Copyright © 2002 Xantrex Technology, Inc. All rights reserved.

This source file and the information contained in it are confidential and
proprietary to Xantrex Technology, Inc. The reproduction or disclosure,
in whole or in part, to anyone outside of Xantrex without the written
approval of a Xantrex officer under a Non-Disclosure Agreement, or to
any employee of Xantrex who has not previously obtained written
authorization for access from the individual responsible for the source
code, will have a significant detrimental effect on Xantrex and is expressly
prohibited.

FILE NAME:  xbwincb.h

PURPOSE:
   This file contains function prototypes for setting up callback functions
   for the XanBus Platform functions.

FUNCTION(S):
   None

NOTES:

HISTORY:
$Log: xbwincb.h $

    ***********************************************
    Revision: XanBus_HenryW/3
    User: HenryW     Date: 08/19/05  Time: 08:07PM
    - cleaned up history list.

    ***********************************************
    Revision: XanBus_HenryW/1
    User: HenryW     Date: 03/01/05  Time: 04:51PM
    - problem with ../xbwincb.h and file date.

    ***********************************************
    Revision: DiagWinEx_DaleM/2
    User: DaleM     Date: 01/14/05  Time: 08:54PM
    merged

    ***********************************************
    Revision: DiagWinEx_DaleM/1
    User: DaleM     Date: 01/12/05  Time: 09:55PM
    Multiport XanBus

    *****************  Version 13  *****************
    User: Peterd       Date: 14/05/04   Time: 3:37p
    Updated in $/PD/Common/Protocols/XanBus/Code/Windows/ComboCan/Test/DiagWinEx/Inc
    Updated fir new XBPLATFORM_fnNodeChange behaviour.

    *****************  Version 12  *****************
    User: Peterd       Date: 9/03/04    Time: 1:58p
    Updated in $/PD/Common/Protocols/XanBus/Code/Windows/ComboCan/Test/DiagWinEx/Inc
    Corrected prototype for CheckError function callback

    *****************  Version 11  *****************
    User: Peterd       Date: 4/02/04    Time: 3:37p
    Updated in $/PD/Common/Protocols/XanBus/Code/Windows/ComboCan/Test/DiagWinEx/Inc
    Updated bus state type based on changes to XanBusDB.mdb version 0.4.21

    *****************  Version 9  *****************
    User: Peterd       Date: 10/07/03   Time: 9:23a
    Updated in $/PD/Common/Protocols/XanBus/Code/Windows/CANCardX/Test/DiagWinEx/Inc
    Added set receive function callback prototype

    *****************  Version 8  *****************
    User: Peterd       Date: 8/19/03    Time: 1:15p
    Updated in $/PD/Common/Protocols/XanBus/Code/Windows/CANCardX/Test/DiagWinEx/Inc
    Added configuration type parameter to load and save functions.

    *****************  Version 7  *****************
    User: Dalem        Date: 7/09/03    Time: 2:52p
    Updated in $/PD/Common/Protocols/XanBus/Code/Windows/CANCardX/Test/DiagEx/Inc
    Updated for latest API changes - no SetIdentIndic, error handling
    changes

    *****************  Version 6  *****************
    User: Dalem        Date: 6/27/03    Time: 9:11a
    Updated in $/PD/Common/Protocols/XanBus/Code/Windows/CANCardX/Test/DiagEx/Inc
    Reflect major changes to XBAPI

    *****************  Version 5  *****************
    User: Dalem        Date: 6/03/03    Time: 4:45p
    Updated in $/PD/Common/Protocols/XanBus/Code/Windows/CANCardX/Test/DiagEx/Inc
    fnNodeChange replaces fnNewNode

    *****************  Version 4  *****************
    User: Dalem        Date: 5/12/03    Time: 11:04a
    Updated in $/PD/Common/Protocols/XanBus/Code/Windows/CANCardX/Test/DiagEx/Inc
    Brought uptodate with latest function declarations

    *****************  Version 3  *****************
    User: Dalem        Date: 4/17/03    Time: 8:21a
    Updated in $/PD/Common/Protocols/XanBus/Code/Windows/CANCardX/Test/DiagEx/Inc
    Changes to keep up with database

    *****************  Version 2  *****************
    User: Dalem        Date: 4/15/03    Time: 1:49p
    Updated in $/PD/Common/Protocols/XanBus/Code/Windows/CANCardX/Test/DiagEx/Inc
    Updated for changes in XB API

    *****************  Version 1  *****************
    User: Dalem        Date: 4/09/03    Time: 8:58a
    Created in $/PD/Common/Protocols/XanBus/Code/Windows/CANCardX/Test/DiagEx/Inc


=============================================================================*/

#ifndef XBWINCB_H
#define XBWINCB_H

#ifndef XBDLL_API
#ifdef XBDLL_EXPORTS
#define XBDLL_API __declspec(dllexport)
#else
#define XBDLL_API __declspec(dllimport)
#endif
#endif // ndef XBDLL_API

#ifdef __cplusplus
extern "C" {
#endif

// Functions to install individual callback routines
XBDLL_API void XBWIN_fnSetCheckErrorsCB( void (*fn) ( schar8 *pscTag ) );

XBDLL_API void XBWIN_fnSetSetNetIndicCB( void (*fn) ( XB_teNETWORK_STATE teBusState,
                                                      tucBOOL tucOn ) );

XBDLL_API void XBWIN_fnSetLoadCB( tuiSTATUS (*fn)( XB_tePARAM_TYPE teType,
                                                   XB_teCFG_TYPE teCfgType,
                                                   void *pData,
                                                   uint16 uiSize ) );

XBDLL_API void XBWIN_fnSetSaveCB( tuiSTATUS (*fn)( XB_tePARAM_TYPE teType,
                                                   XB_teCFG_TYPE teCfgType,
                                                   void *pData,
                                                   uint16 uiSize ) );

XBDLL_API void XBWIN_fnSetNodeChangeCB( void (*fn)( XB_teNODE_CHANGE teChange,
                                                    uchar8 ucOldAddr,
                                                    uchar8 ucNewAddr,
                                                    XB_tzPGN_ISO_ADDR_CLAIM *ptzNAME ) );

XBDLL_API void XBWIN_fnSetRecvXbMsgCB( void (*fn)( PGN_tzRECV_DATA *ptzRecv ) );

XBDLL_API void XBWIN_fnSetRecvGeneralMsgCB( void (*fn)( PGN_tzRECV_DATA *ptzRecv ) );

XBDLL_API tucBOOL XBWIN_fnSendArbitraryFrame( uint32 ulCanId,
                                              tucBOOL ucExtended,
                                              uchar8* pucData,
                                              uchar8 ucDataLen );
//#ifndef XBDLL_EXPORTS

// These functions are not seen by XBDLL

// Call back routines -- windows counterparts to functions in xbplatform.c
void XBWIN_fnCheckErrors( schar8 *pscTag );

void XBWIN_fnSetNetIndic( XB_teNETWORK_STATE teBusState,
                          tucBOOL tucOn );

void XBWIN_fnSetIdentIndic( uchar8 ucState );

tuiSTATUS XBWIN_fnLoad( XB_tePARAM_TYPE teType,
                        XB_teCFG_TYPE teCfgType,
                        void *pData,
                        uint16 uiSize );

tuiSTATUS XBWIN_fnSave( XB_tePARAM_TYPE teType,
                        XB_teCFG_TYPE teCfgType,
                        void *pData,
                        uint16 uiSize );

void XBWIN_fnNodeChange( XB_teNODE_CHANGE teChange,
                         uchar8 ucOldAddr,
                         uchar8 ucNewAddr,
                         XB_tzPGN_ISO_ADDR_CLAIM *ptzNAME );

void XBWIN_fnRecvXbMsg( PGN_tzRECV_DATA *ptzRecv );

void XBWIN_fnRecvGeneralMsg( PGN_tzRECV_DATA *ptzRecv );

void XBWIN_fnWantDiagMsg1( PGN_tzWANT_DATA *ptzWant );

// Function installs all the callback routines
void XBWIN_fnInitCB( void );

//#endif

#ifdef __cplusplus
}
#endif   // _cplusplus

#endif   // xbwincb_h
