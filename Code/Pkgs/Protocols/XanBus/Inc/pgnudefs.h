/******************************************************************************

  (c) 2004 - 2018 Schneider Electric SE. All rights reserved.
  All trademarks are owned or licensed by Schneider Electric SE,
  its subsidiaries or affiliated companies.

****************************************************************************//**

  @file  pgnudefs.h
  @brief Universal definitions for Parameter Groups

  This file contains defines and types intended to be used by the
  application interfacing with PGN, CAN-based protocols (e.g., XanBus).
  The file defines the structures used by the application, and the
  enums used in many of the structure fields.

*******************************************************************************/

#ifndef PGNUDEFS_H
#define PGNUDEFS_H

/*==============================================================================
                              Includes
==============================================================================*/

#include "LIB_stddefs.h"    // Standard defintions for all projects

/*==============================================================================
                              Defines
==============================================================================*/

//--------------------------- Field Value Limits ---------------------------
#define PGN_UL_MAX                  ( 0xFFFFFFFFU )
#define PGN_SL_MAX                  ( 2147483647L )
#define PGN_UI_MAX                  ( 0xFFFFU )
#define PGN_SI_MAX                  ( 32767 )
#define PGN_UC_MAX                  ( (uchar8) 0xFFU )
#define PGN_SC_MAX                  ( 127 )
#define PGN_SI_MIN                  ( -32768 )

#define PGN_UL_NO_DATA              ( PGN_UL_MAX )      // Unsigned Long
#define PGN_UL_OUT_OF_RANGE         ( PGN_UL_MAX - 1U )
#define PGN_UL_RESERVED             ( PGN_UL_MAX - 2U )

#define PGN_SL_NO_DATA              ( PGN_SL_MAX )      // Signed Long
#define PGN_SL_OUT_OF_RANGE         ( PGN_SL_MAX - 1 )
#define PGN_SL_RESERVED             ( PGN_SL_MAX - 2 )

#define PGN_UI_NO_DATA              ( PGN_UI_MAX )      // Unsigned Integer
#define PGN_UI_OUT_OF_RANGE         ( PGN_UI_MAX - 1U )
#define PGN_UI_RESERVED             ( PGN_UI_MAX - 2U )
#define PGN_UI_MAX_DATA             ( PGN_UI_MAX - 3U )

#define PGN_SI_NO_DATA              ( PGN_SI_MAX )      // Signed Integer
#define PGN_SI_OUT_OF_RANGE         ( PGN_SI_MAX - 1 )
#define PGN_SI_RESERVED             ( PGN_SI_MAX - 2 )

#define PGN_UC_NO_DATA              ( PGN_UC_MAX )      // Unsigned Char
#define PGN_UC_OUT_OF_RANGE         ( PGN_UC_MAX - 1U )
#define PGN_UC_RESERVED             ( PGN_UC_MAX - 2U )

#define PGN_SC_NO_DATA              ( PGN_SC_MAX )      // Signed Char
#define PGN_SC_OUT_OF_RANGE         ( PGN_SC_MAX - 1 )
#define PGN_SC_RESERVED             ( PGN_SC_MAX - 2 )

#define PGN_PGN_NO_DATA             ( 0xFFFFFF )        // PGN does not exist
#define PGN_PGN_OUT_OF_RANGE        ( 0xFFFFFE )        // PGN out of range
#define PGN_PGN_RESERVED            ( 0xFFFFFD )        // PGN resevered
#define PGN_PGN_MAX                 ( 0x01FFFF )        // Highest possible PGN
#define PGN_PGN_PROP_MSG            ( 0x00EF00 )        // Addressable Proprietary message in RV-C and J1939

// Define the values to be used when a device does not support
// a certain parameter of a PGN data field.
#define PGNHAN_UC_DATA_NOT_AVAILABLE ( PGN_UC_NO_DATA )
#define PGNHAN_UI_DATA_NOT_AVAILABLE ( PGN_UI_NO_DATA )
#define PGNHAN_UL_DATA_NOT_AVAILABLE ( PGN_UL_NO_DATA )
#define PGNHAN_SC_DATA_NOT_AVAILABLE ( PGN_SC_NO_DATA )
#define PGNHAN_SI_DATA_NOT_AVAILABLE ( PGN_SI_NO_DATA )
#define PGNHAN_SL_DATA_NOT_AVAILABLE ( PGN_SL_NO_DATA )

//--- Define maimum length of messages for protocols ---
#define PGN_MAX_LEN_SINGLEFRAME     ( 8 )
#define PGN_MAX_LEN_FASTPACKET      ( 223 )
#define PGN_MAX_LEN_MULTIFRAME      ( 1785 )

//--- Define length of node name ---
#define PGN_NAME_LEN                ( PGN_MAX_LEN_SINGLEFRAME )

// Default/ignorable Group Function code
#define PGN_GRP_FUNC_DO_NOT_CARE    ( -1 )

// Sequence Identifier Maximum
#define PGN_SID_MAX                 ( 250 )             // Sequence Id Max

//--- Common Network Address Definitions ---
#define PGN_ADDR_GLOBAL             ( 255 )
#define PGN_ADDR_NULL               ( 254 )
#define PGN_ADDR_RESERVE1           ( 253 )
#define PGN_ADDR_RESERVE2           ( 252 )
#define PGN_ADDR_LOWEST             ( 0 )       // Lowest NMEA address
#define PGN_ADDR_HIGHEST            ( 251 )     // Highest NMEA address
#define PGN_ADDR_DONT_CARE          ( PGN_ADDR_RESERVE1 )

// Define common message priorities
#define PGN_PRIORITY_NORMAL         ( 6 )   // Normal message priority
#define PGN_PRIORITY_TP_XFER        ( 7 )   // Priority for ISO Tp Xfer messages

//--- Multi-Frame Definitions ---
#define PGN_NOT_FASTPACKET          ( 0xFF )
#define PGN_MULTI_PKT_DATA_BYTES_PER_FRAME   ( 7 )
#define PGN_FAST_PKT_DATA_BYTES_1ST_FRAME    ( 6 )
#define PGN_FAST_PKT_DATA_BYTES_PER_FRAME    ( 7 )

// PGN Info flags values
#define PGN_FLAG_RX             0x0001  // Device can receive this PGN
#define PGN_FLAG_TX             0x0002  // Device can send this PGN
#define PGN_FLAG_REQ            0x0004  // Network can request this PGN from device
#define PGN_FLAG_DYN            0x0008  // PGN receive can be controlled dynamically
#define PGN_FLAG_PDU1           0x0040  // PGN is PDU1 format
#define PGN_FLAG_PDU2           0x0080  // PGN is PDU2 format
#define PGN_FLAG_RX_DNY         0x0100  // If not receivable, reply with Deny
#define PGN_FLAG_RX_NAK         0x0200  // If not receivable, reply with NACK
#define PGN_FLAG_RX_IGN         0x0400  // If not receivable, ignore
#define PGN_FLAG_NRSP_TX_DNY    0x1000  // If not respondable, reply with Deny
#define PGN_FLAG_NRSP_TX_NAK    0x2000  // If not respondable, reply with NACK
#define PGN_FLAG_RESP_TX_DNY    0x4000  // If respondable, reply with Deny
#define PGN_FLAG_RESP_TX_PGN    0x8000  // If respondable, reply with PGN

// PG Message Type Flag values
#define PGN_PG_TYPE_FLAG_MASK   0x000F  // 
#define PGN_PG_TYPE_CMD         0x0001  //  Command
#define PGN_PG_TYPE_REQ         0x0002  //  Request
#define PGN_PG_TYPE_RSP         0x0003  //  Broadcast/Response
#define PGN_PG_TYPE_ACK         0x0004  //  Acknowledgement
#define PGN_PG_TYPE_GFN         0x0005  //  Group Function
#define PGN_PG_TYPE_CFG         0x0006  //  Configuration

// PG Send Data Control flags
#define PGN_SDCTRL_SIM_RX       0x01    // Simulate receive of sent data

// Xanbus message count field max value
#define PGN_XB_MSGCOUNT_MAX     63      // (6-bits)

/*==============================================================================
                            Type Definitions
==============================================================================*/

// Need to forward declare these structure typedefs for TI compiler
typedef struct PGN_zRECV_DATA PGN_tzRECV_DATA;
typedef struct PGN_zWANT_DATA PGN_tzWANT_DATA;

/*==============================================================================
                                Enums
==============================================================================*/

// PGN field types used by tables for packing and unpacking
typedef enum PGN_eTYPE
{
    PGN_eTYPE_BIT,
    PGN_eTYPE_UCHAR,
    PGN_eTYPE_SCHAR,
    PGN_eTYPE_UINT,
    PGN_eTYPE_SINT,
    PGN_eTYPE_ULONG,
    PGN_eTYPE_SLONG,
    PGN_eTYPE_STRING,
    PGN_eTYPE_FLOAT,
    PGN_eTYPE_DOUBLE,
    PGN_eTYPE_POINTER,
    PGN_eTYPE_UNDEFINED
} PGN_teTYPE;

//  Definitions of error codes. The Teleflex codes are echos of the codes
//  defined in libenv.h

typedef enum PGN_eERR
{
    PGN_eERR_NONE = 0,              // No error (Teleflex TFXCR_OK)

    PGN_eERR_TFX_IDLE,              // Internal operation
    PGN_eERR_TFX_TABLE_FULL,        // Referenced table is full
    PGN_eERR_TFX_HEAP_ALLOC_FAIL,   // Request to allocate heap failed
    PGN_eERR_TFX_NOT_FOUND,         // Requested item was not found
    PGN_eERR_TFX_NEW_DATA,          // New Data from routine
    PGN_eERR_TFX_NO_DATA,           // No Data from routine
    PGN_eERR_TFX_INVALID_PORT,      // Invalid Comm Port
    PGN_eERR_TFX_BUS_OFF,           // CAN Bus is in Bus-Off condition 
                                    // (no further transmissions possible)
    PGN_eERR_TFX_BUS_WARNING,       // CAN Bus is at Error Warning Level Status
    PGN_eERR_TFX_MSG_NOT_HANDLED,   // Message was not handled by routine 
    PGN_eERR_TFX_LIB_BUSY,          // Tick called while library already running
    PGN_eERR_TFX_DRV_BUSY,          // Driver cannot accept more data
    PGN_eERR_TFX_DRV_RX_BUFFER_ALLOCATION_FAILURE, // Allocation failure for 
    // Receive Buffer 
    PGN_eERR_TFX_DRV_TX_BUFFER_ALLOCATION_FAILURE, // Allocation failure for 
    // Transmit Buffer
    PGN_eERR_TFX_MSG_HANDLE_NOT_AVAILABLE,  // All message handles are used up
    PGN_eERR_TFX_INVALID_MSG_HANDLE,        // Message handle is invalid
    PGN_eERR_TFX_PORT_INIT_ERR,     // Got an error while initializing a port.
    
    PGN_eERR_NO_DOB,                // No Data Object
    PGN_eERR_NO_MEM,                // Failed to allocate memory
    PGN_eERR_NO_ROOM,               // Could not find free entry in fixed list
    PGN_eERR_NOT_SET,               // Timer callback called but timer not set
    PGN_eERR_NO_PGDESC,             // Failed to find PGN descriptor
    PGN_eERR_BUF_SIZE,              // Message does not fit in allocated space
    PGN_eERR_TIMEOUT,               // Timer expired before message complete
    PGN_eERR_PACK,                  // Failure when packing fixed PGN fields
    PGN_eERR_PACK_MORE,             // Failure when packing variable PGN fields
    PGN_eERR_UNPACK,                // Failure when unpacking fixed PGN fields
    PGN_eERR_UNPACK_MORE,           // Failure when unpacking variable PGN fields
    PGN_eERR_FIELD_SIZE,            // Error when (un)packing fields
    PGN_eERR_BAD_STATE,             // Fault in address claiming state machine
    PGN_eERR_BAD_VALUE,             // Invalid value
    PGN_eERR_BAD_HEAP,              // Heap corrupted
    PGN_eERR_LAST                   // Keep this last, use to determine number of entries
} PGN_teERR;

// Definitions for sources of errors
// These source definitions are turned into strings in xbdata_fnGetErrorStr().
typedef enum PGN_eESOURCE
{
    PGN_eESRC_NONE,                 // Reserve Zero spot for none
    PGN_eESRC_TFX_IN_TICK_A,        // Tick input processing Port A
    PGN_eESRC_TFX_OUT_TICK_A,       // Tick output processing Port A
    PGN_eESRC_TFX_LIB_INIT,         // TFX library initialization
    PGN_eESRC_TFX_SET_TBASE,        // TFX set timebase
    PGN_eESRC_TFX_QUEUE_DOB,        // TFX queue data object
    PGN_eESRC_MULTIPACKET,          // Multipacket processing
    PGN_eESRC_PACK,                 // Message packing
    PGN_eESRC_UNPACK,               // Message unpacking
    PGN_eESRC_TIMER,                // Timer processing
    PGN_eESRC_CLAIMING,             // Address claim processing
    PGN_eESRC_MESSAGE,              // Message content
    PGN_eESRC_HEAP,                 // Heap processing
    PGN_eESRC_LAST_SOURCE       // Keep this last, use to determine # of entries
} PGN_teESOURCE;

// Standard defines for ISO Ack Status control byte
typedef enum PGN_eACK_STS
{
    PGN_eACK_STS_ACK                    =   0,
    PGN_eACK_STS_NAK                    =   1,
    PGN_eACK_STS_DNY                    =   2
} PGN_teACK_STS;

// Make a generic enum type
typedef enum PGN_eENUM
{
    placeholder
} PGN_teENUM;

/*=============================================================================
                              Structures
==============================================================================*/

//! PGN field descriptor used for encoding/decoding messages
typedef struct PGN_zFIELD_DESC
{
    PGN_teTYPE teType;          ///< Data type of the field
    uint16 uiMsgOffsetBits;     ///< Offset to field in message buffer (bits)
    uint16 uiFieldCount;        ///< Number of bytes or bits in field
    uint16 uiStructOffset;      ///< Offset to field in structure in bytes
    uint16 uiStructSize;        ///< Size of field in structure in bytes
} PGN_tzFIELD_DESC;


//! PGN descriptor used for encoding/decoding messages
typedef struct PGN_zPGN_DESC
{
    sint16 siGrpFuncFieldIndex; ///< Group Function field number if necessary
    uchar8 ucPriority;          ///< Default message priority
    void (*pfnRecv)( PGN_tzRECV_DATA *ptzRecv );    ///< Receive handler
    void (*pfnWant)( PGN_tzWANT_DATA *ptzWant );    ///< Request handler
    uint16 uiFixedLen;          ///< Length of the fixed part of the message in bytes
    uint16 uiFixedSize;         ///< Size of the structure at application level
    uint16 uiFixedFields;       ///< Number of useful fields in the fixed part of the message
    const PGN_tzFIELD_DESC * XT_FAR pktzFixedList;  ///< Array of definitions for the fixed fields
    uint16 uiVarLen;            ///< Length of the variable parts of the message in bytes
    uint16 uiVarSize;           ///< Size of the structure used for the variable length part
    uint16 uiVarFields;         ///< Number of fields within the variable structure
    const PGN_tzFIELD_DESC * XT_FAR pktzVarList;    ///< Array of definitions for the variable fields
} PGN_tzPGN_DESC;

//! PGN Message List entry
typedef struct PGN_zPGN_INFO
{
    uint32 ulPgn;                           ///< The Parameter Group Number of the message
    sint16 siGrpFunc;                       ///< The Group Function Code if necessary
    uchar8 ucFastPacketSeqIdx;              ///< Unique number for every FastPacket PGN
    uint16 uiFlags;                         ///< Control flags
    uint16 uiPgTypeFlags;                   ///< Parameter Group Type flags
    PGN_tzPGN_DESC * XT_FAR pktzDesc;       ///< Descriptor for PGN
} PGN_tzPGN_INFO;

//! Structure for referencing PGN tables
struct PGN_zWORK_BLOCK
{
    const uint32 * const kaulRX_PGN_LIST;         // Receivable PGN list
    const uint32 * const kaulTX_PGN_LIST;         // Transmittable PGN list
    const PGN_tzPGN_INFO * const kptzALL_PGN_LIST;// All supported PGN list
    uint16 const kuiSIZE_OF_RX_PGN_LIST;    // Number of entries in Rx PGN list
    uint16 const kuiSIZE_OF_TX_PGN_LIST;    // Number of entries in Tx PGN list
    uint16 const kuiSIZE_OF_ALL_PGN_LIST;   // Number of entries in All PGN list
    uchar8 * const kpucDynamicList;         // Pointer to Dynamic Rx list
    const uint16 kuiMAX_DYN_RX_LIST;        // # of entries in dynamic Rx list
    uchar8 * const kpucFastPacket_Seq_List; // Pointer to FP sequence num. list 
    const uchar8 kuiMAX_FASTPKT_SEQ_LIST;   // Size of FP sequence number list
    tucBOOL (*fnRxCheck)       // Check the message receivability 
                        ( PGN_tzRECV_DATA *ptzRecv, // pointer to Recv struct
                          uint16 uiSrcLen,          // data length
                          uchar8 ucMsgCount );      // message count in cfg message

    void (*fnRxHandler)        // Route to the receive handler
                        ( PGN_tzRECV_DATA *ptzRecv, // pointer to recv struct
                          PGN_teENUM teErrorCode ); // error code
};


//! Structure for passing a received message to application
struct PGN_zRECV_DATA
{
    uint32 ulPgn;                       // The PG Number of message
    sint16 siGrpFunc;                   // The Group Function Code
    const PGN_tzPGN_INFO *pktzPgnInfo;  // Pointer to PGN Info (the structure) 
    void *pvStruct;                     // Pointer to message (the data)
    uchar8 ucSrcAddr;                   // Source address from message
    uchar8 ucDestAddr;
    uchar8 ucPort;                      // Port message received on
    uchar8 ucUseCount;                  // Use counter of structure memory
};

// Structure for requesting that a message be sent
struct PGN_zWANT_DATA
{
    tucBOOL             tucSolicited;   // TRUE for requested, FALSE for timer
    uchar8              ucDestAddr;     // Address to send Pg
    uint32              ulPgn;          // PGN we want
};
    

// Structure for passing a transmit message from application
typedef struct PGN_zSEND_DATA
{
    const PGN_tzPGN_INFO *pktzPgnInfo;  // Pointer to PGN Info 
    uchar8               ucDestAddr;    // Address to send Pg
    void                 *pvPgMsg;      // Pointer to message as Parameter Group
    uint16               uiPgMsgSize;   // Number of byte in PgMsg
    uchar8               ucCtrlFlags;   // Control flags for send data
} PGN_tzSEND_DATA;

/*==============================================================================
                          External/Public Constants
==============================================================================*/


/*==============================================================================
                          External/Public Variables
==============================================================================*/


/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/

#endif
