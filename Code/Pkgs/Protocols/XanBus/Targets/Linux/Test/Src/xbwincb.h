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
$Log$

=============================================================================*/

#ifndef XBWINCB_H
#define XBWINCB_H


// Functions to install individual callback routines
void XBWIN_fnSetCheckErrorsCB( void (*fn) ( schar8 *pscTag ) );

void XBWIN_fnSetSetNetIndicCB( void (*fn) ( XB_teNETWORK_STATE teBusState,
                                                      tucBOOL tucOn ) );

void XBWIN_fnSetLoadCB( tuiSTATUS (*fn)( XB_tePARAM_TYPE teType,
                                                   XB_teCFG_TYPE teCfgType,
                                                   void *pData,
                                                   uint16 uiSize ) );

void XBWIN_fnSetSaveCB( tuiSTATUS (*fn)( XB_tePARAM_TYPE teType,
                                                   XB_teCFG_TYPE teCfgType,
                                                   void *pData,
                                                   uint16 uiSize ) );

void XBWIN_fnSetNodeChangeCB( void (*fn)( XB_teNODE_CHANGE teChange,
                                                    uchar8 ucOldAddr,
                                                    uchar8 ucNewAddr,
                                                    XB_tzPGN_ISO_ADDR_CLAIM *ptzNAME ) );

void XBWIN_fnSetRecvXbMsgCB( void (*fn)( PGN_tzRECV_DATA *ptzRecv ) );


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

// Set the identity for address claiming
void XBWIN_fnSetName( uchar8 DevInstLower, uchar8 DevInstUpper, XB_teFUNC_CODE DevFunc );

// Function installs all the callback routines
void XBWIN_fnInitCB( void );


#endif   // xbwincb_h
