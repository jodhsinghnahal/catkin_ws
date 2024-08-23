/*==============================================================================
Copyright © 2005 Xantrex International

This file is the property of Xantrex International and shall not be reproduced,
copied, or used as the basis for the manufacture or sale of equipment without
the express written permission of Xantrex International.

FILE NAME:
    F280xBitDefines.h

PURPOSE:
    The purpose of this file is to provide a central repository for all of the
    bit field functionality #defines for the registers on the 280x DSP.

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

    #define F280X_<REGISTER>_<BIT FIELD>_<FUNCTION>       <VALUE>

    Exceptions to this formatting are acceptable provided they make sense. 
    ---------------------------------------------------------------------------

CHANGE HISTORY:
$Log: F280xBitDefines.h $

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/6
    User: JeffF     Date: 02/13/06  Time: 06:59PM
    Ran the copyright updater script to bring the copyright notice on all project
    files up to date.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/5
    User: JeffF     Date: 05/31/05  Time: 10:25PM
    Changed the defines for the ADC registers. Started using the NFO macros
    so that registers can be intialized in a more code efficient manner.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/4
    User: JeffF     Date: 05/20/05  Time: 04:29PM
    Added defines for Action Qualifier control registers on the PWM modules.

    
    ***********************************************
    Revision: SurveyorPfrmA_JeffF/3
    User: JeffF     Date: 05/05/05  Time: 12:20AM
    Made minor change to name of one of the defines

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/7
    User: HueyD     Date: 04/27/05  Time: 05:13PM
    commented

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/6
    User: HueyD     Date: 04/26/05  Time: 02:52AM
    Attempt to reduce code size

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/5
    User: HueyD     Date: 04/25/05  Time: 05:54PM
    Added high res

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/4
    User: HueyD     Date: 04/23/05  Time: 01:12AM
    Used new defines and new paths

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/3
    User: HueyD     Date: 04/22/05  Time: 11:14PM
    Created new way of defining 16bit registers

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/2
    User: HueyD     Date: 04/14/05  Time: 06:19PM
    simplified defines

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/1
    User: HueyD     Date: 04/13/05  Time: 09:40PM
    Added macros for bit manipulation.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/2
    User: JeffF     Date: 04/12/05  Time: 10:26PM
    Added some more defines for the ADC module.

==============================================================================*/

#ifndef F280X_BIT_DEFINES_H
#define F280X_BIT_DEFINES_H


/*==============================================================================
                              Includes
==============================================================================*/


/*==============================================================================
                              Defines
==============================================================================*/

// Define a macro to convert number of bits into an unshifted bit mask
#define F280X_mBITS_TO_MSK_UNSHFTD( bits ) ( ~( 0xFFFF << bits ) )

// Each NFO (info) define is made up of 2 values:
//     1.  The bit position of a field's right most member
//     2.  The number of bits the field utilizes
#define F280X_NFO_POS_BITS  8
#define F280X_NFO_BITS_BITS 8
#define F280X_NFO_POS_SHFT  F280X_NFO_BITS_BITS
#define F280X_NFO_BITS_SHFT 0
#define F280X_NFO_POS_MSK   ( F280X_mBITS_TO_MSK_UNSHFTD( F280X_NFO_POS_BITS ) \
                                << F280X_NFO_POS_SHFT )
#define F280X_NFO_BITS_MSK  ( F280X_mBITS_TO_MSK_UNSHFTD( F280X_NFO_BITS_BITS )\
                                << F280X_NFO_BITS_SHFT )
#define F280X_mNFO( pos, bits ) ( ( pos  << F280X_NFO_POS_SHFT ) \
                                | ( bits << F280X_NFO_BITS_SHFT ) )

// Clears and then sets a register using masks in the proper field
#define F280X_mCLR_N_SET_MSK( Reg, Msk, CfgMsk ) \
            Reg &= ~Msk; \
            Reg |= CfgMsk

#define F280X_mCLR_N_SET_FIELD ( Reg, Pos, Bits, Cfg ) \
            Reg &= ~( F280X_mBITS_TO_MSK_UNSHFTD( Bits ) << Pos ); \
            Reg |=  ( Cfg << Pos )
#define F280X_mSET_FIELD( Reg, Pos, Cfg ) \
            Reg |=  ( Cfg << Pos )
 
// ---------------------------------------------------------------------------
// Defines based on device registers.
// ---------------------------------------------------------------------------

//
// Peripheral Clock Register (PCLKCR)
//
#define F280X_PCLKCR_CLK_OFF                     ( 0x00 ) // ALL BITS: Disables (shuts off) the clock
#define F280X_PCLKCR_CLK_ON                      ( 0x01 ) // ALL BITS: Enables (turns on) the clock

//
// WDT Control Register (WDCR)
//
#define F280X_WDCR_WDT_RST_FLAG_CLR              ( 1 << XT_BITPOS7 ) // clears WDT reset flag
#define F280X_WDCR_WDT_ENABLE_TOGGLE             ( 1 << XT_BITPOS6 ) // disables the WDT
#define F280X_WDCR_WDT_EN                        ( 0 << XT_BITPOS6 ) // enables the WDT

// this value must be written to this field anytime that a write is made to this register
// otherwise the system explodes! well not really, it just resets if the WD is enabled
#define F280X_WDCR_WRITE_OR_DIE                  ( 5 << XT_BITPOS3 )

// these bits configure the watchdog counter clock (WDCLK) rate relative to OSCCLK/512
#define F280X_WDCR_WDCLK_OSCCLK_DIV_1            ( 1 << XT_BITPOS0 )
#define F280X_WDCR_WDCLK_OSCCLK_DIV_2            ( 2 << XT_BITPOS0 )
#define F280X_WDCR_WDCLK_OSCCLK_DIV_4            ( 3 << XT_BITPOS0 )
#define F280X_WDCR_WDCLK_OSCCLK_DIV_8            ( 4 << XT_BITPOS0 )
#define F280X_WDCR_WDCLK_OSCCLK_DIV_16           ( 5 << XT_BITPOS0 )
#define F280X_WDCR_WDCLK_OSCCLK_DIV_32           ( 6 << XT_BITPOS0 )
#define F280X_WDCR_WDCLK_OSCCLK_DIV_64           ( 7 << XT_BITPOS0 )

// set up bit field for Watch Dog Control Register (WDCR)
#define F280X_mWDCR_INIT  ( F280X_WDCR_WDT_RST_FLAG_CLR    \
                    | F280X_WDCR_WDT_ENABLE_TOGGLE        \
                    | F280X_WDCR_WRITE_OR_DIE             \
                    | F280X_WDCR_WDCLK_OSCCLK_DIV_64      \
                    )

//
// High/Low Speed Peripheral Clock Prescalar Register (LOSPCP/HISPCP)
//
#define F280X_XXSPCP_XXPCLK_SYSCLKOUT_DIV_1      ( 0x00 )
#define F280X_XXSPCP_XXPCLK_SYSCLKOUT_DIV_2      ( 0x01 )
#define F280X_XXSPCP_XXPCLK_SYSCLKOUT_DIV_4      ( 0x02 )
#define F280X_XXSPCP_XXPCLK_SYSCLKOUT_DIV_6      ( 0x03 )
#define F280X_XXSPCP_XXPCLK_SYSCLKOUT_DIV_8      ( 0x04 )
#define F280X_XXSPCP_XXPCLK_SYSCLKOUT_DIV_10     ( 0x05 )
#define F280X_XXSPCP_XXPCLK_SYSCLKOUT_DIV_12     ( 0x06 )
#define F280X_XXSPCP_XXPCLK_SYSCLKOUT_DIV_14     ( 0x07 )

//
// System Control and Status Register (SCSR)
//
#define F280X_SCSR_WDINTS                        ( XT_BITPOS2 ) // WDT INT Status
#define F280X_SCSR_WDENINT                       ( XT_BITPOS1 ) // WDT INT enable
#define F280X_SCSR_WDOVERRIDE                    ( XT_BITPOS0 ) // WDT Override

#define F280X_SCSR_WDENINT_INT                   ( 1 << F280X_SCSR_WDENINT )    // WDT INT out enabled
#define F280X_SCSR_WDENINT_RST                   ( 0 << F280X_SCSR_WDENINT )    // WDT RST out enabled
#define F280X_SCSR_WDOVERRIDE_TOG                ( 1 << F280X_SCSR_WDOVERRIDE ) // WDT Override bit Toggle

// since the reset state of WDOVERRIDE is 1, and since writing a 1 to that bit
// will toggle it and thus disallow us access to the hardware WDT disable, we
// must only set the choice of output for the watchdog output.
#define F280X_mSCSR_INIT                         ( F280X_SCSR_WDENINT_RST )

//
// Low-power Mode Register 0 (LPMCR0)
//
#define F280X_LPMCR0_LPM_LPM_IDLE                ( 0x00 )
#define F280X_LPMCR0_LPM_LPM_STANDBY             ( 0x01 )
#define F280X_LPMCR0_LPM_LPM_HALT                ( 0x02 )

//
// Low-power Mode Register 1 (LPMCR1)
//
#define F280X_LPMCR1_NO_STANDBY_INTS             ( 0x00 )

//
// Event Manager - General Purpose Timer Control Register (GTPCONA/GPTCONB)
//
#define F280X_GPTCONX_TXCTRIPE_DIS               ( 0x00 )
#define F280X_GPTCONX_TXCTRIPE_EN                ( 0x01 )
#define F280X_GPTCONX_TXTOADC_DIS                ( 0x00 )
#define F280X_GPTCONX_TXTOADC_UND_INT            ( 0x01 )
#define F280X_GPTCONX_TXTOADC_PER_INT            ( 0x02 )
#define F280X_GPTCONX_TXTOADC_CMP_INT            ( 0x03 )
#define F280X_GPTCONX_TCMPOE_HI_Z                ( 0x00 )
#define F280X_GPTCONX_TCMPOE_TMR_DRVN            ( 0x01 )
#define F280X_GPTCONX_TCMPOE_FORCE_LOW           ( 0x00 )
#define F280X_GPTCONX_TCMPOE_ACTV_LOW            ( 0x01 )
#define F280X_GPTCONX_TCMPOE_ACTV_HI             ( 0x02 )
#define F280X_GPTCONX_TCMPOE_FORCE_HI            ( 0x03 )

//
// Event Manager - Timer X Control Register (T1CON/T2CON/T3CON/T4CON)
//
#define F280X_TXCON_TENABLE_FALSE                ( 0x00 )
#define F280X_TXCON_TENABLE_TRUE                 ( 0x01 )

//
// Compare X Control Register (CONCOMA/CONCOMB)
//
#define F280X_COMCONX_CENABLE_FALSE              ( 0x00 )
#define F280X_COMCONX_CENABLE_TRUE               ( 0x01 )

//
// Dead Band Timer X Register - (DBTCONA/DBTCONB)
//
#define F280X_DBTCONX_DBT_DIS                    ( 0x00 )
#define F280X_DBTCONX_DBT_EN                     ( 0x01 )

//
// Capture X Control Register (CAPCONA/CAPCONB)
//
#define F280X_CAPCONX_CAP_1_2_DIS                ( 0x00 )
#define F280X_CAPCONX_CAP_1_2_EN                 ( 0x01 )
#define F280X_CAPCONX_CAP_3_DIS                  ( 0x00 )
#define F280X_CAPCONX_CAP_3_EN                   ( 0x01 )

//
// Extension Control Register (EXTCONA)
//
#define F280X_EXTCONA_ADC_SOC_DIS                ( 0x00 )
#define F280X_EXTCONA_ADC_SOC_EN                 ( 0x01 )
#define F280X_EXTCONA_CMP_IND_DIS                ( 0x00 )

//
// Peripheral Interrupt Expansion Interrupt Enable Register (PIEIER1 - PIEIER12)
//
#define F280X_PIEIERX_INT_DIS                    ( 0x00 )
#define F280X_PIEIERX_INT_EN                     ( 0xFF )

//
// Peripheral Interrupt Expansion Interrupt Flag Register (PIEIFR1 - PIEIFR12)
//
#define F280X_PIEIFRX_INT_FLG_CLR                ( 0x00 )

//
// Peripheral Interrupt Expansion Control Register (PIECTRL)
//
#define F280X_PIECTRL_ENPIE_FALSE                ( 0x00 )
#define F280X_PIECTRL_ENPIE_TRUE                 ( 0x01 )

//
// Interrupt Flag Register (IFR)
//
#define F280X_IFR_CLR_ALL_FLG                    ( 0x00 )

//
// Interrupt Enable Register (IER)
//
#define F280X_IER_INT_ALL_DIS                    ( 0x00 )

//
// External Interrupt Register (XINT1CR/XINT2CR)
//
#define F280X_XINTXCR_INT_DIS                    ( 0x00 )
#define F280X_XINTXCR_INT_EN                     ( 0x01 )



//
// ADC Control Register Defines (ADCTRL1)
//
#define F280X_ADCTRL1_SEQCASC_NFO                F280X_mNFO(  4,  1 )
#define F280X_ADCTRL1_SEQOVRD_NFO                F280X_mNFO(  5,  1 )
#define F280X_ADCTRL1_CONTRUN_NFO                F280X_mNFO(  6,  1 )
#define F280X_ADCTRL1_CPS_NFO                    F280X_mNFO(  7,  1 )
#define F280X_ADCTRL1_ACQPS_NFO                  F280X_mNFO(  8,  4 )
#define F280X_ADCTRL1_SUSMOD_NFO                 F280X_mNFO(  12, 2 )
#define F280X_ADCTRL1_RESET_NFO                  F280X_mNFO(  14, 1 )

#define F280X_ADCTRL1_SEQCASC_DUAL               ( 0 )
#define F280X_ADCTRL1_SEQCASC_CASC               ( 1 )
#define F280X_ADCTRL1_SEQOVRD_DIS                ( 0 )
#define F280X_ADCTRL1_SEQOVRD_ENA                ( 1 )
#define F280X_ADCTRL1_CONTRUN_START_STOP         ( 0 )
#define F280X_ADCTRL1_CONTRUN_CONT_CONV          ( 1 )
#define F280X_ADCTRL1_CPS_DIVBY_1                ( 0 )
#define F280X_ADCTRL1_CPS_DIVBY_2                ( 1 )
#define F280X_ADCTRL1_ACQPS_WIDTH_1              ( 0 )
#define F280X_ADCTRL1_ACQPS_WIDTH_2              ( 1 )
#define F280X_ADCTRL1_ACQPS_WIDTH_3              ( 2 )
#define F280X_ADCTRL1_ACQPS_WIDTH_4              ( 3 )
#define F280X_ADCTRL1_ACQPS_WIDTH_5              ( 4 )
#define F280X_ADCTRL1_ACQPS_WIDTH_6              ( 5 )
#define F280X_ADCTRL1_ACQPS_WIDTH_7              ( 6 )
#define F280X_ADCTRL1_ACQPS_WIDTH_8              ( 7 )
#define F280X_ADCTRL1_ACQPS_WIDTH_9              ( 8 )
#define F280X_ADCTRL1_ACQPS_WIDTH_10             ( 9 )
#define F280X_ADCTRL1_ACQPS_WIDTH_11             ( 10 )
#define F280X_ADCTRL1_ACQPS_WIDTH_12             ( 11 )
#define F280X_ADCTRL1_ACQPS_WIDTH_13             ( 12 )
#define F280X_ADCTRL1_ACQPS_WIDTH_14             ( 13 )
#define F280X_ADCTRL1_ACQPS_WIDTH_15             ( 14 )
#define F280X_ADCTRL1_ACQPS_WIDTH_16             ( 15 )
#define F280X_ADCTRL1_SUSMOD_MODE_0              ( 0 )
#define F280X_ADCTRL1_SUSMOD_MODE_1              ( 1 )
#define F280X_ADCTRL1_SUSMOD_MODE_2              ( 2 )
#define F280X_ADCTRL1_SUSMOD_MODE_3              ( 3 )
#define F280X_ADCTRL1_RESET_ALL                  ( 1 )

//
// ADC Control Register Defines (ADCTRL2)
//    
#define F280X_ADCTRL2_EPWMSOCBSEQ2_NFO           F280X_mNFO(  0,  1 )
#define F280X_ADCTRL2_INTMODSEQ2_NFO             F280X_mNFO(  2,  1 )
#define F280X_ADCTRL2_INTENASEQ2_NFO             F280X_mNFO(  3,  1 )
#define F280X_ADCTRL2_SOCSEQ2_NFO                F280X_mNFO(  5,  1 )
#define F280X_ADCTRL2_RSTSEQ2_NFO                F280X_mNFO(  6,  1 )
#define F280X_ADCTRL2_EXTSOCSEQ1_NFO             F280X_mNFO(  7,  1 )
#define F280X_ADCTRL2_EPWMSOCASEQ1_NFO           F280X_mNFO(  8,  1 )
#define F280X_ADCTRL2_INTMODSEQ1_NFO             F280X_mNFO(  10, 1 )
#define F280X_ADCTRL2_INTENASEQ1_NFO             F280X_mNFO(  11, 1 )
#define F280X_ADCTRL2_SOCSEQ1_NFO                F280X_mNFO(  13, 1 )
#define F280X_ADCTRL2_RSTSEQ1_NFO                F280X_mNFO(  14, 1 )
#define F280X_ADCTRL2_EPWMSOCBSEQ_NFO            F280X_mNFO(  15, 1 )

#define F280X_ADCTRL2_EPWMSOCBSEQ2_DIS           ( 0 )
#define F280X_ADCTRL2_EPWMSOCBSEQ2_ENA           ( 1 )
#define F280X_ADCTRL2_INTMODSEQ2_EVERY           ( 0 )
#define F280X_ADCTRL2_INTMODSEQ2_EVERY_OTHER     ( 1 )
#define F280X_ADCTRL2_INTENASEQ2_DIS             ( 0 ) 
#define F280X_ADCTRL2_INTENASEQ2_ENA             ( 1 )
#define F280X_ADCTRL2_SOCSEQ2_CLR                ( 0 )
#define F280X_ADCTRL2_SOCSEQ2_START              ( 1 )
#define F280X_ADCTRL2_RSTSEQ2_NO_ACT             ( 0 )
#define F280X_ADCTRL2_RSTSEQ2_RST                ( 1 )
#define F280X_ADCTRL2_EXTSOCSEQ1_DIS             ( 0 )
#define F280X_ADCTRL2_EXTSOCSEQ1_ENA             ( 1 )
#define F280X_ADCTRL2_EPWMSOCASEQ1_DIS           ( 0 )
#define F280X_ADCTRL2_EPWMSOCASEQ1_ENA           ( 1 )
#define F280X_ADCTRL2_INTMODSEQ1_EVERY           ( 0 )
#define F280X_ADCTRL2_INTMODSEQ1_EVERY_OTHER     ( 1 )
#define F280X_ADCTRL2_INTENASEQ1_DIS             ( 0 )
#define F280X_ADCTRL2_INTENASEQ1_ENA             ( 1 )
#define F280X_ADCTRL2_SOCSEQ1_CLR                ( 0 )
#define F280X_ADCTRL2_SOCSEQ1_START              ( 1 )
#define F280X_ADCTRL2_RSTSEQ1_NO_ACT             ( 0 )
#define F280X_ADCTRL2_RSTSEQ1_RST                ( 1 )
#define F280X_ADCTRL2_EPWMSOCBSEQ_DIS            ( 0 )
#define F280X_ADCTRL2_EPWMSOCBSEQ_ENA            ( 1 )

//
// ADC Control Register Defines (ADCTRL3)
//
#define F280X_ADCTRL3_SMODESEL_NFO               F280X_mNFO( 0, 1 )
#define F280X_ADCTRL3_ADCCLKPS_NFO               F280X_mNFO( 1, 4 )
#define F280X_ADCTRL3_ADCPWDN_NFO                F280X_mNFO( 5, 1 )
#define F280X_ADCTRL3_ADCBGRFDN_NFO              F280X_mNFO( 6, 2 )

#define F280X_ADCTRL3_SMODESEL_SEQ_SAMPLE        ( 0  )
#define F280X_ADCTRL3_SMODESEL_SIMUL_SAMPLE      ( 1  )
#define F280X_ADCTRL3_ADCCLKPS_DIVBY_1           ( 0  )
#define F280X_ADCTRL3_ADCCLKPS_DIVBY_2           ( 1  )
#define F280X_ADCTRL3_ADCCLKPS_DIVBY_4           ( 2  )
#define F280X_ADCTRL3_ADCCLKPS_DIVBY_6           ( 3  )
#define F280X_ADCTRL3_ADCCLKPS_DIVBY_8           ( 4  )
#define F280X_ADCTRL3_ADCCLKPS_DIVBY_10          ( 5  )
#define F280X_ADCTRL3_ADCCLKPS_DIVBY_12          ( 6  )
#define F280X_ADCTRL3_ADCCLKPS_DIVBY_14          ( 7  )
#define F280X_ADCTRL3_ADCCLKPS_DIVBY_16          ( 8  )
#define F280X_ADCTRL3_ADCCLKPS_DIVBY_18          ( 9  )
#define F280X_ADCTRL3_ADCCLKPS_DIVBY_20          ( 10 )
#define F280X_ADCTRL3_ADCCLKPS_DIVBY_22          ( 11 )
#define F280X_ADCTRL3_ADCCLKPS_DIVBY_24          ( 12 )
#define F280X_ADCTRL3_ADCCLKPS_DIVBY_26          ( 13 )
#define F280X_ADCTRL3_ADCCLKPS_DIVBY_28          ( 14 )
#define F280X_ADCTRL3_ADCCLKPS_DIVBY_30          ( 15 )
#define F280X_ADCTRL3_ADCPWDN_PWR_OFF            ( 0  )
#define F280X_ADCTRL3_ADCPWDN_PWR_ON             ( 1  )
#define F280X_ADCTRL3_ADCBGRFDN_PWR_OFF          ( 0  )
#define F280X_ADCTRL3_ADCBGRFDN_PWR_ON           ( 3  )


//
// ADC Maximum Conversion Channels Register Defines (ADCMAXCONV)
//
#define F280X_ADCMAXCONV_MAXCONV1_NFO            F280X_mNFO( 0, 4 )
#define F280X_ADCMAXCONV_MAXCONV2_NFO            F280X_mNFO( 4, 3 )

#define F280X_ADCMAXCONV_MAXCONV1_CONV_1        ( 0  )           
#define F280X_ADCMAXCONV_MAXCONV1_CONV_2        ( 1  )
#define F280X_ADCMAXCONV_MAXCONV1_CONV_3        ( 2  )
#define F280X_ADCMAXCONV_MAXCONV1_CONV_4        ( 3  )
#define F280X_ADCMAXCONV_MAXCONV1_CONV_5        ( 4  )
#define F280X_ADCMAXCONV_MAXCONV1_CONV_6        ( 5  )
#define F280X_ADCMAXCONV_MAXCONV1_CONV_7        ( 6  )
#define F280X_ADCMAXCONV_MAXCONV1_CONV_8        ( 7  )
#define F280X_ADCMAXCONV_MAXCONV1_CONV_9        ( 8  )
#define F280X_ADCMAXCONV_MAXCONV1_CONV_10       ( 9  )
#define F280X_ADCMAXCONV_MAXCONV1_CONV_11       ( 10 )
#define F280X_ADCMAXCONV_MAXCONV1_CONV_12       ( 11 )
#define F280X_ADCMAXCONV_MAXCONV1_CONV_13       ( 12 )
#define F280X_ADCMAXCONV_MAXCONV1_CONV_14       ( 13 )
#define F280X_ADCMAXCONV_MAXCONV1_CONV_15       ( 14 )
#define F280X_ADCMAXCONV_MAXCONV1_CONV_16       ( 15 )
#define F280X_ADCMAXCONV_MAXCONV2_CONV_1        ( 0  )           
#define F280X_ADCMAXCONV_MAXCONV2_CONV_2        ( 1  )
#define F280X_ADCMAXCONV_MAXCONV2_CONV_3        ( 2  )
#define F280X_ADCMAXCONV_MAXCONV2_CONV_4        ( 3  )
#define F280X_ADCMAXCONV_MAXCONV2_CONV_5        ( 4  )
#define F280X_ADCMAXCONV_MAXCONV2_CONV_6        ( 5  )
#define F280X_ADCMAXCONV_MAXCONV2_CONV_7        ( 6  )
#define F280X_ADCMAXCONV_MAXCONV2_CONV_8        ( 7  )

//
// ADC Reference Select Register Defines (ADCREFSEL)
//
#define F280X_ADCREFSEL_REFSEL_NFO              F280X_mNFO( 14, 2 )

#define F280X_ADCREFSEL_REFSEL_INTERNAL         ( 0 )
#define F280X_ADCREFSEL_REFSEL_EXTERNAL         ( 1 )


//
// Serial Communication Interface Registers ( SCIA/SCIB )
//
//SCICCR
#define F280X_SCICCR_STOP_BITS_ONE               ( 0 )  // Read/Write
#define F280X_SCICCR_STOP_BITS_TWO               ( 1 )
#define F280X_SCICCR_STOP_BITS_BITPOS            ( XT_BITPOS7 )

#define F280X_SCICCR_PARITY_ODD                  ( 0 )  // Read/Write
#define F280X_SCICCR_PARITY_EVEN                 ( 1 )
#define F280X_SCICCR_PARITY_BITPOS               ( XT_BITPOS6 )

#define F280X_SCICCR_PARITY_ENA_DIS              ( 0 )  // Read/Write
#define F280X_SCICCR_PARITY_ENA_EN               ( 1 )
#define F280X_SCICCR_PARITY_ENA_BITPOS           ( XT_BITPOS5 )

#define F280X_SCICCR_LOOPBACK_DIS                ( 0 )  // Read/Write
#define F280X_SCICCR_LOOPBACK_EN                 ( 1 )
#define F280X_SCICCR_LOOPBACK_BITPOS             ( XT_BITPOS4 )

#define F280X_SCICCR_MULTIPRO_MODE_IDLELINE      ( 0 )  // Read/Write
#define F280X_SCICCR_MULTIPRO_MODE_ADDRBIT       ( 1 )
#define F280X_SCICCR_MULTIPRO_MODE_BITPOS        ( XT_BITPOS3 )

#define F280X_SCICCR_CHARLEN_1                   ( 0 )  // Read/Write
#define F280X_SCICCR_CHARLEN_2                   ( 1 )
#define F280X_SCICCR_CHARLEN_3                   ( 2 )
#define F280X_SCICCR_CHARLEN_4                   ( 3 )
#define F280X_SCICCR_CHARLEN_5                   ( 4 )
#define F280X_SCICCR_CHARLEN_6                   ( 5 )
#define F280X_SCICCR_CHARLEN_7                   ( 6 )
#define F280X_SCICCR_CHARLEN_8                   ( 7 )
#define F280X_SCICCR_CHARLEN_BITPOS              ( XT_BITPOS0 )

// SCICTL1
#define F280X_SCICTL1_RX_ERR_INT_DIS             ( 0 )  // Read/Write
#define F280X_SCICTL1_RX_ERR_INT_EN              ( 1 )
#define F280X_SCICTL1_RX_ERR_INT_BITPOS          ( XT_BITPOS6 )

#define F280X_SCICTL1_SW_RESET_ASSERT            ( 0 )  // Read/Write
#define F280X_SCICTL1_SW_RESET_DEASSERT          ( 1 )
#define F280X_SCICTL1_SW_RESET_BITPOS            ( XT_BITPOS5 )

#define F280X_SCICTL1_TX_WAKE_DIS                ( 0 )  // Read/Write
#define F280X_SCICTL1_TX_WAKE_EN                 ( 1 )
#define F280X_SCICTL1_TX_WAKE_BITPOS             ( XT_BITPOS3 )

#define F280X_SCICTL1_SLEEP_DIS                  ( 0 )  // Read/Write
#define F280X_SCICTL1_SLEEP_EN                   ( 1 )
#define F280X_SCICTL1_SLEEP_BITPOS               ( XT_BITPOS2 )

#define F280X_SCICTL1_TX_DIS                     ( 0 )  // Read/Write
#define F280X_SCICTL1_TX_EN                      ( 1 )
#define F280X_SCICTL1_TX_BITPOS                  ( XT_BITPOS1 )

#define F280X_SCICTL1_RX_DIS                     ( 0 )  // Read/Write
#define F280X_SCICTL1_RX_EN                      ( 1 )
#define F280X_SCICTL1_RX_BITPOS                  ( XT_BITPOS0 )

// SCICTL2
#define F280X_SCICTL2_TXRDY_FLAG_IS_CLR          ( 0 )  // Read only
#define F280X_SCICTL2_TXRDY_FLAG_IS_SET          ( 1 )
#define F280X_SCICTL2_TXRDY_FLAG_BITPOS          ( XT_BITPOS7 )

#define F280X_SCICTL2_TX_EMPTY_FLAG_IS_CLR       ( 0 )  // Read only
#define F280X_SCICTL2_TX_EMPTY_FLAG_IS_SET       ( 1 )
#define F280X_SCICTL2_TX_EMPTY_FLAG_BITPOS       ( XT_BITPOS6 )

#define F280X_SCICTL2_RXBK_INT_DIS               ( 0 )  // Read/Write
#define F280X_SCICTL2_RXBK_INT_EN                ( 1 )
#define F280X_SCICTL2_RXBK_INT_BITPOS            ( XT_BITPOS1 )

#define F280X_SCICTL2_TXRDY_INT_DIS              ( 0 )  // Read/Write
#define F280X_SCICTL2_TXRDY_INT_EN               ( 1 )
#define F280X_SCICTL2_TXRDY_INT_BITPOS           ( XT_BITPOS0 )

// SCIRXST
#define F280X_SCIRXST_RX_ERROR_FLAG_IS_CLR       ( 0 )  // Read only
#define F280X_SCIRXST_RX_ERROR_FLAG_IS_SET       ( 1 )
#define F280X_SCIRXST_RX_ERROR_FLAG_BITPOS       ( XT_BITPOS7 )

#define F280X_SCIRXST_RX_RDY_FLAG_IS_CLR         ( 0 )  // Read only
#define F280X_SCIRXST_RX_RDY_FLAG_IS_SET         ( 1 )
#define F280X_SCIRXST_RX_RDY_FLAG_BITPOS         ( XT_BITPOS6 )

#define F280X_SCIRXST_BRKDT_FLAG_IS_CLR          ( 0 )  // Read only
#define F280X_SCIRXST_BRKDT_FLAG_IS_SET          ( 1 )
#define F280X_SCIRXST_BRKDT_FLAG_BITPOS          ( XT_BITPOS5 )

#define F280X_SCIRXST_FRAME_ERR_FLAG_IS_CLR      ( 0 )  // Read only
#define F280X_SCIRXST_FRAME_ERR_FLAG_IS_SET      ( 1 )
#define F280X_SCIRXST_FRAME_ERR_FLAG_BITPOS      ( XT_BITPOS4 )

#define F280X_SCIRXST_OVERRUN_ERR_FLAG_IS_CLR    ( 0 )  // Read only
#define F280X_SCIRXST_OVERRUN_ERR_FLAG_IS_SET    ( 1 )
#define F280X_SCIRXST_OVERRUN_ERR_FLAG_BITPOS    ( XT_BITPOS3 )

#define F280X_SCIRXST_PARITY_ERR_FLAG_IS_CLR     ( 0 )  // Read only
#define F280X_SCIRXST_PARITY_ERR_FLAG_IS_SET     ( 1 )
#define F280X_SCIRXST_PARITY_ERR_FLAG_BITPOS     ( XT_BITPOS2 )

#define F280X_SCIRXST_RX_WAKE_FLAG_IS_CLR        ( 0 )  // Read only
#define F280X_SCIRXST_RX_WAKE_FLAG_IS_SET        ( 1 )
#define F280X_SCIRXST_RX_WAKE_FLAG_BITPOS        ( XT_BITPOS1 )

// SCIFFTX
#define F280X_SCIFFTX_TXRX_CHAN_RESET            ( 0 )  // Read/Write
#define F280X_SCIFFTX_TXRX_CHAN_RESUME           ( 1 )
#define F280X_SCIFFTX_TXRX_CHAN_BITPOS           ( XT_BITPOS15 )
#define F280X_SCIFFTX_TXRX_FIFO_DIS              ( 0 )  // Read/Write
#define F280X_SCIFFTX_TXRX_FIFO_EN               ( 1 )
#define F280X_SCIFFTX_TXRX_FIFO_BITPOS           ( XT_BITPOS14 )

#define F280X_SCIFFTX_TX_FIFO_PTR_RESET          ( 0 )  // Read/Write
#define F280X_SCIFFTX_TX_FIFO_PTR_REEN           ( 1 )
#define F280X_SCIFFTX_TX_FIFO_PTR_BITPOS         ( XT_BITPOS13 )

#define F280X_SCIFFTX_TX_FIFO_FLAG_IS_CLR        ( 0 )  // Read only
#define F280X_SCIFFTX_TX_FIFO_FLAG_IS_SET        ( 1 )
#define F280X_SCIFFTX_TX_FIFO_FLAG_BITPOS        ( XT_BITPOS7 )

#define F280X_SCIFFTX_CLEAR_TX_FIFO_INT_FLAG     ( 1 )  // Write only
#define F280X_SCIFFTX_CLEAR_TX_FIFO_INT_BITPOS   ( XT_BITPOS6 )

#define F280X_SCIFFTX_TX_FIFO_INT_DIS            ( 0 )  // Read/Write
#define F280X_SCIFFTX_TX_FIFO_INT_EN             ( 1 )
#define F280X_SCIFFTX_TX_FIFO_INT_BITPOS         ( XT_BITPOS5 )

#define F280X_SCIFFTX_TX_FIFO_INTLVL_0           ( 0 )  // Read/Write
#define F280X_SCIFFTX_TX_FIFO_INTLVL_1           ( 1 )
#define F280X_SCIFFTX_TX_FIFO_INTLVL_2           ( 2 )
#define F280X_SCIFFTX_TX_FIFO_INTLVL_3           ( 3 )
#define F280X_SCIFFTX_TX_FIFO_INTLVL_4           ( 4 )
#define F280X_SCIFFTX_TX_FIFO_INTLVL_5           ( 5 )
#define F280X_SCIFFTX_TX_FIFO_INTLVL_6           ( 6 )
#define F280X_SCIFFTX_TX_FIFO_INTLVL_7           ( 7 )
#define F280X_SCIFFTX_TX_FIFO_INTLVL_8           ( 8 )
#define F280X_SCIFFTX_TX_FIFO_INTLVL_9           ( 9 )
#define F280X_SCIFFTX_TX_FIFO_INTLVL_10          ( 10 )
#define F280X_SCIFFTX_TX_FIFO_INTLVL_11          ( 11 )
#define F280X_SCIFFTX_TX_FIFO_INTLVL_12          ( 12 )
#define F280X_SCIFFTX_TX_FIFO_INTLVL_13          ( 13 )
#define F280X_SCIFFTX_TX_FIFO_INTLVL_14          ( 14 )
#define F280X_SCIFFTX_TX_FIFO_INTLVL_15          ( 15 )
#define F280X_SCIFFTX_TX_FIFO_INTLVL_16          ( 16 )
#define F280X_SCIFFTX_TX_FIFO_INTLVL_BITPOS      ( XT_BITPOS0 )

// SCIFFRX
#define F280X_SCIFFRX_RX_FIFO_OVF_FLAG_IS_CLR    ( 0 )  // Read only
#define F280X_SCIFFRX_RX_FIFO_OVF_FLAG_IS_SET    ( 1 )
#define F280X_SCIFFRX_RX_FIFO_OVF_FLAG_BITPOS    ( XT_BITPOS15 )

#define F280X_SCIFFRX_CLEAR_RX_FIFO_OVF_FLAG     ( 1 )  // Write only
#define F280X_SCIFFRX_CLEAR_RX_FIFO_OVF_BITPOS   ( XT_BITPOS14)

#define F280X_SCIFFRX_RX_FIFO_PTR_RESET          ( 0 )  // Read/Write
#define F280X_SCIFFRX_RX_FIFO_PTR_REEN           ( 1 )
#define F280X_SCIFFRX_RX_FIFO_PTR_BITPOS         ( XT_BITPOS13 )

#define F280X_SCIFFRX_RX_FIFO_INT_FLAG_IS_CLR    ( 0 )  // Read only
#define F280X_SCIFFRX_RX_FIFO_INT_FLAG_IS_SET    ( 1 )
#define F280X_SCIFFRX_RX_FIFO_INT_FLAG_BITPOS    ( XT_BITPOS7 )

#define F280X_SCIFFRX_CLEAR_RX_FIFO_INT_FLAG     ( 1 )  // Write only
#define F280X_SCIFFRX_CLEAR_RX_FIFO_INT_BITPOS   ( XT_BITPOS6 )

#define F280X_SCIFFRX_RX_FIFO_INT_DIS            ( 0 )  // Read/Write
#define F280X_SCIFFRX_RX_FIFO_INT_EN             ( 1 )
#define F280X_SCIFFRX_RX_FIFO_INT_BITPOS         ( XT_BITPOS5 )

#define F280X_SCIFFRX_RX_FIFO_INTLVL_0           ( 0 )  // Read/Write
#define F280X_SCIFFRX_RX_FIFO_INTLVL_1           ( 1 )
#define F280X_SCIFFRX_RX_FIFO_INTLVL_2           ( 2 )
#define F280X_SCIFFRX_RX_FIFO_INTLVL_3           ( 3 )
#define F280X_SCIFFRX_RX_FIFO_INTLVL_4           ( 4 )
#define F280X_SCIFFRX_RX_FIFO_INTLVL_5           ( 5 )
#define F280X_SCIFFRX_RX_FIFO_INTLVL_6           ( 6 )
#define F280X_SCIFFRX_RX_FIFO_INTLVL_7           ( 7 )
#define F280X_SCIFFRX_RX_FIFO_INTLVL_8           ( 8 )
#define F280X_SCIFFRX_RX_FIFO_INTLVL_9           ( 9 )
#define F280X_SCIFFRX_RX_FIFO_INTLVL_10          ( 10 )
#define F280X_SCIFFRX_RX_FIFO_INTLVL_11          ( 11 )
#define F280X_SCIFFRX_RX_FIFO_INTLVL_12          ( 12 )
#define F280X_SCIFFRX_RX_FIFO_INTLVL_13          ( 13 )
#define F280X_SCIFFRX_RX_FIFO_INTLVL_14          ( 14 )
#define F280X_SCIFFRX_RX_FIFO_INTLVL_15          ( 15 )
#define F280X_SCIFFRX_TX_FIFO_INTLVL_16          ( 16 )
#define F280X_SCIFFRX_RX_FIFO_INTLVL_BITPOS      ( XT_BITPOS0 )

// SCIRXBUF
#define F280X_SCIRXBUF_SCIFFFE_IS_CLR             ( 0 )  // Read only
#define F280X_SCIRXBUF_SCIFFFE_IS_SET             ( 1 )
#define F280X_SCIRXBUF_SCIFFPE_IS_CLR             ( 0 )  // Read only
#define F280X_SCIRXBUF_SCIFFPE_IS_SET             ( 1 )

// SCIFFCT
#define F280X_SCIFFCT_ABD_FLAG_IS_CLR            ( 0 )  // Read only
#define F280X_SCIFFCT_ABD_FLAG_IS_SET            ( 1 )
#define F280X_SCIFFCT_ABD_FLAG_BITPOS            ( XT_BITPOS15 )

#define F280X_SCIFFCT_CLEAR_ABD_FLAG             ( 1 )  // Write only
#define F280X_SCIFFCT_CLEAR_ABD_BITPOS           ( XT_BITPOS14 )

#define F280X_SCIFFCT_CDC_DIS                    ( 0 )  // Read/Write
#define F280X_SCIFFCT_CDC_EN                     ( 1 )
#define F280X_SCIFFCT_CDC_BITPOS                 ( XT_BITPOS13 )

#define F280X_SCIFFCT_FIFO_DLY_BIT0              ( 0x01 )  // Read/Write
#define F280X_SCIFFCT_FIFO_DLY_BIT1              ( 0x02 )
#define F280X_SCIFFCT_FIFO_DLY_BIT2              ( 0x04 )
#define F280X_SCIFFCT_FIFO_DLY_BIT3              ( 0x08 )
#define F280X_SCIFFCT_FIFO_DLY_BIT4              ( 0x10 )
#define F280X_SCIFFCT_FIFO_DLY_BIT5              ( 0x20 )
#define F280X_SCIFFCT_FIFO_DLY_BIT6              ( 0x40 )
#define F280X_SCIFFCT_FIFO_DLY_BIT7              ( 0x80 )

// SCIPRI
#define F280X_SCIPRI_SUSMODE_STOP                ( 0 )     // Read/Write
#define F280X_SCIPRI_SUSMODE_FREE                ( 1 )
#define F280X_SCIPRI_SUSMODE_SOFT                ( 2 )
#define F280X_SCIPRI_SUSMODE_BITPOS              ( XT_BITPOS3 )

// EV timer interrupt masks
#define F280X_PIE_T1PINT_MASK    ( 0x08 )
#define F280X_PIE_T1CINT_MASK    ( 0x10 )
#define F280X_PIE_T1UFINT_MASK   ( 0x20 )
#define F280X_PIE_T1OFINT_MASK   ( 0x40 )
#define F280X_PIE_T2PINT_MASK    ( 0x01 )
#define F280X_PIE_T2CINT_MASK    ( 0x02 )
#define F280X_PIE_T2UFINT_MASK   ( 0x04 )
#define F280X_PIE_T2OFINT_MASK   ( 0x08 )
#define F280X_PIE_T3PINT_MASK    ( 0x08 )
#define F280X_PIE_T3CINT_MASK    ( 0x10 )
#define F280X_PIE_T3UFINT_MASK   ( 0x20 )
#define F280X_PIE_T3OFINT_MASK   ( 0x40 )
#define F280X_PIE_T4PINT_MASK    ( 0x01 )
#define F280X_PIE_T4CINT_MASK    ( 0x02 )
#define F280X_PIE_T4UFINT_MASK   ( 0x04 )
#define F280X_PIE_T4OFINT_MASK   ( 0x08 )
#define F280X_PIE_T1_ALL_MASK    ( 0x78 )
#define F280X_PIE_T2_ALL_MASK    ( 0x0F )
#define F280X_PIE_T3_ALL_MASK    ( 0x78 )
#define F280X_PIE_T4_ALL_MASK    ( 0x0F )

// global interrupt enable register (IER) - timer bit
#define F280X_IER_INT2_ENA        ( XT_BIT1 )
#define F280X_IER_INT3_ENA        ( XT_BIT2 )
#define F280X_IER_INT4_ENA        ( XT_BIT3 )
#define F280X_IER_INT5_ENA        ( XT_BIT4 )

// gpio direction control register bits for timer compare output
#define F280X_GPADIR_T1DIR_OUTPUT    ( XT_BIT6 )
#define F280X_GPADIR_T2DIR_OUTPUT    ( XT_BIT7 )
#define F280X_GPBDIR_T3DIR_OUTPUT    ( XT_BIT6 )
#define F280X_GPBDIR_T4DIR_OUTPUT    ( XT_BIT7 )

// GP timer interrupt flag bits
// need to be shifted the correct number of bits to match EV and timer number
#define F280X_EVXIFR_NO_INT          ( 0 )
#define F280X_EVXIFR_TXPINT          ( 1 << XT_BITPOS0 )
#define F280X_EVXIFR_TXCINT          ( 1 << XT_BITPOS1 )
#define F280X_EVXIFR_TXUFINT         ( 1 << XT_BITPOS2 )
#define F280X_EVXIFR_TXOFINT         ( 1 << XT_BITPOS3 )

#define F280X_EVAIFRA_T1PINT          ( 1 << XT_BITPOS7 )
#define F280X_EVAIFRA_T1CINT          ( 1 << XT_BITPOS8 )
#define F280X_EVAIFRA_T1UFINT         ( 1 << XT_BITPOS9 )
#define F280X_EVAIFRA_T1OFINT         ( 1 << XT_BITPOS10 )

#define F280X_EVAIFRB_T2PINT          ( 1 << XT_BITPOS0 )
#define F280X_EVAIFRB_T2CINT          ( 1 << XT_BITPOS1 )
#define F280X_EVAIFRB_T2UFINT         ( 1 << XT_BITPOS2 )
#define F280X_EVAIFRB_T2OFINT         ( 1 << XT_BITPOS3 )

#define F280X_EVBIFRA_T3PINT          ( 1 << XT_BITPOS7 )
#define F280X_EVBIFRA_T3CINT          ( 1 << XT_BITPOS8 )
#define F280X_EVBIFRA_T3UFINT         ( 1 << XT_BITPOS9 )
#define F280X_EVBIFRA_T3OFINT         ( 1 << XT_BITPOS10 )

#define F280X_EVBIFRB_T4PINT          ( 1 << XT_BITPOS0 )
#define F280X_EVBIFRB_T4CINT          ( 1 << XT_BITPOS1 )
#define F280X_EVBIFRB_T4UFINT         ( 1 << XT_BITPOS2 )
#define F280X_EVBIFRB_T4OFINT         ( 1 << XT_BITPOS3 )

// GP timer control reg A masks
// GP timer conrol reg A masks
#define F280X_GPTCONA_T1TOADC_MSK    ( XT_BIT7 & XT_BIT8 )
#define F280X_GPTCONA_T2TOADC_MSK    ( XT_BIT9 & XT_BIT10 )
#define F280X_GPTCONB_T3TOADC_MSK    ( XT_BIT7 & XT_BIT8 )
#define F280X_GPTCONB_T4TOADC_MSK    ( XT_BIT9 & XT_BIT10 )

// GP timer compare output polarity bit masks
#define F280X_GPTCONA_T1PIN_MSK      ( XT_BIT0 & XT_BIT1 )
#define F280X_GPTCONA_T2PIN_MSK      ( XT_BIT2 & XT_BIT3 )
#define F280X_GPTCONB_T3PIN_MSK      ( XT_BIT0 & XT_BIT1 )
#define F280X_GPTCONB_T4PIN_MSK      ( XT_BIT2 & XT_BIT3 )

// GP timer control bits
#define F280X_GPTCON_TXTOADC_DISAB    ( 0x0000 )
#define F280X_GPTCON_TXTOADC_COMP     ( XT_BIT0 & XT_BIT1 )
#define F280X_GPTCON_TXTOADC_PRD      ( XT_BIT1 )
#define F280X_GPTCON_TXTOADC_UNDER    ( XT_BIT0 )
#define F280X_GPTCON_TCOMPOE_EN       ( XT_BIT6 )
#define F280X_GPTCON_TXPIN_DISAB      ( 0x0000 )
#define F280X_GPTCON_TXPIN_FRC_LOW    ( 0x0000 )
#define F280X_GPTCON_TXPIN_FRC_HI     ( XT_BIT0 & XT_BIT1 )
#define F280X_GPTCON_TXPIN_ACT_LOW    ( XT_BIT0 )
#define F280X_GPTCON_TXPIN_ACT_HI     ( XT_BIT1 )

// EV timer control bits
#define F280X_TXCON_NO_COMP           ( 0x0000 )
#define F280X_TXCON_ZERO_RELOAD       ( 0x0000 )
#define F280X_TXCON_EQUAL_RELOAD      ( XT_BIT2 )
#define F280X_TXCON_IMM_RELOAD        ( XT_BIT3 )

// EV timer clock source types
#define F280X_TXCON_TCLK_INT          ( 0x0000 )
#define F280X_TXCON_TCLK_EXT          ( XT_BIT4 )
#define F280X_TXCON_TCLK_QEP          ( XT_BIT4 & XT_BIT5 )

// EV timer sync mode bits
#define F280X_TXCON_SYN_MSK           ( XT_BIT7 & XT_BIT0 )
#define F280X_TXCON_SYN_SEP           ( 0x0000 )
#define F280X_TXCON_SYN_PRD           ( XT_BIT0 )
#define F280X_TXCON_SYN_START         ( XT_BIT7 )
#define F280X_TXCON_SYN_PRDST         ( XT_BIT7 & XT_BIT0 )

// GP timer control masks
// Added HM - Nov 19, 2004
#define F280X_TXCON_EMMCTRL_MSK  ( XT_BIT14 & XT_BIT15 )//Emulation Control
#define F280X_TXCON_TMODE_MSK    ( XT_BIT11 & XT_BIT12 )//Count Mode
#define F280X_TXCON_TPS_MSK      ( XT_BIT8 & XT_BIT9 & XT_BIT10 )//Clk Prescaler
#define F280X_TXCON_TXSWTX_MSK   ( XT_BIT7 )//Timer start
#define F280X_TXCON_TENABLE_MASK ( XT_BIT6 )//Timer Enable
#define F280X_TXCON_TCLKS_MSK    ( XT_BIT4 & XT_BIT5 )//Clock Source Select
#define F280X_TXCON_TCLD_MSK     ( XT_BIT2 & XT_BIT3 )//Timer Compare Reg Reload
#define F280X_TXCON_TECMPR_MSK   ( XT_BIT1 )//Timer compare enable
#define F280X_TXCON_SELTXPR_MSK  ( XT_BIT0 )//Timer 1 period select

// GP timer emulation control bits
#define F280X_TXCON_EMMCTRL_NO_EFFECT3       ( 0xC000 )
#define F280X_TXCON_EMMCTRL_NO_EFFECT2       ( 0x8000 )
#define F280X_TXCON_EMMCTRL_STOP_PER_COMP    ( 0x4000 )
#define F280X_TXCON_EMMCTRL_STOP_IMMED       ( 0x0000 )

// timer count mode bits (11 & 12)
#define F280X_TXCON_TMODE_CONTUPDOWN  ( XT_BIT11 )                // 0x0800
#define F280X_TXCON_TMODE_CONTUP      ( XT_BIT12 )                // 0x1000
#define F280X_TXCON_TMODE_DIRUPDOWN   ( XT_BIT11 & XT_BIT12 )     // 0x1800
#define F280X_TXCON_TMODE_STOPHOLD    ( 0x0000 )                  // 0x0000

// timer enable bit positions
#define F280X_T1CON_TENABLE_EN    ( XT_BIT6 )
#define F280X_T2CON_TENABLE_EN    ( XT_BIT6 )
#define F280X_T3CON_TENABLE_EN    ( XT_BIT6 )
#define F280X_T4CON_TENABLE_EN    ( XT_BIT6 )

// timer prescaler bits
#define F280X_T1CON_TPS_MSK       ( XT_BIT8 & XT_BIT9 & XT_BIT10 )
#define F280X_T2CON_TPS_MSK       ( XT_BIT8 & XT_BIT9 & XT_BIT10 )
#define F280X_T3CON_TPS_MSK       ( XT_BIT8 & XT_BIT9 & XT_BIT10 )
#define F280X_T4CON_TPS_MSK       ( XT_BIT8 & XT_BIT9 & XT_BIT10 )

// timer prescaler register shift count
#define F280X_T1CON_TPS0          ( XT_BITPOS8 )
#define F280X_T2CON_TPS0          ( XT_BITPOS8 )
#define F280X_T3CON_TPS0          ( XT_BITPOS8 )
#define F280X_T4CON_TPS0          ( XT_BITPOS8 )

//GPIO General
//
#define F280X_GPIO_PIN_DIR_OUTPUT     ( 1 )
#define F280X_GPIO_PIN_DIR_INPUT      ( 0 )

// GPIO A Control Register bit masks
// NOTE: GPADIR, GPADAT, GPASET, GPACLEAR, GPATOGGLE registers have the same bit
// to I/O signal mapping as the GPAMUX
#define F280X_GPAMUX_MSK_PWM1        ( XT_BIT0 )
#define F280X_GPAMUX_MSK_PWM2        ( XT_BIT1 )
#define F280X_GPAMUX_MSK_PWM3        ( XT_BIT2 )
#define F280X_GPAMUX_MSK_PWM4        ( XT_BIT3 )
#define F280X_GPAMUX_MSK_PWM5        ( XT_BIT4 )
#define F280X_GPAMUX_MSK_PWM6        ( XT_BIT5 )
#define F280X_GPAMUX_MSK_T1PWM_T1CMP ( XT_BIT6 )
#define F280X_GPAMUX_MSK_T2PWM_T2CMP ( XT_BIT7 )
#define F280X_GPAMUX_MSK_CAP1_QEP1   ( XT_BIT8 )
#define F280X_GPAMUX_MSK_CAP2_QEP2   ( XT_BIT9 )
#define F280X_GPAMUX_MSK_CAP3_QEPI1  ( XT_BIT10 )
#define F280X_GPAMUX_MSK_TDIRA       ( XT_BIT11 )
#define F280X_GPAMUX_MSK_TCLKINA     ( XT_BIT12 )
#define F280X_GPAMUX_MSK_C1TRIP      ( XT_BIT13 )
#define F280X_GPAMUX_MSK_C2TRIP      ( XT_BIT14 )
#define F280X_GPAMUX_MSK_C3TRIP      ( XT_BIT15 )

// GPIO B Control Register bit masks
// *NOTE: GPBDIR, GPBDAT, GPBSET, GPBCLEAR, GPBTOGGLE registers have the same bit
// to I/O signal mapping as the GPBMUX (*manual does not explicitly state this)
#define F280X_GPBMUX_MSK_PWM7        ( XT_BIT0 )
#define F280X_GPBMUX_MSK_PWM8        ( XT_BIT1 )
#define F280X_GPBMUX_MSK_PWM9        ( XT_BIT2 )
#define F280X_GPBMUX_MSK_PWM10       ( XT_BIT3 )
#define F280X_GPBMUX_MSK_PWM11       ( XT_BIT4 )
#define F280X_GPBMUX_MSK_PWM12       ( XT_BIT5 )
#define F280X_GPBMUX_MSK_T3PWM_T3CMP ( XT_BIT6 )
#define F280X_GPBMUX_MSK_T4PWM_T4CMP ( XT_BIT7 )
#define F280X_GPBMUX_MSK_CAP4_QEP3   ( XT_BIT8 )
#define F280X_GPBMUX_MSK_CAP5_QEP4   ( XT_BIT9 )
#define F280X_GPBMUX_MSK_CAP6_QEPI2  ( XT_BIT10 )
#define F280X_GPBMUX_MSK_TDIRB       ( XT_BIT11 )
#define F280X_GPBMUX_MSK_TCLKINB     ( XT_BIT12 )
#define F280X_GPBMUX_MSK_C4TRIP      ( XT_BIT13 )
#define F280X_GPBMUX_MSK_C5TRIP      ( XT_BIT14 )
#define F280X_GPBMUX_MSK_C6TRIP      ( XT_BIT15 )

// EVA/B Dead-Band Timer Control Register bit masks (DBTCON)
// NOTE: bits 0 & 1 reserved
#define F280X_DBTCON_MSK_DBTPS0      ( XT_BIT2 )
#define F280X_DBTCON_MSK_DBTPS1      ( XT_BIT3 )
#define F280X_DBTCON_MSK_DBTPS2      ( XT_BIT4 )
#define F280X_DBTCON_MSK_EDBT1       ( XT_BIT5 )
#define F280X_DBTCON_MSK_EDBT2       ( XT_BIT6 )
#define F280X_DBTCON_MSK_EDBT3       ( XT_BIT7 )
#define F280X_DBTCON_MSK_DBT0        ( XT_BIT8 )
#define F280X_DBTCON_MSK_DBT1        ( XT_BIT9 )
#define F280X_DBTCON_MSK_DBT2        ( XT_BIT10 )
#define F280X_DBTCON_MSK_DBT3        ( XT_BIT11 )
// NOTE: bits 12-15 reserved

// EVA/B Dead-Band Timer Control Register bit positions (DBTCON)
// NOTE: bits 0 & 1 reserved
#define F280X_DBTCON_POS_DBTPS0      ( XT_BITPOS2 )
#define F280X_DBTCON_POS_DBTPS1      ( XT_BITPOS3 )
#define F280X_DBTCON_POS_DBTPS2      ( XT_BITPOS4 )
#define F280X_DBTCON_POS_EDBT1       ( XT_BITPOS5 )
#define F280X_DBTCON_POS_EDBT2       ( XT_BITPOS6 )
#define F280X_DBTCON_POS_EDBT3       ( XT_BITPOS7 )
#define F280X_DBTCON_POS_DBT0        ( XT_BITPOS8 )
#define F280X_DBTCON_POS_DBT1        ( XT_BITPOS9 )
#define F280X_DBTCON_POS_DBT2        ( XT_BITPOS10 )
#define F280X_DBTCON_POS_DBT3        ( XT_BITPOS11 )
// NOTE: bits 12-15 reserved

// All Dead-Band Timer enable bits mask
#define F280X_DBTCON_MSK_EDBT        ( 7 << F280X_DBTCON_POS_EDBT1 )
// All Dead-Band Timer prescaler bits mask
#define F280X_DBTCON_MSK_DBTPS       ( 7 << F280X_DBTCON_POS_DBTPS0 )

// Use these defines to configure the DBTCON register
//15 dead-band timer periods
#define F280X_DBTCON_DBT_15          ( 15 << F280X_DBTCON_POS_DBT0 )
//14 dead-band timer periods
#define F280X_DBTCON_DBT_14          ( 14 << F280X_DBTCON_POS_DBT0 )
//13 dead-band timer periods
#define F280X_DBTCON_DBT_13          ( 13 << F280X_DBTCON_POS_DBT0 )
//12 dead-band timer periods
#define F280X_DBTCON_DBT_12          ( 12 << F280X_DBTCON_POS_DBT0 )
//11 dead-band timer periods
#define F280X_DBTCON_DBT_11          ( 11 << F280X_DBTCON_POS_DBT0 )
//10 dead-band timer periods
#define F280X_DBTCON_DBT_10          ( 10 << F280X_DBTCON_POS_DBT0 )
//9 dead-band timer periods
#define F280X_DBTCON_DBT_9           ( 9 << F280X_DBTCON_POS_DBT0 )
//8 dead-band timer periods
#define F280X_DBTCON_DBT_8           ( 8 << F280X_DBTCON_POS_DBT0 )
//7 dead-band timer periods
#define F280X_DBTCON_DBT_7           ( 7 << F280X_DBTCON_POS_DBT0 )
//6 dead-band timer periods
#define F280X_DBTCON_DBT_6           ( 6 << F280X_DBTCON_POS_DBT0 )
//5 dead-band timer periods
#define F280X_DBTCON_DBT_5           ( 5 << F280X_DBTCON_POS_DBT0 )
//4 dead-band timer periods
#define F280X_DBTCON_DBT_4           ( 4 << F280X_DBTCON_POS_DBT0 )
//3 dead-band timer periods
#define F280X_DBTCON_DBT_3           ( 3 << F280X_DBTCON_POS_DBT0 )
//2 dead-band timer periods
#define F280X_DBTCON_DBT_2           ( 2 << F280X_DBTCON_POS_DBT0 )
//1 dead-band timer periods
#define F280X_DBTCON_DBT_1           ( 1 << F280X_DBTCON_POS_DBT0 )
//0 dead-band timer periods
#define F280X_DBTCON_DBT_0           ( 0 << F280X_DBTCON_POS_DBT0 )
//Dead-band tmr 3 en
#define F280X_DBTCON_EDBT3_EN        ( 1 << F280X_DBTCON_POS_EDBT3 )
//Dead-band tmr 3 dis
#define F280X_DBTCON_EDBT3_DIS       ( 0 << F280X_DBTCON_POS_EDBT3 )
//Dead-band tmr 2 en
#define F280X_DBTCON_EDBT2_EN        ( 1 << F280X_DBTCON_POS_EDBT2 )
//Dead-band tmr 2 dis
#define F280X_DBTCON_EDBT2_DIS       ( 0 << F280X_DBTCON_POS_EDBT2 )
//Dead-band tmr 1 en
#define F280X_DBTCON_EDBT1_EN        ( 1 << F280X_DBTCON_POS_EDBT1 )
//Dead-band tmr 1 dis
#define F280X_DBTCON_EDBT1_DIS       ( 0 << F280X_DBTCON_POS_EDBT1 )
//Dead-band timer / 32 prescaler
#define F280X_DBTCON_DBTPS_32        ( 5 << F280X_DBTCON_POS_DBTPS0 )
//Dead-band timer / 16 prescaler
#define F280X_DBTCON_DBTPS_16        ( 4 << F280X_DBTCON_POS_DBTPS0 )
//Dead-band timer / 8 prescaler
#define F280X_DBTCON_DBTPS_8         ( 3 << F280X_DBTCON_POS_DBTPS0 )
//Dead-band timer / 4 prescaler
#define F280X_DBTCON_DBTPS_4         ( 2 << F280X_DBTCON_POS_DBTPS0 )
//Dead-band timer / 2 prescaler
#define F280X_DBTCON_DBTPS_2         ( 1 << F280X_DBTCON_POS_DBTPS0 )
//Dead-band timer / 1 prescaler
#define F280X_DBTCON_DBTPS_1         ( 0 << F280X_DBTCON_POS_DBTPS0 )

// EVA/B Compare Control Register bit masks (COMCON)
#define F280X_COMCON_MSK_C1TRIPE     ( XT_BIT0 )
#define F280X_COMCON_MSK_C2TRIPE     ( XT_BIT1 )
#define F280X_COMCON_MSK_C3TRIPE     ( XT_BIT2 )
// NOTE: bits 3 & 4 reserved
#define F280X_COMCON_MSK_FCMP1OE     ( XT_BIT5 )
#define F280X_COMCON_MSK_FCMP2OE     ( XT_BIT6 )
#define F280X_COMCON_MSK_FCMP3OE     ( XT_BIT7 )
#define F280X_COMCON_MSK_PDPINT      ( XT_BIT8 )
#define F280X_COMCON_MSK_FCMPOE      ( XT_BIT9 )
#define F280X_COMCON_MSK_ACTRLD0     ( XT_BIT10 )
#define F280X_COMCON_MSK_ACTRLD1     ( XT_BIT11 )
#define F280X_COMCON_MSK_SVENABLE    ( XT_BIT12 )
#define F280X_COMCON_MSK_CLD0        ( XT_BIT13 )
#define F280X_COMCON_MSK_CLD1        ( XT_BIT14 )
#define F280X_COMCON_MSK_CENABLE     ( XT_BIT15 )

// EVA/B Compare Control Register bit positions (COMCON)
#define F280X_COMCON_POS_C1TRIPE     ( XT_BITPOS0 )
#define F280X_COMCON_POS_C2TRIPE     ( XT_BITPOS1 )
#define F280X_COMCON_POS_C3TRIPE     ( XT_BITPOS2 )
// NOTE: bits 3 & 4 reserved
#define F280X_COMCON_POS_FCMP1OE     ( XT_BITPOS5 )
#define F280X_COMCON_POS_FCMP2OE     ( XT_BITPOS6 )
#define F280X_COMCON_POS_FCMP3OE     ( XT_BITPOS7 )
#define F280X_COMCON_POS_PDPINT      ( XT_BITPOS8 )
#define F280X_COMCON_POS_FCMPOE      ( XT_BITPOS9 )
#define F280X_COMCON_POS_ACTRLD0     ( XT_BITPOS10 )
#define F280X_COMCON_POS_ACTRLD1     ( XT_BITPOS11 )
#define F280X_COMCON_POS_SVENABLE    ( XT_BITPOS12 )
#define F280X_COMCON_POS_CLD0        ( XT_BITPOS13 )
#define F280X_COMCON_POS_CLD1        ( XT_BITPOS14 )
#define F280X_COMCON_POS_CENABLE     ( XT_BITPOS15 )

// Use these defines to configure the COMCON register
//Compare enable
#define F280X_COMCON_CENABLE_EN      ( 1 << F280X_COMCON_POS_CENABLE )
//Compare disable
#define F280X_COMCON_CENABLE_DIS     ( 0 << F280X_COMCON_POS_CENABLE )
//Reserved by TI
#define F280X_COMCON_CLD_RESERVED    ( 3 << F280X_COMCON_POS_CLD0 )
//Reload CMPRx immediately
#define F280X_COMCON_CLD_IMM         ( 2 << F280X_COMCON_POS_CLD0 )
//Reload CMPRx on T1CNT0 or period
#define F280X_COMCON_CLD_UF_PD       ( 1 << F280X_COMCON_POS_CLD0 )
//Reload CMPRx on T1CNT0
#define F280X_COMCON_CLD_UF          ( 0 << F280X_COMCON_POS_CLD0 )
//Enable space vector PWM
#define F280X_COMCON_SVENABLE_EN     ( 1 << F280X_COMCON_POS_SVENABLE )
//Disable space vector PWM
#define F280X_COMCON_SVENABLE_DIS    ( 0 << F280X_COMCON_POS_SVENABLE )
//Reserved by TI
#define F280X_COMCON_ACTRLD_RESERVED ( 3 << F280X_COMCON_POS_ACTRLD0 )
//Reload action reg immediately
#define F280X_COMCON_ACTRLD_IMM      ( 2 << F280X_COMCON_POS_ACTRLD0 )
//Reload action reg on T1CNT0 or per
#define F280X_COMCON_ACTRLD_UF_PD    ( 1 << F280X_COMCON_POS_ACTRLD0 )
//Reload action reg on T1CNT0
#define F280X_COMCON_ACTRLD_UF       ( 0 << F280X_COMCON_POS_ACTRLD0 )
//PWM output pins are not HI-Z (en)
#define F280X_COMCON_FCMPOE_EN       ( 1 << F280X_COMCON_POS_FCMPOE )
//PWM output pins are HI-Z (dis)
#define F280X_COMCON_FCMPOE_DIS      ( 0 << F280X_COMCON_POS_FCMPOE )

// Compare Action Control Register bit positions (ACTRA)
#define F280X_ACTR_POS_SVRDIR        ( XT_BITPOS15 )
#define F280X_ACTR_POS_SVB_D2        ( XT_BITPOS14 )
#define F280X_ACTR_POS_SVB_D1        ( XT_BITPOS13 )
#define F280X_ACTR_POS_SVB_D0        ( XT_BITPOS12 )
#define F280X_ACTR_POS_CMP12ACT1     ( XT_BITPOS11 )
#define F280X_ACTR_POS_CMP12ACT0     ( XT_BITPOS10 )
#define F280X_ACTR_POS_CMP11ACT1     ( XT_BITPOS9 )
#define F280X_ACTR_POS_CMP11ACT0     ( XT_BITPOS8 )
#define F280X_ACTR_POS_CMP10ACT1     ( XT_BITPOS7 )
#define F280X_ACTR_POS_CMP10ACT0     ( XT_BITPOS6 )
#define F280X_ACTR_POS_CMP9ACT1      ( XT_BITPOS5 )
#define F280X_ACTR_POS_CMP9ACT0      ( XT_BITPOS4 )
#define F280X_ACTR_POS_CMP8ACT1      ( XT_BITPOS3 )
#define F280X_ACTR_POS_CMP8ACT0      ( XT_BITPOS2 )
#define F280X_ACTR_POS_CMP7ACT1      ( XT_BITPOS1 )
#define F280X_ACTR_POS_CMP7ACT0      ( XT_BITPOS0 )
#define F280X_ACTR_POS_CMP6ACT1      ( XT_BITPOS11 )
#define F280X_ACTR_POS_CMP6ACT0      ( XT_BITPOS10 )
#define F280X_ACTR_POS_CMP5ACT1      ( XT_BITPOS9 )
#define F280X_ACTR_POS_CMP5ACT0      ( XT_BITPOS8 )
#define F280X_ACTR_POS_CMP4ACT1      ( XT_BITPOS7 )
#define F280X_ACTR_POS_CMP4ACT0      ( XT_BITPOS6 )
#define F280X_ACTR_POS_CMP3ACT1      ( XT_BITPOS5 )
#define F280X_ACTR_POS_CMP3ACT0      ( XT_BITPOS4 )
#define F280X_ACTR_POS_CMP2ACT1      ( XT_BITPOS3 )
#define F280X_ACTR_POS_CMP2ACT0      ( XT_BITPOS2 )
#define F280X_ACTR_POS_CMP1ACT1      ( XT_BITPOS1 )
#define F280X_ACTR_POS_CMP1ACT0      ( XT_BITPOS0 )

//
// 280X External Memory Interface
//

// XINTF Configuration Register

// XTIMCLK divisor
#define F280X_XINTFCNF2_XTIMCLK_SYSCLKOUT_DIV_2          ( 1 )
#define F280X_XINTFCNF2_XTIMCLK_SYSCLKOUT_DIV_1          ( 0 )

// External bus access
#define F280X_XINTFCNF2_HOLD_AUTO_GRANT                  ( 0 )
#define F280X_XINTFCNF2_HOLD_NO_GRANT                    ( 1 )

// Microprocessor / Microcomputer mode
#define F280X_XINTFCNF2_MPNMC_MICROPROCESSOR_MODE        ( 1 )
#define F280X_XINTFCNF2_MPNMC_MICROCOMPUTER_MODE         ( 0 )

// XCLKOUT
#define F280X_XINTFCNF2_CLKOFF_XCLKOUT_ENABLE            ( 0 )
#define F280X_XINTFCNF2_CLKOFF_XCLKOUT_DISABLE           ( 1 )

// Clock Mode
#define F280X_XINTFCNF2_CLKMODE_XTIMCLK_DIV_2            ( 1 )
#define F280X_XINTFCNF2_CLKMODE_XTIMCLK_DIV_1            ( 0 )

// Write Buffer Depth
#define F280X_XINTFCNF2_WRITEBUFFER_EMPTY                ( 0 )
#define F280X_XINTFCNF2_WRITEBUFFER_0                    ( 0 )
#define F280X_XINTFCNF2_WRITEBUFFER_1                    ( 1 )
#define F280X_XINTFCNF2_WRITEBUFFER_2                    ( 2 )
#define F280X_XINTFCNF2_WRITEBUFFER_3                    ( 3 )
#define F280X_XINTFCNF2_WRITEBUFFER_FULL                 ( 3 )

// XTIMINGx Register

// Wait State Scale Factor
#define F280X_XTIMINGX_X2TIMING_ENABLED                  ( 1 )
#define F280X_XTIMINGX_X2TIMING_DISABLED                 ( 0 )

// memory width
#define F280X_XTIMINGX_XSIZE_16BIT                       ( 3 )

// Ready Mode
#define F280X_XTIMINGX_XREADY_SYNCHRONOUS                ( 0 )
#define F280X_XTIMINGX_XREADY_ASYNCHRONOUS               ( 1 )

// Ready Signal
#define F280X_XTIMINGX_USEREADY_ENABLED                  ( 1 )
#define F280X_XTIMINGX_USEREADY_DISABLED                 ( 0 )

// External Memory Interface Wait States
#define F280X_XTIMINGX_WAIT_1_STATE                      ( 1 )
#define F280X_XTIMINGX_WAIT_2_STATES                     ( 2 )
#define F280X_XTIMINGX_WAIT_3_STATES                     ( 3 )
#define F280X_XTIMINGX_WAIT_4_STATES                     ( 4 )
#define F280X_XTIMINGX_WAIT_5_STATES                     ( 5 )
#define F280X_XTIMINGX_WAIT_6_STATES                     ( 6 )
#define F280X_XTIMINGX_WAIT_7_STATES                     ( 7 )

//
// Flash/OTP Configuration definitions
//

// Flash Options Register
#define F280X_FOPT_PIPELINE_ENABLED                      ( 1 )
#define F280X_FOPT_PIPELINE_DISABLED                     ( 0 )

// Flash Status Register
#define F280X_FSTATUS_CLEAR_V3STAT                       ( 1 )

// Flash Power Register
#define F280X_FPWR_PUMP_BANK_SLEEP                       ( 0 )
#define F280X_FPWR_PUMP_BANK_STDBY                       ( 1 )
#define F280X_FPWR_PUMP_BANK_ACTIVE                      ( 3 )

// Standby Wait Register
#define F280X_FSTDBYWAIT_MAX_WAIT                        ( 0x01FF )

// Active Wait Register
#define F280X_FACTIVEWAIT_MAX_WAIT                       ( 0x01FF )

// Bank Wait states
#define F280X_FBANKWAIT_0_STATES                         ( 0 )
#define F280X_FBANKWAIT_1_STATES                         ( 1 )
#define F280X_FBANKWAIT_2_STATES                         ( 2 )
#define F280X_FBANKWAIT_3_STATES                         ( 3 )
#define F280X_FBANKWAIT_4_STATES                         ( 4 )
#define F280X_FBANKWAIT_5_STATES                         ( 5 )
#define F280X_FBANKWAIT_6_STATES                         ( 6 )
#define F280X_FBANKWAIT_7_STATES                         ( 7 )
#define F280X_FBANKWAIT_8_STATES                         ( 8 )
#define F280X_FBANKWAIT_9_STATES                         ( 9 )
#define F280X_FBANKWAIT_10_STATES                        ( 10 )
#define F280X_FBANKWAIT_11_STATES                        ( 11 )
#define F280X_FBANKWAIT_12_STATES                        ( 12 )
#define F280X_FBANKWAIT_13_STATES                        ( 13 )
#define F280X_FBANKWAIT_14_STATES                        ( 14 )
#define F280X_FBANKWAIT_15_STATES                        ( 15 )



//------------------------------------------------------------------------------
//            BEGIN NFO AND MACRO USE DEFINITIONS
// All definitions below are used with the macros defined at the end of the file
//
// Notes:
//     NFO means Info
//     This convention was used to reduce the number of definitions for each
//     register.  This way we only have to define the field configuraiton once 
//     and use macros to extract bit mask combos.
//     It does not take up any processor time nor any more code because it is
//     all done using prepocessor definitions.
//     
//------------------------------------------------------------------------------

//
// GPIO module
//


//
// System Control and Interrupt module
//

// Peripheral Clock Control 0 (PCLKCR0)
#define F280X_PCLKCR0_TBCLKSYNC_NFO                        F280X_mNFO(  2,  1 )
#define F280X_PCLKCR0_ADCENCLK_NFO                         F280X_mNFO(  3,  1 )
#define F280X_PCLKCR0_I2CAENCLK_NFO                        F280X_mNFO(  4,  1 )
#define F280X_PCLKCR0_SPICENCLK_NFO                        F280X_mNFO(  6,  1 )
#define F280X_PCLKCR0_SPIDENCLK_NFO                        F280X_mNFO(  7,  1 )
#define F280X_PCLKCR0_SPIAENCLK_NFO                        F280X_mNFO(  8,  1 )
#define F280X_PCLKCR0_SPIBENCLK_NFO                        F280X_mNFO(  9,  1 )
#define F280X_PCLKCR0_SCIAENCLK_NFO                        F280X_mNFO( 10,  1 )
#define F280X_PCLKCR0_SCIBENCLK_NFO                        F280X_mNFO( 11,  1 )
#define F280X_PCLKCR0_ECANAENCLK_NFO                       F280X_mNFO( 14,  1 )
#define F280X_PCLKCR0_ECANBENCLK_NFO                       F280X_mNFO( 15,  1 )

#define F280X_PCLKCR0_TBCLKSYNC_DIS                        ( 0 )
#define F280X_PCLKCR0_TBCLKSYNC_ENA                        ( 1 )
#define F280X_PCLKCR0_ADCENCLK_DIS                         ( 0 )
#define F280X_PCLKCR0_ADCENCLK_ENA                         ( 1 )
#define F280X_PCLKCR0_I2CAENCLK_DIS                        ( 0 )
#define F280X_PCLKCR0_I2CAENCLK_ENA                        ( 1 )
#define F280X_PCLKCR0_SPICENCLK_DIS                        ( 0 )
#define F280X_PCLKCR0_SPICENCLK_ENA                        ( 1 )
#define F280X_PCLKCR0_SPIDENCLK_DIS                        ( 0 )
#define F280X_PCLKCR0_SPIDENCLK_ENA                        ( 1 )
#define F280X_PCLKCR0_SPIAENCLK_DIS                        ( 0 )
#define F280X_PCLKCR0_SPIAENCLK_ENA                        ( 1 )
#define F280X_PCLKCR0_SPIBENCLK_DIS                        ( 0 )
#define F280X_PCLKCR0_SPIBENCLK_ENA                        ( 1 )
#define F280X_PCLKCR0_SCIAENCLK_DIS                        ( 0 )
#define F280X_PCLKCR0_SCIAENCLK_ENA                        ( 1 )
#define F280X_PCLKCR0_SCIBENCLK_DIS                        ( 0 )
#define F280X_PCLKCR0_SCIBENCLK_ENA                        ( 1 )
#define F280X_PCLKCR0_ECANAENCLK_DIS                       ( 0 )
#define F280X_PCLKCR0_ECANAENCLK_ENA                       ( 1 )
#define F280X_PCLKCR0_ECANBENCLK_DIS                       ( 0 )
#define F280X_PCLKCR0_ECANBENCLK_ENA                       ( 1 )


// Peripheral Clock Control 1 (PCLKCR1)
#define F280X_PCLKCR1_EPWM1ENCLK_NFO                        F280X_mNFO(  0,  1 )
#define F280X_PCLKCR1_EPWM2ENCLK_NFO                        F280X_mNFO(  1,  1 )
#define F280X_PCLKCR1_EPWM3ENCLK_NFO                        F280X_mNFO(  2,  1 )
#define F280X_PCLKCR1_EPWM4ENCLK_NFO                        F280X_mNFO(  3,  1 )
#define F280X_PCLKCR1_EPWM5ENCLK_NFO                        F280X_mNFO(  4,  1 )
#define F280X_PCLKCR1_EPWM6ENCLK_NFO                        F280X_mNFO(  5,  1 )
#define F280X_PCLKCR1_ECAP1ENCLK_NFO                        F280X_mNFO(  8,  1 )
#define F280X_PCLKCR1_ECAP2ENCLK_NFO                        F280X_mNFO(  9,  1 )
#define F280X_PCLKCR1_ECAP3ENCLK_NFO                        F280X_mNFO( 10,  1 )
#define F280X_PCLKCR1_ECAP4ENCLK_NFO                        F280X_mNFO( 11,  1 )
#define F280X_PCLKCR1_EQEP1ENCLK_NFO                        F280X_mNFO( 14,  1 )
#define F280X_PCLKCR1_EQEP2ENCLK_NFO                        F280X_mNFO( 15,  1 )

#define F280X_PCLKCR1_EPWM1ENCLK_DIS                        ( 0 )
#define F280X_PCLKCR1_EPWM1ENCLK_ENA                        ( 1 )
#define F280X_PCLKCR1_EPWM2ENCLK_DIS                        ( 0 )
#define F280X_PCLKCR1_EPWM2ENCLK_ENA                        ( 1 )
#define F280X_PCLKCR1_EPWM3ENCLK_DIS                        ( 0 )
#define F280X_PCLKCR1_EPWM3ENCLK_ENA                        ( 1 )
#define F280X_PCLKCR1_EPWM4ENCLK_DIS                        ( 0 )
#define F280X_PCLKCR1_EPWM4ENCLK_ENA                        ( 1 )
#define F280X_PCLKCR1_EPWM5ENCLK_DIS                        ( 0 )
#define F280X_PCLKCR1_EPWM5ENCLK_ENA                        ( 1 )
#define F280X_PCLKCR1_EPWM6ENCLK_DIS                        ( 0 )
#define F280X_PCLKCR1_EPWM6ENCLK_ENA                        ( 1 )
#define F280X_PCLKCR1_ECAP1ENCLK_DIS                        ( 0 )
#define F280X_PCLKCR1_ECAP1ENCLK_ENA                        ( 1 )
#define F280X_PCLKCR1_ECAP2ENCLK_DIS                        ( 0 )
#define F280X_PCLKCR1_ECAP2ENCLK_ENA                        ( 1 )
#define F280X_PCLKCR1_ECAP3ENCLK_DIS                        ( 0 )
#define F280X_PCLKCR1_ECAP3ENCLK_ENA                        ( 1 )
#define F280X_PCLKCR1_ECAP4ENCLK_DIS                        ( 0 )
#define F280X_PCLKCR1_ECAP4ENCLK_ENA                        ( 1 )
#define F280X_PCLKCR1_EQEP1ENCLK_DIS                        ( 0 )
#define F280X_PCLKCR1_EQEP1ENCLK_ENA                        ( 1 )
#define F280X_PCLKCR1_EQEP1ENCLK_DIS                        ( 0 )
#define F280X_PCLKCR1_EQEP1ENCLK_ENA                        ( 1 )

// 
// PWM module bit definitions
//

// Time Base Control (TBCTL)
#define F280X_TBCTL_CTRMODE_NFO                             F280X_mNFO(  0,  2 )
#define F280X_TBCTL_PHSEN_NFO                               F280X_mNFO(  2,  1 )
#define F280X_TBCTL_PRDLD_NFO                               F280X_mNFO(  3,  1 )
#define F280X_TBCTL_SYNCOSEL_NFO                            F280X_mNFO(  4,  2 )
#define F280X_TBCTL_SWFSYNC_NFO                             F280X_mNFO(  6,  1 )
#define F280X_TBCTL_HSPCLKDIV_NFO                           F280X_mNFO(  7,  3 )
#define F280X_TBCTL_CLKDIV_NFO                              F280X_mNFO( 10,  3 )
#define F280X_TBCTL_PHSDIR_NFO                              F280X_mNFO( 13,  1 )
#define F280X_TBCTL_FREESOFT_NFO                            F280X_mNFO( 14,  2 )

#define F280X_TBCTL_CTRMODE_COUNT_UP                        ( 0 )
#define F280X_TBCTL_CTRMODE_COUNT_DN                        ( 1 )
#define F280X_TBCTL_CTRMODE_COUNT_UPDN                      ( 2 )
#define F280X_TBCTL_CTRMODE_FREEZE                          ( 3 )
#define F280X_TBCTL_PHSEN_DIS                               ( 0 )
#define F280X_TBCTL_PHSEN_ENA                               ( 1 )
#define F280X_TBCTL_PRDLD_SHDW                              ( 0 )
#define F280X_TBCTL_PRDLD_IMMED                             ( 1 )
#define F280X_TBCTL_SYNCOSEL_EPWMXSYNCI                     ( 0 )
#define F280X_TBCTL_SYNCOSEL_CTR_EQU_ZRO                    ( 1 )
#define F280X_TBCTL_SYNCOSEL_CTR_EQU_CMPB                   ( 2 )
#define F280X_TBCTL_SYNCOSEL_DIS                            ( 3 )
#define F280X_TBCTL_SWFSYNC_SYNC                            ( 1 )
#define F280X_TBCTL_HSPCLKDIV_DIVBY_1                       ( 0 )
#define F280X_TBCTL_HSPCLKDIV_DIVBY_2                       ( 1 )
#define F280X_TBCTL_HSPCLKDIV_DIVBY_4                       ( 2 )
#define F280X_TBCTL_HSPCLKDIV_DIVBY_6                       ( 3 )
#define F280X_TBCTL_HSPCLKDIV_DIVBY_8                       ( 4 )
#define F280X_TBCTL_HSPCLKDIV_DIVBY_10                      ( 5 )
#define F280X_TBCTL_HSPCLKDIV_DIVBY_12                      ( 6 )
#define F280X_TBCTL_HSPCLKDIV_DIVBY_14                      ( 7 )
#define F280X_TBCTL_CLKDIV_DIVBY_1                          ( 0 )
#define F280X_TBCTL_CLKDIV_DIVBY_2                          ( 1 )
#define F280X_TBCTL_CLKDIV_DIVBY_4                          ( 2 )
#define F280X_TBCTL_CLKDIV_DIVBY_8                          ( 3 )
#define F280X_TBCTL_CLKDIV_DIVBY_16                         ( 4 )
#define F280X_TBCTL_CLKDIV_DIVBY_32                         ( 5 )
#define F280X_TBCTL_CLKDIV_DIVBY_64                         ( 6 )
#define F280X_TBCTL_CLKDIV_DIVBY_128                        ( 7 )
#define F280X_TBCTL_PHSDIR_COUNT_DN                         ( 0 )
#define F280X_TBCTL_PHSDIR_COUNT_UP                         ( 1 )
#define F280X_TBCTL_FREESOFT_STOP_NEXT_CNTR                 ( 0 )
#define F280X_TBCTL_FREESOFT_STOP_NEXT_CYCLE                ( 1 )
#define F280X_TBCTL_FREESOFT_FREE                           ( 2 )

// Action-qualifier Output A control Register (AQCTLA)
#define F280X_AQCTLA_ZRO_NFO                                F280X_mNFO(  0,  2 )
#define F280X_AQCTLA_PRD_NFO                                F280X_mNFO(  2,  2 )
#define F280X_AQCTLA_CAU_NFO                                F280X_mNFO(  4,  2 )
#define F280X_AQCTLA_CAD_NFO                                F280X_mNFO(  6,  2 )
#define F280X_AQCTLA_CBU_NFO                                F280X_mNFO(  8,  2 )
#define F280X_AQCTLA_CBD_NFO                                F280X_mNFO(  10, 2 )

#define F280X_AQCTLA_ZRO_DIS                                ( 0 )
#define F280X_AQCTLA_ZRO_CLR                                ( 1 )
#define F280X_AQCTLA_ZRO_SET                                ( 2 )
#define F280X_AQCTLA_ZRO_TOG                                ( 3 )
#define F280X_AQCTLA_PRD_DIS                                ( 0 )
#define F280X_AQCTLA_PRD_CLR                                ( 1 )
#define F280X_AQCTLA_PRD_SET                                ( 2 )
#define F280X_AQCTLA_PRD_TOG                                ( 3 )
#define F280X_AQCTLA_CAU_DIS                                ( 0 )
#define F280X_AQCTLA_CAU_CLR                                ( 1 )
#define F280X_AQCTLA_CAU_SET                                ( 2 )
#define F280X_AQCTLA_CAU_TOG                                ( 3 )
#define F280X_AQCTLA_CAD_DIS                                ( 0 )
#define F280X_AQCTLA_CAD_CLR                                ( 1 )
#define F280X_AQCTLA_CAD_SET                                ( 2 )
#define F280X_AQCTLA_CAD_TOG                                ( 3 )
#define F280X_AQCTLA_CBU_DIS                                ( 0 )
#define F280X_AQCTLA_CBU_CLR                                ( 1 )
#define F280X_AQCTLA_CBU_SET                                ( 2 )
#define F280X_AQCTLA_CBU_TOG                                ( 3 )
#define F280X_AQCTLA_CBD_DIS                                ( 0 )
#define F280X_AQCTLA_CBD_CLR                                ( 1 )
#define F280X_AQCTLA_CBD_SET                                ( 2 )
#define F280X_AQCTLA_CBD_TOG                                ( 3 )


// Action-qualifier Output B control Register (AQCTLB)
#define F280X_AQCTLB_ZRO_NFO                                F280X_mNFO(  0,  2 )
#define F280X_AQCTLB_PRD_NFO                                F280X_mNFO(  2,  2 )
#define F280X_AQCTLB_CAU_NFO                                F280X_mNFO(  4,  2 )
#define F280X_AQCTLB_CAD_NFO                                F280X_mNFO(  6,  2 )
#define F280X_AQCTLB_CBU_NFO                                F280X_mNFO(  8,  2 )
#define F280X_AQCTLB_CBD_NFO                                F280X_mNFO(  10, 2 )

#define F280X_AQCTLB_ZRO_DIS                                ( 0 )
#define F280X_AQCTLB_ZRO_CLR                                ( 1 )
#define F280X_AQCTLB_ZRO_SET                                ( 2 )
#define F280X_AQCTLB_ZRO_TOG                                ( 3 )
#define F280X_AQCTLB_PRD_DIS                                ( 0 )
#define F280X_AQCTLB_PRD_CLR                                ( 1 )
#define F280X_AQCTLB_PRD_SET                                ( 2 )
#define F280X_AQCTLB_PRD_TOG                                ( 3 )
#define F280X_AQCTLB_CAU_DIS                                ( 0 )
#define F280X_AQCTLB_CAU_CLR                                ( 1 )
#define F280X_AQCTLB_CAU_SET                                ( 2 )
#define F280X_AQCTLB_CAU_TOG                                ( 3 )
#define F280X_AQCTLB_CAD_DIS                                ( 0 )
#define F280X_AQCTLB_CAD_CLR                                ( 1 )
#define F280X_AQCTLB_CAD_SET                                ( 2 )
#define F280X_AQCTLB_CAD_TOG                                ( 3 )
#define F280X_AQCTLB_CBU_DIS                                ( 0 )
#define F280X_AQCTLB_CBU_CLR                                ( 1 )
#define F280X_AQCTLB_CBU_SET                                ( 2 )
#define F280X_AQCTLB_CBU_TOG                                ( 3 )
#define F280X_AQCTLB_CBD_DIS                                ( 0 )
#define F280X_AQCTLB_CBD_CLR                                ( 1 )
#define F280X_AQCTLB_CBD_SET                                ( 2 )
#define F280X_AQCTLB_CBD_TOG                                ( 3 )


// Action-qualifier S/W Force Register (AQSFRC)
#define F280X_AQSFRC_ACTSFA_NFO                             F280X_mNFO(  0,  2 )
#define F280X_AQSFRC_OTSFA_NFO                              F280X_mNFO(  2,  1 )
#define F280X_AQSFRC_ACTSFB_NFO                             F280X_mNFO(  3,  2 )
#define F280X_AQSFRC_OTSFB_NFO                              F280X_mNFO(  5,  1 )
#define F280X_AQSFRC_RLDCSF_NFO                             F280X_mNFO(  6,  2 )

#define F280X_AQSFRC_ACTSFA_DIS                             ( 0 )
#define F280X_AQSFRC_ACTSFA_CLR                             ( 1 )
#define F280X_AQSFRC_ACTSFA_SET                             ( 2 )
#define F280X_AQSFRC_OTSFA_TRIG                             ( 1 )
#define F280X_AQSFRC_ACTSFB_DIS                             ( 0 )
#define F280X_AQSFRC_ACTSFB_CLR                             ( 1 )
#define F280X_AQSFRC_ACTSFB_SET                             ( 2 )
#define F280X_AQSFRC_OTSFB_TRIG                             ( 1 )
#define F280X_AQSFRC_RLDCSF_LOAD_ON_ZRO                     ( 0 )
#define F280X_AQSFRC_RLDCSF_LOAD_ON_PRD                     ( 1 )
#define F280X_AQSFRC_RLDCSF_LOAD_ON_ZRO_OR_PRD              ( 2 )
#define F280X_AQSFRC_RLDCSF_IMMED                           ( 3 )


// Action-qualifier Continuous S/F Force Register (AQCSFRC)
#define F280X_AQCSFRC_CSFA_NFO                              F280X_mNFO(  0,  2 )
#define F280X_AQCSFRC_CSFB_NFO                              F280X_mNFO(  2,  2 )

#define F280X_AQCSFRC_CSFA_DIS                              ( 0 )
#define F280X_AQCSFRC_CSFA_LOW                              ( 1 )
#define F280X_AQCSFRC_CSFA_HIGH                             ( 2 )
#define F280X_AQCSFRC_CSFB_DIS                              ( 0 )
#define F280X_AQCSFRC_CSFB_LOW                              ( 1 )
#define F280X_AQCSFRC_CSFB_HIGH                             ( 2 )


// Compare Control (CMPCTL)
#define F280X_CMPCTL_LOADAMODE_NFO                          F280X_mNFO(  1,  2 )
#define F280X_CMPCTL_LOADBMODE_NFO                          F280X_mNFO(  2,  2 )
#define F280X_CMPCTL_SHDWAMODE_NFO                          F280X_mNFO(  4,  1 )
#define F280X_CMPCTL_SHDWBMODE_NFO                          F280X_mNFO(  6,  1 )
#define F280X_CMPCTL_SHDWAFULL_NFO                          F280X_mNFO(  8,  1 )
#define F280X_CMPCTL_SHDWBFULL_NFO                          F280X_mNFO(  9,  1 )

#define F280X_CMPCTL_LOADAMODE_ZRO                          ( 0 )
#define F280X_CMPCTL_LOADAMODE_PRD                          ( 1 )
#define F280X_CMPCTL_LOADAMODE_ZRO_OR_PRD                   ( 2 )
#define F280X_CMPCTL_LOADAMODE_FREEZE                       ( 3 )
#define F280X_CMPCTL_LOADBMODE_ZRO                          ( 0 )
#define F280X_CMPCTL_LOADBMODE_PRD                          ( 1 )
#define F280X_CMPCTL_LOADBMODE_ZRO_OR_PRD                   ( 2 )
#define F280X_CMPCTL_LOADBMODE_FREEZE                       ( 3 )
#define F280X_CMPCTL_SHDWAMODE_SHDW                         ( 0 )
#define F280X_CMPCTL_SHDWAMODE_IMMED                        ( 1 )
#define F280X_CMPCTL_SHDWBMODE_SHDW                         ( 0 )
#define F280X_CMPCTL_SHDWBMODE_IMMED                        ( 1 )
#define F280X_CMPCTL_SHDWAFULL_NOT_FULL                     ( 0 )
#define F280X_CMPCTL_SHDWAFULL_FULL                         ( 1 )
#define F280X_CMPCTL_SHDWBFULL_NOT_FULL                     ( 0 )
#define F280X_CMPCTL_SHDWBFULL_FULL                         ( 0 )


// High Resolution PWM Configuration (HRCNFG)
#define F280X_HRCNFG_EDGMODE_NFO                            F280X_mNFO(  0,  2 )
#define F280X_HRCNFG_CTLMODE_NFO                            F280X_mNFO(  2,  1 )
#define F280X_HRCNFG_HRLOAD_NFO                             F280X_mNFO(  3,  1 )

#define F280X_HRCNFG_EDGMODE_DIS                            ( 0 )
#define F280X_HRCNFG_EDGMODE_RISE                           ( 1 )
#define F280X_HRCNFG_EDGMODE_FALL                           ( 2 )
#define F280X_HRCNFG_EDGMODE_RISE_FALL                      ( 3 )
#define F280X_HRCNFG_CTLMODE_DUTY                           ( 0 )
#define F280X_HRCNFG_CTLMODE_PHASE                          ( 1 )

// TBD waiting for JTate's response on one bit or 2
//#define F280X_HRCNFG_HRLOAD_ZRO                             ( 0 )
//#define F280X_HRCNFG_HRLOAD_PRD                             ( 1 )
//#define F280X_HRCNFG_HRLOAD_ZRO_OR_PRD                      ( 2 )
//#define F280X_HRCNFG_HRLOAD_NO_LOAD                         ( 3 )


// Deadband control (DBCTL)
#define F280X_DBCTL_OUT_MODE_NFO                            F280X_mNFO(  0,  2 )
#define F280X_DBCTL_POLSEL_NFO                              F280X_mNFO(  2,  2 )

#define F280X_DBCTL_OUT_MODE_DIS                            ( 0 )
#define F280X_DBCTL_OUT_MODE_DELAYB                         ( 1 )
#define F280X_DBCTL_OUT_MODE_DELAYA                         ( 2 )
#define F280X_DBCTL_OUT_MODE_ENA                            ( 3 )
#define F280X_DBCTL_POLSEL_ACTIVE_HIGH                      ( 0 )
#define F280X_DBCTL_POLSEL_ACTIVE_LOW_COMPL                 ( 1 )
#define F280X_DBCTL_POLSEL_ACTIVE_HIGH_COMPL                ( 2 )
#define F280X_DBCTL_POLSEL_ACTIVE_LOW                       ( 3 )


// Deadband Rising Edge Delay (DBRED)
#define F280X_DBRED_DEL_NFO                                 F280X_mNFO(  0, 10 )

// Deadband Falling Edge Delay (DBFED)
#define F280X_DBFED_DEL_NFO                                 F280X_mNFO(  0, 10 )


// Trip-zone Select (TZSEL)
#define F280X_TZSEL_CBC1_NFO                                F280X_mNFO(  0,  1 )
#define F280X_TZSEL_CBC2_NFO                                F280X_mNFO(  1,  1 )
#define F280X_TZSEL_CBC3_NFO                                F280X_mNFO(  2,  1 )
#define F280X_TZSEL_CBC4_NFO                                F280X_mNFO(  3,  1 )
#define F280X_TZSEL_CBC5_NFO                                F280X_mNFO(  4,  1 )
#define F280X_TZSEL_CBC6_NFO                                F280X_mNFO(  5,  1 )
#define F280X_TZSEL_OSHT1_NFO                               F280X_mNFO(  8,  1 )
#define F280X_TZSEL_OSHT2_NFO                               F280X_mNFO(  9,  1 )
#define F280X_TZSEL_OSHT3_NFO                               F280X_mNFO( 10,  1 )
#define F280X_TZSEL_OSHT4_NFO                               F280X_mNFO( 11,  1 )
#define F280X_TZSEL_OSHT5_NFO                               F280X_mNFO( 12,  1 )
#define F280X_TZSEL_OSHT6_NFO                               F280X_mNFO( 13,  1 )

#define F280X_TZSEL_CBC1_DIS                                ( 0 )
#define F280X_TZSEL_CBC1_ENA                                ( 1 )
#define F280X_TZSEL_CBC2_DIS                                ( 0 )
#define F280X_TZSEL_CBC2_ENA                                ( 1 )
#define F280X_TZSEL_CBC3_DIS                                ( 0 )
#define F280X_TZSEL_CBC3_ENA                                ( 1 )
#define F280X_TZSEL_CBC4_DIS                                ( 0 )
#define F280X_TZSEL_CBC4_ENA                                ( 1 )
#define F280X_TZSEL_CBC5_DIS                                ( 0 )
#define F280X_TZSEL_CBC5_ENA                                ( 1 )
#define F280X_TZSEL_CBC6_DIS                                ( 0 )
#define F280X_TZSEL_CBC6_ENA                                ( 1 )
#define F280X_TZSEL_OSHT1_DIS                               ( 0 )
#define F280X_TZSEL_OSHT1_ENA                               ( 1 )
#define F280X_TZSEL_OSHT2_DIS                               ( 0 )
#define F280X_TZSEL_OSHT2_ENA                               ( 1 )
#define F280X_TZSEL_OSHT3_DIS                               ( 0 )
#define F280X_TZSEL_OSHT3_ENA                               ( 1 )
#define F280X_TZSEL_OSHT4_DIS                               ( 0 )
#define F280X_TZSEL_OSHT4_ENA                               ( 1 )
#define F280X_TZSEL_OSHT5_DIS                               ( 0 )
#define F280X_TZSEL_OSHT5_ENA                               ( 1 )
#define F280X_TZSEL_OSHT6_DIS                               ( 0 )
#define F280X_TZSEL_OSHT6_ENA                               ( 1 )

// Trip-zone Control (TZCTL)
#define F280X_TZCTL_TZA_NFO                                 F280X_mNFO(  0,  2 )
#define F280X_TZCTL_TZB_NFO                                 F280X_mNFO(  2,  2 )

#define F280X_TZCTL_TZA_HIGH_IMPEDANCE                      ( 0 )
#define F280X_TZCTL_TZA_FORCED_HIGH                         ( 1 )
#define F280X_TZCTL_TZA_FORCED_LOW                          ( 2 )
#define F280X_TZCTL_TZA_DIS                                 ( 3 )
#define F280X_TZCTL_TZB_HIGH_IMPEDANCE                      ( 0 )
#define F280X_TZCTL_TZB_FORCED_HIGH                         ( 1 )
#define F280X_TZCTL_TZB_FORCED_LOW                          ( 2 )
#define F280X_TZCTL_TZB_DIS                                 ( 3 )


// Trip-zone Enable Interrupt (TZEINT)
#define F280X_TZEINT_CBC_NFO                                F280X_mNFO(  1,  1 )
#define F280X_TZEINT_OST_NFO                                F280X_mNFO(  2,  1 )

#define F280X_TZEINT_CBC_DIS                                ( 0 )
#define F280X_TZEINT_CBC_ENA                                ( 1 )
#define F280X_TZEINT_OST_DIS                                ( 0 )
#define F280X_TZEINT_OST_ENA                                ( 1 )


// Trip-zone Force (TZFRC)
#define F280X_TZFRC_CBC_NFO                                 F280X_mNFO(  1,  1 )
#define F280X_TZFRC_OST_NFO                                 F280X_mNFO(  2,  1 )

#define F280X_TZFRC_CBC_FORCE_FAULT                         ( 1 )
#define F280X_TZFRC_OST_FORCE_FAULT                         ( 1 )


// Trip-zone Clear (TZCLR)
#define F280X_TZCLR_INT_NFO                                 F280X_mNFO(  0,  1 )
#define F280X_TZCLR_CBC_NFO                                 F280X_mNFO(  1,  1 )
#define F280X_TZCLR_OST_NFO                                 F280X_mNFO(  2,  1 )

#define F280X_TZCLR_INT_CLR                                 ( 1 )
#define F280X_TZCLR_CBC_CLR                                 ( 1 )
#define F280X_TZCLR_OST_CLR                                 ( 1 )


// Event Trigger Selection Register (ETSEL)
#define F280X_ETSEL_INTSEL_NFO                              F280X_mNFO(  0,  3 )
#define F280X_ETSEL_INTEN_NFO                               F280X_mNFO(  3,  1 )
#define F280X_ETSEL_SOCASEL_NFO                             F280X_mNFO(  8,  3 )
#define F280X_ETSEL_SOCAEN_NFO                              F280X_mNFO( 11,  1 )
#define F280X_ETSEL_SOCBSEL_NFO                             F280X_mNFO( 12,  3 )
#define F280X_ETSEL_SOCBEN_NFO                              F280X_mNFO( 15,  1 )

#define F280X_ETSEL_INTSEL_CTR_EQU_ZRO                      ( 0 )
#define F280X_ETSEL_INTSEL_CTR_PRD_ZRO                      ( 1 )
#define F280X_ETSEL_INTSEL_CTRU_EQU_CMPA                    ( 4 )
#define F280X_ETSEL_INTSEL_CTRD_EQU_CMPA                    ( 5 )
#define F280X_ETSEL_INTSEL_CTRU_EQU_CMPB                    ( 6 )
#define F280X_ETSEL_INTSEL_CTRD_EQU_CMPB                    ( 7 )
#define F280X_ETSEL_INTEN_DIS                               ( 0 )
#define F280X_ETSEL_INTEN_ENA                               ( 1 )
#define F280X_ETSEL_SOCASEL_CTR_EQU_ZRO                     ( 0 )
#define F280X_ETSEL_SOCASEL_CTR_PRD_ZRO                     ( 1 )
#define F280X_ETSEL_SOCASEL_CTRU_EQU_CMPA                   ( 4 )
#define F280X_ETSEL_SOCASEL_CTRD_EQU_CMPA                   ( 5 )
#define F280X_ETSEL_SOCASEL_CTRU_EQU_CMPB                   ( 6 )
#define F280X_ETSEL_SOCASEL_CTRD_EQU_CMPB                   ( 7 )
#define F280X_ETSEL_SOCAEN_DIS                              ( 0 )
#define F280X_ETSEL_SOCAEN_ENA                              ( 1 )
#define F280X_ETSEL_SOCBSEL_CTR_EQU_ZRO                     ( 0 )
#define F280X_ETSEL_SOCBSEL_CTR_PRD_ZRO                     ( 1 )
#define F280X_ETSEL_SOCBSEL_CTRU_EQU_CMPA                   ( 4 )
#define F280X_ETSEL_SOCBSEL_CTRD_EQU_CMPA                   ( 5 )
#define F280X_ETSEL_SOCBSEL_CTRU_EQU_CMPB                   ( 6 )
#define F280X_ETSEL_SOCBSEL_CTRD_EQU_CMPB                   ( 7 )
#define F280X_ETSEL_SOCBEN_DIS                              ( 0 )
#define F280X_ETSEL_SOCBEN_ENA                              ( 1 )


// Event Timer Prescale (ETPS)
#define F280X_ETPS_INTPRD_NFO                              F280X_mNFO(  0,  2 )
#define F280X_ETPS_INTCNT_NFO                              F280X_mNFO(  2,  2 )
#define F280X_ETPS_SOCAPRD_NFO                             F280X_mNFO(  8,  2 )
#define F280X_ETPS_SOCACNT_NFO                             F280X_mNFO( 10,  2 )
#define F280X_ETPS_SOCBPRD_NFO                             F280X_mNFO( 12,  2 )
#define F280X_ETPS_SOCBCNT_NFO                             F280X_mNFO( 14,  2 )

#define F280X_ETPS_INTPRD_DIS                              ( 0 )
#define F280X_ETPS_INTPRD_1                                ( 1 )
#define F280X_ETPS_INTPRD_2                                ( 2 )
#define F280X_ETPS_INTPRD_3                                ( 3 )
#define F280X_ETPS_SOCAPRD_DIS                             ( 0 )
#define F280X_ETPS_SOCAPRD_1                               ( 1 )
#define F280X_ETPS_SOCAPRD_2                               ( 2 )
#define F280X_ETPS_SOCAPRD_3                               ( 3 )
#define F280X_ETPS_SOCBPRD_DIS                             ( 0 )
#define F280X_ETPS_SOCBPRD_1                               ( 1 )
#define F280X_ETPS_SOCBPRD_2                               ( 2 )
#define F280X_ETPS_SOCBPRD_3                               ( 3 )


//
// Usage:
//    Call the macros with the the register name, function and 
//    configuration.  Do not use quotations.
//
//    ie.  EPwm1Regs.CMPCTL.all = F280X_mCFGMSK( CMPCTL, LOADBMODE, ZRO );

// !!! IMPORTANT !!!
// Defines must be in the following format:
// F280X_<RegName>_<Field>_NFO for field info.  ( use F280X_mNFO macro )
// F280X_<RegName>_<Field>_<Config> for function configurations
//  Note <Config> is anything after the <Field> ie it can use underscores.
//       F280X_mCFGMSK( TZFRC, CBC, FORCE_FAULT )
//
// Example:
//
// regname:         TZCTL
//         _________________________   
// fields: |  |  |  |  | TZB | TZA |
//         -------------------------
// bits:     7  6  5  4  3  2  1  0
//
// cfgs:
// TZA : 00=HIGH_IMPEDANCE, 01=FORCE_HIGH, 10=FORCED_LOW, 11=DIS
// TZB : 00=HIGH_IMPEDANCE, 01=FORCE_HIGH, 10=FORCED_LOW, 11=DIS

// Get NFO name
// F280X_mNFONAME( TZCTL, TZA ) returns F280X_TZCTL_TZA_NFO
// F280X_mNFONAME( TZCTL, TZB ) returns F280X_TZCTL_TZB_NFO
#define F280X_mNFONAME( RegName, Field ) \
            ( F280X_ ## RegName ## _ ## Field ## _NFO )
            
// Get left most position of field
// F280X_mPOS( TZCTL, TZA ) returns 0
// F280X_mPOS( TZCTL, TZB ) returns 2
#define F280X_mPOS( RegName, Field ) \
            ( ( F280X_mNFONAME( RegName, Field ) & F280X_NFO_POS_MSK ) \
            >> F280X_NFO_POS_SHFT )

// Get number of bits the field takes up
// F280X_mBITS( TZCTL, TZA ) returns 2
// F280X_mBITS( TZCTL, TZB ) returns 2
#define F280X_mBITS( RegName, Field ) \
            ( ( F280X_mNFONAME( RegName, Field ) & F280X_NFO_BITS_MSK ) \
            >> F280X_NFO_BITS_SHFT )

// Get the bit mask unshifted
// F280X_mMSK_UNSHFTD( TZCTL, TZA ) returns 0x0003
// F280X_mMSK_UNSHFTD( TZCTL, TZB ) returns 0x0003
#define F280X_mMSK_UNSHFTD( RegName, Field ) \
            ( F280X_mBITS_TO_MSK_UNSHFTD( F280X_mBITS( RegName, Field ) ) )

// Get the shifted field mask
// F280X_mFIELDMSK( TZCTL, TZA ) returns 0x0003
// F280X_mFIELDMSK( TZCTL, TZB ) returns 0x000C
#define F280X_mFIELDMSK( RegName, Field ) \
            ( F280X_mMSK_UNSHFTD( RegName, Field ) \
            << F280X_mPOS( RegName, Field ) )

// Get the configuration value unshifted
// F280X_mCFG( TZCTL, TZA, FORCED_HI ) returns 1
// F280X_mCFG( TZCTL, TZB, FORCED_HI ) returns 1
// F280X_mCFG( TZCTL, TZB, DIS )       returns 3
#define F280X_mCFG( RegName, Field, Config ) \
            ( F280X_ ## RegName ## _ ## Field ## _ ## Config )
            
// Get the configuration value shifted to the field position
// F280X_mCFGMSK( TZCTL, TZA, FORCED_HI ) returns 0x0001
// F280X_mCFGMSK( TZCTL, TZB, FORCED_HI ) returns 0x0004
// F280X_mCFGMSK( TZCTL, TZB, DIS )       returns 0x000C
#define F280X_mCFGMSK( RegName, Field, Config ) \
            ( F280X_mCFG( RegName, Field, Config ) \
            << F280X_mPOS( RegName, Field ) )



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


#endif  // F280X_H
