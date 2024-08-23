/*=============================================================================
Copyright 2005 Xantrex Technology Inc.  All rights reserved.

This source file is proprietary to Xantrex Technology Inc. and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:  testmsgs.h

PURPOSE:
    This header defines the interface to the test message handlers for the
    XanBus communcations stack for the TI processor.

FUNCTION(S):

NOTES:

HISTORY:
$Log: $

   
=============================================================================*/

#ifndef TESTMSGS_H
#define TESTMSGS_H

/*==============================================================================
                              Includes
==============================================================================*/

#include "pgnudefs.h"       // Universal definitions for Parameter Groups

/*=============================================================================
                                    Defines
=============================================================================*/


/*=============================================================================
                                   Data Types
=============================================================================*/


/*=============================================================================
                                   Prototypes
=============================================================================*/

void TESTMSGS_fnStart( void );
uchar8 TESTMSGS_fnResult( void );
void TESTMSGS_fnRecvIsoAck( PGN_tzRECV_DATA *ptzRecv );
void TESTMSGS_fnRecvResetCmd( PGN_tzRECV_DATA *ptzRecv );
void TESTMSGS_fnRecvCfg( PGN_tzRECV_DATA *ptzRecv );
void TESTMSGS_fnRecvDcSrcSts( PGN_tzRECV_DATA *ptzRecv );
void TESTMSGS_fnWantDcSrcSts( PGN_tzWANT_DATA *ptzWant );
void TESTMSGS_fnRecvDateTimeSts( PGN_tzRECV_DATA *ptzRecv );


#endif // TESTMSGS_H
