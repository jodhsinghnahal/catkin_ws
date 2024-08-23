/*=============================================================================
  (c) 2002 - 2019 Xantrex Canada ULC. All rights reserved.
  All trademarks are owned or licensed by Xantrex Canada ULC,
  its subsidiaries or affiliated companies.

FILE NAME:  xbtaddr.c

PURPOSE:
    This file contains routines for maintaining node names and addresses.
    It has two basic purposes: first, claim and defend this nodes network
    address, second, to keep a list of active nodes name/address pairs.

FUNCTION(S):
    XBTADDR_fnTick          - Tick the XBT timers
    XBTADDR_fnIsDiag        - Is given address from a diagnostics node       
    XBTADDR_fnSendClaim     - Send an address claim message
    XBTADDR_fnGetAddr       - Get current address
    XBTADDR_fnOnLine        - Is this node allowed to send messages
    XBTADDR_fnBeginClaiming - Initialize and start address claim sequence
    XBTADDR_fnGetMyFunction - Determine my device function
    XBTADDR_fnRecvClaim     - Handler for the ISO Address Claim message

    local:
        xbtaddr_fnRandom        - Protocol requires a random time delay
        xbtaddr_fnStartProTimer - Start the protocol timer
        xbtaddr_fnStopProTImer  - Stop the protocol timer
        xbtaddr_fnCompareNames  - Compare node names for priority
        xbtaddr_fnState2        - Perform NMEA state 2 processing
        xbtaddr_fnState4        - Perform NMEA state 4 processing
        xbtaddr_fnState6        - Perform NMEA state 6 processing
        xbtaddr_fnState7        - Perform NMEA state 7 processing
        xbtaddr_fnState9        - Perform NMEA state 9 processing
        xbtaddr_fnRandomTimer   - Handler for Random Start Timer event
        xbtaddr_fnClaimTimer    - Handler for the Claim Timer event
        xbtaddr_fnChallenge     - Determine outcome of Address Name Challenge

    
=============================================================================*/

/*=============================================================================
                                 Include files
=============================================================================*/

#include <string.h>		    // for memcmp

#include "xbudefs.h"
#include "xbtgdefs.h"
#include "xbtldefs.h"
#include "config.h"

/*==============================================================================
                           Local/Private Constants
==============================================================================*/

// Time in milliseconds that node must wait to complete claim
#define XBTA_CLAIM_TIMEOUT      ( 250 )

/*
    Timeouts in milliseconds for messages that are broadcast periodically
*/
#define XBTA_LOADSTATUS_TIMEOUT ( 1000 )  // Time between load status messages
#define XBTA_KEEPALIVE_TIMEOUT  ( 1000 )  // Time between address claim messages

/*=============================================================================
                              Internal Definitions
=============================================================================*/

/*
    These are the NMEA Claiming States as defined in NMEA 2000, Appendix D.
 */
typedef enum eNMEA_STATE
{
    XBT_eNMEA_STATE0,
    XBT_eNMEA_STATE1,
    XBT_eNMEA_STATE2,
    XBT_eNMEA_STATE3,
    XBT_eNMEA_STATE4,
    XBT_eNMEA_STATE5,
    XBT_eNMEA_STATE6,
    XBT_eNMEA_STATE7,
    XBT_eNMEA_STATE8,
    XBT_eNMEA_STATE9
} XBT_teNMEA_STATE;

/*=============================================================================
                                 Internal Data
=============================================================================*/

// Maintain a list of addresses for Diagnostics nodes, arbitrarily allow 4
static uchar8 aucDiagAddrList[ 4 ];

// Address claiming data
static uchar8 aucMyName[ XB_DLC_ISO_ADDR_CLAIM ]; // My node name
static uchar8 ucMyAddr = XBT_ADDR_NULL;          // Address being used (claimed)
static uchar8 ucPrevAddr = XBT_ADDR_NULL;        // Previous (claimed) address
static uchar8 ucClaimingState;                  // Address claiming state

// Timer data
static uint16 uiProTicks;               // Time counter for protocol timer
static uint16 uiClaimTicks;             // Time counter for periodic claim
static uint16 uiLoadStatusTicks;        // Time counter for periodic loadstatus
static void (*fnProFunc)(void);         // Function pointer for protocol timer

/*=============================================================================
                           Local Functions Declarations
=============================================================================*/

static void xbtaddr_fnState6( void );
static void xbtaddr_fnState9( void );
static void xbtaddr_fnRandomTimer( void );
static void xbtaddr_fnClaimTimer( void );
static void xbtaddr_fnChallenge( uchar8 *pucOther );

/*=============================================================================
                                   Functions
=============================================================================*/

/******************************************************************************

FUNCTION NAME:
    xbtaddr_fnRandom

PURPOSE:
    The NMEA address claiming protocol specifies a random timer such that
    on powerup or some other events, all of the nodes on the network will
    not try to send address claiming messages at the exact same time.
    This function is designed to use very little code space.

OUTPUTS:
    'uchar8' - a random number

NOTES:
    
VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version

******************************************************************************/

static uchar8 xbtaddr_fnRandom( void )
{
    // Use the lowest 2 bytes of our unique number (lowest word of our name)
    return( XT_mMK_BYTE( aucMyName[ 0 ] ^ aucMyName[ 1 ] ) );
}

/******************************************************************************

FUNCTION NAME:
    xbtaddr_fnStartProTimer

PURPOSE:
    This function starts a protocol timer, assigns a callback function
    for the protocol timer and then starts the timer.

INPUTS:
    'uiMilliSec' - timeout period in milli-seconds
    'fnFunc'     - function to call when timeout expires

NOTES:
    
VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version

******************************************************************************/

static void xbtaddr_fnStartProTimer( uint16 uiMilliSec,
                                     void (*fnFunc)(void) )
{
    // Timer never runs if time starts is zero
    if( uiMilliSec == 0 )
    {
        // Set number of ticks to smallest possible
        uiMilliSec++;
    }

    // Set function to be called
    fnProFunc = fnFunc;

    // Set number of ticks to go
    uiProTicks = uiMilliSec;
}

/******************************************************************************

FUNCTION NAME:
    xbtaddr_fnStopProTimer

PURPOSE:
    This function is used to clear the protocol timer before it triggers

NOTES:
    
VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version

******************************************************************************/

static void xbtaddr_fnStopProTimer( void )
{
    // Clear number of tick remaining;
    uiProTicks = 0;

    // Clear function to be called
    fnProFunc = NULL;
    
}

/******************************************************************************

FUNCTION NAME:
    XBTADDR_fnTick

PURPOSE:
    This function controls all timers in the XBT stack. It should be
    called periodically from the application layer.

NOTES:
    
VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version
Version 1.01  Date: 03/28/03  By: dalem
    - Added Loadstatus periodic send
    - Changed sense of timers to avoid need to initialize
Version 1.02  Date: 06/09/03  By: dalem
    - Removed periodic send of status message
Version 1.03  Date: 06/07/06  By: Trevor Monk
    - Added sanity checking to fnProFunc pointer before invoking it

******************************************************************************/

void XBTADDR_fnTick( void )
{
    /*
        Check address claiming protocol timer
    */
    
    // If protocol timer is active
    if( uiProTicks != 0 )
    {
        // If protocol timer has timed out
        if( --uiProTicks == 0 )
        {
            // Call protocol timer callback function
            if( fnProFunc != NULL )
            {
                (*fnProFunc)();
            }
        }
    }

    /*
        Check periodic transmit timers
    */

    // If we are active
    if( XBTADDR_fnOnLine() )
    {
        if( uiLoadStatusTicks++ > XBTA_LOADSTATUS_TIMEOUT )
        {
            // Request sending of a load status message
            XBTLOAD_fnReqLoadStatus( XBT_ADDR_GLOBAL );

            // Restart timer
            uiLoadStatusTicks = 0;
        }

        if( uiClaimTicks++ > XBTA_KEEPALIVE_TIMEOUT )
        {
            // Send status message
            XBTADDR_fnSendClaim( XBT_ADDR_GLOBAL );

            // Restart timer
            uiClaimTicks = 0;
        }
    }

    /*
        Check receive messages time to live timers
    */

    // Countdown busy receive messages
    XBTFRAME_fnBusyMsgCheck();
}

/******************************************************************************

FUNCTION NAME:
    XBTADDR_fnTick2

PURPOSE:
    This function is used to send address claim keep-alive messages, and request
    a load status message

******************************************************************************/
void XBTADDR_fnTick2( void )
{
    /*
        Check address claiming protocol timer
    */

    // If protocol timer is active
    if( uiProTicks != 0 )
    {
        // If protocol timer has timed out
        if( --uiProTicks == 0 )
        {
            // Call protocol timer callback function
            if( fnProFunc != NULL )
            {
                (*fnProFunc)();
            }
        }
    }

    if( XBTADDR_fnOnLine() )
    {
        if( uiLoadStatusTicks++ > XBTA_LOADSTATUS_TIMEOUT )
        {
            // Request sending of a load status message
//            XBTLOAD_fnReqLoadStatus( XBT_ADDR_GLOBAL );

            // Restart timer
            uiLoadStatusTicks = 0;
        }

        if( uiClaimTicks++ > XBTA_KEEPALIVE_TIMEOUT )
        {
            // Send status message
            XBTADDR_fnSendClaim( XBT_ADDR_GLOBAL );

            // Restart timer
            uiClaimTicks = 0;
        }
    }

    // Countdown busy receive messages
    XBTFRAME_fnBusyMsgCheck();

}

/******************************************************************************

FUNCTION NAME:
    xbtaddr_fnCompareNames

PURPOSE:
    This function is used to compare node names.

INPUTS:
    'pucN1' - pointer to the first node name
    'pucN2' - pointer to the second node name

OUTPUTS:
    1 if node 1 name is numerically larger than node 2
    -1 if node 1 name is smaller than node 2
    0 if node names are the same

NOTES:
    
VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version

******************************************************************************/

static sint16 xbtaddr_fnCompareNames( uchar8 *pucN1,
                                      uchar8 *pucN2 )
{
    uchar8 ucLoop;

    // Move pointers to highest order byte
    pucN1 += XB_DLC_ISO_ADDR_CLAIM - 1;
    pucN2 += XB_DLC_ISO_ADDR_CLAIM - 1;

    // Look at 8 bytes of name from last to first
    for( ucLoop = 0;
         ucLoop < XB_DLC_ISO_ADDR_CLAIM;
         ucLoop++, --pucN1, --pucN2 )
    {
        if( *pucN1 == *pucN2 )
            continue;               // Names match to this point
        else if( *pucN1 > *pucN2 )
            return( 1 );               // Name 1 greater than name 2
        else
            return( -1 );              // Name 2 greater than name 1
    }

    // Names are equal
    return( 0 );
}

/******************************************************************************

FUNCTION NAME:
    XBTADDR_fnIsDiag

PURPOSE:
    This function is used to determine if the given address is from
    a diagnostics node.

INPUTS:
    'ucAddr' - the source address of a message from the node in question

OUTPUTS:
    TRUE if address is of a diagnostics node
    FALSE if address is not of a diagnostics node

NOTES:
    
VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version
Version 1.01  Date: 06/11/03  By: dalem
    - Check that address is not a reserved value

******************************************************************************/

tucBOOL XBTADDR_fnIsDiag( uchar8 ucAddr )
{
    uchar8 ucIndex;

    // If address is a valid address
    if( ucAddr < XBT_ADDR_RESERVE2 )
    {
        // Search through all entries in Diag address list
        for( ucIndex = 0; ucIndex < DIM( aucDiagAddrList ); ucIndex++ )
        {
            // If address matches Diag address
            if( aucDiagAddrList[ ucIndex ] == ucAddr )
            {
                // Success
                return( TRUE );
            }
        }
    }

    // Address is not of a registered Diagnotic Node
    return( FALSE );
}

/******************************************************************************

FUNCTION NAME:
    XBTADDR_fnSendClaim

PURPOSE:
    This function is used to send an address claim message.  This can be
    used while claiming CAN bus addresses or as a keep alive message.

INPUTS:
    'ucDest' - destination of message (should always be XBT_ADDR_GLOBAL)

NOTES:
    
VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version
Version 1.01  Date: 04/01/03  By: dalem
    - Added ucDest to be consistent with other send functions
Version 1.02  Date: 07/21/03  By: dalem
    - Address claim always go to global address
Version 1.03  Date: 09/15/03  By: dalem
     - Added switch on claiming state transitions

******************************************************************************/

void XBTADDR_fnSendClaim( uchar8 ucDest )
{
    XBT_tzMSG *ptzMsg;

    // Action depends on claiming state
    switch( ucClaimingState )
    {
        case XBT_eNMEA_STATE2:
        case XBT_eNMEA_STATE7:
        case XBT_eNMEA_STATE9:
            // Sending is allowed, break out and send
            break;

        case XBT_eNMEA_STATE5:
            // Go to state 9
            xbtaddr_fnState9();

            // Do not send here
            return;

        case XBT_eNMEA_STATE8:
            // Go back to state 6
            xbtaddr_fnState6();

            // Do not send here
            return;

        case XBT_eNMEA_STATE0:   // no break
        case XBT_eNMEA_STATE1:   // no break
        case XBT_eNMEA_STATE3:   // no break
        case XBT_eNMEA_STATE4:   // no break
        case XBT_eNMEA_STATE6:
            // Cannot send when in this state
            return;

        default:
            // Invalid state, don't send
            return;
    }

    // Get address claim message
    ptzMsg = XBTMSG_fnNewMsg( XB_PGN_ISO_ADDR_CLAIM );

    // If there is a message buffer
    if( ptzMsg )
    {
        // If address is claimed or requesting a claim
        if( ( ucClaimingState == XBT_eNMEA_STATE9 )
         || ( ucClaimingState == XBT_eNMEA_STATE2 ) )
        {
            // Use my address
            ptzMsg->ucSrcAddr = ucMyAddr;
        }
        else
        {
            // Use network null address
            ptzMsg->ucSrcAddr = XBT_ADDR_NULL;
        }

        // Destination of address claim is always broadcast
        ptzMsg->ucDestAddr = XBT_ADDR_GLOBAL;

        // Copy name into message
        (void) memcpy( ptzMsg->pucData,
                       aucMyName,
                       XB_DLC_ISO_ADDR_CLAIM );

        // Send this message
        XBTMSG_fnSendAddressed( ptzMsg );
    }
}

/******************************************************************************

FUNCTION NAME:
    XBTADDR_fnGetAddr

PURPOSE:
    This function is used to get the current address of this node.  This
    address is the address that has been claimed or that is being claimed.

INPUTS:
    None

OUTPUTS:
    Current address of this node

NOTES:
    
VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version

******************************************************************************/

uchar8 XBTADDR_fnGetAddr( void )
{
    // Return currently used address
    return( ucMyAddr );
}

/******************************************************************************

FUNCTION NAME:
    XBTADDR_fnOnLine

PURPOSE:
    This function is used to test if this node has successfully claimed
    an address and can now use the CAN bus.

INPUTS:
    None

OUTPUTS:
    TRUE if address has been established
    FALSE if address is not valid or claiming is not complete

NOTES:
    
VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version

******************************************************************************/

tucBOOL XBTADDR_fnOnLine( void )
{
    return( ucClaimingState == XBT_eNMEA_STATE5 );
}

/******************************************************************************

FUNCTION NAME:
    XBTADDR_fnBeginClaiming

PURPOSE:
    This function is used to begin the address claiming operation

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    
VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version
Version 1.01  Date: 05/23/03  By: dalem
    - NVM access may be disabled later, keep a previous address in ram

******************************************************************************/

void XBTADDR_fnBeginClaiming( void )
{
    uint16 uiIndex;

    // Search through all entries in Diag address list
    for( uiIndex = 0; uiIndex < DIM( aucDiagAddrList ); uiIndex++ )
    {
        // Clear Diag node address list
        aucDiagAddrList[ uiIndex ] = XBT_ADDR_NULL;
    }

    // Get my node name
    XBTPLATFORM_fnLoadName( aucMyName );

    // Get preferred address
    ucMyAddr = XBTPLATFORM_fnLoadPrefAddr();

    // If preferred address not proper
    if( ucMyAddr >= XBT_ADDR_RESERVE2 )
    {
        // Start claiming at address 0
        ucMyAddr = 0;
    }

    // Save address as previously claimed address
    ucPrevAddr = ucMyAddr;

    // Start random timer
    xbtaddr_fnStartProTimer( xbtaddr_fnRandom(),
                             xbtaddr_fnRandomTimer );

    // Set address claiming state to state 1
    ucClaimingState = XBT_eNMEA_STATE1;
}

/******************************************************************************

FUNCTION NAME:
    xbtaddr_fnState2

PURPOSE:
    This function carries out the NMEA claiming state 2 processing

NOTES:
    
VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version

******************************************************************************/

static void xbtaddr_fnState2( void )
{
    // Set current state to state 2. Processing needs to know this state
    ucClaimingState = XBT_eNMEA_STATE2;

    // Clear random delay or claim timer
    xbtaddr_fnStopProTimer();

    // Send address claim
    XBTADDR_fnSendClaim( XBT_ADDR_GLOBAL );

    // Start 250 millisecond claim timer
    xbtaddr_fnStartProTimer( XBTA_CLAIM_TIMEOUT,
                             xbtaddr_fnClaimTimer );

    // Set next state to state 3
    ucClaimingState = XBT_eNMEA_STATE3;
}

/******************************************************************************

FUNCTION NAME:
    xbtaddr_fnState4

PURPOSE:
    This function carries out the NMEA claiming state 4 processing

NOTES:
    
VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version
Version 1.01  Date: 05/23/03  By: dalem
    - NVM my not be accessable, use previous address in ram

******************************************************************************/

static void xbtaddr_fnState4( void )
{
    // Set current state to state 4. Processing needs to know this state
    ucClaimingState = XBT_eNMEA_STATE4;

    // Clear address claim timer
    xbtaddr_fnStopProTimer();

    // Select new address
    ucMyAddr++;

    // Wrap address
    ucMyAddr = ( ucMyAddr < XBT_ADDR_RESERVE2 ) ? ucMyAddr : 0;

    // If address selection has wrapped back to starting address
    if( ucMyAddr == ucPrevAddr )
    {
        // Start MNEA loop of death
        xbtaddr_fnState6();
    }
    else
    {
        // Go to state 2 to start claiming new address
        xbtaddr_fnState2();
    }
}

/******************************************************************************

FUNCTION NAME:
    xbtaddr_fnState6        - Perform NMEA state 6 processing

PURPOSE:
    This function carries out the NMEA claiming state 6 processing

NOTES:
    
VERSION HISTORY:

Version 1.00  Date: 09/15/03  By: Dalem
    - Created

******************************************************************************/

static void xbtaddr_fnState6( void )
{
    // Set my address to the null address
    ucMyAddr = XBT_ADDR_NULL;

    // Start random timer
    xbtaddr_fnStartProTimer( xbtaddr_fnRandom(),
                             xbtaddr_fnRandomTimer );

    // Set next state to state 6
    ucClaimingState = XBT_eNMEA_STATE6;
}


/******************************************************************************

FUNCTION NAME:
    xbtaddr_fnState7

PURPOSE:
    This function carries out the NMEA claiming state 7 processing

NOTES:
    
VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version

******************************************************************************/

static void xbtaddr_fnState7( void )
{
    // Set current state to state 7. Processing needs to know this state
    ucClaimingState = XBT_eNMEA_STATE7;

    // Send address claim
    XBTADDR_fnSendClaim( XBT_ADDR_GLOBAL );

    // Set next state to state 8
    ucClaimingState = XBT_eNMEA_STATE8;
}

/******************************************************************************

FUNCTION NAME:
    xbtaddr_fnState9        - Perform NMEA state 9 processing

PURPOSE:
    This function carries out the NMEA claiming state 9 processing

NOTES:
    
VERSION HISTORY:

Version 1.00  Date: 09/15/03  By: Dalem
    - Created

******************************************************************************/

static void xbtaddr_fnState9( void )
{
    // Set state to state 9
    ucClaimingState = XBT_eNMEA_STATE9;

    // Send address claim
    XBTADDR_fnSendClaim( XBT_ADDR_GLOBAL );

    // Set state to state 5
    ucClaimingState = XBT_eNMEA_STATE5;
}

/******************************************************************************

FUNCTION NAME:
    xbtaddr_fnRandomTimer

PURPOSE:
    This function handles the timeout of the Random Timer

NOTES:
    
VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version

******************************************************************************/

static void xbtaddr_fnRandomTimer( void )
{
    // Action depends on NMEA claiming state
    switch( ucClaimingState )
    {
        case XBT_eNMEA_STATE1:
        {
            // Start address claiming
            xbtaddr_fnState2();
        }
        break;

        case XBT_eNMEA_STATE6:
        {
            // Send address claim
            xbtaddr_fnState7();
        }
        break;

        default:
        {
            // Should never get to this condition
            XBTERROR_fnSaveError( XBT_ERR_BAD_STATE );
        }
        break;
    }
}

/******************************************************************************

FUNCTION NAME:
    xbtaddr_fnClaimTimer

PURPOSE:
    This function handles the timeout of the Claim Timer

NOTES:
    
VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version
Version 1.01  Date: 03/28/03  By: dalem
    - Changed sense of timers to avoid need to initialize
Version 1.02  Date: 05/23/03  By: dalem
    - NVM my not be accessable, save address in RAM

******************************************************************************/

static void xbtaddr_fnClaimTimer( void )
{
    // Claim timer not valid if state is not 3
    if( ucClaimingState != XBT_eNMEA_STATE3 )
    {
        // Should never get to this condition
        XBTERROR_fnSaveError( XBT_ERR_BAD_STATE );
    }
    else
    {
        // Save claimed address as previously claimed address for next claim
        ucPrevAddr = ucMyAddr;

        // Set claiming state to 5 -- normal operation
        ucClaimingState = XBT_eNMEA_STATE5;
    }
}

/******************************************************************************

FUNCTION NAME:
    XBTADDR_fnGetMyFunction

PURPOSE:
    This function returns the device function for this node by looking at
    the device function field from its own name.

INPUTS:
    None

OUTPUTS:
    Device function for this node

NOTES:
    
VERSION HISTORY:

Version 1.00  Date: 04/02/03  By: dalem
    - Created

******************************************************************************/

uchar8 XBTADDR_fnGetMyFunction( void )
{
    // Return the device function field from my name
    return( aucMyName[ 5 ] );
}

/******************************************************************************

FUNCTION NAME:
    XBTADDDR_fnRecvClaim

PURPOSE:
    This function handles the ISO address claim message.  It is used during
    address claiming and during normal operation.  This function maintains
    the diag node list.

INPUTS:
    'ptzMsg' - pointer to the received message data.

NOTES:
    
VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version
Version 1.01  Date: 06/05/03  By: dalem
    - Only add Diag to list if not already present

******************************************************************************/

void XBTADDR_fnRecvClaim( XBT_tzMSG *ptzMsg )
{
    uint16 uiIndex;

    // If received address is my address
    if( ptzMsg->ucSrcAddr == ucMyAddr )
    {
        // Someone wants to use my address
        xbtaddr_fnChallenge( ptzMsg->pucData );
    }
    else    // Register the node
    {
        // If node is a DIAG
        if( ptzMsg->pucData[ 5 ] == XB_eFUNC_CODE_DIAGNOSTIC_TERMINAL )
        {
            // Search through all entries in diag address list
            for( uiIndex = 0; uiIndex < DIM( aucDiagAddrList ); uiIndex++ )
            {
                // If node matches
                if( aucDiagAddrList[ uiIndex ] == ptzMsg->ucSrcAddr )
                {
                    // Already in the list
                    break;
                }

                // If entry is not used
                if( aucDiagAddrList[ uiIndex ] == XBT_ADDR_NULL )
                {
                    // Remember diag address
                    aucDiagAddrList[ uiIndex ] = ptzMsg->ucSrcAddr;

                    // Done
                    break;
                }
            }
        }
        else    // Unregister registered node
        {
            // Search through all entries in diag addrress list
            for( uiIndex = 0; uiIndex < DIM( aucDiagAddrList ); uiIndex++ )
            {
                // If entry is in use
                if( aucDiagAddrList[ uiIndex ] == ptzMsg->ucSrcAddr )
                {
                    // Remove this address
                    aucDiagAddrList[ uiIndex ] = XBT_ADDR_NULL;

                    // Done
                    break;
                }
            }
        }
    }
}

/******************************************************************************

FUNCTION NAME:
    xbtaddr_fnChallenge

PURPOSE:
    This function is used when a node sends an address claiming message
    with the same address that this node is using.

INPUTS:
    'pucOther' - name of node claiming our address

NOTES:
    
VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version
Version 1.01  Date: 06/05/03  By: dalem
    - Ignore claims while waiting for random delay at start

******************************************************************************/

static void xbtaddr_fnChallenge( uchar8 *pucOther )
{
    sint16 siCmp;

    // Compare my name against challenger
    siCmp = xbtaddr_fnCompareNames( aucMyName,
                                    pucOther );

    // If the name is the same
    if( siCmp == 0 )
    {
        // It's me, ignore it
        return;
    }

    // Action depends on NMEA claiming state
    switch ( ucClaimingState )
    {
        case XBT_eNMEA_STATE1:
        {
            // Ignore names while in the random wait period 
        }
        break;

        case XBT_eNMEA_STATE3:
        {
            // If my name is better (lower) -- I Win
            if( siCmp < 0 )
            {
                // Restart address claiming
                xbtaddr_fnState2();
            }
            else
            {
                // I lose, do state 4 processing
                xbtaddr_fnState4();
            }
        }
        break;

        case XBT_eNMEA_STATE5:
        {
            // If my name is better (lower) -- I Win
            if( siCmp < 0 )
            {
                // Reassert my claim
                XBTADDR_fnSendClaim( XBT_ADDR_GLOBAL );
            }
            else
            {
                // I lose, do state 4 processing
                xbtaddr_fnState4();
            }
        }
        break;

        default:
        {
            // Should never get to this condition
            XBTERROR_fnSaveError( XBT_ERR_BAD_STATE );
        }
    }
}
/******************************************************************************

FUNCTION NAME:
    XBTADDR_fnResetLoadStatusTimer

PURPOSE:
    API to allow other device to reset the timer

NOTES:
    
VERSION HISTORY:

Version 1.00  Date: 06/19/2014  By: Robert Shen
    - Created

******************************************************************************/
void XBTADDR_fnResetLoadStatusTimer(void)
{
  uiLoadStatusTicks = 0;
}
