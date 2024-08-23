/*=============================================================================
  (c) 2017 - 2019 Xantrex Canada ULC. All rights reserved.
  All trademarks are owned or licensed by Xantrex Canada ULC,
  its subsidiaries or affiliated companies.

FILE NAME:  xbtmsg.c

PURPOSE:
    This is the message handling layer of the XanBus Tiny driver

FUNCTION(S):
    XBTMSG_fnLookupPgn      - Lookup PGN in PGN support list
    XBTMSG_fnNewMsg         - Get a free message buffer
    XBTMSG_fnReleaseMsg     - Free a busy message buffer
    XBTMSG_fnSendConnMan    - Send an ISO Connenction Management message
    XBTMSG_fnSendAck        - Send an ISO Acknowledgement PGN
    XBTMSG_fnSend           - Send a PGN
    XBTMSG_fnSendAddressed  - Send a PGN with addresses set
    XBTMSG_fnHandleMessages - Check if there are messages to send or receive
    XBTMSG_fnInit           - Initialize XanBus Tiny driver

    local:
        xbtmsg_fnRecvConnMan    - Receive an ISO Connection Management message
        xbtmsg_fnRecvIsoReq     - Receive an ISO Request for PGN message
        xbtmsg_fnProcessSingleframe- Send a singleframe message and check result
        xbtmsg_fnReceive        - Receive a message

=============================================================================*/

/*==============================================================================
                              Includes
==============================================================================*/

#include <string.h>

#include "xbudefs.h"
#include "xbtgdefs.h"
#include "xbtldefs.h"
#include "config.h"

/*==============================================================================
                           Local/Private Constants
==============================================================================*/

/*==============================================================================
                           Forward Declarations
==============================================================================*/

static void xbtmsg_fnRecvIsoReq( XBT_tzMSG *ptzMsg );   // Received message pointer


/*==============================================================================
                           Local/Private Variables
==============================================================================*/

/*
    List of supported PGNs specifying the protocol used for them
*/
static const XBT_tzPGN_LIST   katzPGN_LIST[] =
{
    //  PGN,
    //  Flags,
    //  Minumum Length,
    //  Receive function,
    //  Transmit function

    { XB_PGN_ISO_ACK,
      XBT_SINGLEFRAME,
      XB_DLC_ISO_ACK,
      NULL,
      NULL },

    { XB_PGN_ISO_REQ,
      XBT_SINGLEFRAME | XBT_ALLOW_OFFLINE,
      XB_DLC_ISO_REQ,
      xbtmsg_fnRecvIsoReq,
      NULL },

    { XB_PGN_ISO_TP_XFER,
      XBT_SINGLEFRAME,
      XB_DLC_ISO_TP_XFER,
      NULL,
      NULL },

    { XB_PGN_ISO_TP_CONN_MAN,
      XBT_SINGLEFRAME,
      XBT_FRAME_SIZE,
      NULL,
      NULL },

    { XB_PGN_ISO_ADDR_CLAIM,
      XBT_SINGLEFRAME | XBT_ALLOW_OFFLINE,
      XB_DLC_ISO_ADDR_CLAIM,
      XBTADDR_fnRecvClaim,
      XBTADDR_fnSendClaim },

    { XB_PGN_FILE_DNLD_BLK,
      XBT_MULTIFRAME | XBT_NO_RELEASE | XBT_DIAG_COMMAND | XBT_ACCESS_DENY | XBT_NO_BROADCAST,
      XB_DLC_FILE_DNLD_BLK,
      XBTLOAD_fnRecvLoadBlock,
      NULL },

    { XB_PGN_FILE_DNLD_INF,
      XBT_FASTPACKET | XBT_NO_RELEASE | XBT_DIAG_COMMAND | XBT_ACCESS_DENY | XBT_NO_BROADCAST,
      XB_DLC_FILE_DNLD_INF,
      XBTLOAD_fnRecvLoadInfo,
      NULL },

    { XB_PGN_FILE_DNLD_STS,
      XBT_SINGLEFRAME,
      XB_DLC_FILE_DNLD_STS,
      NULL,
      XBTLOAD_fnReqLoadStatus },

    { XB_PGN_RESET_CMD,
      XBT_SINGLEFRAME | XBT_NO_RELEASE | XBT_DIAG_COMMAND | XBT_ACCESS_DENY | XBT_ALLOW_OFFLINE,
      XB_DLC_RESET_CMD,
      XBTLOAD_fnRecvResetCmd,
      NULL },
};

/*
    Message buffers for sending PGNs
    There are send buffers set aside specifically for connection management
    and address claiming. Otherwise there is one buffer for single-frame PGNs
    and one buffer for fast-packet PGNs. There are no message buffers for
    multi-frame PGNs.
*/
static uchar8 aucConnMan[ XBT_SINGLEFRAME_MAX ];
static XBT_tzMSG tzConnManMsg =
{
    aucConnMan,                 // Data Pointer
    0,                          // Number of Frame Received Flags
    NULL,                       // Frame Received Flags
    XBT_SINGLEFRAME,            // Protocol
    XBT_PRIORITY_CONNMAN,       // Priority
    XB_PGN_ISO_TP_CONN_MAN,     // PGN
    sizeof( aucConnMan ),       // Data Length
};

static uchar8 aucAddrClaim[ XBT_SINGLEFRAME_MAX ];
static XBT_tzMSG tzAddrClaimMsg =
{
    aucAddrClaim,               // Data Pointer
    0,                          // Number of Frame Received Flags
    NULL,                       // Frame Receive Flags
    XBT_SINGLEFRAME,            // Protocol
    XBT_PRIORITY_NORMAL,        // Priority
    XB_PGN_ISO_ADDR_CLAIM,      // PGN
    sizeof( aucAddrClaim ),     // Data Length
};

static uchar8 aucSingleFrame1[ XBT_SINGLEFRAME_MAX ];
static XBT_tzMSG tzSingleFrame1Msg =
{
    aucSingleFrame1,            // Data Pointer
    0,                          // Number of Frame Received Flags
    NULL,                       // Frame Receive Flags
    XBT_SINGLEFRAME,            // Protocol
};

static uchar8 aucSingleFrame2[ XBT_SINGLEFRAME_MAX ];
static XBT_tzMSG tzSingleFrame2Msg =
{
    aucSingleFrame2,            // Data Pointer
    0,                          // Number of Frame Received Flags
    NULL,                       // Frame Receive Flags
    XBT_SINGLEFRAME,            // Protocol
};

static uchar8 aucSingleFrame3[ XBT_SINGLEFRAME_MAX ];
static XBT_tzMSG tzSingleFrame3Msg =
{
    aucSingleFrame3,            // Data Pointer
    0,                          // Number of Frame Received Flags
    NULL,                       // Frame Receive Flags
    XBT_SINGLEFRAME,            // Protocol
};

// Prioritized list of pointers to transmittable messages
XBT_tzMSG *aptzMsgTxList[] =
{
    &tzConnManMsg,
    &tzAddrClaimMsg,
    &tzSingleFrame1Msg,
    &tzSingleFrame2Msg,
    &tzSingleFrame3Msg,
    NULL
};

// List of messages to be used for transmission
XBT_tzMSG *aptzMsgAssignList[] =
{
    &tzSingleFrame1Msg,
    &tzSingleFrame2Msg,
    &tzSingleFrame3Msg,
    NULL
};


/*==============================================================================
                           Function Definitions
==============================================================================*/

/******************************************************************************

FUNCTION NAME:
    XBTMSG_fnLookupPgn

PURPOSE:
    Find PGN in PGN support list

INPUTS:
    'ulPgn' - PGN being searched for

OUTPUTS:
    Pointer to entry in PGN support list if PGN is found
    NULL is PGN is not found

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version

******************************************************************************/

const XBT_tzPGN_LIST *XBTMSG_fnLookupPgn( uint32 ulPgn )
{
    const XBT_tzPGN_LIST *ptzPgnList;
    const XBT_tzPGN_LIST *ptzEnd;

    // Get pointer to PGN Lookup array
    ptzPgnList = &katzPGN_LIST[ 0 ];

    // Get pointer to end of PGN Lookup array
    ptzEnd = &katzPGN_LIST[ DIM( katzPGN_LIST ) ];

    // While not at end of PGN list
    while( ptzPgnList < ptzEnd )
    {
        // If PGN matches one entry from the PGN list
        if( ptzPgnList->ulPgn == ulPgn )
        {
            // Success
            return( ptzPgnList );
        }

        // Try next entry
        ptzPgnList++;
    }

    // PGN not found in list
    return( NULL );
}

/******************************************************************************

FUNCTION NAME:
    XBTMSG_fnNewMsg

PURPOSE:
    Get an unused message buffer for sending a message

INPUTS:
    'ulPgn' - PGN of message to be sent

OUTPUTS:
    pointer to message buffer of appropriate size
    NULL if no buffer available

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version
Version 1.01  Date: 03/28/03  By: dalem
    - Changed separate protocol field to use flags field

******************************************************************************/

XBT_tzMSG *XBTMSG_fnNewMsg( uint32 ulPgn )
{
    const XBT_tzPGN_LIST *ptzPgnList;
    XBT_tzMSG            **pptzMsg;
    uchar8               ucFlags;

    // If this is an address claim message and it is not busy
    if( ( ulPgn == XB_PGN_ISO_ADDR_CLAIM ) && ( !tzAddrClaimMsg.ucBusy ) )
    {
        // Return address claim message buffer
        return( &tzAddrClaimMsg );
    }
    else
    {
        // Look for the requested PGN in support list
        ptzPgnList = XBTMSG_fnLookupPgn( ulPgn );

        // If PGN is supported
        if( ptzPgnList )
        {
            // Get local copy of protocol compatability flags
            ucFlags = ptzPgnList->ucFlags & XBT_PROTOCOL_MASK;

            // For every entry in message list array
            for( pptzMsg = aptzMsgAssignList; *pptzMsg != NULL; pptzMsg++ )
            {
                // If message buffer protocol matches and not busy
                if( ( ( (*pptzMsg)->ucFlags & ucFlags ) == ucFlags )
                   && ( !(*pptzMsg)->ucBusy ) )
                {
                    // Set PGN of message
                    (*pptzMsg)->ulPgn = ulPgn;

                    // Set message priority to normal
                    (*pptzMsg)->ucPriority = XBT_PRIORITY_NORMAL;

                    // Success, return this buffer
                    return( *pptzMsg );
                }
            }
        }
    }

    // Report an error
    XBTERROR_fnSaveError( XBT_ERR_NO_TX_BUFFER );

    // Failure
    return( NULL );
}

/******************************************************************************

FUNCTION NAME:
    XBTMSG_fnReleaseMsg

PURPOSE:
    Free a message buffer by marking it not busy

INPUTS:
    'ptzMsg' - pointer to message buffer to be freed

OUTPUTS:
    None

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version

******************************************************************************/

void XBTMSG_fnReleaseMsg( XBT_tzMSG *ptzMsg )
{
    // Free the received message
    ptzMsg->ucBusy = FALSE;
}

/******************************************************************************

FUNCTION NAME:
    XBTMSG_fnSendConnMan

PURPOSE:
    Build and then send a connection management message

INPUTS:
    'ucConnMan' - group function code specifying type of message to send
    'ptzMsg'    - pointer to a message that the connection refers to

OUTPUTS:
    None

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version
Version 1.01  Date: 05/14/03  By: dalem
    - Removed unused local variable

******************************************************************************/

void XBTMSG_fnSendConnMan( uchar8 ucConnMan,
                           XBT_tzMSG *ptzMsg )
{
    uchar8 *pucData;
    uchar8 ucDest;

    // Get local data pointer
    pucData = tzConnManMsg.pucData;

    // First byte is the group function
    pucData[0] = ucConnMan;

    // Last three bytes used for the PGN of multi-packet message
    pucData[5] = (uchar8) ( ptzMsg->ulPgn );
    pucData[6] = (uchar8) ( ptzMsg->ulPgn >> 8 );
    pucData[7] = (uchar8) ( ptzMsg->ulPgn >> 16 );

    // The other bytes of the connection message depend on type
    switch ( ucConnMan )
    {
        case XBT_CONN_CTS:
        {
            // Number of frames that can be sent (8 bits)
            pucData[1] = ( ptzMsg->ucEndFrame - ptzMsg->ucFrameNum ) + 1;
            // Number of next frame to be transmitted (8 bits)
            pucData[2] = ptzMsg->ucFrameNum;
            // Reserved bits
            pucData[3] = 0xFF;
            pucData[4] = 0xFF;

            // Send back to message source
            ucDest = ptzMsg->ucSrcAddr;
        }
        break;

        case XBT_CONN_EOM:
        {
            // Total message size (16 bits)
            pucData[1] = (uchar8) ( ptzMsg->uiDataLen );
            pucData[2] = (uchar8) ( ptzMsg->uiDataLen >> 8 );
            // Total number of frames received (8 bits)
            pucData[3] = ptzMsg->ucEndFrame;
            // Reserved bits
            pucData[4] = 0xFF;

            // Send back to message source
            ucDest = ptzMsg->ucSrcAddr;
        }
        break;

        case XBT_CONN_ABORT:
        {
            // Reserved bits
            pucData[1] = 0xFF;
            pucData[2] = 0xFF;
            pucData[3] = 0xFF;
            pucData[4] = 0xFF;

            // Send back to message source
            ucDest = ptzMsg->ucSrcAddr;
        }
        break;

        // XBT_CONN_RTS and XBT_CONN_BAM are not supported --
        default:
        {
            // No support for sending a multiframe message
            XBTERROR_fnSaveError( XBT_ERR_NOT_SUPPORTED );

            // Done
            return;
        }
    }

    // Send message
    XBTMSG_fnSend( &tzConnManMsg,
                   ucDest );
}

/******************************************************************************

FUNCTION NAME:
    xbtmsg_fnRecvConnMan

PURPOSE:
    Handle reception of an ISO Connection Management message

INPUTS:
    'ptzConn' - pointer to the received connection management message

OUTPUTS:
    None

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version
Version 1.01  Date: 05/14/03  By: dalem
    - Use macro to obtain PGN
Version 1.02  Date: 07/22/03  By: dalem
    - Nack message if not of minimum allowable size

******************************************************************************/

static void xbtmsg_fnRecvConnMan( XBT_tzMSG *ptzConn )
{
    uchar8 *pucData;
    uint32 ulPgn;
    uchar8 ucGrpFunc;
    XBT_tzMSG *ptzRxMsg;

    // If PGN does not meet minimum length requirement
    if( ptzConn->uiDataLen < XBT_FRAME_SIZE )
    {
        // Reply with a NACK
        XBTMSG_fnSendAck( ptzConn->ulPgn, XB_eACK_STS_NAK );

        // Cannot handle this message
        return;
    }

    // Get local data pointer
    pucData = ptzConn->pucData;

    // Last three bytes contain the PGN of the multi-packet message
    ulPgn = XBT_mMAKE_PGN( &pucData[ 5 ] );

    // First byte is the group function; action depends on group function
    ucGrpFunc = pucData[ 0 ];

    // Request To Send and Broadcast Announce Message
    // are for receiving a multi-packet
    switch ( ucGrpFunc )
    {
        case XBT_CONN_RTS:
        {
            // Prepare for receiving a multi-frame message
            ptzRxMsg = XBTFRAME_fnStartMultiFrameRx( ulPgn, ptzConn );

            // If the message buffer is OK
            if( ptzRxMsg != NULL )
            {
                // Send a Clear To Send connection message
                XBTMSG_fnSendConnMan( XBT_CONN_CTS, ptzRxMsg );
            }
            else
            {
                // Cannot handle this request, Send an Abort.
                // Need a message with the failed PGN in it for Abort.
                // Reuse the connection management message.
                // Set PGN
                ptzConn->ulPgn = ulPgn;

                // Send an Abort connection message
                XBTMSG_fnSendConnMan( XBT_CONN_ABORT, ptzConn );
            }
        }
        break;

        case XBT_CONN_BAM:
        {
            // Prepare for receiving a multi-frame message
            ptzRxMsg = XBTFRAME_fnStartMultiFrameRx( ulPgn, ptzConn );
        }
        break;

        case XBT_CONN_ABORT:
        {
            // Received request to abort reception of a multi-frame PGN
            // Simply abort the multi-frame PGN reception
            (void) XBTFRAME_fnAbortMultiFrameRx( ulPgn );
        }
        break;

        default:
        {
            // Do not support sending of a multi-frame message
            XBTERROR_fnSaveError( XBT_ERR_NOT_SUPPORTED );
        }
    }
}


/******************************************************************************

FUNCTION NAME:
    xbtmsg_fnRecvIsoReq

PURPOSE:
    Handle reception of an ISO Request for PGN message

INPUTS:
    'ptzMsg' - pointer to received message

OUTPUTS:
    None

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version
Version 1.01  Date: 03/28/03  By: dalem
    - Use flags field to determine for Access Denied
Version 1.02  Date: 05/14/03  By: dalem
    - Use macro to obtain PGN
Version 1.03  Date: 07/22/03  By: dalem
    - Do not send NACK if request was a broadcast
Version 1.04  Date: 12/15/03  By: Alan Travelbea
    - Send Deny on ISO File Download PGN requests.

******************************************************************************/

static void xbtmsg_fnRecvIsoReq( XBT_tzMSG *ptzMsg )
{
    const XBT_tzPGN_LIST *ptzPgnList;
    uint32 ulPgn;

    // Requested PGN is in the first 3 bytes of message
    ulPgn = XBT_mMAKE_PGN( ptzMsg->pucData );

    // Look for the requested PGN in supported list
    ptzPgnList = XBTMSG_fnLookupPgn( ulPgn );

    if( ptzPgnList != NULL )
    {
        // If requesting a diag only PGN and not a diag
        // OR has the access deny flag
        if( ( ( ptzPgnList->ucFlags & XBT_DIAG_COMMAND )
           && ( !XBTADDR_fnIsDiag( ptzMsg->ucSrcAddr ) ) )
         || ( ptzPgnList->ucFlags & XBT_ACCESS_DENY ) )
        {
            // Send Access Denied response
            XBTMSG_fnSendAck( ulPgn, XB_eACK_STS_DNY );
            return;
        }
        // If requested PGN is in supported list and has a send function
        else if( ptzPgnList->fnSend )
        {
            // Call the send function using requesting source as destination
            (*ptzPgnList->fnSend)( ptzMsg->ucSrcAddr );
            return;
        }
    }

    // Send a NACK, but only if request was specifically to me (not broadcast)
    if( ptzMsg->ucDestAddr != XBT_ADDR_GLOBAL )
    {
        // Send a NACK
        XBTMSG_fnSendAck( ulPgn,
                          XB_eACK_STS_NAK );
    }
}

/******************************************************************************

FUNCTION NAME:
    XBTMSG_fnSendAck

PURPOSE:
    Construct and send an ISO Acknowledgement message

INPUTS:
    'ulPgn'     - PGN of message being acknowledged
    'ucAckCode' - which type of acknowledgement being made

OUTPUTS:
    None

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version
Version 1.01  Date: 02/28/05  By: Baldeesh S. Khaira
    - Added make byte to byte assignments for TI compiler

******************************************************************************/

void XBTMSG_fnSendAck( uint32 ulPgn,
                       XB_teACK_STS eAckCode )
{
    XBT_tzMSG *ptzMsg;

    // Get empty message
    ptzMsg = XBTMSG_fnNewMsg( XB_PGN_ISO_ACK );

    // Is the message buffer valid?
    if( ptzMsg )
    {
        // Set Acknowledgement code
        ptzMsg->pucData[0] = (uchar8) eAckCode;

        // Set Group Function to don't care
        ptzMsg->pucData[1] = 0xFF;

        // Set 3 reserved bytes
        (void) memset( &ptzMsg->pucData[2], 0xFF, 3 );

        // Set PGN being ACKed into data
        ptzMsg->pucData[5] = XT_mMK_BYTE( (uchar8) ulPgn );
        ptzMsg->pucData[6] = XT_mMK_BYTE( (uchar8) ( ulPgn >> 8 ) );
        ptzMsg->pucData[7] = XT_mMK_BYTE( (uchar8) ( ulPgn >> 16 ) );

        // Set active data length
        ptzMsg->uiDataLen = 8;

        // Send mesage
        XBTMSG_fnSend( ptzMsg, XBT_ADDR_GLOBAL );
    }
}

/******************************************************************************

FUNCTION NAME:
    xbtmsg_fnProcessSingleframe

PURPOSE:
    Send one frame of a single-frame message and check the return code

INPUTS:
    'ptzMsg' - pointer to message being sent

OUTPUTS:
    XBT_ERR_NO_ERROR if message handled as expected
    other result code for unexpected results

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version

******************************************************************************/

static uint16 xbtmsg_fnProcessSingleframe( XBT_tzMSG *ptzMsg )
{
    uint16 uiResult;

    // Send the frame
    uiResult = XBTFRAME_fnSendSingleFrame( ptzMsg );

    // If send succeeded
    switch ( uiResult )
    {
        case XBT_ERR_COMPLETE:
        {
            // Mark message as not busy
            ptzMsg->ucBusy = FALSE;
            // Success
            return( XBT_ERR_NO_ERROR );
        }
        // no break needed

        case XBT_ERR_NOT_COMPLETE:
        {
            // Not an error
            return( XBT_ERR_NO_ERROR );
        }
        // no break needed

        default:
        {
            // Return error
            return( uiResult );
        }
    }
}

/*******************************************************************************

FUNCTION NAME:
    XBTMSG_fnSend

PURPOSE:
    This function is used to send a complete PGN

INPUTS:
    'ptzMsg'     - pointer to message being sent
    'ucDestAddr' - destination address for message

OUTPUTS:
    none

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version

*******************************************************************************/

void XBTMSG_fnSend( XBT_tzMSG *ptzMsg, uchar8 ucDestAddr )
{
    // Only send message if we are online
    if( XBTADDR_fnOnLine() )
    {
        // Set message destination and source address
        ptzMsg->ucSrcAddr = XBTADDR_fnGetAddr();
        ptzMsg->ucDestAddr = ucDestAddr;

        XBTMSG_fnSendAddressed( ptzMsg );
    }
}

/*******************************************************************************

FUNCTION NAME:
    XBTMSG_fnSendAddressed

PURPOSE:
    This function is used to send a complete PGN; it's assumed that  the source
    and destination addresses are already set in the message structure.

INPUTS:
    'ptzMsg' - pointer to message being sent

OUTPUTS:
    none

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version
Version 1.01  Date: 03/28/03  By: dalem
    - Changed separate protocol field to use flags field
Version 1.02  Date: 03/01/03  By: dalem
    - No sending of fastpackets
Version 1.03  Date: 05/14/03  By: dalem
    - Removed unused local variable

*******************************************************************************/

void XBTMSG_fnSendAddressed( XBT_tzMSG *ptzMsg )
{
    const XBT_tzPGN_LIST *ptzPgnList;
    uint16 uiResult;

    // Look for this PGN in supported list
    ptzPgnList = XBTMSG_fnLookupPgn( ptzMsg->ulPgn );

    // If PGN is not in the support list
    if( !ptzPgnList )
    {
        // No support for this PGN
        uiResult = XBT_ERR_NOT_SUPPORTED;
    }
    else
    {
        // Action depends on protocol of PGN
        if( ptzPgnList->ucFlags & XBT_FASTPACKET )
        {
            // Sending fast-packet not supported
            XBTERROR_fnSaveError( XBT_ERR_NO_FASTPACKET );
        }
        else
        {
            // Not fast-packet; protocol depends on message length, but there
            // is no support for sending multi-frame messages at this time.
            // Assume single-frame

            // Mark message as busy
            ptzMsg->ucBusy = TRUE;

            // Send single data frame
            uiResult = xbtmsg_fnProcessSingleframe( ptzMsg );
        }
    }

    // If there was an error
    if( uiResult != XBT_ERR_NO_ERROR )
    {
        // Save the error code
        XBTERROR_fnSaveError( uiResult );
    }
}

/*******************************************************************************

FUNCTION NAME:
    xbtmsg_fnReceive

PURPOSE:
    This function handles reception of a complete message

INPUTS:
    none

OUTPUTS:
    'ptzMsg' - pointer to the received message

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version

*******************************************************************************/

static XBT_tzMSG *xbtmsg_fnReceive( void )
{
    uint16 uiReason;
    XBT_tzMSG *ptzMsg;

    // Try to receive a frame
    ptzMsg = XBTFRAME_fnRecvFrame( &uiReason );

    // If message received
    if( ptzMsg )
    {
        // Is this a connection management message
        if( ptzMsg->ulPgn == XB_PGN_ISO_TP_CONN_MAN )
        {
            // Handle connection management message
            xbtmsg_fnRecvConnMan( ptzMsg );

            // Free the receive buffer
            ptzMsg->ucBusy = FALSE;

            // Treat as if no message received
            return( NULL );
        }
        else
        {
            // Return with message
            return( ptzMsg );
        }
    }
    else
    {
        switch ( uiReason )
        {
            case XBT_ERR_NOT_FOR_ME:
            case XBT_ERR_NO_DATA:
            case XBT_ERR_NOT_COMPLETE:
                break;
            default:
                XBTERROR_fnSaveError( uiReason );
                break;
        }

        // No message
        return( NULL );
    }
}

/*******************************************************************************

FUNCTION NAME:
    XBTMSG_fnHandleMessages

PURPOSE:
    Check to see if there are messages to send or receive

INPUTS:
    none

OUTPUTS:
    none

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version
Version 1.01  Date: 03/28/03  By: dalem
    - Changed separate protocol field to use flags field
    - Use flags field to determine for check for DIAG
    - Use flags field to determine for auto release
Version 1.02  Date: 04/01/03  By: dalem
    - No sending of fastpackets
Version 1.03  Date: 06/05/03  By: dalem
    - Release message if it is NACKed
Version 1.04  Date: 07/22/03  By: dalem
    - Nack message if not of minimum allowable size
    - Nack message if No Broadcast set and message was broadcast
    - Reject most but not all messages if not on line
Version 1.05  Date: 12/24/03  By: Alan Travelbea
    - Diag security check now sends Dny (PRN#965)
    - Security Dny now takes priority over broadcast Naks (PRN#965)

*******************************************************************************/

void XBTMSG_fnHandleMessages( void )
{
    const XBT_tzPGN_LIST *ptzPgnEntry;
    XBT_tzMSG **pptzMsg;
    XBT_tzMSG *ptzMsg;
    uint16 uiReason;

    // For every entry in message transmittable list
    for( pptzMsg = aptzMsgTxList; *pptzMsg != NULL; pptzMsg++ )
    {
        // If message is busy
        if( (*pptzMsg)->ucBusy )
        {
            // If this is a fast packet message
            if( (*pptzMsg)->ucFlags & XBT_FASTPACKET )
            {
               // Sending fastpacket not supported
                XBTERROR_fnSaveError( XBT_ERR_NO_FASTPACKET );
            }
            else
            {
                // Send a singleframe or multiframe message
                uiReason = xbtmsg_fnProcessSingleframe( *pptzMsg );
            }

            // If send failed
            if( uiReason != XBT_ERR_NO_ERROR )
            {
                // Save the error
                XBTERROR_fnSaveError( uiReason );
            }
        }
    }

    // Look to see if a message has been received
    ptzMsg = xbtmsg_fnReceive();

    // If message received
    if( ptzMsg )
    {
        // Look up PGN support
        ptzPgnEntry = XBTMSG_fnLookupPgn( ptzMsg->ulPgn );

        // If entry exists and there is a receive handler
        if( ( ptzPgnEntry ) && ( ptzPgnEntry->fnRecv ) )
        {
            // If we are offline and message is not handled offline
            if( ( !XBTADDR_fnOnLine() )
             && ( !( ptzPgnEntry->ucFlags & XBT_ALLOW_OFFLINE ) ) )
            {
                // Ignore this message
            }
            // If PGN does not meet minimum length requirement
            else if( ptzMsg->uiDataLen < ptzPgnEntry->uiMinLen )
            {
                // Reply with a NACK
                XBTMSG_fnSendAck( ptzPgnEntry->ulPgn,
                                  XB_eACK_STS_NAK );
            }
            // If PGN is a DIAG only command and source node is not a DIAG
            else if( ( ptzPgnEntry->ucFlags & XBT_DIAG_COMMAND )
              && ( !XBTADDR_fnIsDiag( ptzMsg->ucSrcAddr ) ) )
            {
                // Reply with a NACK
                XBTMSG_fnSendAck( ptzPgnEntry->ulPgn,
                                  XB_eACK_STS_DNY );
            }
            // If No Broadcast flag set and message is a broadcast
            else if( ( ptzPgnEntry->ucFlags & XBT_NO_BROADCAST )
              && ( ptzMsg->ucDestAddr == XBT_ADDR_GLOBAL ) )
            {
                // Reply with a NACK
                XBTMSG_fnSendAck( ptzPgnEntry->ulPgn,
                                  XB_eACK_STS_NAK );
            }
            else
            {
                // Call the receive function
                (*ptzPgnEntry->fnRecv)( ptzMsg );

                // If "No Release" flag is not set
                if( !( ptzPgnEntry->ucFlags & XBT_NO_RELEASE ) )
                {
                    // Release the message
                    ptzMsg->ucBusy = FALSE;
                }

                // Message handling complete
                return;
            }
        }

        // Message not handled, free the received message
        ptzMsg->ucBusy = FALSE;
    }
}

/*******************************************************************************

FUNCTION NAME:
    XBTMSG_fnInit

PURPOSE:
    Initialize XanBus Tiny driver message layer and all other layers

INPUTS:
    none

OUTPUTS:
    XBT_ERR_NO_ERROR if all is well
    other error code if error occurred

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version

*******************************************************************************/

uint16 XBTMSG_fnInit( void )
{
    XBT_tzMSG **pptzMsg;
    uint16 uiErr;

    // Initialize the XBT frames management layer
    uiErr = XBTFRAME_fnInit();
    if( uiErr != XBT_ERR_NO_ERROR )
    {
        // Save the error
        XBTERROR_fnSaveError( uiErr );

        // Return error
        return( uiErr );
    }
    else
    {
        // Initialize the XBT message handling layer

        // Set every entry in message transmittable list as "not busy".
        for( pptzMsg = aptzMsgTxList; *pptzMsg != NULL; pptzMsg++ )
        {
            // Message is not busy
            (*pptzMsg)->ucBusy = FALSE;
        }

        // Return success
        return( XBT_ERR_NO_ERROR );
    }
}
