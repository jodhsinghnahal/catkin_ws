/*=============================================================================
  (c) 2003 - 2019 Xantrex Canada ULC. All rights reserved.
  All trademarks are owned or licensed by Xantrex Canada ULC,
  its subsidiaries or affiliated companies.

FILE NAME:  xbtgdefs.h

PURPOSE:
    This file provides the interface to the XanBus Tiny driver

FUNCTION(S):
    XBTADDR_fnTick          - Tick the XBT timers
    XBTADDR_fnIsDiag        - Is given address from a diagnostics node       
    XBTADDR_fnOnLine        - Is this node allowed to send messages
    XBTADDR_fnBeginClaiming - Initialize and start address claim sequence
    XBTADDR_fnGetMyFunction - Determine my device function
    XBTERROR_fnSaveError    - Store a reported error code
    XBTLOAD_fnNextEvent     - Check for Download event
    XBTLOAD_fnSendLoadStatus- Send Download Status PGN
    XBTMSG_fnReleaseMsg     - Free a busy message buffer
    XBTMSG_fnSendAck        - Send a ISO Acknowledgement PGN
    XBTMSG_fnSend           - Send a PGN
    XBTMSG_fnHandleMessages - Check if there are messages to send or receive
    XBTMSG_fnInit           - Initialize XanBus Tiny driver
    XBTPLATFORM_fnLoadPrefAddr - Load Preferred Address
    XBTPLATFORM_fnLoadName  - Load node name
    XBTPLATFORM_fnLoadHwRev - Load Hardware Revision data
    XBTPLATFORM_fnLoadLoaderVersion - Check EEPROM and verify the Loader
                              Version byte is stored correctly
    
=============================================================================*/

#ifndef XBTGDEFS_H
#define XBTGDEF_H

/*==============================================================================
                              Includes
==============================================================================*/
#include "LIB_stddefs.h"    // Standard definitions for all projects
#include "xbudefs.h"        // XanBus Tiny definitions

/*=============================================================================
                                    Defines
=============================================================================*/

/*
    Determine dimension (number of entries) of an array
*/
#ifndef DIM
#define DIM(x) (sizeof(x)/sizeof(x[0]))
#endif

/*
    XanBus Tiny driver error codes
*/
#define XBT_ERR_NO_ERROR        ( 0 )                   // No error (must be 0)
#define XBT_ERR_DRV             ( 16 << 8 )             // Tiny XanBus errno
#define XBT_ERR_COMPLETE        ( XBT_ERR_DRV + 0 )     // Message is complete
#define XBT_ERR_NOT_COMPLETE    ( XBT_ERR_DRV + 1 )     // Incomplete message
#define XBT_ERR_NO_FASTPACKET   ( XBT_ERR_DRV + 2 )     // Cannot send fastpacket
#define XBT_ERR_NOT_SUPPORTED   ( XBT_ERR_DRV + 4 )     // Message no supported
#define XBT_ERR_NOT_FOUND       ( XBT_ERR_DRV + 5 )     // Not found
#define XBT_ERR_NOT_FOR_ME      ( XBT_ERR_DRV + 6 )     // Not addressed to me
#define XBT_ERR_NO_BUFFER       ( XBT_ERR_DRV + 7 )     // No free receive buffer
#define XBT_ERR_NO_TX_BUFFER    ( XBT_ERR_DRV + 8 )     // No free transmit buffer
#define XBT_ERR_NO_DATA         ( XBT_ERR_DRV + 9 )     // No receive data ready
#define XBT_ERR_BAD_STATE       ( XBT_ERR_DRV + 10 )    // Failure in address claiming state machine
#define XBT_ERR_BAD_EVENT       ( XBT_ERR_DRV + 11 )    // Unhandled message

/*
    XanBus defintions -- borrowed from xbgdefs.h
*/
#define XBT_ADDR_GLOBAL         ( 0xFF )    // Global network address
#define XBT_ADDR_NULL           ( 0xFE )    // Null network address
#define XBT_ADDR_RESERVE1       ( 0xFD )    // Reserved network address
#define XBT_ADDR_RESERVE2       ( 0xFC )    // Another reserved network address

#define XBT_PRIORITY_NORMAL     ( 6 )       // Normal message priority
#define XBT_PRIORITY_CONNMAN    ( 7 )       // Priority for connection messages

#define XBT_BUS_ERROR_ACTIVE    ( 0x01 )    // Error Active CAN bus state
#define XBT_BUS_ERROR_PASSIVE   ( 0x02 )    // Error Passive CAN bus state
#define XBT_BUS_BUS_OFF         ( 0x03 )    // Bus Off CAN bus state

#define XBT_UI_OUT_OF_RANGE     ( 0xFFFE )  // Unsigned integer data out of range

#define XBT_INDUSTRY_GROUP      ( 0 )       // Global
#define XBT_SYSTEM_INSTANCE     ( 15 )      // = 0xF

// Manufacturer 
#define XBT_MFG_XANTREX         ( 2047 )    // = 0x7FF

/*
    Tiny XanBus definitions
*/
#define XBT_TTL_NORMAL          ( 2500 )    // Normal time to live (msec)

#define XBT_FRAME_SIZE          ( 8 )       // Maximum size of a CAN frame
#define XBT_SINGLEFRAME_MAX     ( XBT_FRAME_SIZE )  // Maximum size of singleframe message
#define XBT_FASTPACKET_MAX      ( 223 )     // Maximum length of fastpacket message
#define XBT_MULTIFRAME_MAX      ( 516 )     // Maximum length of multipacket message
                                            // block number, size and 512 block data
#define XBT_MULTIPKT_FRMDATASZ  ( XBT_FRAME_SIZE - 1 )
#define XBT_FASTPKT_FRMDATASZ  ( XBT_FRAME_SIZE - 1 )

/*
    ISO Connection Management PGN Group Function codes
*/
#define XBT_CONN_RTS            ( 0x10 )    // Request To Send
#define XBT_CONN_CTS            ( 0x11 )    // Clear To Send
#define XBT_CONN_EOM            ( 0x13 )    // End Of Message
#define XBT_CONN_BAM            ( 0x20 )    // Broadcast Annouce
#define XBT_CONN_ABORT          ( 0xFF )    // Transfer Abort

/*
    ISO Acknowledge PGN control byte codes
*/
#define XBT_ACK_ACK             ( 0x00 )    // Positive Acknowledgement
#define XBT_ACK_NACK            ( 0x01 )    // Negative Acknowledgement
#define XBT_ACK_DENIED          ( 0x02 )    // Access Denied 

/*
    Download state event codes
*/
#define XBT_LOAD_NO_EVENT       ( 0 )       // No Event
#define XBT_LOAD_INFO_EVENT     ( 1 )       // Received load info message event
#define XBT_LOAD_DATA_EVENT     ( 2 )       // Received data message event
#define XBT_LOAD_STATUS_EVENT   ( 3 )       // Request for status event
#define XBT_LOAD_RESET_EVENT    ( 4 )       // Reset command event

/*
    Received Frame Flag Counts
*/
#define XBT_MULTIPKT_NRXFLAGS ( ( ( ( XBT_MULTIFRAME_MAX + XBT_MULTIPKT_FRMDATASZ - 1 ) / XBT_MULTIPKT_FRMDATASZ + 7 ) / 8 ) * 8 )
#define XBT_FASTPKT_NRXFLAGS ( ( ( ( XBT_FASTPACKET_MAX + XBT_FASTPKT_FRMDATASZ - 1 ) / XBT_FASTPKT_FRMDATASZ + 7 ) / 8 ) * 8 )


/*=============================================================================
                                   Data Types
=============================================================================*/

/*
    XanBus Tiny driver message buffer work block
*/
typedef struct zMSG
{
    // These fields are auto initialized and must stay in the same order
    uchar8 *pucData;        // Data buffer pointer
    uchar8 ucNRxFrameFlags; // Number of Received Frame Flags
    uchar8 *pucRxFrameFlags;// Received Frame Flags
    uchar8 ucFlags;         // Control Flags
    uchar8 ucPriority;      // Message priority
    uint32 ulPgn;           // Parameter Group Number
    uint16 uiDataLen;       // Data Length (expected length for fast/multi-packet)

    // These fields or not auto initialized
    uchar8 ucSrcAddr;       // Source address
    uchar8 ucDestAddr;      // Destination address
    uchar8 ucBusy;          // Buffer in use flags
    uchar8 ucFrameNum;      // Frame number
    uchar8 ucEndFrame;      // Connection management frame number of last frame
    uint16 uiTimeToLive;    // Time to live countdown ticker
    uint16 uiFrameCount;    // Count of frames received so far
} XBT_tzMSG;

/*=============================================================================
                                Function Prototypes
=============================================================================*/

// From xbtaddr.c
extern void XBTADDR_fnTick( void );

extern tucBOOL XBTADDR_fnIsDiag( uchar8 ucAddr );    // Address of node

extern tucBOOL XBTADDR_fnOnLine( void );

extern void XBTADDR_fnBeginClaiming( void );

extern uchar8 XBTADDR_fnGetMyFunction( void );

extern void XBTADDR_fnResetLoadStatusTimer(void);

// From xbterror.c
extern void XBTERROR_fnSaveError( uint16 uiCode );  // Error code

// From xbtframe.c

// From xbtload.c
extern uchar8 XBTLOAD_fnNextEvent( XBT_tzMSG **pptzMsg,     // Pointer to return pointer to message
                                   uchar8 *pucDestAddr );   // Pointer to return destination address

extern void XBTLOAD_fnSendLoadStatus( XB_teOP_STATE_LDR eOpState,   // Operating State
                                      XB_teDL_RESULT_CODE eResult,  // Result code
                                      uint16 uiBlockNum,            // Block Number
                                      uchar8 ucDestAddr,            // Destination Address
                                      uchar8 ucInstance,            // Instance number
                                      uint16 uiModelNum,            // Model Number
                                      uint16 uiHwModRev );          // Hardware Module Revision number

// From xbtmsg.c
extern void XBTMSG_fnReleaseMsg( XBT_tzMSG *ptzMsg );   // Message pointer

extern void XBTMSG_fnSendAck( uint32 ulPgn,             // Parameter Group Number
                              XB_teACK_STS eAckCode );  // Acknowledgement Code

extern void XBTMSG_fnSend( XBT_tzMSG *ptzMsg,       // Parameter Group Number
                           uchar8 ucDestAddr );     // Destination Address

extern void XBTMSG_fnHandleMessages( void );

extern uint16 XBTMSG_fnInit( void );

// From xbtplatform.c
extern uchar8 XBTPLATFORM_fnLoadPrefAddr( void );

extern void XBTPLATFORM_fnLoadName( uchar8 *pucName );  // Pointer to return Name

extern void XBTPLATFORM_fnLoadHwRev( uint16 *puiModelNum,   // Pointer to return Model Number
                                     uint16 *puiHwModRev ); // Pointer to return Hardware Module Revision number

extern tuiSTATUS XBTPLATFORM_fnLoadLoaderVersion( void );

#endif
