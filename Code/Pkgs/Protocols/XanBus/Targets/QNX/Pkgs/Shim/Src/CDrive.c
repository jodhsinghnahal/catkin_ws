/*
***************************************************************************
**
** PROJECT: TIS NMEA 2000 Communications
**
**
** DESCRIPTION: CANDriver Entry points
**
**
**
**
**            Copyright (c) 1999 Teleflex Marine/Industrial
**                       All Rights Reserved
**   Use, duplication, or disclosure of this material, in any
**   form, is forbidden without permission from Teleflex Marine
***************************************************************************
** .HISTORY OF REVISIONS
**
$Log: CDrive.c $

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
    User: HollyZ     Date: 06/12/06  Time: 06:33PM
    Added support to send/receive CAN frame in raw data format

    ***********************************************
    Revision: XanBus_HollyZ/2
    User: HollyZ     Date: 06/17/05  Time: 04:38PM
    Added new parameter BitRate to TFXDRV_i16InitController() and TFXDRV_i16ConnectController()


    ***********************************************
    Revision: XanBus_HollyZ/1
    User: HollyZ     Date: 06/07/05  Time: 07:09PM
    Save

    ***********************************************
    Revision: GateWayPfrmANMEA_DaleM/1
    User: DaleM     Date: 02/10/05  Time: 10:25PM
    Cleaned up old history list


    ** ***********************************************
    ** Revision: XanBus_DaleM/1
    ** User: DaleM     Date: 01/12/05  Time: 07:19PM
    ** Multiport XanBus Library
    **

******************************************************************/

#include <string.h>
#include "libenv.h"
#include "cdrive.h"

#include <stdio.h>
#include "msghandl.h"
#include "LIB_stddefs.h"
#include "candrv.h"
#include "CDrvAPI.h"
#include "canbuf.h"         // Interface to CAN frame buffering

static BOOL bIsPortValid(CANPORT u8PortNumber);
//**********************************************************************
// TFXDRV_i16InitBuffers
//**********************************************************************
TFXCAN_RETURNS TFXDRV_i16InitBuffers(CANPORT u8PortNumber )
{
    if ( !bIsPortValid( u8PortNumber ) )
    {
    	// Failure
        return TFXCR_INVALID_PORT;
    }

    // Initialize buffers for this port fail to initialize
    if ( CANBUF_fnCtor( u8PortNumber ) == FALSE )
    {
        // Return failure
        return ( TFXCR_DRV_RX_BUFFER_ALLOCATION_FAILURE );
    }

    // Success
    return ( TFXCR_OK );
}

//**********************************************************************
// TFXDRV_i16InitController
//**********************************************************************
TFXCAN_RETURNS TFXDRV_i16InitController(CANPORT u8PortNumber,
                                        uchar8  ucBitRate)
{
    // check port and initialize appropriate controller
    if (!bIsPortValid(u8PortNumber))
        return TFXCR_INVALID_PORT;
    return (TFXCAN_RETURNS)DRVAPI_i16InitController(u8PortNumber,
                                    ucBitRate);
}

//**********************************************************************
// TFXDRV_i16ControllerState
//**********************************************************************

TFXCAN_RETURNS TFXDRV_i16ControllerState(CANPORT u8PortNumber)
{
    uchar8 ucBusState;

    // Get the bus state
    ucBusState = DRVAPI_fnBusState( u8PortNumber );

    // Convert bus state from CANDRV values
    if ( ucBusState == CANDRV_eSTATUS_OK )
        return TFXCR_OK;
    else if ( ucBusState == CANDRV_eSTATUS_BUSOFF )
        return TFXCR_BUS_OFF;
    else
        return TFXCR_BUS_WARNING;
}

/*******************************************************************************

FUNCTION NAME:
    TFXDRV_fnTickStatus     - Provide tick for CAN status processing

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

VERSION HISTORY:

Version: 1.00  Date: 10/08/03  By: dalem
    - Created

*******************************************************************************/

void TFXDRV_fnStatusTick( CANPORT Channel )
{
    DRVAPI_fnStatusTick( Channel );
}

/*******************************************************************************

FUNCTION NAME:
    TFXDRV_fnIsBlocked      - Check is CAN channel is blocked

PURPOSE:
    This function is use to return the blocked status of a channel.

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

tucBOOL TFXDRV_fnIsBlocked( CANPORT Channel )
{
    return ( DRVAPI_fnIsBlocked( Channel ) );
}

/*******************************************************************************

FUNCTION NAME:
    TFXDRV_fnGetComStats

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

TFXCAN_RETURNS TFXDRV_fnGetCommStats( uchar8 Channel, CAN_tzCOMM_STATS *ptzStats )
{
    return (TFXCAN_RETURNS)DRVAPI_fnGetCommStats( Channel, ptzStats );
}

/*******************************************************************************

FUNCTION NAME:
    TFXDRV_fnClearComStats

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

TFXCAN_RETURNS TFXDRV_fnClearCommStats( uchar8 Channel )
{
    return (TFXCAN_RETURNS)DRVAPI_fnClearCommStats( Channel );
}

//**********************************************************************
// TFXDRV_i16ConnectController
//**********************************************************************
TFXCAN_RETURNS TFXDRV_i16ConnectController(CANPORT u8PortNumber,
                                           uchar8 ucBitRate)
{
    if (!bIsPortValid(u8PortNumber))
        return TFXCR_INVALID_PORT;

    // Initialize Controller
    return TFXDRV_i16InitController( u8PortNumber,
                                     ucBitRate);
}

//**********************************************************************
// TFXDRV_i16ReceiveFrameCheck
//**********************************************************************
TFXCAN_RETURNS TFXDRV_i16ReceiveFrameCheck( CANPORT u8PortNumber )
{
    // check port
    if (!bIsPortValid(u8PortNumber))
        return TFXCR_INVALID_PORT;
    return TFXCR_OK;
}

//**********************************************************************
// TFXDRV_i16ReceiveFrame
//**********************************************************************
TFXCAN_RETURNS TFXDRV_i16ReceiveFrame( CANPORT u8PortNumber,
                                       CANFRAME *pi16Frame )
{
    CANDRV_tzCAN_FRAME canframe;
    TFXCAN_RETURNS result = TFXCR_NO_DATA;

    /* check if port number is valid */
    if ( bIsPortValid(u8PortNumber) )
    {
        /* see if we can receive a CAN frame */
        if( CANDRV_fnReceive( u8PortNumber, &canframe ) == CANDRV_eRET_SUCCESS )
        {
            if( pi16Frame != NULL )
            {
                /* convert CAN driver frame to application CAN frame */
                DRVAPI_fnBufferToCanData( &canframe, &pi16Frame->m_CanData );
                result = TFXCR_NEW_DATA;
            }
        }
    }
    else
    {
        result = TFXCR_INVALID_PORT;
    }

    return result;
}

//**********************************************************************
// TFXDRV_i16ReceiveForeignFrame
//**********************************************************************
TFXCAN_RETURNS TFXDRV_i16ReceiveForeignFrame( CANPORT u8PortNumber,
                                              CANDRV_tzCAN_FRAME *pi16Frame )
{
    TFXCAN_RETURNS result = TFXCR_NO_DATA;
    CANDRV_teRETURN rc;

    if ( bIsPortValid(u8PortNumber) )
    {
        if( pi16Frame != NULL )
        {
            /* see if we can receive a CAN frame */
            rc = CANDRV_fnReceive( u8PortNumber, pi16Frame );
            if( rc == CANDRV_eRET_SUCCESS )
            {
                result = TFXCR_NEW_DATA;
            }
        }
    }
    else
    {
        result = TFXCR_INVALID_PORT;
    }

    return result;
}

//**********************************************************************
//
//**********************************************************************
TFXCAN_RETURNS TFXDRV_i16SendFrameCheck(CANPORT u8PortNumber)
{
    if (!bIsPortValid(u8PortNumber))
        return TFXCR_INVALID_PORT;

    // All OK return
    return TFXCR_OK;
}

//**********************************************************************
//
//**********************************************************************
TFXCAN_RETURNS TFXDRV_i16SendFrame( CANPORT u8PortNumber,
                                    CANFRAME *pFrame,
                                    INT16 *pi16MessageID )
{
    TFXCAN_RETURNS  ret = TFXCR_MSG_NOT_HANDLED;
    CANDRV_tzCAN_FRAME zCanFrame;

    if( ( pFrame != NULL ) &&
        ( pi16MessageID != NULL ) )
    {
        /* Message buffers not used */
        *pi16MessageID = 1000;

        /* Check port and buffer available to accept frame for transmission */
        ret = TFXDRV_i16SendFrameCheck( u8PortNumber );
        if ( ret == TFXCR_OK )
        {
            // Place PDU in sCanTxObject
            DRVAPI_fnCanDataToBuffer( &pFrame->m_CanData, &zCanFrame );


            if( CANDRV_fnTransmit( u8PortNumber,
                                   &zCanFrame,
                                   NULL ) == CANDRV_eRET_SUCCESS )
            {
                ret = TFXCR_OK;
            }
        }
    }

    /* Return result */
    return ret;
}

//**********************************************************************
//
//**********************************************************************
TFXCAN_RETURNS TFXDRV_i16SendForeignFrame( CANPORT u8PortNumber,
                                           CANDRV_tzCAN_FRAME *pFrame,
                                           INT16 *pi16MessageID )
{
    // not support yet
    return TFXCR_MSG_NOT_HANDLED;
}

//**********************************************************************
//
//**********************************************************************
TFXCAN_RETURNS TFXDRV_WriteFrame(CANPORT u8PortNumber)
{
    if ( !bIsPortValid( u8PortNumber ) )
        return TFXCR_INVALID_PORT;

    // Teleflex message buffers not used
    return TFXCR_NO_DATA;
}

//**********************************************************************
//
//**********************************************************************
static BOOL bIsPortValid(CANPORT u8PortNumber)
{
    return u8PortNumber < MAX_PORTS ? TRUE : FALSE;
}

/*******************************************************************************

FUNCTION NAME:
    TFXDRV_fnSleep

PURPOSE:
    Put CAN channel to sleep so that the CPU can use less power.

INPUTS:
    Channel is the number of the CAN module to put to sleep, usually zero.
    WakeEnable is TRUE if the module is to automatically wake up if there
        is traffic on the CAN bus.

OUTPUTS:
    TFXCR_OK if all goes well

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 10/17/03  By: Peter Drexel
    - Creation

*******************************************************************************/

TFXCAN_RETURNS TFXDRV_fnSleep( uchar8 Channel,
                                uchar8 WakeEnable )
{
	(void)CANDRV_fnSleep( Channel,
                    	WakeEnable );

    return( TFXCR_OK );
}

/*******************************************************************************

FUNCTION NAME:
    TFXDRV_fnInstallWakeupCb

PURPOSE:
    Install callback for handling wakeup interrupts

INPUTS:
    Channel is the number of the CAN module to use, usually zero.
    pFunc is the pointer to the callback function.

OUTPUTS:
    Returns TFXCR_OK on completion, or a failure code.


NOTES:
    The callback functions are called while the interrupt is being
    handled so they should not do very much processing, mostly on the
    order of queueing buffers and setting flags for task-level
    processing later.

VERSION HISTORY:

Version: 1.00  Date: 10/17/03  By: Peter Drexel
    - Creation from equivalent CANDRV function

*******************************************************************************/

TFXCAN_RETURNS TFXDRV_fnInstallWakeupCb( uchar8 Channel,
                                         TFXDRV_tpCALLBACK pFunc )
{
	if( CANDRV_fnInstallCb( Channel,
							CANDRV_eCB_WAKEUP,
                         	pFunc ) != CANDRV_eRET_SUCCESS )
    {
    	return( TFXCR_PORT_INIT_ERR );
    }

    return( TFXCR_OK );
}


/*******************************************************************************

FUNCTION NAME:
    TFXDRV_fnWakeup

PURPOSE:
    Wakeup CAN channel.

INPUTS:
    Channel is the number of the CAN module to put to sleep, usually zero.
    WakeEnable is TRUE if the module is to automatically wake up if there
        is traffic on the CAN bus.

OUTPUTS:
    TFXCR_OK if all goes well

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 10/17/03  By: Peter Drexel
    - Creation

*******************************************************************************/

TFXCAN_RETURNS TFXDRV_fnWakeUp( uchar8 Channel )
{
	(void)CANDRV_fnWakeUp( Channel );

    return( TFXCR_OK );
}
