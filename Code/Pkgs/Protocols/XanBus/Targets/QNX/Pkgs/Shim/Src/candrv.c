/*=============================================================================

Copyright � 2014 Xantrex Technology Inc.

This file is the property of Xantrex Technology Inc. and shall not be reproduced,
copied, or used as the basis for the manufacture or sale of equipment without the
express written permission of Xantrex Technology Inc.

==============================================================================*/

/*============================================================================*/
/*!

@file  candrv.c

@brief
    Controller Area Network (CAN) driver.

@details
    This module provides an interface to the CAN device on /dev/can0
    It uses POSIX devctl interface to interact with the low
    level CAN driver

*//*

*/

/*==============================================================================
                              Includes
==============================================================================*/
#include "LIB_stdtype.h"   /* Standard type definitions */
#include "LIB_stddefs.h"   /* Standard libaray definitions */
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <devctl.h>
#include "can_dcmd.h"  /* from platform/bsp */
#include "candrv.h"
#include "DevCan.h"

/*==============================================================================
                              Defines
==============================================================================*/

/*! Number of supported CAN interfaces */
#define CAN_MAX_CHANNELS            ( 1u )

/*! name of the CAN read device */
#define CAN_READ_DEVICE   "/dev/can0/rx0"

/*! name of the CAN write device */
#define CAN_WRITE_DEVICE  "/dev/can0/tx1"

/*==============================================================================
                               Macros
==============================================================================*/
/*! Fault confinement bit field offset, refer to register FLEXCANx_ESR1 */
#define FLT_CONF_OFFSET                 ( 4 )

/*! Fault confinement bit field mask, refer to register FLEXCANx_ESR1 */
#define FLT_CONF_MASK                   ( 0x3 << FLT_CONF_OFFSET )

/*! ERROR ACTIVE code in fault confinement bit field */
#define FLT_CONF_ERROR_ACTIVE           ( 0x00 )

/*! ERROR PASSIVE code in fault confinement bit field */
#define FLT_CONF_ERROR_PASSIVE          ( 0x01 )

/*! CAN Tx transmission timeout value */
#define CAN_TX_TIME_OUT_CNT_MAX         ( 200 )
/*==============================================================================
                                Enums
==============================================================================*/

/*! Enumeration of a CAN channel states */
typedef enum CANDRV_eSTATE
{
    /*! Define the off state */
    CANDRV_eSTATE_OFF       = 0u,
    /*! Define the disabled state */
    CANDRV_eSTATE_DISABLE   = 1u,
    /*! Define the sleep state */
    CANDRV_eSTATE_SLEEP     = 2u,
    /*! Define the active state */
    CANDRV_eSTATE_ACTIVE    = 3u,
} CANDRV_teSTATE;

/*=============================================================================
                              Structures
==============================================================================*/

/*! Define a CAN driver channel structure. */
typedef struct CANDRV_zCHANNEL
{
    /*! Define the channel state */
    CANDRV_teSTATE       eState;

    /*! Define an handle tracking variable */
    uchar8               aucHandle;

    /*! Define a callback function array. */
    CANDRV_tpfnCALLBACK  atpfnCallback[ CANDRV_eCB_LAST ];
} CANDRV_tzCHANNEL;


/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/

static CANDRV_tzCHANNEL *candrv_fnGetChanInfo( uchar8 ucChannel);

static int candrv_fnData2DrvMsg( CANDRV_tzCAN_FRAME *ptzSrcFrm,
                                 CAN_MSG *ptzDstMsg );

static int candrv_fnDrvMsg2Data( CAN_MSG *ptzSrcMsg,
                                 CANDRV_tzCAN_FRAME *ptzDstFrm );

static CANDRV_teRETURN candrv_fnGetDevSts( CANDRV_tzDEV_STATS* curDevSts,
                                           CANDRV_tzDEV_STATS* nextDevSts,
                                           uint16* fltConf );

static void candrv_fnClearDevSts( uchar8 ucChannel );

static CANDRV_teSTATUS candrv_fnBusOffStateTransition(
                                                CANDRV_tzDEV_STATS* curDevSts,
                                                CANDRV_tzDEV_STATS* nextDevSts,
                                                uint16 fltConf );
static CANDRV_teSTATUS candrv_fnErrStateTransition(
                                                CANDRV_tzDEV_STATS* curDevSts,
                                                CANDRV_tzDEV_STATS* nextDevSts,
                                                uint16 fltConf );

static CANDRV_teSTATUS candrv_fnWarnStateTransition(
                                                CANDRV_tzDEV_STATS* nextDevSts,
                                                uint16 fltConf );

static CANDRV_teSTATUS candrv_fnOKStateTransition(
                                                CANDRV_tzDEV_STATS* nextDevSts,
                                                uint16 fltConf );

static CANDRV_teSTATUS candrv_fnGetBusSts( CANDRV_tzDEV_STATS* curDevSts,
                                           CANDRV_tzDEV_STATS* nextDevSts,
                                           uint16 fltConf );
/*==============================================================================
                           Local/Private Variables
==============================================================================*/

/*! Define the global channel info array. */
static CANDRV_tzCHANNEL candrv_tzChannelInfo[ CAN_MAX_CHANNELS ];

/*! can driver file read descriptor */
static int can_read_fd = -1;

/*! can driver write file descriptor */
static int can_write_fd = -1;

/*! CAN device status */
static CANDRV_tzDEV_STATS candrv_tzDevStats[ CAN_MAX_CHANNELS ];

/*==============================================================================
                           Local/Private Constants
==============================================================================*/

/*==============================================================================
                           Function Definitions
==============================================================================*/

/*============================================================================*/
/* CANDRV_fnInit                                                              */
/*!

    Initialize the CAN module for a given channel

@param[in]
    ucChannel
        is the channel number

@param[in]
    teBitRate
        is an enum of the bit rates supported

@param[in]
    teFilterMode
        indicates if the filter is to be set to pass everything or nothing

@retval CANDRV_eSUCCESS if all goes well
@retval CANDRV_eBIT_RATE_INVALID invalid bit rate
@retval CANDRV_eFILTER_INVALID invalid filter mode
@retval CANDRV_eRET_FAILURE failed to initialize the CAN driver

*//*
REVISION HISTORY:

Version: 1.01    Date: 14-Feb-2017    By: Jon Liu
    - Added candrv_fnClearDevSts function call to clear the CAN device status
      global variable

Version: 1.00    Date: 19-Nov-2016    By: Trevor Monk
    - Created

*/
/*============================================================================*/
CANDRV_teRETURN CANDRV_fnInit( uchar8               ucChannel,
                               CANDRV_teBIT_RATE    teBitRate,
                               CANDRV_teFILTER_MODE teFilterMode )
{
    int result = CANDRV_eRET_FAILURE;

    /* Validate input parameters */
    if( CAN_MAX_CHANNELS <= ucChannel )
    {
        result = CANDRV_eRET_BAD_CHANNEL;
    }
    else if( CANDRV_eBIT_RATE_INVALID <= teBitRate )
    {
        result = CANDRV_eRET_BAD_BIT_RATE;
    }
    else if( CANDRV_eFILTER_INVALID <= teFilterMode )
    {
        result = CANDRV_eRET_BAD_FILTER;
    }
    else
    {
        /* Open CAN device for writing */
        can_write_fd = open( CAN_WRITE_DEVICE, O_RDWR | O_NONBLOCK );
        if ( can_write_fd == -1 )
        {
            fprintf( stderr, "Failed to open " CAN_WRITE_DEVICE "\n" );
        }
        else
        {
            /* open CAN device for reading */
            can_read_fd = open( CAN_READ_DEVICE, O_RDWR | O_NONBLOCK );
            if( can_read_fd == -1 )
            {
                fprintf( stderr, "Failed to open " CAN_READ_DEVICE "\n" );
                close( can_write_fd );
            }
            else
            {
                /* all is great! */
                candrv_fnClearDevSts( ucChannel );
                result = CANDRV_eRET_SUCCESS;
            }
        }
    }

    return( result );
}

/*============================================================================*/
/* CANDRV_fnReceive                                                           */
/*!

    Receive a CAN frame

    Check if there is a new frame in the MSCAN module and if there is, copy
    it into the driver buffer pointed to by pRxFrame

@param[in]
    ucChannel is the number of the CAN module to use, usually zero.

@param[in]
    pRxFrame points to the destination frame structure

@retval CANDRV_eRET_SUCCESS if all goes well
@retval CANDRV_eRET_NO_DATA if no frame has been received

*//*
REVISION HISTORY:

Version: 1.01    Date: 14-Feb-2017    By: Jon Liu
    - Added error handling for reading the CAN frame.

Version: 1.00    Date: 10-Nov-2016    By: Trevor Monk
    - Created

*/
/*============================================================================*/
CANDRV_teRETURN CANDRV_fnReceive( uchar8 ucChannel,
                                  CANDRV_tzCAN_FRAME *ptzRxFrame )
{
    CANDRV_teRETURN result = CANDRV_eRET_NO_DATA;
    CAN_MSG zCanMsg;
    ssize_t ret;

    /* Validate input parameters. */
    if( CAN_MAX_CHANNELS <= ucChannel )
    {
        result = CANDRV_eRET_BAD_CHANNEL;
    }
    else if( ptzRxFrame == NULL )
    {
        result = CANDRV_eRET_FAILURE;
    }
    else if ( can_read_fd == -1 )
    {
        result = CANDRV_eRET_FAILURE;
    }
    else
    {
        /* read a CAN frame */
        ret = devctl( can_read_fd,
                      CAN_DEVCTL_READ_CANMSG_EXT,
                      &zCanMsg,
                      sizeof( zCanMsg ),
                      NULL );

        if( ret == EOK )
        {
            /* convert CAN_Msg to CAN_FRAME */
            if( candrv_fnDrvMsg2Data( &zCanMsg, ptzRxFrame ) == EOK )
            {
                result = CANDRV_eRET_SUCCESS;
            }
        }
        else
        {
            /* TODO: fix this issue: CBG-257 */
            //fprintf( stderr, "Unable to receive CAN frame: (%d) %s",
            //         ret, strerror(ret));
        }
    }

    return result;
}

/*============================================================================*/
/* CANDRV_fnTransmit                                                          */
/*!

    Transmit CAN frame to bus.

    The CANDRV_fnTransmit function converts the Xanbus CANDRV_tzCAN_FRAME
    to a QNX CAN_MSG structure and writes it out to the CAN driver.

@param[in]
    ucChannel
        is the number of the CAN module to use, usually zero.

@param[in]
    pTxFrame
        points to the driver frame to be transmitted

@param[in]
    pHandle
        points to where the user wants the 'Handle' of the transmitted
        frame.  The handle is a bitmask that indicates which transmit buffer
        was used.  If the frame doesn't make it onto the bus on time, it
        may be necessary to abort it.  This handle bitmask will be used
        to abort the frame.  (not used)

@retval CANDRV_eRET_SUCCESS if all goes well
@retval CANDRV_eRET_TX_BUSY if no transmit buffers are available
@retval CANDRV_eRET_BAD_CHANNEL if an invalid CAN channel was specified

*//*
REVISION HISTORY:

Version: 1.00    Date: 19-Nov-2016    By: Trevor Monk / Kaushal Chokhawala
    - Created

*/
/*============================================================================*/
CANDRV_teRETURN CANDRV_fnTransmit( uchar8              ucChannel,
                                   CANDRV_tzCAN_FRAME *ptzTxFrame,
                                   uchar8             *pucHandle )
{
    CANDRV_tzCHANNEL *ptzChnInfo = NULL;
    CANDRV_teRETURN result = CANDRV_eRET_TX_BUSY;
    CAN_MSG zCanMsg;
    ssize_t ret;

    /* Validate input parameters. */
    if( CAN_MAX_CHANNELS <= ucChannel )
    {
        result = CANDRV_eRET_BAD_CHANNEL;
    }
    else
    {
        /* Convert the frame to the sub-driver-message frame. */
        if( candrv_fnData2DrvMsg( ptzTxFrame, &zCanMsg ) == EOK )
        {
            /* send the CAN frame to the driver */
            ret = devctl( can_write_fd,
                          CAN_DEVCTL_TX_FRAME_RAW,
                          &zCanMsg, sizeof( zCanMsg ),
                          NULL );
            if ( ret != EOK )
            {
                fprintf( stderr,
                         "Unable to send raw frame: (%d) %s\n",
                         ret,
                         strerror( ret ) );

                result = CANDRV_eRET_TX_BUSY;
            }
            else
            {

                /* We are not using the message handle - set it to zero */
                if( pucHandle != NULL )
                {
                    *pucHandle = 0;
                }

                result = CANDRV_eRET_SUCCESS;
            }

            /* Get the channel info */
            ptzChnInfo = candrv_fnGetChanInfo( ucChannel );
            if( ptzChnInfo != NULL )
            {
                if( result == CANDRV_eRET_SUCCESS )
                {
                    ptzChnInfo->eState = CANDRV_eSTATE_ACTIVE;
                }
                else
                {
                    ptzChnInfo->eState = CANDRV_eSTATE_OFF;
                }
            }
        }
    }

    return result;
}

/*============================================================================*/
/* candrv_fnDataId2DrvMsgId                                                   */
/*!

    Convert Xanbus CAN frame to QNX driver CAN frame

    The candrv_fnDataId2DrvMsgId function converts the CANDRV_tzCAN_FRAME
    structure used by the Xanbus stack into a CAN_MSG used by the QNX driver.

    Info: (CANDRV_tzCAN_ID)
    [ IDE, SRR, RTR, [ 28 : 18 = standard ] [ 17 : 0 = zeros ] ]
    [ IDE, SRR, RTR, [ 28 : 0 = extended                     ] ]
    IDE = extended identifier flag
    SRR = substitute remote request flag
    RTR = remote transmission request

@param[in]
    ptzSrcFrm
        pointer to the Xanbus CAN frame to be transmitted

@param[out]
    ptzDstMsg
        pointer to the QNX CAN frame to be populated from the Xanbus CAN frame

@retval EOK frame constructed successfully
@retval EINVAL invalid arguments

*//*
REVISION HISTORY:

Version: 1.00    Date: 19-Nov-2016    By: Trevor Monk / Kaushal Chokhawala
    - Created

*/
/*============================================================================*/
static int candrv_fnData2DrvMsg( CANDRV_tzCAN_FRAME *ptzSrcFrm,
                                 CAN_MSG *ptzDstMsg )
{
    int result = EINVAL;
    CANDRV_tzCAN_ID *ptzSrcId;
    uint32 id;
    int i;

    if( ( ptzSrcFrm != NULL ) &&
        ( ptzDstMsg != NULL ) )
    {
        /* get a pointer to the Xanbus CAN ID structure */
        ptzSrcId = &ptzSrcFrm->tzCanId;

        /* construct the CAN_MSG identifier (MID) */
        id = ( ptzSrcId->ucId28_24 << 24 ) | ( ptzSrcId->ucId23_16 << 16 ) |
             ( ptzSrcId->ucId15_8  <<  8 ) | ( ptzSrcId->ucId07_00 );

        /* mask off the upper 3 bits to create a 29-bit CAN identifier */
        id &= 0x1FFFFFFF;

        /* store the CAN message identifier */
        ptzDstMsg->mid = id;

        /* set the CAN message length */
        ptzDstMsg->len = ptzSrcFrm->ucByteCount;

        /* set the extended CAN message information */
        ptzDstMsg->ext.is_extended_mid = 1;
        ptzDstMsg->ext.is_remote_frame = 0;
        ptzDstMsg->ext.timestamp = 0;

        /* copy the data bytes from the Xanbus CAN frame to the CAN message */
        for(i=0;i<CANDRV_MAX_BYTES;i++)
        {
            ptzDstMsg->dat[i] = ptzSrcFrm->aucData[i];
        }

        /* done */
        result = EOK;
    }

    return result;
}

/*============================================================================*/
/* candrv_fnDrvMsg2Data                                                       */
/*!

    Converting a received frame

    The candrv_fnDrvMsg2Data function converts the raw CAN_MSG frame received
    from the QNX driver to a CANDRV_tzCAN_FRAME needed by the Xanbus stack.

@param[in]
    ptzSrcMsg
        pointer to the raw frame received from the QNX driver

@param[out]
    ptzDstFrm
        pointer to the Xanbus CAN frame needed by the Xanbus stack

@retval EOK successfully constructed CANDRV_tzCAN_FRAME from CAN_MSG
@retval EINVAL invalid arguments

*//*
REVISION HISTORY:

Version: 1.00    Date: 19-Nov-2016    By: Trevor Monk / Kaushal Chokhawala
    - Created

*/
/*============================================================================*/
static int candrv_fnDrvMsg2Data( CAN_MSG *ptzSrcMsg,
                                 CANDRV_tzCAN_FRAME *ptzDstFrm )
{
    int result = EINVAL;
    CANDRV_tzCAN_ID *ptzDstId;
    uint32           id;
    int i;

    if( ( ptzSrcMsg != NULL ) &&
        ( ptzDstFrm != NULL ) )
    {
        /* get pointer to the Xanbus CAN frame ID structure */
        ptzDstId = &ptzDstFrm->tzCanId;

        /* get the ID from the QNX driver's CAN_MSG */
        id = ptzSrcMsg->mid;

        /* extract the CAN message identifier */
        ptzDstId->ucId28_24  = ( id >> 24 ) & 0x1F;
        ptzDstId->ucId23_16  = ( id >> 16 ) & 0xFF;
        ptzDstId->ucId15_8   = ( id >>  8 ) & 0xFF;
        ptzDstId->ucId07_00  = ( id & 0xFF );

        /* get the message length */
        ptzDstFrm->ucByteCount = ptzSrcMsg->len;

        /* get the message payload data */
        for( i = 0 ; i < CAN_MSG_DATA_MAX ; i++ )
        {
            ptzDstFrm->aucData[i]  = ptzSrcMsg->dat[i];
        }

        /* success! */
        result = EOK;
    }

    return result;
}


/*============================================================================*/
/* CANDRV_fnEnable                                                            */
/*!

    Enable CAN driver

    The CANDRV_fnEnable function enables the specified CAN channel

@param[in]
    ucChannel
        unused, cause there is only one channel

@return
    CANDRV_eRET_SUCCESS if all goes well


*//*
REVISION HISTORY:

Version: 1.00    Date: 07-Oct-2014    By: Trevor Monk
    - Created

*/
/*============================================================================*/
CANDRV_teRETURN CANDRV_fnEnable( uchar8 ucChannel )
{
    return( CANDRV_eRET_SUCCESS );
}

/*============================================================================*/
/* CANDRV_fnDisable                                                           */
/*!

    Disable the CAN driver

    The CANDRV_fnDisable function disables the indicated CAN channel.

@param[in]
    ucChannel
        is the number of the CAN module to disable, usually zero.

@return
    CANDRV_eRET_SUCCESS if all goes well

*//*

REVISION HISTORY:

Version: 1.00    Date: 07-Oct-2014    By: Trevor Monk
    - Created

*/
/*============================================================================*/
CANDRV_teRETURN CANDRV_fnDisable( uchar8 ucChannel )
{
    return( CANDRV_eRET_SUCCESS );
}


/*============================================================================*/
/*! CANDRV_fnSleep                                                            */
/*!

    Put the CAN channel to sleep.

    The CANDRV_fnSleep function is not used by the QNX driver

@param[in]
    ucChannel
        is the number of the CAN module to put to sleep, usually zero.

@param[in]
    WakeEnable is TRUE if the module is to automatically wake up if there
        is traffic on the CAN bus.

@return
    CANDRV_eRET_SUCCESS if all goes well

*//*
REVISION HISTORY:

Version: 1.00    Date: 07-Oct-2014    By: Trevor Monk
    - Created

*/
/*============================================================================*/
CANDRV_teRETURN CANDRV_fnSleep( uchar8  ucChannel,
                                tucBOOL tucWakeEnable )
{
    return( CANDRV_eRET_SUCCESS );
}


/*============================================================================*/
/* CANDRV_fnWakeUp                                                            */
/*!

    Wake channel up from sleep

    The CANDRV_fnWakeUp function is not used by the QNX driver

@param[in]
    ucChannel
        is the number of the CAN module to wake up, usually zero.

@return
    CANDRV_eRET_SUCCESS if all goes well

*//*
REVISION HISTORY:

Version: 1.00    Date: 07-Oct-2014    By: Trevor Monk
    - Created

*/
/*============================================================================*/
CANDRV_teRETURN CANDRV_fnWakeUp( uchar8 ucChannel )
{
    return( CANDRV_eRET_SUCCESS );
}


/*============================================================================*/
/* CANDRV_fnTransmitOK                                                        */
/*!

    Check if it is ok to transmit

    See if at least one transmit buffer is empty and ready for use.
    This function is used to check the availability of the hardware before
    removing a buffer from a queue.

@param[in]
    ucChannel
        is the number of the CAN module to use, usually zero.

@retval CANDRV_eRET_SUCCESS if at least one transmit buffer is idle
@retval CANDRV_eRET_TX_BUSY if no transmit buffers are available

*//*
REVISION HISTORY:

Version: 1.00    Date: 07-Oct-2014    By: Trevor Monk
    - Created

*/
/*============================================================================*/
CANDRV_teRETURN CANDRV_fnTransmitOk( uchar8 ucChannel )
{
    CANDRV_teRETURN result = CANDRV_eRET_SUCCESS;

    // Validate input parameters.
    if( CAN_MAX_CHANNELS <= ucChannel )
    {
        result = CANDRV_eRET_BAD_CHANNEL;
    }

    return result;
}

/*============================================================================*/
/* CANDRV_fnAbort                                                             */
/*!

    Abort a transmitted frame

    The CANDRV_fnAbort is intended to abort transmission of a frame
    which is pending transmission.  The QNX implementation does not
    support this action.

@param[in]
    ucChannel is the number of the CAN module to use, usually zero.

@param[in]
    Handle is an index to a transmit buffer

@retval CANDRV_eRET_SUCCESS if all goes well
@retval CANDRV_eRET_BAD_CHANNEL an invalid CAN channel was specified

*//*
REVISION HISTORY:

Version: 1.00    Date: 07-Oct-2014    By: Trevor Monk
    - Created

*/
/*============================================================================*/
CANDRV_teRETURN CANDRV_fnAbort( uchar8 ucChannel,
                                uchar8 ucHandle )
{
    CANDRV_teRETURN result = CANDRV_eRET_SUCCESS;

    // Validate input parameters.
    if( CAN_MAX_CHANNELS <= ucChannel )
    {
        result = CANDRV_eRET_BAD_CHANNEL;
    }

    return result;
}


/*============================================================================*/
/* CANDRV_fnTxStatus                                                          */
/*!

    Get transmitter status and optional error count

    The CANDRV_fnTxStatus function is not supported by the QNX implementation

@param[in]
    ucChannel
        is the number of the CAN module to use, usually zero.

@param[in]
    pErrCount
        points to where to send the error count.
        If NULL, it won't be assigned.

@retval CANDRV_eSTATUS_OK status was successfully retrieved
@retval CANDRV_eSTATUS_INVALID an invalid channel was specified

*//*
REVISION HISTORY:

Version: 1.00    Date: 07-Oct-2014    By: Trevor Monk
    - Created

*/
/*============================================================================*/
CANDRV_teSTATUS CANDRV_fnTxStatus( uchar8  ucChannel,
                                   uchar8 *pucErrCount )
{
    CANDRV_teSTATUS result = CANDRV_eSTATUS_OK;

    // Validate input parameters.
    if( CAN_MAX_CHANNELS <= ucChannel )
    {
        result = CANDRV_eSTATUS_INVALID;
    }

    return result;
}


/*============================================================================*/
/* CANDRV_fnRxStatus                                                          */
/*!

    Get receiver status and optional error count

    The CANDRV_fnRxStatus function is not supported by the QNX implementation

@param[in]
    ucChannel
        is the number of the CAN module to use, usually zero.

@param[in]
    pucErrCount
        points to where to send the error count.  If NULL, it won't be
        assigned

@retval CANDRV_eSTATUS_OK status was successfully retrieved
@retval CANDRV_eSTATUS_INVALID an invalid channel was specified

*//*
REVISION HISTORY:

Version: 1.00    Date: 07-Oct-2014    By: Trevor Monk
    - Created

*/
/*============================================================================*/
CANDRV_teSTATUS CANDRV_fnRxStatus( uchar8  ucChannel,
                                   uchar8 *pucErrCount )
{
    // Validate input parameters.
    if( CAN_MAX_CHANNELS <= ucChannel )
    {
        return( CANDRV_eSTATUS_INVALID );
    }

    return( CANDRV_eSTATUS_OK );
}


/*============================================================================*/
/* CANDRV_fnInitCbList                                                        */
/*!

    Initialize the callback list to all NULLs

    This function initializes the callback list to NULLs to disable
    all callback actions.

    Call this function before any CAN channels are initialized.

@param[in]
    None

@return
    None

*//*
REVISION HISTORY:

Version: 1.00    Date: 07-Oct-2014    By: Trevor Monk
    - Created

*/
/*============================================================================*/
void CANDRV_fnInitCbList( void )
{
    CANDRV_tzCHANNEL *ptzChnInfo;
    uchar8            ucChannel; // Counts through the channels
    uchar8            Index;     // Counts through the callback list

    for( ucChannel = 0; ucChannel < CAN_MAX_CHANNELS; ucChannel++ )
    {
        ptzChnInfo = candrv_fnGetChanInfo( ucChannel );
        for( Index = 0; Index < CANDRV_eCB_LAST; Index++ )
        {
            ptzChnInfo->atpfnCallback[ Index ] = NULL;
        }
    }
}

/*============================================================================*/
/* CANDRV_fnInstallCb                                                         */
/*!

    Install callbacks for handling driver actions

    Call this function after calling CAN_fnInitCBList and before any
    CAN channels are initialized.

    The callback functions are called while the interrupt is being
    handled so they should not do very much processing, mostly on the
    order of queueing buffers and setting flags for task-level
    processing later.

@param[in]
    ucChannel
        is the number of the CAN module to use, usually zero.

@param[in]
    CbType
        is an enum of what situation the function is to handle

@param[in]
    pFunc
        is the pointer to the callback function.

@retval CANDRV_eRET_SUCCESS on completion,
@retval failure code if the callback could not be installed

*//*
REVISION HISTORY:

Version: 1.00    Date: 07-Oct-2014    By: Trevor Monk
    - Created

*/
/*============================================================================*/
CANDRV_teRETURN CANDRV_fnInstallCb( uchar8 ucChannel,
                                    CANDRV_teCB_TYPE CbType,
                                    CANDRV_tpfnCALLBACK tpfnFunc )
{
    CANDRV_tzCHANNEL *ptzChnInfo;
    CANDRV_teRETURN result = CANDRV_eRET_FAILURE;

    /* Make sure the channel is in the right range */
    if( CAN_MAX_CHANNELS <= ucChannel )
    {
        /*  Index out of range */
        result = CANDRV_eRET_BAD_CHANNEL;
    }
    else if( CANDRV_eCB_LAST <= CbType )
    {
        /* invalid callback type */
        result = CANDRV_eRET_FAILURE;
    }
    else if( tpfnFunc == NULL )
    {
        /* NULL function pointer */
        result = CANDRV_eRET_FAILURE;
    }
    else
    {
        /* Install the pointer in the list */
        ptzChnInfo = candrv_fnGetChanInfo( ucChannel );
        if( ptzChnInfo != NULL )
        {
            ptzChnInfo->atpfnCallback[ CbType ] = tpfnFunc;
            result = CANDRV_eRET_SUCCESS;
        }
    }

    /* Complete */
    return result;
}

/*============================================================================*/
/* CANDRV_fnGetHandle                                                         */
/*!

    Get the handle of the latest transmitted buffer

    The CANDRV_fnGetHandle gets the handle of the latest transmitted
    message.  The QNX driver does not support this function and will
    always return 0.

@param[in]
    ucChannel
        is the number of the CAN module to use, usually zero.

@return
    Handle of the latest transmitted buffer

*//*
REVISION HISTORY:

Version: 1.00    Date: 07-Oct-2014    By: Trevor Monk
    - Created

*/
/*============================================================================*/
uchar8 CANDRV_fnGetHandle( uchar8 ucChannel )
{
    CANDRV_tzCHANNEL *ptzChnInfo;
    uchar8 result = 0;

    /* Make sure the channel is in the right range */
    if( CAN_MAX_CHANNELS <= ucChannel )
    {
        /* Index out of range */
        result = 0;
    }
    else
    {
        /* get the channel info */
        ptzChnInfo = candrv_fnGetChanInfo( ucChannel );
        if( ptzChnInfo != NULL )
        {
            return ptzChnInfo->aucHandle;
        }
    }

    /* TBD: fix undefined channel return value. */
    return result;
}

/*============================================================================*/
/* CANDRV_fnIsTransmitting                                                    */
/*!

    Check if we are transmitting ok

    The CANDRV_fnIsTransmitting function checks the state of the channel
    and indicates to the caller if the channel is successfully transmitting

    This function is upside down in the logic of the function name.
    is used in DRVAPI_fnIsBlocked returning the not of this function.
    The sense is to return the previous transmission is complete.

@param[in]
    ucChannel
        is the number of the CAN module to use, usually zero.

@retval TRUE if the transmitter is transmiting the frames successfully,
@retval FALSE if the transmitter has failed to send frames successfully

*//*

REVISION HISTORY:

Version: 1.00    Date: 07-Oct-2014    By: Trevor Monk
    - Created

*/
/*============================================================================*/
tucBOOL CANDRV_fnIsTransmitting( uchar8 ucChannel )
{
   CANDRV_tzCHANNEL *ptzChnInfo;
   tucBOOL result = FALSE;

   /* Make sure the channel is in the right range */
   if( CAN_MAX_CHANNELS <= ucChannel )
   {
       /*  Index out of range */
       result = FALSE;
   }
   else
   {
       /* Get the channel info */
       ptzChnInfo = candrv_fnGetChanInfo( ucChannel );
       if( ptzChnInfo != NULL )
       {
           /* If not active, it is not transmitting. */
           if( ptzChnInfo->eState == CANDRV_eSTATE_ACTIVE )
           {
               result = TRUE;
           }
       }
   }

   // ok to transmit
   return result;
}

/*============================================================================*/
/* CANDRV_fnIsBusOn                                                           */
/*!

    Get the status of Bus On/Off bit in CAN driver

@param[in]
    ucChannel is the number of the CAN module to use, usually zero.

@retval TRUE if Bus Status bit is Bus On
@retval FALSE if Bus Status bit is Bus Off

@todo We need to figure out how to do this with the QNX driver!!

*//*
REVISION HISTORY:

Version: 1.00    Date: 07-Oct-2014    By: Trevor Monk
    - Created

Version: 1.01    Date: 10-Aug-2015    By: Trevor Monk
    - Changed comparison to determine if CAN bus is active

Version: 1.02    Date: 08-Dec-2016    By: Trevor Monk
    - Updated to support iMX6x CAN driver

*/
/*============================================================================*/
tucBOOL CANDRV_fnIsBusOn( uchar8 ucChannel )
{
    tucBOOL result = FALSE;
    int ret;
    struct can_devctl_error errors;

    ret = devctl( can_read_fd,
                  CAN_DEVCTL_ERROR,
                  &errors,
                  sizeof( struct can_devctl_error ),
                  NULL );
    if ( ret != EOK )
    {
        fprintf( stderr,
                 "Unable to get bus status: %s\n",
                 strerror( ret ) );
    }
    else
    {
        // If not active, Bus in not on.
        result = ( ( errors.drvr1 & 0x20 ) == 0x20 ) ? FALSE : TRUE;
    }

    return result;
}

/*============================================================================*/
/* CANDRV_fnExitResetMode                                                     */
/*!

    Exit Reset Mode

    This function is not used by the QNX driver implementation

@param[in]
    ucChannel
        is the number of the CAN module to use, usually zero.

@return
    none

*//*
REVISION HISTORY:

Version: 1.00    Date: 07-Oct-2014    By: Trevor Monk
    - Created

*/
/*============================================================================*/
void CANDRV_fnExitResetMode( uchar8 ucChannel )
{
}

/*============================================================================*/
/* CANDRV_fnGetDevStatus                                                      */
/*!

@brief
    Obtaining CAN device status.

    Obtaining CAN device status from hardware and copy the status data to
    the global control structure and the CANDRV_tzDEV_STATS variable pointed
    by ptrCANDevSts.

@param[in]
    ucChannel
        a CAN channel number

@param[out]
    ptrCANDevSts
        the pointer pointed to CANDRV_tzDEV_STATS variable. The global CAN
        device status data will be copy to this variable.

@retval CANDRV_eRET_SUCCESS if everything is working correctly.
@retval CANDRV_eRET_FAILURE if obtaining device status failed.

*//*

REVISION HISTORY:

Version: 1.00    Date: 14-Feb-2017    By: Jon Liu
    - Created

*/
/*============================================================================*/
CANDRV_teRETURN CANDRV_fnGetDevStatus( uchar8 ucChannel,
                                       CANDRV_tzDEV_STATS* ptrCANDevSts )
{
    int ret = CANDRV_eRET_FAILURE;
    CANDRV_tzDEV_STATS nextCANSts;
    CANDRV_tzDEV_STATS* ptrCurCANSts;
    uint16 fltConf = 0;

    if( ucChannel < CAN_MAX_CHANNELS )
    {
        /* Clear the local CANDRV_tzDEV_STATS variable */
        memset( &nextCANSts, 0, sizeof( CANDRV_tzDEV_STATS ) );
        ptrCurCANSts = &candrv_tzDevStats[ ucChannel ];

        /* Obtain the CAN hardware status */
        ret = candrv_fnGetDevSts( ptrCurCANSts, &nextCANSts, &fltConf );

        if( ret == EOK  )
            {
            /* Get CAN bus state */
            nextCANSts.ucLastTxStatus = candrv_fnGetBusSts( ptrCurCANSts,
                                                            &nextCANSts,
                                                            fltConf );

            /* Copy the local CAN status to global CAN status structure */
            memcpy( ptrCurCANSts, &nextCANSts, sizeof( CANDRV_tzDEV_STATS ) );
            memcpy( ptrCANDevSts, ptrCurCANSts, sizeof( CANDRV_tzDEV_STATS ) );
        }
    }

    return ret;
}

/*============================================================================*/
/* candrv_fnChanInfoGet                                                       */
/*!

    Get a pointer to the CAN channel structure for a given channel.

    The candrv_fnChanInfoGet returns a pointer to the CANDRV_tzCHANNEL
    structure for the specified CAN channel.

@param[in]
    ucChannel
        a CAN channel number

@retval pointer to the CANDRV_tzCHANNEL structure for the specified CAN channel.
@retval NULL if the channel info could not be found

*//*

REVISION HISTORY:

Version: 1.00    Date: 28-Feb-2012    By: Marlo Gothe
    - Created

*/
/*============================================================================*/
static CANDRV_tzCHANNEL *candrv_fnGetChanInfo( uchar8 ucChannel )
{
    if( CAN_MAX_CHANNELS <= ucChannel )
    {
        return( NULL );
    }

    return( &candrv_tzChannelInfo[ ucChannel ] );
}

/*============================================================================*/
/* candrv_fnGetDevSts                                                         */
/*!

@brief
    Fill the CAN state data structure with data read from Low level driver.

    Fill the CAN state data structure with total tx transmitted frames count,
    total rx received frames count, tx error count and rx error count.
    Also the fault confinement value will be obtained.

@param[in]
    curDevSts
        The current CANDRV_tzDEV_STATS variable that holds the CAN device
        status.

@param[out]
    nextDevSts
        The newly updated CANDRV_tzDEV_STATS variable. This variable will be
        filled with CAN device status read from low level driver.

@param[out]
    fltConf
        Fault confinement value read from low level CAN driver.

@retval CANDRV_eRET_SUCCESS if everything is working correctly.
@retval CANDRV_eRET_FAILURE if obtaining device status failed.

*//*

REVISION HISTORY:

Version: 1.00    Date: 14-Feb-2017    By: Jon Liu
    - Created

*/
/*============================================================================*/
static CANDRV_teRETURN candrv_fnGetDevSts( CANDRV_tzDEV_STATS* curDevSts,
                                           CANDRV_tzDEV_STATS* nextDevSts,
                                           uint16* fltConf )
{
    CAN_DEVCTL_STATS canDevCtlSts;
    struct can_devctl_error errors;
    int ret = CANDRV_eRET_FAILURE;

    if( ( nextDevSts != NULL ) && ( curDevSts != NULL ) )
    {
        nextDevSts->uiTxTimoutCnt = curDevSts->uiTxTimoutCnt;

        ret = devctl( can_write_fd,
                      CAN_DEVCTL_GET_STATS,
                      &canDevCtlSts,
                      sizeof( CAN_DEVCTL_STATS ),
                      NULL );
        if( ret == EOK )
        {
            /* Copy the total tx and rx frames */
            nextDevSts->ulTxExtFrames = canDevCtlSts.transmitted_frames;
            nextDevSts->ulRxExtFrames = canDevCtlSts.received_frames;

            ret = devctl( can_read_fd,
                          CAN_DEVCTL_ERROR,
                          &errors,
                          sizeof( struct can_devctl_error ),
                          NULL );

            if( ret == EOK )
            {
                /* errors.drvr1: The value of FLEXCANx_ESR1 register.
                 * errors.drvr3: Rx error counter.
                 * errors.drvr4: Tx error counter.
                */
                nextDevSts->uiTxErrCount = (uint16)errors.drvr4;
                nextDevSts->uiRxErrCount = (uint16)errors.drvr3;
                *fltConf = ( errors.drvr1 & FLT_CONF_MASK ) >> FLT_CONF_OFFSET;
            }
            else
            {
                fprintf(stderr, "Unable to obtain CAN error information: "
                                "(%d) %s\n", ret, strerror( ret ) );
                ret = CANDRV_eRET_FAILURE;
            }
        }
        else
        {
            fprintf(stderr, "Unable to obtain CAN status: "
                            "(%d) %s\n", ret, strerror( ret ) );
            ret = CANDRV_eRET_FAILURE;
        }
    }

    return ret;
}

/*============================================================================*/
/* candrv_fnGetBusSts                                                         */
/*!

@brief
    Running the state machine to determine the bus state.

    The state machine based on LastTxStatus is running to determine the current
    CAN bus status. The BUSOFF status is special. It consists of 2 case. One is
    as defined by CAN bus protocol, if txErrCnt or rxErrCnt > 255, then bus is
    in BUSOFF status.The other case is no other CAN active node is on the bus.

@param[in]
    curDevSts
        The current CANDRV_tzDEV_STATS variable that holds the CAN device
        status.

@param[in]
    nextDevSts
        The newly updated CANDRV_tzDEV_STATS variable. When this variable is
        filled with proper data, curDevSts will be updated by nextDevSts.

@retval CANDRV_teSTATUS This is the CAN bus state.

*//*

REVISION HISTORY:

Version: 1.00    Date: 14-Feb-2017    By: Jon Liu
    - Created

*/
/*============================================================================*/
static CANDRV_teSTATUS candrv_fnGetBusSts( CANDRV_tzDEV_STATS* curDevSts,
                                           CANDRV_tzDEV_STATS* nextDevSts,
                                           uint16 fltConf )
{
    CANDRV_teSTATUS stsNext = CANDRV_eSTATUS_INVALID;

    switch ( curDevSts->ucLastTxStatus )
    {
        case CANDRV_eSTATUS_OK:
            nextDevSts->uiTxTimoutCnt = 0;
            stsNext = candrv_fnOKStateTransition( nextDevSts, fltConf );
            break;
        case CANDRV_eSTATUS_WARNING:
            nextDevSts->uiTxTimoutCnt = 0;
            stsNext = candrv_fnWarnStateTransition( nextDevSts, fltConf );
            break;
        case CANDRV_eSTATUS_ERROR:
            if( ( curDevSts->uiTxErrCount == nextDevSts->uiTxErrCount ) &&
                ( curDevSts->uiRxErrCount == nextDevSts->uiRxErrCount) )
            {
                nextDevSts->uiTxTimoutCnt++;
            }
            stsNext = candrv_fnErrStateTransition( curDevSts,
                                                   nextDevSts,
                                                   fltConf );
            break;
        case CANDRV_eSTATUS_BUSOFF:
            nextDevSts->uiTxTimoutCnt = 0;
            stsNext = candrv_fnBusOffStateTransition( curDevSts,
                                                      nextDevSts,
                                                      fltConf );
            break;
        default:
            break;
    }

    return stsNext;
}

/*============================================================================*/
/* candrv_fnClearDevSts                                                       */
/*!

@brief
    Clear CAN device status global variable to zero.

@param[in]
    ucChannel
        A CAN channel to be cleared.

@return None

*//*

REVISION HISTORY:

Version: 1.00    Date: 14-Feb-2017    By: Jon Liu
    - Created

*/
/*============================================================================*/
static void candrv_fnClearDevSts( uchar8 ucChannel )
{
    if( ucChannel < CAN_MAX_CHANNELS )
    {
        memset( &( candrv_tzDevStats[ ucChannel ] ),
                0,
                sizeof( CANDRV_tzDEV_STATS ) );
    }
}

/*============================================================================*/
/* candrv_fnOKStateTransition                                                 */
/*!

@brief
    Determine the next state for the CAN bus if the current state is OK.

@param[in]
    nextDevSts
        The CAN device status variable that required to determine the next state

@param[in]
    fltConf
        the fault confinement value

@retval CANDRV_teSTATUS The next state.

*//*

REVISION HISTORY:

Version: 1.00    Date: 14-Feb-2017    By: Jon Liu
    - Created

*/
/*============================================================================*/
static CANDRV_teSTATUS candrv_fnOKStateTransition(
                                                CANDRV_tzDEV_STATS* nextDevSts,
                                                uint16 fltConf )
{
    CANDRV_teSTATUS stsNext = CANDRV_eSTATUS_INVALID;

    if( ( fltConf == FLT_CONF_ERROR_ACTIVE ) &&
        ( nextDevSts->uiTxErrCount == 0 ) &&
        ( nextDevSts->uiRxErrCount == 0 ) )
    {
        stsNext = CANDRV_eSTATUS_OK;
    }
    else if( ( fltConf == FLT_CONF_ERROR_ACTIVE ) &&
              ( ( nextDevSts->uiTxErrCount != 0) ||
                ( nextDevSts->uiRxErrCount != 0) ) )
    {
        stsNext = CANDRV_eSTATUS_WARNING;
    }
    else if( ( fltConf == FLT_CONF_ERROR_PASSIVE) )
    {
        stsNext = CANDRV_eSTATUS_ERROR;
    }
    else
    {
        stsNext = CANDRV_eSTATUS_BUSOFF;
    }

    return stsNext;
}

/*============================================================================*/
/* candrv_fnWarnStateTransition                                               */
/*!

@brief
    Determine the next state for the CAN bus if the current state is WARNING.

@param[in]
    nextDevSts
        The CAN device status variable that required to determine the next state

@param[in]
    fltConf
        the fault confinement value

@retval CANDRV_teSTATUS The next state.

*//*

REVISION HISTORY:

Version: 1.00    Date: 14-Feb-2017    By: Jon Liu
    - Created

*/
/*============================================================================*/
static CANDRV_teSTATUS candrv_fnWarnStateTransition(
                                                CANDRV_tzDEV_STATS* nextDevSts,
                                                uint16 fltConf )
{
    CANDRV_teSTATUS stsNext = CANDRV_eSTATUS_INVALID;

    if( ( fltConf == FLT_CONF_ERROR_ACTIVE ) &&
        ( nextDevSts->uiTxErrCount == 0 ) &&
        ( nextDevSts->uiRxErrCount == 0 ) )
    {
        stsNext = CANDRV_eSTATUS_OK;
    }
    else if( ( fltConf == FLT_CONF_ERROR_ACTIVE ) &&
              ( ( nextDevSts->uiTxErrCount != 0) ||
                ( nextDevSts->uiRxErrCount != 0) ) )
    {
        stsNext = CANDRV_eSTATUS_WARNING;
    }
    else if( fltConf == FLT_CONF_ERROR_PASSIVE )
    {
        stsNext = CANDRV_eSTATUS_ERROR;
    }
    else
    {
        stsNext = CANDRV_eSTATUS_BUSOFF;
    }

    return stsNext;
}

/*============================================================================*/
/* candrv_fnErrStateTransition                                                */
/*!

@brief
    Determine the next state for the CAN bus if the current state is ERROR.

@param[in]
    curDevSts
        The CAN device status variable (previous state)
        that required to determine the next state

@param[in]
    nextDevSts
        The CAN device status variable (newly updated)
        that required to determine the next state

@param[in]
    fltConf
        the fault confinement value

@retval CANDRV_teSTATUS The next state.

*//*

REVISION HISTORY:

Version: 1.00    Date: 14-Feb-2017    By: Jon Liu
    - Created

*/
/*============================================================================*/
static CANDRV_teSTATUS candrv_fnErrStateTransition(
                                                CANDRV_tzDEV_STATS* curDevSts,
                                                CANDRV_tzDEV_STATS* nextDevSts,
                                                uint16 fltConf )
{
    CANDRV_teSTATUS stsNext = CANDRV_eSTATUS_INVALID;

    if( ( fltConf == FLT_CONF_ERROR_ACTIVE ) &&
        ( nextDevSts->uiTxErrCount == 0 ) &&
        ( nextDevSts->uiRxErrCount == 0 ) )
    {
        stsNext = CANDRV_eSTATUS_OK;
    }
    else if( ( fltConf == FLT_CONF_ERROR_ACTIVE ) &&
              ( ( nextDevSts->uiTxErrCount != 0) ||
                ( nextDevSts->uiRxErrCount != 0) ) )
    {
        stsNext = CANDRV_eSTATUS_WARNING;
    }
    else if( ( fltConf == FLT_CONF_ERROR_PASSIVE ) &&
             ( nextDevSts->uiTxTimoutCnt <= CAN_TX_TIME_OUT_CNT_MAX ) )
    {
        stsNext = CANDRV_eSTATUS_ERROR;
    }
    else
    {
        stsNext = CANDRV_eSTATUS_BUSOFF;
    }

    return stsNext;
}

/*============================================================================*/
/* candrv_fnBusOffStateTransition                                             */
/*!

@brief
    Determine the next state for the CAN bus if the current state is BUSOFF.

@param[in]
    curDevSts
        The CAN device status variable (previous state)
        that required to determine the next state

@param[in]
    nextDevSts
        The CAN device status variable (newly updated)
        that required to determine the next state

@param[in]
    fltConf
        the fault confinement value

@retval CANDRV_teSTATUS The next state.

*//*

REVISION HISTORY:

Version: 1.00    Date: 14-Feb-2017    By: Jon Liu
    - Created

*/
/*============================================================================*/
static CANDRV_teSTATUS candrv_fnBusOffStateTransition(
                                                CANDRV_tzDEV_STATS* curDevSts,
                                                CANDRV_tzDEV_STATS* nextDevSts,
                                                uint16 fltConf )
{
    CANDRV_teSTATUS stsNext = CANDRV_eSTATUS_INVALID;

    if( ( fltConf == FLT_CONF_ERROR_ACTIVE ) &&
        ( nextDevSts->uiTxErrCount == 0 ) &&
        ( nextDevSts->uiRxErrCount == 0 ) )
    {
        stsNext = CANDRV_eSTATUS_OK;
    }
    else if( ( fltConf == FLT_CONF_ERROR_ACTIVE ) &&
              ( ( nextDevSts->uiTxErrCount != 0) ||
                ( nextDevSts->uiRxErrCount != 0) ) )
    {
        stsNext = CANDRV_eSTATUS_WARNING;
    }
    else if( ( fltConf == FLT_CONF_ERROR_PASSIVE ) &&
             ( ( curDevSts->ulTxExtFrames != nextDevSts->ulTxExtFrames) ||
               ( curDevSts->ulRxExtFrames != nextDevSts->ulRxExtFrames) ) )
    {
        stsNext = CANDRV_eSTATUS_ERROR;
    }
    else
    {
        stsNext = CANDRV_eSTATUS_BUSOFF;
    }

    return stsNext;
}
