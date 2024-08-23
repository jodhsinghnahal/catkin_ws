/*==============================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:
    F28X.h

PURPOSE:
    The purpose of this file is to provide a central repository for all of the
    bit field functionality #defines for the registers on the c28x.  For the
    c24x these were all stored in the LF2407.H file, but since such a file
    does not exist for the c28x, a new file (this one) was created to hold them.

FUNCTION(S):
    None

NOTES:

    ---------------------------------------------------------------------------
    What on earth is a "bit field functionality #define"?
    ---------------------------------------------------------------------------
    For each register each bit field has a set of different options, some are
    just simple 0 or 1 (on / off - enabled / disabled ) but some are more
    complex.  These bit fields represent the different possible options that
    can be assigned to the particular register bits.
    ---------------------------------------------------------------------------

    ---------------------------------------------------------------------------
    How do I add new defines to this file?
    ---------------------------------------------------------------------------
    First, check to ensure that someone hasn't already defined what you're
    trying to define with a slightly different name.  The file is organized
    first by register, then by function.  If you're adding a register
    state value make sure that you put it with that register.  If you're
    adding a new register, make sure you follow the same commenting practices
    shown in the rest of the file.

    In general the format of the define is as follows:

    #define F28X_<REGISTER>_<BIT FIELD>_<FUNCTION>       <VALUE>

    Exceptions to this formatting are acceptable provided they make sense. For
    example, there are identical registers on both Event Manager A and B so
    therefore a person could just say EVX instead of EVA or EVB
    ---------------------------------------------------------------------------

CHANGE HISTORY:
$Log: F28x.h $

    ***********************************************
    Revision: NovaPfrmB_JohnB/1
    User: JohnB     Date: 02/24/05  Time: 06:42PM
    Rebuild of calc.lib and ucossii.lib.

    

    ***********************************************
    Revision: NovaPfrmB_Local_HorstM/2
    User: HorstM     Date: 02/22/05  Time: 11:27PM
    added defines for EVxIFR's



    ***********************************************
    Revision: NovaPfrmB_TrevorM/3
    User: TrevorM     Date: 02/18/05  Time: 09:06PM
    Added correct values to the Flash Bank wait state definitions


    ***********************************************
    Revision: NovaPfrmB_TrevorM/2
    User: TrevorM     Date: 02/18/05  Time: 05:32PM
    merged


    ***********************************************
    Revision: NovaPfrmB_Local_HorstM/1
    User: HorstM     Date: 02/18/05  Time: 12:51AM
    added more timer defines


    ***********************************************
    Revision: NovaPfrmB_TrevorM/1
    User: TrevorM     Date: 02/09/05  Time: 03:37PM
    Added support for External Memory Interface bit field
    functionality #defines


    ***********************************************
    Revision: NovaPfrmB_HorstM/3
    User: HorstM     Date: 12/15/04  Time: 08:30PM
    fixed header text


    ***********************************************
    Revision: NovaPfrmB_HorstM/2
    User: HorstM     Date: 12/15/04  Time: 08:28PM
    added new defines for PWM related registers;
    renamed a couple of 'orphans' to join related groups


    ***********************************************
    Revision: NovaPfrmB_HorstM/1
    User: HorstM     Date: 12/07/04  Time: 10:41PM
    added defines for timer driver


    *****************  Version 5  *****************
    User: Horstm       Date: 12/03/04   Time: 2:58p
    Updated in $/PD/Common/Targets/TI/c28x/Inc
    added defines for timer driver

    *****************  Version 4  *****************
    User: Hollyz       Date: 11/18/04   Time: 2:48p
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/Baseline/C28x/Inc
    Added more definitions for SCI registers

    *****************  Version 3  *****************
    User: Brentt       Date: 6/23/04    Time: 9:17a
    Updated in $/PD/Common/Targets/TI/c28x/Inc
    - added new defines for the ADC control registers.

    *****************  Version 2  *****************
    User: Brentt       Date: 6/18/04    Time: 8:56a
    Updated in $/PD/Common/Targets/TI/c28x/Inc
    - Fixed some errors with the define names
    - Fixed some macros that had arguments that were missing the file
    header prefix (F28X_)

    *****************  Version 1  *****************
    User: Brentt       Date: 6/17/04    Time: 2:56p
    Created in $/PD/Inverter_Charger/Nova_Series/Code/Baseline/C28x/Inc
    First Check-In into VSS

==============================================================================*/

#ifndef F28X_H
#define F28X_H


/*==============================================================================
                              Includes
==============================================================================*/


/*==============================================================================
                              Defines
==============================================================================*/

// ---------------------------------------------------------------------------
// Defines based on device registers.
// ---------------------------------------------------------------------------

//
// Peripheral Clock Register (PCLKCR)
//
#define F28X_PCLKCR_CLK_OFF                      ( 0x00 ) // ALL BITS: Disables (shuts off) the clock
#define F28X_PCLKCR_CLK_ON                      ( 0x01 ) // ALL BITS: Enables (turns on) the clock

//
// WDT Control Register (WDCR)
//
#define F28X_WDCR_WDT_RST_FLAG_CLR              ( 1 << XT_BITPOS7 ) // clears WDT reset flag
#define F28X_WDCR_WDT_ENABLE_TOGGLE             ( 1 << XT_BITPOS6 ) // disables the WDT
#define F28X_WDCR_WDT_EN                        ( 0 << XT_BITPOS6 ) // enables the WDT

// this value must be written to this field anytime that a write is made to this register
// otherwise the system explodes! well not really, it just resets if the WD is enabled
#define F28X_WDCR_WRITE_OR_DIE                  ( 5 << XT_BITPOS3 )

// these bits configure the watchdog counter clock (WDCLK) rate relative to OSCCLK/512
#define F28X_WDCR_WDCLK_OSCCLK_DIV_1            ( 1 << XT_BITPOS0 )
#define F28X_WDCR_WDCLK_OSCCLK_DIV_2            ( 2 << XT_BITPOS0 )
#define F28X_WDCR_WDCLK_OSCCLK_DIV_4            ( 3 << XT_BITPOS0 )
#define F28X_WDCR_WDCLK_OSCCLK_DIV_8            ( 4 << XT_BITPOS0 )
#define F28X_WDCR_WDCLK_OSCCLK_DIV_16           ( 5 << XT_BITPOS0 )
#define F28X_WDCR_WDCLK_OSCCLK_DIV_32           ( 6 << XT_BITPOS0 )
#define F28X_WDCR_WDCLK_OSCCLK_DIV_64           ( 7 << XT_BITPOS0 )

// set up bit field for Watch Dog Control Register (WDCR)
#define F28X_mWDCR_INIT  ( F28X_WDCR_WDT_RST_FLAG_CLR    \
                    | F28X_WDCR_WDT_ENABLE_TOGGLE        \
                    | F28X_WDCR_WRITE_OR_DIE             \
                    | F28X_WDCR_WDCLK_OSCCLK_DIV_64      \
                    )

//
// High/Low Speed Peripheral Clock Prescalar Register (LOSPCP/HISPCP)
//
#define F28X_XXSPCP_XXPCLK_SYSCLKOUT_DIV_1      ( 0x00 )
#define F28X_XXSPCP_XXPCLK_SYSCLKOUT_DIV_2      ( 0x01 )
#define F28X_XXSPCP_XXPCLK_SYSCLKOUT_DIV_4      ( 0x02 )
#define F28X_XXSPCP_XXPCLK_SYSCLKOUT_DIV_6      ( 0x03 )
#define F28X_XXSPCP_XXPCLK_SYSCLKOUT_DIV_8      ( 0x04 )
#define F28X_XXSPCP_XXPCLK_SYSCLKOUT_DIV_10     ( 0x05 )
#define F28X_XXSPCP_XXPCLK_SYSCLKOUT_DIV_12     ( 0x06 )
#define F28X_XXSPCP_XXPCLK_SYSCLKOUT_DIV_14     ( 0x07 )

//
// System Control and Status Register (SCSR)
//
#define F28X_SCSR_WDINTS                        ( XT_BITPOS2 ) // WDT INT Status
#define F28X_SCSR_WDENINT                       ( XT_BITPOS1 ) // WDT INT enable
#define F28X_SCSR_WDOVERRIDE                    ( XT_BITPOS0 ) // WDT Override

#define F28X_SCSR_WDENINT_INT                   ( 1 << F28X_SCSR_WDENINT )    // WDT INT out enabled
#define F28X_SCSR_WDENINT_RST                   ( 0 << F28X_SCSR_WDENINT )    // WDT RST out enabled
#define F28X_SCSR_WDOVERRIDE_TOG                ( 1 << F28X_SCSR_WDOVERRIDE ) // WDT Override bit Toggle

// since the reset state of WDOVERRIDE is 1, and since writing a 1 to that bit
// will toggle it and thus disallow us access to the hardware WDT disable, we
// must only set the choice of output for the watchdog output.
#define F28X_mSCSR_INIT                         ( F28X_SCSR_WDENINT_RST )

//
// Low-power Mode Register 0 (LPMCR0)
//
#define F28X_LPMCR0_LPM_LPM_IDLE                ( 0x00 )
#define F28X_LPMCR0_LPM_LPM_STANDBY             ( 0x01 )
#define F28X_LPMCR0_LPM_LPM_HALT                ( 0x02 )

//
// Low-power Mode Register 1 (LPMCR1)
//
#define F28X_LPMCR1_NO_STANDBY_INTS             ( 0x00 )

//
// Event Manager - General Purpose Timer Control Register (GTPCONA/GPTCONB)
//
#define F28X_GPTCONX_TXCTRIPE_DIS               ( 0x00 )
#define F28X_GPTCONX_TXCTRIPE_EN                ( 0x01 )
#define F28X_GPTCONX_TXTOADC_DIS                ( 0x00 )
#define F28X_GPTCONX_TXTOADC_UND_INT            ( 0x01 )
#define F28X_GPTCONX_TXTOADC_PER_INT            ( 0x02 )
#define F28X_GPTCONX_TXTOADC_CMP_INT            ( 0x03 )
#define F28X_GPTCONX_TCMPOE_HI_Z                ( 0x00 )
#define F28X_GPTCONX_TCMPOE_TMR_DRVN            ( 0x01 )
#define F28X_GPTCONX_TCMPOE_FORCE_LOW           ( 0x00 )
#define F28X_GPTCONX_TCMPOE_ACTV_LOW            ( 0x01 )
#define F28X_GPTCONX_TCMPOE_ACTV_HI             ( 0x02 )
#define F28X_GPTCONX_TCMPOE_FORCE_HI            ( 0x03 )

//
// Event Manager - Timer X Control Register (T1CON/T2CON/T3CON/T4CON)
//
#define F28X_TXCON_TENABLE_FALSE                ( 0x00 )
#define F28X_TXCON_TENABLE_TRUE                 ( 0x01 )

//
// Compare X Control Register (CONCOMA/CONCOMB)
//
#define F28X_COMCONX_CENABLE_FALSE              ( 0x00 )
#define F28X_COMCONX_CENABLE_TRUE               ( 0x01 )

//
// Dead Band Timer X Register - (DBTCONA/DBTCONB)
//
#define F28X_DBTCONX_DBT_DIS                    ( 0x00 )
#define F28X_DBTCONX_DBT_EN                     ( 0x01 )

//
// Capture X Control Register (CAPCONA/CAPCONB)
//
#define F28X_CAPCONX_CAP_1_2_DIS                ( 0x00 )
#define F28X_CAPCONX_CAP_1_2_EN                 ( 0x01 )
#define F28X_CAPCONX_CAP_3_DIS                  ( 0x00 )
#define F28X_CAPCONX_CAP_3_EN                   ( 0x01 )

//
// Extension Control Register (EXTCONA)
//
#define F28X_EXTCONA_ADC_SOC_DIS                ( 0x00 )
#define F28X_EXTCONA_ADC_SOC_EN                 ( 0x01 )
#define F28X_EXTCONA_CMP_IND_DIS                ( 0x00 )

//
// Peripheral Interrupt Expansion Interrupt Enable Register (PIEIER1 - PIEIER12)
//
#define F28X_PIEIERX_INT_DIS                    ( 0x00 )
#define F28X_PIEIERX_INT_EN                     ( 0xFF )

//
// Peripheral Interrupt Expansion Interrupt Flag Register (PIEIFR1 - PIEIFR12)
//
#define F28X_PIEIFRX_INT_FLG_CLR                ( 0x00 )

//
// Peripheral Interrupt Expansion Control Register (PIECTRL)
//
#define F28X_PIECTRL_ENPIE_FALSE                ( 0x00 )
#define F28X_PIECTRL_ENPIE_TRUE                 ( 0x01 )

//
// Interrupt Flag Register (IFR)
//
#define F28X_IFR_CLR_ALL_FLG                    ( 0x00 )

//
// Interrupt Enable Register (IER)
//
#define F28X_IER_INT_ALL_DIS                    ( 0x00 )

//
// External Interrupt Register (XINT1CR/XINT2CR)
//
#define F28X_XINTXCR_INT_DIS                    ( 0x00 )
#define F28X_XINTXCR_INT_EN                     ( 0x01 )

//
// ADC Control Register Defines (ADCTRL3)
//
#define F28X_ADCTRL1_RESET_ALL                  ( 0x01 )  // Reset entire ADC module
#define F28X_ADCTRL1_SUSP_STOP                  ( 0x03 )  // Sequencer and other wrapper logic stops
#define F28X_ADCTRL1_ACQPS_1                    ( 0x00 )  // Acq prescale 1 x Tclk
#define F28X_ADCTRL1_CPS_DIV2                   ( 0x01 )  // Conv clk prescale of 1/2
#define F28X_ADCTRL1_CPS_DIV1                   ( 0x00 )  // Conv clk prescale of 1
#define F28X_ADCTRL1_RUN_CONT                   ( 0x01 )  // continuous conv mode
#define F28X_ADCTRL1_RUN_STARTSTOP              ( 0x00 )  // start/stop conv mode
#define F28X_ADCTRL1_SEQ_WRAP                   ( 0x00 )  // Allows the seq to wrap around
#define F28X_ADCTRL1_SEQ_CASC                   ( 0x01 )  // cascaded sequencer
#define F28X_ADCTRL1_SEQ_DUAL                   ( 0x00 )  // dual sequencer

#define F28X_ADCTRL2_SEQ_EVXSOC_CASC            ( 0x01 ) // Cascaed seq. to start by EVB
#define F28X_ADCTRL2_SEQ_EVXSOC_NO_ACT          ( 0x00 ) // No action
#define F28X_ADCTRL2_RST_SEQX_IMMED             ( 0x01 ) // Reset SEQX immediately
#define F28X_ADCTRL2_RST_SEQX_NO_ACT            ( 0x00 ) // No Action
#define F28X_ADCTRL2_SEQX_SOC_START_TRIG        ( 0x01 ) // Start SEQX from currently stopped pos
#define F28X_ADCTRL2_SEQX_SOC_CLR_TRIG          ( 0x00 ) // Clr pending SOC trigger
#define F28X_ADCTRL2_SEQX_INT_DIS               ( 0x00 ) // Interrupt disabled
#define F28X_ADCTRL2_SEQX_INT_EN                ( 0x01 ) // Interrupt disabled
#define F28X_ADCTRL2_SEQX_INTMODE_SET_EVERY     ( 0x00 ) // INT is set at end of every SEQX sequence
#define F28X_ADCTRL2_SEQX_INTMODE_SET_OTHER     ( 0x00 ) // INT is set at end of every other SEQX sequence
#define F28X_ADCTRL2_SEQX_EXTSOC_EN             ( 0x01 )    // En ADC auto conv.
#define F28X_ADCTRL2_SEQX_EXTSOC_NO_ACT         ( 0x00 )    // No action

#define F28X_ADCTRL3_ADCLKPS_DIV_0              ( 0x0000 )  // Core CLK div 0 (+1)
#define F28X_ADCTRL3_SMODE_SEQ_SAMPLE           ( 0x00 )    // Sequential sampling
#define F28X_ADCTRL3_SMODE_SIMUL_SAMPLE         ( 0x01 )    // Simultaneous sampling
#define F28X_ADCTRL3_ADCBGRFDN_BG_RF_PWR_OFF    ( 0x00 )
#define F28X_ADCTRL3_ADCBGRFDN_BG_RF_PWR_ON     ( 0x03 )
#define F28X_ADCTRL3_ADCPWDN_PWR_OFF            ( 0x00 )
#define F28X_ADCTRL3_ADCPWDN_PWR_ON             ( 0x01 )

//
// Serial Communication Interface Registers ( SCIA/SCIB )
//
//SCICCR
#define F28X_SCICCR_STOP_BITS_ONE               ( 0 )  // Read/Write
#define F28X_SCICCR_STOP_BITS_TWO               ( 1 )
#define F28X_SCICCR_STOP_BITS_BITPOS            ( XT_BITPOS7 )

#define F28X_SCICCR_PARITY_ODD                  ( 0 )  // Read/Write
#define F28X_SCICCR_PARITY_EVEN                 ( 1 )
#define F28X_SCICCR_PARITY_BITPOS               ( XT_BITPOS6 )

#define F28X_SCICCR_PARITY_ENA_DIS              ( 0 )  // Read/Write
#define F28X_SCICCR_PARITY_ENA_EN               ( 1 )
#define F28X_SCICCR_PARITY_ENA_BITPOS           ( XT_BITPOS5 )

#define F28X_SCICCR_LOOPBACK_DIS                ( 0 )  // Read/Write
#define F28X_SCICCR_LOOPBACK_EN                 ( 1 )
#define F28X_SCICCR_LOOPBACK_BITPOS             ( XT_BITPOS4 )

#define F28X_SCICCR_MULTIPRO_MODE_IDLELINE      ( 0 )  // Read/Write
#define F28X_SCICCR_MULTIPRO_MODE_ADDRBIT       ( 1 )
#define F28X_SCICCR_MULTIPRO_MODE_BITPOS        ( XT_BITPOS3 )

#define F28X_SCICCR_CHARLEN_1                   ( 0 )  // Read/Write
#define F28X_SCICCR_CHARLEN_2                   ( 1 )
#define F28X_SCICCR_CHARLEN_3                   ( 2 )
#define F28X_SCICCR_CHARLEN_4                   ( 3 )
#define F28X_SCICCR_CHARLEN_5                   ( 4 )
#define F28X_SCICCR_CHARLEN_6                   ( 5 )
#define F28X_SCICCR_CHARLEN_7                   ( 6 )
#define F28X_SCICCR_CHARLEN_8                   ( 7 )
#define F28X_SCICCR_CHARLEN_BITPOS              ( XT_BITPOS0 )

// SCICTL1
#define F28X_SCICTL1_RX_ERR_INT_DIS             ( 0 )  // Read/Write
#define F28X_SCICTL1_RX_ERR_INT_EN              ( 1 )
#define F28X_SCICTL1_RX_ERR_INT_BITPOS          ( XT_BITPOS6 )

#define F28X_SCICTL1_SW_RESET_ASSERT            ( 0 )  // Read/Write
#define F28X_SCICTL1_SW_RESET_DEASSERT          ( 1 )
#define F28X_SCICTL1_SW_RESET_BITPOS            ( XT_BITPOS5 )

#define F28X_SCICTL1_TX_WAKE_DIS                ( 0 )  // Read/Write
#define F28X_SCICTL1_TX_WAKE_EN                 ( 1 )
#define F28X_SCICTL1_TX_WAKE_BITPOS             ( XT_BITPOS3 )

#define F28X_SCICTL1_SLEEP_DIS                  ( 0 )  // Read/Write
#define F28X_SCICTL1_SLEEP_EN                   ( 1 )
#define F28X_SCICTL1_SLEEP_BITPOS               ( XT_BITPOS2 )

#define F28X_SCICTL1_TX_DIS                     ( 0 )  // Read/Write
#define F28X_SCICTL1_TX_EN                      ( 1 )
#define F28X_SCICTL1_TX_BITPOS                  ( XT_BITPOS1 )

#define F28X_SCICTL1_RX_DIS                     ( 0 )  // Read/Write
#define F28X_SCICTL1_RX_EN                      ( 1 )
#define F28X_SCICTL1_RX_BITPOS                  ( XT_BITPOS0 )

// SCICTL2
#define F28X_SCICTL2_TXRDY_FLAG_IS_CLR          ( 0 )  // Read only
#define F28X_SCICTL2_TXRDY_FLAG_IS_SET          ( 1 )
#define F28X_SCICTL2_TXRDY_FLAG_BITPOS          ( XT_BITPOS7 )

#define F28X_SCICTL2_TX_EMPTY_FLAG_IS_CLR       ( 0 )  // Read only
#define F28X_SCICTL2_TX_EMPTY_FLAG_IS_SET       ( 1 )
#define F28X_SCICTL2_TX_EMPTY_FLAG_BITPOS       ( XT_BITPOS6 )

#define F28X_SCICTL2_RXBK_INT_DIS               ( 0 )  // Read/Write
#define F28X_SCICTL2_RXBK_INT_EN                ( 1 )
#define F28X_SCICTL2_RXBK_INT_BITPOS            ( XT_BITPOS1 )

#define F28X_SCICTL2_TXRDY_INT_DIS              ( 0 )  // Read/Write
#define F28X_SCICTL2_TXRDY_INT_EN               ( 1 )
#define F28X_SCICTL2_TXRDY_INT_BITPOS           ( XT_BITPOS0 )

// SCIRXST
#define F28X_SCIRXST_RX_ERROR_FLAG_IS_CLR       ( 0 )  // Read only
#define F28X_SCIRXST_RX_ERROR_FLAG_IS_SET       ( 1 )
#define F28X_SCIRXST_RX_ERROR_FLAG_BITPOS       ( XT_BITPOS7 )

#define F28X_SCIRXST_RX_RDY_FLAG_IS_CLR         ( 0 )  // Read only
#define F28X_SCIRXST_RX_RDY_FLAG_IS_SET         ( 1 )
#define F28X_SCIRXST_RX_RDY_FLAG_BITPOS         ( XT_BITPOS6 )

#define F28X_SCIRXST_BRKDT_FLAG_IS_CLR          ( 0 )  // Read only
#define F28X_SCIRXST_BRKDT_FLAG_IS_SET          ( 1 )
#define F28X_SCIRXST_BRKDT_FLAG_BITPOS          ( XT_BITPOS5 )

#define F28X_SCIRXST_FRAME_ERR_FLAG_IS_CLR      ( 0 )  // Read only
#define F28X_SCIRXST_FRAME_ERR_FLAG_IS_SET      ( 1 )
#define F28X_SCIRXST_FRAME_ERR_FLAG_BITPOS      ( XT_BITPOS4 )

#define F28X_SCIRXST_OVERRUN_ERR_FLAG_IS_CLR    ( 0 )  // Read only
#define F28X_SCIRXST_OVERRUN_ERR_FLAG_IS_SET    ( 1 )
#define F28X_SCIRXST_OVERRUN_ERR_FLAG_BITPOS    ( XT_BITPOS3 )

#define F28X_SCIRXST_PARITY_ERR_FLAG_IS_CLR     ( 0 )  // Read only
#define F28X_SCIRXST_PARITY_ERR_FLAG_IS_SET     ( 1 )
#define F28X_SCIRXST_PARITY_ERR_FLAG_BITPOS     ( XT_BITPOS2 )

#define F28X_SCIRXST_RX_WAKE_FLAG_IS_CLR        ( 0 )  // Read only
#define F28X_SCIRXST_RX_WAKE_FLAG_IS_SET        ( 1 )
#define F28X_SCIRXST_RX_WAKE_FLAG_BITPOS        ( XT_BITPOS1 )

// SCIFFTX
#define F28X_SCIFFTX_TXRX_CHAN_RESET            ( 0 )  // Read/Write
#define F28X_SCIFFTX_TXRX_CHAN_RESUME           ( 1 )
#define F28X_SCIFFTX_TXRX_CHAN_BITPOS           ( XT_BITPOS15 )
#define F28X_SCIFFTX_TXRX_FIFO_DIS              ( 0 )  // Read/Write
#define F28X_SCIFFTX_TXRX_FIFO_EN               ( 1 )
#define F28X_SCIFFTX_TXRX_FIFO_BITPOS           ( XT_BITPOS14 )

#define F28X_SCIFFTX_TX_FIFO_PTR_RESET          ( 0 )  // Read/Write
#define F28X_SCIFFTX_TX_FIFO_PTR_REEN           ( 1 )
#define F28X_SCIFFTX_TX_FIFO_PTR_BITPOS         ( XT_BITPOS13 )

#define F28X_SCIFFTX_TX_FIFO_FLAG_IS_CLR        ( 0 )  // Read only
#define F28X_SCIFFTX_TX_FIFO_FLAG_IS_SET        ( 1 )
#define F28X_SCIFFTX_TX_FIFO_FLAG_BITPOS        ( XT_BITPOS7 )

#define F28X_SCIFFTX_CLEAR_TX_FIFO_INT_FLAG     ( 1 )  // Write only
#define F28X_SCIFFTX_CLEAR_TX_FIFO_INT_BITPOS   ( XT_BITPOS6 )

#define F28X_SCIFFTX_TX_FIFO_INT_DIS            ( 0 )  // Read/Write
#define F28X_SCIFFTX_TX_FIFO_INT_EN             ( 1 )
#define F28X_SCIFFTX_TX_FIFO_INT_BITPOS         ( XT_BITPOS5 )

#define F28X_SCIFFTX_TX_FIFO_INTLVL_0           ( 0 )  // Read/Write
#define F28X_SCIFFTX_TX_FIFO_INTLVL_1           ( 1 )
#define F28X_SCIFFTX_TX_FIFO_INTLVL_2           ( 2 )
#define F28X_SCIFFTX_TX_FIFO_INTLVL_3           ( 3 )
#define F28X_SCIFFTX_TX_FIFO_INTLVL_4           ( 4 )
#define F28X_SCIFFTX_TX_FIFO_INTLVL_5           ( 5 )
#define F28X_SCIFFTX_TX_FIFO_INTLVL_6           ( 6 )
#define F28X_SCIFFTX_TX_FIFO_INTLVL_7           ( 7 )
#define F28X_SCIFFTX_TX_FIFO_INTLVL_8           ( 8 )
#define F28X_SCIFFTX_TX_FIFO_INTLVL_9           ( 9 )
#define F28X_SCIFFTX_TX_FIFO_INTLVL_10          ( 10 )
#define F28X_SCIFFTX_TX_FIFO_INTLVL_11          ( 11 )
#define F28X_SCIFFTX_TX_FIFO_INTLVL_12          ( 12 )
#define F28X_SCIFFTX_TX_FIFO_INTLVL_13          ( 13 )
#define F28X_SCIFFTX_TX_FIFO_INTLVL_14          ( 14 )
#define F28X_SCIFFTX_TX_FIFO_INTLVL_15          ( 15 )
#define F28X_SCIFFTX_TX_FIFO_INTLVL_16          ( 16 )
#define F28X_SCIFFTX_TX_FIFO_INTLVL_BITPOS      ( XT_BITPOS0 )

// SCIFFRX
#define F28X_SCIFFRX_RX_FIFO_OVF_FLAG_IS_CLR    ( 0 )  // Read only
#define F28X_SCIFFRX_RX_FIFO_OVF_FLAG_IS_SET    ( 1 )
#define F28X_SCIFFRX_RX_FIFO_OVF_FLAG_BITPOS    ( XT_BITPOS15 )

#define F28X_SCIFFRX_CLEAR_RX_FIFO_OVF_FLAG     ( 1 )  // Write only
#define F28X_SCIFFRX_CLEAR_RX_FIFO_OVF_BITPOS   ( XT_BITPOS14)

#define F28X_SCIFFRX_RX_FIFO_PTR_RESET          ( 0 )  // Read/Write
#define F28X_SCIFFRX_RX_FIFO_PTR_REEN           ( 1 )
#define F28X_SCIFFRX_RX_FIFO_PTR_BITPOS         ( XT_BITPOS13 )

#define F28X_SCIFFRX_RX_FIFO_INT_FLAG_IS_CLR    ( 0 )  // Read only
#define F28X_SCIFFRX_RX_FIFO_INT_FLAG_IS_SET    ( 1 )
#define F28X_SCIFFRX_RX_FIFO_INT_FLAG_BITPOS    ( XT_BITPOS7 )

#define F28X_SCIFFRX_CLEAR_RX_FIFO_INT_FLAG     ( 1 )  // Write only
#define F28X_SCIFFRX_CLEAR_RX_FIFO_INT_BITPOS   ( XT_BITPOS6 )

#define F28X_SCIFFRX_RX_FIFO_INT_DIS            ( 0 )  // Read/Write
#define F28X_SCIFFRX_RX_FIFO_INT_EN             ( 1 )
#define F28X_SCIFFRX_RX_FIFO_INT_BITPOS         ( XT_BITPOS5 )

#define F28X_SCIFFRX_RX_FIFO_INTLVL_0           ( 0 )  // Read/Write
#define F28X_SCIFFRX_RX_FIFO_INTLVL_1           ( 1 )
#define F28X_SCIFFRX_RX_FIFO_INTLVL_2           ( 2 )
#define F28X_SCIFFRX_RX_FIFO_INTLVL_3           ( 3 )
#define F28X_SCIFFRX_RX_FIFO_INTLVL_4           ( 4 )
#define F28X_SCIFFRX_RX_FIFO_INTLVL_5           ( 5 )
#define F28X_SCIFFRX_RX_FIFO_INTLVL_6           ( 6 )
#define F28X_SCIFFRX_RX_FIFO_INTLVL_7           ( 7 )
#define F28X_SCIFFRX_RX_FIFO_INTLVL_8           ( 8 )
#define F28X_SCIFFRX_RX_FIFO_INTLVL_9           ( 9 )
#define F28X_SCIFFRX_RX_FIFO_INTLVL_10          ( 10 )
#define F28X_SCIFFRX_RX_FIFO_INTLVL_11          ( 11 )
#define F28X_SCIFFRX_RX_FIFO_INTLVL_12          ( 12 )
#define F28X_SCIFFRX_RX_FIFO_INTLVL_13          ( 13 )
#define F28X_SCIFFRX_RX_FIFO_INTLVL_14          ( 14 )
#define F28X_SCIFFRX_RX_FIFO_INTLVL_15          ( 15 )
#define F28X_SCIFFRX_TX_FIFO_INTLVL_16          ( 16 )
#define F28X_SCIFFRX_RX_FIFO_INTLVL_BITPOS      ( XT_BITPOS0 )

// SCIRXBUF
#define F28X_SCIRXBUF_SCIFFFE_IS_CLR             ( 0 )  // Read only
#define F28X_SCIRXBUF_SCIFFFE_IS_SET             ( 1 )
#define F28X_SCIRXBUF_SCIFFPE_IS_CLR             ( 0 )  // Read only
#define F28X_SCIRXBUF_SCIFFPE_IS_SET             ( 1 )

// SCIFFCT
#define F28X_SCIFFCT_ABD_FLAG_IS_CLR            ( 0 )  // Read only
#define F28X_SCIFFCT_ABD_FLAG_IS_SET            ( 1 )
#define F28X_SCIFFCT_ABD_FLAG_BITPOS            ( XT_BITPOS15 )

#define F28X_SCIFFCT_CLEAR_ABD_FLAG             ( 1 )  // Write only
#define F28X_SCIFFCT_CLEAR_ABD_BITPOS           ( XT_BITPOS14 )

#define F28X_SCIFFCT_CDC_DIS                    ( 0 )  // Read/Write
#define F28X_SCIFFCT_CDC_EN                     ( 1 )
#define F28X_SCIFFCT_CDC_BITPOS                 ( XT_BITPOS13 )

#define F28X_SCIFFCT_FIFO_DLY_BIT0              ( 0x01 )  // Read/Write
#define F28X_SCIFFCT_FIFO_DLY_BIT1              ( 0x02 )
#define F28X_SCIFFCT_FIFO_DLY_BIT2              ( 0x04 )
#define F28X_SCIFFCT_FIFO_DLY_BIT3              ( 0x08 )
#define F28X_SCIFFCT_FIFO_DLY_BIT4              ( 0x10 )
#define F28X_SCIFFCT_FIFO_DLY_BIT5              ( 0x20 )
#define F28X_SCIFFCT_FIFO_DLY_BIT6              ( 0x40 )
#define F28X_SCIFFCT_FIFO_DLY_BIT7              ( 0x80 )

// SCIPRI
#define F28X_SCIPRI_SUSMODE_STOP                ( 0 )     // Read/Write
#define F28X_SCIPRI_SUSMODE_FREE                ( 1 )
#define F28X_SCIPRI_SUSMODE_SOFT                ( 2 )
#define F28X_SCIPRI_SUSMODE_BITPOS              ( XT_BITPOS3 )

// EV timer interrupt masks
#define F28X_PIE_T1PINT_MASK    ( 0x08 )
#define F28X_PIE_T1CINT_MASK    ( 0x10 )
#define F28X_PIE_T1UFINT_MASK   ( 0x20 )
#define F28X_PIE_T1OFINT_MASK   ( 0x40 )
#define F28X_PIE_T2PINT_MASK    ( 0x01 )
#define F28X_PIE_T2CINT_MASK    ( 0x02 )
#define F28X_PIE_T2UFINT_MASK   ( 0x04 )
#define F28X_PIE_T2OFINT_MASK   ( 0x08 )
#define F28X_PIE_T3PINT_MASK    ( 0x08 )
#define F28X_PIE_T3CINT_MASK    ( 0x10 )
#define F28X_PIE_T3UFINT_MASK   ( 0x20 )
#define F28X_PIE_T3OFINT_MASK   ( 0x40 )
#define F28X_PIE_T4PINT_MASK    ( 0x01 )
#define F28X_PIE_T4CINT_MASK    ( 0x02 )
#define F28X_PIE_T4UFINT_MASK   ( 0x04 )
#define F28X_PIE_T4OFINT_MASK   ( 0x08 )
#define F28X_PIE_T1_ALL_MASK    ( 0x78 )
#define F28X_PIE_T2_ALL_MASK    ( 0x0F )
#define F28X_PIE_T3_ALL_MASK    ( 0x78 )
#define F28X_PIE_T4_ALL_MASK    ( 0x0F )

// global interrupt enable register (IER) - timer bit
#define F28X_IER_INT2_ENA        ( XT_BIT1 )
#define F28X_IER_INT3_ENA        ( XT_BIT2 )
#define F28X_IER_INT4_ENA        ( XT_BIT3 )
#define F28X_IER_INT5_ENA        ( XT_BIT4 )

// gpio direction control register bits for timer compare output
#define F28X_GPADIR_T1DIR_OUTPUT    ( XT_BIT6 )
#define F28X_GPADIR_T2DIR_OUTPUT    ( XT_BIT7 )
#define F28X_GPBDIR_T3DIR_OUTPUT    ( XT_BIT6 )
#define F28X_GPBDIR_T4DIR_OUTPUT    ( XT_BIT7 )

// GP timer interrupt flag bits
// need to be shifted the correct number of bits to match EV and timer number
#define F28X_EVXIFR_NO_INT          ( 0 )
#define F28X_EVXIFR_TXPINT          ( 1 << XT_BITPOS0 )
#define F28X_EVXIFR_TXCINT          ( 1 << XT_BITPOS1 )
#define F28X_EVXIFR_TXUFINT         ( 1 << XT_BITPOS2 )
#define F28X_EVXIFR_TXOFINT         ( 1 << XT_BITPOS3 )

#define F28X_EVAIFRA_T1PINT          ( 1 << XT_BITPOS7 )
#define F28X_EVAIFRA_T1CINT          ( 1 << XT_BITPOS8 )
#define F28X_EVAIFRA_T1UFINT         ( 1 << XT_BITPOS9 )
#define F28X_EVAIFRA_T1OFINT         ( 1 << XT_BITPOS10 )

#define F28X_EVAIFRB_T2PINT          ( 1 << XT_BITPOS0 )
#define F28X_EVAIFRB_T2CINT          ( 1 << XT_BITPOS1 )
#define F28X_EVAIFRB_T2UFINT         ( 1 << XT_BITPOS2 )
#define F28X_EVAIFRB_T2OFINT         ( 1 << XT_BITPOS3 )

#define F28X_EVBIFRA_T3PINT          ( 1 << XT_BITPOS7 )
#define F28X_EVBIFRA_T3CINT          ( 1 << XT_BITPOS8 )
#define F28X_EVBIFRA_T3UFINT         ( 1 << XT_BITPOS9 )
#define F28X_EVBIFRA_T3OFINT         ( 1 << XT_BITPOS10 )

#define F28X_EVBIFRB_T4PINT          ( 1 << XT_BITPOS0 )
#define F28X_EVBIFRB_T4CINT          ( 1 << XT_BITPOS1 )
#define F28X_EVBIFRB_T4UFINT         ( 1 << XT_BITPOS2 )
#define F28X_EVBIFRB_T4OFINT         ( 1 << XT_BITPOS3 )

// GP timer control reg A masks
// GP timer conrol reg A masks
#define F28X_GPTCONA_T1TOADC_MSK    ( XT_BIT7 & XT_BIT8 )
#define F28X_GPTCONA_T2TOADC_MSK    ( XT_BIT9 & XT_BIT10 )
#define F28X_GPTCONB_T3TOADC_MSK    ( XT_BIT7 & XT_BIT8 )
#define F28X_GPTCONB_T4TOADC_MSK    ( XT_BIT9 & XT_BIT10 )

// GP timer compare output polarity bit masks
#define F28X_GPTCONA_T1PIN_MSK      ( XT_BIT0 & XT_BIT1 )
#define F28X_GPTCONA_T2PIN_MSK      ( XT_BIT2 & XT_BIT3 )
#define F28X_GPTCONB_T3PIN_MSK      ( XT_BIT0 & XT_BIT1 )
#define F28X_GPTCONB_T4PIN_MSK      ( XT_BIT2 & XT_BIT3 )

// GP timer control bits
#define F28X_GPTCON_TXTOADC_DISAB    ( 0x0000 )
#define F28X_GPTCON_TXTOADC_COMP     ( XT_BIT0 & XT_BIT1 )
#define F28X_GPTCON_TXTOADC_PRD      ( XT_BIT1 )
#define F28X_GPTCON_TXTOADC_UNDER    ( XT_BIT0 )
#define F28X_GPTCON_TCOMPOE_EN       ( XT_BIT6 )
#define F28X_GPTCON_TXPIN_DISAB      ( 0x0000 )
#define F28X_GPTCON_TXPIN_FRC_LOW    ( 0x0000 )
#define F28X_GPTCON_TXPIN_FRC_HI     ( XT_BIT0 & XT_BIT1 )
#define F28X_GPTCON_TXPIN_ACT_LOW    ( XT_BIT0 )
#define F28X_GPTCON_TXPIN_ACT_HI     ( XT_BIT1 )

// EV timer control bits
#define F28X_TXCON_NO_COMP           ( 0x0000 )
#define F28X_TXCON_ZERO_RELOAD       ( 0x0000 )
#define F28X_TXCON_EQUAL_RELOAD      ( XT_BIT2 )
#define F28X_TXCON_IMM_RELOAD        ( XT_BIT3 )

// EV timer clock source types
#define F28X_TXCON_TCLK_INT          ( 0x0000 )
#define F28X_TXCON_TCLK_EXT          ( XT_BIT4 )
#define F28X_TXCON_TCLK_QEP          ( XT_BIT4 & XT_BIT5 )

// EV timer sync mode bits
#define F28X_TXCON_SYN_MSK           ( XT_BIT7 & XT_BIT0 )
#define F28X_TXCON_SYN_SEP           ( 0x0000 )
#define F28X_TXCON_SYN_PRD           ( XT_BIT0 )
#define F28X_TXCON_SYN_START         ( XT_BIT7 )
#define F28X_TXCON_SYN_PRDST         ( XT_BIT7 & XT_BIT0 )

// GP timer control masks
// Added HM - Nov 19, 2004
#define F28X_TXCON_EMMCTRL_MSK  ( XT_BIT14 & XT_BIT15 )//Emulation Control
#define F28X_TXCON_TMODE_MSK    ( XT_BIT11 & XT_BIT12 )//Count Mode
#define F28X_TXCON_TPS_MSK      ( XT_BIT8 & XT_BIT9 & XT_BIT10 )//Clk Prescaler
#define F28X_TXCON_TXSWTX_MSK   ( XT_BIT7 )//Timer start
#define F28X_TXCON_TENABLE_MASK ( XT_BIT6 )//Timer Enable
#define F28X_TXCON_TCLKS_MSK    ( XT_BIT4 & XT_BIT5 )//Clock Source Select
#define F28X_TXCON_TCLD_MSK     ( XT_BIT2 & XT_BIT3 )//Timer Compare Reg Reload
#define F28X_TXCON_TECMPR_MSK   ( XT_BIT1 )//Timer compare enable
#define F28X_TXCON_SELTXPR_MSK  ( XT_BIT0 )//Timer 1 period select

// GP timer emulation control bits
#define F28X_TXCON_EMMCTRL_NO_EFFECT3       ( 0xC000 )
#define F28X_TXCON_EMMCTRL_NO_EFFECT2       ( 0x8000 )
#define F28X_TXCON_EMMCTRL_STOP_PER_COMP    ( 0x4000 )
#define F28X_TXCON_EMMCTRL_STOP_IMMED       ( 0x0000 )

// timer count mode bits (11 & 12)
#define F28X_TXCON_TMODE_CONTUPDOWN  ( XT_BIT11 )                // 0x0800
#define F28X_TXCON_TMODE_CONTUP      ( XT_BIT12 )                // 0x1000
#define F28X_TXCON_TMODE_DIRUPDOWN   ( XT_BIT11 & XT_BIT12 )     // 0x1800
#define F28X_TXCON_TMODE_STOPHOLD    ( 0x0000 )                  // 0x0000

// timer enable bit positions
#define F28X_T1CON_TENABLE_EN    ( XT_BIT6 )
#define F28X_T2CON_TENABLE_EN    ( XT_BIT6 )
#define F28X_T3CON_TENABLE_EN    ( XT_BIT6 )
#define F28X_T4CON_TENABLE_EN    ( XT_BIT6 )

// timer prescaler bits
#define F28X_T1CON_TPS_MSK       ( XT_BIT8 & XT_BIT9 & XT_BIT10 )
#define F28X_T2CON_TPS_MSK       ( XT_BIT8 & XT_BIT9 & XT_BIT10 )
#define F28X_T3CON_TPS_MSK       ( XT_BIT8 & XT_BIT9 & XT_BIT10 )
#define F28X_T4CON_TPS_MSK       ( XT_BIT8 & XT_BIT9 & XT_BIT10 )

// timer prescaler register shift count
#define F28X_T1CON_TPS0          ( XT_BITPOS8 )
#define F28X_T2CON_TPS0          ( XT_BITPOS8 )
#define F28X_T3CON_TPS0          ( XT_BITPOS8 )
#define F28X_T4CON_TPS0          ( XT_BITPOS8 )

//GPIO General
//
#define F28X_GPIO_PIN_DIR_OUTPUT     ( 1 )
#define F28X_GPIO_PIN_DIR_INPUT      ( 0 )

// GPIO A Control Register bit masks
// NOTE: GPADIR, GPADAT, GPASET, GPACLEAR, GPATOGGLE registers have the same bit
// to I/O signal mapping as the GPAMUX
#define F28X_GPAMUX_MSK_PWM1        ( XT_BIT0 )
#define F28X_GPAMUX_MSK_PWM2        ( XT_BIT1 )
#define F28X_GPAMUX_MSK_PWM3        ( XT_BIT2 )
#define F28X_GPAMUX_MSK_PWM4        ( XT_BIT3 )
#define F28X_GPAMUX_MSK_PWM5        ( XT_BIT4 )
#define F28X_GPAMUX_MSK_PWM6        ( XT_BIT5 )
#define F28X_GPAMUX_MSK_T1PWM_T1CMP ( XT_BIT6 )
#define F28X_GPAMUX_MSK_T2PWM_T2CMP ( XT_BIT7 )
#define F28X_GPAMUX_MSK_CAP1_QEP1   ( XT_BIT8 )
#define F28X_GPAMUX_MSK_CAP2_QEP2   ( XT_BIT9 )
#define F28X_GPAMUX_MSK_CAP3_QEPI1  ( XT_BIT10 )
#define F28X_GPAMUX_MSK_TDIRA       ( XT_BIT11 )
#define F28X_GPAMUX_MSK_TCLKINA     ( XT_BIT12 )
#define F28X_GPAMUX_MSK_C1TRIP      ( XT_BIT13 )
#define F28X_GPAMUX_MSK_C2TRIP      ( XT_BIT14 )
#define F28X_GPAMUX_MSK_C3TRIP      ( XT_BIT15 )

// GPIO B Control Register bit masks
// *NOTE: GPBDIR, GPBDAT, GPBSET, GPBCLEAR, GPBTOGGLE registers have the same bit
// to I/O signal mapping as the GPBMUX (*manual does not explicitly state this)
#define F28X_GPBMUX_MSK_PWM7        ( XT_BIT0 )
#define F28X_GPBMUX_MSK_PWM8        ( XT_BIT1 )
#define F28X_GPBMUX_MSK_PWM9        ( XT_BIT2 )
#define F28X_GPBMUX_MSK_PWM10       ( XT_BIT3 )
#define F28X_GPBMUX_MSK_PWM11       ( XT_BIT4 )
#define F28X_GPBMUX_MSK_PWM12       ( XT_BIT5 )
#define F28X_GPBMUX_MSK_T3PWM_T3CMP ( XT_BIT6 )
#define F28X_GPBMUX_MSK_T4PWM_T4CMP ( XT_BIT7 )
#define F28X_GPBMUX_MSK_CAP4_QEP3   ( XT_BIT8 )
#define F28X_GPBMUX_MSK_CAP5_QEP4   ( XT_BIT9 )
#define F28X_GPBMUX_MSK_CAP6_QEPI2  ( XT_BIT10 )
#define F28X_GPBMUX_MSK_TDIRB       ( XT_BIT11 )
#define F28X_GPBMUX_MSK_TCLKINB     ( XT_BIT12 )
#define F28X_GPBMUX_MSK_C4TRIP      ( XT_BIT13 )
#define F28X_GPBMUX_MSK_C5TRIP      ( XT_BIT14 )
#define F28X_GPBMUX_MSK_C6TRIP      ( XT_BIT15 )

// EVA/B Dead-Band Timer Control Register bit masks (DBTCON)
// NOTE: bits 0 & 1 reserved
#define F28X_DBTCON_MSK_DBTPS0      ( XT_BIT2 )
#define F28X_DBTCON_MSK_DBTPS1      ( XT_BIT3 )
#define F28X_DBTCON_MSK_DBTPS2      ( XT_BIT4 )
#define F28X_DBTCON_MSK_EDBT1       ( XT_BIT5 )
#define F28X_DBTCON_MSK_EDBT2       ( XT_BIT6 )
#define F28X_DBTCON_MSK_EDBT3       ( XT_BIT7 )
#define F28X_DBTCON_MSK_DBT0        ( XT_BIT8 )
#define F28X_DBTCON_MSK_DBT1        ( XT_BIT9 )
#define F28X_DBTCON_MSK_DBT2        ( XT_BIT10 )
#define F28X_DBTCON_MSK_DBT3        ( XT_BIT11 )
// NOTE: bits 12-15 reserved

// EVA/B Dead-Band Timer Control Register bit positions (DBTCON)
// NOTE: bits 0 & 1 reserved
#define F28X_DBTCON_POS_DBTPS0      ( XT_BITPOS2 )
#define F28X_DBTCON_POS_DBTPS1      ( XT_BITPOS3 )
#define F28X_DBTCON_POS_DBTPS2      ( XT_BITPOS4 )
#define F28X_DBTCON_POS_EDBT1       ( XT_BITPOS5 )
#define F28X_DBTCON_POS_EDBT2       ( XT_BITPOS6 )
#define F28X_DBTCON_POS_EDBT3       ( XT_BITPOS7 )
#define F28X_DBTCON_POS_DBT0        ( XT_BITPOS8 )
#define F28X_DBTCON_POS_DBT1        ( XT_BITPOS9 )
#define F28X_DBTCON_POS_DBT2        ( XT_BITPOS10 )
#define F28X_DBTCON_POS_DBT3        ( XT_BITPOS11 )
// NOTE: bits 12-15 reserved

// All Dead-Band Timer enable bits mask
#define F28X_DBTCON_MSK_EDBT        ( 7 << F28X_DBTCON_POS_EDBT1 )
// All Dead-Band Timer prescaler bits mask
#define F28X_DBTCON_MSK_DBTPS       ( 7 << F28X_DBTCON_POS_DBTPS0 )

// Use these defines to configure the DBTCON register
//15 dead-band timer periods
#define F28X_DBTCON_DBT_15          ( 15 << F28X_DBTCON_POS_DBT0 )
//14 dead-band timer periods
#define F28X_DBTCON_DBT_14          ( 14 << F28X_DBTCON_POS_DBT0 )
//13 dead-band timer periods
#define F28X_DBTCON_DBT_13          ( 13 << F28X_DBTCON_POS_DBT0 )
//12 dead-band timer periods
#define F28X_DBTCON_DBT_12          ( 12 << F28X_DBTCON_POS_DBT0 )
//11 dead-band timer periods
#define F28X_DBTCON_DBT_11          ( 11 << F28X_DBTCON_POS_DBT0 )
//10 dead-band timer periods
#define F28X_DBTCON_DBT_10          ( 10 << F28X_DBTCON_POS_DBT0 )
//9 dead-band timer periods
#define F28X_DBTCON_DBT_9           ( 9 << F28X_DBTCON_POS_DBT0 )
//8 dead-band timer periods
#define F28X_DBTCON_DBT_8           ( 8 << F28X_DBTCON_POS_DBT0 )
//7 dead-band timer periods
#define F28X_DBTCON_DBT_7           ( 7 << F28X_DBTCON_POS_DBT0 )
//6 dead-band timer periods
#define F28X_DBTCON_DBT_6           ( 6 << F28X_DBTCON_POS_DBT0 )
//5 dead-band timer periods
#define F28X_DBTCON_DBT_5           ( 5 << F28X_DBTCON_POS_DBT0 )
//4 dead-band timer periods
#define F28X_DBTCON_DBT_4           ( 4 << F28X_DBTCON_POS_DBT0 )
//3 dead-band timer periods
#define F28X_DBTCON_DBT_3           ( 3 << F28X_DBTCON_POS_DBT0 )
//2 dead-band timer periods
#define F28X_DBTCON_DBT_2           ( 2 << F28X_DBTCON_POS_DBT0 )
//1 dead-band timer periods
#define F28X_DBTCON_DBT_1           ( 1 << F28X_DBTCON_POS_DBT0 )
//0 dead-band timer periods
#define F28X_DBTCON_DBT_0           ( 0 << F28X_DBTCON_POS_DBT0 )
//Dead-band tmr 3 en
#define F28X_DBTCON_EDBT3_EN        ( 1 << F28X_DBTCON_POS_EDBT3 )
//Dead-band tmr 3 dis
#define F28X_DBTCON_EDBT3_DIS       ( 0 << F28X_DBTCON_POS_EDBT3 )
//Dead-band tmr 2 en
#define F28X_DBTCON_EDBT2_EN        ( 1 << F28X_DBTCON_POS_EDBT2 )
//Dead-band tmr 2 dis
#define F28X_DBTCON_EDBT2_DIS       ( 0 << F28X_DBTCON_POS_EDBT2 )
//Dead-band tmr 1 en
#define F28X_DBTCON_EDBT1_EN        ( 1 << F28X_DBTCON_POS_EDBT1 )
//Dead-band tmr 1 dis
#define F28X_DBTCON_EDBT1_DIS       ( 0 << F28X_DBTCON_POS_EDBT1 )
//Dead-band timer / 32 prescaler
#define F28X_DBTCON_DBTPS_32        ( 5 << F28X_DBTCON_POS_DBTPS0 )
//Dead-band timer / 16 prescaler
#define F28X_DBTCON_DBTPS_16        ( 4 << F28X_DBTCON_POS_DBTPS0 )
//Dead-band timer / 8 prescaler
#define F28X_DBTCON_DBTPS_8         ( 3 << F28X_DBTCON_POS_DBTPS0 )
//Dead-band timer / 4 prescaler
#define F28X_DBTCON_DBTPS_4         ( 2 << F28X_DBTCON_POS_DBTPS0 )
//Dead-band timer / 2 prescaler
#define F28X_DBTCON_DBTPS_2         ( 1 << F28X_DBTCON_POS_DBTPS0 )
//Dead-band timer / 1 prescaler
#define F28X_DBTCON_DBTPS_1         ( 0 << F28X_DBTCON_POS_DBTPS0 )

// EVA/B Compare Control Register bit masks (COMCON)
#define F28X_COMCON_MSK_C1TRIPE     ( XT_BIT0 )
#define F28X_COMCON_MSK_C2TRIPE     ( XT_BIT1 )
#define F28X_COMCON_MSK_C3TRIPE     ( XT_BIT2 )
// NOTE: bits 3 & 4 reserved
#define F28X_COMCON_MSK_FCMP1OE     ( XT_BIT5 )
#define F28X_COMCON_MSK_FCMP2OE     ( XT_BIT6 )
#define F28X_COMCON_MSK_FCMP3OE     ( XT_BIT7 )
#define F28X_COMCON_MSK_PDPINT      ( XT_BIT8 )
#define F28X_COMCON_MSK_FCMPOE      ( XT_BIT9 )
#define F28X_COMCON_MSK_ACTRLD0     ( XT_BIT10 )
#define F28X_COMCON_MSK_ACTRLD1     ( XT_BIT11 )
#define F28X_COMCON_MSK_SVENABLE    ( XT_BIT12 )
#define F28X_COMCON_MSK_CLD0        ( XT_BIT13 )
#define F28X_COMCON_MSK_CLD1        ( XT_BIT14 )
#define F28X_COMCON_MSK_CENABLE     ( XT_BIT15 )

// EVA/B Compare Control Register bit positions (COMCON)
#define F28X_COMCON_POS_C1TRIPE     ( XT_BITPOS0 )
#define F28X_COMCON_POS_C2TRIPE     ( XT_BITPOS1 )
#define F28X_COMCON_POS_C3TRIPE     ( XT_BITPOS2 )
// NOTE: bits 3 & 4 reserved
#define F28X_COMCON_POS_FCMP1OE     ( XT_BITPOS5 )
#define F28X_COMCON_POS_FCMP2OE     ( XT_BITPOS6 )
#define F28X_COMCON_POS_FCMP3OE     ( XT_BITPOS7 )
#define F28X_COMCON_POS_PDPINT      ( XT_BITPOS8 )
#define F28X_COMCON_POS_FCMPOE      ( XT_BITPOS9 )
#define F28X_COMCON_POS_ACTRLD0     ( XT_BITPOS10 )
#define F28X_COMCON_POS_ACTRLD1     ( XT_BITPOS11 )
#define F28X_COMCON_POS_SVENABLE    ( XT_BITPOS12 )
#define F28X_COMCON_POS_CLD0        ( XT_BITPOS13 )
#define F28X_COMCON_POS_CLD1        ( XT_BITPOS14 )
#define F28X_COMCON_POS_CENABLE     ( XT_BITPOS15 )

// Use these defines to configure the COMCON register
//Compare enable
#define F28X_COMCON_CENABLE_EN      ( 1 << F28X_COMCON_POS_CENABLE )
//Compare disable
#define F28X_COMCON_CENABLE_DIS     ( 0 << F28X_COMCON_POS_CENABLE )
//Reserved by TI
#define F28X_COMCON_CLD_RESERVED    ( 3 << F28X_COMCON_POS_CLD0 )
//Reload CMPRx immediately
#define F28X_COMCON_CLD_IMM         ( 2 << F28X_COMCON_POS_CLD0 )
//Reload CMPRx on T1CNT0 or period
#define F28X_COMCON_CLD_UF_PD       ( 1 << F28X_COMCON_POS_CLD0 )
//Reload CMPRx on T1CNT0
#define F28X_COMCON_CLD_UF          ( 0 << F28X_COMCON_POS_CLD0 )
//Enable space vector PWM
#define F28X_COMCON_SVENABLE_EN     ( 1 << F28X_COMCON_POS_SVENABLE )
//Disable space vector PWM
#define F28X_COMCON_SVENABLE_DIS    ( 0 << F28X_COMCON_POS_SVENABLE )
//Reserved by TI
#define F28X_COMCON_ACTRLD_RESERVED ( 3 << F28X_COMCON_POS_ACTRLD0 )
//Reload action reg immediately
#define F28X_COMCON_ACTRLD_IMM      ( 2 << F28X_COMCON_POS_ACTRLD0 )
//Reload action reg on T1CNT0 or per
#define F28X_COMCON_ACTRLD_UF_PD    ( 1 << F28X_COMCON_POS_ACTRLD0 )
//Reload action reg on T1CNT0
#define F28X_COMCON_ACTRLD_UF       ( 0 << F28X_COMCON_POS_ACTRLD0 )
//PWM output pins are not HI-Z (en)
#define F28X_COMCON_FCMPOE_EN       ( 1 << F28X_COMCON_POS_FCMPOE )
//PWM output pins are HI-Z (dis)
#define F28X_COMCON_FCMPOE_DIS      ( 0 << F28X_COMCON_POS_FCMPOE )

// Compare Action Control Register bit positions (ACTRA)
#define F28X_ACTR_POS_SVRDIR        ( XT_BITPOS15 )
#define F28X_ACTR_POS_SVB_D2        ( XT_BITPOS14 )
#define F28X_ACTR_POS_SVB_D1        ( XT_BITPOS13 )
#define F28X_ACTR_POS_SVB_D0        ( XT_BITPOS12 )
#define F28X_ACTR_POS_CMP12ACT1     ( XT_BITPOS11 )
#define F28X_ACTR_POS_CMP12ACT0     ( XT_BITPOS10 )
#define F28X_ACTR_POS_CMP11ACT1     ( XT_BITPOS9 )
#define F28X_ACTR_POS_CMP11ACT0     ( XT_BITPOS8 )
#define F28X_ACTR_POS_CMP10ACT1     ( XT_BITPOS7 )
#define F28X_ACTR_POS_CMP10ACT0     ( XT_BITPOS6 )
#define F28X_ACTR_POS_CMP9ACT1      ( XT_BITPOS5 )
#define F28X_ACTR_POS_CMP9ACT0      ( XT_BITPOS4 )
#define F28X_ACTR_POS_CMP8ACT1      ( XT_BITPOS3 )
#define F28X_ACTR_POS_CMP8ACT0      ( XT_BITPOS2 )
#define F28X_ACTR_POS_CMP7ACT1      ( XT_BITPOS1 )
#define F28X_ACTR_POS_CMP7ACT0      ( XT_BITPOS0 )
#define F28X_ACTR_POS_CMP6ACT1      ( XT_BITPOS11 )
#define F28X_ACTR_POS_CMP6ACT0      ( XT_BITPOS10 )
#define F28X_ACTR_POS_CMP5ACT1      ( XT_BITPOS9 )
#define F28X_ACTR_POS_CMP5ACT0      ( XT_BITPOS8 )
#define F28X_ACTR_POS_CMP4ACT1      ( XT_BITPOS7 )
#define F28X_ACTR_POS_CMP4ACT0      ( XT_BITPOS6 )
#define F28X_ACTR_POS_CMP3ACT1      ( XT_BITPOS5 )
#define F28X_ACTR_POS_CMP3ACT0      ( XT_BITPOS4 )
#define F28X_ACTR_POS_CMP2ACT1      ( XT_BITPOS3 )
#define F28X_ACTR_POS_CMP2ACT0      ( XT_BITPOS2 )
#define F28X_ACTR_POS_CMP1ACT1      ( XT_BITPOS1 )
#define F28X_ACTR_POS_CMP1ACT0      ( XT_BITPOS0 )

//
// F28X External Memory Interface
//

// XINTF Configuration Register

// XTIMCLK divisor
#define F28X_XINTFCNF2_XTIMCLK_SYSCLKOUT_DIV_2          ( 1 )
#define F28X_XINTFCNF2_XTIMCLK_SYSCLKOUT_DIV_1          ( 0 )

// External bus access
#define F28X_XINTFCNF2_HOLD_AUTO_GRANT                  ( 0 )
#define F28X_XINTFCNF2_HOLD_NO_GRANT                    ( 1 )

// Microprocessor / Microcomputer mode
#define F28X_XINTFCNF2_MPNMC_MICROPROCESSOR_MODE        ( 1 )
#define F28X_XINTFCNF2_MPNMC_MICROCOMPUTER_MODE         ( 0 )

// XCLKOUT
#define F28X_XINTFCNF2_CLKOFF_XCLKOUT_ENABLE            ( 0 )
#define F28X_XINTFCNF2_CLKOFF_XCLKOUT_DISABLE           ( 1 )

// Clock Mode
#define F28X_XINTFCNF2_CLKMODE_XTIMCLK_DIV_2            ( 1 )
#define F28X_XINTFCNF2_CLKMODE_XTIMCLK_DIV_1            ( 0 )

// Write Buffer Depth
#define F28X_XINTFCNF2_WRITEBUFFER_EMPTY                ( 0 )
#define F28X_XINTFCNF2_WRITEBUFFER_0                    ( 0 )
#define F28X_XINTFCNF2_WRITEBUFFER_1                    ( 1 )
#define F28X_XINTFCNF2_WRITEBUFFER_2                    ( 2 )
#define F28X_XINTFCNF2_WRITEBUFFER_3                    ( 3 )
#define F28X_XINTFCNF2_WRITEBUFFER_FULL                 ( 3 )

// XTIMINGx Register

// Wait State Scale Factor
#define F28X_XTIMINGX_X2TIMING_ENABLED                  ( 1 )
#define F28X_XTIMINGX_X2TIMING_DISABLED                 ( 0 )

// memory width
#define F28X_XTIMINGX_XSIZE_16BIT                       ( 3 )

// Ready Mode
#define F28X_XTIMINGX_XREADY_SYNCHRONOUS                ( 0 )
#define F28X_XTIMINGX_XREADY_ASYNCHRONOUS               ( 1 )

// Ready Signal
#define F28X_XTIMINGX_USEREADY_ENABLED                  ( 1 )
#define F28X_XTIMINGX_USEREADY_DISABLED                 ( 0 )

// External Memory Interface Wait States
#define F28X_XTIMINGX_WAIT_1_STATE                      ( 1 )
#define F28X_XTIMINGX_WAIT_2_STATES                     ( 2 )
#define F28X_XTIMINGX_WAIT_3_STATES                     ( 3 )
#define F28X_XTIMINGX_WAIT_4_STATES                     ( 4 )
#define F28X_XTIMINGX_WAIT_5_STATES                     ( 5 )
#define F28X_XTIMINGX_WAIT_6_STATES                     ( 6 )
#define F28X_XTIMINGX_WAIT_7_STATES                     ( 7 )

//
// Flash/OTP Configuration definitions
//

// Flash Options Register
#define F28X_FOPT_PIPELINE_ENABLED                      ( 1 )
#define F28X_FOPT_PIPELINE_DISABLED                     ( 0 )

// Flash Status Register
#define F28X_FSTATUS_CLEAR_V3STAT                       ( 1 )

// Flash Power Register
#define F28X_FPWR_PUMP_BANK_SLEEP                       ( 0 )
#define F28X_FPWR_PUMP_BANK_STDBY                       ( 1 )
#define F28X_FPWR_PUMP_BANK_ACTIVE                      ( 3 )

// Standby Wait Register
#define F28X_FSTDBYWAIT_MAX_WAIT                        ( 0x01FF )

// Active Wait Register
#define F28X_FACTIVEWAIT_MAX_WAIT                       ( 0x01FF )

// Bank Wait states
#define F28X_FBANKWAIT_0_STATES                         ( 0 )
#define F28X_FBANKWAIT_1_STATES                         ( 1 )
#define F28X_FBANKWAIT_2_STATES                         ( 2 )
#define F28X_FBANKWAIT_3_STATES                         ( 3 )
#define F28X_FBANKWAIT_4_STATES                         ( 4 )
#define F28X_FBANKWAIT_5_STATES                         ( 5 )
#define F28X_FBANKWAIT_6_STATES                         ( 6 )
#define F28X_FBANKWAIT_7_STATES                         ( 7 )
#define F28X_FBANKWAIT_8_STATES                         ( 8 )
#define F28X_FBANKWAIT_9_STATES                         ( 9 )
#define F28X_FBANKWAIT_10_STATES                        ( 10 )
#define F28X_FBANKWAIT_11_STATES                        ( 11 )
#define F28X_FBANKWAIT_12_STATES                        ( 12 )
#define F28X_FBANKWAIT_13_STATES                        ( 13 )
#define F28X_FBANKWAIT_14_STATES                        ( 14 )
#define F28X_FBANKWAIT_15_STATES                        ( 15 )


//
// ---------------------------------------------------------------------------
// Defines based on function
// ---------------------------------------------------------------------------


/*==============================================================================
                            Type Definitions
==============================================================================*/


/*==============================================================================
                                Enums
==============================================================================*/


/*=============================================================================
                              Structures
==============================================================================*/


/*==============================================================================
                          External/Public Constants
==============================================================================*/


/*==============================================================================
                          External/Public Variables
==============================================================================*/


/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/


#endif  // F28X_H
