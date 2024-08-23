/*==============================================================================
Copyright © 2005 Xantrex International

This file is the property of Xantrex International and shall not be reproduced,
copied, or used as the basis for the manufacture or sale of equipment without
the express written permission of Xantrex International.

FILE NAME:
    Adcdrv.c

PURPOSE:
    ADC driver implementation for a TI 280x device.  Setup, start conversion,
    and data retrevial are all provided here.

FUNCTION(S):
    ADCDRV_fnStartConv        - Start the conv process
    ADCDRV_fnReset            - Resets the adc module
    ADCDRV_fnCtor             - Setup the ADC for this implemenation
    ADCDRV_fnDtor             - Shuts down the ADC module
    ADCDRV_fnSetMaxConv       - Sets the number of conversions
    ADCDRV_fnGetResultReg     - Returns the desired result register
    ADCDRV_fnGetResultChnl    - Returns the result of the desired channel
    ADCDRV_fnGetBoardRevision - Returns the version number for the ctrl board    
    ADCDRV_fnGetSeqCnt        - Returns which conversion is taking place
    ADCDRV_fnGetSeqBusyStatus - Checks if sequencer is busy
    ADCDRV_fnSeq1Isr          - Interrupt service routine for Sequence 1
    ADCDRV_fnSeq2Isr          - Interrupt service routine for Sequence 2

    local:
        adcdrv_fnPerformAdcOffsetCorr - Perform offset and gain correction

NOTES:
    GENERAL NOTES
    -------------
    The 280x ADC has two options for reading results. One gets the results from
    registers mapped to a periferal frame that has 2 wait states for read, and
    the results are left justified. The other option is to obtain the results 
    from registers that are mapped to a different peripheral frame, that has
    0 wait states, and the results are already right justified. This is obviously
    the more desireable option, but the choice is yours to make as the driver has 
    a compile time switch that allows you to choose between the two options.

    This driver sets the ADC into cascade mode where the two available
    sequencers are put together to make one big one.

    In order to save some code space, this driver has few options available to
    the user.  However, changing the way the AD operates is fairly isolated to
    the function ADCDRV_fnCtor().

    USING THIS DRIVER
    -----------------
    To use this driver, call ADCDRV_fnCtor() to setup the module.
    After than, ADCDRV_fnStartConv() is used to start the conversion process as
    needed.

    To read a result, call ADCDRV_fnGetResult( ResultReg ), with the
    desired result register as specified by the enum ADCDRV_teRESULT_REG. You
    can also use ADCDRV_fnGetResultChnl() with the desired channel number as
    specified by ADCDRV_teCHNLS.  ADCDRV_fnGetResultChnl() takes a bit longer
    but is better for abstraction and limits changes to application code if
    AD signals get moved to different channels.   

CHANGE HISTORY:
$Log: adcdrv.c $

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/14
    User: JeffF     Date: 02/13/07  Time: 01:05AM
    Added in a digital filter on the hardware revision reading. It now samples
    the input 8 times and takes the average reading.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/13
    User: JeffF     Date: 05/10/06  Time: 12:27AM
    Added code to read the voltage that corresponds to a hardware version number
    for the control board. Added a new function to convert the digital readout
    into a board version number for use by other files.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/12
    User: JeffF     Date: 05/08/06  Time: 08:40PM
    Changed the compile time switch to allow for future models to be taken
    care of.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/11
    User: JeffF     Date: 04/12/06  Time: 09:32PM
    Removed the gain calibration code. True calibration can be done through
    the interface now.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/10
    User: JeffF     Date: 02/13/06  Time: 06:59PM
    Ran the copyright updater script to bring the copyright notice on all project
    files up to date.

    ***********************************************
    Revision: SurveyorPfrmA2_YingranD/1
    User: YingranD     Date: 01/18/06  Time: 07:39PM
    new digctrl package design and related file update

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/9
    User: JeffF     Date: 11/24/05  Time: 06:13PM
    Added compile switch to prepare for analog signal changes on B-model boards.
    Changed the name of the offset and gain correction function. Changed the
    number of samples taken for offset and gain calibration to 16, which is
    more binary friendly than 20.

    ***********************************************
    Revision: SurveyorPfrmA_HenryC/2
    User: HenryC     Date: 11/01/05  Time: 05:14PM
    Added 48khz support (interrupt every other on SOC)

    Tweaked Sample time to 250ns , was 1us

    Changed offset calabration to run at same sampling as core code runs

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/8
    User: JeffF     Date: 08/05/05  Time: 10:07PM
    We are now using the revA silicon that has corrected the ADC issue that
    required 2 dummy reads at the beginning of an auto sequence, so i removed
    that code. I also made a few changes related to the auto gain calibration
    routine.

    ***********************************************
    Revision: SurveyorPfrmA_HenryC/1
    User: HenryC     Date: 08/03/05  Time: 02:08PM
    adcdrv_fnPerformAdcOffsetAndGainCorr modifed to also perform gain error calabration

    
    ***********************************************
    Revision: SurveyorPfrmA_JeffF/7
    User: JeffF     Date: 06/14/05  Time: 09:42PM
    Extended the delay after reset of the ADC. Found that it was causing problems
    with the initialization of ADCTRL1 when the FLASH wait states timing was
    reduced to 3 from 5.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/6
    User: JeffF     Date: 06/13/05  Time: 10:56PM
    Changed offset calibration routine to use the slowest ADC clock possible,
    and the longest sample hold time. Also changed the define names for some
    of the function compile options.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/5
    User: JeffF     Date: 06/05/05  Time: 07:15PM
    Made change in ADC sequence order to temporarily fix ADC settling problem
    documented in the DSP's errata sheet for rev 0 of the silicon.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/4
    User: JeffF     Date: 05/31/05  Time: 10:23PM
    Changed the way the registers are setup. They now use the *.all ability
    of the register structure to be more code efficient.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/3
    User: JeffF     Date: 05/05/05  Time: 12:17AM
    Fairly extensive modifications. Switched from a cascaded sequencer setup
    to a dual sequencer. Enabled interrupts for both sequence 1 and 2 upon
    end of conversion. Enabled sequencers to be started by PWM events. 

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/2
    User: JeffF     Date: 04/28/05  Time: 08:47PM
    Began integration of ADC and PWM into project

==============================================================================*/


/*==============================================================================
                              Includes
==============================================================================*/
#include "adcdrv.h"      //Device interface
#include "xassert.h"     //Assert for debugging
#include "config.h"      //for CTRL_CPU_MAXSPEED
#include "Device.h"      //Device header info
#include "devicemacro.h" 

/*==============================================================================
                              external functions
==============================================================================*/
// Assembly code functions
extern void DSP28x_usDelay( uint32 Count );
extern void ISR_fnADCSEQ1INT( void );
extern void ISR_fnADCSEQ2INT( void );

/*==============================================================================
                              Defines
==============================================================================*/
/* ^^^^^^^^^^^^^^^^^^^^^^^ Function Enables ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
                                      //--------------- Public ----------------
#define ADCDRV_FN_SOC_EN              ( TRUE  )//Inc code for start of conv
#define ADCDRV_FN_RESET_EN            ( FALSE )//Exc code for reseting ADC
#define ADCDRV_FN_CTOR_EN             ( TRUE  )//Inc code for setup
#define ADCDRV_FN_DTOR_EN             ( FALSE )//Inc code for dtor
#define ADCDRV_FN_SET_MAXCONV_EN      ( TRUE  )//Inc code for writing max conv
#define ADCDRV_FN_GET_RESULT_REG_EN   ( TRUE  )//Inc code for getting result reg
#define ADCDRV_FN_GET_CHNL_RESULT_EN  ( FALSE )//Inc code for getting chnl result
#define ADCDRV_FN_GET_SEQ_CNT_EN      ( FALSE )//Inc code for getting seq cnt
                                                  
/*^^^^^^^^^^^^^^^^^^^^^^^^^^^ Feature Enable ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
// NOTE: ONLY ENABLE ONE OF THE FOLLOWING 2 DEFINES
// The ADC result registers are dual mapped to peripheral frame 0 and peripheral
// frame 2. If read from peripheral frame 0, the results are right justified, 
// and have zero wait states. If read from peripheral frame 2, they are left 
// justified, and have 2 wait states when reading them. Peripheral frame 0
// is the recommended choice
#define ADCDRV_RESULT_PERIPHERAL_FRAME_0  ( TRUE )
#define ADCDRV_RESULT_PERIPHERAL_FRAME_2  ( !ADCDRV_RESULT_PERIPHERAL_FRAME_0 )

#if( ADCDRV_RESULT_PERIPHERAL_FRAME_0 == FALSE && \
     ADCDRV_RESULT_PERIPHERAL_FRAME_2 == FALSE )
#error Either ADCDRV_RESULT_PERIPHERAL_FRAME_0 or \
       ADCDRV_RESULT_PERIPHERAL_FRAME_2 must TRUE
#endif

#if( ADCDRV_RESULT_PERIPHERAL_FRAME_0 == TRUE && \
     ADCDRV_RESULT_PERIPHERAL_FRAME_2 == TRUE )
#error Only one of ADCDRV_RESULT_PERIPHERAL_FRAME_0 or \
       ADCDRV_RESULT_PERIPHERAL_FRAME_2 must be TRUE
#endif

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^ MACRO'S ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//To reset the ADC module
#define ADCDRV_mRESET()  ( AdcRegs.ADCTRL1.bit.RESET = F280X_ADCTRL1_RESET_ALL )


/*^^^^^^^^^^^^^^^^^^^^^ Channel order selection ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
//This maps the channel to when in the sequence it will be read.
//SEQ_0_CHNL will be read first followed by SEQ_1_CHNL and so on.
//Set each SEQ_n_CHNL to a value in teADC_CHNLS.
#if( defined( CTRL_CONFIG_OPTION_SURVEYOR_A ) )

 #define SEQ1_0_CHNL      ADCDRV_eCHNL_1     //This channel is read first
 #define SEQ1_1_CHNL      ADCDRV_eCHNL_4     //..second
 #define SEQ1_2_CHNL      ADCDRV_eCHNL_0     //..third
 #define SEQ1_3_CHNL      ADCDRV_eCHNL_3     
 #define SEQ1_4_CHNL      ADCDRV_eCHNL_11    
 #define SEQ1_5_CHNL      ADCDRV_eCHNL_7
 #define SEQ1_6_CHNL      ADCDRV_eCHNL_7
 #define SEQ1_7_CHNL      ADCDRV_eCHNL_7

 #define SEQ2_0_CHNL      ADCDRV_eCHNL_5
 #define SEQ2_1_CHNL      ADCDRV_eCHNL_6
 #define SEQ2_2_CHNL      ADCDRV_eCHNL_9
 #define SEQ2_3_CHNL      ADCDRV_eCHNL_2
 #define SEQ2_4_CHNL      ADCDRV_eCHNL_7
 #define SEQ2_5_CHNL      ADCDRV_eCHNL_7
 #define SEQ2_6_CHNL      ADCDRV_eCHNL_7
 #define SEQ2_7_CHNL      ADCDRV_eCHNL_7     //..last

#else

 #define SEQ1_0_CHNL      ADCDRV_eCHNL_1     //This channel is read first
 #define SEQ1_1_CHNL      ADCDRV_eCHNL_4     //..second
 #define SEQ1_2_CHNL      ADCDRV_eCHNL_0     //..third
 #define SEQ1_3_CHNL      ADCDRV_eCHNL_3     
 #define SEQ1_4_CHNL      ADCDRV_eCHNL_2    
 #define SEQ1_5_CHNL      ADCDRV_eCHNL_7
 #define SEQ1_6_CHNL      ADCDRV_eCHNL_7
 #define SEQ1_7_CHNL      ADCDRV_eCHNL_7

 #define SEQ2_0_CHNL      ADCDRV_eCHNL_5
 #define SEQ2_1_CHNL      ADCDRV_eCHNL_6
 #define SEQ2_2_CHNL      ADCDRV_eCHNL_9
 #define SEQ2_3_CHNL      ADCDRV_eCHNL_12
 #define SEQ2_4_CHNL      ADCDRV_eCHNL_7
 #define SEQ2_5_CHNL      ADCDRV_eCHNL_7
 #define SEQ2_6_CHNL      ADCDRV_eCHNL_7
 #define SEQ2_7_CHNL      ADCDRV_eCHNL_7     //..last

#endif

// ADC Delay defines
#define ADC_US_DELAY_POWER_UP               ( 6000L )
#define ADC_US_DELAY_OFFSET_CORR            ( 500L )

// The number of shifts needed to to make the result right justified
#define ADCDRV_RIGHT_JUSTIFY_SHIFT_VAL      ( 4 )

// Defines used to perform offset calibration on all the channels
#define ADCDRV_OFFSET_ERR_CORR_CHAN         ( ADCDRV_eCHNL_7 )
#define ADCDRV_OFFSET_ERR_CORR_SAMPLES      (  16  )
#define ADCDRV_OFFSET_ERR_CORR_STEP         (  30  )
#define ADCDRV_OFFSET_ERR_CORR_MAX          (  255 )
#define ADCDRV_OFFSET_ERR_CORR_MIN          ( -256 )

#define ADCDRV_HARDWARE_REV_CHAN            ( ADCDRV_eCHNL_8 )

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
#if( ADCDRV_FN_GET_CHNL_RESULT_EN == TRUE ) //Function compile option
static const uint16 AdChnlMapping[] =
{
    SEQ1_0_CHNL,
    SEQ1_1_CHNL,
    SEQ1_2_CHNL,
    SEQ1_3_CHNL,
    SEQ1_4_CHNL,
    SEQ1_5_CHNL,
    SEQ1_6_CHNL,
    SEQ1_7_CHNL,
    SEQ2_0_CHNL,
    SEQ2_1_CHNL,
    SEQ2_2_CHNL,
    SEQ2_3_CHNL,
    SEQ2_4_CHNL,
    SEQ2_5_CHNL,
    SEQ2_6_CHNL,
    SEQ2_7_CHNL
};

#define AD_CHNL_ARRAY_SIZE ( sizeof( AdChnlMapping    ) / \
                             sizeof( AdChnlMapping[ 0 ] ) )
#endif
/*==============================================================================
                           Local/Private Variables
==============================================================================*/
static uint16 HardwareRev;

/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/
static void adcdrv_fnPerformAdcOffsetCorr( void );
static void adcdrv_fnReadHardwareRevision( void );

/*==============================================================================
                           Function Definitions
==============================================================================*/
/*******************************************************************************

FUNCTION NAME:
    ADCDRV_fnStartConv

PURPOSE:
    Starts the conversion process for one of the ADC sequencers.

INPUTS:
    Sequence - SEQ1 or SEQ2

OUTPUTS:
    none

NOTES:

VERSION HISTORY:
Version: 1.00  Date: 12-Apr-2005  By: Jeff Fieldhouse
    - Created based on 281x driver
    
Version: 1.10  Date: 02-May-2005  By: Jeff Fieldhouse
    - Added input argument to select between sequence 1 or 2    

*******************************************************************************/
#if( ADCDRV_FN_SOC_EN == TRUE )        //Function compile option
void ADCDRV_fnStartConv( ADCDRV_teSEQ Sequence )
{
    // Check the input parameter
    assert( ( Sequence == ADCDRV_eSEQ_1 ) || ( Sequence == ADCDRV_eSEQ_2 ) );
    
    if( Sequence == ADCDRV_eSEQ_1 )
    {    
        //Put the sequencer back to zero.
        AdcRegs.ADCTRL2.bit.RST_SEQ1 = F280X_ADCTRL2_RSTSEQ1_RST;
    
        //Start the conversion
        AdcRegs.ADCTRL2.bit.SOC_SEQ1 = F280X_ADCTRL2_SOCSEQ1_START;
    }
    else
    {
        //Put the sequencer back to zero.
        AdcRegs.ADCTRL2.bit.RST_SEQ2 = F280X_ADCTRL2_RSTSEQ2_RST;
    
        //Start the conversion
        AdcRegs.ADCTRL2.bit.SOC_SEQ2 = F280X_ADCTRL2_SOCSEQ2_START;   
    }
}
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
    This code puts the AD into a dual sequencer mode (up to 8 channels per seq).

    The reset at the beginning of this function needed as per the TI
    documentation.

    If AD hardware pins change, no sweat, just change the order of what is read
    (which is important since interrupts need these in a certain order) in the
    CHSELSEQn regs.

VERSION HISTORY:
Version: 1.00  Date: 12-Apr-2005  By: Jeff Fieldhouse
    - Created based on 281x driver
Version: 1.00  Date: 02-May-2005  By: Jeff Fieldhouse
    - Changed setup to work as dual sequencer now

*******************************************************************************/
#if( ADCDRV_FN_CTOR_EN == TRUE )  //Function compile enable
void ADCDRV_fnCtor( void )
{    
    // Enable access to protected System Register
    DEVICE_mACCESS_EN();
    
    // Enable the peripheral clock to the ADC module
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = F280X_PCLKCR_CLK_ON;

    // Load a pointer to the SEQ1 ISR function into the vector table 
    PieVectTable.SEQ1INT = &ISR_fnADCSEQ1INT;
    PieVectTable.SEQ2INT = &ISR_fnADCSEQ2INT;    
    
    // Enable the interrupt for Sequence 1 and 2
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;
    PieCtrlRegs.PIEIER1.bit.INTx2 = 1;    
    
    // Disable access to protected register
    DEVICE_mACCESS_DIS();
    
    // Reset the ADC to get to a known state.   This is also required by the
    // devices hardware as documented in the ADC data sheet.
    // Set the "RESET" bit to cause a reset.  Cleared by device hardware.
    ADCDRV_mRESET();

    // The reset of the ADC has a latency of 3 clock cycles, and therefore
    // the other ADC control register bits should not be modified until
    // 3 cycles after the reset.
    DEVICE_mDELAY_US( 1 ); 
            
    // ADC Control register 1 setup (ADCTRL1)
    AdcRegs.ADCTRL1.all = ( F280X_mCFGMSK( ADCTRL1, SEQCASC, DUAL       )
                          | F280X_mCFGMSK( ADCTRL1, SEQOVRD, DIS        )
                          | F280X_mCFGMSK( ADCTRL1, CONTRUN, START_STOP )
                          | F280X_mCFGMSK( ADCTRL1, CPS,     DIVBY_1    )
                          | F280X_mCFGMSK( ADCTRL1, ACQPS,   WIDTH_2    )
                          | F280X_mCFGMSK( ADCTRL1, SUSMOD,  MODE_3     )
                          );

    // ADC control register 2 setup (ADCTRL2)
    AdcRegs.ADCTRL2.all = ( F280X_mCFGMSK( ADCTRL2, EPWMSOCBSEQ2, ENA       )
                          | F280X_mCFGMSK( ADCTRL2, INTMODSEQ2,   EVERY     )
                          | F280X_mCFGMSK( ADCTRL2, INTENASEQ2,   ENA       )
                          | F280X_mCFGMSK( ADCTRL2, SOCSEQ2,      CLR       )
                          | F280X_mCFGMSK( ADCTRL2, RSTSEQ2,      RST       )
                          | F280X_mCFGMSK( ADCTRL2, EXTSOCSEQ1,   DIS       )
                          | F280X_mCFGMSK( ADCTRL2, EPWMSOCASEQ1, ENA       )
                          | F280X_mCFGMSK( ADCTRL2, INTMODSEQ1,   EVERY     )
                          | F280X_mCFGMSK( ADCTRL2, INTENASEQ1,   ENA       )
                          | F280X_mCFGMSK( ADCTRL2, SOCSEQ1,      CLR       )
                          | F280X_mCFGMSK( ADCTRL2, RSTSEQ1,      RST       )
                          | F280X_mCFGMSK( ADCTRL2, EPWMSOCBSEQ,  DIS       )
                          );
                          
    // Enable the external reference 
    // *** Must be done before bandgap is powered ***
    AdcRegs.ADCREFSEL.all = ( F280X_mCFGMSK( ADCREFSEL, REFSEL, EXTERNAL ) );
    
    // ADC control register 3 setup (ADCTRL3)
    // Power up reference, bandgap circuits, and the analog core
    // Set the clock prescaler for the ADC module
    // Max clock frequency is 12.5khz, and is based off the HSPCLK    
    AdcRegs.ADCTRL3.all = ( F280X_mCFGMSK( ADCTRL3, SMODESEL,  SEQ_SAMPLE )
                          | F280X_mCFGMSK( ADCTRL3, ADCPWDN,   PWR_ON     ) 
                          | F280X_mCFGMSK( ADCTRL3, ADCBGRFDN, PWR_ON     )
                          | F280X_mCFGMSK( ADCTRL3, ADCCLKPS,  DIVBY_4    )
                          );    

    // need to wait for 5ms after all circuits are powered up before first conv
    DEVICE_mDELAY_US( ADC_US_DELAY_POWER_UP );

    // Calibrate out the offset for all the channels
    // Must do before setting all the ADCCHSELSEQx registers
    // Otherwise it will screw up the conversion order
    adcdrv_fnPerformAdcOffsetCorr( );
    
    // Read in the control board version number
    adcdrv_fnReadHardwareRevision( );
    
    // Setup the conversion sequences
    // Note that the SEQ_X_CHNL are defined to be whatever 
    // channel you want at the top of this file
    // Conversion sequence register 1 (ADCCHSELSEQ1)
    AdcRegs.ADCCHSELSEQ1.all = ( SEQ1_0_CHNL << 0
                               | SEQ1_1_CHNL << 4
                               | SEQ1_2_CHNL << 8
                               | SEQ1_3_CHNL << 12
                               );
                               
    // Conversion sequence register 2 (ADCCHSELSEQ2)
    AdcRegs.ADCCHSELSEQ2.all = ( SEQ1_4_CHNL << 0
                               | SEQ1_5_CHNL << 4
                               | SEQ1_6_CHNL << 8
                               | SEQ1_7_CHNL << 12
                               );
                               
    // Conversion sequence register 3 (ADCCHSELSEQ3)
    AdcRegs.ADCCHSELSEQ3.all = ( SEQ2_0_CHNL << 0
                               | SEQ2_1_CHNL << 4
                               | SEQ2_2_CHNL << 8
                               | SEQ2_3_CHNL << 12
                               );
                               
    // Conversion sequence register 4 (ADCCHSELSEQ4)
    AdcRegs.ADCCHSELSEQ4.all = ( SEQ2_4_CHNL << 0
                               | SEQ2_5_CHNL << 4
                               | SEQ2_6_CHNL << 8
                               | SEQ2_7_CHNL << 12
                               );                                                                                                                                                                                                                         
    
    // Set sequence 1 and 2 to perform the appropriate number of conversions
    AdcRegs.ADCMAXCONV.all = ( F280X_mCFGMSK( ADCMAXCONV, MAXCONV1, CONV_5 )
                             | F280X_mCFGMSK( ADCMAXCONV, MAXCONV2, CONV_4 )
                             );
    
    // Enable interrupts on INT1 
    DEVICE_mINT1_EN();      
}
#endif  //#if( ADCDRV_FN_CTOR_EN == TRUE )

/*******************************************************************************

FUNCTION NAME:
    ADCDRV_fnDtor

PURPOSE:
    Shuts down the ADC module to reduce power consumption.

INPUTS:
    None

OUTPUTS:
    None

NOTES:

VERSION HISTORY:
Version: 1.00  Date: 11-Apr-2005  By: Jeff Fieldhouse
    - Created

*******************************************************************************/
#if( ADCDRV_FN_DTOR_EN == TRUE )  //Function compile enable
void ADCDRV_fnDtor( void )
{
    // Shutdown the analog circuitry
    AdcRegs.ADCTRL3.all = ( F280X_mCFGMSK( ADCTRL3, ADCPWDN,   PWR_OFF    ) 
                          | F280X_mCFGMSK( ADCTRL3, ADCBGRFDN, PWR_OFF    )
                          );
    
    // Set to use internal reference voltage
    AdcRegs.ADCREFSEL.bit.REF_SEL = F280X_ADCREFSEL_REFSEL_INTERNAL;
    
    // Enable access to protected System Register
    DEVICE_mACCESS_EN();

    // Disable the interrupts for Sequence 1 and 2
    PieCtrlRegs.PIEIER1.bit.INTx1 = 0;
    PieCtrlRegs.PIEIER1.bit.INTx2 = 0;

    // Disable the peripheral clock to the ADC module
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = F280X_PCLKCR_CLK_OFF;

    // Disable access to protected register
    DEVICE_mACCESS_DIS();    
}
#endif

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
} 
#endif  //#if( ADCDRV_FN_RESET_EN == TRUE )


/*******************************************************************************

FUNCTION NAME:
    ADCDRV_fnSetMaxConv

PURPOSE:
    Sets the MaxConv Register for how many conversion need to take place.

INPUTS:
    teSeqNum - Auto Sequencer 1 or 2
    teNumConv  - The number of desired conversions.

OUTPUTS:
    none

NOTES:
    Instead of using run time error checking, the assert is used during
    development.

VERSION HISTORY:
Version: 1.00  Date: 12-Apr-2005  By: Jeff Fieldhouse
    - Created based on 281x driver   
    
Version: 1.10  Date: 02-May-2005  By: Jeff Fieldhouse
    - Added input argument to select between sequence 1 or 2
    
*******************************************************************************/
#if( ADCDRV_FN_SET_MAXCONV_EN == TRUE )        //Function compile option
void ADCDRV_fnSetMaxConv( ADCDRV_teSEQ teSeqNum, ADCDRV_teNUM_CONV teNumConv )
{
    // Perform range checking in debug mode only
    assert( ( teSeqNum == ADCDRV_eSEQ_1 ) || ( teSeqNum == ADCDRV_eSEQ_2 ) );
    assert( teNumConv >= ADCDRV_eNUM_CONV_1 );
    assert( teNumConv < ADCDRV_eNUM_CONV_MAX );

    // Setup the number of conversions to take place
    if( teSeqNum == ADCDRV_eSEQ_1 )
    {       
        AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = teNumConv;
    }
    else
    {
        AdcRegs.ADCMAXCONV.bit.MAX_CONV2 = teNumConv;   
    }
}
#endif //#if( ADCDRV_FN_SET_MAXCONV_EN == TRUE )

/*******************************************************************************

FUNCTION NAME:
    ADCDRV_fnGetResultReg

PURPOSE:
    To retrieve a raw un modified adc result using a specified result register.

INPUTS:
    teAdcReg - Desired result register which is linked to a specific channel

OUTPUTS:
    The raw unmodified adc result.

NOTES:

VERSION HISTORY:
Version: 1.00  Date: 12-Apr-2005  By: Jeff Fieldhouse
    - Created based on 281x driver

*******************************************************************************/
#if( ADCDRV_FN_GET_RESULT_REG_EN == TRUE ) //Function compile option
uint16 ADCDRV_fnGetResultReg( ADCDRV_teRESULT_REG teAdcReg )
{
    //Make sure the input parameter is good to go!
    assert( teAdcReg < ADCDRV_eRESULT_REG_MAX );

    //Using Result0 as the based address, add the value of the channel number
    //to the address of the result reg to get the desired result register.
    //So if Result0 is at address 0x7108, this would store the result of
    //channel 0.  0x7108 + 0 = 0x7108 and so on.
#if( ADCDRV_RESULT_PERIPHERAL_FRAME_0 == TRUE )
    
    return( *( &( AdcMirror.ADCRESULT0 ) + teAdcReg ) );

#elif( ADCDRV_RESULT_PERIPHERAL_FRAME_2 == TRUE )

    return( ( *( &( AdcRegs.ADCRESULT0 ) + teAdcReg ) ) 
            >> ADCDRV_RIGHT_JUSTIFY_SHIFT_VAL );

#endif // ( ADCDRV_RESULT_PERIPHERAL_FRAME_0 == TRUE )

}
#endif // #if( ADCDRV_FN_GET_RESULT_REG_EN == TRUE )

/*******************************************************************************

FUNCTION NAME:
    ADCDRV_fnGetBoardRevision

PURPOSE:
    Retreive the control board version number which is based on an analog 
    reading on one of the ADC pins

INPUTS:
    none

OUTPUTS:
    uint16 - an integer number that indicates the board version
             The first board version will be version 1.

NOTES:
    The voltage range for each board version is specified in the requirements
    for this project which are located in twiki

VERSION HISTORY:
Version: 1.00  Date: 12-Apr-2005  By: Jeff Fieldhouse
    - Created

*******************************************************************************/
uint16 ADCDRV_fnGetBoardRevision( void )
{
    // Instead of using a big if statement to check which voltage range the 
    // board version pin falls into, I came up with a way to do a couple 
    // calculations that will convert the ADC result into a board version 
    // number defined in the requirements
    if( HardwareRev < 205 )
    {
        // Essentially, the voltage sensed was zero, which indicates board rev 1
        return( 1 );
    }
    else
    {        
        // This calculation assumes that the 3 volt range of the ADC is broken
        // up into 11 ranges. ie. 0V - 0.15V, 0.15V - 0.45V, 0.45V - 0.75V, etc
        // with the center points being 0V, 0.3V, 0.6V, 0.9V.... up to 3.0V
        return( ( ( HardwareRev - 205 ) / 410 ) + 2 );
        
        // FYI on where the magic numbers come from. 
        // A max result for a 12 bit ADC is 4095, which divided by 10 equals
        // 410 when rounded. 205 is half this value and serves as a way to split
        // up the range. I add 2 at the end because this gives us proper mapping
        // for the board version number.
    }
} 

/*******************************************************************************

FUNCTION NAME:
    ADCDRV_fnGetResultChnl

PURPOSE:
    To retrieve a raw un modified adc result by using the channel number.

INPUTS:
    teChnl - Desired ADC channel result

OUTPUTS:
    The raw unmodified adc result.

NOTES:

VERSION HISTORY:
Version: 1.00  Date: 12-Apr-2005  By: Jeff Fieldhouse
    - Created based on 281x driver

*******************************************************************************/
#if( ADCDRV_FN_GET_CHNL_RESULT_EN == TRUE ) //Function compile option
uint16 ADCDRV_fnGetResultChnl( ADCDRV_teCHNLS teChnl )
{
    uint16 Cnt;

    //Make sure the input parameter is good to go!
    assert( teChnl < ADCDRV_eCHNL_MAX );

    //Since the channel order isn't chronological, we need to search
    //the array of channels in order to find the proper result reg.  The
    //result reg is mapped so the index of the AdChnlMapping array.
    for( Cnt = 0; Cnt < AD_CHNL_ARRAY_SIZE; Cnt++ )
    {
        if( AdChnlMapping[ Cnt ] == teChnl )
        {
            //Build the address which is the base (&AdcMirror.ADCRESULT0)+offset
#if( ADCDRV_RESULT_PERIPHERAL_FRAME_0 == TRUE )

            return( *( &( AdcMirror.ADCRESULT0 ) + Cnt ) );

#elif( ADCDRV_RESULT_PERIPHERAL_FRAME_2 == TRUE )

            return( ( *( &( AdcRegs.ADCRESULT0 ) + Cnt )) 
                    >> ADCDRV_RIGHT_JUSTIFY_SHIFT_VAL );
#endif // #if( ADCDRV_RESULT_PERIPHERAL_FRAME_0 == TRUE )
        }
    }

    //should not get here in normal condition
    assert( FALSE );
    return( 0 );

} 
#endif // #if( ADCDRV_FN_GET_CHNL_RESULT_EN == TRUE )


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
    The number of conversions left in the currently running sequence

NOTES:
    The returned result is the value of how many conversions are left.
    This is setup to get the number based on the two sequencers being
    cascaded.

VERSION HISTORY:
Version: 1.00  Date: 12-Apr-2005  By: Jeff Fieldhouse
    - Created based on 281x driver
    
*******************************************************************************/
#if( ADCDRV_FN_GET_SEQ_CNT_EN == TRUE )   //Function compile option
uchar8 ADCDRV_fnGetSeqCnt( void )
{
    return( AdcRegs.ADCASEQSR.bit.SEQ_CNTR );
}
#endif // #if( ADCDRV_FN_GET_SEQ_CNT_EN == TRUE )

/*******************************************************************************

FUNCTION NAME:
    ADCDRV_fnGetSeqCnt

PURPOSE:
    Checks if the sequencer is still busy or not

INPUTS:
    SeqNum - The sequencer that you want to check

OUTPUTS:
    return - TRUE means that the sequencer is busy
           - FALSE means that the sequencer is not busy

NOTES:

VERSION HISTORY:
Version: 1.00  Date: 09-Apr-2005  By: Jeff Fieldhouse
    - Created.

*******************************************************************************/
tucBOOL ADCDRV_fnGetSeqBusyStatus( ADCDRV_teSEQ teSeqNum )
{
    // Perform range checking in debug mode only
    assert( ( teSeqNum == ADCDRV_eSEQ_1 ) || ( teSeqNum == ADCDRV_eSEQ_2 ) );    
    
    if( teSeqNum == ADCDRV_eSEQ_1 )
    {
        // Check if greater than zero to force return value to either a T or F
        return( AdcRegs.ADCST.bit.SEQ1_BSY > 0 );
    }
    else
    {
        // Check if greater than zero to force return value to either a T or F
        return( AdcRegs.ADCST.bit.SEQ2_BSY > 0 );
    }
}

/*******************************************************************************

FUNCTION NAME:
    ADCDRV_fnSeq1Isr

PURPOSE:
    Handles the interrupt for sequence 1 end of conversion

INPUTS:
    none

OUTPUTS:
    none

NOTES:

VERSION HISTORY:
Version: 1.00  Date: 02-May-2005  By: Jeff Fieldhouse
    - Created    

*******************************************************************************/
void ADCDRV_fnSeq1Isr( void )
{
    // Reset the sequence in preparation for the next conversion sequence
    AdcRegs.ADCTRL2.bit.RST_SEQ1 = F280X_ADCTRL2_RSTSEQ1_RST;
                              
    // Clear the sequence 1 interrupt flag
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;
    
    // Re-enable interrupts on INT1
    PieCtrlRegs.PIEACK.all |= IER_INT1_ACK;
}

/*******************************************************************************

FUNCTION NAME:
    ADCDRV_fnSeq2Isr

PURPOSE:
    Handles the interrupt for sequence 2 end of conversion

INPUTS:
    none

OUTPUTS:
    none

NOTES:

VERSION HISTORY:
Version: 1.00  Date: 02-May-2005  By: Jeff Fieldhouse
    - Created    

*******************************************************************************/
void ADCDRV_fnSeq2Isr( void )
{   
    // Reset the conversion in preparation for the next conversion sequence
    AdcRegs.ADCTRL2.bit.RST_SEQ2 = F280X_ADCTRL2_RSTSEQ2_RST;    
    
    // Clear the sequence 2 interrupt flag
    AdcRegs.ADCST.bit.INT_SEQ2_CLR = 1;
    
    // Re-enable interrupts on INT1
    PieCtrlRegs.PIEACK.all |= IER_INT1_ACK;
}

/*******************************************************************************

FUNCTION NAME:
    adcdrv_fnPerformAdcOffsetCorr

PURPOSE:
    Performs offset error correction on the ADC result registers.

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    This function runs once when the constructor is called, and that's it. 
    There is no run time penalty in regards to offset error correction. The
    result registers automatically include the offset in their results.

VERSION HISTORY:
Version: 1.00  Date: 11-Apr-2005  By: Jeff Fieldhouse
    - Created

*******************************************************************************/
static void adcdrv_fnPerformAdcOffsetCorr( void )
{
    uint16  i;
    tucBOOL ZeroFlag; 
    uint16  Result[ ADCDRV_OFFSET_ERR_CORR_SAMPLES ];
    uint16  ResultTotal = 0;
    sint16  OffsetCorr;
    
    // Set the sequence register to convert the appropriate channel
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = ADCDRV_OFFSET_ERR_CORR_CHAN;

    // Perform the following loop until a positive offset is calculated
    // If the required offset exceeds 255, there is a problem, so exit this loop
    while( TRUE )
    {
        // Run a loop that obtains x amount of samples on the specific 
        // channel that is connected to ADCLO
        for( i = 0; i < ADCDRV_OFFSET_ERR_CORR_SAMPLES; i++ )
        {
            // Delay between each measurement
            DEVICE_mDELAY_US( ADC_US_DELAY_OFFSET_CORR );
            
            // Tell the sequencer to perform only one sample
            ADCDRV_fnSetMaxConv( ADCDRV_eSEQ_1, ADCDRV_eNUM_CONV_1 );
            
            // Start the conversion
            ADCDRV_fnStartConv( ADCDRV_eSEQ_1 );
            
            // Delay until the sequencer is no longer busy
            while( ADCDRV_fnGetSeqBusyStatus( ADCDRV_eSEQ_1 ) )
            {
            }
            
            // Read the result into the array
            Result[ i ] = ADCDRV_fnGetResultReg( ADCDRV_eRESULT_REG_0 );            
        }
        
        // Initialize the zero flag to false
        ZeroFlag = FALSE;
        
        // Scan the results array for any zero values
        // A zero value indicates that there might be a negative offset,
        // in which case, we set the offset trim reg to some positive value
        // and then redo the samples
        for( i = 0; i < ADCDRV_OFFSET_ERR_CORR_SAMPLES; i++ )
        {
            // Check if the result equaled zero
            if( Result[ i ] == 0 )
            {
                AdcRegs.ADCOFFTRIM.bit.OFFSET_TRIM += ADCDRV_OFFSET_ERR_CORR_STEP;
                ZeroFlag = TRUE;
                break;              
            }           
        }
        
        // If something is wrong, this line of code will break the forever loop
        if( AdcRegs.ADCOFFTRIM.bit.OFFSET_TRIM > ADCDRV_OFFSET_ERR_CORR_MAX )
        {
            // Shouldn't ever enter this if(), if do, stop processor if in debug           
            assert( FALSE );
            
            // if not debugging, set the trim register to max and return
            AdcRegs.ADCOFFTRIM.bit.OFFSET_TRIM = ADCDRV_OFFSET_ERR_CORR_MAX;
            return;
        }
        
        // Used to break out of the while( TRUE ) loop
        // Indicates all the conversion codes are non zero (which is good)
        if( ZeroFlag == FALSE )
        {
            break;
        }           
    }
          
    // Add up all the conversion results
    for( i = 0; i < ADCDRV_OFFSET_ERR_CORR_SAMPLES; i++ )
    {
        ResultTotal += Result[ i ];
    }
    
    // To ensure we get proper rounding, add a value that is half the number 
    // of samples before we divide by num of samples (same as adding 0.5)
    ResultTotal += ( ADCDRV_OFFSET_ERR_CORR_SAMPLES >> 1 );
    
    // Calculate the average conversion value by dividing by num of samples
    OffsetCorr = ( sint16 )( ResultTotal / ADCDRV_OFFSET_ERR_CORR_SAMPLES );
    
    // Subtract the average conversion value from the trim register
    // This value becomes the official (calibrated) trim value
    OffsetCorr = ( ( sint16 )AdcRegs.ADCOFFTRIM.bit.OFFSET_TRIM - OffsetCorr );
    
    // Clamp the value at a minimum (max was done early in function)
    if( OffsetCorr > ADCDRV_OFFSET_ERR_CORR_MIN )
    {
        AdcRegs.ADCOFFTRIM.bit.OFFSET_TRIM = OffsetCorr;
    }
    else
    {
        // This statement causes a warning, but it is ok, it works as is
        AdcRegs.ADCOFFTRIM.bit.OFFSET_TRIM = ADCDRV_OFFSET_ERR_CORR_MIN;
    }   
}

/*******************************************************************************

FUNCTION NAME:
    adcdrv_fnReadHardwareRevision

PURPOSE:
    Does a one time read of the voltage level present on the ADC pin used to 
    indicate the control boards version number. The resulting digital value is
    used to map into a predefined range for board version numbers.

INPUTS:
    None

OUTPUTS:
    None

NOTES:

VERSION HISTORY:
Version: 1.00  Date: 08-May-2006  By: Jeff Fieldhouse
    - Created

*******************************************************************************/
static void adcdrv_fnReadHardwareRevision( void )
{    
    uint16 i;
    uint32 Result;
    
    // Set the sequence register to convert the appropriate channel
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = ADCDRV_HARDWARE_REV_CHAN;

    Result = 0L;
    
    // Read the hardware revision number 8 times so that we can get an average
    for( i = 0; i < 8; i++ )
    {
        // Delay between each measurement
        DEVICE_mDELAY_US( ADC_US_DELAY_OFFSET_CORR );
        
        // Tell the sequencer to perform only one sample
        ADCDRV_fnSetMaxConv( ADCDRV_eSEQ_1, ADCDRV_eNUM_CONV_1 );
        
        // Start the conversion
        ADCDRV_fnStartConv( ADCDRV_eSEQ_1 );
        
        // Delay until the sequencer is no longer busy
        while( ADCDRV_fnGetSeqBusyStatus( ADCDRV_eSEQ_1 ) )
        {
        }
        
        // Read the result into the running total
        Result += ( uint32 )ADCDRV_fnGetResultReg( ADCDRV_eRESULT_REG_0 );            
    }
    
    // Divide the running total according to how many conversions we did
    HardwareRev = ( uint16 )( Result >> 3 );               
} 

