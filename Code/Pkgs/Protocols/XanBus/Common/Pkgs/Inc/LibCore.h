/*=============================================================================
Copyright 2003 - 2022 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by 
copyright. The reproduction, in whole or in part, by anyone without the written 
approval of Xantrex is prohibited.

FILE NAME:  LibCore.h

PURPOSE:
   Library Core Entry Points and structures
   
FUNCTION(S):
    TFXLibInit                  - Initialize the Teleflex library
    TFXLibTimeBase              - 
    TFXLibMilliToTicks          - Convert milliseconds to ticks
    TFXLibTickInput             - 
    TFXLibCycleInput            - 
    TFXLibTickOutput            - 
    TFXLibCycleOutput           - 
    TFXLibMaxXmitFramesPerCycle - 
    TFXLibMaxRcvFramesPerCycle  - 
    TFXLibDelay                 - Delay for a specified time
    TFXLibCancelDelay           - Cancel a delay
   
NOTES:

CHANGE HISTORY:
$Log: LibCore.h $

    ***********************************************
    Revision: XanBus4_TomL/1
    User: TomL     Date: 01/20/10  Time: 10:24PM
    Matched case of include statements to files

    ***********************************************
    Revision: XanBus_HollyZ/5
    User: HollyZ     Date: 08/22/07  Time: 10:48PM
    Set the default alive timer for the transmitting message to 1.25s

    ***********************************************
    Revision: XanBus_HollyZ/4
    User: HollyZ     Date: 05/25/06  Time: 07:12PM
    Revert Xb stack to V1.2.7

    ***********************************************
    Revision: XanBus_HenryW/1
    User: HenryW     Date: 08/19/05  Time: 08:07PM
    - cleaned up history list.

    ***********************************************
    Revision: XanBus_HollyZ/3
    User: HollyZ     Date: 08/02/05  Time: 05:19PM
    Added Link List struct and moved the pointer m_pNextDob to the first element
    in struct pgndob 

    ***********************************************
    Revision: XanBus_HollyZ/2
    User: HollyZ     Date: 06/17/05  Time: 04:33PM
    Added bit rate parameter for TFXLibInit()

    ***********************************************
    Revision: XanBus_HollyZ/1
    User: HollyZ     Date: 06/03/05  Time: 10:21PM
    Added TFXLibGetTimeBase()

    ***********************************************
    Revision: XanBus_DaleM/3
    User: DaleM     Date: 03/22/05  Time: 11:27PM
    - Fast packet sequence numbers were chosen and discarded if frame could
    not be queue for transmission.

    ***********************************************
    Revision: XanBus_DaleM/2
    User: DaleM     Date: 02/17/05  Time: 04:43PM
    Need to forward declare structures for TI compiler

    ***********************************************
    Revision: GateWayPfrmANMEA_DaleM/1
    User: DaleM     Date: 02/16/05  Time: 11:13PM
    Cleaned up old history list

=============================================================================*/
#ifndef TFXLIBCORE_H
#define TFXLIBCORE_H

#include "cdrive.h"
#include "xbudefs.h"
#include "pgnudefs.h"

//***************************************************************************
// PGNDOb
//  
// The PGNDOb contains all the methods and properties needed to create,
// obtain frames and receive frames
//***************************************************************************

typedef enum
{
    TFXCB_FRAME_OUT,  // frame related to pgn has been placed on can bus
                      // no data is passed
    TFXCB_PGN_RCVD,   // pgn has been completely received
                      // u32Data1 is PGNDOB *
                      // u32Data2 is reserved
    TFXCB_PGN_XMIT,   // pgn has been compeltely transmitted
                      // u32Data1 is PGNDOB *
                      // u32Data2 is reserved
    TFXCB_PGN_XMIT_ER,// pgn has not been compeltely transmitted
                      // u32Data1 is PGNDOB *
                      // u32Data2 is reserved
    TFXCB_PGN_ACK,    // pgn has been acked
                      // u32Data1 is PGNDOB *
                      // u32Data2 is pointer to acknowledge data
    TFXCB_PGN_TO     // pgn has timed out
                      // u32Data1 is PGNDOB *
                      // u32Data2 is reserved
} TFXLIB_CB_REASON;

typedef int (*TFXLIBCB)(TFXLIB_CB_REASON, UINT32 u32Data1, UINT32 u32Data2, UINT32 u32UserData);

#define PGN_STD_TTL     1250       // standard time to live in milliseconds

// PGNSTATE indicates the state of the PGN Data Object
typedef enum
{
    XEMPTY,         // on receiving, waiting to receive data
    XCTS,           // on receiving, xmit cts
    XEOM,           // on receiving, xmit eom
    XRTS,           // on sending, xmit rts
    XDATA,          // on sending, xmit data
    XABORT,         // on send/recv transmit abort
    XWAIT,          // transmit, waiting for cts or abort, or eom, or ACK, or ticker
                    // receiving, waiting for data or ticker
    XSTATUS,        // check status on transmit
    XCOMPLETE       // next tick, signal complete
} PGNSTATE;

typedef enum
{
    DTD_ILLEGAL,
    DTD_OK,
    DTD_FRAME_READY,
    DTD_DELETE,
    DTD_COMPLETE,   // on receives indicates that the PGN has been completly assembled
    DTD_TIMED_OUT,
    DTD_STATUS      // on transmits indicates to find out status of transmitted data
} DOBTICK_DISPOSITION;

typedef enum
{
    DTS_OK,
    DTS_FRAME_TRANS_ERROR
} DOBTERM_STATUS;

//! Packet and DOB definitions
#define DOB_DOB_FROM_HEAP       0x01
#define DOB_DATA_FROM_HEAP      0x02
#define DOB_IS_RECV             0x04
#define DOB_RECV_COMPLETE       0x08
#define DOB_IS_BAM              0x10
#define DOB_IS_MULTIPACKET      0x20
#define DOB_IS_FASTPACKET       0x40
#define DOB_IS_VIP              0x80

// Additional definitions to simplify coding

//! Packet type mask
#define DOB_PACKET_TYPE_MASK    ( DOB_IS_MULTIPACKET | DOB_IS_FASTPACKET | DOB_IS_RECV )
//! Receive fast packet type
#define DOB_IS_RECV_FASTPACKET  ( DOB_IS_FASTPACKET | DOB_IS_RECV )

typedef struct
{
    PGN             m_PGN;
    UINT32          m_CB1UserData;
    TFXLIBCB        m_CB1;                      // frameout callback
    const struct PGN_zPGN_INFO * m_pPgnInfo;
    uchar8          *m_pRxFrameFlags;           // Optional buffer for received frame flags
    uchar8          *m_pData;                   // stream data
    sint16          m_GroupFunction;
    INT16           m_i16TickerValue;
    INT16           m_eTerminatingStatus;
    uint16          m_uiFramesToSend;
    INT16           m_i16ByteCount;
    INT16           m_i16pDataBufPos;           // current read/write position
    INT16           m_CanHandle;                // handle to output via driver
    PGNSTATE        m_eState;
    uchar8          m_ucTotalFrames;
    uchar8          m_ucCurrentFrame;
    uchar8          m_ucFlags;
    uchar8          m_SourceAddr;
    uchar8          m_DestAddr;
    UINT8           m_u8Priority;
    UINT8           m_u8FastPacketSequenceNo;   // sequence number for fast packets
    CANPORT         m_i8Port;                   // which port is being used
    CANFRAME        m_Frame;                    // contains frame data associated with a xmit
    UINT8           m_NRxFrameFlags;            // Optional count of the received frame flags (below)
    uchar8          m_ucNRxFrames;              // Number of received frames so far
    uchar8          m_ucListIdx;                // Index of the link-list node holding the DOB
} PGNDOB;

struct llist
{
    struct llist  *pNext;   // Pointer to Linked list structure
};

typedef struct llist LLIST; 

//*****************************************************************************************************
// External  library entry points
//*****************************************************************************************************
typedef enum
{
    TFXSTATE_ONLINE,                    // allows reads/writes of PGN's
    TFXSTATE_OFFLINE,                   // stop all bus activity
    TFXSTATE_CONFIGURING                // can only receive in this state
} TFXSTATE;

#ifdef __CPLUSPLUS
extern "C"
{
#endif

DllExport TFXCAN_RETURNS TFXLibInit(CANPORT i8Port, uchar8 ucBitRate );
DllExport TFXCAN_RETURNS TFXLibTimeBase(INT32 i32TicksPerSecond);
DllExport INT32          TFXLibMilliToTicks(INT32 i32Milliseconds);
DllExport TFXCAN_RETURNS TFXLibTickInput(CANPORT i8Port);
DllExport TFXCAN_RETURNS TFXLibCycleInput(CANPORT i8Port);
DllExport TFXCAN_RETURNS TFXLibTickOutput(CANPORT i8Port);
DllExport TFXCAN_RETURNS TFXLibCycleOutput(CANPORT i8Port);
DllExport void           TFXLibMaxXmitFramesPerCycle(CANPORT i8Port, int nMaxFramesPerCycle);
DllExport void           TFXLibMaxRcvFramesPerCycle(CANPORT i8Port, int nMaxFramesPerCycle);


// Utility
DllExport TFXCAN_RETURNS TFXLibDelay(CANPORT i8Port, UINT16 u16Milliseconds, PGNDOB *pDOB, TFXLIBCB cbFunction, UINT32 u32UserData);
DllExport TFXCAN_RETURNS TFXLibCancelDelay(PGNDOB *pDOB);

// next declarations provide a callback facility to notify application after every n FramesProcessed (in/out)
// The return from the application function if set to '1' will cause the tick to return back to the caller and
//  stop processing additional frames
typedef int (*TFXLIB_FRAMES_PROCESSED_CB)(CANPORT i8Port, BOOL bIsInputTick, int nFramesLeftToProcessOnTick, UINT32 u32UserData);

#ifdef __CPLUSPLUS
}
#endif




#endif

