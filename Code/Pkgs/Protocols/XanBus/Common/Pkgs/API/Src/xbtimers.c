/******************************************************************************

  (c) 2003 - 2023 Xantrex Canada ULC. All rights reserved.
  All trademarks are owned or licensed by Xantrex Canada ULC,
  its subsidiaries or affiliated companies.

****************************************************************************//**

  @addtogroup xbtimers XBTIMERS
  @{

    @brief Management of RV-C Message Timers

****************************************************************************//**

  @page xbtimers_design CAN Message Timers Management Design

    This file supplies some timer routines for the XanBus interface

  @par API Functions
    XBTIMERS_fnLoadTxTimers     - Load periodic transmit timers from NVRam
    XBTIMERS_fnSetTxTimer       - Change a periodic transmit timer
    XBTIMERS_fnSetPeriodReports - Enable or disable periodic reporting
    XBTIMERS_fnLoadRxTimers     - Load receive timeouts from NVRam
    XBTIMERS_fnTickTimers       - Count down and look for expired timers
    XBTIMERS_fnRecvPerXmitCfg   - Receive Period Transmit Configuration
    XBTIMERS_fnSendPerXmitCfg   - Send Periodic Transmit Configuration
    XBTIMERS_fnRecvRxToCfg      - Receive an Rx Timeout Configuration
    XBTIMERS_fnSendRxToCfg      - Send Receive Timeout Configuration
    XBTIMERS_fnResetTxToCfg     - Handle a reset config command for Tx Timers
    XBTIMERS_fnResetRxToCfg     - Handle a reset config command for Rx Timers

  @par Local Functions
        xbtimers_fnPeriodicSend     - Send periodic reports
        xbtimers_fnFindTxTimer      - Look for periodic transmit timer
        xbtimers_fnSetFirstTxPeriod - Load time into tx timer
        xbtimers_fnDropClaimTimer   - Remove an address claim timer
        xbtimers_fnCheckTxCfg       - Validate TX timeout configuration
        xbtimers_fnChangeTxToCfg    - Change Tx Timer records
        xbtimers_fnSendOneTxToCfg   - Send Tx timers for one configuration type
        xbtimers_fnCheckRxCfg       - Validate RX timeout configuration
        xbtimers_fnChangeRxToCfg    - Change Tx Timer records
        xbtimers_fnSendOneRxToCfg   - Send Rx timers for one configuration type

*******************************************************************************/

/*==============================================================================
                              Includes
==============================================================================*/

// XanBus library include files
#include "xbldefs.h"    // Local Definitions
#include "xbpgfns.h"    // XanBus-specific PGN handler functions

// System include files
#include <string.h>     // for mem functions
#include "xassert.h"    // Debugging

/*==============================================================================
                              Defines
==============================================================================*/

#define XB_NO_TIME  ( PGN_UI_NO_DATA )

/*==============================================================================
                            Type Definitions
==============================================================================*/

//--- Structure used by XanBus to manage receive timeout timers ---
typedef struct zRECV_TO
{
    uint16 uiTicks;
    uchar8 ucSrcAddr;
} tzRECV_TO;

/*==============================================================================
                              External Functions
==============================================================================*/


/*==============================================================================
                           Local/Private Variables
==============================================================================*/

// The period transmit (send) timers are used to issue a send request of a
// specific PGN and address at a set rate. The Send Timer list is in this
// format to facilitate responding to a XanBus request for the data.  The Send
// Timer list has an associated Send Ticks list used to keep track of the
// expiring time per PGN.

// Storage for Periodic Transmit timeout active configuration data
static XB_tzSEND_TIMERS tzSendTimers;

// This is the array of time counts for each PerXmit timer
static uint16 auiSendTicks[ XB_TIMER_MAX_SEND ];

// Flag for controlling periodic reports
static tucBOOL tucReportsControl = FALSE;

// Reload time of claim timers in ticks
static uint16 uiClaimTicks;

/*==============================================================================
                              Forward declarations
==============================================================================*/
static tucBOOL xbtimers_fnLoadRxTimers( XB_tzRECV_TIMERS *ptzRecvTimers );
static tucBOOL xbtimers_fnLoadTxTimers( XB_tzSEND_TIMERS *ptzSendTimers );

static void xbtimers_fnPeriodicSend( uint32 ulPgn,
                                     uchar8 ucDest );
static uint16 xbtimers_fnFindTxTimer( uint32 ulPgn,
                                      uchar8 ucDest );
static void xbtimers_fnSetFirstTxPeriod( uint16 uiTimer,
                                         uint16 uiStartAt );
#if defined( HANDLE_RX_TX_PERIOD )
static tucBOOL xbtimers_fnCheckTxCfg( XB_tzPGN_EXTRA_PER_XMIT_CFG *ptzCfg,
                                      uint16 uiCount );
static tucBOOL xbtimers_fnChangeTxToCfg( XB_tzSEND_TIMERS *ptzTimers,
                                         XB_tzPGN_EXTRA_PER_XMIT_CFG *ptzNew,
                                         uint16 uiNewCount );
#endif // defined( HANDLE_RX_TX_PERIOD )

static void xbtimers_fnSendOneTxToCfg( XB_teCFG_TYPE teCfgType,
                                       uchar8 ucMsgCount,
                                       XB_tzSEND_TIMERS *ptzTimers,
                                       uchar8 ucDest );

#if defined( HANDLE_RX_TX_PERIOD )
static tucBOOL xbtimers_fnCheckRxCfg( XB_tzPGN_EXTRA_RX_TO_CFG *ptzCfg,
                                      uint16 uiCount );
static tucBOOL xbtimers_fnChangeRxToCfg( XB_tzRECV_TIMERS *ptzTimers,
                                         XB_tzPGN_EXTRA_RX_TO_CFG *ptzNew,
                                         uint16 uiNewCount );
#endif // defined( HANDLE_RX_TX_PERIOD )

static void xbtimers_fnSendOneRxToCfg( XB_teCFG_TYPE teCfgType,
                                       uchar8 ucMsgCount,
                                       XB_tzRECV_TIMERS *ptzTimers,
                                       uchar8 ucDest );

/*==============================================================================
                           Function Definitions
==============================================================================*/

/*******************************************************************************

FUNCTION NAME:
    XBTIMERS_fnGetClaimTimer  - Return the claim timer

PURPOSE:
    Get the claim timer currently in use

INPUTS:
    None

OUTPUTS:
    Claim timer

NOTES:

VERSION HISTORY:

Version 1.00  Date: 07/24/05  By: Hollyz
    - Created

*******************************************************************************/

uint16 XBTIMERS_fnGetClaimTimer( void )
{
    return ( uiClaimTicks );
}


/******************************************************************************

FUNCTION NAME:
    XBTIMERS_fnLoadRxTimers     - Load receive timeouts from NVRam

PURPOSE:
    This function is used to set up the receive message timeout data list

INPUTS:
    None

OUTPUTS:
    TRUE if load from platform succeeds
    FALSE it load fails

NOTES:
    The only receive timer of interest for now is the Address Claim timer.

VERSION HISTORY:

Version 1.01  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.02  Date: 08/12/03  By: Dalem
    - Explicit use of XB_eCFG_TYPE_USER
    - RecvTimers structure changed
    - Return parameter changed
Version 1.03  Date: 07/28/05  By: Hollyz
    - Call local funciton to load the receive timer from the specified location

******************************************************************************/
tucBOOL XBTIMERS_fnLoadRxTimers( void )
{
    XB_tzRECV_TIMERS tzRecvTimers;

    // If receive timeouts can be read
    if( XBPLATFORM_fnLoad( XB_ePARAM_TYPE_RX_TIMEOUTS,
                           XB_eCFG_TYPE_USER,
                           &tzRecvTimers,
                           sizeof( tzRecvTimers ) ) == ( tuiSTATUS )eSTATUS_OK )
    {
        return ( xbtimers_fnLoadRxTimers( &tzRecvTimers ) );
    }

    // timers cannot be read
    return( FALSE );
}

/******************************************************************************

FUNCTION NAME:
    XBTIMERS_fnLoadTxTimers     - Load periodic transmit timers from NVRam

PURPOSE:
    Load the periodic transmit timers from non-volatile memory and
    start any timers that are not zero.

INPUTS:
    none

OUTPUTS:
    TRUE if load from platform succeeds
    FALSE it load fails

NOTES:

VERSION HISTORY:

Version 1.03  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.04  Date: 08/12/03  By: Dalem
    - Explicit use of XB_eCFG_TYPE_USER
    - SendTimers structure changed
    - Return parameter changed
Version 1.05  Date: 07/28/05  By: Hollyz
    - Call local funciton to load the transmit timer from the specified
      location

******************************************************************************/

tucBOOL XBTIMERS_fnLoadTxTimers( void )
{
    // If transmit timeouts can be read
    if( XBPLATFORM_fnLoad( XB_ePARAM_TYPE_TX_TIMEOUTS,
                           XB_eCFG_TYPE_USER,
                           &tzSendTimers,
                           sizeof( tzSendTimers ) ) == ( tuiSTATUS )eSTATUS_OK )
    {
        return ( xbtimers_fnLoadTxTimers( &tzSendTimers ) );
    }

    // timers cannot be read
    return( FALSE );
}

/******************************************************************************

FUNCTION NAME:
    XBTIMERS_fnSetTxTimer       - Change a periodic transmit timer

PURPOSE:
    This function is used to set up a change the period of a periodic timer
    the periodic sending of a PGN to a destination address.

INPUTS:
    'ulPGgn is Program Group Number of message to be sent
    'ucDest' is Destination address for PGN
    'uiCentiSec' is period in centiseconds between sends
    'uiStartAt' is what time to start the first period in centiseconds

OUTPUTS:
    TRUE on success
    FALSE if a timer could not be allotted

NOTES:
    This function assumes that the timer will be connected to the periodic
    tick supplied for the Teleflex stack and calculates timer periods
    based on the configured tick rate.

    Timer must match in PGN and destination to be considered the same.
    Setting uiPeriod will modify a timer except as noted below.
    Timer period will not change if uiPeriod is set to XB_TIMER_NO_CHANGE.
    uiStartAt will be used for the initial period except as noted below.
    Setting uiStartAt to 0 will stop the timer but not remove it.
    Setting uiStartAt to 1 is the shortest timer period.
    Setting uiStartAt to 1 cause the timer to trigger on the next Tick.
    Setting uiStartAt to a value greater than the period (such as 0xFFFF) will
        restart the timer using the full period.

VERSION HISTORY:

Version 1.01  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.02  Date: 08/14/03  By: Dalem
    - Timers will not be created or destroyed
    - Timers will not be saved to non-volatile memory

******************************************************************************/

tucBOOL XBTIMERS_fnSetTxTimer( uint32 ulPgn,      // PGN
                               uchar8 ucDest,      // Destination address
                               uint16 uiPeriod,    // Timer period in centiseconds
                               uint16 uiStartAt )  // Time of first period
{
    uint16 uiTimer;

    // Look for existing periodic timer
    uiTimer = xbtimers_fnFindTxTimer( ulPgn,
                                      ucDest );

    // If timer is not found
    if( uiTimer == PGN_UI_NO_DATA )
    {
        // Don't set/change it
        return( FALSE );
    }
    else
    {
        // If period is the 'no change' value
        // or period is not changing
        if( ( uiPeriod == XB_TIMER_NO_CHANGE )
         || ( tzSendTimers.atzTimers[ uiTimer ].uiXmitPeriod == uiPeriod ) )
        {
            // Set initial period
            xbtimers_fnSetFirstTxPeriod( uiTimer,
                                         uiStartAt );

            // Done
            return( TRUE );
        }

        // Timer is changing
        else
        {
            // Change the period
            tzSendTimers.atzTimers[ uiTimer ].uiXmitPeriod = uiPeriod;

             // Set initial period
             xbtimers_fnSetFirstTxPeriod( uiTimer,
                                          uiStartAt );

            // Done
            return( TRUE );
        }
    }
}

/******************************************************************************

FUNCTION NAME:
    XBTIMERS_fnSetPeriodReports - Enable or disable periodic reporting

PURPOSE:
    This function is used to set the state of the reports control flag.
    This flag is used to control production of periodic messages.

INPUTS:
    'tucActive' is TRUE to allow Period Reporting
                is FALSE to block Period Reporting

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.03  Date: 06/24/03  By: Dalem
    - Major change to XB API

******************************************************************************/

void XBTIMERS_fnEnablePeriodReports( tucBOOL tucActive )
{
    // Set state of periodic message reporting
    tucReportsControl = tucActive;
}

/******************************************************************************

FUNCTION NAME:
    XBTIMERS_fnTickTimers       - Count down and look for expired timers

PURPOSE:
    This function supplies message timers.  When called it counts down
    active timers and checks for expired timers.  For expired PGN timers
    the application is called with the PGN number and destination address.


INPUTS:
    None

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.02  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.03  Date: 08/12/03  By: Dalem
    - SendTimers structure changed
Version 1.03  Date: 02/09/09  By: Hollyz
    - Disable the periodic tramsmit messages except ISO_ADDR_CLAIM if the
      CAN Tx Transmitter is blocked

******************************************************************************/
void XBTIMERS_fnTickTimers( void )
{
    uint16 uiTimer;
    XB_tzPGN_EXTRA_PER_XMIT_CFG *ptzTimers;

    // Go through list of active periodic transmit timers
    for( uiTimer = 0; uiTimer < XB_TIMER_MAX_SEND; uiTimer++ )
    {
        // If timer is active -- tick count not zero
        if( auiSendTicks[ uiTimer ] != 0 )
        {
            // Get pointer for convenience
            ptzTimers = &tzSendTimers.atzTimers[ uiTimer ];

            // If timer is valid (reloadable)
            if( ( ptzTimers->uiXmitPeriod != 0 )
             && ( ptzTimers->uiXmitPeriod != XB_NO_TIME ) )
            {
                // Count down timer and check for expired
                if( --auiSendTicks[ uiTimer ] == 0 )
                {
                    // If we are on line
                    if( XBADDR_fnOnLine() != FALSE )
                    {
                        // if the CAN transmitter is active, or
                        // if the message to send is ISO Address Claim
                        if( ( ptzTimers->ulParmGrpNum == XB_PGN_ISO_ADDR_CLAIM ) ||
                            ( TFXDRV_fnIsBlocked(  XB_PORT_A  ) == FALSE  ) )
                        {
                            // Tell app to send PGN
                            xbtimers_fnPeriodicSend( ptzTimers->ulParmGrpNum,
                                                     ptzTimers->ucXmitAddr );
                        }
                    }

                    // Reload timer
                    auiSendTicks[ uiTimer ] =
                            XBINIT_fnCSec2Ticks( ( uint32 )ptzTimers->uiXmitPeriod  );
                }
            }
        }
    }

    // check all claim timers
    XBADDR_fnTickClaimTimers();
}


/******************************************************************************

FUNCTION NAME:
    xbtimers_fnPeriodicSend     - Send periodic reports

PURPOSE:
    This function is used to handle a periodic transmit request from a timer

INPUTS:
    'ulPgn' is PGN from periodic timer
    'ucDest' is destination for message send

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.03  Date: 06/24/03  By: Dalem
    - Major change to XB API

******************************************************************************/

static void xbtimers_fnPeriodicSend( uint32 ulPgn,
                                     uchar8 ucDest )
{
    PGN_tzWANT_DATA tzWant;

    // If address claim PGN
    if( ulPgn == XB_PGN_ISO_ADDR_CLAIM )
    {
        // Request an address claim be sent from me
        tzWant.tucSolicited = FALSE;
        tzWant.ucDestAddr = PGN_ADDR_GLOBAL;

        XBADDR_fnWantClaim( &tzWant );
    }

    // If reporting is allowed
    else if( tucReportsControl != FALSE )
    {
        // Request application to send off an unsolicited PGN
        XBMSG_fnPeriodicSend( ulPgn,
                              ucDest );
    }
}

/******************************************************************************

FUNCTION NAME:
    xbtimers_fnFindTxTimer      - Look for periodic transmit timer

PURPOSE:
    This function checks the list of periodic timers for one matching
    the given PGN, destination address pair.

INPUTS:
    'ulPgn' is PGN
    'ucDest' is destination address

OUTPUTS:
    PGN_UI_NO_DATA if timer is not found
    index number of timer in periodic timer list if found

NOTES:

VERSION HISTORY:

Version 1.02  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.03  Date: 08/12/03  By: Dalem
    - SendTimers structure changed

******************************************************************************/

static uint16 xbtimers_fnFindTxTimer( uint32 ulPgn,
                                      uchar8 ucDest )
{
    XB_tzPGN_EXTRA_PER_XMIT_CFG *ptzTimer;
    uint16 uiEntry;

    // Go through list of active PGN timeout records
    for( uiEntry = 0; uiEntry < XB_TIMER_MAX_SEND; uiEntry++ )
    {
        // Get pointer for convenience
        ptzTimer = &tzSendTimers.atzTimers[ uiEntry ];

        // Look for given PGN and address in the list
        if( ( ptzTimer->ulParmGrpNum == ulPgn )
         && ( ptzTimer->ucXmitAddr == ucDest ) )
        {
            // Record found, exit search loop
            return( uiEntry );
        }
    }

    // Failed to find periodic timer
    return( PGN_UI_NO_DATA );
}

/******************************************************************************

FUNCTION NAME:
    xbtimers_fnSetFirstTxPeriod - Load time into tx timer

PURPOSE:
    This function is used to load time into a TX timer. It is intended
    to be used for loading the initial time period of a TX timer. Sometimes
    it is convienient to trigger the timer 'right away' (use StartAt = 1)
    and other times it does not matter so use XB_TIMER_NO_CHANGE. Using 0 will
    disable the timer without removing it so that the timer can be restarted
    without knowing what period is currently configured.

INPUTS:
    'siTimer' is index of TX timer being started
    'uiStartAt' is time to use for first time period in centiseconds

OUTPUTS:
    None

NOTES:
    A start time of zero is allowed and will turn off the timer without
    removing it.

VERSION HISTORY:

Version 1.01  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.02  Date: 08/12/03  By: Dalem
    - SendTimers structure changed

******************************************************************************/

static void xbtimers_fnSetFirstTxPeriod( uint16 uiTimer,     // Index of timer
                                         uint16 uiStartAt )  // Initial time
{
    uint16 uiPeriod;
    uint16 uiTicks;

    // Look up the timer period
    uiPeriod = tzSendTimers.atzTimers[ uiTimer ].uiXmitPeriod;

    // If the start at time is less than the period
    if( uiStartAt < uiPeriod )
    {
        // First period becomes the start at time
        uiPeriod = uiStartAt;
    }

    // If start at is 1
    if( uiStartAt == 1 )
    {
        // Use the smallest value of ticks (which is 1)
        uiTicks = 1;
    }
    else
    {
        // Convert start time to ticks
        uiTicks = XBINIT_fnCSec2Ticks( ( uint32 )uiPeriod );
    }

    // Set tick count to start time
    auiSendTicks[ uiTimer ] = uiTicks;
}
/******************************************************************************

FUNCTION NAME: from NVRam
    xbtimers_fnLoadRxTimers     - Load receive timeouts

PURPOSE:
    This function is used to load the receive timers from the specified memory
    location

INPUTS:
    ptzRecvTiemrs - Pointer to the receive timer struct

OUTPUTS:
    TRUE if load from platform succeeds
    FALSE it load fails

NOTES:
    The only receive timer of interest for now is the Address Claim timer.

VERSION HISTORY:

Version 1.01  Date: 07/24/05  By: Hollyz
    - Created

******************************************************************************/
static tucBOOL xbtimers_fnLoadRxTimers( XB_tzRECV_TIMERS *ptzRecvTimers )
{
    uchar8 ucEntry;
    XB_tzPGN_EXTRA_RX_TO_CFG *ptzTimer;

    assert( ptzRecvTimers );

    // For all timer records
    for( ucEntry = 0; ucEntry < XB_TIMER_MAX_RECV; ucEntry++ )
    {
        // Get convenience pointer
        ptzTimer = &ptzRecvTimers->atzTimers[ ucEntry ];

        // Look for device Claim timer
        if( ptzTimer->ulParmGrpNum == DEVICE_CLAIM_PGN )
        {
            // Save claim timeout restart value
            uiClaimTicks = XBINIT_fnCSec2Ticks( ( uint32 )ptzTimer->uiTimeout );

            // load the claim timers for all active nodes
            XBADDR_fnLoadClaimTimers();

            // Found timer of interest
            return( TRUE );
        }
    }

    return ( FALSE );
}

/******************************************************************************

FUNCTION NAME:
    xbtimer_fnLoadTxTimers     - Load periodic transmit timers

PURPOSE:
    Load the periodic transmit timers from the specified timer struct and
    start any timers that are not zero.

INPUTS:
    ptzSendTimers - Pointer to the send timers struct

OUTPUTS:
    TRUE if load from platform succeeds
    FALSE it load fails

NOTES:

VERSION HISTORY:

Version 1.00  Date: 07/24/05  By: Hollyz
    - Created

******************************************************************************/

static tucBOOL xbtimers_fnLoadTxTimers( XB_tzSEND_TIMERS *ptzSendTimers )
{
    uchar8 ucEntry;
    uint16 uiPeriod;

    // Look for active timers
    for( ucEntry = 0; ucEntry < XB_TIMER_MAX_SEND; ucEntry++ )
    {
        // Get timeout for this entry
        uiPeriod = ptzSendTimers->atzTimers[ ucEntry ].uiXmitPeriod;

        // If timeout is not NO DATA
        if( uiPeriod != PGN_UI_NO_DATA )
        {
            // We do not want all timers of a particular class to
            // all tigger at the same time, and stay in lock step
            // all the time so adjust first period of timers by an
            // offset dependent on entry number as long as the
            // time would not go to zero. Use offset of 4 ticks per item
            if ( uiPeriod > ucEntry * 4 )
            {
                uiPeriod -= ( ucEntry * 4 );
            }

            // Set tick count to start timer
            auiSendTicks[ ucEntry ] = XBINIT_fnCSec2Ticks( ( uint32 )uiPeriod );
         }
         else
         {
             // Set tick count to stop timer
             auiSendTicks[ ucEntry ] = 0;
         }
     }

     // Success
     return( TRUE );
}



/******************************************************************************

FUNCTION NAME:
    xbtimers_fnCheckTxCfg       - Validate TX timeout configuration

PURPOSE:
    This function is used, when setting timers, to check if timer
    configuration is acceptable.

INPUTS:
    'ptzCfg' is pointer to configuration list
    'uiCount' is number of entries in the list

OUTPUTS:
    TRUE if timer configuration passes tests
    FALSE if timer is not valid

NOTES:

VERSION HISTORY:

Version 1.00  Date: 10/01/03  By: Dalem
    - Created
Version 1.01  Date: 06/25/04  By: Peterd
    - Added check to ensure PGN is a status message
    - PRN 2494
Version 1.02  Date: 07/19/04  By: Peterd
    - Fixed check as field is an enumeration not a collection of bit flags
    - PRN 2494, again

******************************************************************************/
#if defined( HANDLE_RX_TX_PERIOD )
static tucBOOL xbtimers_fnCheckTxCfg( XB_tzPGN_EXTRA_PER_XMIT_CFG *ptzCfg,
                                      uint16 uiCount )
{
    const PGN_tzPGN_INFO *pktzInfo;
    uchar8 ucIndex;
    uint32 ulPgn;
    uchar8 ucAddr;
    uint16 uiPeriod;

    // Verify parameter
    (void)assert( ptzCfg != NULL );

    // For all entries in the configuration
    for ( ucIndex = 0; ucIndex < uiCount; ucIndex++ )
    {
        // Get the PGN and address and period for convenience
        ulPgn = ptzCfg[ ucIndex ].ulParmGrpNum;
        ucAddr = ptzCfg[ ucIndex ].ucXmitAddr;
        uiPeriod = ptzCfg[ ucIndex ].uiXmitPeriod;

        // If time value is reserved or greater but not Do Not Change
        if( ( uiPeriod >= PGN_UI_RESERVED ) && ( uiPeriod != PGN_UI_NO_DATA ) )
        {
            // Reject this message
            return( FALSE );
        }

    // Short periods can swamp the output and take the device offline
    if (( uiPeriod < XB_PERIOD_MIN ) && ( uiPeriod != 0 ))
        {
            // Reject this message
            return( FALSE );
        }

        // If address is a reserved or special address but not Global address
        if( ( ucAddr >= PGN_ADDR_RESERVE2 ) && ( ucAddr != PGN_ADDR_GLOBAL ) )
        {
            // Reject the message
            return( FALSE );
        }

        // If PGN if not the wild card
        if( ulPgn != PGN_PGN_NO_DATA )
        {
            // Search list of support for PGNs
            pktzInfo = XBPACK_fnGetPgnInfo( ulPgn,
                                            ( sint16 )XB_eGRP_FUNC_VALUE_DO_NOT_CARE );

            // If PGN not found or no send function exists or the PGN is not a Status message
            if( ( pktzInfo == NULL ) ||
                ( pktzInfo->pktzDesc == NULL ) ||
                ( pktzInfo->pktzDesc->pfnWant == NULL ) ||
                ( pktzInfo->uiPgTypeFlags != PGN_PG_TYPE_RSP ) )
            {
                // Requested PGN cannot be used
                return( FALSE );
            }
        }
    }

    // Success
    return( TRUE );
}
#endif // defined( HANDLE_RX_TX_PERIOD )

/******************************************************************************

FUNCTION NAME:
    xbtimers_fnChangeTxToCfg    - Change Tx Timer records

PURPOSE:
    This function is used to change transmit timeouts configuration records.

INPUTS:
    'ptzTimers' is the transmit timers data
    'ptzNew' is a list of new timeout data
    'uiNewCount' is number of entries in the new timeout list

OUTPUTS:
    TRUE if all new records were updated
    FALSE if some data is invalid and the changes should be aborted

NOTES:

VERSION HISTORY:

Version 1.00  Date: 08/13/03  By: Dalem
    - Created
Version 1.01  Date: 09/11/03  By: Dalem
    - Added check of PGN
    - If time is zero, remove the timer record

******************************************************************************/
#if defined( HANDLE_RX_TX_PERIOD )
static tucBOOL xbtimers_fnChangeTxToCfg( XB_tzSEND_TIMERS *ptzTimers,
                                         XB_tzPGN_EXTRA_PER_XMIT_CFG *ptzNew,
                                         uint16 uiNewCount )
{
    uint16 uiNew;
    uint16 uiOld;
    uint32 ulPgn;
    uchar8 ucAddr;
    uint16 uiPeriod;

    // Verify parameter
    (void)assert( ptzTimers != NULL );
    (void)assert( ptzNew != NULL );

    // For every new received timer
    for ( uiNew = 0; uiNew < uiNewCount; uiNew++ )
    {
        // Get the PGN and address and period for convenience
        ulPgn = ptzNew[ uiNew ].ulParmGrpNum;
        ucAddr = ptzNew[ uiNew ].ucXmitAddr;
        uiPeriod = ptzNew[ uiNew ].uiXmitPeriod;

        // If time value is the Do Not Change value
        if( uiPeriod == PGN_UI_NO_DATA )
        {
            // Ignore this setting
            continue;
        }

        // For every existing timer
        for ( uiOld = 0; uiOld < XB_TIMER_MAX_SEND; uiOld++ )
        {
            // If timers matches in PGN
            if( ptzTimers->atzTimers[ uiOld ].ulParmGrpNum == ulPgn )
            {
                // Entry found

                // If period is zero
                if( uiPeriod == 0 )
                {
                    // Clear the timer record
                    ptzTimers->atzTimers[ uiOld ].ulParmGrpNum = PGN_PGN_NO_DATA;
                    ptzTimers->atzTimers[ uiOld ].ucXmitAddr = PGN_UC_NO_DATA;
                    ptzTimers->atzTimers[ uiOld ].uiXmitPeriod = PGN_UI_NO_DATA;
                }
                else
                {
                    // Set address and timeout to the received values
                    ptzTimers->atzTimers[ uiOld ].ucXmitAddr = ucAddr;
                    ptzTimers->atzTimers[ uiOld ].uiXmitPeriod = uiPeriod;
                }

                // Finished search
                break;
            }
        }

        // If existing record not found and time value is not zero
        if( ( uiOld >= XB_TIMER_MAX_SEND ) && ( uiPeriod != 0 ) )
        {
            // Search timer list for unused extry
            for ( uiOld = 0; uiOld < XB_TIMER_MAX_SEND; uiOld++ )
            {
                // If timer not in use
                if( ptzTimers->atzTimers[ uiOld ].uiXmitPeriod == XB_NO_TIME )
                {
                    // Found an empty record
                    ptzTimers->atzTimers[ uiOld ].ulParmGrpNum = ulPgn;
                    ptzTimers->atzTimers[ uiOld ].ucXmitAddr = ucAddr;
                    ptzTimers->atzTimers[ uiOld ].uiXmitPeriod = uiPeriod;

                    // Finished search
                    break;
                }
            }

            // If free record not found
            if( uiOld >= XB_TIMER_MAX_SEND )
            {
                // Reject this message
                return( FALSE );
            }
        }
    }

    // Success
    return( TRUE );
}
#endif // defined( HANDLE_RX_TX_PERIOD )

/******************************************************************************

FUNCTION NAME:
    XBTIMERS_fnRecvPerXmitCfg   - Receive Period Transmit Configuration

PURPOSE:
    This function handles receiving of a period transmit message by saving
    the timer value for the given PGN

INPUTS:
    'ptzRecv' is a pointer to the received message work block

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.03  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.04  Date: 08/13/03  By: Dalem
    - SendTimers structure changed
    - Increased error checking
    - Handle different configuration types
Version 1.05  Date: 08/25/03  By: Dalem
    - Update CAC when updating User level timers
Version 1.06  Date: 08/29/03  By: Dalem
    - Added reason code for rejecting configuration
Version 1.07  Date: 09/03/03  By: Tom Lightfoot
    - Passed reported configurations to the application
Version 1.08  Date: 09/05/03  By: Dalem
    - Moved memcpy outside of for loop
Version 1.09  Date: 09/11/03  By: Dalem
    - Added check of PGN
Version 1.10  Date: 10/01/03  By: Dalem
    - Check all elements of configurations
Version 1.11  Date: 12/11/03  By: Dalem
    - If setting all timers, reject if time is 0 or NO DATA
Version 1.12  Date: 06/25/04  By: Peterd
    - Changed call to XBADDR_fnCheckCfgSrc to include Ack Type parameter
    - PRN 2514

******************************************************************************/
#if defined( HANDLE_RX_TX_PERIOD )
void XBTIMERS_fnRecvPerXmitCfg( PGN_tzRECV_DATA *ptzRecv )
{
    XB_tzPGN_PER_XMIT_CFG   *ptzCfg;
    XB_tzSEND_TIMERS        tzTimers;
    uchar8                  ucIndex;
    XB_teACK_STS            teAckType;
    XB_teGRP_FUNC_VALUE     teReason;

    // Verify parameter
    (void)assert( ptzRecv != NULL );

    // Set a default response
    teAckType = XB_eACK_STS_NAK;
    teReason = XB_eGRP_FUNC_VALUE_DATA_NOT_AVAILABLE;

    // Convert receive message void pointer to receive timeout config pointer
    ptzCfg = ( XB_tzPGN_PER_XMIT_CFG * )ptzRecv->pvStruct;

    // Is this a requested configuration report?
    if( ptzCfg->ucMsgCount != 0 )
    {
        // Pass the message on to the configuration
        XBPLATFORM_fnRecvXbMsg( ptzRecv );

        // And don't ack/nak
        return;
    }

    // Check the source of the configuration
    if( XBMSG_fnCheckCfgSrc( ptzCfg->teCfgType,
                               ptzRecv->ucSrcAddr,
                               &teAckType ) == FALSE )
    {
        // Reject with appropriate code
        teReason = XB_eGRP_FUNC_VALUE_SECURITY;
    }
    // If message is larger than we can handle
    else if( ptzCfg->uiExtraCount > XB_TIMER_MAX_SEND )
    {
        // Reject for invalid data
        teAckType = XB_eACK_STS_NAK;
        teReason = XB_eGRP_FUNC_VALUE_DATA_INVALID;
    }
    // If we are in the wrong mode to handle this configuration
    else if (( XBINIT_pfnAppCfgFilter != NULL )
         && !XBINIT_pfnAppCfgFilter( ptzRecv->ulPgn ) )
    {
        teAckType = XB_eACK_STS_DNY;
        teReason = XB_eGRP_FUNC_VALUE_WRONG_MODE;
    }
    // If loading of configuration fails
    else if( XBPLATFORM_fnLoad( XB_ePARAM_TYPE_TX_TIMEOUTS,
                                ptzCfg->teCfgType,
                                &tzTimers,
                                sizeof( tzTimers ) ) != ( tuiSTATUS )eSTATUS_OK )
    {
        // Reject for read failure
        teAckType = XB_eACK_STS_NAK;
        teReason = XB_eGRP_FUNC_VALUE_DATA_ACCESS_ERROR;
    }
    // If the concurrent access counter does not match
    else if( ptzCfg->ucConcurAccCtr != tzTimers.ucConcurAccCtr )
    {
        // Reject for CAC
        teAckType = XB_eACK_STS_NAK;
        teReason = XB_eGRP_FUNC_VALUE_CAC;
    }
    // If preliminary configuation check does not pass
    else if( xbtimers_fnCheckTxCfg( ptzCfg->ptzExtra,
                                     ptzCfg->uiExtraCount ) == FALSE )
    {
        // Reject for invalid data
        teAckType = XB_eACK_STS_NAK;
        teReason = XB_eGRP_FUNC_VALUE_DATA_INVALID;
    }
    else
    {
        // If not setting all timers
        if( ptzCfg->uiExtraCount < XB_TIMER_MAX_SEND )
        {
            // If the changing of the per xmit timers fails
            if( xbtimers_fnChangeTxToCfg( &tzTimers,
                                           ptzCfg->ptzExtra,
                                           ptzCfg->uiExtraCount ) == FALSE )
            {
                // Reject with a NAK
                XBMSG_fnSendISOAck( XB_PGN_PER_XMIT_CFG,
                                    XB_eGRP_FUNC_VALUE_DATA_INVALID,
                                    XB_eACK_STS_NAK,
                                    ptzRecv->ucSrcAddr );
                // Quit now
                return;
            }
        }
        else
        {
            // Setting all timers

            // Search received list
            for ( ucIndex = 0; ucIndex < XB_TIMER_MAX_SEND; ucIndex++ )
            {
                // Look for Do Not Change time or Zero time but with a PGN
                if( ( ( ptzCfg->ptzExtra->uiXmitPeriod == PGN_UI_NO_DATA )
                   || ( ptzCfg->ptzExtra->uiXmitPeriod == 0 ) )
                 && ( ptzCfg->ptzExtra->ulParmGrpNum != PGN_PGN_NO_DATA ) )
                {
                    // Reject for invalid data
                    XBMSG_fnSendISOAck( XB_PGN_PER_XMIT_CFG,
                                        XB_eGRP_FUNC_VALUE_DATA_INVALID,
                                        XB_eACK_STS_NAK,
                                        ptzRecv->ucSrcAddr );
                    // Quit now
                    return;
                }
            }

            // Search received list for address claim PGN
            for ( ucIndex = 0; ucIndex < XB_TIMER_MAX_SEND; ucIndex++ )
            {
                // Look for a broadcast address claim with non zero time
                if( ( ptzCfg->ptzExtra->ulParmGrpNum == XB_PGN_ISO_ADDR_CLAIM )
                 && ( ptzCfg->ptzExtra->ucXmitAddr == PGN_ADDR_GLOBAL )
                 && ( ptzCfg->ptzExtra->uiXmitPeriod != 0 ) )
                {
                    // Found a default address claim
                    break;
                }
            }

            // If PGN found before end of list
            if( ucIndex < XB_TIMER_MAX_SEND )
            {
                // Copy the timers configuration
                (void) memcpy( tzTimers.atzTimers,
                               ptzCfg->ptzExtra,
                               sizeof( tzTimers.atzTimers ) );
            }
            else
            {
                // No claim timer found, reject with a NAK
                XBMSG_fnSendISOAck( XB_PGN_PER_XMIT_CFG,
                                    XB_eGRP_FUNC_VALUE_DATA_INVALID,
                                    XB_eACK_STS_NAK,
                                    ptzRecv->ucSrcAddr );
                // Quit now
                return;
            }
        }

        // Increment concurrent access counter
        tzTimers.ucConcurAccCtr++;

        // If saving configuration fails
        if( XBPLATFORM_fnSave( XB_ePARAM_TYPE_TX_TIMEOUTS,
                               ptzCfg->teCfgType,
                               &tzTimers,
                               sizeof( tzTimers ) ) != ( tuiSTATUS )eSTATUS_OK )
        {
            // Reject for write failure
            teAckType = XB_eACK_STS_NAK;
            teReason = XB_eGRP_FUNC_VALUE_DATA_ACCESS_ERROR;
        }
        else
        {
            // Set acceptance
            teAckType = XB_eACK_STS_ACK;
            teReason = XB_eGRP_FUNC_VALUE_DO_NOT_CARE;
        }

            // If the configuration is USER
        if( ptzCfg->teCfgType == XB_eCFG_TYPE_USER )
        {
            // Start/restart timers

            // update the local copy for user send timers
            ( void )memcpy( &tzSendTimers,
                            &tzTimers,
                            sizeof( XB_tzSEND_TIMERS ) );

            // reload all tx timers
            (void)xbtimers_fnLoadTxTimers( &tzSendTimers );
        }
    }

    // Send reply
    XBMSG_fnSendISOAck( XB_PGN_PER_XMIT_CFG,
                        teReason,
                        teAckType,
                        ptzRecv->ucSrcAddr );
}
#endif // defined( HANDLE_RX_TX_PERIOD )

/******************************************************************************

FUNCTION NAME:
    xbtimers_fnSendOneTxToCfg   - Send Tx timers for one configuration type

PURPOSE:
    This function is used to build and send a periodic transmit timer message
    for one configuration type.

INPUTS:
    'teCfgType' is configuration type (User, OEM, Factory) for the message
    'ucMsgCount' is the value for the message count field for the message
    'ptzTimers' is the transmit timers data for the message
    'ucDest' is destination for message send

OUTPUTS:
    none

NOTES:

VERSION HISTORY:

Version 1.00  Date: 08/13/03  By: Dalem
    - Created

******************************************************************************/

static void xbtimers_fnSendOneTxToCfg( XB_teCFG_TYPE teCfgType,
                                       uchar8 ucMsgCount,
                                       XB_tzSEND_TIMERS *ptzTimers,
                                       uchar8 ucDest )
{
    XB_tzPGN_PER_XMIT_CFG tzCfg;

    // Verify parameter
    (void)assert( ptzTimers != NULL );

    // Set the configuration type and message count
    tzCfg.teCfgType = teCfgType;
    tzCfg.ucMsgCount = ucMsgCount;

    // Copy in the concurrent access counter and assign timeout list
    if ( teCfgType == XB_eCFG_TYPE_FACTORY )
    {
        tzCfg.ucConcurAccCtr = PGN_UC_NO_DATA;
    }
    else
    {
        tzCfg.ucConcurAccCtr = ptzTimers->ucConcurAccCtr;
    }
    tzCfg.ptzExtra = ptzTimers->atzTimers;
    tzCfg.uiExtraCount = XB_TIMER_MAX_SEND;

    // Send a Periodic Transmit configuration PGN
    XBMSG_fnSend( XB_PGN_PER_XMIT_CFG,
                  &tzCfg,
                  sizeof(tzCfg),
                  ucDest );
}

/******************************************************************************

FUNCTION NAME:
    XBTIMERS_fnWantPerXmitCfg   - Send Periodic Transmit Configuration

PURPOSE:
    This function is used to build and send a periodic transmit
    configuration message

INPUTS:
    'ucDest' is destination for message send
    'tucSolicited' is TRUE if request is from network

OUTPUTS:
    none

NOTES:

VERSION HISTORY:

Version 1.04  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.05  Date: 08/13/03  By: Dalem
    - SendTimers structure changed

******************************************************************************/

void XBTIMERS_fnWantPerXmitCfg( PGN_tzWANT_DATA *ptzWant )
{
    XB_tzSEND_TIMERS tzTimers;
    uchar8 ucMsgCount;

    // Message count depends on message solicitation
    ucMsgCount = ( ptzWant->tucSolicited ? 1 : 0 );

    // Send the current active timers as USER configuration
    xbtimers_fnSendOneTxToCfg( XB_eCFG_TYPE_USER,
                               ucMsgCount,
                               &tzSendTimers,
                               ptzWant->ucDestAddr );

    // If the requester/destination is a diag
    if( XBMSG_fnIsFromDiag(  ptzWant->ucDestAddr ) != FALSE )
    {
        // Must send OEM and Factory config as well

        // If read of OEM configuration succeeds
        if( XBPLATFORM_fnLoad( XB_ePARAM_TYPE_TX_TIMEOUTS,
                               XB_eCFG_TYPE_OEM,
                               &tzTimers,
                               sizeof( tzTimers ) ) == ( tuiSTATUS )eSTATUS_OK )
        {
            // Send OEM configuration
            xbtimers_fnSendOneTxToCfg( XB_eCFG_TYPE_OEM,
                                       ucMsgCount,
                                       &tzTimers,
                                       ptzWant->ucDestAddr );

        }

        // If read of Factory configuration succeeds
        if( XBPLATFORM_fnLoad( XB_ePARAM_TYPE_TX_TIMEOUTS,
                               XB_eCFG_TYPE_FACTORY,
                               &tzTimers,
                               sizeof( tzTimers ) ) == ( tuiSTATUS )eSTATUS_OK )
        {
            // Send Factory configuration
            xbtimers_fnSendOneTxToCfg( XB_eCFG_TYPE_FACTORY,
                                       ucMsgCount,
                                       &tzTimers,
                                       ptzWant->ucDestAddr );

        }
    }
}

/******************************************************************************

FUNCTION NAME:
    xbtimers_fnCheckRxCfg       - Validate RX timeout configuration

PURPOSE:
    This function is used, when setting timers, to check if timer
    configuration is acceptable.

INPUTS:
    'ptzCfg' is pointer to configuration list
    'uiCount' is number of entries in the list

OUTPUTS:
    TRUE if timer configuration passes tests
    FALSE if timer is not valid

NOTES:

VERSION HISTORY:

Version 1.00  Date: 10/01/03  By: Dalem
    - Created
Version 1.01  Date: 06/25/04  By: Peterd
    - Added check to ensure PGN is a status message
    - PRN 2494
Version 1.02  Date: 07/19/04  By: Peterd
    - Fixed check as field is an enumeration not a collection of bit flags
    - PRN 2494, again

******************************************************************************/
#if defined( HANDLE_RX_TX_PERIOD )
static tucBOOL xbtimers_fnCheckRxCfg( XB_tzPGN_EXTRA_RX_TO_CFG *ptzCfg,
                                      uint16 uiCount )
{
    const PGN_tzPGN_INFO *pktzInfo;
    uchar8 ucIndex;
    uint32 ulPgn;
    uint16 uiTimeout;

    // Verify parameter
    (void)assert( ptzCfg != NULL );

    // For all entries in the configuration
    for ( ucIndex = 0; ucIndex < uiCount; ucIndex++ )
    {
        // Get the PGN and address and period for convenience
        ulPgn = ptzCfg[ ucIndex ].ulParmGrpNum;
        uiTimeout = ptzCfg[ ucIndex ].uiTimeout;

        // If time value is reserved or greater but not Do Not Change
        if( ( uiTimeout >= PGN_UI_RESERVED ) && ( uiTimeout != PGN_UI_NO_DATA ) )
        {
            // Reject this message
            return( FALSE );
        }

        // If PGN if not the wild card
        if( ulPgn != PGN_PGN_NO_DATA )
        {
            // Search list of support for PGNs
            pktzInfo = XBPACK_fnGetPgnInfo( ulPgn,
                                           ( sint16 )XB_eGRP_FUNC_VALUE_DO_NOT_CARE );

            // If PGN not found or PGN is not a Status message
            if( ( pktzInfo == FALSE ) ||
                ( pktzInfo->pktzDesc == FALSE ) ||
                ( pktzInfo->uiPgTypeFlags != PGN_PG_TYPE_RSP ) )
            {
                // PGN is unknown
                return( FALSE );
            }
        }
    }

    // Success
    return( TRUE );
}
#endif // defined( HANDLE_RX_TX_PERIOD )

/******************************************************************************

FUNCTION NAME:
    xbtimers_fnChangeRxToCfg    - Change Rx Timer records

PURPOSE:
    This function is used to change receive timeout configuration records.

INPUTS:
    'ptzTimers' is the transmit timers data
    'ptzNew' is a list of new timeout data
    'uiNewCount' is number of entries in the new timeout list

OUTPUTS:
    TRUE if all new records were updated
    FALSE if some data is invalid and the changes should be aborted

NOTES:

VERSION HISTORY:

Version 1.00  Date: 08/13/03  By: Dalem
    - Created
Version 1.01  Date: 09/11/03  By: Dalem
    - Look for time of zero for idle timer, unused PGN is not 0xFFFFFFFF
Version 1.02 Date: 10/01/03  By: Dalem
    - Some checking moved to common function

******************************************************************************/
#if defined( HANDLE_RX_TX_PERIOD )
static tucBOOL xbtimers_fnChangeRxToCfg( XB_tzRECV_TIMERS *ptzTimers,
                                         XB_tzPGN_EXTRA_RX_TO_CFG *ptzNew,
                                         uint16 uiNewCount )
{
    uint16 uiNew;
    uint16 uiOld;
    uint32 ulPgn;
    uint16 uiTimeout;

    // Verify parameter
    (void)assert( ptzTimers != NULL );
    (void)assert( ptzNew != NULL );

    // For every new received timer
    for ( uiNew = 0; uiNew < uiNewCount; uiNew++ )
    {
        // Get the PGN and timeout for convenience
        ulPgn = ptzNew[ uiNew ].ulParmGrpNum;
        uiTimeout = ptzNew[ uiNew ].uiTimeout;

        // If time value is the Do Not Change value
        if( uiTimeout == PGN_UI_NO_DATA )
        {
            // Ignore this setting
            continue;
        }

        // For every existing timer
        for ( uiOld = 0; uiOld < XB_TIMER_MAX_RECV; uiOld++ )
        {
            // If timers matches in PGN
            if( ptzTimers->atzTimers[ uiOld ].ulParmGrpNum == ulPgn )
            {
                // Record found, copy received timeout value
                ptzTimers->atzTimers[ uiOld ].uiTimeout = uiTimeout;

                // Finished search
                break;
            }
        }

        // If existing record not found
        if( uiOld >= XB_TIMER_MAX_RECV )
        {
            // Search timer list for unused extry
            for ( uiOld = 0; uiOld < XB_TIMER_MAX_RECV; uiOld++ )
            {
                // If timer is not in use
                if( ptzTimers->atzTimers[ uiOld ].uiTimeout == XB_NO_TIME )
                {
                    // Found an empty record
                    ptzTimers->atzTimers[ uiOld ].ulParmGrpNum = ulPgn;
                    ptzTimers->atzTimers[ uiOld ].uiTimeout = uiTimeout;

                    // Finished search
                    break;
                }
            }

            // If free record not found
            if( uiOld >= XB_TIMER_MAX_RECV )
            {
                // Reject this message
                return( FALSE );
            }
        }
    }

    // Success
    return( TRUE );
}
#endif // defined( HANDLE_RX_TX_PERIOD )

/******************************************************************************

FUNCTION NAME:
    XBTIMERS_fnRecvRxToCfg      - Receive an Rx Timeout Configuration

PURPOSE:
    This function handles receiving of a receive message timeout
    configuration message

INPUTS:
    'ptzRecv' is a pointer to the received message work block

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.02  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.03  Date: 07/03/03  By: Dalem
    - Fixed save to load when reading timers
Version 1.04  Date: 08/13/03  By: Dalem
    - RecvTimers structure changed
    - Increased error checking
    - Handle different configuration types
Version 1.05  Date: 08/29/03  By: Dalem
    - Added reason code for rejecting configuration
Version 1.06  Date: 09/03/03  By: Tom Lightfoot
    - Passed reported configurations to the application
Version 1.07 Date: 10/01/03  By: Dalem
    - Check all elements of configurations
Version 1.08 Date: 12/11/03  By: Dalem
    - Source must be Diag
Version 1.09  Date: 06/25/04  By: Peterd
    - Changed call to XBADDR_fnCheckCfgSrc to include Ack Type parameter
    - PRN 2514

******************************************************************************/
#if 0
void XBTIMERS_fnRecvRxToCfg( PGN_tzRECV_DATA *ptzRecv )
{
    XB_tzPGN_RX_TO_CFG  *ptzCfg;
    XB_tzRECV_TIMERS    tzTimers;
    uchar8              ucIndex;
    XB_teACK_STS        teAckType;
    XB_teGRP_FUNC_VALUE teReason;

    // Verify parameter
    (void)assert( ptzRecv != NULL );

    // Set a default response
    teAckType = XB_eACK_STS_NAK;
    teReason = XB_eGRP_FUNC_VALUE_DATA_NOT_AVAILABLE;

    // Convert receive message void pointer to receive timeout config pointer
    ptzCfg = ptzRecv->pvStruct;

    // Is this a requested configuration report?
    if( ptzCfg->ucMsgCount != 0 )
    {
        // Pass the message on to the configuration
        XBPLATFORM_fnRecvXbMsg( ptzRecv );

        // And don't ack/nak
        return;
    }

    // This is one message that can only come from a DIAG
    // Check that the source of the configuration is a DIAG
    if( XBMSG_fnIsFromDiag( ptzRecv->ucSrcAddr ) == FALSE )
    {
        // Reject with an Deny
        teAckType = XB_eACK_STS_DNY;
        teReason = XB_eGRP_FUNC_VALUE_SECURITY;
    }
    // Check the source of the configuration for config type
    else if( XBMSG_fnCheckCfgSrc( ptzCfg->teCfgType,
                               ptzRecv->ucSrcAddr,
                               &teAckType ) == FALSE )
    {
        // Reject with an Deny
        teReason = XB_eGRP_FUNC_VALUE_SECURITY;
    }
    // If message is larger than we can handle
    else if( ptzCfg->uiExtraCount > XB_TIMER_MAX_RECV )
    {
        // Reject for non concurrent access
        teAckType = XB_eACK_STS_NAK;
        teReason = XB_eGRP_FUNC_VALUE_DATA_INVALID;
    }
    // If we are in the wrong mode to handle this configuration
    else if (( XBINIT_pfnAppCfgFilter != NULL )
         && !XBINIT_pfnAppCfgFilter( ptzRecv->ulPgn ) )
    {
        teAckType = XB_eACK_STS_DNY;
        teReason = XB_eGRP_FUNC_VALUE_WRONG_MODE;
    }
    // If loading of configuration fails
    else if( XBPLATFORM_fnLoad( XB_ePARAM_TYPE_RX_TIMEOUTS,
                               ptzCfg->teCfgType,
                               &tzTimers,
                               sizeof( tzTimers ) ) != ( tuiSTATUS )eSTATUS_OK )
    {
        // Reject for read failure
        teAckType = XB_eACK_STS_NAK;
        teReason = XB_eGRP_FUNC_VALUE_DATA_ACCESS_ERROR;
    }
    // If the concurrent access counter does not match
    else if( ptzCfg->ucConcurAccCtr != tzTimers.ucConcurAccCtr )
    {
        // Reject for CAC
        teAckType = XB_eACK_STS_NAK;
        teReason = XB_eGRP_FUNC_VALUE_CAC;
    }
    // If preliminary configuation check does not pass
    else if( xbtimers_fnCheckRxCfg( ptzCfg->ptzExtra,
                                     ptzCfg->uiExtraCount ) == FALSE )
    {
        // Reject for invalid data
        teAckType = XB_eACK_STS_NAK;
        teReason = XB_eGRP_FUNC_VALUE_DATA_INVALID;
    }
    else
    {
        // If not setting all timers
        if( ptzCfg->uiExtraCount < XB_TIMER_MAX_RECV )
        {
            // If the changing of the receive timers fails
            if( xbtimers_fnChangeRxToCfg( &tzTimers,
                                           ptzCfg->ptzExtra,
                                           ptzCfg->uiExtraCount ) == FALSE )
            {
                // Reject with a NAK
                XBMSG_fnSendISOAck( XB_PGN_RX_TO_CFG,
                                    XB_eGRP_FUNC_VALUE_DATA_INVALID,
                                    XB_eACK_STS_NAK,
                                    ptzRecv->ucSrcAddr );
                // Quit now
                return;
            }
        }
        else
        {
            // Search received list for address claim PGN
            for ( ucIndex = 0; ucIndex < XB_TIMER_MAX_RECV; ucIndex++ )
            {
                // Look for a address claim
                if( ptzCfg->ptzExtra->ulParmGrpNum == XB_PGN_ISO_ADDR_CLAIM )
                {
                    // Found a default address claim
                    break;
                }
            }

            // If PGN found before end of list
            if( ucIndex < XB_TIMER_MAX_RECV )
            {
                // Copy the timers configuration
                (void) memcpy( tzTimers.atzTimers,
                               ptzCfg->ptzExtra,
                               sizeof( tzTimers.atzTimers ) );
            }
            else
            {
                // No claim timer found, reject with a NAK
                XBMSG_fnSendISOAck( XB_PGN_RX_TO_CFG,
                                    XB_eGRP_FUNC_VALUE_DATA_INVALID,
                                    XB_eACK_STS_NAK,
                                    ptzRecv->ucSrcAddr );
                // Quit now
                return;
            }
        }

        // Increment concurrent access counter
        tzTimers.ucConcurAccCtr++;

        // If saving configuration fails
        if( XBPLATFORM_fnSave( XB_ePARAM_TYPE_RX_TIMEOUTS,
                               ptzCfg->teCfgType,
                               &tzTimers,
                               sizeof( tzTimers ) ) != ( tuiSTATUS )eSTATUS_OK )
        {
            // Reject for write failure
            teAckType = XB_eACK_STS_NAK;
            teReason = XB_eGRP_FUNC_VALUE_DATA_ACCESS_ERROR;
        }
        else
        {
            // Set acceptance
            teAckType = XB_eACK_STS_ACK;
            teReason = XB_eGRP_FUNC_VALUE_DO_NOT_CARE;

            // If the configuration is USER
            if( ptzCfg->teCfgType == XB_eCFG_TYPE_USER )
            {
                 // Reload the receive timers
                (void) xbtimers_fnLoadRxTimers(&tzTimers);
            }
        }
    }

    // Send reply
    XBMSG_fnSendISOAck( XB_PGN_RX_TO_CFG,
                        teReason,
                        teAckType,
                        ptzRecv->ucSrcAddr );
}
#endif
/******************************************************************************

FUNCTION NAME:
    xbtimers_fnSendOneRxToCfg   - Send Rx timers for one configuration type

PURPOSE:
    This function is used to build and send a receive timeout message for
    one configuration type.

INPUTS:
    'teCfgType' is configuration type (User, OEM, Factory) for the message
    'ucMsgCount' is the value for the message count field for the message
    'ptzTimers' is the receive timeout data for the message
    'ucDest' is destination for message send

OUTPUTS:
    none

NOTES:

VERSION HISTORY:

Version 1.00  Date: 08/13/03  By: Dalem
    - Created

******************************************************************************/

static void xbtimers_fnSendOneRxToCfg( XB_teCFG_TYPE teCfgType,
                                       uchar8 ucMsgCount,
                                       XB_tzRECV_TIMERS *ptzTimers,
                                       uchar8 ucDest )
{
    XB_tzPGN_RX_TO_CFG tzCfg;

    // Verify parameter
    (void)assert( ptzTimers != NULL );

    // Set the configuration type and message count
    tzCfg.teCfgType = teCfgType;
    tzCfg.ucMsgCount = ucMsgCount;

    // Copy in the concurrent access counter and assign timeout list
    if ( teCfgType == XB_eCFG_TYPE_FACTORY )
    {
        tzCfg.ucConcurAccCtr = PGN_UC_NO_DATA;
    }
    else
    {
        tzCfg.ucConcurAccCtr = ptzTimers->ucConcurAccCtr;
    }
    tzCfg.ptzExtra = ptzTimers->atzTimers;
    tzCfg.uiExtraCount = XB_TIMER_MAX_RECV;

    // Send a Receive Timeout configuration PGN
    XBMSG_fnSend( XB_PGN_RX_TO_CFG,
                  &tzCfg,
                  sizeof(tzCfg),
                  ucDest );
}

/******************************************************************************

FUNCTION NAME:
    XBTIMERS_fnSendRxToCfg      - Send Receive Timeout Configuration

PURPOSE:
    This function is used to build and send a receive message timeout
    configuration message

INPUTS:
    'ucDest' is destination for message send
    'tucSolicited' is TRUE if request is from network

OUTPUTS:
    none

NOTES:

VERSION HISTORY:

Version 1.04  Date: 06/24/03  By: Dalem
    - Major change to XB API

******************************************************************************/

void XBTIMERS_fnWantRxToCfg( PGN_tzWANT_DATA *ptzWant )
{
    XB_tzRECV_TIMERS tzTimers;
    uchar8 ucMsgCount;

    // Message count depends on message solicitation
    ucMsgCount = ( ptzWant->tucSolicited ? 1 : 0 );


    // If read of USER configuration succeeds
    if( XBPLATFORM_fnLoad( XB_ePARAM_TYPE_RX_TIMEOUTS,
                           XB_eCFG_TYPE_USER,
                           &tzTimers,
                           sizeof( tzTimers ) ) == ( tuiSTATUS )eSTATUS_OK )
    {
        // Send OEM configuration
        xbtimers_fnSendOneRxToCfg( XB_eCFG_TYPE_USER,
                                   ucMsgCount,
                                   &tzTimers,
                                   ptzWant->ucDestAddr );
    }

    // If the requester/destination is a diag
    if( XBMSG_fnIsFromDiag( ptzWant->ucDestAddr ) != FALSE )
    {
        // Must send OEM and Factory config as well

        // If read of OEM configuration succeeds
        if( XBPLATFORM_fnLoad( XB_ePARAM_TYPE_RX_TIMEOUTS,
                               XB_eCFG_TYPE_OEM,
                               &tzTimers,
                               sizeof( tzTimers ) ) == ( tuiSTATUS )eSTATUS_OK )
        {
            // Send OEM configuration
            xbtimers_fnSendOneRxToCfg( XB_eCFG_TYPE_OEM,
                                       ucMsgCount,
                                       &tzTimers,
                                       ptzWant->ucDestAddr );
        }

        // If read of Factory configuration succeeds
        if( XBPLATFORM_fnLoad( XB_ePARAM_TYPE_RX_TIMEOUTS,
                               XB_eCFG_TYPE_FACTORY,
                               &tzTimers,
                               sizeof( tzTimers ) ) == ( tuiSTATUS )eSTATUS_OK )
        {
            // Send Factory configuration
            xbtimers_fnSendOneRxToCfg( XB_eCFG_TYPE_FACTORY,
                                       ucMsgCount,
                                       &tzTimers,
                                       ptzWant->ucDestAddr );
        }
    }
}

/******************************************************************************

FUNCTION NAME:
    XBTIMERS_fnResetTxToCfg     - Handle a reset config command for Tx Timers

PURPOSE:
    This function is used to manage a command to reset the periodic transmit
    timer configuration.

INPUTS:
    'teCmd' is the type of the configuration reset command

OUTPUTS:
    TRUE if successful
    FALSE if error occurred

NOTES:

VERSION HISTORY:

Version 1.00  Date: 08/14/03  By: Dalem
    - Created

******************************************************************************/

tucBOOL XBTIMERS_fnResetTxToCfg( XB_teRST_CMD teCmd )
{
    XB_tzSEND_TIMERS tzTimers;
    uchar8 ucUserCac;
    uchar8 ucOemCac = 0;

    // Read the concurrent access counters from present configurations
    switch( teCmd )
    {
        case XB_eRST_CMD_FACTORY:
        {
            // If reading of OEM configuration fails
            if( XBPLATFORM_fnLoad( XB_ePARAM_TYPE_TX_TIMEOUTS,
                                   XB_eCFG_TYPE_OEM,
                                   &tzTimers,
                                   sizeof( tzTimers ) ) != ( tuiSTATUS )eSTATUS_OK )
            {
                // Configuration does not exist
                ucOemCac = 0;
            }
            else
            {
                // Save Access counter for OEM
                ucOemCac = tzTimers.ucConcurAccCtr;
            }
        }
        //lint -fallthrough
        // Fall through to OEM command
        case XB_eRST_CMD_OEM:
        {
            // If reading of User configuration fails
            if( XBPLATFORM_fnLoad( XB_ePARAM_TYPE_TX_TIMEOUTS,
                                   XB_eCFG_TYPE_USER,
                                   &tzTimers,
                                   sizeof( tzTimers ) ) != ( tuiSTATUS )eSTATUS_OK )
            {
                // Configuration does not exist
                ucUserCac = 0;
            }
            else
            {
                // Save Access counter for user
                ucUserCac = tzTimers.ucConcurAccCtr;
            }
        }
        break;

        case XB_eRST_CMD_REBOOT:
        case XB_eRST_CMD_REBOOT_DL_MODE:
        case XB_eRST_CMD_CLEAR_NV_AND_REBOOT:
        case XB_eRST_CMD_NO_CHANGE:
        default:
            // Failure
            return( FALSE );
    }

    // Determine which configuration we will use
    switch( teCmd )
    {
        case XB_eRST_CMD_FACTORY:
        {
            // If read of Factory configuration fails
            if( XBPLATFORM_fnLoad( XB_ePARAM_TYPE_TX_TIMEOUTS,
                                   XB_eCFG_TYPE_FACTORY,
                                   &tzTimers,
                                   sizeof( tzTimers ) ) != ( tuiSTATUS )eSTATUS_OK )
            {
                // Failure
                return( FALSE );
            }
        }
        break;

        case XB_eRST_CMD_OEM:
        {
            // If read of OEM configuration fails
            if( XBPLATFORM_fnLoad( XB_ePARAM_TYPE_TX_TIMEOUTS,
                                   XB_eCFG_TYPE_OEM,
                                   &tzTimers,
                                   sizeof( tzTimers ) ) != ( tuiSTATUS )eSTATUS_OK )
            {
                // Failure
                return( FALSE );
            }
        }
        break;

        case XB_eRST_CMD_REBOOT:
        case XB_eRST_CMD_REBOOT_DL_MODE:
        case XB_eRST_CMD_CLEAR_NV_AND_REBOOT:
        case XB_eRST_CMD_NO_CHANGE:
        default:
            // Failure
            return( FALSE );
    }

    // Increment access counters and save configurations
    switch( teCmd )
    {
        case XB_eRST_CMD_FACTORY:
        {
            // Increment access counter for OEM
            tzTimers.ucConcurAccCtr = ucOemCac + 1;

            // If writing of OEM configuration fails
            if( XBPLATFORM_fnSave( XB_ePARAM_TYPE_TX_TIMEOUTS,
                                   XB_eCFG_TYPE_OEM,
                                   &tzTimers,
                                   sizeof( tzTimers ) ) != ( tuiSTATUS )eSTATUS_OK )
            {
                // Failure
                return( FALSE );
            }
        }
        //lint -fallthrough
        // Fall through to OEM
        case XB_eRST_CMD_OEM:
        {
            // Increment access counter for User
            tzTimers.ucConcurAccCtr = ucUserCac + 1;

            // If writing of OEM configuration fails
            if( XBPLATFORM_fnSave( XB_ePARAM_TYPE_TX_TIMEOUTS,
                                   XB_eCFG_TYPE_USER,
                                   &tzTimers,
                                   sizeof( tzTimers ) ) != ( tuiSTATUS )eSTATUS_OK )
            {
                // Failure
                return( FALSE );
            }
        }
        break;

        case XB_eRST_CMD_REBOOT:
        case XB_eRST_CMD_REBOOT_DL_MODE:
        case XB_eRST_CMD_CLEAR_NV_AND_REBOOT:
        case XB_eRST_CMD_NO_CHANGE:
        default:
            // Failure
            return( FALSE );
    }

    // update the local copy
    ( void )memcpy( &tzSendTimers,
                    &tzTimers,
                    sizeof( XB_tzSEND_TIMERS ) );

    // If Reloading of timers fails
    if( xbtimers_fnLoadTxTimers( &tzTimers ) == FALSE )
    {
        // Failure
        return( FALSE );
    }
    else
    {
         // Success
         return( TRUE );
    }
}

/******************************************************************************

FUNCTION NAME:
    XBTIMERS_fnResetRxToCfg     - Handle a reset config command for Rx Timers

PURPOSE:
    This function is used to manage a command to reset the receive
    timeout configuration.

INPUTS:
    'teCmd' is the type of the configuration reset command

OUTPUTS:
    TRUE if successful
    FALSE if error occurred

NOTES:

VERSION HISTORY:

Version 1.00  Date: 08/14/03  By: Dalem
    - Created

******************************************************************************/

tucBOOL XBTIMERS_fnResetRxToCfg( XB_teRST_CMD teCmd )
{
    XB_tzRECV_TIMERS tzTimers;
    uchar8 ucUserCac;
    uchar8 ucOemCac = 0;

    // Read the concurrent access counters from present configurations
    switch( teCmd )
    {
        case XB_eRST_CMD_FACTORY:
        {
            // If reading of OEM configuration fails
            if( XBPLATFORM_fnLoad( XB_ePARAM_TYPE_RX_TIMEOUTS,
                                   XB_eCFG_TYPE_OEM,
                                   &tzTimers,
                                   sizeof( tzTimers ) ) != ( tuiSTATUS )eSTATUS_OK )
            {
                // Configuration does not exist
                ucOemCac = 0;
            }
            else
            {
                // Save Access counter for OEM
                ucOemCac = tzTimers.ucConcurAccCtr;
            }
        }
        //lint -fallthrough
        // Fall through to OEM command
        case XB_eRST_CMD_OEM:
        {
            // If reading of User configuration fails
            if( XBPLATFORM_fnLoad( XB_ePARAM_TYPE_RX_TIMEOUTS,
                                   XB_eCFG_TYPE_USER,
                                   &tzTimers,
                                   sizeof( tzTimers ) ) != ( tuiSTATUS )eSTATUS_OK )
            {
                // Configuration does not exist
                ucUserCac = 0;
            }
            else
            {
                // Save Access counter for user
                ucUserCac = tzTimers.ucConcurAccCtr;
            }
        }
        break;

        case XB_eRST_CMD_REBOOT:
        case XB_eRST_CMD_REBOOT_DL_MODE:
        case XB_eRST_CMD_CLEAR_NV_AND_REBOOT:
        case XB_eRST_CMD_NO_CHANGE:
        default:
            // Failure
            return( FALSE );
    }

    // Determine which configuration we will use
    switch( teCmd )
    {
        case XB_eRST_CMD_FACTORY:
        {
            // If read of Factory configuration fails
            if( XBPLATFORM_fnLoad( XB_ePARAM_TYPE_RX_TIMEOUTS,
                                   XB_eCFG_TYPE_FACTORY,
                                   &tzTimers,
                                   sizeof( tzTimers ) ) != ( tuiSTATUS )eSTATUS_OK )
            {
                // Failure
                return( FALSE );
            }
        }
        break;

        case XB_eRST_CMD_OEM:
        {
            // If read of OEM configuration fails
            if( XBPLATFORM_fnLoad( XB_ePARAM_TYPE_RX_TIMEOUTS,
                                   XB_eCFG_TYPE_OEM,
                                   &tzTimers,
                                   sizeof( tzTimers ) ) != ( tuiSTATUS )eSTATUS_OK )
            {
                // Failure
                return( FALSE );
            }
        }
        break;

        case XB_eRST_CMD_REBOOT:
        case XB_eRST_CMD_REBOOT_DL_MODE:
        case XB_eRST_CMD_CLEAR_NV_AND_REBOOT:
        case XB_eRST_CMD_NO_CHANGE:
        default:
            // Failure
            return( FALSE );
    }

    // Increment access counters and save configurations
    switch( teCmd )
    {
        case XB_eRST_CMD_FACTORY:
        {
            // Increment access counter for OEM
            tzTimers.ucConcurAccCtr = ucOemCac + 1;

            // If writing of OEM configuration fails
            if( XBPLATFORM_fnSave( XB_ePARAM_TYPE_RX_TIMEOUTS,
                                   XB_eCFG_TYPE_OEM,
                                   &tzTimers,
                                   sizeof( tzTimers ) ) != ( tuiSTATUS )eSTATUS_OK )
            {
                // Failure
                return( FALSE );
            }
        }
        //lint -fallthrough
        // Fall through to OEM
        case XB_eRST_CMD_OEM:
        {
            // Increment access counter for User
            tzTimers.ucConcurAccCtr = ucUserCac + 1;

            // If writing of OEM configuration fails
            if( XBPLATFORM_fnSave( XB_ePARAM_TYPE_RX_TIMEOUTS,
                                   XB_eCFG_TYPE_USER,
                                   &tzTimers,
                                   sizeof( tzTimers ) ) != ( tuiSTATUS )eSTATUS_OK )
            {
                // Failure
                return( FALSE );
            }
        }
        break;

        case XB_eRST_CMD_REBOOT:
        case XB_eRST_CMD_REBOOT_DL_MODE:
        case XB_eRST_CMD_CLEAR_NV_AND_REBOOT:
        case XB_eRST_CMD_NO_CHANGE:
        default:
            // Failure
            return( FALSE );
    }

    // If Reloading of timers fails
    if( xbtimers_fnLoadRxTimers( &tzTimers ) == FALSE )
    {
        // Failure
        return( FALSE );
    }
    else
    {
         // Success
         return( TRUE );
    }
}

