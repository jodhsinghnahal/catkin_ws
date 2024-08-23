/*=============================================================================
Copyright 2003 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:  cdrvapi.c

PURPOSE:
    This source file is an intermediate layer within the XanBus driver
    that resides between the low level HAL CAN driver and the rest of the
    driver code.  This layer connects the low level hardware driver with
    buffering and handle management.  It is meant to be non-target
    specific.

FUNCTION(S):
    DRVAPI_i16InitController
    DRVAPI_vDisable_int
    DRVAPI_vEnable_int
    DRVAPI_fnBufferToCanData
    DRVAPI_fnCanDataToBuffer
    DRVAPI_fnBusState       - Get the current bus state
    DRVAPI_fnTickStatus     - Provide tick for CAN status processing
    DRVAPI_bOK_to_send
    DRVAPI_iXmit_object
    DRVAPI_fnTxTimeOut

    local:
        drvapi_fnUpdateHandle
        drvapi_fnReceiveCB
        drvapi_fnTransmitCB
        drvapi_fnAbortCB
        drvapi_fnStatusCB

NOTES:
    There is no use or assumption of any operating system services.

HISTORY:
$Log: cdrvapi.c $

    ***********************************************
    Revision: XanBus_TrevorM/2
    User: TrevorM     Date: 08/25/10  Time: 04:20PM
    Changed Luminary Micro Xanbus Library shim code to unconditionally enable/disable
    interrupts regardless of whether the code is executing inside an ISR or
    not.

    ***********************************************
    Revision: XanBus_HollyZ/4
    User: HollyZ     Date: 02/11/09  Time: 10:40PM
    XanBus Lib V1.4.0 BN0730



    Database updates:

    Added PGN support for HV MPPT



    Library updates:

    Updated the source code in Common and Shim level to avoid the heap overflow
    issue while the network is not healthy

    ***********************************************
    Revision: XanBus_HollyZ/3
    User: HollyZ     Date: 02/06/09  Time: 11:37PM
    Few updates in this version:

    Removed the Tx status filter for counting the Tx error count;

    Abort frames that get stuck in the transmit buffer;

    Added the feature to check if the CAN Tx transmitter is sending out the
    frames;

    ***********************************************
    Revision: XanBus_HollyZ/2
    User: HollyZ     Date: 06/17/05  Time: 04:38PM
    Added new parameter BitRate to DRVAPI_i16InitController()

    ***********************************************
    Revision: XanBus_HollyZ/1
    User: HollyZ     Date: 06/03/05  Time: 10:50PM
    Kept again for the last changes

    ***********************************************
    Revision: GateWayPfrmANMEA_DaleM/2
    User: DaleM     Date: 02/18/05  Time: 06:24PM
    Removed Tx Counts and aborting of frames


    ***********************************************
    Revision: XanBus_DaleM/3
    User: DaleM     Date: 02/15/05  Time: 09:31PM
    merged


    ***********************************************
    Revision: XanBus_DaleM/2
    User: DaleM     Date: 02/14/05  Time: 10:58PM
    Can Id structure has been changed

    ***********************************************
    Revision: XanBus_DaleM/1
    User: DaleM     Date: 01/12/05  Time: 07:19PM
    Multiport XanBus Library


=============================================================================*/

/*==============================================================================
                              Includes
==============================================================================*/

#include <string.h>
#include "libenv.h"
#include "msghandl.h"
#include "cdrive.h"
#include "LIB_stddefs.h"
#include "candrv.h"
#include "canbuf.h"         // Interface to CAN frame buffering
#include "CDrvAPI.h"
#ifdef _UCOSII
#include "includes.h"
#endif
#include "xassert.h"        // Debugging
#include "device.h"

#define DRVAPI_MAX_BUFFERS  ( CANDRV_TX_BUFFERS )

// OS tick count
#define DRVAPI_TX_TIMEOUT   ( 50 )

/*=============================================================================
                              Structures
==============================================================================*/

typedef struct DRVAPI_zCAN_STATUS
{
    uchar8  ucPrevStatus;
    tucBOOL tucBusWasOff;
    uchar8  ucBusyCount;
} DRVAPI_tzCAN_STATUS;

/*==============================================================================
                           Local/Private Constants
==============================================================================*/

/*==============================================================================
                           Local/Private Variables
==============================================================================*/

// Timeout counters, one for each transmit buffer of each controller
static uint16 drvapi_auiTxTimeCount[ MAX_PORTS ][ DRVAPI_MAX_BUFFERS ];

// Array of transmit message handles
static INT16 siTxMsgHandle[ MAX_PORTS ][ DRVAPI_MAX_BUFFERS ];

// Array of communications statistics records
static CAN_tzCOMM_STATS atzCanCommStats[ MAX_PORTS ];

/*==============================================================================
                       Internal Function Prototypes
==============================================================================*/

static void drvapi_fnUpdateHandle( CANPORT Channel, uchar8 ucHandleIndex, BOOL bTXOK );

// Callback functions for interrupt handling
void drvapi_fnReceiveCB( uchar8 Channel );
void drvapi_fnTransmitCB( uchar8 Channel );
void drvapi_fnAbortCB( uchar8 Channel );
void drvapi_fnStatusCB( uchar8 Channel );
static void drvapi_fnTxTimeOut( CANPORT Channel );

/*==============================================================================
                           Function Definitions
==============================================================================*/

/*******************************************************************************

FUNCTION NAME:
    DRVAPI_i16InitController

PURPOSE:
    Initialize and enable the CAN channel

INPUTS:
    'Channel' is the CAN channel to be initialized, usually zero
    'ucBitRate' is the CAN port bit rate, must be CANDRV_eBIT_RATE_20KBPS or
                CANDRV_eBIT_RATE_250KBPS

OUTPUTS:
    TFXCR_OK if all goes well

NOTES:
    The function name is from the original driver API by Teleflex.

VERSION HISTORY:

Version: 1.00  Date: 02/20/03  By: Tom Lightfoot
    - Creation
Version: 1.01  Date: 08/01/03  By: Dalem
    - Check return from CANDRV_fnInstallCb
Version: 1.02  Date: 09/16/03  By: Dalem
    - Added install of status callback
Version: 1.02  Date: 10/12/04  By: Dalem
    - Init Can Status for the channel
Version: 1.03  Date: 02/18/05  By: Dalem
    - Removed txTimeCounts

*******************************************************************************/

INT16 DRVAPI_i16InitController( CANPORT Channel,
                                uchar8  ucBitRate )
{
    uchar8 Handle;    // A counter

    // Install all callback functions before any interrupts get enabled
    if( ( CANDRV_fnInstallCb( Channel,
                              CANDRV_eCB_RECEIVE,
                              drvapi_fnReceiveCB ) != CANDRV_eRET_SUCCESS )
     || ( CANDRV_fnInstallCb( Channel,
                              CANDRV_eCB_TXOK,
                              drvapi_fnTransmitCB ) != CANDRV_eRET_SUCCESS )
     || ( CANDRV_fnInstallCb( Channel,
                              CANDRV_eCB_TXABORT,
                              drvapi_fnAbortCB ) != CANDRV_eRET_SUCCESS )
     || ( CANDRV_fnInstallCb( Channel,
                              CANDRV_eCB_STATUS,
                              drvapi_fnStatusCB ) != CANDRV_eRET_SUCCESS ) )
    {
        return TFXCR_PORT_INIT_ERR;
    }

    // Other callback functions are possible but they are not installed.
    // - Bus status is polled so there is (so far) no need for the status
    //   change callback.

    // Clear all of the transmit timeouts
    for ( Handle = 0; Handle < DRVAPI_MAX_BUFFERS; Handle++ )
    {
        drvapi_auiTxTimeCount[ Channel ][ Handle ] = 0;
    }

   // Clear the array of communications statistics records
    ( void )DRVAPI_fnClearCommStats(  Channel  );

    // Initialize the CAN hardware for 250kbps and passing all frames
    switch( CANDRV_fnInit( Channel,
                           (CANDRV_teBIT_RATE)ucBitRate,
                           CANDRV_eFILTER_PASS_ALL )  )
    {
    case CANDRV_eRET_SUCCESS:
        break;

    case CANDRV_eRET_BAD_CHANNEL:
        return TFXCR_INVALID_PORT;

    case CANDRV_eRET_BAD_BIT_RATE:
    case CANDRV_eRET_BAD_FILTER:
    case CANDRV_eRET_INIT_FAIL:
    default:
        return TFXCR_PORT_INIT_ERR;
    }

    // Enable the CAN port
    if ( CANDRV_fnEnable( Channel ) != CANDRV_eRET_SUCCESS )
    {
        return TFXCR_PORT_INIT_ERR;
    }

    // Indicate successful initialization
    return TFXCR_OK;
}

/*******************************************************************************

FUNCTION NAME:
    DRVAPI_vDisable_int

PURPOSE:
    Disable interrupts so that the stack can add or remove frames from
    the circular buffers.

INPUTS:
    The CAN channel who's interrupts are to be disabled

OUTPUTS:
    Nothing

NOTES:
    All interrupts are masked in this simple implementation

VERSION HISTORY:

Version: 1.00  Date: 02/20/03  By: Tom Lightfoot
    - Creation

*******************************************************************************/

void DRVAPI_vDisable_int( CANPORT Channel )
{
    // fool the compiler
    if( Channel )
        ;

    // Take the simple route and disable ALL interrupts
    DEVICE_mGLOBAL_INT_DIS();
}

/*******************************************************************************

FUNCTION NAME:
    DRVAPI_vEnable_int

PURPOSE:
    Enable interrupts after the stack has added or removed frames from
    the circular buffers.

INPUTS:
    The CAN channel who's interrupts are to be enabled

OUTPUTS:
    Nothing

NOTES:
    All interrupts are masked in this simple implementation

VERSION HISTORY:

Version: 1.00  Date: 02/20/03  By: Tom Lightfoot
    - Creation
Version: 1.01  Date: 04/01/03  By: Tom Lightfoot
    - Interrupts only get re-enabled if not in an ISR
Version: 1.02  Date: 08/23/03  By: Trevor Monk
    - Created SHIM for Luminary Micro

*******************************************************************************/

void DRVAPI_vEnable_int( CANPORT Channel )
{
    // fool the compiler
    if( Channel )
        ;

    DEVICE_mGLOBAL_INT_EN();
}

/*******************************************************************************

FUNCTION NAME:
    DRVAPI_fnBufferToCanData

PURPOSE:
    This function converts info from a CAN driver structure,
    presumably from the circular buffer, into the CANDATA
    format used by the stack code.

INPUTS:
    'pzCanBuf' is a pointer to the source buffer
    'pzCanData' is a pointer to the destination structure

OUTPUTS:
    Nothing

NOTES:
    This function is provided for use by the stack code.

VERSION HISTORY:

Version: 1.00  Date: 02/20/03  By: Tom Lightfoot
    - Creation
Version: 1.01  Date: 01/28/05  By: Dalem
    - Can Id structure was changed

*******************************************************************************/

void DRVAPI_fnBufferToCanData( CANDRV_tzCAN_FRAME *pzFrame,
                               CANDATA *pzCanData )
{
    CANDRV_tzCAN_ID *pzId;       // Convenience pointer to pzCanBuf's ID field

    // Set up the convenience pointers
    pzId = &pzFrame->tzCanId;

    // Priority is bits 28, 27, and 26
    pzCanData->m_u8Priority = ( pzId->ucId28_24 >> 2 ) & 0x07;

    // Data Page bit is ID bit 24
    pzCanData->m_u8DataPage = pzId->ucId28_24 & 0x01;

    // PF byte is bits ID23 to ID16
    pzCanData->m_u8PF = pzId->ucId23_16;

    // PS byte is bits ID15 to ID8
    pzCanData->m_u8PS = pzId->ucId15_8;

    // SA byte is bits ID7 to ID0
    pzCanData->m_u8SA = pzId->ucId07_00;

    // Byte count
    pzCanData->m_u8DataByteCount = pzFrame->ucByteCount;

    // Copy the data bytes
    (void) memcpy( pzCanData->m_u8Data, pzFrame->aucData, CANDRV_MAX_BYTES );
}

/*******************************************************************************

FUNCTION NAME:
    DRVAPI_fnCanDataToBuffer

PURPOSE:
    This function converts frame information from a
    CANDATA structure used by the stack code
    into a buffer used by the MSCAN driver.
    This function is called when sending a frame to the bus.

INPUTS:
    'pzCanData' is a pointer to the source structure from the CAN stack
    'pzFrame' is a pointer to the destination driver buffer

OUTPUTS:
    Nothing

NOTES:
    This function is provided for use by the stack when sending a frame.

VERSION HISTORY:

Version: 1.00  Date: 02/20/03  By: Tom Lightfoot
    - Creation
Version: 1.01  Date: 01/28/05  By: Dalem
    - Can Id structure was changed

*******************************************************************************/

void DRVAPI_fnCanDataToBuffer( CANDATA *pzCanData,
                               CANDRV_tzCAN_FRAME *pzFrame )
{
    CANDRV_tzCAN_ID *pzId;       // Convenience pointer to pzCanBuf's ID field

    // Set up the convenience pointers
    pzId = &pzFrame->tzCanId;

    // Set the Data length
    pzFrame->ucByteCount = pzCanData->m_u8DataByteCount;

    // Most significant Identifier byte
    pzId->ucId28_24 = ( pzCanData->m_u8Priority << 2 )  // priority bits 28 - 26
                      | ( pzCanData->m_u8DataPage )     // data page bit 24
                      | CANDRV_ID_MSK_IDE
                      | CANDRV_ID_MSK_SRR;      // IDE and SRR bits always set
                                                // RTR always zero

    // Second ID byte
    pzId->ucId23_16 = pzCanData->m_u8PF;

    // Third ID byte
    pzId->ucId15_8 = pzCanData->m_u8PS;

    // Fourth ID byte
    pzId->ucId07_00 = pzCanData->m_u8SA;

    // Copy the data bytes
    (void) memcpy( pzFrame->aucData, pzCanData->m_u8Data, CANDRV_MAX_BYTES );
}

/*******************************************************************************

FUNCTION NAME:
    drvapi_fnUpdateHandle

PURPOSE:
    This function is called after a buffer has been sent to the bus to
    indicate how successful the send was.

INPUTS:
    'Channel' is The MSCAN module being used (usually 0)
    'ucHandleIndex' is the handle of the message just sent
    'bTXOK' is the indicator if the message was sent successfully

OUTPUTS:

NOTES:
    Nothing

VERSION HISTORY:

Version: 1.00  Date: 02/20/03  By: Tom Lightfoot
    - Creation
Version: 1.01  Date: 10/08/04  By: dalem
    - Removed the reading of the contoller state

*******************************************************************************/

static void drvapi_fnUpdateHandle( CANPORT Channel, uchar8 ucHandleIndex, BOOL bTXOK )
{
    MSG_HANDLE *pmhHandle;

    pmhHandle = pmhGet_handle_ptr( siTxMsgHandle[ Channel ][ ucHandleIndex ] );
    if ( pmhHandle != NULL )
    {
        pmhHandle->iNumFramesTransmitted++;

        // Explicitly set or clear the TXOK bit according to whether
        // or not the frame was sent successfully or aborted due to
        // a timeout.
        if( bTXOK )
        {
            pmhHandle->uiStatus |= SB_TXOK;
        }
        else
        {
            pmhHandle->uiStatus &= ~SB_TXOK;
        }
    }
}

/*******************************************************************************

FUNCTION NAME:
    DRVAPI_fnBusState       - Get the current bus state

PURPOSE:
    This function checks the status of the indicated MSCAN module
    and returns a bitmask indicating transmit and receive
    error states.

INPUTS:
    'Channel' is the CAN port number being queried

OUTPUTS:
    One of: CANDRV_eSTATUS_OK,
            CANDRV_eSTATUS_BUSOFF,
            CANDRV_eSTATUS_WARNING,
            CANDRV_eSTATUS_ERROR,
            CANDRV_eSTATUS_INVALID

NOTES:

VERSION HISTORY:

Version: 1.01  Date: 07/31/03  By: dalem
    - Created

*******************************************************************************/

uchar8 DRVAPI_fnBusState( CANPORT Channel )
{
    assert( Channel < MAX_PORTS );

    // Return the saved last status
    return ( atzCanCommStats[ Channel ].ucLastTxStatus );
}

/*******************************************************************************

FUNCTION NAME:
    DRVAPI_fnTickStatus     - Provide tick for CAN status processing

PURPOSE:
    This function is use to provide a time tick to CAN status processing.
    The CAN controller will quickly switch into and out of Bus Off when it
    should just stay in the Bus Off condition. This function is used to
    provide time filtering for Bus Off status.

INPUTS:
    Channel is the number of the CAN port being serviced

OUTPUTS:
    Nothing

NOTES:
    The CAN device does not stay in the BUS OFF state when there is a
    condition that causes BUS OFF. The device will cycle from bus off to
    ok. Also, when the device does not restart transmissions when it changes
    from bus off or bus error to Ok.


VERSION HISTORY:

Version: 1.00  Date: 10/08/04  By: dalem
    - Created
Version: 1.01  Date: 10/12/04  By: dalem
    - Added counting of TX Busy occurances
Version: 1.02  Date: 10/13/04  By: dalem
    - Timeout and abort frames if they are stuck.
Version: 1.03  Date: 02/18/05  By: dalem
    - Removed timeout and aborting frames

*******************************************************************************/

void DRVAPI_fnStatusTick( CANPORT Channel )
{
     drvapi_fnTxTimeOut( Channel );

    /*
    // If all transmit buffers are busy
    if ( CANDRV_fnTransmitOk( Channel ) == CANDRV_eRET_TX_BUSY )
    {
        // Do not wrap counter
        if ( atzCanStatus[ Channel ].ucBusyCount < 255 )
        {
            // Count busy occurance
            atzCanStatus[ Channel ].ucBusyCount++;
        }
    }

    // If the latest status change is a Bus Off
    // or no Bus Off occured since last check
    if( ( atzCanStatus[ Channel ].ucPrevStatus == CANDRV_eSTATUS_BUSOFF )
     || ( atzCanStatus[ Channel ].tucBusWasOff == FALSE ) )
    {
        // Keep and use the last status change
        atzCanCommStats[ Channel ].ucLastTxStatus
                            = atzCanStatus[ Channel ].ucPrevStatus;
    }

    // If tx status has changed
    if ( atzCanStatus[ Channel ].ucPrevStatus
                            != atzCanCommStats[ Channel ].ucLastTxStatus )
    {
        // If status is error
        if( atzCanStatus[ Channel ].ucPrevStatus == CANDRV_eSTATUS_ERROR )
        {
            // Count tx error event
            atzCanCommStats[ Channel ].uiTxErrCount++;
        }

        // If status is busoff
        if ( atzCanStatus[ Channel ].ucPrevStatus == CANDRV_eSTATUS_BUSOFF )
        {
            // Count busoff event
            atzCanCommStats[ Channel ].uiBusoffCount++;
        }
    }

    // Clear the BusWasOff flag for next check
    atzCanStatus[ Channel ].tucBusWasOff = FALSE;
    */
}

/*******************************************************************************

FUNCTION NAME:
    DRVAPI_fnIsBlocked      - Check is CAN channel is blocked

PURPOSE:
    This function is use to return the blocked status of a channel. A
    channel is considered blocked if the transmitter has been busy for
    a number of ticks.

INPUTS:
    Channel is the number of the CAN port being checked

OUTPUTS:
    TRUE if CAN port is blocked (cannot accept any more frames)
    FALSE if frames can be sent

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 10/12/04  By: dalem
    - Created

*******************************************************************************/

tucBOOL DRVAPI_fnIsBlocked( CANPORT Channel )
{
    return ( !CANDRV_fnIsTransmitting( Channel ) );
}

/*******************************************************************************

FUNCTION NAME:
    DRVAPI_fnCountTxLost

PURPOSE:
    Increment count of dropped Tx Frames

INPUTS:
    'Channel' is the channel number

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 09/17/03  By: dalem
    - Creation

*******************************************************************************/

void DRVAPI_fnCountTxLost( uchar8 Channel )
{
    // Validate channel number
    if ( Channel < MAX_PORTS )
    {
        // Get copy of statistics
        atzCanCommStats[ Channel ].uiTxDropped++;
    }
}

/*******************************************************************************

FUNCTION NAME:
    DRVAPI_fnGetCommStats

PURPOSE:
    Return current value of the CAN communications statistics counters

INPUTS:
    'Channel' is the channel number, usually zero
    'ptzStats' is a pointer to a location to copy the statistics into

OUTPUTS:
    TFXCR_OK if all goes well

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 03/03/03  By: dalem
    - Creation

*******************************************************************************/

uint16 DRVAPI_fnGetCommStats( uchar8 Channel, CAN_tzCOMM_STATS *ptzStats )
{
    // Validate channel number
    if ( Channel >= MAX_PORTS )
    {
        //  Index out of range
        return TFXCR_INVALID_PORT;
    }

    // Get copy of statistics
    *ptzStats = atzCanCommStats[ Channel ];

    // Success
    return TFXCR_OK;
}

/*******************************************************************************

FUNCTION NAME:
    DRVAPI_fnClearComStats

PURPOSE:
    Clear communications statistics counters

INPUTS:
    Channel is the channel number, usually zero

OUTPUTS:
    Clears communications statistics counters
    and returns TFXCR_OK if all goes well

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 03/03/03  By: dalem
    - Creation

*******************************************************************************/

uint16 DRVAPI_fnClearCommStats( uchar8 Channel )
{
    // Validate channel number
    if ( Channel >= MAX_PORTS )
    {
        //  Index out of range
        return TFXCR_INVALID_PORT;
    }

    // Clear communications statics for the channel
    (void) memset( &atzCanCommStats[ Channel ], 0, sizeof( CAN_tzCOMM_STATS ) );

    // Success
    return TFXCR_OK;
}

/*******************************************************************************

FUNCTION NAME:
    DRVAPI_bOK_to_send

PURPOSE:
    This function checks an MSCAN module to see if there is
    an unoccupied transmit buffer that can be used to send a new
    frame.

INPUTS:
    'Channel' is the module number to be checked, usually 0

OUTPUTS:
    TRUE  => if there is at least one module free
    FALSE => if the channel is invalid or all buffers are busy

NOTES:
    The function name comes from the original CAN driver API from Teleflex

VERSION HISTORY:

Version: 1.00  Date: 02/20/03  By: Tom Lightfoot
    - Creation

*******************************************************************************/

BOOL DRVAPI_bOK_to_send( CANPORT Channel )
{
    if ( CANDRV_fnTransmitOk( Channel ) == CANDRV_eRET_SUCCESS )
    {
        // A transmit buffer is available
        return TRUE;
    }
    else
    {
        // Regardless of the return value, there is no available buffer
        return FALSE;
    }
}

/*******************************************************************************

FUNCTION NAME:
    DRVAPI_iXmit_object

PURPOSE:
    This is THE function that sends a frame object to the MSCAN module
    for transmission on the bus.  When done, it enables the interrupt for the
    transmit buffer that was used.  It is up to the interrupt service routine to
    update the message's handle and grab the next buffer from the
    circular buffer.

INPUTS:
    'Channel' is the MSCAN module being used, usually 0
    'pzCanObj' is a pointer to the object to be sent

OUTPUTS:
    TFXCR_OK if all goes well

NOTES:
    This function is called both by the higher level driver send function
    and the transmit interrupt callback function.
    The function name comes from the original CAN driver API from Teleflex

VERSION HISTORY:

Version: 1.00  Date: 02/20/03  By: Tom Lightfoot
    - Creation
Version: 1.01  Date: 05/16/03  By: Dalem
    - Changed size/names of CanCommStats members
Version: 1.02  Date: 05/20/03  By: Tom Lightfoot
    - Chanced size/names of CanCommStats back the way it was

*******************************************************************************/

INT16 DRVAPI_iXmit_object( CANPORT Channel,
                           CANBUF_tzTX_BUF *ptzTxBuf )
{
    uchar8 Handle;    // Handle indicating which transmit buffer was used

    // This has to be an atomic operation
    DRVAPI_vDisable_int( Channel );

    // Send the buffer to the CAN bus
    switch ( CANDRV_fnTransmit( Channel,
                                &ptzTxBuf->zCanFrame,
                                &Handle ) )
    {
    case CANDRV_eRET_SUCCESS:
        break;

    case CANDRV_eRET_BAD_CHANNEL:
        DRVAPI_vEnable_int( Channel );
        return TFXCR_INVALID_PORT;

    case CANDRV_eRET_TX_BUSY:
    default:
        DRVAPI_vEnable_int( Channel );
        return TFXCR_MSG_NOT_HANDLED;
    }

    // Save the handle to the message for update by the ISR.
    siTxMsgHandle[ Channel ][ Handle ] = ptzTxBuf->uiMsgHandle;

    // Initialize the timeout counter for the buffer
    drvapi_auiTxTimeCount[ Channel ][ Handle ] = DRVAPI_TX_TIMEOUT;

    // Enable interrupts for this buffer
    DRVAPI_vEnable_int( Channel );

    // Count frame as transmitted
    atzCanCommStats[ Channel ].ulTxExtFrames++;

    return TFXCR_OK;
}

/*******************************************************************************

FUNCTION NAME:
    DRVAPI_fnTxTimeOut

PURPOSE:
    To abort frames that get stuck in the transmit buffer

INPUTS:
    'Channel' is the CAN module being used

OUTPUTS:
    Nothing

NOTES:
    It's a sad fact that if the bus is unplugged a frame can get
    stuck in the transmit buffer.  The CAN module will stop trying
    to send the frame and no interrupt will be generated saying that the
    buffer is no longer busy.  This function should be called periodically
    to check if any frame has been in its transmit buffer too long
    and to abort it if it has.

VERSION HISTORY:

Version: 1.00  Date: 02/20/03  By: Tom Lightfoot
    - Creation

*******************************************************************************/

static void drvapi_fnTxTimeOut( CANPORT Channel )
{
    uchar8 Handle;    // Transmit buffer number

    // Get a pointer to the right controller
    if ( Channel >= MAX_PORTS )
    {
        // Index out of range
        return;
    }

    // Go through each transmit buffer
    for ( Handle = 0; Handle < DRVAPI_MAX_BUFFERS; Handle++ )
    {
        // If the counter is non-zero then it is being actively used
        if ( drvapi_auiTxTimeCount[ Channel ][ Handle ] )
        {
            // Decrement the counter
            drvapi_auiTxTimeCount[ Channel ][ Handle ]--;

            // Is it zero now?
            if ( drvapi_auiTxTimeCount[ Channel ][ Handle ] == 0 )
            {
                // Then this is a timeout situation and the frame should be aborted
                (void) CANDRV_fnAbort( Channel, Handle );

                // If the abort is successful then the transmit
                // buffer will clear and an interrupt will be generated.
                // It's up to the transmit ISR to check if the interrupt
                // was generated by an abort or a successful frame transmission
            }
        }
    }
}



/*=============================================================================
                      Callback Functions for CAN Interrupts
=============================================================================*/

/*******************************************************************************

FUNCTION NAME:
    drvapi_fnReceiveCB

PURPOSE:
    To respond to a CAN receive interrupt by putting the received frame
    into the circular buffer.

INPUTS:
    The number of the CAN module that caused the interrupt

OUTPUTS:
    Nothing

NOTES:
    As this function is executed during an interrupt, it assumes that
    interrupts are already disabled while placing frames into the
    circular buffer.

VERSION HISTORY:

Version: 1.00  Date: 02/20/03  By: Tom Lightfoot
    - Creation
Version: 1.01  Date: 04/01/03  By: Tom Lightfoot
    - Interrupts blocked from being re-enabled
Version: 1.02  Date: 05/16/03  By: Dalem
    - Changed size/names of CanCommStats members
Version: 1.03  Date: 05/20/03  By: Tom Lightfoot
    - Changed size/names of CanComStats back to 1.01
Version: 1.04  Date: 08/23/03  By: Trevor Monk
    - Created SHIM for Luminary Micro

*******************************************************************************/

void drvapi_fnReceiveCB( uchar8 Channel )
{
    static CANBUF_tzRX_BUF zRxBuf;      // Intermediate receive buffer

    // Handle the received frame
    if ( CANDRV_fnReceive( Channel, &zRxBuf.zCanFrame ) == CANDRV_eRET_SUCCESS )
    {

        // Throw the object into the circular buffer
        if ( CANBUF_fnRxInsert( Channel,
                                &zRxBuf ) == CANBUF_BUFFER_FULL )
        {
            // Increment the error counter
            atzCanCommStats[ Channel ].uiRxDropped++;
        }
        else
        {
            // Count frame as received
            atzCanCommStats[ Channel ].ulRxExtFrames++;
        }
    }
}

/*******************************************************************************

FUNCTION NAME:
    drvapi_fnTransmitCB

PURPOSE:
    To send more frames in response to a transmit buffer empty interrupt.

INPUTS:
    Channel is the number of the CAN port that caused the interrupt

OUTPUTS:
    Nothing

NOTES:
    As this function is executed during an interrupt, it assumes that
    interrupts are already disabled while removing frames from the
    circular buffer.

VERSION HISTORY:

Version: 1.00  Date: 02/20/03  By: Tom Lightfoot
    - Creation
Version: 1.01  Date: 04/01/03  By: Tom Lightfoot
    - Interrupts blocked from being re-enabled
Version: 1.02  Date: 10/12/04  By: Dalem
    - Clear busy count for the channel
Version: 1.03  Date: 02/18/05  By: Dalem
    - Removed clear of busy count
Version: 1.04  Date: 08/23/03  By: Trevor Monk
    - Created SHIM for Luminary Micro

*******************************************************************************/

void drvapi_fnTransmitCB( uchar8 Channel )
{
    uchar8 Handle;  // Transmit buffer recently empty
    static CANBUF_tzTX_BUF zTxBuf;

    // Find out which transmit buffer caused the interrupt
    Handle = CANDRV_fnGetHandle( Channel );

    // Update the handle for that buffer
    drvapi_fnUpdateHandle( Channel, Handle, TRUE );

    // Clear the timeout for that buffer
    drvapi_auiTxTimeCount[ Channel ][ Handle ] = 0;

    // Send another frame if there is a buffer free
    if ( DRVAPI_bOK_to_send( Channel ) )
    {
        // Is there a frame waiting to be sent?
        if ( CANBUF_fnTxExtract( Channel,
                                 &zTxBuf ) != CANBUF_BUFFER_EMPTY )
        {
            if ( DRVAPI_iXmit_object( Channel, &zTxBuf ) != TFXCR_OK )
            {
                 // Failed to transmit the frame.
                atzCanCommStats[ Channel ].uiTxDropped++;
            }
        }
    }
}

/*******************************************************************************

FUNCTION NAME:
    drvapi_fnAbortCB

PURPOSE:
    To respond to the interrupt situation where a transmit buffer is
    empty due to an aborted frame.  This function updates the handle
    in the frame's object.

INPUTS:
    Channel is the number of the CAN port that caused the interrupt

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/20/03  By: Tom Lightfoot
    - Creation
Version: 1.01  Date: 04/01/03  By: Tom Lightfoot
    - Interrupts blocked from being re-enabled
Version: 1.02  Date: 08/23/03  By: Trevor Monk
    - Created SHIM for Luminary Micro

*******************************************************************************/

void drvapi_fnAbortCB( uchar8 Channel )
{
    uchar8 Handle;  // Transmit buffer recently empty

    // Find out which transmit buffer caused the interrupt
    Handle = CANDRV_fnGetHandle( Channel );

    // Update the handle for that buffer
    drvapi_fnUpdateHandle( Channel, Handle, FALSE );
}

/*******************************************************************************

FUNCTION NAME:
    drvapi_fnStatusCB

PURPOSE:
    To respond to a Status Change interrupt.

INPUTS:
    Channel is the number of the CAN port that caused the interrupt

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 09/16/03  By: Dalem
    - Creation
Version: 1.01  Date: 10/08/04  By: Dalem
    - Added time filtering for tx status
Version: 1.02  Date: 01/22/09  By: Hollyz
    - Removed the time filering for tx status, CAN transmitter status can be
      retrived from DRVAPI_fnIsBlocked()
Version: 1.03  Date: 08/23/03  By: Trevor Monk
    - Created SHIM for Luminary Micro

*******************************************************************************/

void drvapi_fnStatusCB( uchar8 Channel )
{
    CANDRV_teSTATUS teStatus;

    // Read tx status
    teStatus = CANDRV_fnTxStatus( Channel,
                                  NULL );

   // If tx status has changed
    if( teStatus != atzCanCommStats[ Channel ].ucLastTxStatus )
    {
        // If status is error
        if( teStatus == CANDRV_eSTATUS_ERROR )
        {
            // Count tx error event
            atzCanCommStats[ Channel ].uiTxErrCount++;
        }

        // If status is busoff
        if ( teStatus == CANDRV_eSTATUS_BUSOFF )
        {
            // Count busoff event
            atzCanCommStats[ Channel ].uiBusoffCount++;
        }
    }

    // Save tx status
    atzCanCommStats[ Channel ].ucLastTxStatus = (uchar8) teStatus;

    // Read rx status
    teStatus = CANDRV_fnRxStatus( Channel,
                                  NULL );

    // If rx status has changed
    if( teStatus != atzCanCommStats[ Channel ].ucLastRxStatus )
    {
        // If status is error
        if( teStatus == CANDRV_eSTATUS_ERROR )
        {
            // Count rx error event
            atzCanCommStats[ Channel ].uiRxErrCount++;
        }
    }

    // Save rx status
    atzCanCommStats[ Channel ].ucLastRxStatus = (uchar8) teStatus;
}

