/******************************************************************************

  (c) 2017 - 2023 Xantrex Canada ULC. All rights reserved.
  All trademarks are owned or licensed by Xantrex Canada ULC,
  its subsidiaries or affiliated companies.

***************************************************************************//**

  @addtogroup can_drv_shim  CAN Driver Shim to XanBus Library
  @{
  @brief XanBus stack CAN driver shim interface

    Defines the types and API functions implemented by the
    Xanbus stack CAN driver shim.

*******************************************************************************/

#ifndef CANDRIVER_H
#define CANDRIVER_H

/*==============================================================================
                              Includes
==============================================================================*/

#include "libenv.h"
#include "LIB_stddefs.h"
#include "candrv.h"

/*==============================================================================
                              Defines
==============================================================================*/

//! Defines for Comm Commands.
#define NO_COMMAND 0

#define ONLINE     1  ///< puts communications into a two way online mode with
                      ///< the handle specified external communication bus
                      ///< (note: When a unit is placed back online Address will
                      ///< need to be done.)

#define OFFLINE    2  ///< ends participation in communications with the handle
                      ///< specified external communication bus.  (Note: No
                      ///< Communication Message will be received or sent)

#define LISTENONLY 3  ///< Will handle only received messages from the handle
                      ///< specified external communication bus. (Note: No
                      ///< Message Acknowledgements will be transmitted.)

//! Status bits from controller status
#define SB_BUSOFF  0x01 ///< Bus off
#define SB_ERRINT  0x02 ///< Error int (details in general purpose error status word)
#define SB_ERRWRN  0x04 ///< Errors at warning level
#define SB_TXOK    0x10 ///< Transmission OK
#define SB_RXOK    0x20 ///< Receive OK

//! Error values relating to controller status
//! The first 6 match the 167 error values.
#define ERRVAL_NONE   0 ///< No errors.
#define ERRVAL_STUFF  1 ///< Stuff error
#define ERRVAL_FORM   2 ///< Form error
#define ERRVAL_ACK    3 ///< Ack error
#define ERRVAL_BIT1   4 ///< Bit 1 error
#define ERRVAL_BIT0   5 ///< Bit 0 error
#define ERRVAL_CRC    6 ///< CRC error
#define ERRVAL_OVFL   7 ///< RX Overflow

//! Invalid message handle definition
#define INV_MSG_HANDLE  ( -1 )

/*==============================================================================
                            Type Definitions
==============================================================================*/

//! CAN receive callback function type
typedef int (*TFXCANDRVCB)( tfxINT8 i8CBReason, void *pCanFrame, UINT32 i32StatusInfo, UINT32 u32UserData );

//! Type for a callback function
typedef void (*TFXDRV_tpCALLBACK)( uchar8 Channel );

/*=============================================================================
                              Structures
==============================================================================*/

//! CAN message structure definition
typedef struct
{
    UINT8       m_u8Priority;
    UINT8       m_u8DataPage;
    UINT8       m_u8PF;
    UINT8       m_u8PS;  // destination address or group extension
    UINT8       m_u8SA;
    UINT8       m_u8Data[8];
    UINT8       m_u8DataByteCount;
} CANDATA;

//! CAN Frame definition
typedef struct
{
    CANDATA     m_CanData;
    TFXCANDRVCB pFrameCB;      // CAN Driver callback
    UINT32      u32UserData;
} CANFRAME;

//! Structure for collecting communications statistics
typedef struct zCAN_COMM_STATS
{
    uint32 ulTxExtFrames;       // Count of transmitted extended frames
    uint32 ulRxExtFrames;       // Count of received extended frames
    uint16 uiRxErrCount;        // Count of times Rx went to error
    uint16 uiTxErrCount;        // Count of times Tx went to error
    uint16 uiBusoffCount;       // Count of times CAN went to bus off
    uint16 uiRxDropped;         // Count of lost Rx messages
    uint16 uiTxDropped;         // Count of lost Tx frames
    uchar8 ucLastTxStatus;      // Save previous Tx Status
    uchar8 ucLastRxStatus;      // Save previous Rx Status
} CAN_tzCOMM_STATS;


#ifdef __CPLUSPLUS
extern "C"
{
#endif // __CPLUSPLUS


/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/


// Prototypes for the driver interface routines.
// CAN Peripherals
DllExport TFXCAN_RETURNS TFXDRV_i16InitRxCallback(void (*pFn)(uint32 data));

DllExport TFXCAN_RETURNS TFXDRV_i16InitBuffers(CANPORT u8PortNumber);
DllExport TFXCAN_RETURNS TFXDRV_i16InitController(CANPORT u8PortNumber,
                                                  uchar8 ucBitRate );
DllExport TFXCAN_RETURNS TFXDRV_i16ControllerState(CANPORT u8PortNumber);
DllExport tucBOOL TFXDRV_fnIsBlocked( CANPORT Channel );

DllExport TFXCAN_RETURNS TFXDRV_fnGetCommStats( uchar8 Channel, CAN_tzCOMM_STATS *ptzStats );
DllExport TFXCAN_RETURNS TFXDRV_fnClearCommStats( uchar8 Channel );
DllExport TFXCAN_RETURNS TFXDRV_i16ConnectController(CANPORT u8PortNumber,
                                                     uchar8 ucBitRate );
DllExport void TFXDRV_fnStatusTick( CANPORT Channel );
// Message Handling
// *** RECEIVE
//  checks for data to receive
DllExport TFXCAN_RETURNS TFXDRV_i16ReceiveFrameCheck(CANPORT u8PortNumber);
//  retrieves receive data from buffer
DllExport TFXCAN_RETURNS TFXDRV_i16ReceiveFrame (CANPORT u8PortNumber, CANFRAME *pFrame);
//  retrieves receive raw data from buffer
DllExport TFXCAN_RETURNS TFXDRV_i16ReceiveForeignFrame (CANPORT u8PortNumber, CANDRV_tzCAN_FRAME *pFrame);

// *** SEND
//  checks to see if space available to accept send frame buffer
DllExport TFXCAN_RETURNS TFXDRV_i16SendFrameCheck (CANPORT u8PortNumber);
//  sends frame to transmit buffers
DllExport TFXCAN_RETURNS TFXDRV_i16SendFrame (CANPORT u8PortNumber, CANFRAME *pFrame, INT16 *pi16MessageID);
//  sends frame to transmit buffers
DllExport TFXCAN_RETURNS TFXDRV_i16SendForeignFrame (CANPORT u8PortNumber, CANDRV_tzCAN_FRAME *pFrame, INT16 *pi16MessageID);
// output next frame in buffer (if port not busy)
TFXCAN_RETURNS TFXDRV_WriteFrame(CANPORT u8PortNumber);

// *** Tools
DllExport TFXCAN_RETURNS TFXDRV_DebugToolPrint(  CANFRAME *pi16Frame );

// Message handle functions.
DllExport void TFXDRV_vInit_msg_handles(void);
DllExport TFXCAN_RETURNS  TFXDRV_iGet_msg_status(INT16 iHandle, UINT16 *puiStatus, INT16 *piNumFramesTransmitted);
DllExport TFXCAN_RETURNS  TFXDRV_iClear_msg_handle(INT16 iHandle);
DllExport TFXCAN_RETURNS TFXDRV_fnInstallWakeupCb( uchar8 Channel,
                                    TFXDRV_tpCALLBACK pFunc );
DllExport TFXCAN_RETURNS TFXDRV_fnSleep( uchar8 Channel,
                                uchar8 WakeEnable );
DllExport TFXCAN_RETURNS TFXDRV_fnWakeUp( uchar8 Channel );

#ifdef __CPLUSPLUS
}
#endif // __CPLUSPLUS

#endif // CANDRIVER_H

//*****************************************************************************
// End of file.
//! @}
//*****************************************************************************

