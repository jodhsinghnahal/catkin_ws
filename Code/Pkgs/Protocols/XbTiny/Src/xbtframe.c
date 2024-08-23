/*=============================================================================
  (c) 2005 - 2023 Xantrex Canada ULC. All rights reserved.
  All trademarks are owned or licensed by Xantrex Canada ULC,
  its subsidiaries or affiliated companies.


PURPOSE:
    This file provides CAN frame handling for the XanBus Tiny component.
    As such this file interfaces between the upper level XanBus Tiny Message
    layer and lower level CAN driver

FUNCTION(S):
    XBTFRAME_fnBusyMsgCheck - Timeout check for all busy message buffers
    XBTFRAME_fnStartMultiFrameRx - Setup to receive multiframe message
    XBTFRAME_fnAbortMultiFrameRx - Free multiframe receive buffer
    XBTFRAME_fnRecvFrame    - Handle receiving of a CAN frame
    XBTFRAME_fnSendSingleFrame - Handle sending of a single frame message
    XBTFRAME_fnBusTxState   - Check the CAN bus status
    XBTFRAME_fnInit         - Initilialize this module

    local:
        xbtframe_fnFindFreeMsg  - Choose a free message buffer
        xbtframe_fnFindMsgMatch - Find a message buffer that matches PGN
        xbtframe_fnMakeFrameID  - Build frame ID from PGN
        xbtframe_fnGetMsgID     - Build message identifier from a frame ID
        xbtframe_fnPutMsgID     - Copy message identifier data to a message buffer
        xbtframe_fnCopyData     - Copy bytes from frame to message buffer
        xbtframe_fnReleaseCANFrame - Free a CAN frame buffer
        xbtframe_fnRecvFastFrame - Handle rx of a frame of a fastpacket message
        xbtframe_fnRecvMultiFrame - Handle rx of a frame of a multiframe message
        xbtframe_fnRecvSingleFrame - Handle rx of a frame of singleframe message
        xbtframe_fnReceiveCb    - Callback routine for receiving CAN frames


=============================================================================*/

/*==============================================================================
                              Includes
==============================================================================*/

#include <string.h>
#include "xbudefs.h"
#include "xbtgdefs.h"
#include "xbtldefs.h"
#include "candrv.h"
#include "config.h"
#include "Device.h"
#include "xassert.h"

/*==============================================================================
                           Local/Private Constants
==============================================================================*/

#define XBT_CHANNEL     ( 0 )      // Hardware channel number being used

/*==============================================================================
                           Local/Private Variables
==============================================================================*/

/*
    Temporary buffer for storing CAN frames until a PGN can be assembled.
    Consider that a Multi-Packet protocol message can use up to 255 frames
*/
static CANDRV_tzCAN_FRAME   atzRxFrames[ 480 ];
static uint16               uiRxFrameIn = 0;
static uint16               uiRxFrameOut = 0;
static uint16               uiRxFrameCount = 0;

// Temporary CAN frame storage used for filtering the frames
static CANDRV_tzCAN_FRAME   tzRxFrameBuf;

/*
    Multiframe message buffers
*/

// One bit for each frame in the maximum size multi-packet message
// Each frame contains seven bytes of data for multi-packet
static uchar8 aucMpBamFrameRecvFlags[ XBT_MULTIPKT_NRXFLAGS / 8 ];

static uchar8 aucMultiFrameBamRx[ XBT_MULTIFRAME_MAX ];
static XBT_tzMSG tzRxBam =
{
    aucMultiFrameBamRx,         // Data Pointer
    XBT_MULTIPKT_NRXFLAGS,      // Number of Frame Received Flags
    aucMpBamFrameRecvFlags,     // Frame Received Flags
    XBT_MULTIFRAME,             // Protocol
    0,                          // The other members are set to zero
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};

// One bit for each frame in the maximum size multi-packet message
// Each frame contains seven bytes of data for multi-packet
static uchar8 aucMpRtsFrameRecvFlags[ XBT_MULTIPKT_NRXFLAGS / 8 ];

static uchar8 aucMultiFrameRtsRx[ XBT_MULTIFRAME_MAX ];
static XBT_tzMSG tzRxRts =
{
    aucMultiFrameRtsRx,         // Data Pointer
    XBT_MULTIPKT_NRXFLAGS,      // Number of Frame Received Flags
    aucMpRtsFrameRecvFlags,     // Frame Receive Flags
    XBT_MULTIFRAME,             // Protocol
    0,                          // The other members are set to zero
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};

/*
    Supporting only one Singleframe message buffer
*/
static uchar8 aucSingleFrameRx[ XBT_SINGLEFRAME_MAX ];
static XBT_tzMSG tzRxSingleFrame =
{
    aucSingleFrameRx,           // Data Pointer
    0,                          // Number of Frame Received Flags
    NULL,                       // Frame Received Flags
    XBT_SINGLEFRAME,            // Protocol
    0,                          // The other members are set to zero
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};

/*
    Supporting two Fastpacket message buffers
*/
static uchar8 aucFastPacketRx1[ XBT_FASTPACKET_MAX ];

// One bit for each frame in the maximum size fast-packet message
// Each frame contains seven bytes of data for fast-packet
static uchar8 aucFpFrameRecvFlags1[ XBT_FASTPKT_NRXFLAGS ];

static XBT_tzMSG tzFastPacketRx1 =
{
    aucFastPacketRx1,           // Data Pointer
    XBT_FASTPKT_NRXFLAGS,       // Number of Frame Received Flags
    aucFpFrameRecvFlags1,       // Frame Received Flags
    XBT_FASTPACKET,             // Protocol
    0,                          // The other members are set to zero
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};

static uchar8 aucFastPacketRx2[ XBT_FASTPACKET_MAX ];

// One bit for each frame in the maximum size fast-packet message
// Each frame contains seven bytes of data for fast-packet
static uchar8 aucFpFrameRecvFlags2[ XBT_FASTPKT_NRXFLAGS ];

static XBT_tzMSG tzFastPacketRx2 =
{
    aucFastPacketRx2,           // Data Pointer
    XBT_FASTPKT_NRXFLAGS,       // Number of Frame Received Flags
    aucFpFrameRecvFlags2,       // Frame Receive Flags
    XBT_FASTPACKET,             // Protocol
    0,                          // The other members are set to zero
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};

/*
    List of pointers to receiving message buffers
    Only need buffers for fast-packets; single frames are handled upon
    reception, multi-frames are constrained because of their size.
*/
static const XBT_tzMSG * const aptzMsgRxList[] =
{
    &tzFastPacketRx1,
    &tzFastPacketRx2,
};

/*
    List of pointers to all receiving message buffers
    Used for maintaining their time to live timers
*/
static const XBT_tzMSG * const aptzMsgTtlList[] =
{
    &tzRxBam,
    &tzRxRts,
    &tzRxSingleFrame,
    &tzFastPacketRx1,
    &tzFastPacketRx2,
};

/*==============================================================================
                        Static Function Definitions
==============================================================================*/

// Check if received frame is for this device
static tucBOOL xbtframe_fnFilterRxFrame( CANDRV_tzCAN_ID *pzId );


/*==============================================================================
                           Function Definitions
==============================================================================*/

/*******************************************************************************

FUNCTION NAME:
    xbtframe_fnFindFreeMsg

PURPOSE:
    Find a message buffer that is not marked as busy

INPUTS:
    'ucFlags' - the protocol compatability flags

OUTPUTS:
    A pointer to a not busy message buffer
    NULL if no message buffer available

NOTES:
    This function is used by the XBTiny stack when receiving a frame.

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version
Version 1.01  Date: 03/28/03  By: dalem
    - Changed separate protocol field to use flags field
Version 1.02  Date: 08/01/03  By: dalem
    - Changed for loop to terminate when address beyond end of list

*******************************************************************************/

static XBT_tzMSG *xbtframe_fnFindFreeMsg( uchar8 ucFlags )
{
    XBT_tzMSG **pptzMsg;

    // For every entry in message receive list
    for( pptzMsg = (XBT_tzMSG **)aptzMsgRxList;
         pptzMsg < (XBT_tzMSG **)&aptzMsgRxList[ DIM( aptzMsgRxList ) ];
         pptzMsg++ )
    {
        // If protocol flags match and buffer not busy
        if( ( ( (*pptzMsg)->ucFlags & ucFlags ) == ucFlags )
         && ( !(*pptzMsg)->ucBusy ) )
        {
            // Return the message buffer
            return( *pptzMsg );
        }
    }

    // Failed, no buffer found
    return( NULL );
}

/*******************************************************************************

FUNCTION NAME:
    xbtframe_fnFindMsgMatch

PURPOSE:
    Find a message buffer whose PGN matches the given PGN

INPUTS:
    'ulPgn' - the received frame PGN to be searched for in the message buffer

OUTPUTS:
    A pointer to a message buffer of the same PGN
    NULL if no message buffer available

NOTES:
    This function is used by the XBTiny stack when receiving a frame.

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version
Version 1.01  Date: 08/01/03  By: dalem
    - Changed for loop to terminate when address beyond end of list

*******************************************************************************/

static XBT_tzMSG *xbtframe_fnFindMsgMatch( uint32 ulPgn )
{
    XBT_tzMSG **pptzMsg;

    // For every entry in message receive list
    for( pptzMsg = (XBT_tzMSG **)aptzMsgRxList;
         pptzMsg < (XBT_tzMSG **)&aptzMsgRxList[ DIM( aptzMsgRxList ) ];
         pptzMsg++ )
    {
        // If protocol matches and buffer is busy
        if( ( (*pptzMsg)->ulPgn == ulPgn ) && ( (*pptzMsg)->ucBusy ) )
        {
            // Return the message buffer
            return( *pptzMsg );
        }
    }

    // Failed, no buffer found
    return( NULL );
}

/*******************************************************************************

FUNCTION NAME:
    XBTFRAME_fnBusyMsgCheck

PURPOSE:
    Count down timers for all active receive buffers (marked as busy). Message
    buffers that are expired will be marked as not busy. These buffers can then
    be used for receiving other messages. Any received messages not complete when
    the buffer's busy flag is reset are lost and forgotten.

INPUTS:
    none

OUTPUTS:
    none

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version
Version 1.01  Date: 08/01/03  By: dalem
    - Changed for loop to terminate when address beyond end of list

*******************************************************************************/

void XBTFRAME_fnBusyMsgCheck( void )
{
    XBT_tzMSG **pptzMsg;

    // For every entry in message receive time to live list
    for( pptzMsg = (XBT_tzMSG **)aptzMsgTtlList;
         pptzMsg < (XBT_tzMSG **)&aptzMsgTtlList[ DIM( aptzMsgTtlList ) ];
         pptzMsg++ )
    {
        // If time to live is not zero
        if( (*pptzMsg)->uiTimeToLive )
        {
            // Decrement and test for zero
            if( --(*pptzMsg)->uiTimeToLive == 0 )
            {
                // Mark message as no longer in use
                (*pptzMsg)->ucBusy = FALSE;
            }
        }
    }
}

/*******************************************************************************

FUNCTION NAME:
    xbtframe_fnMakeFrameId

PURPOSE:
    This function builds a CAN Driver Frame ID from entries in the
    XanBus Tiny Driver message structure.
    This function is called when sending a frame to the bus.

INPUTS:
    'ptzMsg' - pointer to the source structure from the XBT stack
    'pzId'   - pointer to the destination CANID structure

OUTPUTS:
    None

NOTES:
    This function is provided for use by the stack when sending a frame.

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version
Version 1.01  Date: 02/14/05  By: Baldeesh K
    - Modified to use new names for the ID frame due to change to CANDRV.h
Version 1.02  Date: 03/31/05  By: Baldeesh K
    - Added packet specific Make Byte macro

*******************************************************************************/

static void xbtframe_fnMakeFrameId( XBT_tzMSG *ptzMsg,
                                    CANDRV_tzCAN_ID *pzId )
{
    uchar8  ucPF;       // Packet format
    uchar8  ucPS;       // Packet Specific
    uchar8  ucDP;       // Data Page

    // Packet Format from PGN
    ucPF = (uchar8) XT_mMK_BYTE( ( ptzMsg->ulPgn >> XT_BITS_PER_BYTE ) );

    // Packet Specific depends if Packet Format below 0xF0
    if( ucPF < 0xF0 )
    {
        // Packet Specific is destination address
        ucPS = ptzMsg->ucDestAddr;
    }
    else
    {
        // Packet Specific is from PGN
        ucPS = (uchar8) XT_mMK_BYTE( ptzMsg->ulPgn );
    }

    // Data Page from PGN
    ucDP = ( ptzMsg->ulPgn & 0x10000 ) ? 1 : 0;

    // Most significant Identifier byte
    pzId->ucId28_24 = ( ptzMsg->ucPriority << 2 ) // 3 bit priority to top of byte
                    | ( ucDP & 0x01 )             // Followed by Data Page bit
                    | CANDRV_ID_MSK_IDE           // Add IDE bit
                    | CANDRV_ID_MSK_SRR;          // and SRR bit

    // Second ID byte
    pzId->ucId23_16 = ucPF;

    // Third ID byte
    pzId->ucId15_8 = ucPS;

    // Fourth ID byte
    pzId->ucId07_00 = ptzMsg->ucSrcAddr;
}

/*******************************************************************************

FUNCTION NAME:
    xbtframe_fnFilterRxFrame

PURPOSE:
    This function checks if received CAN frame is addressed to this device
    and if it is supported by XbTiny.

INPUTS:
    'pzId' - pointer to a received CAN frame ID

OUTPUTS:
    tucBOOL TRUE  - frame should be accepted
            FALSE - frame should be discarded

NOTES:
    This function is used to filter incoming CAN frames to reduce
    CPU processing load and buffering requirements.

VERSION HISTORY:

Version 1.00  Date: Dec.06,2017  By: Doru Botez
    - Creation from xbtframe_fnGetMsgId.

*******************************************************************************/

static tucBOOL xbtframe_fnFilterRxFrame( CANDRV_tzCAN_ID *pzId )
{
    uchar8 ucPF;        // Packet format
    uchar8 ucPS;        // Packet Specific
    uchar8 ucDP;        // Data Page
    uint32 ulPgn;       // PGN ID
    tucBOOL tucRetVal = FALSE;

    // Data Page is ID bit 24
    ucDP = XT_mMK_BYTE( pzId->ucId28_24 & 0x01 );

    // Packet Format is ID bits 23-16
    ucPF = XT_mMK_BYTE( pzId->ucId23_16 );

    // Packet Specific is ID bits 15-8
    ucPS = XT_mMK_BYTE( pzId->ucId15_8 );

    // PGN is built from Data Page and Packet Format
    ulPgn = ( (uint32) ucDP << 16 ) | ( (uint32) ucPF << 8 );

    // Destination address is Packet Specific if Packet Format below 0xF0
    if( ucPF < 0xF0 )
    {
        // Packet Specific field is the destination address
        // Check if destination is this device
        if( ( ucPS == XBTADDR_fnGetAddr() ) ||
            // or global
            ( ucPS == XBT_ADDR_GLOBAL ) ||
            // or is zero and the PGN is ISO_ADDR_CLAIM
            ( ( ucPS == 0 ) && ( ulPgn == XB_PGN_ISO_ADDR_CLAIM ) )
          )
        {
            // Assume we can accept the frame
            tucRetVal = TRUE;
        }
    }
    else
    {
        // PGN includes Packet Specific
        ulPgn |= ucPS;
        // Assume we can accept the frame
        tucRetVal = TRUE;
    }

    // Were the previous checks OK?
    if( tucRetVal == TRUE )
    {
        // Check if this PGN is supported by the XbTiny
        if( XBTMSG_fnLookupPgn( ulPgn ) == NULL )
        {
            // Frame is not supported, reject it.
            tucRetVal = FALSE;
        }
    }

    return tucRetVal;
}

/*******************************************************************************

FUNCTION NAME:
    xbtframe_fnGetMsgId

PURPOSE:
    This function converts the Id of a received CAN frame into a XanBus
    message ID.

INPUTS:
    'pzId' is pointer to a received CAN frame ID
    'ptzMsgId' is pointer to return XanBus message ID

OUTPUTS:
    None

NOTES:
    This function is used by the XBTiny stack when receiving a frame.

VERSION HISTORY:

Version 1.00  Date: 03/26/03  By: dalem
    - Creation from xbtf_fnGetFramePGN and xbtf_fnGetFrameSrc
Version 1.01  Date: 02/14/05  By: Baldeesh K
    - Modified to use new names for the ID frame due to change to CANDRV.h

*******************************************************************************/

static void xbtframe_fnGetMsgId( CANDRV_tzCAN_ID *pzId,
                                 XBT_tzMSG_ID *ptzMsgId )
{
    uchar8 ucPF;       // Packet format
    uchar8 ucPS;       // Packet Specific
    uchar8 ucDP;       // Data Page

    // Data Page is ID bit 24
    ucDP = XT_mMK_BYTE( pzId->ucId28_24 & 0x01 );

    // Packet Format is ID bits 23-16
    ucPF = XT_mMK_BYTE( pzId->ucId23_16 );

    // Packet Specific is ID bits 15-8
    ucPS = XT_mMK_BYTE( pzId->ucId15_8 );

    // PGN is built from Data Page and Packet Format
    ptzMsgId->ulPgn = ( (uint32) ucDP << 16 ) | ( (uint32) ucPF << 8 );

    // Priority is ID bits 28-26
    ptzMsgId->ucPriority = XT_mMK_BYTE( pzId->ucId28_24 >> 3 );

    // Source Address is ID bits 7-0
    ptzMsgId->ucSrcAddr = XT_mMK_BYTE( pzId->ucId07_00 );

    // Destination address is Packet Specific if Packet Format below 0xF0
    if( ucPF < 0xF0 )
    {
        // Packet specific is destination address
        ptzMsgId->ucDestAddr = ucPS;
    }
    else
    {
        // Destination is assumed to be global address
        ptzMsgId->ucDestAddr = XBT_ADDR_GLOBAL;

        // PGN includes Packet Specific
        ptzMsgId->ulPgn |= ucPS;
    }
}

/*******************************************************************************

FUNCTION NAME:
    xbtframe_fnPutMsgId

PURPOSE:
    This function sets the PGN, priority, source address and destintion
    address into an XBT_MSG structure.

INPUTS:
    'ptzMsg'   - pointer to message as destination of PGN, etc.
    'ptzMsgId' - pointer to source of PGN, etc.

OUTPUTS:
    none

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version

*******************************************************************************/

static void xbtframe_fnPutMsgId( XBT_tzMSG *ptzMsg,
                                 XBT_tzMSG_ID *ptzMsgId )
{
    // Set PGN, destination address, source address and priority
    ptzMsg->ulPgn = ptzMsgId->ulPgn;
    ptzMsg->ucDestAddr = ptzMsgId->ucDestAddr;
    ptzMsg->ucSrcAddr = ptzMsgId->ucSrcAddr;
    ptzMsg->ucPriority = ptzMsgId->ucPriority;
}

/*******************************************************************************

FUNCTION NAME:
    xbtframe_fnClrRecvFlags

PURPOSE:
    This function resets the received frame flags list/array.

INPUTS:
    'pucFlags' - pointer to the flags buffer.
    'ucNFlags' - the number of flags represented by the buffer

OUTPUTS:
    None

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/23/04  By: peterd
    - First version

*******************************************************************************/

void xbtframe_fnClrRecvFlags( uchar8 *pucFlags, uchar8 ucNFlags )
{
    ( void )memset( pucFlags, 0, ucNFlags / 8 );
}

/*******************************************************************************

FUNCTION NAME:
    xbtframe_fnTestAndSetFlag

PURPOSE:
    This function tests and sets a received frame flag.

INPUTS:
    'pucFlags' - pointer to the flags buffer.
    'ucNFlags' - the number of flags represented by the buffer
    'ucFlagId' - a zero-index flag identifier

OUTPUTS:
    FALSE if the flag specified by ucFlagId was not set on entry
    TRUE if the flag specified by ucFlagId was already set on entry

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/23/04  By: peterd
    - First version

*******************************************************************************/

tucBOOL xbtframe_fnTestAndSetFlag( uchar8 *pucFlags,
                                   uchar8 ucNFlags,
                                   uchar8 ucFlagId )
{
    uchar8 ucArrayIndex;
    uchar8 ucBitOffset;
    uchar8 ucBitMask = 1;

    assert( pucFlags != NULL );

    assert( ucFlagId < ucNFlags );

    // Each array element has 8 bits ( shift by 3 and mask the least
    // significant 3 bits instead of using divide and modulo operations )
    ucArrayIndex = ucFlagId >> 3;   // divide by 8

    ucBitOffset = ucFlagId & 0x07;  // modulo 8

    // Shift to select the right bit within the element
    while ( ucBitOffset > 0 )
    {
        ucBitMask <<= 1;

        ucBitOffset--;
    }

    // If the bit is not set, set it
    // Return indication of old setting
    if( ( pucFlags[ ucArrayIndex ] & ucBitMask ) == 0 )
    {
        pucFlags[ ucArrayIndex ] |= ucBitMask;

        return FALSE;
    }
    else    // Already set
    {
        return TRUE;
    }
}

/*******************************************************************************

FUNCTION NAME:
    xbtframe_fnCopyData

PURPOSE:
    This function copies the received CAN frame to a message buffer.
    It is used to limit the number of bytes copied to avoid buffer overrun.

INPUTS:
    'pucFrameData' - pointer to a received CAN frame data
    'ptzMsg'       - pointer to message to hold extracted data
    'uiMsgOffset'  - byte offset of the insertion point in message data buffer
    'uiNewBytes'   - number of bytes to copy

OUTPUTS:
    Number of bytes actually copied

NOTES:
    This function is used by the XBTiny stack when receiving a frame.

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version
Version 1.01  Date: 03/23/04  By: peterd
    - Removed check for completion
    - Changed return value to number of bytes copied

*******************************************************************************/

static uint16 xbtframe_fnCopyData( uchar8 *pucFrameData,
                                   XBT_tzMSG *ptzMsg,
                                   uint16 uiMsgOffset,
                                   uint16 uiNewBytes )
{
    // If number of bytes needed to finish message less than
    // number of bytes in a full frame
    if( ( ptzMsg->uiDataLen - uiMsgOffset ) < uiNewBytes )
    {
        // Limit number of new bytes
        uiNewBytes = ptzMsg->uiDataLen - uiMsgOffset;
    }

    // Copy data bytes
    (void) memcpy( &ptzMsg->pucData[ uiMsgOffset ], pucFrameData, uiNewBytes );

    return uiNewBytes;
}

/*******************************************************************************

FUNCTION NAME:
    XBTFRAME_fnStartMultiFrameRx

PURPOSE:
    Setup a multiframe message buffer for receiving

INPUTS:
    'ulPgn'   - PGN of message to be received
    'ptzConn' - pointer to Connection message used to start the reception
                of a multi-frame packet.

OUTPUTS:
    pointer to message buffer for receiving multiframe message
    NULL if no message buffer available

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version
Version 1.01  Date: 04/01/03  By: dalem
    - Compare message length to buffer size
Version 1.02  Date: 04/25/03  By: dalem
    - Error in getting data length from message
Version 1.03  Date: 03/23/04  By: peterd
    - Added call to clear received frame flags
    - Added reset of frame counter

*******************************************************************************/

XBT_tzMSG *XBTFRAME_fnStartMultiFrameRx( uint32 ulPgn,
                                         XBT_tzMSG *ptzConn )
{
    uchar8 *pucData;
    uchar8 ucDest;
    XBT_tzMSG *ptzMsg;

    // Get local data pointer
    pucData = ptzConn->pucData;

    // Message buffer depends on group function
    if( pucData[ 0 ] == XBT_CONN_RTS )
    {
        // Directed messages use RTS
        ptzMsg = &tzRxRts;

        // Set destination address according to destination in RTS
        ucDest = ptzConn->ucDestAddr;
    }
    else
    {
        // Broadcast messages use BAM
        ptzMsg = &tzRxBam;

        // Set destination to global addresss
        ucDest = XBT_ADDR_GLOBAL;
    }

    // If buffer is already in use
    if( ptzMsg->ucBusy )
    {
        // Return failure
        return( NULL );
    }
    else
    {
        // Set PGN being transported
        ptzMsg->ulPgn = ulPgn;

        // Total message size from connection message
        ptzMsg->uiDataLen = pucData[1] + pucData[2] * 256;

        // Cannot receive frame if it will not fit
        if( ptzMsg->uiDataLen > XBT_MULTIFRAME_MAX )
        {
            // Make sure buffer is not marked for use so BAM will not use it
            ptzMsg->ucBusy = FALSE;

            // Return failure
            return( NULL );
        }

        // Total number of frames from connection message
        ptzMsg->ucEndFrame = pucData[3];

        // No frames received yet
        ptzMsg->ucFrameNum = 1;

        // Set source and destination according to connection message
        ptzMsg->ucSrcAddr = ptzConn->ucSrcAddr;
        ptzMsg->ucDestAddr = ucDest;

        // Set busy flag and time to live
        ptzMsg->ucBusy = TRUE;
        ptzMsg->uiTimeToLive = XBT_TTL_NORMAL;

        // Clear multi-packet frame received flags
        xbtframe_fnClrRecvFlags( ptzMsg->pucRxFrameFlags, ptzMsg->ucNRxFrameFlags );

        // Reset frame counter
        ptzMsg->uiFrameCount = 0;

        // Success
        return( ptzMsg );
    }
}

/*******************************************************************************

FUNCTION NAME:
    XBTFRAME_fnAbortMultiFrameRx

PURPOSE:
    Abort reception of a multi-frame message.

INPUTS:
    'ulPgn' - PGN of multi-frame message to abort

OUTPUTS:
    TRUE if PGN matches a multiframe message that is being received
    FALSE if no PGN receiving to abort

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/25/03  By: dalem
    - Created

*******************************************************************************/

uchar8 XBTFRAME_fnAbortMultiFrameRx( uint32 ulPgn )
{
    // If PGN matches received RTS message and it is in use
    if( ( ulPgn == tzRxRts.ulPgn ) && ( tzRxRts.ucBusy ) )
    {
        // Free the message
        tzRxRts.ucBusy = FALSE;

        // Return TRUE to indicate mesasge was found
        return( TRUE );
    }
    // If PGN matches received BAM message and it is in use
    else if( ( ulPgn == tzRxBam.ulPgn ) && ( tzRxBam.ucBusy ) )
    {
        // Free the message
        tzRxBam.ucBusy = FALSE;

        // Return TRUE to indicate mesasge was found
        return( TRUE );
    }
    else
    {
        // Return FALSE to indicate mesasge was not found
        return( FALSE );
    }
}

/*******************************************************************************

FUNCTION NAME:
    xbtframe_fnReleaseCANFrame

PURPOSE:
    This function is used to free the received CAN frame.

INPUTS:
    none

OUTPUTS:
    none

NOTES:
    This function is used by the XBTiny stack when receiving a frame.

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version

*******************************************************************************/

static void xbtframe_fnReleaseCanFrame( void )
{
    // Decrease frame count with interrupt protection
    DEVICE_mGLOBAL_INT_DIS();
    uiRxFrameCount--;
    DEVICE_mGLOBAL_INT_EN();
    uiRxFrameOut++;

    // Wrap insert point if necessary
    if( uiRxFrameOut >= DIM( atzRxFrames ) )
    {
        uiRxFrameOut = 0;
    }
}

/*******************************************************************************

FUNCTION NAME:
    xbtframe_fnRecvFastFrame

PURPOSE:
    Handle reception of one frame of a fast-packet protocol message

INPUTS:
    'ptzFrame'  - pointer to the received CAN frame
    'ptzMsgID'  - pointer to the message identifier of the received CAN frame;
                  provides message PGN, priority, destination and source addresss.
    'puiReason' - pointer to return how frame was handled

OUTPUTS:
    pointer to message if complete message receive
    NULL if problems detected in handling the frame;
    The "Reason" is updated according to PGN reception status.

NOTES:

VERSION HISTORY:

Version 1.01  Date: 03/26/03  By: dalem
    - Extrtacted from xbtf_fnRecvFrame
Version 1.02  Date: 03/23/04  By: peterd
    - Added call to clear received frame flags
    - Added reset of frame count
    - Added call to test and set received frame flag
    - Added message completion test

*******************************************************************************/

static XBT_tzMSG *xbtframe_fnRecvFastFrame( CANDRV_tzCAN_FRAME *ptzFrame,
                                            XBT_tzMSG_ID *ptzMsgId,
                                            uint16 *puiReason )
{
    XBT_tzMSG *ptzMsg;
    uchar8 ucFrameNum;
    uint16 uiMsgOffset;
    uint16 uiNewBytes;
    uchar8 *pucFrameData;
    uint16 uiExpectFrames;

    // Get local copy of frame data pointer
    pucFrameData = ptzFrame->aucData;

    // Frame number is part of first data byte
    ucFrameNum = *pucFrameData++ & 0x1F;

    // Is this a start of a fast packet
    if( ucFrameNum == 0 )
    {
        // Find a free fast packet message
        ptzMsg = xbtframe_fnFindFreeMsg( XBT_FASTPACKET );

        // If no message available
        if( !ptzMsg )
        {
            // No message free
            *puiReason = XBT_ERR_NO_BUFFER;

            // Failed, exit
            return( NULL );
        }

        xbtframe_fnClrRecvFlags( ptzMsg->pucRxFrameFlags,
                                 ptzMsg->ucNRxFrameFlags );

        // Reset frame counter
        ptzMsg->uiFrameCount = 0;

        // Mark message buffer as busy
        ptzMsg->ucBusy = TRUE;

        // Set PGN, destination address, source address and priority
        xbtframe_fnPutMsgId( ptzMsg, ptzMsgId );

        // Total message length is second byte of frame
        ptzMsg->uiDataLen = *pucFrameData++;

        // Set maximum number of new bytes
        uiNewBytes = ( CANDRV_MAX_BYTES - 2 );

        // Data goes into message at start
        uiMsgOffset = 0;
    }
    else
    {
        // Look for message with matching PGN
        ptzMsg = xbtframe_fnFindMsgMatch( ptzMsgId->ulPgn );

        // If no message available
        if( !ptzMsg )
        {
            // No message found
            *puiReason = XBT_ERR_NOT_FOUND;

            // Failed, exit
            return( NULL );
        }

        // Determine data insertion point; first frame is shorter by one byte.
        uiMsgOffset = ( ucFrameNum * ( CANDRV_MAX_BYTES - 1 ) ) - 1;

        // Set maximum number of new bytes
        uiNewBytes = ( CANDRV_MAX_BYTES - 1 );
    }

    // Copy the data bytes
    uiNewBytes = xbtframe_fnCopyData( pucFrameData,
                                      ptzMsg,
                                      uiMsgOffset,
                                      uiNewBytes );

    // If we have not already received this frame
    // Fast-Packet frames are zero-indexed
    if( xbtframe_fnTestAndSetFlag( ptzMsg->pucRxFrameFlags,
                                    ptzMsg->ucNRxFrameFlags,
                                    ucFrameNum ) == FALSE )
    {
        // Count it
        ptzMsg->uiFrameCount++;
    }

    // For Fast-Packets first frame is one byte shorter than the other frames
    if( ptzMsg->uiDataLen > ( XBT_FASTPKT_FRMDATASZ - 1 ) )
    {
        // First frame is 6 bytes and subsequent frames are 7 bytes
        uiExpectFrames = ( ptzMsg->uiDataLen - ( XBT_FASTPKT_FRMDATASZ - 1 )
                         + XBT_FASTPKT_FRMDATASZ - 1 )
                         / XBT_FASTPKT_FRMDATASZ + 1;
    }
    else
    {
        // Only one frame
        uiExpectFrames = 1;
    }

    // Check if we received all expected frames
    if( ptzMsg->uiFrameCount >= uiExpectFrames )
    {
        *puiReason = XBT_ERR_COMPLETE;
    }
    else
    {
        *puiReason = XBT_ERR_NOT_COMPLETE;
    }

    // Return message pointer
    return( ptzMsg );
}

/*******************************************************************************

FUNCTION NAME:
    xbtframe_fnRecvMultiFrame

PURPOSE:
    Handle receiving of one frame of a multiframe protocol message

INPUTS:
    'ptzFrame'  - pointer to the received CAN frame
    'ptzMsgID'  - pointer to the message identifier of the received CAN frame;
                  provides message PGN, priority, destination and source addresss.
    'puiReason' - pointer to return how frame was handled

OUTPUTS:
    pointer to message if complete message receive
    NULL if problems detected in handling the frame;
    The "Reason" is updated according to PGN reception status.

NOTES:

VERSION HISTORY:

Version 1.01  Date: 03/26/03  By: dalem
    - Extrtacted from xbtf_fnRecvFrame
Version 1.02  Date: 04/25/03  By: dalem
    - Whole first byte is frame number
Version 1.03  Date: 03/23/04  By: peterd
    - Added test and set of received frame flag
    - Added message completion test

*******************************************************************************/

static XBT_tzMSG *xbtframe_fnRecvMultiFrame( CANDRV_tzCAN_FRAME *ptzFrame,
                                             XBT_tzMSG_ID *ptzMsgId,
                                             uint16 *puiReason )
{
    XBT_tzMSG *ptzMsg;
    uchar8 ucFrameNum;
    uint16 uiMsgOffset;
    uint16 uiNewBytes;
    uchar8 *pucFrameData;

    // Message buffer depends on destination address
    if( ptzMsgId->ucDestAddr == XBT_ADDR_GLOBAL )
    {
        // Broadcast messages use BAM
        ptzMsg = &tzRxBam;
    }
    else
    {
        // Directed messages use RTS
        ptzMsg = &tzRxRts;
    }

    // If buffer is not marked as busy
    if( !ptzMsg->ucBusy )
    {
        // Message not started yet, cannot use buffer
        *puiReason = XBT_ERR_NO_BUFFER;

        // Clear the message pointer and exit, message is not complete
        return( NULL );
    }
    else
    {
        // Get local copy of frame data pointer
        pucFrameData = ptzFrame->aucData;

        // Frame number is part of first data byte
        ucFrameNum = *pucFrameData++;

        // Determine data insertion point
        uiMsgOffset = ( ucFrameNum - 1 ) * ( CANDRV_MAX_BYTES - 1 );

        // Set maximum number of new bytes
        uiNewBytes = ( CANDRV_MAX_BYTES - 1 );

        // Copy the frame data into the message buffer
        uiNewBytes = xbtframe_fnCopyData( pucFrameData,
                                          ptzMsg,
                                          uiMsgOffset,
                                          uiNewBytes );

        // If we have not already received this frame
        // Multi-Packet frames are one-indexed
        if( xbtframe_fnTestAndSetFlag( ptzMsg->pucRxFrameFlags,
                                        ptzMsg->ucNRxFrameFlags,
                                        ucFrameNum - 1 ) == FALSE )
        {
            // Count it
            ptzMsg->uiFrameCount++;
        }

        // If we have received enough frames
        if( ptzMsg->uiFrameCount >= ( ptzMsg->uiDataLen +
                                       XBT_MULTIPKT_FRMDATASZ - 1 ) /
                                       XBT_MULTIPKT_FRMDATASZ )
        {
            *puiReason = XBT_ERR_COMPLETE;

            // If this is not a BAM
            if( ptzMsg->ucDestAddr != XBT_ADDR_GLOBAL )
            {
                // Need to send an EOM for RTS data transfer
                XBTMSG_fnSendConnMan( XBT_CONN_EOM, ptzMsg );
            }
        }
        else
        {
            *puiReason = XBT_ERR_NOT_COMPLETE;
        }
    }

    // Return message pointer
    return( ptzMsg );
}

/*******************************************************************************

FUNCTION NAME:
    xbtframe_fnRecvSingleFrame

PURPOSE:
    Handle receiving of one frame of a single-frame protocol message

INPUTS:
    'ptzFrame' - pointer to the received CAN frame
    'ptzMsgID' - pointer to the message identifier of the received CAN frame;
                 provides message PGN, priority, destination and source addresss.
    'puiReason' is pointer to return how frame was handled

OUTPUTS:
    pointer to message if complete message receive
    NULL if problems detected in handling the frame;
    The "Reason" is updated according to PGN reception status.

NOTES:

VERSION HISTORY:

Version 1.01  Date: 03/26/03  By: dalem
    - Extrtacted from xbtf_fnRecvFrame

*******************************************************************************/

static XBT_tzMSG *xbtframe_fnRecvSingleFrame( CANDRV_tzCAN_FRAME *ptzFrame,
                                              XBT_tzMSG_ID *ptzMsgId,
                                              uint16 *puiReason )
{
    XBT_tzMSG *ptzMsg;

    // Find a free single frame message
    ptzMsg = &tzRxSingleFrame;

    // If no free message
    if( ptzMsg->ucBusy )
    {
        // No free buffer
        *puiReason = XBT_ERR_NO_BUFFER;

        // Exit
        return( NULL );
    }

    // Mark frame as busy
    ptzMsg->ucBusy = TRUE;

    // Set PGN, destination address, source address and priority
    xbtframe_fnPutMsgId( ptzMsg,
                         ptzMsgId );

    // Set data length into message
    ptzMsg->uiDataLen = ptzFrame->ucByteCount;

    // Copy bytes into message
    (void) memcpy( ptzMsg->pucData,
                   ptzFrame->aucData,
                   CANDRV_MAX_BYTES );

    // Message received
    *puiReason = XBT_ERR_COMPLETE;

    // Return message pointer
    return( ptzMsg );
}

/*******************************************************************************

FUNCTION NAME:
    XBTFRAME_fnRecvFrame

PURPOSE:
    This function is used to attempt to receive a complete PGN

INPUTS:
    'puiReason' is pointer to reason this is, or isn't a frame

OUTPUTS:
    pointer to message if complete message receive
    NULL if problems detected in handling the frame;
    The "Reason" is updated according to PGN reception status.

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version
Version 1.01  Date: 03/28/03  By: dalem
    - Restructured
    - Three daughter function split off for single, fast and multi frames
    - Changed separate protocol field to use flags field
Version 1.02  Date: 05/30/03  By: dalem
    - Added check for online and address claim PGN
Version 1.03  Date: 07/23/03  By: dalem
    - Check for handling message while not online was moved up the stack

*******************************************************************************/

XBT_tzMSG *XBTFRAME_fnRecvFrame( uint16 *puiReason )
{
    CANDRV_tzCAN_FRAME *ptzFrame;
    XBT_tzMSG_ID tzMsgId;
    const XBT_tzPGN_LIST *ptzPgnList;
    XBT_tzMSG *ptzMsg;

    // If no data available from receive interrupt
    if( uiRxFrameCount == 0 )
    {
        // No received frame data
        *puiReason = XBT_ERR_NO_DATA;
        return( NULL );
    }

    // Get local pointer to next received frame
    ptzFrame = &atzRxFrames[ uiRxFrameOut ];

    // Get PGN and destination address from frame
    xbtframe_fnGetMsgId( &ptzFrame->tzCanId, &tzMsgId );

    // Modify the destination address for the EE00h (ISO_ADDR_CLAIM) PGN
    // so it will not be rejected below
    if( ( tzMsgId.ucDestAddr == 0 ) && ( tzMsgId.ulPgn == XB_PGN_ISO_ADDR_CLAIM ) )
    {
        tzMsgId.ucDestAddr = XBT_ADDR_GLOBAL;
    }

    // If destination is not global and not for us in particular
    if( ( tzMsgId.ucDestAddr != XBT_ADDR_GLOBAL )
     && ( tzMsgId.ucDestAddr != XBTADDR_fnGetAddr() ) )
    {
        // Do not handle this message
        *puiReason = XBT_ERR_NOT_FOR_ME;
        ptzMsg = NULL;
    }
    else
    {
        // Look for this PGN in supported list
        ptzPgnList = XBTMSG_fnLookupPgn( tzMsgId.ulPgn );

        // If PGN is not in supported list
        if( !ptzPgnList )
        {
            // This PGN not supported
            *puiReason = XBT_ERR_NOT_FOR_ME;
            ptzMsg = NULL;
        }
        else
        {
            // If PGN is a fastpacket
            if( ptzPgnList->ucFlags & XBT_FASTPACKET )
            {
                // Receive a fastpacket frame
                ptzMsg = xbtframe_fnRecvFastFrame( ptzFrame,
                                                   &tzMsgId,
                                                   puiReason );
            }
            else
            {
                // If PGN is for data transfer
                if( tzMsgId.ulPgn == XB_PGN_ISO_TP_XFER )
                {
                    // This is part of a multiframe transfer
                    // Process a multiframe frame
                    ptzMsg = xbtframe_fnRecvMultiFrame( ptzFrame,
                                                        &tzMsgId,
                                                        puiReason );
                }
                else
                {
                    // Receive a single frame
                    ptzMsg = xbtframe_fnRecvSingleFrame( ptzFrame,
                                                         &tzMsgId,
                                                         puiReason );
                }
            }
        }
    }

    // Finished with this CAN frame, release it
    xbtframe_fnReleaseCanFrame();

    // If message is complete
    if( *puiReason == XBT_ERR_COMPLETE )
    {
        // If there is a message pointer
        if( ptzMsg )
        {
            // Set/Reset time to live for received message
            ptzMsg->uiTimeToLive = XBT_TTL_NORMAL;
        }

        // Return message pointer
        return( ptzMsg );
    }
    else
    {
        // Message reception is not yet complete
        return( NULL );
    }
}

/*******************************************************************************

FUNCTION NAME:
    XBTFRAME_fnSendSingleFrame

PURPOSE:
    This function converts a single-frame message into the CAN frame format
    used by the CAN driver and then sends it via the CAN driver.

INPUTS:
    'ptzMsg' - pointer to single-frame message to send

OUTPUTS:
    XBT_ERR_COMPLETE if frame was sent
    Error code from driver if send not successful

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version
Version 1.01  Date: 05/30/03  By: dalem
    - Wait until hardware buffer if available

*******************************************************************************/

uint16 XBTFRAME_fnSendSingleFrame( XBT_tzMSG *ptzMsg )
{
    CANDRV_tzCAN_FRAME tzFrame;
    uint16 uiResult;
    uchar8 ucFrameHandle;

    // Form a Frame ID from XBT message
    xbtframe_fnMakeFrameId( ptzMsg,
                            &tzFrame.tzCanId );

    // Set the Data length
    tzFrame.ucByteCount = (uchar8) ptzMsg->uiDataLen;

    // Copy the data bytes from start if message buffer
    (void)memcpy( tzFrame.aucData, ptzMsg->pucData, CANDRV_MAX_BYTES );

    // Wait if there is no hardware transmit buffer available
    while( CANDRV_fnTransmitOk( XBT_CHANNEL ) != CANDRV_eRET_SUCCESS )
    {
        ;   // Try again
    }

    // Send the frame
    uiResult = CANDRV_fnTransmit( XBT_CHANNEL,
                                  &tzFrame,
                                  &ucFrameHandle );

    // Return result (convert CAN success to XBT complete)
    return( ( uiResult == CANDRV_eRET_SUCCESS ) ? XBT_ERR_COMPLETE : uiResult );
}

/*******************************************************************************

FUNCTION NAME:
    xbtfrane_fnReceiveCb

PURPOSE:
    Store the received frame into a circular buffer in response to a CAN
    receive interrupt.

INPUTS:
    The CAN channel (port) that generated the interrupt.

OUTPUTS:
    None

NOTES:
    As this function is executed during an interrupt, it assumes that
    interrupts are already disabled while placing frames into the
    circular buffer.

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version

Version 1.01  Date: 12/08/17  By: DoruB
    - Discard frames that are not addressed to this device or are not supported
      by XbTiny.

*******************************************************************************/

static void xbtframe_fnReceiveCb( uchar8 Channel )
{
    CANDRV_teRETURN teResult;

    // If there is room for the new frame
    if( uiRxFrameCount < DIM( atzRxFrames ) )
    {
        // Clear the Rx CAN frame buffer
        memset( (void*)&tzRxFrameBuf, 0, sizeof(tzRxFrameBuf) );

        // Receive frame into the Rx frame buffer
        teResult = CANDRV_fnReceive( Channel, &tzRxFrameBuf );

        // If frame received
        if( teResult == CANDRV_eRET_SUCCESS )
        {
            // Should it be stored?
            if( xbtframe_fnFilterRxFrame( &tzRxFrameBuf.tzCanId ) == TRUE )
            {
                // Copy the frame into the Rx frame array
                memcpy( (void *)&atzRxFrames[ uiRxFrameIn ],
                        (void *)&tzRxFrameBuf, sizeof(CANDRV_tzCAN_FRAME) );
                // Frame received increase count
                uiRxFrameCount++;
                uiRxFrameIn++;

                // Wrap insert point if necessary
                if( uiRxFrameIn >= DIM( atzRxFrames ) )
                {
                    uiRxFrameIn = 0;
                }
            }
        }
    }
}

/*******************************************************************************

FUNCTION NAME:
    XBTFRAME_fnBusTxState

PURPOSE:
    Get Bus Status from CAN driver and convert it to XanBus codes

INPUTS:
    none

OUTPUTS:
    One of XBT_BUS_ERROR_ACTIVE, XBT_BUS_ERROR_PASSIVE, XBT_BUS_BUS_OFF

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version

*******************************************************************************/

uchar8 XBTFRAME_fnBusTxState( void )
{
    // Get transmitter status
    switch( CANDRV_fnTxStatus( XBT_CHANNEL, NULL ) )
    {
        case CANDRV_eSTATUS_OK:
            return( XBT_BUS_ERROR_ACTIVE );

        case CANDRV_eSTATUS_WARNING:
        case CANDRV_eSTATUS_ERROR:
            return( XBT_BUS_ERROR_PASSIVE );

        case CANDRV_eSTATUS_BUSOFF:
        default:
            return( XBT_BUS_BUS_OFF );
    }
}

/*******************************************************************************

FUNCTION NAME:
    XBTFRAME_fnInit

PURPOSE:
    Initialize the XanBus Tiny frame driver module

INPUTS:
    none

OUTPUTS:
    XBT_ERR_NO_ERROR if all goes well
    Error code if initialization fails

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version

*******************************************************************************/

uint16 XBTFRAME_fnInit( void )
{
    CANDRV_teRETURN teResult;

    // Install callback functions before any interrupts get enabled
    CANDRV_fnInitCbList();
    teResult = CANDRV_fnInstallCb( XBT_CHANNEL,
                                   CANDRV_eCB_RECEIVE,
                                   xbtframe_fnReceiveCb );
    if( teResult != CANDRV_eRET_SUCCESS )
    {
        // Return failure code
        return( teResult );
    }

    // Initialize CAN
    teResult = CANDRV_fnInit( XBT_CHANNEL,
                              CANDRV_eBIT_RATE_250KBPS,
                              CANDRV_eFILTER_PASS_ALL );
    if( teResult != CANDRV_eRET_SUCCESS )
    {
        // Return failure code
        return( teResult );
    }

    // Enable the CAN port
    teResult = CANDRV_fnEnable( XBT_CHANNEL );
    if( teResult != CANDRV_eRET_SUCCESS )
    {
        // Return failure code
        return( teResult );
    }

    // Success
    return( XBT_ERR_NO_ERROR );
}
