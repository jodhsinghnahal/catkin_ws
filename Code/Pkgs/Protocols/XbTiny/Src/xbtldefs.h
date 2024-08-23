/*=============================================================================
Copyright © 2003 Xantrex Technology, Inc. All rights reserved.

This source file and the information contained in it are confidential and 
proprietary to Xantrex Technology, Inc. The reproduction or disclosure, in 
whole or in part, to anyone outside of Xantrex without the written approval of 
a Xantrex officer under a Non-Disclosure Agreement, or to any employee of 
Xantrex who has not previously obtained written authorization for access from 
the individual responsible for the source code, will have a significant 
detrimental effect on Xantrex and is expressly prohibited.

FILE NAME:  xbtldefs.h

PURPOSE:
    This file provides internal interface data for the XanBus Tiny driver

FUNCTION(S):
    XBTADDR_fnGetAddr       - Get current address
    XBTADDR_fnSendClaim     - Send an address claim message
    XBTADDR_fnRecvClaim     - Handler for the ISO Address Claim message
    XBTFRAME_fnBusyMsgCheck - Timeout check for all busy message buffers
    XBTFRAME_fnStartMultiFrameRx - Setup to receive multiframe message
    XBTFRAME_fnAbortMultiFrameRx - Free multiframe receive buffer
    XBTFRAME_fnRecvFrame    - Handle receiving of a CAN frame
    XBTFRAME_fnSendSingleFrame - Handle sending of a single frame message
    XBTFRAME_fnBusTxState   - Check the CAN bus status
    XBTFRAME_fnInit         - Initilialize this module
    XBTLOAD_fnRecvLoadInfo  - Handle File Download Info PGN
    XBTLOAD_fnRecvLoadBlock - Handle File Download Block PGN
    XBTLOAD_fnRecvResetCmd  - Handle Reset Command PGN
    XBTLOAD_fnReqLoadStatus - Handle request to send a Download Status PGN
    XBTMSG_fnSendConnMan    - Send an ISO Connenction Management message
    XBTMSG_fnLookupPgn      - Lookup PGN in PGN support list
    XBTMSG_fnNewMsg         - Get a free message buffer
    XBTMSG_fnSendAddressed  - Send a PGN with addresses set

NOTES:
    None.
    
HISTORY:
$Log: xbtldefs.h $

    ***********************************************
    Revision: XanBus_DaleM/2
    User: DaleM     Date: 01/28/05  Time: 12:41AM
    Removed old VSS historys.
    
    
=============================================================================*/

#ifndef XBTLDEFS_H
#define XBTLDEFS_H

/*=============================================================================
                                    Defines
=============================================================================*/

/*
    Control flag definitions for PGN List Flags field

    There is no distiction between singleframe and multiframe, a multiframe
    can be sent as a singleframe if the message is small enough to fit.
    FastPackets must be distinct from singleframes and multiframes because
    there is no way to tell the protocols apart. These flags are mutually
    exclusive.

    Use the no release flag to control whether the stack releases received
    messages or not. Some messages are pass to loader application and these
    messages should not be release by the stack.

    Some received messages are commands. Commands can only be accepted if they
    come from a diagnostics node. If a PGN has the Diag Command flag the stack
    will send a NAK if the source address is not from a diagnostics node.

    If sending of a PGN (using the ISO Req PGN) is not supported the node
    should reply with a aknowlegde NAK. A subset of PGNs are requires to use
    an aknowledge with Access Denied. The Access Deny flag is used to mark this
    subset of messages so that the correct reply will be generated.

    Some received messages should be addressed specifically to the node. 
    Use the no broadcast flag to reject messages addressed to the global
    (broadcast) address.

    Most messages should be ignored if the node does not have a valid
    address (not on-line). Some messages need to be handled even when
    off-line. Use the allow offline flag to not reject messages when offline.
*/

#define XBT_SINGLEFRAME     0x00    // Message uses singleframe protocol
#define XBT_MULTIFRAME      ( XBT_SINGLEFRAME )   // Multiframe
#define XBT_FASTPACKET      0x01    // Message uses fastpacket protocol
#define XBT_PROTOCOL_MASK   0x01    // Bits of PGN flag involving protocol

#define XBT_ALLOW_OFFLINE   0x08    // Handle message even if off-line
#define XBT_NO_RELEASE      0x10    // Do not automatically release message
#define XBT_DIAG_COMMAND    0x20    // Accept command from DIAG only
#define XBT_NO_BROADCAST    0x40    // Reject message if broadcast
#define XBT_ACCESS_DENY     0x80    // Reject Tx request with access denied

/*
    Macro for converting three contiguous little endian bytes into a PGN
*/
#define XBT_mMAKE_PGN( puc )    ( ( puc )[ 0 ] + \
                                (uint16)( ( puc )[ 1 ] * 0x100 ) + \
                                (uint32)( ( puc )[ 2 ] * 0x10000 ) )

/*=============================================================================
                                   Data Types
=============================================================================*/

/*
    Message identifier data structure for XanBus use. This structure
    contains data fields from the arbitration portion of the CAN frame
*/
typedef struct zMSG_ID
{
    uint32 ulPgn;           // Parameter Group Number - message identifier
    uchar8 ucPriority;      // Priority
    uchar8 ucSrcAddr;       // Source Address
    uchar8 ucDestAddr;      // Destination Address 
} XBT_tzMSG_ID;

/*
    Data structure for table of supported Parameter Group Number messages
*/
typedef struct zPGN_LIST
{
    uint32 ulPgn;                           // Parameter Group Number
    uchar8 ucFlags;                         // Control Flags
    uint16 uiMinLen;                        // Minimum allowable PGN length
    void   (*fnRecv)( XBT_tzMSG *ptzMsg );  // Receive function pointer
    void   (*fnSend)( uchar8 ucDest );      // Requested send function pointer
    uchar8 ucSeqNum;                        // Fastpacket sequence number
} XBT_tzPGN_LIST;

/*=============================================================================
                                Function Prototypes
=============================================================================*/

// From xbtaddr.c
extern uchar8 XBTADDR_fnGetAddr( void );

extern void XBTADDR_fnSendClaim( uchar8 ucDest );       // Destination address

extern void XBTADDR_fnRecvClaim( XBT_tzMSG *ptzMsg );   // Received message pointer

// From xbterror.c

// From xbtframe.c
extern void XBTFRAME_fnBusyMsgCheck( void );

extern XBT_tzMSG *XBTFRAME_fnStartMultiFrameRx( uint32 ulPgn,           // Parameter Group Number
                                                XBT_tzMSG *ptzConn );   // Message pointer

extern uchar8 XBTFRAME_fnAbortMultiFrameRx( uint32 ulPgn );    // Parameter group number

extern XBT_tzMSG *XBTFRAME_fnRecvFrame( uint16 *puiReason );    // Pointer to return reason code

extern uint16 XBTFRAME_fnSendSingleFrame( XBT_tzMSG *ptzMsg );  // Message pointer

extern uchar8 XBTFRAME_fnBusTxState( void );

extern uint16 XBTFRAME_fnInit( void );

// From xbtload.c
extern void XBTLOAD_fnRecvLoadInfo( XBT_tzMSG *ptzMsg );    // Received message pointer

extern void XBTLOAD_fnRecvLoadBlock( XBT_tzMSG *ptzMsg );   // Received message pointer

extern void XBTLOAD_fnReqLoadStatus( uchar8 ucDest );       // Destination address

extern void XBTLOAD_fnRecvResetCmd( XBT_tzMSG *ptzMsg );    // Received message pointer

// From xbtmsg.c
extern const XBT_tzPGN_LIST *XBTMSG_fnLookupPgn( uint32 ulPgn );   // Parameter group number

extern void XBTMSG_fnSendConnMan( uchar8 ucConnMan,     // ISO Connection Management message type
                                  XBT_tzMSG *ptzMsg );  // Message pointer

extern XBT_tzMSG *XBTMSG_fnNewMsg( uint32 ulPgn );     // Parameter group number

extern void XBTMSG_fnSendAddressed( XBT_tzMSG *ptzMsg );    // Message pointer

// From xbtplatform.c

#endif
