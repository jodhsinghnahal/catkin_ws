/*=============================================================================

Copyright (c) 2017 Schneider Electric SE. All rights reserved.

All trademarks are owned or licensed by Schneider Electric SE,
its subsidiaries or affiliated companies.

================================================================================*/
/*!
\file  CDrive.c

\brief 
    CAN driver interface to XanBus stack

\details
    This module acts as a shim between the Xanbus stack and the STM32F4xx's
    HAL CAN driver.  It sends and receives CAN frames and gets the current
    status of the CAN bus.

\author Tom Lightfoot
        
*/
/*============================================================================*/

/*==============================================================================
                              Includes
==============================================================================*/

#include <stdio.h>
#include <string.h>

#include "xbudefs.h"
#include "xbgdefs.h"

#include "libenv.h"         // Teleflex interface
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "cdrive.h"         // Interface to this file

// Uncomment the line below to use diagnostic messages
// #define CDRIVE_USE_DIAG
#ifdef CDRIVE_USE_DIAG
#include "diag.h"
#else
#define DIAG_fnPrint(...)
#endif  // defined CDRIVE_USE_DIAG

/*==============================================================================
                              Defines
==============================================================================*/

#define CDRIVE_TX_Q_DEPTH  (16)
#define CDRIVE_RX_Q_DEPTH  (20)

/*==============================================================================
                            Type Definitions
==============================================================================*/


/*==============================================================================
                                Enums
==============================================================================*/


/*=============================================================================
                              Structures
==============================================================================*/


/*==============================================================================
                           Local/Private Constants
==============================================================================*/

/*==============================================================================
                           Local/Private Variables
==============================================================================*/

// Array of communications statistics records
static CAN_tzCOMM_STATS atzCanCommStats[ MAX_PORTS ];

// Mail queue for outgoing frames
osMailQDef(CanTxQueue, CDRIVE_TX_Q_DEPTH, CANDATA);
static osMailQId cdrive_TxQueueId = NULL;

// Mail queue for incoming frames
osMailQDef(CanRxQueue, CDRIVE_RX_Q_DEPTH, CANDATA);
static osMailQId cdrive_RxQueueId = NULL;

// CAN handle, needed by request for status outside of receive or transmit tasks
static CAN_HandleTypeDef *cdrive_pCanHandle = NULL;

// The ID of the transmit task
static osThreadId cdrive_hTxTask;

// Receive and transmit frames as used by the HAL driver
static CanRxMsgTypeDef cdrive_tzHalRxFrame;
static CanTxMsgTypeDef cdrive_tzHalFrame;

/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/

static BOOL bIsPortValid( CANPORT u8PortNumber );

/*==============================================================================
                           Function Definitions
==============================================================================*/

/*******************************************************************************

FUNCTION NAME: 
    TFXDRV_i16InitBuffers
  
PURPOSE: 
    Initialize buffers for holding CAN messages.

INPUTS: 
    'u8PortNumber' is number representing the device hardware port

OUTPUTS:
    returns TFXCR_OK

NOTES:
    Not used by this driver

*******************************************************************************/

TFXCAN_RETURNS TFXDRV_i16InitBuffers(CANPORT u8PortNumber )
{
    // Teleflex message buffers not used
    return TFXCR_OK;
}

/*******************************************************************************

FUNCTION NAME: 
    TFXDRV_i16InitController
  
PURPOSE: 
    Initialize CAN controller hardware.

INPUTS: 
    'u8PortNumber' is number representing the device hardware port

OUTPUTS:
    returns TFXCR_OK on success
    returns TFXCR_INVALID_PORT if port is invalid
    returns TFXCR_PORT_INIT_ERR and prints diagnostic for other errors

NOTES:

*******************************************************************************/

TFXCAN_RETURNS TFXDRV_i16InitController(CANPORT u8PortNumber,
                                        uchar8 ucBitRate)
{
    CAN_FilterConfTypeDef FilterConfig;
    HAL_StatusTypeDef hal_status;
    int waits = 0;

    // check port and initialize appropriate controller
    if (!bIsPortValid(u8PortNumber))
        return TFXCR_INVALID_PORT;

    // Wait for transmit task to record HAL handle for the CAN interface
    while ((cdrive_pCanHandle == NULL) && (waits++ < 2))
    {
        osDelay(10);
    }

    if (cdrive_pCanHandle == NULL)
    {
        DIAG_fnPrint(DIAG_eLEV_ERROR, "Null handle for CAN\r\n");
        return TFXCR_PORT_INIT_ERR;
    }
    
    // Set up filtering
    FilterConfig.FilterNumber = 0;
    FilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    FilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    FilterConfig.FilterIdHigh = 0x0000;
    FilterConfig.FilterIdLow = 0x0000;
    FilterConfig.FilterMaskIdHigh = 0x0000;
    FilterConfig.FilterMaskIdLow = 0x0000;
    FilterConfig.FilterFIFOAssignment = 0;
    FilterConfig.FilterActivation = ENABLE;
    FilterConfig.BankNumber = 14;
    hal_status = HAL_CAN_ConfigFilter(cdrive_pCanHandle, &FilterConfig);
    if (hal_status != HAL_OK)
    {
        DIAG_fnPrint(DIAG_eLEV_ERROR, "Could not set up CAN filter 0: %d\r\n", hal_status);
        return TFXCR_PORT_INIT_ERR;
    }

    // FIFO 1 filters everything out so that we're just looking at FIFO0
    FilterConfig.FilterNumber = 1;
    FilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    FilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    FilterConfig.FilterIdHigh = 0x0000;
    FilterConfig.FilterIdLow = 0x0000;
    FilterConfig.FilterMaskIdHigh = 0xFFFF;
    FilterConfig.FilterMaskIdLow = 0xFFFF;
    FilterConfig.FilterFIFOAssignment = 1;
    FilterConfig.FilterActivation = ENABLE;
    FilterConfig.BankNumber = 15;
    hal_status = HAL_CAN_ConfigFilter(cdrive_pCanHandle, &FilterConfig);
    if (hal_status != HAL_OK)
    {
        DIAG_fnPrint(DIAG_eLEV_ERROR, "Could not set up CAN filter 1: %d\r\n", hal_status);
        return TFXCR_PORT_INIT_ERR;
    }
    
    // Set up the queue for frames sent by this task to the application
    cdrive_RxQueueId = osMailCreate(osMailQ(CanRxQueue), NULL);

    // Link in the receive frame structure
    cdrive_pCanHandle->pRxMsg = &cdrive_tzHalRxFrame;
    
    hal_status = HAL_CAN_Receive_IT(cdrive_pCanHandle, CAN_FIFO0);
    if (hal_status != HAL_OK)
    {
        DIAG_fnPrint(DIAG_eLEV_ERROR, "Could not enable reception interrupt: %d\r\n", hal_status);
        return TFXCR_PORT_INIT_ERR;
    }

    //--- Set up transmit ---
    // Set up the queue for frames coming in to transmit task
    cdrive_TxQueueId = osMailCreate(osMailQ(CanTxQueue), NULL);
    
    // Set up the transmit message structure
    cdrive_pCanHandle->pTxMsg = &cdrive_tzHalFrame;
    cdrive_tzHalFrame.IDE = CAN_ID_EXT;
    cdrive_tzHalFrame.RTR = CAN_RTR_DATA;
    
    // Success
    return TFXCR_OK;
}

/*******************************************************************************

FUNCTION NAME: 
    TFXDRV_i16ControllerState
  
PURPOSE: 
    Return controller state information

INPUTS: 
    'u8PortNumber' is number representing the device hardware port

OUTPUTS:
    returns TFXCR_OK

NOTES:

*******************************************************************************/

TFXCAN_RETURNS TFXDRV_i16ControllerState(CANPORT u8PortNumber)
{
    // check port and initialize appropriate controller
    if (!bIsPortValid(u8PortNumber))
    {
        return TFXCR_INVALID_PORT;
    }

    if (cdrive_pCanHandle == NULL)
    {
        return TFXCR_OK;
    }

    if (__HAL_CAN_GET_FLAG(cdrive_pCanHandle, CAN_FLAG_BOF))
    {
        return TFXCR_BUS_OFF;
    }

    if (__HAL_CAN_GET_FLAG(cdrive_pCanHandle, CAN_FLAG_EWG))
    {
        return TFXCR_BUS_WARNING;
    }

    if (__HAL_CAN_GET_FLAG(cdrive_pCanHandle, CAN_FLAG_EPV))
    {
        return TFXCR_BUS_WARNING;
    }
    
    return TFXCR_OK;
}

/*******************************************************************************

FUNCTION NAME: 
    TFXDRV_i16ConnectController
  
PURPOSE: 
    Reinitialize CAN controller 

INPUTS: 
    'u8PortNumber' is number representing the device hardware port

OUTPUTS:
    returns result of TFXDRV_i16InitController

NOTES:

VERSION HISTORY:

Version: 1.00  Date: Feb 27, 2003  By: dalem
    - first version

*******************************************************************************/

TFXCAN_RETURNS TFXDRV_i16ConnectController(CANPORT u8PortNumber,
                                           uchar8  ucBitRate)
{
    // Initialize Controller
    return TFXDRV_i16InitController(u8PortNumber,
                                    ucBitRate);
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

Version: 1.00  Date: 01/10/05  By: dalem
    - Created

*******************************************************************************/

void TFXDRV_fnStatusTick( CANPORT Channel )
{
    return;
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

Version: 1.00  Date: 01/10/05  By: dalem
    - Created

*******************************************************************************/

tucBOOL TFXDRV_fnIsBlocked( CANPORT Channel )
{
    return ( FALSE );
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
    // Validate channel number
    if ( Channel >= MAX_PORTS )
    {
        //  Index out of range
        return TFXCR_INVALID_PORT;
    }

    // Clear communications statics for the channel
    memset( &atzCanCommStats[ Channel ], 0, sizeof( CAN_tzCOMM_STATS ) );

    // Success
    return TFXCR_OK;
}

/*******************************************************************************

FUNCTION NAME: 
    TFXDRV_i16ReceiveFrame
  
PURPOSE: 
    Poll for a received frame

INPUTS: 
    'u8PortNumber' is number representing the device hardware port
    'pFrame' is a pointer to copy CAN message into

OUTPUTS:
    returns TFXCR_INVALID_PORT if port is not supported
    returns TFXCR_NEW_DATA if data is read and ready
    returns TFXCR_NO_DATA if no data available
    returns TFXCR_MSG_NOT_HANDLED and prints diagnostic if some other error

NOTES:

*******************************************************************************/

TFXCAN_RETURNS TFXDRV_i16ReceiveFrame( CANPORT u8PortNumber, CANFRAME *pFrame )
{
    osEvent evt;
    CANDATA *pQueueFrame;
    HAL_CAN_StateTypeDef CanState;
    
    if (!bIsPortValid(u8PortNumber))
        return TFXCR_INVALID_PORT;

    if (pFrame == NULL)
    {
        return TFXCR_NO_DATA;
    }
    
    // Check the receive queue in a non-blocking way
    evt = osMailGet(cdrive_RxQueueId, 0);
    if (evt.status == osOK)
    {
        // Nothing in the queue

        // Make sure we're still waiting to receive a frame
        CanState = HAL_CAN_GetState(cdrive_pCanHandle);
        if ((CanState != HAL_CAN_STATE_BUSY_RX0)
            && (CanState != HAL_CAN_STATE_BUSY_TX_RX0))
        {
            // If we're not actively waiting to receive a frame then
            // we're somehow out of sync and need to get back into listening
            HAL_CAN_Receive_IT(cdrive_pCanHandle, CAN_FIFO0);
        }
        
        /* DIAG_fnPrint(DIAG_eLEV_INFO, "CANRX: queue empty\r\n"); */
        return TFXCR_NO_DATA;
    }

    if (evt.status != osEventMail)
    {
        DIAG_fnPrint(DIAG_eLEV_ERROR, "CANRX: osMailGet returned %d\r\n", evt.status);
        return TFXCR_NO_DATA;
    }

    // Copy the received frame into the supplied buffer and indicate success
    pQueueFrame = (CANDATA *)evt.value.p;
    memcpy(&pFrame->m_CanData, pQueueFrame, sizeof(*pQueueFrame));
    osMailFree(cdrive_RxQueueId, pQueueFrame);
    
    return TFXCR_NEW_DATA;
}

/*******************************************************************************

FUNCTION NAME: 
    TFXDRV_i16SendFrameCheck
  
PURPOSE: 
    Check on sending of CAN frames

INPUTS: 
    'u8PortNumber' is number representing the device hardware port

OUTPUTS:
    returns TFXCR_OK

NOTES:
    Not used by this driver

VERSION HISTORY:

Version: 1.00  Date: Feb 27, 2003  By: dalem
    - first version

*******************************************************************************/

TFXCAN_RETURNS TFXDRV_i16SendFrameCheck(CANPORT u8PortNumber)
{
    if (!bIsPortValid(u8PortNumber))
        return TFXCR_INVALID_PORT;

    // All OK return
    return TFXCR_OK;
}

/*******************************************************************************

FUNCTION NAME: 
    TFXDRV_i16SendFrame
  
PURPOSE: 
    Queue a CAN frame for transmission

INPUTS: 
    'u8PortNumber' is number representing the device hardware port
    'pFrame' is a pointer to the CAN data be be sent
    'pi16MessageID' is a pointer to field to return a buffer handle (NOT USED)

OUTPUTS:
    returns TFXCR_INVALID_PORT if port is invalid
    returns TFXCR_MSG_NOT_HANDLED if isCAN return an error
    returns TFXCR_OK if message queue for sending

NOTES:

*******************************************************************************/

TFXCAN_RETURNS TFXDRV_i16SendFrame( CANPORT u8PortNumber,
                                    CANFRAME *pFrame,
                                    INT16 *pi16MessageID )
{
    CANDATA *ptzQueueFrame;

    if (pFrame == NULL)
    {
        return TFXCR_MSG_NOT_HANDLED;
    }

    if (cdrive_TxQueueId == NULL)
    {
        // Driver not initialized yet
        return TFXCR_DRV_BUSY;
    }

    ptzQueueFrame = (CANDATA *)osMailAlloc(cdrive_TxQueueId, 0);
    if (ptzQueueFrame == NULL)
    {
        return TFXCR_DRV_BUSY;
    }

    // Copy CANFRAME to CANDATA
    memcpy(ptzQueueFrame, &pFrame->m_CanData, sizeof(*ptzQueueFrame));
    osMailPut(cdrive_TxQueueId, ptzQueueFrame);

    return TFXCR_OK;
}


/*******************************************************************************

FUNCTION NAME: 
    bIsPortValid
  
PURPOSE: 
    Check if port is supported

INPUTS: 
    'u8PortNumber' is number representing the device hardware port

OUTPUTS:
    returns TRUE if port is valid
            FALSE if port is not valid

NOTES:

VERSION HISTORY:

Version: 1.00  Date: Feb 27, 2003  By: dalem
    - first version

*******************************************************************************/

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

*******************************************************************************/

TFXCAN_RETURNS TFXDRV_fnSleep( uchar8 Channel,
                                uchar8 WakeEnable )
{
    HAL_CAN_Sleep(cdrive_pCanHandle);   

    return TFXCR_OK;
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

*******************************************************************************/

TFXCAN_RETURNS TFXDRV_fnInstallWakeupCb( uchar8 Channel,
                                    TFXDRV_tpCALLBACK pFunc )
{
    return( TFXCR_OK );
}

/*******************************************************************************

FUNCTION NAME:
    TFXDRV_fnWakeUp

PURPOSE:
    Wake up CAN channel.

INPUTS:
    Channel is the number of the CAN module to put to wakeup, usually zero.

OUTPUTS:
    TFXCR_OK if all goes well

NOTES:

*******************************************************************************/

TFXCAN_RETURNS TFXDRV_fnWakeUp( uchar8 Channel )
{
    HAL_CAN_WakeUp(cdrive_pCanHandle);

    return( TFXCR_OK );
}


/*============================================================================*/
/*!

\brief
    Initialize the list of handles for PGNS being transmitted

\note
    This driver doesn't use message handles

*/
/*============================================================================*/

void TFXDRV_vInit_msg_handles( void )
{
}

/*============================================================================*/
/*!

\brief
    Release the handle of a frame that is timed out or otherwise needs to be aborted

\param[in] iHandle handle for outgoing PGN

\retval TFXCR_INVALID_MSG_HANDLE

\note
    This driver doesn't use message handles

*/
/*============================================================================*/

TFXCAN_RETURNS  TFXDRV_iClear_msg_handle( INT16 iHandle )
{
    return TFXCR_INVALID_MSG_HANDLE;
}

/*============================================================================*/
/*!

\brief
    Get the status of a message being transmitted by the hardware

\param[in]  iHandle                 handle for outgoing PGN
\param[out] puiStatus               Captures status of outgoing PGN
\param[out] piNumFramesTransmitted  Captures number of frames successfully transmitted for the PGN

\retval TFXCR_INVALID_MSG_HANDLE

\note
    This driver doesn't use message handles

*/
/*============================================================================*/

TFXCAN_RETURNS  TFXDRV_iGet_msg_status(INT16 iHandle, UINT16 *puiStatus, INT16 *piNumFramesTransmitted)
{
    return TFXCR_INVALID_MSG_HANDLE;
}

/*============================================================================*/
/*!

\brief
    The task for transmitting CAN frames to the CAN port

\param[in] pvParam points to CAN handle used by HAL functions

\details
    TFXDRV_i16SendFrame sends CAN frames to a mail queue and this task
    feeds those frames to the driver in a sequential, interrupt-driven manner.
    After sending the frame to the driver, this task waits for a signal from
    the CAN ISR's callback to send the next frame.

\note
    This task uses the ARM-centric CMSIS interface to the RTOS, which is
    typically FreeRTOS in the STM32 toolchain.

*/
/*============================================================================*/

void CDRIVE_fnTxTask(void const *pvParam)
{
    CAN_HandleTypeDef *pHandle = (CAN_HandleTypeDef *)pvParam;
    CANDATA *ptzFrame;
    HAL_StatusTypeDef hal_status;
    int i;
    osEvent evt;

    // Save the handle
    cdrive_pCanHandle = pHandle;
    cdrive_hTxTask = osThreadGetId();

    for (;;)
    {
        // Get the next frame to transmit
        evt = osMailGet(cdrive_TxQueueId, osWaitForever);
        if (evt.status == osEventMail)
        {
            // Load it up into the driver
            ptzFrame = (CANDATA *)evt.value.p;
            cdrive_tzHalFrame.ExtId = (ptzFrame->m_u8Priority << 26)
                + (ptzFrame->m_u8DataPage << 24)
                + (ptzFrame->m_u8PF << 16)
                + (ptzFrame->m_u8PS << 8)
                + ptzFrame->m_u8SA;
            cdrive_tzHalFrame.DLC = ptzFrame->m_u8DataByteCount;
            for (i = 0; i < 8; i++)
            {
                cdrive_tzHalFrame.Data[i] = ptzFrame->m_u8Data[i];
            }

            // Free up the memory used by the frame structure in the queue
            osMailFree(cdrive_TxQueueId, ptzFrame);

            // Send the frame and wait for the callback to signal that transmit is complete
            // This really shouldn't take long, unless the cable is unplugged
            hal_status = HAL_CAN_Transmit_IT(pHandle);
            if (hal_status == HAL_OK)
            {
                // Wait a finite time for transmit to complete.  If there is a timeout
                // then the cable is probably unplugged because there would be no other
                // device to provide an acknowledgement.
                evt = osSignalWait(SIG_TX_READY, 20);
                if (evt.status == osEventTimeout)
                {
                    DIAG_fnPrint(DIAG_eLEV_ERROR, "CAN transmit timeout\r\n");

                    // Cancel the frame
                    if (!__HAL_CAN_GET_FLAG(pHandle, CAN_FLAG_TME0))
                    {
                        __HAL_CAN_CANCEL_TRANSMIT(pHandle, CAN_TXMAILBOX_0);
                    }

                    if (!__HAL_CAN_GET_FLAG(pHandle, CAN_FLAG_TME1))
                    {
                        __HAL_CAN_CANCEL_TRANSMIT(pHandle, CAN_TXMAILBOX_1);
                    }

                    if (!__HAL_CAN_GET_FLAG(pHandle, CAN_FLAG_TME2))
                    {
                        __HAL_CAN_CANCEL_TRANSMIT(pHandle, CAN_TXMAILBOX_2);
                    }
                }
            }
            else
            {
                DIAG_fnPrint(DIAG_eLEV_ERROR, "CAN transmit failed: %d\r\n", hal_status);
            }
        }
    }
}

/*============================================================================*/
/*!

\brief
    Callback for the CAN transmit complete ISR

    This callback signals the CAN transmit task that it can send the next frame

\param[in] hcan points to the CAN module that generated the interrupt

*/
/*============================================================================*/

void HAL_CAN_TxCpltCallback(CAN_HandleTypeDef *hcan)
{
    if (hcan == cdrive_pCanHandle)
    {
        osSignalSet(cdrive_hTxTask, SIG_TX_READY);
    }
}

/*============================================================================*/
/*!

\brief
    Callback for the CAN receive ISR

    This callback retrieves the frame from the HAL driver and then puts
    it in the mail queue for TFXDRV_i16ReceiveFrame to pick up

\param[in] hcan points to the CAN module that generated the interrupt

*/
/*============================================================================*/

void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef *hcan)
{
    CanRxMsgTypeDef *ptzHalRxFrame;
    CANDATA *ptzFrame;
    int i;

    /* DIAG_fnPrint(DIAG_eLEV_INFO, "RXCB\r\n"); */
    if ((cdrive_RxQueueId == NULL) || (hcan->pRxMsg == NULL))
    {
        // CAN driver isn't initialized yet
        return;
    }

    // Put the received frame onto the queue
    ptzFrame = (CANDATA *)osMailAlloc(cdrive_RxQueueId, 0);
    if (ptzFrame == NULL)
    {
        // Queue is full
        DIAG_fnPrint(DIAG_eLEV_ERROR, "CAN receive queue full\r\n");
    }
    else
    {
        // Load up the structure
        ptzHalRxFrame = hcan->pRxMsg;
        ptzFrame->m_u8Priority = (uchar8)((ptzHalRxFrame->ExtId >> 26) & 0x07);
        ptzFrame->m_u8DataPage = (uchar8)((ptzHalRxFrame->ExtId >> 24) & 0x03);
        ptzFrame->m_u8PF = (uchar8)((ptzHalRxFrame->ExtId >> 16) & 0xff);
        ptzFrame->m_u8PS = (uchar8)((ptzHalRxFrame->ExtId >> 8) & 0xff);
        ptzFrame->m_u8SA = (uchar8)(ptzHalRxFrame->ExtId & 0xff);
        ptzFrame->m_u8DataByteCount = ptzHalRxFrame->DLC;
        for (i = 0; i < 8; i++)
        {
            ptzFrame->m_u8Data[i] = ptzHalRxFrame->Data[i];
        }
        
        // Send it on the queue
        /* DIAG_fnPrint(DIAG_eLEV_INFO, "Putting %02x\r\n", ptzFrame->aucData[0]); */
        if (osMailPut(cdrive_RxQueueId, ptzFrame) != osOK)
        {
            DIAG_fnPrint(DIAG_eLEV_ERROR, "osMailPut failed\r\n");
        }
    }

    // Set up for receipt of next frame
    if (HAL_CAN_Receive_IT(hcan, CAN_FIFO0) != HAL_OK)
    {
        DIAG_fnPrint(DIAG_eLEV_ERROR, "HAL_CAN_Receive_IT failed\r\n");
    }
}
