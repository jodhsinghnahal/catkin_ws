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
    Revision: XanBus_HollyZ/2
    User: HollyZ     Date: 06/12/06  Time: 06:33PM
    Added support to send/receive CAN frame in raw data format 

    ***********************************************
    Revision: XanBus_HollyZ/1
    User: HollyZ     Date: 06/17/05  Time: 04:32PM
    Added bit rate parameter for TFXDRV_i16InitController() and TFXDRV_i16ConnectController()

    
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
                                        uchar8 ucBitRate)
{
    // check port and initialize appropriate controller
    if (!bIsPortValid(u8PortNumber))
        return TFXCR_INVALID_PORT;
    return ( TFXCAN_RETURNS ) DRVAPI_i16InitController(u8PortNumber,
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
    return ( TFXCAN_RETURNS ) DRVAPI_fnGetCommStats( Channel, ptzStats );
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
    return ( TFXCAN_RETURNS ) DRVAPI_fnClearCommStats( Channel );
}

//**********************************************************************
// TFXDRV_i16ConnectController
//**********************************************************************
TFXCAN_RETURNS TFXDRV_i16ConnectController(CANPORT u8PortNumber,
                                           uchar8  ucBitRate)
{
    if (!bIsPortValid(u8PortNumber))
        return TFXCR_INVALID_PORT;
    
    // Initialize Controller
    return TFXDRV_i16InitController(u8PortNumber,
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
    CANBUF_tzRX_BUF zRxBuf;      // Intermediate object
    
    if (!bIsPortValid(u8PortNumber))
        return TFXCR_INVALID_PORT;
    // check for port and buffer for data
    
    // get CAN Message Object from buffer (*2 is since each port has 2 buffers)
    DRVAPI_vDisable_int(u8PortNumber); // Disable interrupts
    if ( CANBUF_fnRxExtract( u8PortNumber,
                             &zRxBuf ) != CANBUF_BUFFER_EMPTY )
    {
        DRVAPI_vEnable_int(u8PortNumber); // enable interrupts
        DRVAPI_fnBufferToCanData( &zRxBuf.zCanFrame,
                                  &pi16Frame->m_CanData );
        return ( TFXCR_NEW_DATA );
    }
    DRVAPI_vEnable_int(u8PortNumber); // enable interrupts
    return TFXCR_NO_DATA;
    
}
//**********************************************************************
// TFXDRV_i16ReceiveForeignFrame
//**********************************************************************
TFXCAN_RETURNS TFXDRV_i16ReceiveForeignFrame( CANPORT u8PortNumber,
                                              CANDRV_tzCAN_FRAME *pi16Frame )
{
    CANBUF_tzRX_BUF zRxBuf;      // Intermediate object
    
    if (!bIsPortValid(u8PortNumber))
        return TFXCR_INVALID_PORT;
    // check for port and buffer for data
    
    // get CAN Message Object from buffer (*2 is since each port has 2 buffers)
    DRVAPI_vDisable_int(u8PortNumber); // Disable interrupts
    if ( CANBUF_fnRxExtract( u8PortNumber,
                             &zRxBuf ) != CANBUF_BUFFER_EMPTY )
    {
        DRVAPI_vEnable_int(u8PortNumber); // enable interrupts
        ( void )memcpy( pi16Frame,
                        &zRxBuf.zCanFrame,
                        sizeof( CANDRV_tzCAN_FRAME ) );
        return ( TFXCR_NEW_DATA );
    }
    DRVAPI_vEnable_int(u8PortNumber); // enable interrupts
    return TFXCR_NO_DATA;
    
}

//**********************************************************************
// 
//**********************************************************************
TFXCAN_RETURNS TFXDRV_i16SendFrameCheck(CANPORT u8PortNumber)
{
    if (!bIsPortValid(u8PortNumber))
        return TFXCR_INVALID_PORT;
    
    // check for buffer available to accept frame for transmission
    if ( CANBUF_fnTxFull( u8PortNumber ) )
    {
        // Message cannot be qued before return try to get message from que to xmit object
        // this will start transmit interrupt sequence going   [chg rcl 06/10/01]
		return ( TFXDRV_WriteFrame( u8PortNumber ) );
    }
    
    // All OK return
    return TFXCR_OK;
}

//**********************************************************************
// 
//**********************************************************************
TFXCAN_RETURNS TFXDRV_i16SendFrame (CANPORT u8PortNumber, CANFRAME *pFrame, INT16 *pi16MessageID )
{
    TFXCAN_RETURNS ret;
    int iSts;
    CANBUF_tzTX_BUF zTxBuf;
    
    // Check for port and buffer available to accept frame for transmission
    ret = TFXDRV_i16SendFrameCheck( u8PortNumber );
    if ( ret != TFXCR_OK )
    {
        return (ret);
    }
    
    // Place PDU in sCanTxObject
	DRVAPI_fnCanDataToBuffer( &pFrame->m_CanData, &zTxBuf.zCanFrame );

    // Try to obtain a message handle
    ret = ( TFXCAN_RETURNS ) iGet_msg_handle( pi16MessageID );
    
    // If message handle was obtained
    if ( ret == TFXCR_OK)
    {
        // Got a message handle.
        zTxBuf.uiMsgHandle = *pi16MessageID;
        
        DRVAPI_vDisable_int(u8PortNumber); // Disable interrupts
        iSts = CANBUF_fnTxInsert( u8PortNumber, &zTxBuf );
        DRVAPI_vEnable_int(u8PortNumber);  // enable interrupts
        if( iSts == CANBUF_BUFFER_FULL )
        {
            // The circular buffer is full.  We have to drop this message.
            DRVAPI_fnCountTxLost( u8PortNumber );
            (void) TFXDRV_iClear_msg_handle(*pi16MessageID);
            return TFXCR_MSG_NOT_HANDLED;
        }

        // Try to output a frame
		(void) TFXDRV_WriteFrame( u8PortNumber );
    } // end of if get message handle is valid.

    // Return result of handle get or frame write
    return ret;
}
//**********************************************************************
// 
//**********************************************************************
TFXCAN_RETURNS TFXDRV_i16SendForeignFrame( CANPORT u8PortNumber, 
                                           CANDRV_tzCAN_FRAME *pFrame, 
                                           INT16 *pi16MessageID )
{
    TFXCAN_RETURNS ret;
    int iSts;
    CANBUF_tzTX_BUF zTxBuf;
    
    // Check for port and buffer available to accept frame for transmission
    ret = TFXDRV_i16SendFrameCheck( u8PortNumber );
    if ( ret != TFXCR_OK )
    {
        return (ret);
    }
    
    // Place PDU in sCanTxObject
    ( void )memcpy( &zTxBuf.zCanFrame,
                    pFrame,
                    sizeof( CANDRV_tzCAN_FRAME ) ); 

    // Try to obtain a message handle
    ret = ( TFXCAN_RETURNS ) iGet_msg_handle( pi16MessageID );
    
    // If message handle was obtained
    if ( ret == TFXCR_OK)
    {
        // Got a message handle.
        zTxBuf.uiMsgHandle = *pi16MessageID;
        
        DRVAPI_vDisable_int(u8PortNumber); // Disable interrupts
        iSts = CANBUF_fnTxInsert( u8PortNumber, &zTxBuf );
        DRVAPI_vEnable_int(u8PortNumber);  // enable interrupts
        if( iSts == CANBUF_BUFFER_FULL )
        {
            // The circular buffer is full.  We have to drop this message.
            DRVAPI_fnCountTxLost( u8PortNumber );
            (void) TFXDRV_iClear_msg_handle(*pi16MessageID);
            return TFXCR_MSG_NOT_HANDLED;
        }

        // Try to output a frame
		(void) TFXDRV_WriteFrame( u8PortNumber );
    } // end of if get message handle is valid.

    // Return result of handle get or frame write
    return ret;
}

//**********************************************************************
// 
//**********************************************************************
TFXCAN_RETURNS TFXDRV_WriteFrame(CANPORT u8PortNumber)
{
    CANBUF_tzTX_BUF zTxBuf;
	int iSts;
	TFXCAN_RETURNS ret = TFXCR_OK;

    DRVAPI_vDisable_int(u8PortNumber); // Disable interrupts
    if(DRVAPI_bOK_to_send(u8PortNumber))
    {
        iSts = CANBUF_fnTxExtract( u8PortNumber,
                                   &zTxBuf );
        if( iSts != CANBUF_BUFFER_EMPTY )
        {
            if( DRVAPI_iXmit_object( u8PortNumber, &zTxBuf ) != 0 )
            {
                // Failed to transmit the message.
                DRVAPI_fnCountTxLost( u8PortNumber );
                (void) TFXDRV_iClear_msg_handle( zTxBuf.uiMsgHandle );
                ret = TFXCR_MSG_NOT_HANDLED;
            }
        }
    }
    else
    {
        // No data to send
        ret = TFXCR_DRV_BUSY;
    }

    DRVAPI_vEnable_int(u8PortNumber); // enable interrupts
    return ret;
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
