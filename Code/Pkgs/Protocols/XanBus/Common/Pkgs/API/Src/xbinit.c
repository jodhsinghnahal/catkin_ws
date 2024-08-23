/******************************************************************************

  (c) 2003 - 2022 Xantrex Canada ULC. All rights reserved.
  All trademarks are owned or licensed by Xantrex Canada ULC,
  its subsidiaries or affiliated companies.

****************************************************************************//**

  @addtogroup xbinit XBINIT
  @{

    @brief Xanbus Initialization

****************************************************************************//**

  @page xbinit_design XanBus Initialization

    Implements the XanBus initialization API.

  @par API Functions
    XBINIT_fnInit               - Initialize XB module
    XBINIT_fnNetCtrl            - Control operation if XanBus
    XBINIT_fnSleepCtrl          - Set or clear CAN sleep state
    XBINIT_fnSaveError          - save last error from Teleflex or xbapi code
    XBINIT_fnGetErrorNum        - Get last unreported error as a number
    XBINIT_fnResetCfg           - Handle reset configurations request
    XBINIT_fnTick               - Periodically check if Teleflex stack can Tx/Rx
    XBINIT_fnCSec2Ticks         - Convert centiseconds to ticks
    XBINIT_fnStartProTimer      - Start protocol timer
    XBINIT_fnStopProTimer       - Stop protocol timer
    XBINIT_fnGetVersion         - Get XanBus library version

  @par Local Functions
    xbinit_fnWakeupCb       - CAN wakeup interrupt callback
    xbinit_fnSleepTest      - Test if CAN should be sleeping


*******************************************************************************/


/*==============================================================================
                              Includes
==============================================================================*/

// XanBus include files
#include "xbldefs.h"    // Local Definitions
#include "xbmem.h"      // memory allocation
#include "canbuf.h"

// System include files
#include "xassert.h"    // Debugging


/*==============================================================================
                              Defines
==============================================================================*/

#define XBINIT_SHOW_MAX     ( 10 )      // Ticks for show lamp off time

#define XBINIT_SLEEP_MODE   ( 0x01 )    // CAN is supposed to be asleep
#define XBINIT_AWAKE        ( 0x02 )    // CAN has been woken up
#define XBINIT_SNOOZING     ( 0x04 )    // CAN is trying to get back to sleep
#define XBINIT_WAIT_SLEEP   ( 0x08 )    // Empty send queue before sleeping
#define XBINIT_SUSPEND      ( 0x10 )    // Suspend periodic transmit
#define XBINIT_HEAP_MIN     ( 128 )     // Minimum size for heap

/*==============================================================================
                           Global Variables
==============================================================================*/

// Application level filter for received configurations (disabled by default)
XB_pfnCONTEXT_TEST XBINIT_pfnAppCfgFilter = NULL;

/*==============================================================================
                           Local/Private Variables
==============================================================================*/

static uchar8 ucSleepFlags;         // Flags for controlling sleep mode

static PGN_teESOURCE uiErrorSource;        // Save Error Source
static uint16 uiErrorParam;         // Save Error Parameter

static uint16 uiMsPerTick;          // Number of milliseconds per tick
static uint16 uiProTicks;           // Time counter for protocol timer
static void (*pfnProFunc)(void);    // Function pointer for protocol timer

static void (*pfnSleepCb)(void);    // Callback after putting CAN to sleep

/*==============================================================================
                              Forward declarations
==============================================================================*/

static void xbinit_fnWakeupCb( uchar8 ucChannel );
tucBOOL xbinit_fnSleepTest( void );

/*==============================================================================
                           Function Definitions
==============================================================================*/

/******************************************************************************

FUNCTION NAME:
    XBINIT_fnInit               - Initialize XB module

PURPOSE:
    Initialize the XB package and packages that it relies on (TeleFlex
    and the XanBus memory manager)

INPUTS:
    'ucTicksPerSec' is rate in ticks per second tick routine will be called
    'pvHeapStart' points to the start of the XanBus heap
    'uiHeapSize' is the number of bytes in the XanBus heap

OUTPUTS:
    TRUE if successful,
    FALSE if some step of initialization failed.

NOTES:

VERSION HISTORY:

Version 1.03  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.04  Date: 10/15/03  By: Dalem
    - Added installing of CAN wakeup interrupt
Version 1.05  Date: 15/12/03  By: Tom Lightfoot
    - Added parameters for the XanBus heap

******************************************************************************/

tucBOOL XBINIT_fnInit( uchar8 ucPort,
                       uchar8 ucBitRate,
                       uchar8 ucTicksPerSec )
{
    TFXCAN_RETURNS TFX_teResult;
    uchar8 aucName[ XB_NAME_LEN ];

    // Initialize teleflex library
    TFX_teResult = TFXLibInit( ucPort,
                               ucBitRate );
    if( TFX_teResult != TFXCR_OK )
    {
        // Save error
        XBINIT_fnSaveError( PGN_eESRC_TFX_LIB_INIT, ( uint16 )TFX_teResult );

        // Failure
        return( FALSE );
    }

    // Tell teleflex how many ticks per second it will get
    TFX_teResult = TFXLibTimeBase( ( uint32 )ucTicksPerSec );
    if( TFX_teResult != TFXCR_OK )
    {
        // Save error
        XBINIT_fnSaveError( PGN_eESRC_TFX_SET_TBASE, ( uint16 )TFX_teResult );

        // Failure
        return( FALSE );
    }

    // Install callback for handling CAN wakeup interrupt
    if( TFXDRV_fnInstallWakeupCb( XB_PORT_A,
                            xbinit_fnWakeupCb ) != TFXCR_OK )
    {
        // Save error
        XBINIT_fnSaveError( PGN_eESRC_TFX_LIB_INIT, ( uint16 )PGN_eERR_NOT_SET );

        // Failure
        return( FALSE );
    }

    // Save number of milliseconds per tick for timers
    uiMsPerTick = (uint16) ( 1000 / ucTicksPerSec );

    // Get name for random number seed
    (void) XBPLATFORM_fnLoad( XB_ePARAM_TYPE_NAME,
                              XB_eCFG_TYPE_USER,
                              aucName,
                              XB_NAME_LEN );

    // Initialize message data structures
    if( XBMSG_fnInit() == FALSE )
    {
        // Failure
        return( FALSE );
    }

    // Start with sleeping not sleeping
    ucSleepFlags = 0;

    // Start with identify indicator off and network indicator on
    XBPLATFORM_fnSetNetIndic( XB_eNETWORK_STATE_ERROR_ACTIVE, ON );

    // Application level filtering turned off by default
    XBINIT_pfnAppCfgFilter = NULL;

    // Completed successfully
    return( TRUE );
}

/******************************************************************************

FUNCTION NAME:
    XBINIT_fnNetCtrl            - Control operation of XanBus

PURPOSE:
    This function can be used from an application to control the activity
    of the XanBus API. Suspend will stop periodic timers so that the
    Candriver can be put to sleep. Resume will reenable periodic timers.

INPUTS:
    'teControl' is control mode for XanBus

OUTPUTS:
    Nothing


NOTES:

VERSION HISTORY:

Version 1.00  Date: 09/19/03  By: Dalem
    - Created
Version 1.01  Date: 10/15/03  By: Dalem
    - Use one bit of sleep flags to conserve memory

******************************************************************************/

void XBINIT_fnNetCtrl( XB_teNET_CTRL teControl )
{
    switch( teControl )
    {
        case XB_eNET_CTRL_POWERUP:
        {
            // Start address claiming
            (void) XBADDR_fnBeginClaiming();

            // Enable periodic transmit
            ucSleepFlags &= ~XBINIT_SUSPEND;
        }
        break;

        case XB_eNET_CTRL_SUSPEND:
        {
            // Suspend periodic transmit
            ucSleepFlags |= XBINIT_SUSPEND;
        }
        break;

        case XB_eNET_CTRL_RESUME:
        {
            // Resume periodic transmit
            ucSleepFlags &= ~XBINIT_SUSPEND;
        }
        break;

        default:
        {
            // Ignore
            (void)assert( FALSE );
        }
        break;
    }
}

/******************************************************************************

FUNCTION NAME:
    XBINIT_fnChkAsleep            - Check if XanBus Asleep

PURPOSE:
    This function is used by other XanBus code to determine if it is asleep.

INPUTS:
    Nothing

OUTPUTS:
    Nothing


NOTES:

VERSION HISTORY:

Version 1.00  Date: 04/14/04  By: Peterd
    - Created

******************************************************************************/

tucBOOL XBINIT_fnChkAsleep( void )
{
    return( ( ucSleepFlags & XBINIT_SUSPEND ) != 0 ? TRUE : FALSE );
}

/******************************************************************************

FUNCTION NAME:
    XBINIT_fnSleepCtrl            - Set or clear CAN sleep state

PURPOSE:
    This function is used control the sleep state of the CAN interface.

INPUTS:
    'ucSleep' is TRUE if the can driver should be sleeping
              or FALSE if the CAN should be awake
    'callback' is function to call when sleep starts

OUTPUTS:
    Nothing


NOTES:

VERSION HISTORY:

Version 1.00  Date: 10/14/03  By: Dalem
    - Created
Version 1.01  Date: 04/15/04  By: Peterd
    - Added call to wake up the CAN driver

******************************************************************************/

void XBINIT_fnSleepCtrl( tucBOOL ucSleep,
                         XB_tfnSLEEP_CALLBACK fnCallback )
{
    // If we are going to sleep
    if( ucSleep != FALSE )
    {
        // Make sure network is suspended
        XBINIT_fnNetCtrl( XB_eNET_CTRL_SUSPEND );

        // Set the sleep flag
        ucSleepFlags |= ( XBINIT_SLEEP_MODE | XBINIT_WAIT_SLEEP );

        // Save sleeping fnCallback function
        pfnSleepCb = fnCallback;
    }
    else
    {
        // Make sure network is not suspended
        XBINIT_fnNetCtrl( XB_eNET_CTRL_RESUME );

        // Take CAN controller out of sleep
        ( void )TFXDRV_fnWakeUp( XB_PORT_A );

        // Clear the sleep flags
        ucSleepFlags = 0;
    }
}

/******************************************************************************

FUNCTION NAME:
    xbinit_fnWakeupCb - CAN wakeup interrupt fnCallback

PURPOSE:
    This function is intended to be installed for a fnCallback from the
    CAN wakeup interrupt. As such this function runs at interrupt time
    and needs to limit what it does.

INPUTS:
    'ucChannel' is the channel number of the waking channel

OUTPUTS:
    Nothing

NOTES:
    This fnCallback is called at interrupt time so be careful when modifying.

VERSION HISTORY:

Version 1.00  Date: 10/14/03  By: Dalem
    - Created

******************************************************************************/

static void xbinit_fnWakeupCb( uchar8 ucChannel )
{
    if( ucChannel == XB_PORT_A )
    {
        ucSleepFlags |= XBINIT_AWAKE;
    }
}

/******************************************************************************

FUNCTION NAME:
    XBINIT_fnSaveError          - Save last error from Teleflex or xbapi code

PURPOSE:
    This function is used to save occurance data for errors that cannot be
    otherwise handled such as errors occuring during timer tick handling.

INPUTS:
    'uiSource' is enumeration for the source of the error
    'uiParam' is extra parameter for further defining error

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.01  Date: 06/24/03  By: Dalem
    - Major change to XB API

******************************************************************************/

void XBINIT_fnSaveError( PGN_teESOURCE uiSource, uint16 uiParam )
{
    uiErrorSource = uiSource;
    uiErrorParam = uiParam;
}

/******************************************************************************

FUNCTION NAME:
    XBINIT_fnGetErrorNum        - Get last unreported error as a number

PURPOSE:
    This function is used to get the latest unreported error code number.
    The error is then cleared.

INPUTS:
    None

OUTPUTS:
    Returns error code
    FALSE if no error was stored

NOTES:

VERSION HISTORY:

Version 1.00  Date: 07/08/03  By: Dalem
    - Created

******************************************************************************/

uint16 XBINIT_fnGetErrorNum( void )
{
    uint16 uiErrNum;

    // Save the error code
    uiErrNum = uiErrorParam;

    // Clear the error
    uiErrorParam = 0;

    // Return the saved error
    return( uiErrNum );
}

/******************************************************************************

FUNCTION NAME:
    XBINIT_fnGetErrorSrc       - Get last unreported error's source

PURPOSE:
    This function is used to get the source of the latest unreported error

INPUTS:
    None

OUTPUTS:
    Returns source code of the error
    FALSE if no error was stored

NOTES:

VERSION HISTORY:

Version 1.00  Date: 11/16/05  By: Hollyz
    - Created

******************************************************************************/

uint16 XBINIT_fnGetErrorSrc( void )
{
    uint16 uiErrSrc;

    // Save the error source
    uiErrSrc = uiErrorSource;

    // Clear the error
    uiErrorSource = PGN_eESRC_NONE;

    // Return the saved error source
    return( uiErrSrc );
}

/******************************************************************************

FUNCTION NAME:
    XBINIT_fnResetCfg           - Handle reset configurations request

PURPOSE:
    This function is used to reset configurations that XanBus maintains

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

tucBOOL XBINIT_fnResetCfg( XB_teRST_CMD teCmd )
{
    if( ( XBTIMERS_fnResetTxToCfg( teCmd ) != FALSE )
     && ( XBTIMERS_fnResetRxToCfg( teCmd ) != FALSE )
     && ( XBADDR_fnResetNameCfg( teCmd ) ) != FALSE )
    {
        // Success
        return( TRUE );
    }

    // Failure
    return( FALSE );
}

/******************************************************************************

FUNCTION NAME:
    xbinit_fnSleepTest              - Test if CAN should be sleeping

PURPOSE:
    This function is used to check the sleep control flags and sleep the
    CAN controller if necessary.

INPUTS:
    NONE

OUTPUTS:
    TRUE if CAN is put to sleep
    FALSE if CAN sleep state is not chnaged

NOTES:
    The sleep control flags:
        XBINIT_SLEEP_MODE is set if the node is supposed to be asleep. It may
            not be asleep yet because of other flags.
        XBINIT_WAIT_SLEEP is set to allow the node to empty the transmit queue
            before trying to sleep
        XBINIT_AWAKE flag is set if the CAN controller wakes up. The node
            will process the receive message list to see if the network
            has sent a wakeup message. If there is no wakeup message the
            sets the snooze flag to node more time to find a wakeup message.
        XBINIT_SNOOZING is set to allow the node to stay awake for one extra
            tick to look for wakeup command message. If the node does not
            get a wakeup command the CAN controller is put to sleep. If the
            CAN controller is put to sleep the sleep fnCallback function is
            called (if there is one) so an application can know and put
            the processor to sleep. Everytime the CAN controller is put to
            sleep the fnCallback is called so that if the CAN controller wakes
            up the node erroniouly the processor can be put back to sleep.

        If the CAN controller wakes up but the network does not send a wakeup
        message the node will try to go back to sleep.

VERSION HISTORY:

Version 1.00  Date: 10/15/03  By: Dalem
    - Created
Version 1.01  Date: 03/08/043  By: Peterd
    - Added check for NULL callback

******************************************************************************/

tucBOOL xbinit_fnSleepTest( void )
{
    // If we are supposed to be asleep
    if( ( ucSleepFlags & XBINIT_SLEEP_MODE ) != 0 )
    {
        // If waiting for sleep
        if( ( ucSleepFlags & XBINIT_WAIT_SLEEP ) != 0 )
        {
            // If queue empty
            if(( DOBHLDFirst() == NULL ) && ( searchFirstVipDOB() == NULL ))
            {
                // Clear waiting for sleep flag
                ucSleepFlags &= ~XBINIT_WAIT_SLEEP;

                // Set snooze flag to allow more time for network to settle
                ucSleepFlags |= XBINIT_SNOOZING;
            }
        }
        // If awake flag set
        else if( ( ucSleepFlags & XBINIT_AWAKE ) != 0 )
        {
            // Set the snooze flag to stay awake for another tick
            ucSleepFlags |= XBINIT_SNOOZING;

            // Clear the awake flag
            ucSleepFlags &= ~XBINIT_AWAKE;
        }
        else
        {
            // If snooze flag set
            if( ( ucSleepFlags & XBINIT_SNOOZING ) != 0 )
            {
                // Clear snooze flag
                ucSleepFlags &= ~XBINIT_SNOOZING;

                // Sleep the CAN controller
                (void) TFXDRV_fnSleep( XB_PORT_A,
                                       TRUE );

                if ( pfnSleepCb != NULL )
                {
                    // Call back
                    pfnSleepCb();
                }

                // CAN was put to sleep
                return( TRUE );
            }
        }
    }

    // CAN sleep state was not changed
    return( FALSE );
}

/******************************************************************************

FUNCTION NAME:
    XBINIT_fnTick - Periodically check if Teleflex stack can send or receive

PURPOSE:
    This function must be called periodically to receive and/or send
    PG messages. This function is also used to blink the network status
    indicator. This function should not be called from a timer interrupt
    because it could do significant processing depending on the configuration
    of the underlying TeleFlex library.

INPUTS:
    None

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.04  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.05  Date: 10/15/03  By: Dalem
    - Changed show timer data to conserve memory
    - Added call to check for sleep mode

******************************************************************************/

void XBINIT_fnTick( void )
{
    TFXCAN_RETURNS TFX_teResult;
    XB_teNETWORK_STATE teBusState;
    CAN_tzCOMM_STATS tzStats;
    uint32 ulTxCount;
    static uchar8 ucShowTimer = 0;

    // If indicator timer is active
    if( ucShowTimer > 0 )
    {
        // Count it down
        ucShowTimer--;
    }

    // If protocol timer is active
    if( uiProTicks != 0 )
    {
        // If protocol timer has timed out
        if( --uiProTicks == 0 )
        {
            // Call protocol timer fnCallback function
            (*pfnProFunc)();
        }
    }

    TFXDRV_fnStatusTick( XB_PORT_A );

    // If timers are enabled
    if( ( ucSleepFlags & XBINIT_SUSPEND ) == 0 )
    {
        // Call XanBus timers tick processing
        XBTIMERS_fnTickTimers();
    }

    // Call Teleflex tick processing -- input
    TFX_teResult = TFXLibTickInput( XB_PORT_A );
    if( TFX_teResult != TFXCR_OK )
    {
        // Save error
        XBINIT_fnSaveError( PGN_eESRC_TFX_IN_TICK_A, ( uint16 )TFX_teResult );
    }

    // Get count of frames that have been sent
    if( TFXDRV_fnGetCommStats( XB_PORT_A, &tzStats ) == TFXCR_OK )
    {
        // Save count of sent frames
        ulTxCount = tzStats.ulTxExtFrames;
    }
    else
    {
        // Error
        ulTxCount = PGN_UL_NO_DATA;
    }

    // Call Teleflex tick processing -- output
    TFX_teResult = TFXLibTickOutput( XB_PORT_A );
    if( TFX_teResult != TFXCR_OK )
    {
        // Save error
        XBINIT_fnSaveError( PGN_eESRC_TFX_OUT_TICK_A, ( uint16 )TFX_teResult );
    }

    // Get count of frames that have been sent now
    if( TFXDRV_fnGetCommStats( XB_PORT_A, &tzStats ) == TFXCR_OK )
    {
        // If count is different frames were sent
        if( ulTxCount != tzStats.ulTxExtFrames )
        {
            // Message were sent, can we restart the show timer yet
            if( ucShowTimer == 0 )
            {
                // Restart the show timer
                ucShowTimer = XBINIT_SHOW_MAX * 2;
            }
        }
    }

    // Get controller state for port A and set condition code
    teBusState = XBMSG_fnGetBusState();

    // Process the sleep flags
    if( xbinit_fnSleepTest() != FALSE )
    {
        // Clear the show timer
        ucShowTimer = 0;
    }


    // Set the network indication, state depends on show timer
    // Ensure state of indicator is on for at least as long as it is off
    XBPLATFORM_fnSetNetIndic( teBusState,
                              ( tucBOOL )( ucShowTimer < XBINIT_SHOW_MAX ) );
}

/******************************************************************************

FUNCTION NAME:
    XBINIT_fnCSec2Ticks         - Convert centiseconds to ticks

PURPOSE:
    This function is used to convert centiseconds to tick for the
    count down timers.  Some PGNs use centiseconds for time periods

INPUTS:
    'ulCentiSecs' is time period in centiseconds

OUTPUTS:
    number of timer ticks required for the given period in centiseconds

NOTES:

VERSION HISTORY:

Version 1.01  Date: 06/24/03  By: Dalem
    - Major change to XB API

******************************************************************************/

uint16 XBINIT_fnCSec2Ticks( uint32 ulCentiSecs )
{
    // Convert centiseconds, that is number of 1/100 seconds, to ticks, rounding upwards
    return (uint16)( ( ulCentiSecs * 10 + uiMsPerTick - 1 ) / uiMsPerTick );
}

/******************************************************************************

FUNCTION NAME:
    XBINIT_fnStartProTimer      - Start protocol timer

PURPOSE:
    This function is used to assign a fnCallback function for the protcol
    timer and start the timer ticking.

INPUTS:
    'uiCentiSec' is timeout period in centi-seconds
    'pfunc' is function to call when time expires

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.02  Date: 06/24/03  By: Dalem
    - Major change to XB API

******************************************************************************/

void XBINIT_fnStartProTimer( uint16 uiCentiSec, XB_pfnPROTOCOL_TO pfunc )
{
    // Verify parameter
    (void)assert( pfunc != NULL );

    // Timer does not work if time is zero
    if( uiCentiSec == 0 )
    {
        // Set number of ticks to smallest possible
        uiProTicks = 1;
    }
    else
    {
        // Set number of ticks remaining
        uiProTicks = XBINIT_fnCSec2Ticks( ( uint32 )uiCentiSec );
    }

    // Set function to be called
    pfnProFunc = pfunc;
}

/******************************************************************************

FUNCTION NAME:
    XBINIT_fnStopProTimer       - Stop protocol timer

PURPOSE:
    This function is used to clear the protocol timer before it goes off

INPUTS:
    None

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.01  Date: 06/24/03  By: Dalem
    - Major change to XB API

******************************************************************************/

void XBINIT_fnStopProTimer( void )
{
    // Clear number of tick remaining;
    uiProTicks = 0;

    // Clear function to be called
    pfnProFunc = NULL;

}


/******************************************************************************

FUNCTION NAME:
    XBINIT_fnGetVersion       - Get XanBus library version

PURPOSE:
    This function is used to get the version of the library

INPUTS:
    None

OUTPUTS:
    pulVer - XanBus lib version in SwVerSts format (thanks RonM)
    puiBuild - XanBus lib build number (in case i screw it up the first time)

NOTES:

VERSION HISTORY:

Version 1.00  Date: 04/11/26  By: HenryW
    - init

******************************************************************************/

void XBINIT_fnGetVersion( uint32 *pulVer, uint16 *puiBuild )
{
    *pulVer = RVC_VER;
    *puiBuild = RVC_BUILD;
}

/******************************************************************************

FUNCTION NAME:
    XBINIT_fnSetAppCfgFilter

PURPOSE:
    This function is used to set the application hook for accepting
    a XanBus configuration PGN based on the state of the device.
    If this function is not called, then the application hook remains
    disabled

INPUTS:
    pfnFilter - pointer to filter function.  The function takes the PGN
                as a parameter and returns TRUE if it is okay to accept
                the configuration at this time, FALSE otherwise.

OUTPUTS:
    None

NOTES:

VERSION HISTORY:

Version 1.00  Date: 2006-12-07  By: Tom Lightfoot
    - init

******************************************************************************/

void XBINIT_fnSetAppCfgFilter( XB_pfnCONTEXT_TEST pfnFilter )
{
    XBINIT_pfnAppCfgFilter = pfnFilter;
}


//*****************************************************************************
// End of file.
//! @}
//*****************************************************************************
