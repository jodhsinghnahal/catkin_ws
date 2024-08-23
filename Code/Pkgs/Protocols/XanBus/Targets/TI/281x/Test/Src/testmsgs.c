/*****************************************************************************
Copyright 2005 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by 
copyright. The reproduction, in whole or in part, by anyone without the written 
approval of Xantrex is prohibited.

FILE NAME:
    testmsgs.c          - Message handlers for XanBus Test
 
PURPOSE:
    This file contains the interface to the message handlers for testing XanBus

FUNCTION(S):

    local:
    
NOTES:

CHANGE HISTORY:
$Log: testmsgs.c $

   ***********************************************
   Revision: XanBus_DaleM/2
   User: DaleM     Date: 03/12/05  Time: 01:46AM
   Add source flags to Want data.

    
******************************************************************************/

/*==============================================================================
                              Includes
==============================================================================*/

#include "testmsgs.h"       // Interface to message handlers for testing XanBus
#include "xbudefs.h"        // XanBus message definitions
#include "xbgdefs.h"        // XanBus general interface

/*==============================================================================
                              Defines                              
==============================================================================*/

/*==============================================================================
                            Type Definitions
==============================================================================*/

/*==============================================================================
                                Enums
==============================================================================*/

typedef enum
{
    eIDLE,
    eADDR_BUMP,
    eADDR_BUMPED,
    eSENT_DC_SRC_STS,
    eWAIT_DC_SRC_STS,
    eWAIT_DATE_TIME_STS,
    eFAILED,
    eSUCCESS
} teTEST_STATE;

/*=============================================================================
                              Structures
==============================================================================*/

/*==============================================================================
                           Local/Private Constants
==============================================================================*/

/*==============================================================================
                           Local/Private Variables
==============================================================================*/

static teTEST_STATE teTestMachine = eIDLE;
static uchar8 ucMyAddr;

/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/

static void testmsgs_fnNext( void );
static void testmsgs_fnFail( void );

/*==============================================================================
                          External/Public Variables
==============================================================================*/

/*==============================================================================
                           Function Definitions
==============================================================================*/

/*******************************************************************************

FUNCTION NAME: 
    TESTMSGS_fnStart        - Start test state machine
  
PURPOSE:
    Start the tests

INPUTS: 
    none

OUTPUTS:
    nothing

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/28/05  By: Dalem
    - Created

*******************************************************************************/

void TESTMSGS_fnStart( void )
{
#if 0
    // If test is done, allowe it to start over
    if( (  teTestMachine == eSUCCESS )
     || ( teTestMachine == eFAILED ) )
#endif
    {
        teTestMachine = eIDLE;
    }

    // Only start if in idle state
    if ( teTestMachine == eIDLE )
    {
        testmsgs_fnNext();
    }
}

/*******************************************************************************

FUNCTION NAME: 
    TESTMSGS_fnResult       - Check if test is finished
  
PURPOSE:
    Check if test if finished and return result of test

INPUTS: 
    none

OUTPUTS:
    returns 0 if test is not done yet
    returns 1 if test has fails
    returns 2 if test is finished and successful

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/28/05  By: Dalem
    - Created

*******************************************************************************/

uchar8 TESTMSGS_fnResult( void )
{
    // If our state machine shows done
    switch ( teTestMachine )
    {
        case eFAILED:
            return 1;
        case eSUCCESS:
            return 2;
        default:
            return 0;
    }
}

/*******************************************************************************

FUNCTION NAME: 
    testmsgs_fnNext         - Determine next test to run
  
PURPOSE:
    Determine the next test to run.

INPUTS: 
    none

OUTPUTS:
    none

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/28/05  By: Dalem
    - Created

*******************************************************************************/

static void testmsgs_fnNext( void )
{
    PGN_tzWANT_DATA tzWant;
    XB_tzPGN_CFG    tzCfg;

    // Action depends on state.
    switch ( teTestMachine )
    {
        case eIDLE:
        {
            // Save current address
            ucMyAddr = XBADDR_fnGetMyAddr();

            memset( &tzCfg, 0xFFFF, sizeof( tzCfg ) );

            // Set Id Period to 1 for this test
            tzCfg.uiIdPeriod = 1;

            // Send off cfg message
            XBMSG_fnSend( XB_PGN_CFG,
                          &tzCfg,
                          sizeof( tzCfg ),
                          PGN_ADDR_GLOBAL );

            // Next state wait for bump
            teTestMachine = eADDR_BUMP;
        }
        break;

        case eADDR_BUMP:
        {
            // Compare my current address with saved address
            if ( XBADDR_fnGetMyAddr() != ucMyAddr + 1 )
            {
                // Address did not change properly
                testmsgs_fnFail();
            }
            else
            {
                // Change state to bumped and do next state
                teTestMachine = eADDR_BUMPED;
                testmsgs_fnNext();
            }
        }
        break;

        case eADDR_BUMPED:
        {
            // Send first message
            // tzWant.tucSelf = 
            tzWant.tucSolicited = FALSE;
            tzWant.ucDestAddr = PGN_ADDR_GLOBAL;
            tzWant.ucSrcFlags = 0;
        
            // Trigger sending of DC Source Status message
            TESTMSGS_fnWantDcSrcSts( &tzWant );

            // Set state to first state
            teTestMachine = eSENT_DC_SRC_STS;
        }
        break;

        case eSENT_DC_SRC_STS:
            // Wait to receive DC Source Status
            teTestMachine = eWAIT_DC_SRC_STS;
            break;

        case eWAIT_DC_SRC_STS:
            // Set state to first state
            teTestMachine = eWAIT_DATE_TIME_STS;
            break;

        case eWAIT_DATE_TIME_STS:
            // Complete
            teTestMachine = eSUCCESS;
            break;

        default:
            testmsgs_fnFail();
            break;

    }
}

/*******************************************************************************

FUNCTION NAME: 
    testmsgs_fnFail         - Set test state to failed
  
PURPOSE:
    Set test state to failed

INPUTS: 
    none

OUTPUTS:
    none

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/28/05  By: Dalem
    - Created

*******************************************************************************/

static void testmsgs_fnFail( void )
{
    teTestMachine = eFAILED;
}

/*******************************************************************************

FUNCTION NAME: 
    TESTMSGS_fnRecvIsoAck   - Handle ISO Ack  PGN
  
PURPOSE:
    This function handles receiving an Acknowledge.

INPUTS: 
    'ptzRecv' is a pointer to the receive data 

OUTPUTS:
    nothing

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/28/05  By: Dalem
    - Created

*******************************************************************************/

void TESTMSGS_fnRecvIsoAck( PGN_tzRECV_DATA *ptzRecv )
{
    XB_tzPGN_ISO_ACK *ptzPgn;

    // Convert void structure to ISO Ack pgn
    ptzPgn = ptzRecv->pvStruct;

    if ( ptzPgn->teCtrlByte == XB_eACK_STS_ACK )
    {
        // Action depends on state.
        switch ( teTestMachine )
        {
            case eSENT_DC_SRC_STS:
            {
                if ( ptzPgn->ulParmGrpNum == XB_PGN_DC_SRC_STS )
                {
                    testmsgs_fnNext();
                }
            }
            break;

            default:
                testmsgs_fnFail();
                break;
                
        }

    }
    else
    {
        testmsgs_fnFail();
    }
}

/*******************************************************************************

FUNCTION NAME: 
    TESTMSGS_fnRecvResetCmd - Handle Reset command PGN
  
PURPOSE:
    This function handles receiving a Reset Command PGN.

INPUTS: 
    'ptzRecv' is a pointer to the receive data 

OUTPUTS:
    nothing

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/22/05  By: Dalem
    - Created

*******************************************************************************/

void TESTMSGS_fnRecvResetCmd( PGN_tzRECV_DATA *ptzRecv )
{
    XB_tzPGN_RESET_CMD *ptzPgn;

    // Assign void structure to my structure
    ptzPgn = ptzRecv->pvStruct;

    if ( ptzPgn->ucProcessorInst == 0 )
    {
        switch ( ptzPgn->teCmd )
        {
            case XB_eRST_CMD_OEM:
                // Change state to bumped and do next state
                teTestMachine = eADDR_BUMPED;
                testmsgs_fnNext();
                break;

            case XB_eRST_CMD_FACTORY:
            default:
                TESTMSGS_fnStart();
                break;
        }
    }
}

/*******************************************************************************

FUNCTION NAME: 
    TESTMSGS_fnRecvCfg      - Handle CFG PGN
  
PURPOSE:
    This function handles receiving a CFG PGN.

INPUTS: 
    'ptzRecv' is a pointer to the receive data 

OUTPUTS:
    nothing

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/28/05  By: Dalem
    - Created

*******************************************************************************/

void TESTMSGS_fnRecvCfg( PGN_tzRECV_DATA *ptzRecv )
{
    XB_tzPGN_CFG *ptzPgn;

    // Assign void structure to my structure
    ptzPgn = ptzRecv->pvStruct;

    if ( ptzPgn->uiIdPeriod == 1 )
    {
        testmsgs_fnNext();
    }
}

/*******************************************************************************

FUNCTION NAME: 
    TESTMSGS_fnRecvDcSrcSts - Handle DC Source Status PGN
  
PURPOSE:
    This function handles receiving a DC Source Status PGN.
    If the values are correct then send an ACK otherwise send a NAK.

INPUTS: 
    'ptzRecv' is a pointer to the receive data 

OUTPUTS:
    nothing

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/22/05  By: Dalem
    - Created

*******************************************************************************/

void TESTMSGS_fnRecvDcSrcSts( PGN_tzRECV_DATA *ptzRecv )
{
    XB_tzPGN_DC_SRC_STS *ptzPgn;

    // Assign void structure to my structure
    ptzPgn = ptzRecv->pvStruct;

    // If values are set to these
    if( ( ptzPgn->ucMsgCount == 0 )
     && ( ptzPgn->teDcSrcId == XB_eDC_SRC_ID_HOUSE_BAT_BANK6 )
     && ( ptzPgn->ulVoltage == 119456 )		// 119.456 V
     && ( ptzPgn->slCurrent == -12400 )		// 124.000 I
     && ( ptzPgn->teTempSensId == XB_eEXT_TEMP_SENS_IDS_BATTERY6 )
     && ( ptzPgn->uiTemp == 29300 )			// 293 K or 20 C
     && ( ptzPgn->ucSoc == 100 )			// 100 %
     && ( ptzPgn->uiTimeRemain == 160 )		// 160 minutes
     && ( ptzPgn->ucSoh == 96 )				// 95 %
     && ( ptzPgn->uiBattCapRemain == 120 )	// 125 AH
     && ( ptzPgn->ucBattCapPer == 70 )		// 75 %
     && ( ptzPgn->ulPwr == 1300 ) )			// 1300 W
    {
        // Send ACK
        XBMSG_fnSendIsoAck( XB_PGN_DC_SRC_STS,
                            XB_eGRP_FUNC_VALUE_DO_NOT_CARE,
                            XB_eACK_STS_ACK,
                            ptzRecv->ucSrcAddr );

        // If this fits my test states
        if ( teTestMachine == eWAIT_DC_SRC_STS )
        {
            // Move to next test
            testmsgs_fnNext();
        }
    }
    else
    {
        // Send NAK
        XBMSG_fnSendIsoAck( XB_PGN_DC_SRC_STS,
                            XB_eGRP_FUNC_VALUE_DATA_INVALID,
                            XB_eACK_STS_NAK,
                            ptzRecv->ucSrcAddr );
        // If this fits my test states
        if ( teTestMachine == eWAIT_DC_SRC_STS )
        {
            // Fail the test
            testmsgs_fnFail();
        }
    }
}

/*******************************************************************************

FUNCTION NAME: 
    TESTMSGS_fnWantDcSrcSts - Handle Request for a DC Source Status PGN
  
PURPOSE:
    This function handles a request for a DC Source Status PGN

INPUTS: 
    'ptzWant' is a pointer to the request data 

OUTPUTS:
    nothing

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/22/05  By: Dalem
    - Created

*******************************************************************************/

void TESTMSGS_fnWantDcSrcSts( PGN_tzWANT_DATA *ptzWant )
{
    XB_tzPGN_DC_SRC_STS tzPgn;
    uchar8 ucMC;

    memset( &tzPgn, 0xFFFF, sizeof( tzPgn ) );

    ucMC = ( ptzWant->tucSolicited == FALSE ? 0 : 1 );

    tzPgn.ucMsgCount = ucMC;                            // Message Count
    tzPgn.teDcSrcId = XB_eDC_SRC_ID_HOUSE_BAT_BANK5;    // DC Source Identifier
    tzPgn.ulVoltage = 0x12345678;                       // Voltage
    tzPgn.slCurrent = -24680;                           // Current
    tzPgn.teTempSensId = XB_eEXT_TEMP_SENS_IDS_BATTERY5; // Temp. Sensor Id
    tzPgn.uiTemp = 27300;                                 // Temperature
    tzPgn.ucSoc = 81;                                   // State of Charge
    tzPgn.uiTimeRemain = 129;                           // Time Remaining
    tzPgn.ucSoh = 95;                                   // State of Health
    tzPgn.uiBattCapRemain = 125;                        // Batt Capacity Remain.
    tzPgn.ucBattCapPer = 75;                            // Batt Cap. % of Max
    tzPgn.ulPwr = 369;                                  // Power

    XBMSG_fnSend( XB_PGN_DC_SRC_STS,
                  &tzPgn,
                  sizeof( tzPgn ),
                  ptzWant->ucDestAddr );
}

/*******************************************************************************

FUNCTION NAME: 
    TESTMSGS_fnRecvDcSrcSts - Handle DC Source Status PGN
  
PURPOSE:
    This function handles receiving a DC Source Status PGN.
    If the values are correct then send an ACK otherwise send a NAK.

INPUTS: 
    'ptzRecv' is a pointer to the receive data 

OUTPUTS:
    nothing

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/22/05  By: Dalem
    - Created

*******************************************************************************/

void TESTMSGS_fnRecvDateTimeSts( PGN_tzRECV_DATA *ptzRecv )
{
    XB_tzPGN_DATE_TIME_STS *ptzPgn;

    // Assign void structure to my structure
    ptzPgn = ptzRecv->pvStruct;

    // If values are set to these
    if( ( ptzPgn->ucMsgCount == 0 )
     && ( ptzPgn->ulUtcTime == 0x23456789 )
     && ( ptzPgn->teLocalDow == 5 )
     && ( ptzPgn->siLocalOffsetMins == 480 ) )
    {
        // Send ACK
        XBMSG_fnSendIsoAck( XB_PGN_DATE_TIME_STS,
                            XB_eGRP_FUNC_VALUE_DO_NOT_CARE,
                            XB_eACK_STS_ACK,
                            ptzRecv->ucSrcAddr );

        // If this fits my test states
        if ( teTestMachine == eWAIT_DATE_TIME_STS )
        {
            // Move to next test
            testmsgs_fnNext();
        }
    }
    else
    {
        // Send NAK
        XBMSG_fnSendIsoAck( XB_PGN_DATE_TIME_STS,
                            XB_eGRP_FUNC_VALUE_DATA_INVALID,
                            XB_eACK_STS_NAK,
                            ptzRecv->ucSrcAddr );

        // If this fits my test states
        if ( teTestMachine == eWAIT_DATE_TIME_STS )
        {
            // Fail the test
            testmsgs_fnFail();
        }
    }
}

