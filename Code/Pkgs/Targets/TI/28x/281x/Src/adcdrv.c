/*==============================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:
    Adcdrv.c

PURPOSE:
    ADC driver implementation for a TI 2812 device.  Setup, start conversion,
    and data retrevial are all provided here.

FUNCTION(S):
    ADCDRV_fnStartConv     - Start the conv process
    ADCDRV_fnReset         - Resets the adc module
    ADCDRV_fnCtor          - Setup the ADC for this implemenation
    ADCDRV_fnSetMaxConv    - Sets the number of conversions
    ADCDRV_fnGetResultReg  - Returns the desired result register
    ADCDRV_fnGetResultChnl - Returns the result of the desired channel
    ADCDRV_fnGetSeqCnt     - Returns which conversion is taking place

    local:
        None

NOTES:
    See adcdrv.h for other details.

CHANGE HISTORY:
$Log: adcdrv.c $

    ***********************************************
    Revision: NovaPfrmB_Karli/2
    User: Karli     Date: 03/09/05  Time: 01:21AM
    Depricated macros to maintain abstraction.
    

    ***********************************************
    Revision: NovaPfrmB_HorstM/1
    User: HorstM     Date: 01/13/05  Time: 05:09PM
    changed calls to deprecated delay macros to locally
    defined macros
    
    
    *****************  Version 3  *****************
    User: Yingrand     Date: 10/21/04   Time: 5:01p
    Updated in $/PD/Common/Targets/TI/c28x/HAL/Test/adcdrv
    correct a compile warning of missing return statement at end of
    non-void function "ADCDRV_fnGetResultChnl".
    
    *****************  Version 2  *****************
    User: Johnb        Date: 6/23/04    Time: 11:46a
    Updated in $/PD/Common/Targets/TI/c28x/HAL/Test/adcdrv
    Updated copyright notice.
    
    *****************  Version 1  *****************
    User: Brentt       Date: 6/22/04    Time: 2:45p
    Created in $/PD/Common/Targets/TI/c28x/HAL/Test/adcdrv


==============================================================================*/


/*==============================================================================
                              Includes
==============================================================================*/
#include "adcdrv.h"     //Device interface
#include "xassert.h"    //Assert for debugging
#include "config.h"     //for CTRL_CPU_MAXSPEED
#include "Device.h"     //Device header info
#include "devicemacro.h" 

/*==============================================================================
                              external functions
==============================================================================*/
extern void DSP28x_usDelay( uint32 Count );

/*==============================================================================
                              Defines
==============================================================================*/
/* ^^^^^^^^^^^^^^^^^^^^^^^ Function Enables ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
                                    //---------------- Public -----------------
#define ADCDRV_FN_SOC_EN            ( TRUE  )//Include code for start of conv
#define ADCDRV_FN_RESET_EN          ( FALSE )//Exclude code for reseting ADC
#define ADCDRV_FN_CTOR_EN           ( TRUE  )//Include code for setup
#define ADCDRV_FN_SET_MAXCONV_EN    ( FALSE )//Exclude code for writing max conv
#define ADCDRV_FN_GET_RESULT_REG    ( TRUE  )//Include code for getting result reg
#define ADCDRV_FN_GET_CHNL_RESULT   ( TRUE  )//Include code for getting chnl result
#define ADCDRV_FN_GET_SEQ_CNT       ( TRUE )//Exclude code for getting seq cnt

                                    //--------------- Private -----------------
                                    //None


/*^^^^^^^^^^^^^^^^^^^^^^^^^^^ Feature Enable ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
//NOTE: ONLY ENABLE ONE OF THE FOLLOWING 2 DEFINES
//Since the results are default left justified in the result reg, these defines
//can be used for compile time enabling of which way the user would the result.
#define ADCDRV_RESULT_LEFT_JUST_EN  ( FALSE )
#define ADCDRV_RESULT_RIGHT_JUST_EN ( !ADCDRV_RESULT_LEFT_JUST_EN )

#if( ADCDRV_RESULT_LEFT_JUST_EN == FALSE && ADCDRV_RESULT_RIGHT_JUST_EN == FALSE )
#error Either ADCDRV_RESULT_LEFT_JUST_EN or ADCDRV_RESULT_RIGHT_JUST_EN must TRUE
#endif

#if( ADCDRV_RESULT_LEFT_JUST_EN == TRUE && ADCDRV_RESULT_RIGHT_JUST_EN == TRUE )
#error Only one of ADCDRV_RESULT_LEFT_JUST_EN or ADCDRV_RESULT_RIGHT_JUST_EN must be TRUE
#endif

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^ MACRO'S ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//To reset the ADC module
#define ADCDRV_mRESET()     ( AdcRegs.ADCTRL1.bit.RESET = F28X_ADCTRL1_RESET_ALL )

//To set the maximum number of conversion to do.
#define ADCDRV_mSET_MAX_CONV( Val ) ( AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = Val )

// ---------------------------------------------------------------------------
// Delay microsecond macros
// ---------------------------------------------------------------------------
//
// TEST_mCPU_RATE is the # of nanoseconds per clock cycle, so for example:
//
// System when MIPS = 150MHz
//
// TEST_mCPU_RATE = 1 / 150MHz = 6.6667nS/clock cycle
//
// This value is then passed into the DSP28x_usDelay function for processing
#define ADCDRV_mCPU_RATE   ( (long double)( ( 1 / (float) CTRL_CPU_MAXSPEED ) \
                              * (float)1.0E9 ) )
// This is a formula derived by TI, They convert the input value into
// nano-seconds using:
//
// ( long double ) A * 1000L
//
// Then they find the number of clock cycles required:
//
// ( long double ) A * 1000L / ( long double ) TEST_mCPU_RATE
//
// According the ASM function created by TI, there is 9/10 of a cycle of
// overhead for each loop and each run through the loop takes 5 cycles
//
// ( ( (long double)A * 1000L / (long double)TEST_mCPU_RATE ) - 9.0L ) / 5.0L
//
#define ADCDRV_mDELAY_US(A)  DSP28x_usDelay( ( ( ( (long double) A * 1000.0L ) \
                                          / (long double) ADCDRV_mCPU_RATE )   \
                                          - 9.0L ) / 5.0L )

//Delay using ns unit
#define ADCDRV_mDELAY_NS(A)  DSP28x_usDelay( ( ( (long double) A  \
                                          / (long double) ADCDRV_mCPU_RATE )   \
                                          - 9.0L ) / 5.0L )

/*^^^^^^^^^^^^^^^^^^^^^ Channel order selection ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
//This maps the channel to when in the sequence it will be read.
//SEQ_0_CHNL will be read first followed by SEQ_1_CHNL and so on.
//Set each SEQ_n_CHNL to a value in teADC_CHNLS.
#define SEQ_0_CHNL      ADCDRV_eCHNL_9     //This channel is read first
#define SEQ_1_CHNL      ADCDRV_eCHNL_5     //..second
#define SEQ_2_CHNL      ADCDRV_eCHNL_8     //..third
#define SEQ_3_CHNL      ADCDRV_eCHNL_1
#define SEQ_4_CHNL      ADCDRV_eCHNL_4
#define SEQ_5_CHNL      ADCDRV_eCHNL_10
#define SEQ_6_CHNL      ADCDRV_eCHNL_6
#define SEQ_7_CHNL      ADCDRV_eCHNL_11
#define SEQ_8_CHNL      ADCDRV_eCHNL_0
#define SEQ_9_CHNL      ADCDRV_eCHNL_7
#define SEQ_10_CHNL     ADCDRV_eCHNL_2
#define SEQ_11_CHNL     ADCDRV_eCHNL_3
#define SEQ_12_CHNL     ADCDRV_eCHNL_14
#define SEQ_13_CHNL     ADCDRV_eCHNL_12
#define SEQ_14_CHNL     ADCDRV_eCHNL_13
#define SEQ_15_CHNL     ADCDRV_eCHNL_15     //..last

//The maximum possible conversions
#define ADCDRV_MAX_CONVERSION               ( 16 )

//Conversion types
#define MAX_CONV_CASC_16                    ( 0x0F )  // Cascaded seq for 16 convs

// ADC Delay defines
//
// time to wait after powering bandgap and ref
#define ADC_US_DELAY_1                      ( 10000L )
// time to wait after power rest of analog core
#define ADC_US_DELAY_2                      ( 20L   )

//The number of shifts needed to to make the result right justified
//
#define ADCDRV_RIGHT_JUSTIFY_SHIFT_VAL  (    4   )
/*==============================================================================
                            Type Definitions
==============================================================================*/
//None

/*==============================================================================
                                Enums
==============================================================================*/
//None

/*=============================================================================
                              Structures
==============================================================================*/
//None

/*==============================================================================
                           Local/Private Constants
==============================================================================*/
//A mapped channel number to result register is needed since the result reg
//holds acquired data based on the order it is read.  This means that SEQ_0
//may read Channel 9 first which is stored in result reg 0.  This array is used
//to find the channel numbers according to the sequence.  So when you search
//through this array, the index into the array will be the offset for the
//appropriate result register.
//NOTE:  THIS MUST ALWAYS BE IN THE ORDER IN WHICH THE CHANNELS ARE READ.  HENCE
//       WHY THIS IS ASSIGNED AS THE SEQ ORDER.
const uint16 AdChnlMapping[] =
{
    SEQ_0_CHNL,
    SEQ_1_CHNL,
    SEQ_2_CHNL,
    SEQ_3_CHNL,
    SEQ_4_CHNL,
    SEQ_5_CHNL,
    SEQ_6_CHNL,
    SEQ_7_CHNL,
    SEQ_8_CHNL,
    SEQ_9_CHNL,
    SEQ_10_CHNL,
    SEQ_11_CHNL,
    SEQ_12_CHNL,
    SEQ_13_CHNL,
    SEQ_14_CHNL,
    SEQ_15_CHNL
};

#define AD_CHNL_ARRAY_SIZE ( sizeof( AdChnlMapping    ) / \
                             sizeof( AdChnlMapping[0] ) )

/*==============================================================================
                           Local/Private Variables
==============================================================================*/
//None

/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/
//None

/*==============================================================================
                           Function Definitions
==============================================================================*/
//None


/*******************************************************************************

FUNCTION NAME:
    ADCDRV_fnStartConv

PURPOSE:
    Starts the conversion process for the ADC.

INPUTS:
    none

OUTPUTS:
    none

NOTES:
    This SOC is setup to start a conversion process for a cascaded ADC.

    Most of the code in this fn used to be in ADCDRV_fnSetup().  In order to
    save some speed, this function doesn't make a call to ADCDRV_fnSetup(), but
    instead executes the code directly.  This is b/c this start of conv can be
    called at interrupt level.

VERSION HISTORY:

Version: 1.00  Date: Jan 16, 2003  By: John Bellini
    - Created.

Version: 1.10  Date: April 8, 2003 By: Johon Bellini
    - Renamed function

Version: 2.00  Date: July 28, 2003 By: John Bellini
    - Removed call to ADCDRV_fnSetup() to save execution time of this fn.
      Code from ADCDRV_fnSetup() is cut and pasted to this fn and the start
      of conversion integrated to the final assignment to ADCTRL2.

Version: 2.10  Date: Sept 5, 2003  By: John Bellini
    - Removed setup of module initialization registers.  These are now in the
      constructor.  This is possible because the clearing of these registers
      upon reset of the entire ADC module is NO longer done here.  To do a start
      of conversion, only the sequencer needs to be reset to zero, then the
      start of conversion initiated.

Version: 2.20  Date: June 15, 2004 By: Brent Tokarchuk
    - Updated to reflect c28x register definition

*******************************************************************************/
#if( ADCDRV_FN_SOC_EN == TRUE )        //Function compile option
void ADCDRV_fnStartConv( void )
{
    //Put the sequencer back to zero.
    AdcRegs.ADCTRL2.bit.RST_SEQ1 = F28X_ADCTRL2_RST_SEQX_IMMED;

    //Start the conversion
    AdcRegs.ADCTRL2.bit.SOC_SEQ1 = F28X_ADCTRL2_SEQX_SOC_START_TRIG;
} // ADCDRV_fnStartConv
#endif // #if( ADCDRV_FN_SOC_EN == TRUE )


/*******************************************************************************

FUNCTION NAME:
    ADCDRV_fnCtor

PURPOSE:
    To setup the ADC module for how it is needed for the intended application.

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    Notes:
    This code puts the AD into a cascaded mode (all 16 channels as one unit).
    The AD read 0 then 1 then 2 then 3...et to 15.  Once it gets to the end
    it stops.  To start the sequencing again, call this function.

    The reset at the beginning of this function needed as per the TI
    documentation.

    If AD hardware pins change, no sweat, just change the order of what is read
    (which is important since interrupts need these in a certain order) in the
    CHSELSEQn regs.

    There is an errata documented that a prescaler of 0 with CPS = 1 causes the
    ADC to no sequence and hence, only channel 0 of the ADC ever gets read b/c
    the ADC never sequences to the next channel.

    There is code available to have the sequence order user adjustabled.
    This is code must be enabled to be used by setting the option
    ADCDRV_VARIABLE_CHNL_SEL to TRUE.

    WARNING!
    There is errata noted in the silicon where if CPS = 1 and the clock
    scale is set to 0, the AD may only do one conversion.

VERSION HISTORY:
Version: 1.00  Date: Jan 16, 2003  By: John Bellini
    - Created

Version: 1.10  Date: April 2, 2003  By: John Bellini
    - Updated clock speed for conversions to be the fastest possible.

Version: 1.20  Date: June 6, 2003  By: John Bellini
    - Made the routine faster by replacing fn calls with macro's.

Version: 1.30  Date: Sept 5, 2003  By: John Bellini
    - Changed fn name from fnSetup to fnCtor

Version: 2.00  Date: June 15, 2004 By: Brent Tokarchuk
    - Totally overhaulled function to reflect new register structure on c28x
    - followed original algorithm as closely as possible
    - added legitimate 3 NOP delay to account for reset latency (see bit definition)

Version: 2.10  Date: JAN-12-2005 By: Horst Maurer
    - changed calls to deprecated delay macros to locally defined macros

*******************************************************************************/
#if( ADCDRV_FN_CTOR_EN == TRUE )  //Function compile enable
void ADCDRV_fnCtor( void )
{
	// Enable access to protected System Register
	EALLOW;
    // Enable the peripheral clock to the ADC module
    SysCtrlRegs.PCLKCR.bit.ADCENCLK   = F28X_PCLKCR_CLK_ON;
	// Disable access to protected register
	EDIS;
	
    // Reset the ADC to get to a known state.   This is also required by the
    // devices hardware as documented in the ADC data sheet.
    // Set the "RESET" bit to cause a reset.  Cleared by device hardware.
    // Because this is called from an interrupt, the following line is used
    // instead of a call to ADCDRV_fnReset().
    ADCDRV_mRESET();

    // The reset of the ADC has a latency of 3 clock cycles, and therefore
    // the other ADC control register bits should not be modified until
    // 3 cycles after the reset.
    asm(" NOP");
    asm(" NOP");
    asm(" NOP");

    // Set the number of channels to convert.
    // Because this is called from an interrupt, the following line is used
    // instead of a call to ADCDRV_fnSetMaxConv( MAX_CONV_CASC_16 );
    ADCDRV_mSET_MAX_CONV( MAX_CONV_CASC_16 );

    // Setup the conversion sequences
    // Conversion sequence register 1 (ADCCHSELSEQ1)
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = SEQ_0_CHNL;
    AdcRegs.ADCCHSELSEQ1.bit.CONV01 = SEQ_1_CHNL;
    AdcRegs.ADCCHSELSEQ1.bit.CONV02 = SEQ_2_CHNL;
    AdcRegs.ADCCHSELSEQ1.bit.CONV03 = SEQ_3_CHNL;

    // Conversion sequence register 2 (ADCCHSELSEQ2)
    AdcRegs.ADCCHSELSEQ2.bit.CONV04 = SEQ_4_CHNL;
    AdcRegs.ADCCHSELSEQ2.bit.CONV05 = SEQ_5_CHNL;
    AdcRegs.ADCCHSELSEQ2.bit.CONV06 = SEQ_6_CHNL;
    AdcRegs.ADCCHSELSEQ2.bit.CONV07 = SEQ_7_CHNL;

    // Conversion sequence register 3 (ADCCHSELSEQ3)
    AdcRegs.ADCCHSELSEQ3.bit.CONV08 = SEQ_8_CHNL;
    AdcRegs.ADCCHSELSEQ3.bit.CONV09 = SEQ_9_CHNL;
    AdcRegs.ADCCHSELSEQ3.bit.CONV10 = SEQ_10_CHNL;
    AdcRegs.ADCCHSELSEQ3.bit.CONV11 = SEQ_11_CHNL;

    // Conversion sequence register 4 (ADCCHSELSEQ4)
    AdcRegs.ADCCHSELSEQ4.bit.CONV12 = SEQ_12_CHNL;
    AdcRegs.ADCCHSELSEQ4.bit.CONV13 = SEQ_13_CHNL;
    AdcRegs.ADCCHSELSEQ4.bit.CONV14 = SEQ_14_CHNL;
    AdcRegs.ADCCHSELSEQ4.bit.CONV15 = SEQ_15_CHNL;

    // ADC Control register 1 setup (ADCTRL1)
    AdcRegs.ADCTRL1.bit.SUSMOD   = F28X_ADCTRL1_SUSP_STOP;
    AdcRegs.ADCTRL1.bit.ACQ_PS   = F28X_ADCTRL1_ACQPS_1;
    AdcRegs.ADCTRL1.bit.CPS      = F28X_ADCTRL1_CPS_DIV2;
    AdcRegs.ADCTRL1.bit.CONT_RUN = F28X_ADCTRL1_RUN_STARTSTOP;
    AdcRegs.ADCTRL1.bit.SEQ_OVRD = F28X_ADCTRL1_SEQ_WRAP;
    AdcRegs.ADCTRL1.bit.SEQ_CASC = F28X_ADCTRL1_SEQ_CASC;

    // ADC control register 2 setup (ADCTRL2)
    AdcRegs.ADCTRL2.bit.EVB_SOC_SEQ  = F28X_ADCTRL2_SEQ_EVXSOC_NO_ACT;
    AdcRegs.ADCTRL2.bit.RST_SEQ1     = F28X_ADCTRL2_RST_SEQX_IMMED;
    AdcRegs.ADCTRL2.bit.SOC_SEQ1     = F28X_ADCTRL2_SEQX_SOC_CLR_TRIG;
    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = F28X_ADCTRL2_SEQX_INT_DIS;
    AdcRegs.ADCTRL2.bit.INT_MOD_SEQ1 = F28X_ADCTRL2_SEQX_INTMODE_SET_EVERY;
    AdcRegs.ADCTRL2.bit.EVA_SOC_SEQ1 = F28X_ADCTRL2_SEQ_EVXSOC_NO_ACT;
    AdcRegs.ADCTRL2.bit.EXT_SOC_SEQ1 = F28X_ADCTRL2_SEQX_EXTSOC_NO_ACT;
    AdcRegs.ADCTRL2.bit.RST_SEQ2     = F28X_ADCTRL2_RST_SEQX_NO_ACT;
    AdcRegs.ADCTRL2.bit.SOC_SEQ2     = F28X_ADCTRL2_SEQX_SOC_CLR_TRIG;
    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ2 = F28X_ADCTRL2_SEQX_INT_DIS;
    AdcRegs.ADCTRL2.bit.EVB_SOC_SEQ2 = F28X_ADCTRL2_SEQ_EVXSOC_NO_ACT;

    // ADC control register 3 setup (ADCTRL3)
    //
    // Power up reference and bandgap circuits
    AdcRegs.ADCTRL3.bit.ADCBGRFDN = F28X_ADCTRL3_ADCBGRFDN_BG_RF_PWR_ON;

    // bandgap and reference must be on for at least 7ms
    ADCDRV_mDELAY_US( ADC_US_DELAY_1 );

    // power up the rest of the analog core
    AdcRegs.ADCTRL3.bit.ADCPWDN = F28X_ADCTRL3_ADCPWDN_PWR_ON;

    // need to wait for more than 20us just to be sure everything is peachy-keen
    ADCDRV_mDELAY_US( ADC_US_DELAY_2 );

    AdcRegs.ADCTRL3.bit.ADCCLKPS     = F28X_ADCTRL3_ADCLKPS_DIV_0;
    AdcRegs.ADCTRL3.bit.SMODE_SEL    = F28X_ADCTRL3_SMODE_SEQ_SAMPLE;
} // ADCDRV_fnCtor
#endif  //#if( ADCDRV_FN_CTOR_EN == TRUE )


/*******************************************************************************

FUNCTION NAME:
    ADCDRV_fnReset

PURPOSE:
    Reset the ADC module by setting the RESET bit.  ADC hardware clears the
    bit by itself.

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    This bit causes a master reset on the entire ADC mdoule.  All register bits
    and sequencer state machines are reset to the initial state as occurs when
    the device reset pin is pulled low (or after a power-pn reset).
    The ADC module is reset during a system reset.  If an ADC module reset is
    desired at any other time, you can do by writing a 1 to this bit.  After a
    NOP, you can then write the appropriate values to the ADCTRL1 reg bits.

    This bit is automatically cleared by the hardware.

VERSION HISTORY:

Version: 1.00  Date: Jan 16, 2003  By: John Bellini
    - Created

Version: 1.01  Date: June 6, 2003  By: John Bellini
    - Replaced writing to register with a macro.

*******************************************************************************/
#if( ADCDRV_FN_RESET_EN == TRUE )  //Function compile enable
void ADCDRV_fnReset( void )
{
    //Set the "RESET" bit to cause a reset.  Cleared by device hardware.
    ADCDRV_mRESET();
} // ADCDRV_fnReset
#endif  //#if( ADCDRV_FN_RESET_EN == TRUE )


/*******************************************************************************

FUNCTION NAME:
    ADCDRV_fnSetMaxConv

PURPOSE:
    Sets the MaxConv Register for how many conversion need to take place.

INPUTS:
    The number of desired conversions.

OUTPUTS:
    Status if setting the value passed or failed.

NOTES:
    Instead of using run time error checking, the assert is used during
    development.

VERSION HISTORY:

Version: 1.00  Date: Jan 16, 2003  By: John Bellini
    - Created.

Version: 1.10  Date: June 15, 2004 By: Brent Tokarchuk
    - Updated to reflect c28x register definition
    - Changed *MAX_CONV to AdcRegs.ADCMAXCONV.all

*******************************************************************************/
#if( ADCDRV_FN_SET_MAXCONV_EN == TRUE )        //Function compile option
void ADCDRV_fnSetMaxConv( uint16 uiNumConv )
{
    assert( uiNumConv <= ADCDRV_MAX_CONVERSION );

    //Setup the number of conversions to take place
    AdcRegs.ADCMAXCONV.all = uiNumConv;
} // ADCDRV_fnSetMaxConv
#endif //#if( ADCDRV_FN_SET_MAXCONV_EN == TRUE )

/*******************************************************************************

FUNCTION NAME:
    ADCDRV_fnGetResultReg

PURPOSE:
    To retrieve a raw un modified adc result.

INPUTS:
    teADCDRV_RESULT_REG which:  This is the desired result register which is linked
    to the specific channel being read.

OUTPUTS:
    The raw unmodified adc result.

NOTES:
    The results for this ADC are left justified.

VERSION HISTORY:

Version: 1.00  Date: Jan 16, 2003  By: John Bellini
    - Created.

Version: 1.10  Date: April 8, 2003 By: John Bellini
    - Added compile time enable of right or left justification of the result.

Version: 1.11  Date: Sept 5, 2003   By: John Bellini
    - Removed prefixing on local variables.

Version: 1.12  Date: Sept 10, 2003  By: John Bellini
    - Added an assert for the input parameter to be in range.

Version: 1.20  Date: June 15, 2004 By: Brent Tokarchuk
    - Updated to reflect c28x register definition
    - Changed *RESULT0 to &(AdcRegs.ADCRESULT0)

*******************************************************************************/
#if( ADCDRV_FN_GET_RESULT_REG == TRUE ) //Function compile option
uint16 ADCDRV_fnGetResultReg( ADCDRV_teRESULT_REG AdcReg )
{
    //Make sure the input parameter is good to go!
    assert( AdcReg < ADCDRV_eRESULT_REG_MAX );

    //Using Result0 as the based address, add the value of the channel number
    //to the address of the result reg to get the desired result register.
    //So if Result0 is at address 0x70A8, this would store the result of
    //channel 0.  0x70A8 + 0 = 0x70A8 and so on.
#if( ADCDRV_RESULT_LEFT_JUST_EN == TRUE )

    return( *( &(AdcRegs.ADCRESULT0) + AdcReg ) );

#elif( ADCDRV_RESULT_RIGHT_JUST_EN == TRUE )

    return( (*( &(AdcRegs.ADCRESULT0) + AdcReg )) >> ADCDRV_RIGHT_JUSTIFY_SHIFT_VAL );

#endif // ( ADCDRV_RESULT_LEFT_JUST_EN == TRUE )

} // ADCDRV_fnGetResultReg
#endif // #if( ADCDRV_FN_GET_RESULT_REG == TRUE )


/*******************************************************************************

FUNCTION NAME:
    ADCDRV_fnGetResultChnl

PURPOSE:
    To retrieve a raw un modified adc result by using the channel number.

INPUTS:
    teADCDRV_CHNLS which is an available adc channel.

OUTPUTS:
    The raw unmodified adc result.

NOTES:
    The results for this ADC are left justified.

VERSION HISTORY:
Version: 1.00  Date: Jan 28, 2003  By: John Bellini
    - Created.

Version: 1.10  Date: April 2, 2003  By: John Bellini
    - Removed an assert that was at the end of the function to catch an improper
      value being passed in.  Removed because this function may be called from
      an interrupt and hence the assert may cause unpredictable results from an
      interrupt.
    - Change the name of iCnt to Cnt.

Version: 1.20  Date: April 8, 2003 By: John Bellini
    - Added compile time enable of right or left justification of the result.

Version: 1.21  Date: Sept 5, 2003   By: John Bellini
    - Removed prefixing on local variables.

Version: 1.22  Date: Sept 10, 2003  By: John Bellini
    - Added an assert for the input parameter to be in range.

Version: 1.30  Date: June 15, 2004 By: Brent Tokarchuk
    - Updated to reflect c28x register definition
    - Changed *RESULT0 to &(AdcRegs.ADCRESULT0)

Version: 1.40  Date: Oct. 21, 2004 By: Yingran Duan
    - add return after "for" loop to avoid warning of missing return on non-void
      function

*******************************************************************************/
#if( ADCDRV_FN_GET_CHNL_RESULT == TRUE ) //Function compile option
uint16 ADCDRV_fnGetResultChnl( ADCDRV_teCHNLS Chnl )
{
    uint16 Cnt;

    //Make sure the input parameter is good to go!
    assert( Chnl < ADCDRV_eCHNL_MAX );

    //Since the channel order isn't chronological, we need to search
    //the array of channels in order to find the proper result reg.  The
    //result reg is mapped so the index of the AdChnlMapping array.
    for( Cnt = 0; Cnt < AD_CHNL_ARRAY_SIZE; Cnt++ )
    {
        if( AdChnlMapping[ Cnt ] == Chnl )
        {
            //Build the address which is the base (&AdcRegs.ADCRESULT0) + offset (Cnt)
#if( ADCDRV_RESULT_LEFT_JUST_EN == TRUE )

            return( *( &(AdcRegs.ADCRESULT0) + Cnt ) );

#elif( ADCDRV_RESULT_RIGHT_JUST_EN == TRUE )

            return( (*( &(AdcRegs.ADCRESULT0) + Cnt )) >> ADCDRV_RIGHT_JUSTIFY_SHIFT_VAL );
#endif // #if( ADCDRV_RESULT_LEFT_JUST_EN == TRUE )
        } // if
    } // for

    //should not get here in normal condition
    assert( 0 );
    return( 0 );

} // ADCDRV_fnGetResultChnl
#endif // #if( ADCDRV_FN_GET_CHNL_RESULT == TRUE )


/*******************************************************************************

FUNCTION NAME:
    ADCDRV_fnGetSeqCnt

PURPOSE:
    To retrieve the value of the sequence counter.  The sequence counter says
    which conversion (0 to MAX_CONV ) is currently being processed.  This
    counter counts down to zero, signifying that all conversions are completed.

INPUTS:
    None

OUTPUTS:
    The value of the sequence register.

NOTES:
    The returned result is the value of how many conversions are left.
    This is setup to get the number based on the two sequencers being
    cascaded.

VERSION HISTORY:

Version: 1.00  Date: Jan 16, 2003  By: John Bellini
    - Created.

Version: 1.10  Date: April 2, 2003  By: John Bellini
    - Updated to return the actual count left based on the way the ADC has been
      setup.  This is changed since there wasn't much point in just returning
      the value of the register and then letting the user do the math to get it.

Version: 1.11  Date: Sept 5, 2003   By: John Bellini
    - Replaced the code that was here with the already existing macro for
      getting the sequence couunt

Version: 1.12  Date: June 15, 2004 By: Brent Tokarchuk
    - Deleted comment that doesn't make sense since the code was replaced by the
      macro in the last version update.

Version: 1.20  Date: Mar 8, 2005 By: Karl Isham
    - Replace return of macro with actual register value (Macro was depricated
      for public use).
*******************************************************************************/
#if( ADCDRV_FN_GET_SEQ_CNT == TRUE )   //Function compile option
uchar8 ADCDRV_fnGetSeqCnt( void )
{
    return( AdcRegs.ADCASEQSR.bit.SEQ_CNTR );
} // ADCDRV_fnGetSeqCnt
#endif // #if( ADCDRV_FN_GET_SEQ_CNT == TRUE )
