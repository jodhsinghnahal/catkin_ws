/*=============================================================================
Copyright 2005 Xantrex Technology Inc.  All rights reserved.

This source file is proprietary to Xantrex Technology Inc. and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:  candrv.h

PURPOSE:
    This header defines the Hardware Abstraction Layer for
    a Controller Area Network module.
    CAN modules often have the same feature set but the implementation
    is quite variable.  These common features are:
    - Initialize with a settable bit rate
    - Acceptance filtering set in hardware
    - Send a frame
    - Receive a frame
    - Enable and disable
    - Sleep in low-power mode and wake up on command or on detection
      of traffic.
    - Error state querying.
    - Handling of a variety of CAN related interrupts.

FUNCTION(S):
    CANDRV_fnInit       - Initialize CAN driver channel
    CANDRV_fnSetFilter  - Install a filter (in hardware)
    CANDRV_fnEnable     - Enable channel
    CANDRV_fnDisable    - Disable channel
    CANDRV_fnSleep      - Put channel to sleep
    CANDRV_fnWakeUp     - Wake channel up from sleep
    CANDRV_fnReceive    - Non-blocking receive of CAN frame
    CANDRV_fnTransmitOK - Check if there is a free transmit buffer
    CANDRV_fnTransmit   - Transmit CAN frame
    CANDRV_fnAbort      - Abort a transmitted frame
    CANDRV_fnTxStatus   - Get transmitter status and optional error count
    CANDRV_fnRxStatus   - Get receiver status and optional error count
    CANDRV_fnGetHandle  - Get handle of transmit buffer being serviced by ISR
    CANDRV_fnInitCbList - Initialize the callback list
    CANDRV_fnInstallCb  - Install a callback for an interrupt

NOTES:
    The driver that implements this API does no buffering
    of frames aside from what is implemented in hardware.
    There is also no use or assumption of any operating system services.

HISTORY:
$Log: candrv.h $

    ***********************************************
    Revision: XanBus_HenryW/1
    User: HenryW     Date: 08/19/05  Time: 08:07PM
    - cleaned up history list.

    ***********************************************
    Revision: XanBus_HollyZ/3
    User: HollyZ     Date: 06/17/05  Time: 06:06PM
    merged

    ***********************************************
    Revision: GateWayPfrmANMEA_HollyZ/1
    User: HollyZ     Date: 06/06/05  Time: 10:25PM
    Added CAN port macro definitions

    ***********************************************
    Revision: Xantrex1_JohnB/2
    User: JohnB     Date: 03/18/05  Time: 11:44PM
    Step 5 of Julie's evil twin fix.
    Note, the files are already in the kept state.

    ***********************************************
    Revision: XanBus_DaleM/3
    User: DaleM     Date: 02/22/05  Time: 10:50PM
    Corrected number of CANDRV_TX_BUFFERS

    ***********************************************
    Revision: XanBus_DaleM/2
    User: DaleM     Date: 02/22/05  Time: 06:06PM
    Added function comments

    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/2
    User: BaldeeshK     Date: 02/10/05  Time: 05:23PM
    - Initial add to Accurev of candrv header
    
    
   
=============================================================================*/

#ifndef CANDRV_H
#define CANDRV_H

/*==============================================================================
                              Includes
==============================================================================*/

#include "LIB_stddefs.h"   // Xantrex basic type definitions
#include "config.h"        // for different manufacturers

/*=============================================================================
                                    Defines
=============================================================================*/
// CAN Channel
#define CANDRV_PORT_0           ( 0 )
#define CANDRV_PORT_1           ( 1 )

// Maximum number of data bytes in a CAN frame
#define CANDRV_MAX_BYTES        ( 8 )

// Number of transmit buffers in each MSCAN module
#if ( CTRL_CONFIG_MANUFACTURER == MANUFACTURER_TEXAS_INSTRUMENTS )
#define CANDRV_TX_BUFFERS       ( 7 )
#elif ( CTRL_CONFIG_MANUFACTURER == MANUFACTURER_MOTOROLA )
#define CANDRV_TX_BUFFERS       ( 3 )
#endif

// Bit masks for the last byte of CANID structure
#define CANDRV_ID_MSK_IDE       ( 0x80 )
#define CANDRV_ID_MSK_SRR       ( 0x40 )
#define CANDRV_ID_MSK_RTR       ( 0x20 )

/*=============================================================================
                                   Data Types
=============================================================================*/

// Enum of CAN bit rates
typedef enum eBIT_RATE
{
    CANDRV_eBIT_RATE_20KBPS = 0,
    CANDRV_eBIT_RATE_125KBPS,
    CANDRV_eBIT_RATE_250KBPS,       // Speed used for XanBus
    CANDRV_eBIT_RATE_500KBPS,
    CANDRV_eBIT_RATE_1MBPS,
    CANDRV_eBIT_RATE_INVALID
} CANDRV_teBIT_RATE;

// Enum of possible driver return codes
typedef enum CANDRV_eRETURN
{
    CANDRV_eRET_SUCCESS,            // Function executed with no errors
    CANDRV_eRET_FAILURE,            // General function failure
    CANDRV_eRET_UNIMPLEMENTED,      // Function not implimented
    CANDRV_eRET_BAD_BIT_RATE,       // Bit rate not implemented
    CANDRV_eRET_BAD_FILTER,         // Filter mode not implemented
    CANDRV_eRET_INIT_FAIL,          // Could not go into init mode
    CANDRV_eRET_SLEEP_FAIL,         // Could not go into sleep mode
    CANDRV_eRET_BAD_CHANNEL,        // Channel number out of range
    CANDRV_eRET_TX_BUSY,            // All transmit buffers are busy
    CANDRV_eRET_NO_DATA            // No data in receive buffer
} CANDRV_teRETURN;

// Enum of possible initial filter settings
typedef enum eFILTER_MODE
{
    CANDRV_eFILTER_PASS_ALL,        // Pass all messages
    CANDRV_eFILTER_PASS_NONE,       // Block all messages
    CANDRV_eFILTER_INVALID          // Invalid setting
} CANDRV_teFILTER_MODE;

// Enum of transmitter or receiver status
typedef enum CANDRV_eSTATUS
{
    CANDRV_eSTATUS_OK,              // CAN controller Okay
    CANDRV_eSTATUS_WARNING,         // CAN bus at warning level
    CANDRV_eSTATUS_ERROR,           // CAN bus at error level
    CANDRV_eSTATUS_BUSOFF,          // CAN bus at Bus Off level
    CANDRV_eSTATUS_INVALID          // Status is invalid
} CANDRV_teSTATUS;

// Enum of callback types
typedef enum eCB_TYPE
{
    CANDRV_eCB_RECEIVE,         // Receive callback function
    CANDRV_eCB_OVERFLOW,        // Receive overflow callback function
    CANDRV_eCB_TXOK,            // Transmit complete callback function
    CANDRV_eCB_TXABORT,         // Transmit abort complete callback function
    CANDRV_eCB_WAKEUP,          // Wakeup callback function
    CANDRV_eCB_STATUS,          // Status callback function
    CANDRV_eCB_LAST             // Last entry
} CANDRV_teCB_TYPE;

// Common structure of a CAN frame's ID
typedef struct zCAN_ID
{
    uchar8 ucId28_24;           // Bits 28 to 24 of the ID field
                                // This field includes the SSR, IDE, and RTR
                                // bits
    uchar8 ucId23_16;           // Bits 23 to 16 of the ID field
    uchar8 ucId15_8;            // Bits 15 to 8 of the ID field
    uchar8 ucId07_00;           // Bits 7 to 0 of the ID field
} CANDRV_tzCAN_ID;

// Structure for a CAN frame
typedef struct zCAN_FRAME
{
    CANDRV_tzCAN_ID tzCanId;            // ID portion of frame
    uchar8 ucByteCount;                 // Number of bytes in aucData
    uchar8 aucData[ CANDRV_MAX_BYTES ]; // The data being sent
} CANDRV_tzCAN_FRAME;

// Type for a callback function
typedef void (*CANDRV_tpfnCALLBACK)( uchar8 Channel );

/*=============================================================================
                                   Prototypes
=============================================================================*/

// CANDRV_fnInit
//
// Purpose: 
//    Initialize the CAN driver
// Inputs: 
//    ucChannel - which CAN channel
//    teBitRate - which bit rate
//    teFilterMode - pass all or pass no ID's   
// Outputs: 
//    CANDRV_teRETURN - return ID
CANDRV_teRETURN CANDRV_fnInit( uchar8 ucChannel,
                               CANDRV_teBIT_RATE teBitRate,
                               CANDRV_teFILTER_MODE teFilterMode );

// CANDRV_fnSetFilter
//
// Purpose: 
//    Initialize the CAN filter ID
// Inputs: 
//    ucChannel - which CAN channel
//    ucFilterID - ID to set
//    ptzCanId - ID compare value 
//    ptzMask  - mask out don't cares  
// Outputs: 
//    CANDRV_teRETURN - return ID
CANDRV_teRETURN CANDRV_fnSetFilter( uchar8 ucChannel,
                                    uchar8 ucFilterID,
                                    CANDRV_tzCAN_ID *ptzCanId,
                                    CANDRV_tzCAN_ID *ptzMask );

// CANDRV_fnEnable
//
// Purpose: 
//    Enable the CAN driver
// Inputs: 
//    ucChannel - which CAN channel
// Outputs: 
//    CANDRV_teRETURN - return ID
CANDRV_teRETURN CANDRV_fnEnable( uchar8 ucChannel );


// CANDRV_fnDisable
//
// Purpose: 
//    Disable the CAN driver
// Inputs: 
//    ucChannel - which CAN channel
// Outputs: 
//    CANDRV_teRETURN - return ID
CANDRV_teRETURN CANDRV_fnDisable( uchar8 ucChannel );


// CANDRV_fnSleep
//
// Purpose: 
//    Put the CAN module to sleep
// Inputs: 
//    ucChannel - which CAN channel
//    tucWakeEnable - whether to wake up upon message arrival
// Outputs: 
//    CANDRV_teRETURN - return ID
CANDRV_teRETURN CANDRV_fnSleep( uchar8 ucChannel,
                                tucBOOL tucWakeEnable );

// CANDRV_fnWakeUp
//
// Purpose: 
//    Wake up the CAN module
// Inputs: 
//    ucChannel - which CAN channel
// Outputs: 
//    CANDRV_teRETURN - return ID
CANDRV_teRETURN CANDRV_fnWakeUp( uchar8 ucChannel );

// CANDRV_fnReceive
//
// Purpose: 
//    Receive a message from the CAN module
// Inputs: 
//    ucChannel - which CAN channel
//    ptzRxFrame - buffer to store frame
// Outputs: 
//    CANDRV_teRETURN - return ID
CANDRV_teRETURN CANDRV_fnReceive( uchar8 ucChannel,
                                  CANDRV_tzCAN_FRAME *ptzRxFrame );

// CANDRV_fnTransmitOk
//
// Purpose: 
//    Check if it is okay to transmit
// Inputs: 
//    ucChannel - which CAN channel
// Outputs: 
//    CANDRV_teRETURN - return ID
CANDRV_teRETURN CANDRV_fnTransmitOk( uchar8 ucChannel );

// CANDRV_fnTransmit
//
// Purpose: 
//    Transmit a message
// Inputs: 
//    ucChannel - which CAN channel
//    ptzTxFrame - frame to transmit
//    pucHandle - handle referring to the transmit message
// Outputs: 
//    CANDRV_teRETURN - return ID
CANDRV_teRETURN CANDRV_fnTransmit( uchar8 ucChannel,
                                   CANDRV_tzCAN_FRAME *ptzTxFrame,
                                   uchar8 *pucHandle );

// CANDRV_fnAbort
//
// Purpose: 
//    Abort a transmit message
// Inputs: 
//    ucChannel - which CAN channel
//    pucHandle - handle referring to the transmit message
// Outputs: 
//    CANDRV_teRETURN - return ID
CANDRV_teRETURN CANDRV_fnAbort( uchar8 ucChannel,
                                uchar8 ucHandle );

// CANDRV_fnTxStatus
//
// Purpose: 
//    Get the transmit status
// Inputs: 
//    ucChannel - which CAN channel
//    pucErrCount - how many errors
// Outputs: 
//    CANDRV_teSTATUS - status value of the CAN module
CANDRV_teSTATUS CANDRV_fnTxStatus( uchar8 ucChannel,
                                   uchar8 *pucErrCount );

// CANDRV_fnRxStatus
//
// Purpose: 
//    Get the receive status
// Inputs: 
//    ucChannel - which CAN channel
//    pucErrCount - how many errors
// Outputs: 
//    CANDRV_teSTATUS - status value of the CAN module
CANDRV_teSTATUS CANDRV_fnRxStatus( uchar8 ucChannel,
                                   uchar8 *pucErrCount );

// CANDRV_fnInitCbList
//
// Purpose: 
//    Initialize the callback lis
// Inputs: 
//     none
// Outputs: 
//    none
void CANDRV_fnInitCbList( void );

// CANDRV_fnInstallCb
//
// Purpose: 
//    Install a callback
// Inputs: 
//    ucChannel - which CAN channel
//    teCbType - call back type
//    tpfnFunc - function to call back
// Outputs: 
//    CANDRV_teRETURN - return ID
CANDRV_teRETURN CANDRV_fnInstallCb( uchar8 ucChannel,
                                    CANDRV_teCB_TYPE teCbType,
                                    CANDRV_tpfnCALLBACK tpfnFunc );

// CANDRV_fnGetHandle
//
// Purpose: 
//    Get the handle of the recently transmitted message
// Inputs: 
//    ucChannel - which CAN channel
// Outputs: 
//    uchar8 - handle of the recently transmitted message
uchar8 CANDRV_fnGetHandle( uchar8 ucChannel );

#endif
