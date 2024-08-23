/*=============================================================================
Copyright © 2005 Xantrex International

This file is the property of Xantrex International and shall not be reproduced,
copied, or used as the basis for the manufacture or sale of equipment without
the express written permission of Xantrex International.

FILE NAME:  candrv.c

PURPOSE:
    This driver defines the Hardware Abstraction Layer for
    the Motorola Scalable Controller Area Network module on the
    TI F281x processor.

FUNCTION(S):
    CANDRV_fnInit       - Initialize CAN driver channel
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
    CANDRV_fnInitCbList - Initialize the callback list
    CANDRV_fnInstallCb  - Install a callback for an interrupt

    interrupt handlers:
        CANDRV_fnECAN0INTIsr     - Transmit and status interrupt
        CANDRV_fnECAN1INTIsr     - Receive interrupt


    local:
        candrv_fnInitMBoxes   - initialize the various mailbox properties
        candrv_fnInitInterrupt- Configure the various interrupt registers
        candrv_fnDataId2HwId  - convert passed in ID to id as stored in
                                registers
        candrv_fnHwId2DataId  - convert Id as stored in registers to ID
                                as expected by the application
        candrv_fnSetPriority  - Set transmit priority of the next frame to
                                transmit
        candrv_fnGetErr      - Read the current errors from the registers
        candrv_fnIsTxBuffersEmpty - check to see if the buffer is empty
        candrv_fnResetTxPrio - reset the transmit priority



NOTES:

    This driver does no buffering
    of frames aside from what is implemented through the mailboxes.
    There is also no use or assumption of any operating system services.

    A number of the registers used in this module are 32-bits wide.  However,
    this TI chip/compiler compiles it to 16-bit register access.  As a result,
    a shadow register is created for the CAN control registers, to allow
    32-bit access, as 16-bit accesses can potentially corrupt them.  This
    is especially true while writing to the bits 16-31.

    It is recommended that the user install callbacks for the various functions
    of this driver.  This will let this driver inform the user when a frame is
    received or transmitted.

    This driver uses the 32 mailboxes in the eCAN module.  Any of the mailboxes
    can be configured as tranmit or receive.  In this implementation, the first
    25 mailboxes( 0 to 24 ) are configured to be receive mailboxes, whereas,
    the next 7 mailboxes are configured to be tranmit mailboxes.

    The highest numbered mailbox is always considered to be highest priority
    mailbox.  For example, for receive mailboxes, if we have 25 Rx mailboxes,
    then mailbox 24 is the first mailbox to receive a message that matches
    its ID ( more about ID's below ), if its empty.  For transmit mailboxes,
    the same logic applies, however, you can explicitly assign a priority
    to the transmit mailbox that supercededs the mailbox number itself.

    This eCAN module allows the user to specify a filter ID to each of the
    receive mailboxes.  In this implementation, no such filtering is done.
    The receive mailboxes are configured such that it accepts all messages
    from the nodes.

    Only one bit rate is currently supported, 250KBPS.  The bit timing is
    very closely related to your current clock speed and the bit rate that
    you require must contain certain parameters in the TSEG1 and TSEG2 values
    such that they divide evenly with the clock speed and bit rate.  Any
    deviation will cause a problem.  In addition, for each bit, the sampling
    point should be between 80 and 90 %.  See diagram below:

    For each bit, the following are some of the design considerations:
    |   |                                           |                   |
    |   |                                           |                   |
    |   |                                   |  SJW  |           |  SJW  |
    |   |                                   |       |           |       |
    |--------------------------------------------------------------------
    | ^                                                                 |
      |  <--------------TSEG1----------------------><-----TSEG2--------->
      |
      |
      |
      SYNCSEG


                Sampling Point = SYNCSEG + TSEG1
                                 -----------------------  x 100%
                                 SYNCSEG + TSEG1 + TSEG2


                Sychronous Jump Width - SJW <= TSEG2

                SYNCSEG == 1  ( always )


                Number of Time Quantum per bit ( aka Bit Time ) =
                                SYNCSEG + TSEG1 + TSEG2


                Bit Rate             =  SYSCLKOUT
                (ie 250KPBS, 1MBPS )    ------------------------------
                                        BitRatePrescale x BitTime

    Hence the calculation for BitRatePrescale =

                BRP =               SYSCLKOUT
                            -------------------------------------
                            BitRate x BitTime

                    =               SYSCLKOUT
                            -------------------------------------
                            BitRate x ( SYNCSEG + TSEG1 + TSEG2 )


    BRP, Bit Rate Prescaler, must be divisible evenly or else you will
    be an erroneous bit rate.
    
    Also note, this driver tries to maintain a first in, first out transmit
    priority.  By default, this is not done by the mailboxes, so we have
    to ensure our own priority.  Currently, only 5 bits are supported by
    the mailboxes for priority, so if more than 32 frames are sent without
    a reset, then the driver will wait until all remaining frames are sent
    out and only then will it be free to transmit again.
    
    This module has function switch enables to not link in functions that
    aren't used.  If you don't want certain functions to be linked in, 
    declare the appropriate switch to be false in you project file.


HISTORY:
$Log: candrv.c $

    ***********************************************
    Revision: SurveyorPfrmA_HollyZ/3
    User: HollyZ     Date: 02/05/09  Time: 05:11PM
    Added new function to get the current CAN transmitter status for indicating
    whether the frames are stuck in the Tx buffer or not.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/7
    User: JeffF     Date: 02/15/07  Time: 09:54PM
    Resolved PRN 4873. The unit was not coming out of hibernate properly sometimes.
    The unit was sometimes not receiving messages properly after coming out
    of hibernate. It appears that the unit was not powering down its CAN module
    properly and this was causing i

    ssues when it was being commanded to power back up. The CAN driver now
    clears the WUBA bit to ensure that it does not try to wake up on bus activity.
    

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/6
    User: JeffF     Date: 02/08/07  Time: 09:48PM
    Changed the CAN driver to use Auto Bus On.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/5
    User: JeffF     Date: 02/08/07  Time: 05:40PM
    Added in three lines of code to clear some status bits after they have
    been read. For this register, if bits are not cleared, then new status
    events will not be indicated as the register latches to the previous value
    until the bit(s) are cleared.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/4
    User: JeffF     Date: 01/09/07  Time: 09:14PM
    Made a couple minor changes as per Holly's code review on this file. The
    only significant change was removing the instructions in the init function
    that were disabling and enabling the global interrupt bit. I didn't realize
    that this bit was disabled at staru

    tp and enabled once all the ctor's and init's had run, so it is unnessary
    to use it here.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/3
    User: JeffF     Date: 01/09/07  Time: 12:12AM
    Made extensive changes to address PRN 4766 - Xanbus connection being permanently
    lost on some units. 

    The control and status registers for the CAN module on the 28xx DSPs have
    a requirement of only accessing their entire 32-bits at once (ie. no 16-bit
    instructions should be used to read or write to them). There was many instances
    of registers being accessed u

    sing the individual bit fields from the header file. This was causing inconsistent
    results where some registers were getting corrupted and being set to the
    wrong values. This resulted in some units being unable to communicate on
    our Xanbus network.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/2
    User: JeffF     Date: 12/18/06  Time: 09:59PM
    Fixed problems with the enable and disable functions in the Can driver
    file. Everything was being enabled in the init function, so it never showed
    up as a problem. For sake of writing the code correctly, the init function
    has been stripped of the enabling functionality, and the enab/disab 
    functions have been repaired to function properly.

    ***********************************************
    Revision: SurveyorPfrmA_New_HueyD/2
    User: HueyD     Date: 06/21/06  Time: 05:35PM
    Added conditional compile defines to avoid warnings

    ***********************************************
    Revision: SurveyorPfrmA_New_HueyD/1
    User: HueyD     Date: 04/14/06  Time: 12:10AM
    Fixed debug issue where debugger could not step through code when receiving
    CANbus packets

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/1
    User: JeffF     Date: 02/13/06  Time: 06:59PM
    Ran the copyright updater script to bring the copyright notice on all project
    files up to date.

    ***********************************************
    Revision: SurveyorPfrmA_HollyZ/2
    User: HollyZ     Date: 10/31/05  Time: 05:48PM
    Added complier option for loader mode

    ***********************************************
    Revision: SurveyorPfrmA_HollyZ/1
    User: HollyZ     Date: 09/02/05  Time: 10:08PM
    - Added prescale validation check at debug time

    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/11
    User: BaldeeshK     Date: 03/30/05  Time: 05:45PM
    - Added function call enables/disable switches

    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/10
    User: BaldeeshK     Date: 03/24/05  Time: 09:46PM
    - Made all the shadow variables volatile in order to prevent errors when
    reading registers

    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/9
    User: BaldeeshK     Date: 03/23/05  Time: 12:52AM
    - Modified operation of the transmit priority.  When too many messages
    arrive, the priority is then frozen until all the remaining messages are
    output, ie the buffer is flushed.  Then it resets the priority back to
    its maximum state

    ***********************************************
    Revision: NovaPfrmB_DaleM/1
    User: DaleM     Date: 03/22/05  Time: 05:18PM
    - In function, fnInitInterrupts, removed device enable/disable 

    ***********************************************
    Revision: NovaPfrmB_JohnB/1
    User: JohnB     Date: 03/22/05  Time: 12:55AM
    Update ISR setup.  Each driver now writes to the Pie vector table, the
    address of the ISR is cares about.

    Updated Main to call the default ISR copying.

   ***********************************************
   Revision: NovaPfrmB_BaldeeshK/8
   User: BaldeeshK     Date: 03/17/05  Time: 05:36PM
   - Added priority to all transmitted messages so that messages are sent first
   in, first out.  Also, cleaned up comments in file header

    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/7
    User: BaldeeshK     Date: 03/09/05  Time: 07:55PM
    - Added byte masks to transmit function


    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/6
    User: BaldeeshK     Date: 02/24/05  Time: 09:46PM
    - Fixed bug with transmit interrupts, made transmit interrupts to be always
    on, and changed operation of clearing the CANTA and CANRMP flags



    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/5
    User: BaldeeshK     Date: 02/18/05  Time: 07:45PM
    - Changed the handler in transmit functions to be offset from 0 instead of representing
    the mailbox number.  Added function fnGetHandle.


    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/4
    User: BaldeeshK     Date: 02/17/05  Time: 09:39PM
    - Fixed text error with a misplaced line in function CANDRV_fnECAN0INTIsr


    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/3
    User: BaldeeshK     Date: 02/16/05  Time: 04:56PM
    - Bug fix when extracting the bits for EXTMSGID_H.  Previously, it was masking
    with XT_BITPOS2, when it should have been XT_BIT2.


    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/2
    User: BaldeeshK     Date: 02/10/05  Time: 05:23PM
    - Initial add to Accurev of candrv source file



=============================================================================*/

/*==============================================================================
                              Includes
==============================================================================*/

#include <limits.h>         // for uint32, uint16 max
#include "candrv.h"         // CANDRV interface declarations
#include "device.h"         // for device registers
#include "devicemacro.h"    // for device macros
#include "config.h"         // Hardware specifications
#include "xassert.h"        // for assert

/*=============================================================================
                                Defines
=============================================================================*/
#define CANDRV_SUPPORTED_BIT_RATE  ( CANDRV_eBIT_RATE_250KBPS )
// number of time quantums for time segment 1
#define CANDRV_250KBPS_TSEG1    ( 13 )
// number of time quantumes for time segment 2
#define CANDRV_250KBPS_TSEG2    ( 2 )
// synchronous jump width, must be less than or equal to TSEG2, make it equal
#define CANDRV_250KBPS_SJW      ( CANDRV_250KBPS_TSEG2 )
// speed for 250kBps for calculation purposes
#define CANDRV_250KBPS_BIT_RATE ( 250000L ) 
// baud rate prescaler value based on above settings
#define CANDRV_250KBPS_BRP      ( CTRL_CPU_MAXSPEED /              \
                                ( ( uint32 )( CANDRV_250KBPS_TSEG1 \
                                + CANDRV_250KBPS_TSEG2 + 1 )       \
                                * CANDRV_250KBPS_BIT_RATE ) )

// total number of mailboxes availabe in this module
#define CANDRV_TOTAL_NUM_MBOXES ( 32 )

// how many mailboxes of the 32 are to be tranmit mailboxes
#define CANDRV_NUM_TX_MBOXES    ( CANDRV_TX_BUFFERS )

// how many mailboxes of the 32 are to be receive mailboxes
#define CANDRV_NUM_RX_MBOXES    ( CANDRV_TOTAL_NUM_MBOXES - \
                                  CANDRV_NUM_TX_MBOXES )
// mailbox mask for the transmit Mboxes.  Ie if the top three mailboxes
// are tranmit mailboxes, the mask should be 0xE0000000
#define CANDRV_TX_MBOX_MASK    ( CANDRV_UINT32_MAX & \
                                ~( ( 1L << CANDRV_NUM_RX_MBOXES ) - 1 )
#define CANDRV_RX_MBOX_MASK    ( ( 1L << CANDRV_NUM_RX_MBOXES ) - 1 )

// bit masks for the general interrupt flags register, used to clear
// the appropriate interrupt
#define CANDRV_STATUS_MASK_BITS  ( 0x00032700L )

// Max tries while waiting for the CAN port to change state
#define CANDRV_MAX_TRIES       ( 0xFFFF )

#define CANDRV_UINT16_MAX   ( UINT_MAX )
// Max value for uint32 register
#define CANDRV_UINT32_MAX   ( ULONG_MAX )

// Byte Mask
#define CANDRV_BYTE_MASK    ( 0xFF )

// mask for accepting all for filters
#define CANDRV_ACCEPT_ALL   ( CANDRV_UINT32_MAX )
#define CANDRV_ACCEPT_NONE  ( 0L )

// define how many channels
#define CANDRV_MAX_CHANNELS    ( 1 )

// maximum number of bytes per frame
#define CANDRV_MAX_BYTES_PER_FRAME ( 8 )

// mask for the error bits in the CANES status register
#define CANDRV_ERROR_STATUS_BITS ( 0x01FA0000L )

// value for the maximum tx priority
#define CANDRV_MAX_TX_PRIORITY    ( 0x1F )

/*=============================================================================
                                Macros
=============================================================================*/
// get the number of time quantums per bit
#define CANDRV_mGET_TQ_SIZE( x )    ( x.ucTSeg1 + x.ucTSeg2 + 1 )

/*=============================================================================
                                Function Compile Switches
=============================================================================*/

// include code for disabling the candrv
#ifndef CANDRV_DISABLE_FN_ENABLE
#define CANDRV_DISABLE_FN_ENABLE       ( TRUE )
#endif

// include code for going to sleep
#ifndef CANDRV_SLEEP_ENABLE
#ifdef LOADER
#define CANDRV_SLEEP_ENABLE            ( FALSE )
#else
#define CANDRV_SLEEP_ENABLE            ( TRUE )
#endif
#endif

// include code for wakeup
#ifndef CANDRV_WAKEUP_ENABLE
#ifdef LOADER
#define CANDRV_WAKEUP_ENABLE           ( FALSE )
#else
#define CANDRV_WAKEUP_ENABLE           ( TRUE )
#endif
#endif

// include code for getting transmit status
#ifndef CANDRV_TX_STATUS_ENABLE
#ifdef LOADER
#define CANDRV_TX_STATUS_ENABLE        ( FALSE )
#else
#define CANDRV_TX_STATUS_ENABLE        ( TRUE )
#endif
#endif

// include code for getting receive status
#ifndef CANDRV_RX_STATUS_ENABLE 
#ifdef LOADER
#define CANDRV_RX_STATUS_ENABLE        ( FALSE )
#else
#define CANDRV_RX_STATUS_ENABLE        ( TRUE )
#endif
#endif

// include code for abort
#ifndef CANDRV_ABORT_ENABLE 
#ifdef LOADER
#define CANDRV_ABORT_ENABLE            ( FALSE )
#else
#define CANDRV_ABORT_ENABLE            ( TRUE )
#endif
#endif

// include code for getting handle
#ifndef CANDRV_GET_HANDLE_ENABLE 
#ifdef LOADER
#define CANDRV_GET_HANDLE_ENABLE        ( FALSE )
#else
#define CANDRV_GET_HANDLE_ENABLE        ( TRUE )
#endif
#endif

/*=============================================================================
                               Data Types
=============================================================================*/

typedef struct zMSCAN_STATE
{
    // flag to indicate whether we have a successful transmision 
    // after a transmitter state change
    uchar8 ucTransmitting;
    
    // change of TX state
    uchar8 ucTxStateChange;       
    
    // change of RX state
    uchar8 ucRxStateChange;
    
    // last Tx State
    uchar8 ucPreTxState;
    
    // last Rx State
    uchar8 ucPreRxState;
} tzMSCAN_STATE;       

/*==============================================================================
                           Local/Private Constants
==============================================================================*/

/*==============================================================================
                           Local/Private Variables
==============================================================================*/

// The list of function pointers for interrupt callbacks
static CANDRV_tpfnCALLBACK candrv_apCallback[ CANDRV_eCB_LAST ];

#if ( CANDRV_GET_HANDLE_ENABLE == TRUE )
// Handle of the latest transmitted buffer
static uchar8 candrv_ucHandle;
#endif

// transmit priority counter to ensure tx message are sent in order
static uchar8 candrv_ucTxPrio = CANDRV_MAX_TX_PRIORITY;

// transmit priority wait flag
static tucBOOL candrv_tucTxWaitFlag = FALSE;  

// CAN controller state and statistics info
static tzMSCAN_STATE tzCanState[ CANDRV_MAX_CHANNELS ];
/*==============================================================================
                           Local Function Definitions
==============================================================================*/

// initialize the mailboxes
static tuiSTATUS candrv_fnInitMBoxes( CANDRV_teFILTER_MODE FilterMode );

// initialize the interrupt registers
static void candrv_fnInitInterrupts( void );

// convert the ID passed in by the user to data found in the
// mailbox registers
static void candrv_fnDataId2HwId( union CANMSGID_REG* puDestId,
                                  CANDRV_tzCAN_ID *ptzSrcId );

// conver the ID found in the mailbox register to the user data
// as passed in by the user
static void candrv_fnHwId2DataId( CANDRV_tzCAN_ID *ptzSrcId,
                                  union CANMSGID_REG* puDestId );

#if ( ( CANDRV_TX_STATUS_ENABLE == TRUE ) || ( CANDRV_RX_STATUS_ENABLE == TRUE ) )
// get the error from the module
static CANDRV_teSTATUS candrv_fnGetError( void );
#endif

// check to see if the tx buffers are empty
static tucBOOL candrv_fnIsTxBuffersEmpty( void );

// reset the transmit buffer
static void candrv_fnResetTxPrio( void );

extern void ISR_fnECAN0INT( void );
extern void ISR_fnECAN1INT( void );

/*==============================================================================
                           Function Definitions
==============================================================================*/

/*******************************************************************************

FUNCTION NAME:
    CANDRV_fnInit

PURPOSE:
    Initialize the CAN module for a given channel

INPUTS:
    ucChannel is the channel number, usually zero  - NOT USED, only one channel
                                                   exists on the F281x DSP's
    BitRate is an enum of the bit rates supported
    FilterMode indicates if the filter is to be set to pass everything or
    nothing

OUTPUTS:
    CANDRV_eSUCCESS if all goes well

NOTES:


VERSION HISTORY:

Version: 1.00  Date: 01/25/05  By: Baldeesh S. Khaira
    - Created
Version: 1.01  Date: 02/24/05  By: Baldeesh S. Khaira
    - Moved location of clearing CANTA and CANRMP
Version: 1.02  Date: 09/02/05  By: Holly Zhou
    - Added prescale validation check at debug time
Version: 1.02  Date: 18-Dec-2006  By: Jeff Fieldhouse
    - Removed register assigments that are supposed to be handled in the 
      enable and disable functions.    

*******************************************************************************/

CANDRV_teRETURN CANDRV_fnInit( uchar8 ucChannel,
                               CANDRV_teBIT_RATE teBitRate,
                               CANDRV_teFILTER_MODE teFilterMode )
{
    uint16 Cntr = CANDRV_MAX_TRIES;
    volatile uint32 ulShadowReg;
    
    // only 1 channel is supported on this device
    assert( ( ucChannel == 0 ) || ( ucChannel > CANDRV_MAX_CHANNELS ) );

     // clear the CAN state struct
    ( void )memset( &tzCanState[ ucChannel ] , 0x00, sizeof( tzMSCAN_STATE ) );
   
   
    // Driver is set up for a specific baud rate, so make sure user wants
    // the configured value
    if( CANDRV_SUPPORTED_BIT_RATE != teBitRate )
    {
        assert( FALSE );
        return( CANDRV_eRET_INIT_FAIL );
    }
    
    // Enable access to the write protected registers
    DEVICE_mACCESS_EN();

    // prior to setting the mode, ensure that the CAN clock is connected
    SysCtrlRegs.PCLKCR0.bit.ECANAENCLK = 1;
    
    // ************************************************************************
    // ************************ Very Important!!!! ****************************
    // ************************************************************************
    // Spru074e.pdf states that: Only 32-bit accesses are allowed to the CAN 
    // control and status registers. This restriction does not apply to the 
    // mailbox RAM area. Therefore, DO NOT use the bit fields provided in the
    // TI header file for the CAN module.

    // make sure the pins are assigned to the CAN module
    ECanaRegs.CANTIOC.all = ( F280X_CANTIOC_TXFUNC_ENA << F280X_CANTIOC_POS_TXFUNC );
    ECanaRegs.CANRIOC.all = ( F280X_CANTIOC_RXFUNC_ENA << F280X_CANTIOC_POS_RXFUNC );    
    
    // Set up the CANMC register. Make sure CCR bit is set for config mode
    ECanaRegs.CANMC.all = 
        ( ( F280X_CANMC_SUSP_SOFT       << F280X_CANMC_POS_SUSP )                          
        | ( F280X_CANMC_MBCC_NO_RST     << F280X_CANMC_POS_MBCC )                          
        | ( F280X_CANMC_TCC_NO_RST      << F280X_CANMC_POS_TCC  )                          
        | ( F280X_CANMC_SCB_ECAN_MODE   << F280X_CANMC_POS_SCB  ) // eCAN mode                         
        | ( F280X_CANMC_CCR_CONFIG_MODE << F280X_CANMC_POS_CCR  ) // Set for config mode                          
        | ( F280X_CANMC_PDR_NORMAL_MODE << F280X_CANMC_POS_PDR  )                          
        | ( F280X_CANMC_DBO_LSB_FIRST   << F280X_CANMC_POS_DBO  ) // LSB first                         
        | ( F280X_CANMC_WUBA_OFF        << F280X_CANMC_POS_WUBA )                          
        | ( F280X_CANMC_CDR_NORMAL_MODE << F280X_CANMC_POS_CDR  )                          
        | ( F280X_CANMC_ABO_ON          << F280X_CANMC_POS_ABO  )                          
        | ( F280X_CANMC_STM_OFF         << F280X_CANMC_POS_STM  )                          
        | ( F280X_CANMC_SRES_NO_RST     << F280X_CANMC_POS_SRES ) );
            
    // Delay until the status flag is set indicating we are in config mode
    while( ( ECanaRegs.CANES.all & F280X_CANES_MSK_CCE ) == 0L )
    {
        // Use counter to ensure we do not get stuck in an infinite loop
        if( --Cntr == 0 )
        {
            DEVICE_mACCESS_DIS();
            assert( FALSE );
            return( CANDRV_eRET_INIT_FAIL );
        }
    }

    // Assign the bit timing parameters, the synchronous jump width,
    // time segment 1, and time segment 2
    // Time segment 1 comprises of the propogation delay and phase
    // delays
    // Time segment 2 comprises of the receiver phase delays
    //
    // One is subtracted to each of these values because the register
    // contains values from 0...n-1, whereas the minimum value is
    // 1...n
    ECanaRegs.CANBTC.all = 
        ( ( ( CANDRV_250KBPS_BRP - 1   ) << F280X_CANBTC_POS_BRP )
        | ( ( CANDRV_250KBPS_SJW - 1   ) << F280X_CANBTC_POS_SJW )
        | ( ( 0L                       ) << F280X_CANBTC_POS_SAM )
        | ( ( CANDRV_250KBPS_TSEG1 - 1 ) << F280X_CANBTC_POS_TSEG1 )
        | ( ( CANDRV_250KBPS_TSEG2 - 1 ) << F280X_CANBTC_POS_TSEG2 ) );

    // Once the CANBTC register is set, clear the CCR bit for normal mode
    ulShadowReg = ECanaRegs.CANMC.all;
    ulShadowReg &= ~F280X_CANMC_MSK_CCR; 
    ECanaRegs.CANMC.all = ulShadowReg;             

    // initialize the control registers of the mailboxes
    if( candrv_fnInitMBoxes( teFilterMode ) != eSTATUS_OK )
    {
        DEVICE_mACCESS_DIS();
        assert( FALSE );
        return( CANDRV_eRET_INIT_FAIL );
    }

    // setup the interrupt registers
    candrv_fnInitInterrupts();

    // reset the transmit priority
    candrv_fnResetTxPrio();

    // *** The two interrupt lines get enabled in the driver enable function

    PieCtrlRegs.PIEIER9.bit.INTx5 = 1;  // Enable INTx.5 of INT9 (eCAN0INT)
    PieCtrlRegs.PIEIER9.bit.INTx6 = 1;  // Enable INTx.6 of INT9 (eCAN1INT)
    DEVICE_mINT9_EN();

    // Enable the mailboxes
    ECanaRegs.CANME.all = CANDRV_UINT32_MAX;

    // Clear all the transmission acknowledge bits
    ECanaRegs.CANTA.all = CANDRV_UINT32_MAX;

    // Clear all messages pending in all mailboxes
    ECanaRegs.CANRMP.all = CANDRV_UINT32_MAX;

    // disable register access
    DEVICE_mACCESS_DIS();

    // return ok
    return ( CANDRV_eRET_SUCCESS );
}


/*******************************************************************************

FUNCTION NAME:
    CANDRV_fnEnable

PURPOSE:
    This function enables the specified CAN channel and brings it out of
    init mode.

INPUTS:
    ucChannel - unused, cause there is only one channel

OUTPUTS:
    CANDRV_eRET_SUCCESS if all goes well

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/12/03  By: Tom Lightfoot
    - Creation
Version: 1.01  Date: 02/08/05  By: BaldeeshK
    - New version for 281x processor
Version: 1.02  Date: 18-Dec-2006  By: Jeff Fieldhouse
    - This function was missing the instruction to enable writing to the
      EALLOW protected registers. Essentially nothing was happening when this
      function was called
      
*******************************************************************************/

CANDRV_teRETURN CANDRV_fnEnable( uchar8 ucChannel )
{
    volatile uint32 ulShadowReg;
    
    // only a single channel defined
    assert( ucChannel == 0 );

    // enable the mailboxes
    ECanaRegs.CANME.all = CANDRV_UINT32_MAX;

    // Enable register access
    DEVICE_mACCESS_EN();
    
    // Set the mux registers for CAN RX and TX functionality
    GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 1;
    GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 1;

    // enable the two interrupt lines
    ulShadowReg = ECanaRegs.CANGIM.all;
    ulShadowReg |= ( ( 1L << F280X_CANGIM_POS_I0EN )
                   | ( 1L << F280X_CANGIM_POS_I1EN ) );
                                      
    ECanaRegs.CANGIM.all = ulShadowReg; 
    
    // disable register access
    DEVICE_mACCESS_DIS();    

    return ( CANDRV_eRET_SUCCESS );
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
    ucChannel is the number of the CAN module to disable, usually zero.

OUTPUTS:
    CANDRV_eRET_SUCCESS if all goes well

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/12/03  By: Tom Lightfoot
    - Creation
Version: 1.01  Date: 02/08/05  By: BaldeeshK
    - New version for 281x processor
Version: 1.02  Date: 18-Dec-2006  By: Jeff Fieldhouse
    - This function was missing the instruction to enable writing to the
      EALLOW protected registers. Essentially nothing was happening when this
      function was called

*******************************************************************************/
#if ( CANDRV_DISABLE_FN_ENABLE == TRUE )
CANDRV_teRETURN CANDRV_fnDisable( uchar8 ucChannel )
{
    volatile uint32 ulShadowReg;

     // only a single channel defined
    assert( ucChannel == 0 );

    // Enable register access
    DEVICE_mACCESS_EN();    
    
    // disable the mailboxes
    ECanaRegs.CANME.all = 0L;

    // Change the mux so that the pins are set as GPIOs instead of CAN RX and TX
    GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 0;

    // Disable the interrupts
    ulShadowReg = ECanaRegs.CANGIM.all;
    ulShadowReg &= ~( F280X_CANGIM_MSK_I0EN | F280X_CANGIM_MSK_I1EN );                                      
    ECanaRegs.CANGIM.all = ulShadowReg; 
    
    // disable register access
    DEVICE_mACCESS_DIS();    

    return ( CANDRV_eRET_SUCCESS );
}
#endif // #if ( CANDRV_DISABLE_FN_ENABLE == TRUE )

/*******************************************************************************

FUNCTION NAME:
    CANDRV_fnSleep

PURPOSE:
    Put CAN channel to sleep so that the CPU can use less power.

INPUTS:
    ucChannel is the number of the CAN module to put to sleep, usually zero.
    WakeEnable is TRUE if the module is to automatically wake up if there
        is traffic on the CAN bus.

OUTPUTS:
    CANDRV_eRET_SUCCESS if all goes well

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/08/05  By: Baldeesh S. Khaira
    - Created
Version: 1.10  Date: 15-Feb-2007  By: Jeff Fieldhouse
    - Removed the if statement that checked whether WakeEnable was true or false.
      It was causing problems when we tried to come out of hibernate. If we 
      allowed WUBA to be set, the unit would not go to power down when other
      units were continuing to communicate. Sometimes this resulted in the
      CANRMP register having all of its bits set and we then stopped receiving
      PGNs when we came out of sleep. This could probably have been solved
      by clearing the CANRMP reigster in the wakeup function, but we don't
      actually want it to wake up on bus activitity, so this makes more sense.
*******************************************************************************/
#if ( CANDRV_SLEEP_ENABLE == TRUE )
CANDRV_teRETURN CANDRV_fnSleep( uchar8 ucChannel,
                                tucBOOL tucWakeEnable )
{
    uint16 uiIndex;
    volatile uint32 ulShadowReg;

    // assert channel is 0
    assert( ucChannel == 0 );

    // enable access
    DEVICE_mACCESS_EN();

    // Read in the entire CANMC register
    ulShadowReg = ECanaRegs.CANMC.all;

    // When we are put to sleep, we do not want to wake up on bus activity, so
    // ensure the WUBA bit is not set. It actually keeps the unit from entering
    // power down properly if other units are still talking on the bus.       
    ulShadowReg &= ~F280X_CANMC_MSK_WUBA;        
    
    // Set the PDR bit to request power down
    ulShadowReg |= F280X_CANMC_MSK_PDR;
    
    // Write whole value back to the CANMC register
    ECanaRegs.CANMC.all = ulShadowReg;

    //disable access
    DEVICE_mACCESS_DIS();

    // set how long to wait until the bit changes
    uiIndex = CANDRV_MAX_TRIES;
    while( uiIndex-- != 0 )
    {
        // if the PDA bit changes, the sleep took effect, so we can return
        // success
        if( ( ECanaRegs.CANES.all & F280X_CANES_MSK_PDA ) != 0L )
        {
            return ( CANDRV_eRET_SUCCESS );
        }
    }

    // going to power down failed
    assert( FALSE );
    return( CANDRV_eRET_SLEEP_FAIL );

}
#endif // #if ( CANDRV_SLEEP_ENABLE == TRUE )

/*******************************************************************************

FUNCTION NAME:
    CANDRV_fnWakeUp

PURPOSE:
    Wake channel up from sleep

INPUTS:
    ucChannel is the number of the CAN module to wake up, usually zero.

OUTPUTS:
    CANDRV_eRET_SUCCESS if all goes well

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/08/05  By: Baldeesh S. Khaira
    - Created

*******************************************************************************/
#if ( CANDRV_WAKEUP_ENABLE == TRUE )
CANDRV_teRETURN CANDRV_fnWakeUp( uchar8 ucChannel )
{
    uint16 uiIndex;
    volatile uint32 ulShadowReg;

    // assert channel is 0
    assert( ucChannel == 0 );

    // enable access
    DEVICE_mACCESS_EN();

    // begin request for wakeup by clearing the PDR bit in CANMC register
    ulShadowReg = ECanaRegs.CANMC.all;
    ulShadowReg &= ~F280X_CANMC_MSK_PDR;
    ECanaRegs.CANMC.all = ulShadowReg;

    //disable access
    DEVICE_mACCESS_DIS();

    // set how long to wait until the bit changes
    uiIndex = CANDRV_MAX_TRIES;
    while( uiIndex-- != 0 )
    {
        // if the PDA bit is clear, we have come out of sleep, so we can return
        // success
        if( ( ECanaRegs.CANES.all & F280X_CANES_MSK_PDA ) == 0L )
        {
            return( CANDRV_eRET_SUCCESS );
        }
    }

    // going to power up failed
    assert( FALSE );
    return( CANDRV_eRET_SLEEP_FAIL );

}
#endif // #if ( CANDRV_WAKEUP_ENABLE == TRUE )

/*******************************************************************************

FUNCTION NAME:
    CANDRV_fnReceive

PURPOSE:
    Check if there is a new frame in the MSCAN module and if there is, copy
    it into the driver buffer pointed to by pRxFrame

INPUTS:
    ucChannel is the number of the CAN module to use, usually zero.
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
Version: 1.02  Date: 02/08/05  By: BaldeeshK
    - New version for 281x processor

*******************************************************************************/

CANDRV_teRETURN CANDRV_fnReceive( uchar8 ucChannel,
                                  CANDRV_tzCAN_FRAME *ptzRxFrame )
{
    uchar8 ucIndex;   // index for counting
    uchar8 ucCount;   // counter
    struct MBOX *CurrentMBox; // pointer to current mailbox
    uint16* Data;     // temporary data pointer

    // only a single channel defined
    assert( ucChannel == 0 );
    // assert frame
    assert( ptzRxFrame );

    // we have to count downwards because if data has arrived, then
    // it will put data in the highest priority mailbox, and the highest
    // priority mailbox, as defined by TI, is the mailbox with the highest
    // number.  So, if we set mailboxes 0...15 as receive mailboxes, then
    // mailbox 15 is highest priority receive mailbox
    ucIndex = CANDRV_NUM_RX_MBOXES - 1;
    do  
    {
        // if this mailbox has a message pending
        if( ( ECanaRegs.CANRMP.all & ( 1L << ucIndex ) ) != 0L )
        {
            CurrentMBox = ( ( struct MBOX* )( &ECanaMboxes.MBOX0 ) ) + ucIndex;

            // Copy the byte count (only the last four bits count)
            ptzRxFrame->ucByteCount = 
                (uchar8)( ( CurrentMBox->MSGCTRL.all & F280X_MSGCTRL_MSK_DLC ) 
                         >> F280X_MSGCTRL_POS_DLC );
            
            Data = ( uint16* )&CurrentMBox->MDL.all;

            // This mbox has a message
            // Pack the data from the registers
            for( ucCount = 0; ucCount < CANDRV_MAX_BYTES_PER_FRAME; ++ucCount )
            {
                ptzRxFrame->aucData[ ucCount++ ] = *Data & CANDRV_BYTE_MASK;
                ptzRxFrame->aucData[ ucCount ] = ( *Data >> XT_BITPOS8 ) &
                                                            CANDRV_BYTE_MASK;
                Data++;

            }

            candrv_fnHwId2DataId( &ptzRxFrame->tzCanId,
                                  &CurrentMBox->MSGID );

            // reset the ID to be all FF's so it passes the filter
            CurrentMBox->MSGID.all = CANDRV_UINT32_MAX;

            ECanaRegs.CANRMP.all = ( 1L << ucIndex );
            return ( CANDRV_eRET_SUCCESS );

        }
    // move to the next mailbox
    }while( ucIndex-- != 0 );

    return( CANDRV_eRET_FAILURE );
}

/*******************************************************************************

FUNCTION NAME:
    CANDRV_fnTransmitOK

PURPOSE:
    See if at least one transmit buffer is empty and ready for use.
    This function is used to check the availability of the hardware before
    removing a buffer from a queue.

INPUTS:
    ucChannel is the number of the CAN module to use, usually zero.

OUTPUTS:
    CANDRV_eRET_SUCCESS if at least one transmit buffer is idle
    CANDRV_eRET_TX_BUSY if no transmit buffers are available

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/13/03  By: Tom Lightfoot
    - Creation
Version: 1.01  Date: 08/26/03  By: Dalem
    - Check TFLG and TIER bytes as in CANDRV_fnTransmit
Version: 1.02  Date: 02/08/05  By: BaldeeshK
    - New version for 281x processor
Version: 1.03  Date: 02/08/05  By: BaldeeshK
    - Added check for waiting for buffer flush
Version: 1.04  Date: 03/24/05  By: BaldeeshK
    - Delcared shadow registers as volatile in order to prevent optimizations

*******************************************************************************/

CANDRV_teRETURN CANDRV_fnTransmitOk( uchar8 ucChannel )
{
    volatile uint32 ulShadowReg;

    // only a single channel defined
    assert( ucChannel == 0 );

    // check the transmit buffers
    ulShadowReg = ( ECanaRegs.CANTRS.all | CANDRV_RX_MBOX_MASK );

    // check the transmit set register
    if( ( ulShadowReg != CANDRV_UINT32_MAX ) &&
        ( candrv_tucTxWaitFlag == FALSE ) )
    {
        return( CANDRV_eRET_SUCCESS );
    }

    // All buffers are full, awaiting transmission
    return( CANDRV_eRET_TX_BUSY );
}

/*******************************************************************************

FUNCTION NAME:
    CANDRV_fnTransmit

PURPOSE:
    Transmit CAN frame to bus.

INPUTS:
    ucChannel is the number of the CAN module to use, usually zero.
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

Version: 1.00  Date: 01/27/05  By: Baldeesh S. Khaira
    - Created
Version: 1.01  Date: 02/25/05  By: Baldeesh S. Khaira
    - Modified so that it doesn't set the interrupt in this function anymore,
      it is always on by default
Version: 1.10  Date: 03/16/05  By: Baldeesh S. Khaira
    - Added priority feature so that messages are sent first in first out
Version: 1.11  Date: 03/22/05  By: Baldeesh S. Khaira
    - Added checking for wait for buffer flush, setting flag for buffer flush
      checking    
Version: 1.12  Date: 03/24/05  By: BaldeeshK
    - Delcared shadow registers as volatile in order to prevent optimizations
    
*******************************************************************************/

CANDRV_teRETURN CANDRV_fnTransmit( uchar8 ucChannel,
                                   CANDRV_tzCAN_FRAME *ptzTxFrame,
                                   uchar8 *pucHandle )
{
    uchar8 ucIndex;     // loop to determine which Mbox can transmit
    uchar8 ucCount;     // loop through the data bytes
    struct MBOX* CurrentMBox;   // pointer to current mailbox
    uint16 *Data;       // base data unit pointer
    uint32 Mask;        // 32-bit mask for the current MBox
    volatile uint32 ulShadowReg;   // shadow register, see notes on top of file

    // only a single channel defined
    assert( ucChannel == 0 );
    // assert the frame
    assert( ptzTxFrame );
    // assert the Handle
    assert( pucHandle );

    // if the buffers are empty, then we can reset our tranmit priority
    // and our wait flags
    if( candrv_fnIsTxBuffersEmpty() != FALSE )
    {
        candrv_fnResetTxPrio();
        candrv_tucTxWaitFlag = FALSE;
    }
    
    // if we are waiting for the buffer to flush out
    if( candrv_tucTxWaitFlag != FALSE )
    {
        return ( CANDRV_eRET_TX_BUSY );
    }

    // we start looking for an empty buffer at the highest priority tx mbox
    // in this module, that is the one with the highest numbered mailbox
    ucIndex = CANDRV_NUM_RX_MBOXES + CANDRV_NUM_TX_MBOXES - 1;

    do
    {
        // if the this current mailbox is not currently transmitting
        if( ( ECanaRegs.CANTRS.all & ( 1L << ucIndex ) ) == 0L )
        {
            // get the mask for this mailbox
            Mask = ( 1L << ucIndex );

            // get a pointer to the current mailbox
            CurrentMBox = ( ( struct MBOX* )( &ECanaMboxes.MBOX0 ) ) + ucIndex;

            // disable the current mailbox because we have to write to the
            // identifier area of this mailbox.  Any writes the mailbox ID
            // field requires the disabling of the mailbox
            ulShadowReg = ( ECanaRegs.CANME.all & ( ~Mask ) );
            ECanaRegs.CANME.all = ulShadowReg;

            // Copy the ID bytes
            candrv_fnDataId2HwId( &CurrentMBox->MSGID,
                                  &ptzTxFrame->tzCanId );

            // assign the pointer to current mailbox data area
            Data = ( uint16* )&CurrentMBox->MDL.all;

            // This mbox has a message
            // Pack the data from the registers
            for( ucCount = 0; ucCount < CANDRV_MAX_BYTES_PER_FRAME; ++ucCount )
            {
                *Data = ptzTxFrame->aucData[ ucCount++ ] & CANDRV_BYTE_MASK;
                *Data += ( ptzTxFrame->aucData[ ucCount ] & CANDRV_BYTE_MASK )
                                                                  << XT_BITPOS8;
                Data++;

            }

            // Copy the MSGCTRL register
            ulShadowReg = CurrentMBox->MSGCTRL.all;
            // Clear the DLC and TPL
            ulShadowReg &= ~( F280X_MSGCTRL_MSK_DLC | F280X_MSGCTRL_MSK_TPL );
            // Add in the desired byte count
            ulShadowReg |= ( ptzTxFrame->ucByteCount << F280X_MSGCTRL_POS_DLC );            

            // Add in the priority            
            ulShadowReg |= ( ( uint32 )( candrv_ucTxPrio-- & CANDRV_MAX_TX_PRIORITY ) 
                             << F280X_MSGCTRL_POS_TPL );
            
            // Write the new value to the register
            CurrentMBox->MSGCTRL.all = ulShadowReg;

            // if we reached are limit, we must let the buffer flush
            // out in order to preserve our priority
            if( candrv_ucTxPrio == 0 )
            {
                candrv_tucTxWaitFlag = TRUE;
            }                                                       

            // assign the handle to the current mailbox number
            *pucHandle = ( ucIndex - CANDRV_NUM_RX_MBOXES );

            // re-enable this mailbox
            ulShadowReg = ECanaRegs.CANME.all | Mask;
            ECanaRegs.CANME.all = ECanaRegs.CANME.all | Mask;//ulShadowReg;

            // allow register access
            DEVICE_mACCESS_EN();

            // clear the transmit acknowledge bit by setting it to 1
            ECanaRegs.CANTA.all = Mask;

            // start transmission by setting the Transmission Request register
            // tranmission does not occur until the corresponding bit
            // representing the appropriate mailbox is set
            ulShadowReg = ECanaRegs.CANTRS.all | Mask;
            ECanaRegs.CANTRS.all = ulShadowReg;

            // disallow register access
            DEVICE_mACCESS_DIS();

            // return sucess
            return ( CANDRV_eRET_SUCCESS );
        }

     // while our index reached the receive mailbox area
    }while( ucIndex-- != CANDRV_NUM_RX_MBOXES );


    // nothing available, CAN controller is busy
    return( CANDRV_eRET_TX_BUSY );
}



/*******************************************************************************

FUNCTION NAME:
    CANDRV_fnAbort

PURPOSE:
    Abort a transmitted frame

INPUTS:
    ucChannel is the number of the CAN module to use, usually zero.
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
Version: 1.03  Date: 02/08/05  By: BaldeeshK
    - New version for 281x processor
Version: 1.04  Date: 02/24/05  By: BaldeeshK
    - Modified assert to use index instead of mbox number

*******************************************************************************/
#if ( CANDRV_ABORT_ENABLE == TRUE )
CANDRV_teRETURN CANDRV_fnAbort( uchar8 ucChannel,
                                uchar8 ucHandle )
{

    // only a single channel defined
    assert( ucChannel == 0 );
    assert( ucHandle < CANDRV_NUM_TX_MBOXES );

    // only abort if the message hasn't been sent yet, so make sure
    // that their is a transmit still pending
    if( ( ECanaRegs.CANTRS.all & ( 1L << ( ucHandle + CANDRV_NUM_RX_MBOXES ) ) ) != 0L )
    {
        ECanaRegs.CANAA.all |= ( 1L << ( ucHandle + CANDRV_NUM_RX_MBOXES ) );
    }

    return( CANDRV_eRET_SUCCESS );
}
#endif



/*******************************************************************************

FUNCTION NAME:
    CANDRV_fnTxStatus

PURPOSE:
    Get transmitter status and optional error count

INPUTS:
    ucChannel is the number of the CAN module to use, usually zero.
    pErrCount points to where to send the error count.  If NULL, it won't be
    assigned.

OUTPUTS:
    An enum of the transmitter's error status.

NOTES:
    Status errors are the same for transmit and receive

VERSION HISTORY:

Version: 1.00  Date: 02/12/03  By: Tom Lightfoot
    - Creation
Version: 1.01  Date: 05/27/03  By: Tom Lightfoot
    - Used the right enum type when channel number out of range
Version: 1.02  Date: 08/01/03  By: dalem
    - Use merged bits for tx status from hardware
Version: 1.03  Date: 02/08/05  By: BaldeeshK
    - New version for 281x processor

*******************************************************************************/
#if ( CANDRV_TX_STATUS_ENABLE == TRUE )
CANDRV_teSTATUS CANDRV_fnTxStatus( uchar8 ucChannel,
                                   uchar8 *pucErrCount )
{    
    assert( ucChannel == 0 );

    if( pucErrCount != NULL )
    {
        // read and store the tranmit error count
        *pucErrCount = ( uchar8 )( ECanaRegs.CANTEC.all & F280X_CANTEC_MSK_TEC );
    }
    return ( candrv_fnGetError() );
}
#endif // #if ( CANDRV_TX_STATUS_ENABLE == TRUE )

/*******************************************************************************

FUNCTION NAME:
    CANDRV_fnRxStatus

PURPOSE:
    Get receiver status and optional error count

INPUTS:
    ucChannel is the number of the CAN module to use, usually zero.
    pucErrCount points to where to send the error count.  If NULL, it won't be
    assigned

OUTPUTS:
    An enum of the receiver's error status.

NOTES:
    Status errors are the same for transmit and receive

VERSION HISTORY:

Version: 1.00  Date: 02/12/03  By: Tom Lightfoot
    - Creation
Version: 1.01  Date: 05/27/03  By: Tom Lightfoot
    - Used the right enum type when channel number out of range
Version: 1.02  Date: 08/01/03  By: dalem
    - Use merged bits for rx status from hardware
Version: 1.03  Date: 02/08/05  By: BaldeeshK
    - New version for 281x processor

*******************************************************************************/
#if ( CANDRV_RX_STATUS_ENABLE == TRUE )
CANDRV_teSTATUS CANDRV_fnRxStatus( uchar8 ucChannel,
                                   uchar8 *pucErrCount )
{
    assert( ucChannel == 0 );

    if( pucErrCount != NULL )
    {
        // read and store the receive error count        
        *pucErrCount = ( uchar8 )( ECanaRegs.CANREC.all & F280X_CANREC_MSK_REC );
    }
    return ( candrv_fnGetError() );

}
#endif // #if ( CANDRV_RX_STATUS_ENABLE == TRUE )

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
Version: 1.01  Date: 01/20/05  By: Baldeesh S. Khaira
    - New version for TI 281x processor

*******************************************************************************/

void CANDRV_fnInitCbList( void )
{
    uchar8 Index;   // Counts through the callback list

    // assign each callback a value of NULL
    for( Index = 0; Index < CANDRV_eCB_LAST; Index++ )
    {
        candrv_apCallback[ Index ] = NULL;
    }
}

/*******************************************************************************

FUNCTION NAME:
    CANDRV_fnInstallCb

PURPOSE:
    Install callbacks for handling interrupts

INPUTS:
    ucChannel is the number of the CAN module to use, usually zero.
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
Version: 1.02  Date: 01/20/05  By: Baldeesh S. Khaira
    - New version for TI 281x processor

*******************************************************************************/

CANDRV_teRETURN CANDRV_fnInstallCb( uchar8 ucChannel,
                                    CANDRV_teCB_TYPE CbType,
                                    CANDRV_tpfnCALLBACK tpfnFunc )
{
    assert ( ucChannel == 0 );
    assert( tpfnFunc );

    // Make sure this is a valid callback type
    if( CbType >= CANDRV_eCB_LAST )
    {
        // Index out of range
        return( CANDRV_eRET_FAILURE );
    }

    // Install the pointer in the list
    candrv_apCallback[ CbType ] = tpfnFunc;

    // Complete
    return( CANDRV_eRET_SUCCESS );
}

/*******************************************************************************

FUNCTION NAME:
    CANDRV_fnGetHandle

PURPOSE:
    Get the handle of the latest transmitted buffer

INPUTS:
    ucChannel is the number of the CAN module to use, usually zero.

OUTPUTS:
    Handle of the latest transmitted buffer

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/18/05  By: Baldeesh S. Khaira
    - Created

*******************************************************************************/
#if ( CANDRV_GET_HANDLE_ENABLE == TRUE )
uchar8 CANDRV_fnGetHandle( uchar8 ucChannel )
{
    // only channel 0 is valid
    assert( ucChannel == 0 );

    // return the Handle
    return ( candrv_ucHandle );
}
#endif

/*******************************************************************************

FUNCTION NAME:
    candrv_fnInitMBoxes

PURPOSE:
    Initialize the mailbox control registers

INPUTS:
    pzCan points to the MSCAN module being initialized
    FilterMode indicates if the filter is to be set to pass everything or nothing

OUTPUTS:
    CANDRV_eSUCCESS if all goes well

NOTES:
    This function is only to be called from CAN_fnInit

VERSION HISTORY:

Version: 1.00  Date: 02/14/04  By: Baldeesh Khaira
    - Created

Version: 1.01  Date: 03/17/05  By: Baldeesh Khaira
    - Initialized MSGCTRL field to all zeroes as required by the Enhanced
      CAN reference guide

*******************************************************************************/

static tuiSTATUS candrv_fnInitMBoxes( CANDRV_teFILTER_MODE FilterMode )
{
    uchar8 ucIndex; // Index for counting
    uint32 Mask;    // Acceptance mask for the filters, either all ones
                    // for accepting anything, or all zeros
    union CANLAM_REG * LamRegs = ( union CANLAM_REG * )&ECanaLAMRegs.LAM0;
                                                // pointer to first acceptance
                                                // mask register
    struct MBOX *  CurrentMBox = ( struct MBOX * )&ECanaMboxes.MBOX0;
                                                     // pointer to first mailbox
    // disable the mailboxes before writing to them
    ECanaRegs.CANME.all = 0L;

    if( FilterMode == CANDRV_eFILTER_PASS_ALL )
    {
        // Set all mask bytes to 0xFF to allow all frames to be received
        // (A 1 in the mask means don't care)
        Mask = CANDRV_ACCEPT_ALL;
    }
    else if( FilterMode == CANDRV_eFILTER_PASS_NONE )
    {
        // Set all mask bytes to 0x00 to not pass any frames
        Mask = CANDRV_ACCEPT_NONE;
    }
    else
    {
        // Invalid filter parameter
        return( eSTATUS_ERR );
    }

    // set the first CANDRV_NUM_RX_MBOXES to be RX mailboxes, and the rest
    // to be TX mailboxes
    // This sets the first CANDRV_NUM_RX_MBOXES number of bits in the register
    // Note:  Setting it to 1 makes it a receive mailbox, setting it to 0 makes
    //        it a transmit mailbox
    ECanaRegs.CANMD.all = ( ( 1L << CANDRV_NUM_RX_MBOXES ) - 1 );

    // set all mailboxes to not overwrite/enable overwrite protection
    ECanaRegs.CANOPC.all = CANDRV_UINT32_MAX;

    // loop through the mailboxes
    for( ucIndex = 0; ucIndex < CANDRV_TOTAL_NUM_MBOXES; ucIndex++ )
    {

        // initialize the mailbox parameters
        CurrentMBox->MSGCTRL.all = 0L;
        CurrentMBox->MSGID.all = Mask;

        LamRegs->all = Mask;    // assign the mask
        LamRegs++;              // increment the acceptance mask pointer
        CurrentMBox++;          // increment the mailbox pointer

    }

    return ( eSTATUS_OK );
}

/*******************************************************************************

FUNCTION NAME:
    candrv_fnInitInterrupts

PURPOSE:
    Initialize the eCAN interrupt registers

INPUTS:
    None

OUTPUTS:
    none

NOTES:
    This function is only to be called from CAN_fnInit

VERSION HISTORY:

Version: 1.00  Date: 01/24/05  By: Baldeesh S. Khaira
    - Created

Version: 1.01  Date: 02/24/05  By: Baldeesh S. Khaira
    - Now setting all interrupt in this function, instead of only receive ones

Version: 1.10    Date: March 21, 2005  By: John Bellini
    - Added initialization of ISR to the vector table.
    
Version: 1.11  Date: 03/22/05  By: Baldeesh S. Khaira
    - Removed device enable/disable
    
Version: 1.12  Date: 08-Jan-2007  By: Jeff Fieldhouse 
    - Changed so that it only accesses CAN regs as 32-bit register   
        
*******************************************************************************/

static void candrv_fnInitInterrupts( void )
{    
    // The following interrupts are enabled as part of this instruction:
    // Abort Acknowledge, Wake up, Receive message lost, bus-off, error passive,
    // Warning Level, and the Global interrupt level is mapped to ECAN0INT   
    ECanaRegs.CANGIM.all = ( ( 0L << F280X_CANGIM_POS_MTOM  )    // Disab
                           | ( 0L << F280X_CANGIM_POS_TCOM  )    // Disab
                           | ( 1L << F280X_CANGIM_POS_AAIM  )    // Enab
                           | ( 0L << F280X_CANGIM_POS_WDIM  )    // Disab
                           | ( 1L << F280X_CANGIM_POS_WUIM  )    // Enab
                           | ( 1L << F280X_CANGIM_POS_RMLIM )    // Enab
                           | ( 1L << F280X_CANGIM_POS_BOIM  )    // Enab
                           | ( 1L << F280X_CANGIM_POS_EPIM  )    // Enab
                           | ( 1L << F280X_CANGIM_POS_WLIM  )    // Enab
                           | ( 0L << F280X_CANGIM_POS_GIL   )    // Map to ECAN0INT
                           | ( 0L << F280X_CANGIM_POS_I1EN  )    // Disab
                           | ( 0L << F280X_CANGIM_POS_I0EN  ) ); // Disab

    // route the receive mailboxes to  ECAN1INT and all transmit interrupts
    // to ECAN0INT
    ECanaRegs.CANMIL.all = ( ( 1L << CANDRV_NUM_RX_MBOXES ) - 1L );

    // enable all the mailbox interrupts
    ECanaRegs.CANMIM.all = ( CANDRV_UINT32_MAX );

    // Initialize interrupt vectors
    //
    PieVectTable.ECAN0INTA= &ISR_fnECAN0INT;
    PieVectTable.ECAN1INTA= &ISR_fnECAN1INT;
}

/*******************************************************************************

FUNCTION NAME:
    candrv_fnDataId2HwId

PURPOSE:
    Copy the package header ID to the ID format found in the mailbox

INPUTS:
    ptzDestId points to an ID field in the mailbox
    puSrcId points to an id field in the format the driver uses

OUTPUTS:
    Nothing

NOTES:
    This function is used both for sending frames as well as setting
    hardware filtering parameters.

VERSION HISTORY:

Version: 1.00  Date: 01/24/05  By: Baldeesh S. Khaira
    - Creation

Version: 1.01  Date: 02/15/05  By: Baldeesh S. Khaira
    - Bug fix when extracting the bits for EXTMSGID_H.  Previously, it was
      masking with XT_BITPOS2, when it should have been XT_BIT2.

*******************************************************************************/

static void candrv_fnDataId2HwId( union CANMSGID_REG*    puDestId,
                                  CANDRV_tzCAN_ID *ptzSrcId )
{
    assert( puDestId && ptzSrcId );

    // Note: It's ok to use the .bit fields for the MSGID registers as they
    // can be accessed as 8-bit, 16-bit, or 32-bit registers. They do not
    // have the same restriction as the CAN control and status registers
    
    // first 16 bits of ID
    puDestId->bit.EXTMSGID_L = ptzSrcId->ucId07_00 |
                               ( ptzSrcId->ucId15_8 << XT_BITPOS8 );

    // next 2 bits
    puDestId->bit.EXTMSGID_H = ( ptzSrcId->ucId23_16 & ( XT_BIT2 - 1 ) );

    // the starndard part of the extended ID message
    puDestId->bit.STDMSGID = ( ptzSrcId->ucId23_16 >> XT_BITPOS2 ) |
                             ( ptzSrcId->ucId28_24 << XT_BITPOS6 );

    // IDE bit
    puDestId->bit.IDE = ( ( ptzSrcId->ucId28_24 & CANDRV_ID_MSK_IDE ) != 0 )
                                                                    ? 1 : 0;
    // these next two aren't user modifiable
    puDestId->bit.AME = 1;
    puDestId->bit.AAM = 0;
}

/*******************************************************************************

FUNCTION NAME:
    candrv_fnHwId2DataId

PURPOSE:
    Copy a  mailbox ID to the ID found in the package header file.  Some
    bit shifting and manipulation is required for this.

INPUTS:
    ptzDestId points to an id field in the format the driver uses
    puSrcId points to an ID field in the mailbox

OUTPUTS:
    Nothing

NOTES:
    This function is used for receiving frames.

VERSION HISTORY:

Version: 1.00  Date: 01/24/05  By: Baldeesh S. Khaira
    - Created

*******************************************************************************/

static void candrv_fnHwId2DataId( CANDRV_tzCAN_ID *ptzDestId,
                                  union CANMSGID_REG*  puSrcId )
{
    assert( ptzDestId && puSrcId );

    // First part of the ID masked out
    ptzDestId->ucId07_00 = ( puSrcId->bit.EXTMSGID_L & ( XT_BIT8 - 1 ) );

    // Second part of the ID
    ptzDestId->ucId15_8 = ( ( puSrcId->bit.EXTMSGID_L & ( CANDRV_UINT16_MAX ) )
                                                                >> XT_BITPOS8 );

    // Third part of the ID
    ptzDestId->ucId23_16 = ( puSrcId->bit.EXTMSGID_H ) |
                           ( ( puSrcId->bit.STDMSGID & ( XT_BIT6 - 1 ) )
                                                            << XT_BITPOS2 );

    // Fourth part of the ID
    ptzDestId->ucId28_24 = ( puSrcId->bit.STDMSGID >> XT_BITPOS6 );

    // if this frame has an extended identifier, then add the IDE bit to the
    // most significant byte of the CAN frame ID
    if( puSrcId->bit.IDE == 1 )
    {
        ptzDestId->ucId28_24 |= CANDRV_ID_MSK_IDE;
    }


}

/*******************************************************************************

FUNCTION NAME:
    candrv_fnGetError

PURPOSE:
    Get the bus error conditions

INPUTS:
    None

OUTPUTS:
    An enum of the transmitter or receiver status.

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/12/03  By: Tom Lightfoot
    - Creation
Version: 1.01  Date: 02/20/03  By: Tom Lightfoot
    - Fixed case numbers which were written in binary but interpreted in hex
Version: 1.02  Date: 01/24/05  By: Baldeesh S. Khaira
    - New version for 281x processor

*******************************************************************************/
#if ( ( CANDRV_TX_STATUS_ENABLE == TRUE ) || ( CANDRV_RX_STATUS_ENABLE == TRUE ) )
static CANDRV_teSTATUS candrv_fnGetError( void )
{ 
    // bus off error gets highest priority
    if( ( ECanaRegs.CANES.all & F280X_CANES_MSK_BO ) != 0L )
    {
        // Clear the BO bit
        ECanaRegs.CANES.all |= F280X_CANES_MSK_BO;
        
        return ( CANDRV_eSTATUS_BUSOFF );
    }
    // check for any other error bits
    else if( ( ECanaRegs.CANES.all & CANDRV_ERROR_STATUS_BITS ) != 0L )
    {
        // Clear the other error bits
        ECanaRegs.CANES.all |= CANDRV_ERROR_STATUS_BITS;        
        
        return ( CANDRV_eSTATUS_ERROR );
    }
    // check for the warning bits
    else if( ( ECanaRegs.CANES.all & F280X_CANES_MSK_EW ) != 0L )
    {
        // Clear the warning bit
        ECanaRegs.CANES.all |= F280X_CANES_MSK_EW;        
        
        return ( CANDRV_eSTATUS_WARNING );
    }

    // no errors or warnings
    return( CANDRV_eSTATUS_OK );
}
#endif // #if ( ( CANDRV_TX_STATUS_ENABLE == TRUE ) || ( CANDRV_RX_STATUS_ENABLE == TRUE ) )


/*******************************************************************************

FUNCTION NAME:
    candrv_fnIsTxBuffersEmpty

PURPOSE:
    Check to see if the transmit buffers are empty
INPUTS:

OUTPUTS:
    TRUE - buffer is empty
    FALSE - buffer is not empty

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/16/05  By: Baldeesh S. Khaira
    - Created
Version: 1.01  Date: 03/24/05  By: BaldeeshK
    - Delcared shadow registers as volatile in order to prevent optimizations

*******************************************************************************/
tucBOOL candrv_fnIsTxBuffersEmpty( void )
{
    volatile uint32 ulShadowReg;

    // check the transmit buffers
    ulShadowReg = ( ECanaRegs.CANTRS.all & ( ~CANDRV_RX_MBOX_MASK ) );

    // check the transmit set register
    if( ulShadowReg == 0L )
    {
        // buffer is empty
        return( TRUE );
    }

    // buffer is not empty
    return( FALSE );
}


/*******************************************************************************

FUNCTION NAME:
    candrv_fnResetTxPrio

PURPOSE:
    Reset the transmit priority

INPUTS:
   none

OUTPUTS:
   none

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/16/05  By: Baldeesh S. Khaira
    - Created

*******************************************************************************/
void candrv_fnResetTxPrio( void )
{
    candrv_ucTxPrio = CANDRV_MAX_TX_PRIORITY;
}

/*******************************************************************************

FUNCTION NAME:
    CANDRV_fnIsTransmitting

PURPOSE:
    Check if we have a successful transmision after a state change of transmitter

INPUTS:
    Channel is the number of the CAN module to use,
    
OUTPUTS:
    True if the transmitter is transmiting the frames successfully,
    False otherwise

NOTES:
    

VERSION HISTORY:

Version: 1.00  Date: 11/07/08  By: Holly Zhou
    - Creation

*******************************************************************************/

tucBOOL CANDRV_fnIsTransmitting( uchar8 Channel )
{
    if( Channel >= CANDRV_MAX_CHANNELS )
    {
        //  Index out of range
        return( FALSE );
    }

    return( ( tucBOOL )tzCanState[ Channel ].ucTransmitting );	
}
/*******************************************************************************

FUNCTION NAME:
    CANDRV_fnResetCount

PURPOSE:
    Clear the statistics counter for the CAN controller

INPUTS:
    Channel is the number of the CAN module to use,
    
OUTPUTS:
    None

NOTES:
    

VERSION HISTORY:

Version: 1.00  Date: 11/07/08  By: Holly Zhou
    - Creation

*******************************************************************************/
void CANDRV_fnResetCount( uchar8 Channel )
{
    if( Channel >= CANDRV_MAX_CHANNELS )
    {
        //  Index out of range
        return;
    }
         
     tzCanState[ Channel ].ucTxStateChange = 0; 
     tzCanState[ Channel ].ucRxStateChange = 0;       
}
/*==============================================================================
                           Interrupt Routines
==============================================================================*/


/*******************************************************************************

FUNCTION NAME:
    CANDRV_fnECAN0INTIsr

PURPOSE:
    General purpose interrupt routine for transmit buffer empty.
    This function determines if the buffer is empty because of a successful
    transmission or because of an aborted frame and call either the
    call back for transmit OK or abort as appropriate.
    The interrupt for the empty buffer is then disabled.

INPUTS:
    ucChannel is the number of the CAN module to use, usually zero.

OUTPUTS:
    None

NOTES:
    This function is called from the transmit interrupt routines of
    all channels.

VERSION HISTORY:

Version: 1.00  Date: 01/31/05  By: Baldeesh S. Khaira
    - Created
Version: 1.01  Date: 02/17/05  By: Baldeesh S. Khaira
    - Fixed text error with a misplaced line
Version: 1.02  Date: 02/24/05  By: Baldeesh S. Khaira
    - Moved storing of handle to before clearing of CANTA, fixed bug with
      clearing of CANTA
Version: 1.03  Date: 03/22/05  By: Baldeesh S. Khaira
    - Modified so that we are only calling transmit callbacks if we aren't
      waiting for the buffers to flush out
Version: 1.04  Date: 03/24/05  By: BaldeeshK
    - Delcared shadow registers as volatile in order to prevent optimizations

*******************************************************************************/

void CANDRV_fnECAN0INTIsr( void )
{
    volatile uint32 ulShadowReg;
    CANDRV_teCB_TYPE Reason;    // Reason for interrupt, indicating which callback

    // This interrupt came from transmit mailbox
    if( ( ECanaRegs.CANGIF0.all & F280X_CANGIF_MSK_GMIF ) != 0L )
    {
#if ( CANDRV_GET_HANDLE_ENABLE == TRUE )
        // store the handle, before it gets cleared by clearing tx acknowledge
        candrv_ucHandle = ( ( ECanaRegs.CANGIF0.all & F280X_CANGIF_MSK_MIV ) 
                            - CANDRV_NUM_RX_MBOXES );
#endif        

        // assign the bit to be cleared
        ulShadowReg = ( 1L << ( ECanaRegs.CANGIF0.all & F280X_CANGIF_MSK_MIV ) ); 

        // clear the transmit acknowledge bit
        ECanaRegs.CANTA.all = ulShadowReg;

        // check to see if the buffers are empty
        if( candrv_fnIsTxBuffersEmpty() != FALSE )
        {
            candrv_tucTxWaitFlag = FALSE;
        }

        // assign the reason for the interrupt
        Reason = CANDRV_eCB_TXOK;

		       
        if( tzCanState[ 0 ].ucTransmitting == FALSE )
        {
            tzCanState[ 0 ].ucTransmitting = TRUE;
        }       
    }
    else
    {
        // Was the frame aborted?
        if( ( ECanaRegs.CANGIF0.all & F280X_CANGIF_MSK_AAIF ) != 0L )
        {
            ulShadowReg = F280X_CANGIF_MSK_AAIF;
            Reason = CANDRV_eCB_TXABORT;
        }
        // interrupt cause by wakeup
        else if( ( ECanaRegs.CANGIF0.all & F280X_CANGIF_MSK_WUIF ) != 0L )
        {
            ulShadowReg = F280X_CANGIF_MSK_WUIF;
            Reason = CANDRV_eCB_WAKEUP;
        }
        // interrupt caused by receive message overflow
        else if( ( ECanaRegs.CANGIF0.all & F280X_CANGIF_MSK_RMLIF ) != 0L )
        {
            ulShadowReg = F280X_CANGIF_MSK_RMLIF;
            Reason = CANDRV_eCB_OVERFLOW;	
        }
        else
        {
            // misecllaneous status problem occured, ie bus off, error passive
            ulShadowReg = CANDRV_STATUS_MASK_BITS;
            Reason = CANDRV_eCB_STATUS;	
            tzCanState[ 0 ].ucTransmitting = FALSE; 
        }

        ECanaRegs.CANGIF0.all = ulShadowReg;
    }

    // Call the function if there is a function
    if( candrv_apCallback[ Reason ] != NULL )
    {
        // call the function with channel 0
        candrv_apCallback[ Reason ]( 0 );
    }

    // set the appropriate acknoledge bit in the pie vector table
    PieCtrlRegs.PIEACK.bit.ACK9 = 1;        // Issue PIE ACK
}

/*******************************************************************************

FUNCTION NAME:
    CANDRV_fnECAN1INTIsr

PURPOSE:
    Interrupt routine for CAN interrupt line 1.  This is a higher priority
    interrupt line than interrupt line 0.  This driver dedicates interrupt
    line 1 to be used for receive messages only.  Hence, only receive
    messages will be routed to this routine.

INPUTS:

OUTPUTS:
    None

NOTES:

VERSION HISTORY:
Version: 1.00  Date: 01/31/05  By: Baldeesh S. Khaira
    - Created

*******************************************************************************/

void CANDRV_fnECAN1INTIsr( void )
{
    // Call the function if there is a function
    if( candrv_apCallback[ CANDRV_eCB_RECEIVE ] != NULL )
    {
        candrv_apCallback[ CANDRV_eCB_RECEIVE ]( 0 );
    }

    // set the appropriate acknoledge bit in the pie vector table
    PieCtrlRegs.PIEACK.bit.ACK9 = 1;        // Issue PIE ACK
}

