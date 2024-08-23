/*=============================================================================
Copyright 2005 Xantrex Technology Inc.  All rights reserved.

This source file is proprietary to Xantrex Technology Inc. and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:  candrv.c

PURPOSE:
    This driver defines the Hardware Abstraction Layer for
    the Motorola Scalable Controller Area Network module on the
    HCS12 series of microcontrollers.

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

    interrupt handlers:
        C0TX_ISR     - CAN 0 Transmit
        C0RX_ISR     - CAN 0 Recieve
        C0ERR_ISR    - CAN 0 Error
        C0WU_ISR     - CAN 0 Wakeup
        C1TX_ISR     - CAN 1 Transmit
        C1RX_ISR     - CAN 1 Recieve
        C1ERR_ISR    - CAN 1 Error
        C1WU_ISR     - CAN 1 Wakeup
        C2TX_ISR     - CAN 2 Transmit
        C2RX_ISR     - CAN 2 Recieve
        C2ERR_ISR    - CAN 2 Error
        C2WU_ISR     - CAN 2 Wakeup
        C3TX_ISR     - CAN 3 Transmit
        C3RX_ISR     - CAN 3 Recieve
        C3ERR_ISR    - CAN 3 Error
        C3WU_ISR     - CAN 3 Wakeup
        C4TX_ISR     - CAN 4 Transmit
        C4RX_ISR     - CAN 4 Recieve
        C4ERR_ISR    - CAN 4 Error
        C4WU_ISR     - CAN 4 Wakeup

    local:
        candrv_fnFilterInit   - Initialize hardware filters to pass or block everything
        candrv_fnLoadHWID     - Load driver ID structure into hardware
        candrv_fnUnloadHWID   - Unload hardware ID into driver ID structure
        candrv_fnGoSleep      - Command the CAN to go to sleep
        candrv_fnWaitBit      - Wait for a specific bit to change to a desired state
        candrv_fnSetPriority  - Set transmit priority of the next frame to transmit
        candrv_fnStatusEnum   - Convert hardware status bitmap into driver status enum
        candrv_fnMaskToHandle - Convert a bitmask to a transmitter handle
        candrv_fnHandleToMask - Convert a transmitter handle to a bitmask
        candrv_fnTxIsr        - General purpose transmit interrupt handler
        candrv_fnRxIsr        - General purpose receive interrupt handler
        candrv_fnErrIsr       - General purpose error interrupt handler
        candrv_fnWakeIsr      - General purpose wakeup interrupt handler

NOTES:
    This driver does no buffering
    of frames aside from what is implemented in hardware.
    There is also no use or assumption of any operating system services.

HISTORY:
$Log: candrv.c $

    ***********************************************
    Revision: GateWayPfrmANMEA_HollyZ/1
    User: HollyZ     Date: 06/17/05  Time: 05:59PM
    Added ability to run at different bit rate
    
    *****************  Version 30  *****************
    User: Dalem        Date: 2/02/05    Time: 10:14a
    Updated in $/PD/Network_Extensions/Code/Gateway/CAN/N2K/HAL/CAN
    Can Id structure has been changed
    Data structure names changed to add t or fn
    
    *****************  Version 29  *****************
    User: Dalem        Date: 11/25/04   Time: 3:42p
    Updated in $/PD/Network_Extensions/Code/Gateway/CAN/N2K/HAL/CAN
    Change CAN setup for 87% sample point
    
    *****************  Version 28  *****************
    User: Dalem        Date: 10/05/04   Time: 10:50a
    Updated in $/PD/Network_Extensions/Code/Gateway/CAN/N2K/HAL/CAN
    Let CAN port initialize io ports
    
    *****************  Version 27  *****************
    User: Dalem        Date: 10/04/04   Time: 8:50a
    Updated in $/PD/Network_Extensions/Code/Gateway/CAN/N2K/HAL/CAN
    Proper layout for dg256 with two CAN ports
    
    *****************  Version 24  *****************
    User: Peterd       Date: 3/05/04    Time: 4:08p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/UIEx/HAL/Src
    Updated port M initialization to only tweek bits for a specific CAN
    port, not all CAN ports
    
    *****************  Version 22  *****************
    User: Peterd       Date: 10/02/04   Time: 2:57p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/InvChgEx/HAL/Src
    Changed candrv_fnSetPriority to include memory of previous-previous
    frame priority and ID as the MSCAN module has three transmit buffers.
    
    *****************  Version 21  *****************
    User: Fredp        Date: 23/01/04   Time: 4:07p
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/Baseline/HCS12/Packages/HAL/Src
    Added code to show ISR timing
    
    *****************  Version 20  *****************
    User: Toml         Date: 10/31/03   Time: 5:54p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/UIEx/HAL/Src
    Reduced non-banked flash needs to bare minimum.
    
    *****************  Version 19  *****************
    User: Dalem        Date: 8/26/03    Time: 2:24p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/AGSEx/HAL/Src
    Problem with frames being lost, check TFLG and TIER bytes
    
    *****************  Version 18  *****************
    User: Dalem        Date: 8/01/03    Time: 2:13p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/AGSEx/HAL/Src
    Changes from review of code:
        CAN_MAX_CHANNELS 5 or 2
        constant name for can list and br params
        some loop termination optimizations
        prefixing of enums
    
    *****************  Version 16  *****************
    User: Alant        Date: 23/07/03   Time: 1:46p
    Updated in $/PD/Common/FieldReprogram/Loader/Test/XanBus/Motorola/HCS12
    - Renamed hwcfg.h to config.h
    
    *****************  Version 15  *****************
    User: Dalem        Date: 7/18/03    Time: 11:24a
    Updated in $/PD/Common/FieldReprogram/Loader/Test/XanBus/Motorola/HCS12
    Added package prefix and other naming changes before code review
    
    *****************  Version 12  *****************
    User: Toml         Date: 7/14/03    Time: 3:19p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/BattMonEx/HAL/Src
    Fixed multi-frame sequencing problem by adjusting transmit priority of
    frames.
    
    *****************  Version 10  *****************
    User: Toml         Date: 5/27/03    Time: 3:09p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DP256B/Test/UIEx/HAL/Src
    Moved some includes into header files that use them.
    Removed unused local variable.
    Corrected a const declaration.
    Used return enum types appropriate to the function.

    *****************  Version 8  *****************
    User: Dalem        Date: 3/06/03    Time: 1:22p
    Updated in $/PD/Common/Targets/Motorola/HCS12/HAL/Src
    Speed up byte transfers in receive and transmit

    *****************  Version 7  *****************
    User: Dalem        Date: 3/06/03    Time: 10:54a
    Updated in $/PD/Common/Targets/Motorola/HCS12/HAL/Src
    Receive interrupt cleared twice cause loss of messages

    *****************  Version 6  *****************
    User: Dalem        Date: 2/27/03    Time: 10:03a
    Updated in $/PD/Common/Targets/Motorola/HCS12/HAL/Src
    Added REROUTE_CAN_0 compile flag to use when connecting Axiom board to
    Can Isolated Interface board.

    *****************  Version 5  *****************
    User: Baldeeshk    Date: 26/02/03   Time: 8:49a
    Updated in $/PD/Common/Targets/Motorola/HCS12/HAL/Src
    Included #ifdef statements on declarations of CANlist to accomodate the
    DG128 target.

    *****************  Version 4  *****************
    User: Toml         Date: 2/21/03    Time: 3:38p
    Updated in $/PD/Common/Targets/Motorola/HCS12/HAL/Src
    Updated function headers

    *****************  Version 3  *****************
    User: Toml         Date: 2/20/03    Time: 8:55p
    Updated in $/PD/Common/Targets/Motorola/HCS12/HAL/Src
    Wakeup enable works
    Hangups waiting for INTAK and SLPAK prevented

    *****************  Version 2  *****************
    User: Toml         Date: 2/17/03    Time: 6:31p
    Updated in $/PD/Common/Targets/Motorola/HCS12/HAL/Src
    Updated for new <6812xxxxxB.h> header files

    *****************  Version 1  *****************
    User: Toml         Date: 2/14/03    Time: 5:35p
    Created in $/PD/Common/Targets/Motorola/HCS12/HAL/Src
    First basically working version.
    Wakeup due to traffic doesn't work yet.

=============================================================================*/

/*==============================================================================
                              Includes
==============================================================================*/

#include <string.h>         // for memcpy

#include "candrv.h"         // CANDRV interface declarations
#include "config.h"         // Hardware specifying

#ifdef _UCOSII
#include "includes.h"
#endif

//
// define CAN_TX_ISR_TASK_TIME_CHECK to show the CANbus TX ISR time
// define CAN_RX_ISR_TASK_TIME_CHECK to show the CANbus RX ISR time
// define CAN_ER_ISR_TASK_TIME_CHECK to show the CANbus ER ISR time
//
// #define CAN_TX_ISR_TASK_TIME_CHECK
// #define CAN_RX_ISR_TASK_TIME_CHECK
// #define CAN_ER_ISR_TASK_TIME_CHECK
//

#if (defined(CAN_TX_ISR_TASK_TIME_CHECK)  ||  \
     defined(CAN_RX_ISR_TASK_TIME_CHECK)  ||  \
     defined(CAN_ER_ISR_TASK_TIME_CHECK))
#include "debug.h"
#endif  //  CAN_TX_ISR_TASK_TIME_CHECK || CAN_RX_ISR_TASK_TIME_CHECK || CAN_ER_ISR_TASK_TIME_CHECK

/*=============================================================================
                                Defines
=============================================================================*/

#ifdef MC9S12DP256B

#define CAN_MAX_CHANNELS    5       // Number of CAN controllers on the chip

#define CANDRV_USING_CAN0   0       // Index into kazCAN_LIST array below
#define CANDRV_USING_CAN1   1       // Index into kazCAN_LIST array below
#define CANDRV_USING_CAN2   2       // Index into kazCAN_LIST array below
#define CANDRV_USING_CAN3   3       // Index into kazCAN_LIST array below
#define CANDRV_USING_CAN4   4       // Index into kazCAN_LIST array below

#else

#define CAN_MAX_CHANNELS    2       // Number of CAN controllers on the chip

#define CANDRV_USING_CAN0   0       // Index into kazCAN_LIST array below
#define CANDRV_USING_CAN4   1       // Index into kazCAN_LIST array below

#endif

#define CAN_MAX_FILTERS     2       // Number of CAN acceptance filters available on the chip

// Masks for testing and clearing interrupt flags
#define CANRFLG_WUPIF       0x80    // Wake-up Interrupt flag
#define CANRFLG_CSIF        0x40    // CAN Status Change Interrupt Flag
#define CANRFLG_OVRIF       0x02    // Overrun Interrupt flag
#define CANRFLG_RXF         0x01    // Receive buffer full interrupt flag

// Masks for the special bits in the MSCAN id field
#define CANBUF_IDR1_SRR     0x10    // SRR bit (1 for extended frame)
#define CANBUF_IDR1_IDE     0x08    // IDE bit (1 for extended frame)
#define CANBUF_IDR3_RTR     0x01    // Remote Request bit

// More useful bitmasks
#define CANCTL1_INITAK      0x01    // Initialization acknowledge
#define CANCTL1_SLPAK       0x02    // Sleep acknowledge

// Max tries while waiting for the CAN port to change state
#define CAN_MAX_TRIES       0x2FFF

// Macro for calculating value of CANBTR0 register
#define CANDRV_mBTR0( SJW, BRP )    ((( SJW - 1 ) << 6 ) + ( BRP - 1 ))

// Macro for calculating value of CANBTR1 register
#define CANDRV_mBTR1( SAMP, TSEG1, TSEG2 )  \
                         (( SAMP << 7 ) + (( TSEG2 - 1 ) << 4 ) + ( TSEG1 - 1 ))

/*=============================================================================
                               Data Types
=============================================================================*/

// Structure for the ID in MSCAN transmit and receive buffers and filters
typedef struct zMSCANID
{
    uchar8 IDR0;  // Four ID bytes
    uchar8 IDR1;
    uchar8 IDR2;
    uchar8 IDR3;
} tzMSCANID;

// Structure for frame buffers used for Transmit and receive
typedef struct
{
    tzMSCANID zID;  // Four ID bytes
    uchar8 DSR[ CANDRV_MAX_BYTES ];
    uchar8 DLR;    // Data Length register
    uchar8 PRIO;   // Transmit priority
    uint16 TSR;    // Time Stamp
} tzMSCANBUF;

// Structure for the whole MSCAN controller
typedef struct
{
    CAN0CTL0STR   CANCTL0;        // Control registers
    CAN0CTL1STR   CANCTL1;
    uchar8        CANBTR0;        // Timing registers
    uchar8        CANBTR1;
    CAN0RFLGSTR   CANRFLG;        // Receiver flag register
    CAN0RIERSTR   CANRIER;        // Receiver interrupt enable
    CAN0TFLGSTR   CANTFLG;        // Transmit flag register
    CAN0TIERSTR   CANTIER;        // Transmit interrupt enable
    CAN0TARQSTR   CANTARQ;        // Transmit abort request
    CAN0TAAKSTR   CANTAAK;        // Transmit abort acknowledge
    CAN0TBSELSTR  CANTBSEL;       // Transmit buffer select
    CAN0IDACSTR   CANIDAC;        // Filter control
    uchar8        reserved1;
    uchar8        reserved2;
    uchar8        CANRXERR;       // Receive error count
    uchar8        CANTXERR;       // Transmit error count
    tzMSCANID     zAcceptId0;     // Filter 0 acceptance id
    tzMSCANID     zMaskId0;       // Filter 0 acceptance mask
    tzMSCANID     zAcceptId1;     // Filter 1 acceptance id
    tzMSCANID     zMaskId1;       // Filter 1 acceptance mask
    tzMSCANBUF    zRxBuf;          // Receive buffer (windowed)
    tzMSCANBUF    zTxBuf;          // Transmit buffer (windowed)
} tzMSCAN;

// Structure for setting parameters according to a bit rate
typedef struct zCAN_BR_PARAMS
{
    CANDRV_teBIT_RATE   eBitRate;   // Supported bit rate
    uchar8              ucCANBTR0;  // Value for CANBTR0
    uchar8              ucCANBTR1;  // Value for CANBTR1
} tzCAN_BR_PARAMS;

/*==============================================================================
                           Local/Private Constants
==============================================================================*/

// The list of CAN controllers to choose from
tzMSCAN * const kazCAN_LIST[ CAN_MAX_CHANNELS ] =
{
    (tzMSCAN *)&_CAN0CTL0,
#ifdef MC9S12DP256B
    (tzMSCAN *)&_CAN1CTL0,
    (tzMSCAN *)&_CAN2CTL0,
    (tzMSCAN *)&_CAN3CTL0,
#endif
    (tzMSCAN *)&_CAN4CTL0
};


// Parameters for each supported bus speed
const tzCAN_BR_PARAMS kazCAN_BR_PARAMS[] =
{
    // Speed                        SJW BRP                 SAMP TSEG1 TSEG2
    { CANDRV_eBIT_RATE_20KBPS,      CANDRV_mBTR0( 3,  10 ), CANDRV_mBTR1( 0,   16,   3 ) },
    { CANDRV_eBIT_RATE_125KBPS,     CANDRV_mBTR0( 2,  2 ),  CANDRV_mBTR1( 0,   13,   2 ) },
    { CANDRV_eBIT_RATE_250KBPS,     CANDRV_mBTR0( 2,  1 ),  CANDRV_mBTR1( 0,   13,   2 ) },
};


/*==============================================================================
                           Local/Private Variables
==============================================================================*/

// The list of function pointers for interrupt callbacks
static CANDRV_tpfnCALLBACK
                    candrv_atpfnCallback[ CAN_MAX_CHANNELS ][ CANDRV_eCB_LAST ];

// The transmit buffer currently being serviced during ISR
static uchar8 candrv_aucHandle[ CAN_MAX_CHANNELS ];

/*=============================================================================
                         Forward Declarations
=============================================================================*/

static CANDRV_teRETURN candrv_fnFilterInit( tzMSCAN *pzCan,
                                            CANDRV_teFILTER_MODE FilterMode );
                                             
static void candrv_fnLoadHwId( tzMSCANID *pHwId,
                                         CANDRV_tzCAN_ID *pCanId );
                                         
static void candrv_fnUnloadHwId( CANDRV_tzCAN_ID *pCanId,
                                 tzMSCANID *pHwId );
                                 
static CANDRV_teSTATUS candrv_fnStatusEnum( uchar8 StatusBits );

static uchar8 candrv_fnMaskToHandle( uchar8 Mask );

static uchar8 candrv_fnHandleToMask( uchar8 Handle );

static CANDRV_teRETURN candrv_fnGoSleep( tzMSCAN *pzCan );

static CANDRV_teRETURN candrv_fnWaitBit( volatile uchar8 *pucIoReg,
                                         uchar8 Mask,
                                         tucBOOL State );
                                            
static uchar8 candrv_fnSetPriority( CANDRV_tzCAN_ID *pzCanId );

static void candrv_fnTxIsr( uchar8 Channel );
static void candrv_fnRxIsr( uchar8 Channel );
static void candrv_fnErrIsr( uchar8 Channel );
static void candrv_fnWakeIsr( uchar8 Channel );

/*==============================================================================
                           Function Definitions
==============================================================================*/

/*******************************************************************************

FUNCTION NAME:
    candrv_fnFilterInit

PURPOSE:
    Initialize the CAN driver's filters

INPUTS:
    pzCan points to the MSCAN module being initialized
    FilterMode indicates if the filter is to be set to pass everything or nothing

OUTPUTS:
    CANDRV_eSUCCESS if all goes well

NOTES:
    This function is only to be called from CAN_fnInit

VERSION HISTORY:

Version: 1.00  Date: 02/12/03  By: Tom Lightfoot
    - Creation

*******************************************************************************/

static CANDRV_teRETURN candrv_fnFilterInit( tzMSCAN *pzCan,
                                            CANDRV_teFILTER_MODE FilterMode )
{
    uchar8 Mask;    // Acceptance mask for the filters, either all ones
                    // for accepting anything, or all zeros

    // Set identifier acceptance mode to two blocks of 32 bits
    pzCan->CANIDAC.Bits.IDAM1 = 0;
    pzCan->CANIDAC.Bits.IDAM0 = 0;

    if( FilterMode == CANDRV_eFILTER_PASS_ALL )
    {
        // Set all mask bytes to 0xFF to allow all frames to be received
        // (A 1 in the mask means don't care)
        Mask = 0xff;
    }
    else if( FilterMode == CANDRV_eFILTER_PASS_NONE )
    {
        // Set all mask bytes to 0x00 to not pass any frames
        Mask = 0x00;
    }
    else
    {
        // Invalid filter parameter
        return( CANDRV_eRET_BAD_FILTER );
    }

    // Set all acceptance registers to zero
    pzCan->zAcceptId0.IDR0 = 0x00;
    pzCan->zAcceptId0.IDR1 = 0x00;
    pzCan->zAcceptId0.IDR2 = 0x00;
    pzCan->zAcceptId0.IDR3 = 0x00;

    pzCan->zAcceptId1.IDR0 = 0x00;
    pzCan->zAcceptId1.IDR1 = 0x00;
    pzCan->zAcceptId1.IDR2 = 0x00;
    pzCan->zAcceptId1.IDR3 = 0x00;

    // Set all of the acceptance masks according to the Filter Mode
    pzCan->zMaskId0.IDR0 = Mask;
    pzCan->zMaskId0.IDR1 = Mask;
    pzCan->zMaskId0.IDR2 = Mask;
    pzCan->zMaskId0.IDR3 = Mask;

    pzCan->zMaskId1.IDR0 = Mask;
    pzCan->zMaskId1.IDR1 = Mask;
    pzCan->zMaskId1.IDR2 = Mask;
    pzCan->zMaskId1.IDR3 = Mask;

    return( CANDRV_eRET_SUCCESS );
}

/*******************************************************************************

FUNCTION NAME:
    CANDRV_fnInit

PURPOSE:
    Initialize the CAN module for a given channel

INPUTS:
    Channel is the channel number, usually zero
    BitRate is an enum of the bit rates supported
    FilterMode indicates if the filter is to be set to pass everything or nothing

OUTPUTS:
    CANDRV_eSUCCESS if all goes well

NOTES:


VERSION HISTORY:

Version: 1.00  Date: 02/12/03  By: Tom Lightfoot
    - Creation
Version: 1.01  Date: 07/31/03  By: dalem
    - Removed conditional rerouting of CAN port 0
    - Changed search termination of BR parameters
Version: 1.10  Date: 01/20/04  By: Fred Pang
    - Added task timing code to show interrupts timing
Version: 1.11  Date: 06/07/04  By: dalem
    - Added Port J for CAN 4 and added conditional so second channel is CAN 4
Version: 1.11  Date: 10/05/04  By: dalem
    - At startup there were CAN error frames produces, removed code
     initializing ports and let CAN hardware set them up.

*******************************************************************************/

CANDRV_teRETURN CANDRV_fnInit( uchar8 Channel,
                               CANDRV_teBIT_RATE BitRate,
                               CANDRV_teFILTER_MODE FilterMode )
{
    tzMSCAN *pzCan;     // Points to the controller being initialized
    uchar8 Index;       // Index for counting

    // Validate channel number and map to port number
    if( Channel >= CAN_MAX_CHANNELS )
    {
        //  Index out of range
        return( CANDRV_eRET_BAD_CHANNEL );
    }
    
    // Initialize data ports for CAN
    // --- Port M, which holds most of the CAN ports ---
    //
    // Port M controls 4 CAN ports, CAN 0 to 3
    // Bit 7   Bit 6   Bit 5   Bit 4   Bit 3   Bit 2   Bit 1   Bit 0
    // CANTx3  CANRx3  CANTx2  CANRx2  CANTx1  CANRx1  CANTx0  CANRx0
    //
    // Port J controls 1 CAN port, CAN 4
    // Bit 7   Bit 6   Bit 5   Bit 4   Bit 3   Bit 2   Bit 1   Bit 0
    // CANTx4  CANRx4

    // --- Not required --- CAN hardware initializes the ports internally
    
    // Need to change routing for CAN 4 to use CAN 2 output pins
    // MODRR_MODRR0 = 0;   // Module Routing, CAN 0 Rx to Pin PM0
    // MODRR_MODRR1 = 0;   // Module Routing, CAN 0 Tx to Pin PM1
    MODRR_MODRR2 = 1;   // Module Routing, CAN 4 Rx to Pin PM4
    MODRR_MODRR3 = 0;   // Module Routing, CAN 4 Tx to Pin PM5

    // Get a pointer to the right controller
    pzCan = kazCAN_LIST[ Channel ];

    // Make sure the controller is enabled before we do anything to it
    pzCan->CANCTL1.Bits.CANE = 1;

    // If the controller isn't already in INIT state then put it
    // to sleep first so that no frames in progress don't get cut
    if( !pzCan->CANCTL1.Bits.INITAK )
    {
        // Disable all CAN interrupts
        pzCan->CANRIER.Byte = 0x00;
        pzCan->CANTIER.Byte = 0x00;

        // Go to sleep mode
        if( candrv_fnGoSleep( pzCan ) != CANDRV_eRET_SUCCESS )
        {
            return( CANDRV_eRET_SLEEP_FAIL );
        }

        // Now it's safe to go into initialization mode
        pzCan->CANCTL0.Bits.INITRQ = 1;

        // And get out of sleep mode
        pzCan->CANCTL0.Bits.SLPRQ = 0;

        // Wait to get into initialization mode
        if( candrv_fnWaitBit( &pzCan->CANCTL1.Byte,
                              CANCTL1_INITAK,
                              TRUE ) != CANDRV_eRET_SUCCESS )
        {
            return( CANDRV_eRET_INIT_FAIL );
        }
    }

    // --- Miscellaneous initializations ---

    pzCan->CANCTL1.Bits.LOOPB = 0;   // Use loopback for initial debugging
    pzCan->CANCTL1.Bits.LISTEN = 0;  // Able to send and acknowledge frames
    pzCan->CANCTL1.Bits.WUPM = 0;    // Don't use filtering on wakeup; just one
                                     // recessive to dominant edge.

    // --- Set the bit timing ---
    // Search for data entry for given bit rate
    for( Index = 0; Index < XT_mDIM( kazCAN_BR_PARAMS ); Index++ )
    {
        // Are we at the right entry in the parameter table?
        if( BitRate == kazCAN_BR_PARAMS[ Index ].eBitRate )
        {
            // Set the bit rate registers according to this entry of the table
            pzCan->CANBTR0 = kazCAN_BR_PARAMS[ Index ].ucCANBTR0;
            pzCan->CANBTR1 = kazCAN_BR_PARAMS[ Index ].ucCANBTR1;

            // We're done with this loop
            break;
        }
    }

    // Are we at the end of the table?
    if ( Index >= XT_mDIM( kazCAN_BR_PARAMS ) )
    {
        // We're at the end of the parameter list -> bit rate not supported
        return( CANDRV_eRET_BAD_BIT_RATE );
    }

    // Set the clock source to the crystal oscillator
    pzCan->CANCTL1.Bits.CLKSRC = 0;
    

#if (defined(CAN_TX_ISR_TASK_TIME_CHECK)  ||  \
     defined(CAN_RX_ISR_TASK_TIME_CHECK)  ||  \
     defined(CAN_ER_ISR_TASK_TIME_CHECK))
    // initialize output port pins
    DEBUG_mINIT_ALLPINS();
#endif  //  CAN_TX_ISR_TASK_TIME_CHECK || CAN_RX_ISR_TASK_TIME_CHECK || CAN_ER_ISR_TASK_TIME_CHECK
    

    // Set the acceptance filtering
    return( candrv_fnFilterInit( pzCan, FilterMode ) );
}

/*******************************************************************************

FUNCTION NAME:
    candrv_fnLoadHwId

PURPOSE:
    Copy the driver id into the MSCAN id.  A driver ID pointed to by pCANID
    gets copied to an ID field in the MSCAN driver.  The bit ordering of the
    two structures is different so some conversion is required.

INPUTS:
    pHwId points to an ID field in the MSCAN module
    pCanId points to an id field in the format the driver uses

OUTPUTS:
    Nothing

NOTES:
    This function is used both for sending frames as well as setting
    hardware filtering parameters.

    MSCAN Registers
         +---------------------------------------------------------------+
    IDR0 | ID 28 | ID 27 | ID 26 | ID 25 | ID 24 | ID 23 | ID 22 | ID 21 |
         +---------------------------------------------------------------+
    IDR1 | ID 20 | ID 19 | ID 18 |  SRR  |  IDE  | ID 17 | ID 16 | ID 15 |
         +---------------------------------------------------------------+
    IDR2 | ID 14 | ID 13 | ID 12 | ID 11 | ID 10 | ID 9  | ID 8  | ID 7  |
         +---------------------------------------------------------------+
    IDR3 | ID 6  | ID 5  | ID 4  | ID 3  | ID 2  | ID 1  | ID 0  |  RTR  |
         +---------------------------------------------------------------+

VERSION HISTORY:

Version: 1.00  Date: 02/12/03  By: Tom Lightfoot
    - Creation
Version: 1.01  Date: 01/28/05  By: Dalem
    - Can Id structure was changed

*******************************************************************************/

static void candrv_fnLoadHwId( tzMSCANID *pHwId,
                               CANDRV_tzCAN_ID *pCanId )
{
    // Id bits 28 to 24 go into IDR0 as top 5 bits
    pHwId->IDR0 = ( pCanId->ucId28_24 << 3 );
    // Id Bits 23 to 21 go into IDR0 as bottom 3 bits
    pHwId->IDR0 |= ( pCanId->ucId23_16 >> 5 );

    // Id bits 20 to 18 go into IDR1 as top three bits
    pHwId->IDR1 = ( pCanId->ucId23_16 << 3 ) & 0xE0;
    // Id bits 17 and 16 go into IDR1 as bits 2 and 1
    pHwId->IDR1 |= ( pCanId->ucId23_16 << 1 ) & 0x06;
    // Id bit 15 goes into IDR2 as bottom bit
    pHwId->IDR1 |= ( pCanId->ucId15_8 & 0x80 ) ? 0x01 : 0x00;
    // SRR and IDE bits go into IDR1 as bits 4 and 3
    pHwId->IDR1 |= (pCanId->ucId28_24 & CANDRV_ID_MSK_SRR ) ? 0x10 : 0;
    pHwId->IDR1 |= (pCanId->ucId28_24 & CANDRV_ID_MSK_IDE ) ? 0x08 : 0;

    // Id bits 14 to 8 go into IDR2 as top 7 bits
    pHwId->IDR2 = ( pCanId->ucId15_8 << 1 );
    // Id bit 7 goes into IDR2 as bottom bit
    pHwId->IDR2 |= ( pCanId->ucId07_00 & 0x80 ) ? 0x01 : 0x00;

    // Id bits 6 to zero go into IDR3
    pHwId->IDR3 = ( pCanId->ucId07_00 << 1 );
    // RTR bit goes into IDR3 as bottom bit
    pHwId->IDR3 |= (pCanId->ucId28_24 & CANDRV_ID_MSK_RTR ) ? 0x01 : 0x00;
}

/*******************************************************************************

FUNCTION NAME:
    candrv_fnUnloadHwId

PURPOSE:
    Copy a MSCAN format frame ID into a driver format frame ID.
    The bit ordering of the two structures is different so some
    conversion is required.

INPUTS:
    pCanId points to an id field in the format the driver uses
    pHwId points to an ID field in the MSCAN module

OUTPUTS:
    Nothing

NOTES:
    This function is used for receiving frames.

    MSCAN Registers
         +---------------------------------------------------------------+
    IDR0 | ID 28 | ID 27 | ID 26 | ID 25 | ID 24 | ID 23 | ID 22 | ID 21 |
         +---------------------------------------------------------------+
    IDR1 | ID 20 | ID 19 | ID 18 |  SRR  |  IDE  | ID 17 | ID 16 | ID 15 |
         +---------------------------------------------------------------+
    IDR2 | ID 14 | ID 13 | ID 12 | ID 11 | ID 10 | ID 9  | ID 8  | ID 7  |
         +---------------------------------------------------------------+
    IDR3 | ID 6  | ID 5  | ID 4  | ID 3  | ID 2  | ID 1  | ID 0  |  RTR  |
         +---------------------------------------------------------------+

VERSION HISTORY:

Version: 1.00  Date: 02/12/03  By: Tom Lightfoot
    - Creation
Version: 1.01  Date: 01/28/05  By: Dalem
    - Can Id structure was changed

*******************************************************************************/

static void candrv_fnUnloadHwId( CANDRV_tzCAN_ID *pCanId,
                                 tzMSCANID *pHwId )
{
    // Id bits 28 to 24 comes from top 5 bits of IDR0
    pCanId->ucId28_24 = pHwId->IDR0 >> 3;
    // SRR, IDE and RTR bits store with id bits 28 to 24
    if ( pHwId->IDR1 & 0x10 )
        pCanId->ucId28_24 |= CANDRV_ID_MSK_SRR;
    if ( pHwId->IDR1 & 0x08 )
        pCanId->ucId28_24 |= CANDRV_ID_MSK_IDE;
    if ( pHwId->IDR3 & 0x01 )
        pCanId->ucId28_24 |= CANDRV_ID_MSK_RTR;

    // Id Bit 23 to 21 come from bottom 3 bits of IDR0
    pCanId->ucId23_16 = ( pHwId->IDR0 << 5 );
    // Id bits 20 to 18 come from top 3 bits of IDR1
    pCanId->ucId23_16 |= ( pHwId->IDR1 >> 3 ) & 0x1C;
    // Id bits 17 and 16 come from bits 2 and 1 of IDR1
    pCanId->ucId23_16 |= ( pHwId->IDR1 >> 1 ) & 0x03;

    // Id bits 14 to 8 come from top 7 bits of IDR2
    pCanId->ucId15_8 = pHwId->IDR2 >> 1;
    // Id bit 15 comes from bottom bit of IDR1
    if ( pHwId->IDR1 & 0x01 )
        pCanId->ucId15_8 |= 0x80;

    // Id bits 6 to 0 come from top 7 bits of IDR3
    pCanId->ucId07_00 = pHwId->IDR3 >> 1;
    // Id bit 7 comes from bottom bit of IDR2
    if ( pHwId->IDR2 & 0x01 )
        pCanId->ucId07_00 |= 0x80;
}

/*******************************************************************************

FUNCTION NAME:
    CANDRV_fnSetFilter

PURPOSE:
    Install a filter in hardware.

INPUTS:
    Channel is the module to use, usually zero
    FilterID is the filter to use.  Two are available on the MSCAN module
    pCanId points to the desired ID to match
    pMask indicates which bits of *pCanId to match.  1 means don't care.

OUTPUTS:
    CANDRV_eSUCCESS if all goes well

NOTES:
    This function can only be called when the MSCAN module is in initialization
    mode.  Filters cannot be set 'on the fly'.

VERSION HISTORY:

Version: 1.00  Date: 02/12/03  By: Tom Lightfoot
    - Creation

*******************************************************************************/

CANDRV_teRETURN CANDRV_fnSetFilter( uchar8 Channel,
                                    uchar8 FilterID,
                                    CANDRV_tzCAN_ID *pCanId,
                                    CANDRV_tzCAN_ID *pMask )
{
    tzMSCAN *pzCan;       // CAN module being used
    tzMSCANID *pzHwId;    // Acceptance ID on hardware
    tzMSCANID *pzHwMask;  // Acceptance mask on hardware

    // Get a pointer to the right controller
    if( Channel >= CAN_MAX_CHANNELS )
    {
        //  Index out of range
        return( CANDRV_eRET_BAD_CHANNEL );
    }

    pzCan = kazCAN_LIST[ Channel ];

    // The module must be in init mode in order for a new filter to be applied
    if( !pzCan->CANCTL0.Bits.INITRQ || !pzCan->CANCTL1.Bits.INITAK )
    {
        return( CANDRV_eRET_FAILURE );
    }

    // Validate the filter id
    switch ( FilterID )
    {
    case 0:
        // Use the first block of filter registers
        pzHwId = &pzCan->zAcceptId0;
        pzHwMask = &pzCan->zMaskId0;
        break;

    case 1:
        // Use the second block of filter registers
        pzHwId = &pzCan->zAcceptId1;
        pzHwMask = &pzCan->zMaskId1;
        break;

    default:
        return( CANDRV_eRET_BAD_FILTER );
    }

    // Load the acceptance register structure
    candrv_fnLoadHwId( pzHwId, pCanId );

    // Load the mask register structure
    candrv_fnLoadHwId( pzHwMask, pMask );

    return( CANDRV_eRET_SUCCESS );
}

/*******************************************************************************

FUNCTION NAME:
    CANDRV_fnEnable

PURPOSE:
    This function enables the specified CAN channel and brings it out of
    init mode.

INPUTS:
    Channel is the number of the CAN module to enable, usually zero.

OUTPUTS:
    CANDRV_eRET_SUCCESS if all goes well

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/12/03  By: Tom Lightfoot
    - Creation

*******************************************************************************/

CANDRV_teRETURN CANDRV_fnEnable( uchar8 Channel )
{
    tzMSCAN *pzCan;             // Points to the controller being initialized

    // Get a pointer to the right controller
    if( Channel >= CAN_MAX_CHANNELS )
    {
        //  Index out of range
        return( CANDRV_eRET_BAD_CHANNEL );
    }

    pzCan = kazCAN_LIST[ Channel ];

    // Make sure that the channel is enabled
    pzCan->CANCTL1.Bits.CANE = 1;

    // Request to come out of init mode
    pzCan->CANCTL0.Bits.INITRQ = 0;

    // Wait for the controller to come out of initialization mode
    if( candrv_fnWaitBit( &pzCan->CANCTL1.Byte,
                          CANCTL1_INITAK,
                          FALSE ) != CANDRV_eRET_SUCCESS )
    {
        return( CANDRV_eRET_INIT_FAIL );
    }

    // Enable all interrupts except for transmit
    // since there is nothing to send yet.
    pzCan->CANRIER.Bits.OVRIE = 1;    // Receiver Overrun interrupt
    pzCan->CANRIER.Bits.RXFIE = 1;    // Receiver interrupt
    pzCan->CANRIER.Bits.CSCIE = 1;    // CAN Status Change interrupt
    pzCan->CANRIER.Bits.RSTATE1 = 1;  // Interrupt on any receive status change
    pzCan->CANRIER.Bits.RSTATE0 = 1;
    pzCan->CANRIER.Bits.TSTATE1 = 1;  // Interrupt on any transmit status change
    pzCan->CANRIER.Bits.TSTATE0 = 1;

    // Indicate successful initialization
    return( CANDRV_eRET_SUCCESS );
}

/*******************************************************************************

FUNCTION NAME:
    CANDRV_fnDisable

PURPOSE:
    This function disables the indicated CAN channel.  It attempts to do so
    safely by by first putting the channel into sleep mode so that any
    frames that are being transmitted get a chance to finish.  Then the
    module is put into init mode so that it is possible to change
    filters.

INPUTS:
    Channel is the number of the CAN module to disable, usually zero.

OUTPUTS:
    CANDRV_eRET_SUCCESS if all goes well

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/12/03  By: Tom Lightfoot
    - Creation

*******************************************************************************/

CANDRV_teRETURN CANDRV_fnDisable( uchar8 Channel )
{
    tzMSCAN *pzCan;  // CAN module being used

    // Get a pointer to the right controller
    if( Channel >= CAN_MAX_CHANNELS )
    {
        //  Index out of range
        return( CANDRV_eRET_BAD_CHANNEL );
    }

    pzCan = kazCAN_LIST[ Channel ];

    // If we're not already in initialization state then we'll have to get there
    if( !pzCan->CANCTL1.Bits.INITAK )
    {
        // Disable all CAN interrupts
        pzCan->CANRIER.Byte = 0x00;
        pzCan->CANTIER.Byte = 0x00;

        // Get into sleep mode so as to not chop off any frames in progress
        if( candrv_fnGoSleep( pzCan ) != CANDRV_eRET_SUCCESS )
        {
            return( CANDRV_eRET_SLEEP_FAIL );
        }

        // Now it's safe to go into initialization mode
        pzCan->CANCTL0.Bits.INITRQ = 1;
        pzCan->CANCTL0.Bits.SLPRQ = 0;

        // Wait to get into initialization mode
        if( candrv_fnWaitBit( &pzCan->CANCTL1.Byte,
                              CANCTL1_INITAK,
                              TRUE ) != CANDRV_eRET_SUCCESS )
        {
            return( CANDRV_eRET_INIT_FAIL );
        }
    }

    // Disable the controller
    pzCan->CANCTL1.Bits.CANE = 0;

    return( CANDRV_eRET_SUCCESS );
}

/*******************************************************************************

FUNCTION NAME:
    CANDRV_fnSleep

PURPOSE:
    Put CAN channel to sleep so that the CPU can use less power.

INPUTS:
    Channel is the number of the CAN module to put to sleep, usually zero.
    WakeEnable is TRUE if the module is to automatically wake up if there
        is traffic on the CAN bus.

OUTPUTS:
    CANDRV_eRET_SUCCESS if all goes well

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/12/03  By: Tom Lightfoot
    - Creation

*******************************************************************************/

CANDRV_teRETURN CANDRV_fnSleep( uchar8 Channel,
                                uchar8 WakeEnable )
{
    tzMSCAN *pzCan;  // CAN module being used

    // Get a pointer to the right controller
    if( Channel >= CAN_MAX_CHANNELS )
    {
        //  Index out of range
        return( CANDRV_eRET_BAD_CHANNEL );
    }

    pzCan = kazCAN_LIST[ Channel ];

    // Enable wakeup if necessary
    if( WakeEnable )
    {
        // Enable wakeup on bus traffic
        pzCan->CANCTL0.Bits.WUPE = 1;

        // Enable the wakeup interrupt
        pzCan->CANRIER.Bits.WUPIE = 1;
    }
    else
    {
        // Disable wakeup and the interrupt
        pzCan->CANCTL0.Bits.WUPE = 0;
        pzCan->CANRIER.Bits.WUPIE = 0;
    }

    // Then we wait until all frames queued for transmission
    // are sent
    return( candrv_fnGoSleep( pzCan ) );
}

/*******************************************************************************

FUNCTION NAME:
     candrv_fnGoSleep

PURPOSE:
    Tell CAN module to go to sleep and wait until it is successful

INPUTS:
    pzCan points to the CAN module being put to sleep

OUTPUTS:
    CANDRV_eRET_SUCCESS if all goes well
    CANDRV_eRET_FAILURE if the module could not be put into sleep mode

NOTES:
    The MSCAN module cannot go to sleep if frames are being received or
    transmitted.  The function will wait a finite amount of time for frames
    in the transmit buffer to make it on the bus by their own accord.
    If this fails because the bus is offline then the frames will be aborted
    and the function will wait again.  If the SLPAK bit still fails to
    change state then this function will give up and return CANDRV_eRET_FAILURE

VERSION HISTORY:

Version: 1.00  Date: 02/20/03  By: Tom Lightfoot
    - Creation

*******************************************************************************/

static CANDRV_teRETURN candrv_fnGoSleep( tzMSCAN *pzCan )
{
    // Sleep mode has to be requested
    pzCan->CANCTL0.Bits.SLPRQ = 1;

    // Wait for the module to go to sleep
    if( candrv_fnWaitBit( &pzCan->CANCTL1.Byte,
                          CANCTL1_SLPAK,
                          TRUE ) == CANDRV_eRET_SUCCESS )
    {
        return( CANDRV_eRET_SUCCESS );
    }

    // If we get this far then there is a problem.
    // If there are frames in the transmit buffer that can't get out because
    // the bus is down then the module can't go to sleep.
    // Try aborting those and wait again

    // Buffers queued for transmission are the ones where the TXE flags are clear
    pzCan->CANTARQ.Byte = ~pzCan->CANTFLG.Byte;

    // Last chance!
    return( candrv_fnWaitBit( &pzCan->CANCTL1.Byte,
                              CANCTL1_SLPAK,
                              TRUE ) );
}


/*******************************************************************************

FUNCTION NAME:
    candrv_fnWaitBit

PURPOSE:
    Wait for a volatile bit to change state.

INPUTS:
    pIOReg points to the I/O register with the bit we're waiting for
    Mask indicates which bit of *pIOReg to wait on
    State indicates the desired state of the bit

OUTPUTS:
    CANDRV_eRET_SUCCESS if all goes well
    CANDRV_eRET_FAILURE if the bit did not change to the desired state

NOTES:
    This function is used for verifying that the MSCAN module goes
    into or out of sleep or initialization mode.

VERSION HISTORY:

Version: 1.00  Date: 02/20/03  By: Tom Lightfoot
    - Creation
Version: 1.01  Date: 07/31/03  By: Dalem
    - Logic in wait loop changed to make code smaller

*******************************************************************************/

static CANDRV_teRETURN candrv_fnWaitBit( volatile uchar8 *pIoReg,
                                         uchar8 Mask,
                                         tucBOOL State )
{
    uint16 TryCount;  // Counter for number of times SLPAK bit has been checked

    // Wait a finite period of time for the bit to go to the desired state
    for( TryCount = 0; TryCount < CAN_MAX_TRIES; TryCount++ )
    {
        // We are waiting for a bit in the io register to be in the right state
        // If the logical value of the masked bit matches the requested state 
        if( !!( *pIoReg & Mask ) == ( !!State ) )
        {
            // We're done
            return( CANDRV_eRET_SUCCESS );
        }
    }

    // If we get this far then the bit didn't change state in time
    return( CANDRV_eRET_FAILURE );
}


/*******************************************************************************

FUNCTION NAME:
    CANDRV_fnWakeUp

PURPOSE:
    Wake channel up from sleep

INPUTS:
    Channel is the number of the CAN module to wake up, usually zero.

OUTPUTS:
    CANDRV_eRET_SUCCESS if all goes well

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/12/03  By: Tom Lightfoot
    - Creation

*******************************************************************************/

CANDRV_teRETURN CANDRV_fnWakeUp( uchar8 Channel )
{
    tzMSCAN *pzCan;  // CAN module being used

    // Get a pointer to the right controller
    if( Channel >= CAN_MAX_CHANNELS )
    {
        //  Index out of range
        return( CANDRV_eRET_BAD_CHANNEL );
    }

    pzCan = kazCAN_LIST[ Channel ];

    // Cancel sleep mode
    pzCan->CANCTL0.Bits.SLPRQ = 0;

    // No need to enable wakeup on bus activity
    pzCan->CANCTL0.Bits.WUPE = 0;

    return( CANDRV_eRET_SUCCESS );
}


/*******************************************************************************

FUNCTION NAME:
    CANDRV_fnReceive

PURPOSE:
    Check if there is a new frame in the MSCAN module and if there is, copy
    it into the driver buffer pointed to by pRxFrame

INPUTS:
    Channel is the number of the CAN module to use, usually zero.
    pRxFrame points to the destination frame structure

OUTPUTS:
    CANDRV_eRET_SUCCESS if all goes well
    CANDRV_eRET_NO_DATA if no frame has been received

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/12/03  By: Tom Lightfoot
    - Creation
Version: 1.01  Date: 03/06/03  By: Dale Mernett
    - Do not clear interrupt here. It is done can_fnRxISR.

*******************************************************************************/

CANDRV_teRETURN CANDRV_fnReceive( uchar8 Channel,
                                      CANDRV_tzCAN_FRAME *pRxFrame )
{
    tzMSCAN *pzCan;     // CAN module being used

    // Get a pointer to the right controller
    if( Channel >= CAN_MAX_CHANNELS )
    {
        //  Index out of range
        return( CANDRV_eRET_BAD_CHANNEL );
    }

    pzCan = kazCAN_LIST[ Channel ];

    // See if there is a valid frame in the input buffer
    if( !pzCan->CANRFLG.Bits.RXF )
    {
        // No valid data in the buffer
        return( CANDRV_eRET_NO_DATA );
    }

    // Copy the ID part of the frame
    candrv_fnUnloadHwId( &pRxFrame->tzCanId,
                         &pzCan->zRxBuf.zID );

    // Copy the data bytes of the frame
    (void) memcpy( pRxFrame->aucData,
                   pzCan->zRxBuf.DSR,
                   CANDRV_MAX_BYTES );

    // Copy the byte count (only the last four bits count)
    pRxFrame->ucByteCount = pzCan->zRxBuf.DLR & 0x0f;

    return( CANDRV_eRET_SUCCESS );
}

/*******************************************************************************

FUNCTION NAME:
    CANDRV_fnTransmitOK

PURPOSE:
    See if at least one transmit buffer is empty and ready for use.
    This function is used to check the availability of the hardware before
    removing a buffer from a queue.

INPUTS:
    Channel is the number of the CAN module to use, usually zero.

OUTPUTS:
    CANDRV_eRET_SUCCESS if at least one transmit buffer is idle
    CANDRV_eRET_TX_BUSY if no transmit buffers are available

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/13/03  By: Tom Lightfoot
    - Creation
Version: 1.01  Date: 08/26/03  By: Dalem
    - Check TFLG and TIER bytes as in CANDRV_fnTransmit

*******************************************************************************/

CANDRV_teRETURN CANDRV_fnTransmitOk( uchar8 Channel )
{
    tzMSCAN *pzCan;          // CAN module being used

    // Get a pointer to the right controller
    if( Channel >= CAN_MAX_CHANNELS )
    {
        //  Index out of range
        return( CANDRV_eRET_BAD_CHANNEL );
    }

    pzCan = kazCAN_LIST[ Channel ];

    // See if any of the transmit buffer empty bits are set
    if(  pzCan->CANTFLG.Byte & ~pzCan->CANTIER.Byte )
    {
        // At least one buffer is empty
        return( CANDRV_eRET_SUCCESS );
    }
    else
    {
        // All buffers are full, awaiting transmission
        return( CANDRV_eRET_TX_BUSY );
    }
}

/*******************************************************************************

FUNCTION NAME:
    CANDRV_fnTransmit

PURPOSE:
    Transmit CAN frame to bus.

INPUTS:
    Channel is the number of the CAN module to use, usually zero.
    pTxFrame points to the driver frame to be transmitted
    pHandle points to where the user wants the 'Handle' of the transmitted
        frame.  The handle is a bitmask that indicates which transmit buffer
        was used.  If the frame doesn't make it onto the bus on time, it
        may be necessary to abort it.  This handle bitmask will be used
        to abort the frame.

OUTPUTS:
    CANDRV_eRET_SUCCESS if all goes well
    CANDRV_eRET_TX_BUSY if no transmit buffers are available

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/12/03  By: Tom Lightfoot
    - Creation
Version: 1.01  Date: 02/20/03  By: Tom Lightfoot
    - Handle is now a number from 0 to 2 instead of a bitmask

*******************************************************************************/

CANDRV_teRETURN CANDRV_fnTransmit( uchar8 Channel,
                                   CANDRV_tzCAN_FRAME *pTxFrame,
                                   uchar8 *pHandle )
{
    tzMSCAN *pzCan;          // CAN module being used
    volatile uchar8 ucFlags; // Bit mask of which transmit buffer is used

    // Get a pointer to the right controller
    if( Channel >= CAN_MAX_CHANNELS )
    {
        //  Index out of range
        return( CANDRV_eRET_BAD_CHANNEL );
    }

    pzCan = kazCAN_LIST[ Channel ];

    // Check the transmit flags for an empty buffer
    ucFlags = pzCan->CANTFLG.Byte & ~pzCan->CANTIER.Byte;
    if( !ucFlags )
    {
        // No buffers are available
        return( CANDRV_eRET_TX_BUSY );
    }

    // Select the first free buffer
    if( ucFlags & 0x01 )
    {
        ucFlags = 0x01;
    }
    else if( ucFlags &0x02 )
    {
        ucFlags = 0x02;
    }
    else
    {
        ucFlags = 0x04;
    }

    pzCan->CANTBSEL.Byte = ucFlags;

    // The register will indicate which buffer is actually chosen,
    // which would be the one with the lowest order bit.

    // Copy the ID bytes
    candrv_fnLoadHwId( &pzCan->zTxBuf.zID, &pTxFrame->tzCanId );

    // Copy the data bytes of the frame
    (void) memcpy( pzCan->zTxBuf.DSR,
                   pTxFrame->aucData,
                   CANDRV_MAX_BYTES );

    // Copy the number of relevant data bytes
    pzCan->zTxBuf.DLR = pTxFrame->ucByteCount;

    // Set the transmit priority for this frame
    pzCan->zTxBuf.PRIO = candrv_fnSetPriority( &pTxFrame->tzCanId );

    // Send the buffer to the CAN bus
    pzCan->CANTFLG.Byte = ucFlags;

    // Save the handle for aborting this frame later on if necessary
    if( pHandle )
    {
        *pHandle = candrv_fnMaskToHandle( ucFlags );
    }

    // Enable interrupts for this buffer
    pzCan->CANTIER.Byte |= ucFlags;

    return( CANDRV_eRET_SUCCESS );
}

/*******************************************************************************

FUNCTION NAME:
    candrv_fnSetPriority

PURPOSE:
    Set the transmit priority of the next frame to transmit

INPUTS:
    'pzCanId' is a pointer to the transmit frame's ID

OUTPUTS:
    The internal transmit priority of the next frame

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 07/14/03  By: Tom Lightfoot
    - Creation
Version: 1.01  Date: 02/10/04  By: P.Drexel
    - Added concept of previous-previous priority and ID as there are
    three (3) transmit buffers in the Motorola MSCAN unit
Version: 1.02  Date: 01/28/05  By: Dalem
    - Can Id structure was changed

*******************************************************************************/

static uchar8 candrv_fnSetPriority( CANDRV_tzCAN_ID *pzCanId )
{
    static uint32 ulPrevId = 0;     // Previous frame's ID
    static uchar8 ucPrevPrio = 0;   // Previous frame's priority
    static uint32 ulPrevPrevId = 0;     // Previous, previous frame's ID
    static uchar8 ucPrevPrevPrio = 0;   // Previous, previous frame's priority
    uint32 ulCurId;                 // Current frame's ID

    // Is the ID of this frame the same as the previous frame?
    // If it is then this is likely a multi-frame message who's
    // sequence must be preserved.
    ulCurId = *( (uint32 *)pzCanId );
    
    if( ulCurId == ulPrevId )
    {
        // Increment the previous priority number to give
        // this frame a lower priority
        ucPrevPrio++;
        
        return( ucPrevPrio );
    }
    else if ( ulCurId == ulPrevPrevId )
    {
        ucPrevPrevPrio++;
        
        return( ucPrevPrevPrio );
    }
    else
    {
        ucPrevPrevPrio = ucPrevPrio;
        
        ulPrevPrevId = ulPrevId;
        
        // This is a new message so use the nominal priority from the top
        // three bits of the id
        ucPrevPrio = ( pzCanId->ucId28_24 >> 2 ) & 0x07;

        // We're now on a new ID
        ulPrevId = ulCurId;

        // Return the new decided upon priority
        return( ucPrevPrio );
    }
}

/*******************************************************************************

FUNCTION NAME:
    CANDRV_fnAbort

PURPOSE:
    Abort a transmitted frame

INPUTS:
    Channel is the number of the CAN module to use, usually zero.
    Handle is an index to a transmit buffer

OUTPUTS:
    CANDRV_eRET_SUCCESS if all goes well

NOTES:
    This function will clear the transmit buffer but the user will have
    to wait until the transmit interrupt to find out if the frame was
    aborted or if it made it onto the bus before the abort could take effect.

VERSION HISTORY:

Version: 1.00  Date: 02/12/03  By: Tom Lightfoot
    - Creation
Version: 1.01  Date: 02/20/03  By: Tom Lightfoot
    - Handle is now a number from 0 to 2 instead of a bitmask
Version: 1.02  Date: 07/31/03  By: Dalem
    - Convert handle to bitmask to check the CANTFLG bits

*******************************************************************************/

CANDRV_teRETURN CANDRV_fnAbort( uchar8 Channel,
                                uchar8 Handle )
{
    tzMSCAN *pzCan;  // CAN module being used
    uchar8 ucMask;  

    // Get a pointer to the right controller
    if( Channel >= CAN_MAX_CHANNELS )
    {
        //  Index out of range
        return( CANDRV_eRET_BAD_CHANNEL );
    }

    // Get MSCAN structure pointer for given channel
    pzCan = kazCAN_LIST[ Channel ];

    // Convert handle to mask bit
    ucMask = candrv_fnHandleToMask( Handle );

    // Make sure the specified transmit buffer is busy
    if( ucMask & pzCan->CANTFLG.Byte )
    {
        // Buffer is empty so nothing to abort
        return( CANDRV_eRET_SUCCESS );
    }

    // Set the indicated abort request bit
    pzCan->CANTARQ.Byte = ucMask;

    // We'll have to wait until the transmit empty interrupt
    // in order to see if the frame was actually aborted or
    // if it actually made it onto the bus.
    return( CANDRV_eRET_SUCCESS );
}

/*******************************************************************************

FUNCTION NAME:
    candrv_fnStatusEnum

PURPOSE:
    Convert the MSCAN two-bit status into the driver's status enum

INPUTS:
    StatusBits is the two-bit field used to indicate either transmitter
        or receiver status.  The field is right aligned.

OUTPUTS:
    An enum of the transmitter or receiver status.

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/12/03  By: Tom Lightfoot
    - Creation
Version: 1.01  Date: 02/20/03  By: Tom Lightfoot
    - Fixed case numbers which were written in binary but interpreted in hex

*******************************************************************************/

static CANDRV_teSTATUS candrv_fnStatusEnum( uchar8 StatusBits )
{
    switch ( StatusBits )
    {
    case 0x00:                          // Hardware bits for status OK
        return( CANDRV_eSTATUS_OK );

    case 0x01:                          // Hardware bits for bus warning
        return( CANDRV_eSTATUS_WARNING );

    case 0x02:                          // Hardware bits for bus error
        return( CANDRV_eSTATUS_ERROR );

    case 0x03:                          // Hardware bits for Bus Off 
        return( CANDRV_eSTATUS_BUSOFF );

    default:
        // We're only looking at the last two bits
        return( CANDRV_eSTATUS_INVALID );
    }
}


/*******************************************************************************

FUNCTION NAME:
    CANDRV_fnTxStatus

PURPOSE:
    Get transmitter status and optional error count

INPUTS:
    Channel is the number of the CAN module to use, usually zero.
    pErrCount points to where to send the error count.  If this parameter is
        NULL then no error count is stored.

OUTPUTS:
    An enum of the transmitter's error status.

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/12/03  By: Tom Lightfoot
    - Creation
Version: 1.01  Date: 05/27/03  By: Tom Lightfoot
    - Used the right enum type when channel number out of range
Version: 1.02  Date: 08/01/03  By: dalem
    - Use merged bits for tx status from hardware

*******************************************************************************/

CANDRV_teSTATUS CANDRV_fnTxStatus( uchar8 Channel,
                                   uchar8 *pErrCount )
{
    tzMSCAN *pzCan;      // CAN module being used
    uchar8 StatusBits;   // Two bits from the register

    // Get a pointer to the right controller
    if( Channel >= CAN_MAX_CHANNELS )
    {
        //  Index out of range
        return( CANDRV_eSTATUS_INVALID );
    }

    pzCan = kazCAN_LIST[ Channel ];

    // Update the error count if a valid pointer was given
    if( pErrCount != NULL )
    {
        *pErrCount = pzCan->CANTXERR;
    }

    // Get the value of the two bits that indicate the transmit status
    StatusBits = pzCan->CANRFLG.MergedBits.TSTAT;

    // Return the enum that corresponds to those two bits
    return( candrv_fnStatusEnum( StatusBits ) );
}

/*******************************************************************************

FUNCTION NAME:
    CANDRV_fnRxStatus

PURPOSE:
    Get receiver status and optional error count

INPUTS:
    Channel is the number of the CAN module to use, usually zero.
    pErrCount points to where to send the error count.  If this parameter is
        NULL then no error count is stored.

OUTPUTS:
    An enum of the receiver's error status.

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/12/03  By: Tom Lightfoot
    - Creation
Version: 1.01  Date: 05/27/03  By: Tom Lightfoot
    - Used the right enum type when channel number out of range
Version: 1.02  Date: 08/01/03  By: dalem
    - Use merged bits for rx status from hardware

*******************************************************************************/

CANDRV_teSTATUS CANDRV_fnRxStatus( uchar8 Channel,
                                   uchar8 *pErrCount )
{
    tzMSCAN *pzCan;      // CAN module being used
    uchar8  StatusBits;  // Two-bits indicating receiver status

    // Get a pointer to the right controller
    if( Channel >= CAN_MAX_CHANNELS )
    {
        //  Index out of range
        return( CANDRV_eSTATUS_INVALID );
    }

    pzCan = kazCAN_LIST[ Channel ];

    // Update the error count if a valid pointer was given
    if( pErrCount != NULL )
    {
        *pErrCount = pzCan->CANRXERR;
    }

    // Get the value of the two bits that indicate the receive status
    StatusBits = pzCan->CANRFLG.MergedBits.RSTAT;

    // Return the enum that corresponds to those two bits
    return( candrv_fnStatusEnum( StatusBits ) );
}

/*******************************************************************************

FUNCTION NAME:
    CANDRV_fnGetHandle

PURPOSE:
    Get Handle on transmit buffer being serviced during interrupt

INPUTS:
    Channel is the number of the CAN module to use, usually zero.

OUTPUTS:
    The number of the transmit buffer that the transmit ISR detected
    as causing the interrupt.

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/20/03  By: Tom Lightfoot
    - Creation

*******************************************************************************/

uchar8 CANDRV_fnGetHandle( uchar8 Channel )
{
    // Return the value retrieved during the Transmit ISR
    return( candrv_aucHandle[ Channel ] );
}

/*******************************************************************************

FUNCTION NAME:
    candrv_fnMaskToHandle

PURPOSE:
    Translate a bitmask to a handle

INPUTS:
    A bitmask that indicates a transmitter.

OUTPUTS:
    The number of the bit that is set
    8 if no bits in Mask are set

NOTES:
    On the MSCAN module there are three transmit buffers.  The handle number
    will therefore be a number from 0 to 2.

VERSION HISTORY:

Version: 1.00  Date: 02/20/03  By: Tom Lightfoot
    - Creation

*******************************************************************************/

static uchar8 candrv_fnMaskToHandle( uchar8 Mask )
{
    uchar8 Handle;  // The handle returned by this function

    // Count how many times that Mask can be shifted to the
    // right before finding 1 in bit 0
    for( Handle = 0; Handle < 8; Handle++ )
    {
        // Is there a 1 in bit zero?
        if( Mask & 0x01 )
        {
            // Success! Return the current count
            break;
        }
        else
        {
            // Keep shifting
            Mask >>= 1;
        }
    }

    // Return the final count. 8 indicates an empty mask
    return( Handle );
}

/*******************************************************************************

FUNCTION NAME:
    candrv_fnMaskToHandle

PURPOSE:
    Translate a handle to a bitmask

INPUTS:
    A transmit buffer handle number

OUTPUTS:
    A bitmask that indicates the transmitter for use by the hardware

NOTES:
    On the MSCAN module there are three transmit buffers.  The handle number
    will therefore be a number from 0 to 2 and the Mask will be 1, 2 or 4

VERSION HISTORY:

Version: 1.00  Date: 02/20/03  By: Tom Lightfoot
    - Creation

*******************************************************************************/

static uchar8 candrv_fnHandleToMask( uchar8 Handle )
{
    // Make sure that the handle is in range
    if( Handle < CANDRV_TX_BUFFERS )
    {
        // Shift a 1 by the Handle number
        return( 0x01 << Handle );
    }
    else
    {
        // Bad handle
        return( 0 );
    }
}

/*******************************************************************************

FUNCTION NAME:
    CANDRV_fnInitCbList

PURPOSE:
    Initialize the callback list to all NULLs

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    Call this function before any CAN channels are initialized.

VERSION HISTORY:

Version: 1.00  Date: 02/12/03  By: Tom Lightfoot
    - Creation

*******************************************************************************/

void CANDRV_fnInitCbList( void )
{
    uchar8 Channel; // Counts through the channels
    uchar8 Index;   // Counts through the callback list

    for( Channel = 0; Channel < CAN_MAX_CHANNELS; Channel++ )
    {
        for( Index = 0; Index < CANDRV_eCB_LAST; Index++ )
        {
            candrv_atpfnCallback[ Channel ][ Index ] = NULL;
        }
    }
}

/*******************************************************************************

FUNCTION NAME:
    CANDRV_fnInstallCb

PURPOSE:
    Install callbacks for handling interrupts

INPUTS:
    Channel is the number of the CAN module to use, usually zero.
    CbType is an enum of what situation the function is to handle
    pFunc is the pointer to the callback function.

OUTPUTS:
    Returns CANDRV_eRET_SUCCESS on completion, or a failure code.
    

NOTES:
    Call this function after calling CAN_fnInitCBList and before any
    CAN channels are initialized.

    The callback functions are called while the interrupt is being
    handled so they should not do very much processing, mostly on the
    order of queueing buffers and setting flags for task-level
    processing later.

VERSION HISTORY:

Version: 1.00  Date: 02/12/03  By: Tom Lightfoot
    - Creation
Version: 1.01  Date: 07/31/03  By: dalem
    - Added returning of return code

*******************************************************************************/

CANDRV_teRETURN CANDRV_fnInstallCb( uchar8 Channel,
                                    CANDRV_teCB_TYPE CbType,
                                    CANDRV_tpfnCALLBACK pFunc )
{
    // Make sure the channel is in the right range
    if( Channel >= CAN_MAX_CHANNELS )
    {
        //  Index out of range
        return( CANDRV_eRET_BAD_CHANNEL );
    }

    // Make sure this is a valid callback type
    if( CbType >= CANDRV_eCB_LAST )
    {
        // Index out of range
        return( CANDRV_eRET_FAILURE );
    }

    // Install the pointer in the list
    candrv_atpfnCallback[ Channel ][ CbType ] = pFunc;

    // Complete
    return( CANDRV_eRET_SUCCESS );
}



/*==============================================================================
                           Interrupt Routines
==============================================================================*/



/*******************************************************************************

FUNCTION NAME:
    candrv_fnTxIsr

PURPOSE:
    General purpose interrupt routine for transmit buffer empty.
    This function determines if the buffer is empty because of a successful
    transmission or because of an aborted frame and call either the
    call back for transmit OK or abort as appropriate.
    The interrupt for the empty buffer is then disabled.

INPUTS:
    Channel is the number of the CAN module to use, usually zero.

OUTPUTS:
    None

NOTES:
    This function is called from the transmit interrupt routines of
    all channels.

VERSION HISTORY:

Version: 1.00  Date: 02/12/03  By: Tom Lightfoot
    - Creation
Version: 1.01  Date: 02/20/03  By: Tom Lightfoot
    - Save handle of transmitted buffer for use by callback functions

*******************************************************************************/

static void candrv_fnTxIsr( uchar8 Channel )
{
    CANDRV_tpfnCALLBACK pFunc;  // Callback function
    tzMSCAN *pzCan;             // CAN module being used
    CANDRV_teCB_TYPE Reason;    // Reason for interrupt, indicating which callback
    uchar8 Flags;               // Bit mask of possible interrupt causes
    uchar8 Mask;                // Mask of the interrupt we're actually handling

    // Point to the module causing the interrupt
    pzCan = kazCAN_LIST[ Channel ];

    // This interrupt can be due to successful transmission
    // or aborted frame from any of three transmit buffers

    // Find out which transmit buffer caused the interrupt
    Flags = pzCan->CANTFLG.Byte & pzCan->CANTIER.Byte;

    // Determine which transmit buffer we're handling.
    // Pick the lowest order buffer first.
    for( Mask = 0x01; Mask < ( 0x01 << CANDRV_TX_BUFFERS ); Mask <<= 1 )
    {
        if( Mask & Flags )
        {
            break;
        }
    }

    // Save the handle that this mask refers to for the ISR
    candrv_aucHandle[ Channel ] = candrv_fnMaskToHandle( Mask );

    // Was the frame aborted?
    if( pzCan->CANTAAK.Byte & Mask )
    {
        Reason = CANDRV_eCB_TXABORT;
    }
    else
    {
        // Interrupt must be due to transmitted frame
        Reason = CANDRV_eCB_TXOK;
    }

    // Disable interrupts for that buffer
    pzCan->CANTIER.Byte &= ~Mask;

    // Look up function for this channel and reason
    pFunc = candrv_atpfnCallback[ Channel ][ Reason ];

    // Call the function if there is a function
    if( pFunc != NULL )
    {
        pFunc( Channel );
    }

    // Don't bother clearing the flag for the transmit buffer.
    // Wait until there is another frame to send in that buffer.
}

/*******************************************************************************

FUNCTION NAME:
    candrv_fnRxIsr

PURPOSE:
    General purpose interrupt routine for receive buffer full.
    This function calls the callback for receive and then clears the
    receive interrupt flag.

INPUTS:
    Channel is the number of the CAN module to use, usually zero.

OUTPUTS:
    None

NOTES:
    This function is called from the receive interrupt routines of
    all channels.

VERSION HISTORY:

Version: 1.00  Date: 02/12/03  By: Tom Lightfoot
    - Creation

*******************************************************************************/

static void candrv_fnRxIsr( uchar8 Channel )
{
    tzMSCAN *pzCan;             // CAN module being used
    CANDRV_tpfnCALLBACK pFunc;  // Callback function

    // Point to the module causing the interrupt
    pzCan = kazCAN_LIST[ Channel ];

    // Disable receive interrupts in the event that the callback enables
    // nested interrupts
    pzCan->CANRIER.Bits.RXFIE = 0;

    // Look up the callback for receive for this channel
    pFunc = candrv_atpfnCallback[ Channel ][ CANDRV_eCB_RECEIVE ];

    // Call the function if there is a function
    if( pFunc )
    {
        pFunc( Channel );
    }

    // Clear the interrupt flag
    pzCan->CANRFLG.Byte = CANRFLG_RXF;

    // Re-enable receive interrupts
    pzCan->CANRIER.Bits.RXFIE = 1;
}

/*******************************************************************************

FUNCTION NAME:
    candrv_fnErrIsr

PURPOSE:
    General purpose interrupt routine for errors.
    This function determines if the interrupt is due to a change of status
    or a receiver overrun and calls the appropriate callback.
    This function then  clears the error interrupt flag.

INPUTS:
    Channel is the number of the CAN module to use, usually zero.

OUTPUTS:
    None

NOTES:
    This function is called from the error interrupt routines of
    all channels.

VERSION HISTORY:

Version: 1.00  Date: 02/12/03  By: Tom Lightfoot
    - Creation

*******************************************************************************/

static void candrv_fnErrIsr( uchar8 Channel )
{
    tzMSCAN *pzCan;             // CAN module being used
    CANDRV_tpfnCALLBACK pFunc;  // Callback function

    // Point to the module causing the interrupt
    pzCan = kazCAN_LIST[ Channel ];

    // This interrupt can be due to either an overrun or a change of status

    // Is this a change of status?
    if( pzCan->CANRFLG.Byte & CANRFLG_CSIF )
    {
        // Look up the callback for change of status
        pFunc = candrv_atpfnCallback[ Channel ][ CANDRV_eCB_STATUS ];

        // Call the function if it exists
        if( pFunc != NULL )
        {
            pFunc( Channel );
        }

        // Clear the change of status flag
        pzCan->CANRFLG.Byte = CANRFLG_CSIF;
    }

    // Is this an overrun?
    else if( pzCan->CANRFLG.Byte & CANRFLG_OVRIF )
    {
        // Look up the callback for overrun
        pFunc = candrv_atpfnCallback[ Channel ][ CANDRV_eCB_OVERFLOW ];

        // Call the function if it exists
        if( pFunc != NULL )
        {
            pFunc( Channel );
        }

        // Clear the overrun flag
        pzCan->CANRFLG.Byte = CANRFLG_OVRIF;
    }
}

/*******************************************************************************

FUNCTION NAME:
    candrv_fnWakeIsr

PURPOSE:
    General purpose interrupt routine for wakeup.
    This function calls the callback for wakeup and then clears the
    wakeup interrupt flag.

INPUTS:
    Channel is the number of the CAN module to use, usually zero.

OUTPUTS:
    None

NOTES:
    This function is called from the wakeup interrupt routines of
    all channels.

VERSION HISTORY:

Version: 1.00  Date: 02/12/03  By: Tom Lightfoot
    - Creation

*******************************************************************************/

static void candrv_fnWakeIsr( uchar8 Channel )
{
    tzMSCAN *pzCan;             // CAN module being used
    CANDRV_tpfnCALLBACK pFunc;  // Callback function

    // Look up the callback for wakeup
    pFunc = candrv_atpfnCallback[ Channel ][ CANDRV_eCB_WAKEUP ];

    // Call the function if there is a function
    if( pFunc != NULL )
    {
        pFunc( Channel );
    }

    // Point to the module causing the interrupt
    pzCan = kazCAN_LIST[ Channel ];

    // Clear the interrupt flag by writing a 1 to the WUPIF bit.
    // The other bits won't be affected.
    pzCan->CANRFLG.Byte = CANRFLG_WUPIF;

    // Disable the interrupt until the next sleep cycle
    pzCan->CANRIER.Bits.WUPIE = 0;
}

/*==============================================================================
                       Interrupt routines for CAN channel 0
==============================================================================*/

#pragma CODE_SEG __NEAR_SEG INTERRUPT_ROUTINES


/*******************************************************************************

FUNCTION NAME:
    C0TX_ISR

PURPOSE:
    Transmit buffer empty interrupt handler for CAN channel 0.

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    Everything except for the uCOS mandated entry and exit functions can
    be done with the generalized interrupt handler.

VERSION HISTORY:

Version: 1.00  Date: 02/12/03  By: Tom Lightfoot
    - Creation

Version: 1.10  Date: Jan 20, 2004 By: Fred Pang
    - Added code to show ISR timing

*******************************************************************************/

#pragma TRAP_PROC
void C0TX_ISR( void )
{
#ifdef _UCOSII
    OSIntEnterExt();
#endif

#ifdef CAN_TX_ISR_TASK_TIME_CHECK
    DEVICE_mGLOBAL_INT_DIS();
    DEBUG_mPIN0_SET();
#endif  //  CAN_TX_ISR_TASK_TIME_CHECK

    candrv_fnTxIsr( CANDRV_USING_CAN0 );
    
#ifdef CAN_TX_ISR_TASK_TIME_CHECK
    DEBUG_mPIN0_CLR();
    DEVICE_mGLOBAL_INT_EN();
#endif  //  CAN_TX_ISR_TASK_TIME_CHECK

#ifdef _UCOSII
    OSIntExitExt();
#endif
}

/*******************************************************************************

FUNCTION NAME:
    C0RX_ISR

PURPOSE:
    Receive interrupt handler for CAN channel 0.

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    Everything except for the uCOS mandated entry and exit functions can
    be done with the generalized interrupt handler.

VERSION HISTORY:

Version: 1.00  Date: 02/12/03  By: Tom Lightfoot
    - Creation

Version: 1.10  Date: Jan 20, 2004 By: Fred Pang
    - Added code to show ISR timing

*******************************************************************************/

#pragma TRAP_PROC
void C0RX_ISR( void )
{
#ifdef _UCOSII
    OSIntEnterExt();
#endif

#ifdef CAN_RX_ISR_TASK_TIME_CHECK
    DEVICE_mGLOBAL_INT_DIS();
    DEBUG_mPIN0_SET();
#endif  //  CAN_RX_ISR_TASK_TIME_CHECK

    candrv_fnRxIsr( CANDRV_USING_CAN0 );

#ifdef CAN_RX_ISR_TASK_TIME_CHECK
    DEBUG_mPIN0_CLR();
    DEVICE_mGLOBAL_INT_EN();
#endif  //  CAN_RX_ISR_TASK_TIME_CHECK

#ifdef _UCOSII
    OSIntExitExt();
#endif
}

/*******************************************************************************

FUNCTION NAME:
    C0ERR_ISR

PURPOSE:
    Error interrupt handler for CAN channel 0.

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    Everything except for the uCOS mandated entry and exit functions can
    be done with the generalized interrupt handler.

VERSION HISTORY:

Version: 1.00  Date: 02/12/03  By: Tom Lightfoot
    - Creation

Version: 1.10  Date: Jan 20, 2004 By: Fred Pang
    - Added code to show ISR timing

*******************************************************************************/

#pragma TRAP_PROC
void C0ERR_ISR( void )
{
#ifdef _UCOSII
    OSIntEnterExt();
#endif

#ifdef CAN_ER_ISR_TASK_TIME_CHECK
    DEVICE_mGLOBAL_INT_DIS();
    DEBUG_mPIN0_SET();
#endif  //  CAN_ER_ISR_TASK_TIME_CHECK

    candrv_fnErrIsr( CANDRV_USING_CAN0 );

#ifdef CAN_ER_ISR_TASK_TIME_CHECK
    DEBUG_mPIN0_CLR();
    DEVICE_mGLOBAL_INT_EN();    
#endif  //  CAN_ER_ISR_TASK_TIME_CHECK

#ifdef _UCOSII
    OSIntExitExt();
#endif
}

/*******************************************************************************

FUNCTION NAME:
    C0WU_ISR

PURPOSE:
    Receive interrupt handler for CAN channel 0.

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    Everything except for the uCOS mandated entry and exit functions can
    be done with the generalized interrupt handler.

VERSION HISTORY:

Version: 1.00  Date: 02/12/03  By: Tom Lightfoot
    - Creation

*******************************************************************************/

#pragma TRAP_PROC
void C0WU_ISR( void )
{
#ifdef _UCOSII
    OSIntEnterExt();
#endif

    candrv_fnWakeIsr( CANDRV_USING_CAN0 );

#ifdef _UCOSII
    OSIntExitExt();
#endif
}


/*==============================================================================
                       Interrupt routines for CAN channel 1
==============================================================================*/

#ifdef CANDRV_USING_CAN1

// Transmit
#pragma TRAP_PROC
void C1TX_ISR( void )
{
#ifdef _UCOSII
    OSIntEnterExt();
#endif

    candrv_fnTxIsr( CANDRV_USING_CAN1 );

#ifdef _UCOSII
    OSIntExitExt();
#endif
}

// Receive
#pragma TRAP_PROC
void C1RX_ISR( void )
{
#ifdef _UCOSII
    OSIntEnterExt();
#endif

    candrv_fnRxIsr( CANDRV_USING_CAN1 );

#ifdef _UCOSII
    OSIntExitExt();
#endif
}

// Error
#pragma TRAP_PROC
void C1ERR_ISR( void )
{
#ifdef _UCOSII
    OSIntEnterExt();
#endif

    candrv_fnErrIsr( CANDRV_USING_CAN1 );

#ifdef _UCOSII
    OSIntExitExt();
#endif
}

// Wakeup
#pragma TRAP_PROC
void C1WU_ISR( void )
{
#ifdef _UCOSII
    OSIntEnterExt();
#endif

    candrv_fnWakeIsr( CANDRV_USING_CAN1 );

#ifdef _UCOSII
    OSIntExitExt();
#endif
}

#endif  // ifdef CANDRV_USING_CAN1

/*==============================================================================
                       Interrupt routines for CAN channel 2
==============================================================================*/

#ifdef CANDRV_USING_CAN2

// Transmit
#pragma TRAP_PROC
void C2TX_ISR( void )
{
#ifdef _UCOSII
    OSIntEnterExt();
#endif

    candrv_fnTxIsr( CANDRV_USING_CAN2 );

#ifdef _UCOSII
    OSIntExitExt();
#endif
}

// Receive
#pragma TRAP_PROC
void C2RX_ISR( void )
{
#ifdef _UCOSII
    OSIntEnterExt();
#endif

    candrv_fnRxIsr( CANDRV_USING_CAN2 );

#ifdef _UCOSII
    OSIntExitExt();
#endif
}

// Error
#pragma TRAP_PROC
void C2ERR_ISR( void )
{
#ifdef _UCOSII
    OSIntEnterExt();
#endif

    candrv_fnErrIsr( CANDRV_USING_CAN2 );

#ifdef _UCOSII
    OSIntExitExt();
#endif
}

// Wakeup
#pragma TRAP_PROC
void C2WU_ISR( void )
{
#ifdef _UCOSII
    OSIntEnterExt();
#endif

    candrv_fnWakeIsr( CANDRV_USING_CAN2 );

#ifdef _UCOSII
    OSIntExitExt();
#endif
}

#endif  // ifdef CANDRV_USING_CAN2

/*==============================================================================
                       Interrupt routines for CAN channel 3
==============================================================================*/

#ifdef CANDRV_USING_CAN3

// Transmit
#pragma TRAP_PROC
void C3TX_ISR( void )
{
#ifdef _UCOSII
    OSIntEnterExt();
#endif

    candrv_fnTxIsr( CANDRV_USING_CAN3 );

#ifdef _UCOSII
    OSIntExitExt();
#endif
}

// Receive
#pragma TRAP_PROC
void C3RX_ISR( void )
{
#ifdef _UCOSII
    OSIntEnterExt();
#endif

    candrv_fnRxIsr( CANDRV_USING_CAN3 );

#ifdef _UCOSII
    OSIntExitExt();
#endif
}

// Error
#pragma TRAP_PROC
void C3ERR_ISR( void )
{
#ifdef _UCOSII
    OSIntEnterExt();
#endif

    candrv_fnErrIsr( CANDRV_USING_CAN3 );

#ifdef _UCOSII
    OSIntExitExt();
#endif
}

// Wakeup
#pragma TRAP_PROC
void C3WU_ISR( void )
{
#ifdef _UCOSII
    OSIntEnterExt();
#endif

    candrv_fnWakeIsr( CANDRV_USING_CAN3 );

#ifdef _UCOSII
    OSIntExitExt();
#endif
}

#endif  // ifdef CANDRV_USING_CAN3

/*==============================================================================
                       Interrupt routines for CAN channel 4
==============================================================================*/

// Interrupt routines for CAN channel 4

// Transmit
#pragma TRAP_PROC
void C4TX_ISR( void )
{
#ifdef _UCOSII
    OSIntEnterExt();
#endif

    candrv_fnTxIsr( CANDRV_USING_CAN4 );

#ifdef _UCOSII
    OSIntExitExt();
#endif
}

// Receive
#pragma TRAP_PROC
void C4RX_ISR( void )
{
#ifdef _UCOSII
    OSIntEnterExt();
#endif

    candrv_fnRxIsr( CANDRV_USING_CAN4 );

#ifdef _UCOSII
    OSIntExitExt();
#endif
}

// Error
#pragma TRAP_PROC
void C4ERR_ISR( void )
{
#ifdef _UCOSII
    OSIntEnterExt();
#endif

    candrv_fnErrIsr( CANDRV_USING_CAN4 );

#ifdef _UCOSII
    OSIntExitExt();
#endif
}

// Wakeup
#pragma TRAP_PROC
void C4WU_ISR( void )
{
#ifdef _UCOSII
    OSIntEnterExt();
#endif

    candrv_fnWakeIsr( CANDRV_USING_CAN4 );

#ifdef _UCOSII
    OSIntExitExt();
#endif
}


#pragma CODE_SEG DEFAULT

