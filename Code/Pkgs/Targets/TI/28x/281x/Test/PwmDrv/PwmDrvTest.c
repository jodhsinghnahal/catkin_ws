/*==============================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:  
    PwmDrvTest.c

PURPOSE:
    To test the PWMDRV package.

FUNCTION(S):
    See Prototypes
   
    local:
        -
   
NOTES:
    Tests (EVA) PWM's 1, 2, 3, & 4, and (EVB) PWM 7. EVA PWM's are set up for
    symmetric output with deadband as used for H-bridge control.  EVB PWM 7 
    is set up as it will be used for fan control - 20KHz. frequency, symmetric
    output.
    
    TO TEST:
    Connect a scope to the PWM outputs in order to see that they
    are doing.  Are they present?  Are they at the correct duty cycle?  Are
    they behaving the way the unit test expects them to?
    
    The program does:
     - 5 seconds of a constant PWM
     - 5 seconds of a PWM increasing the decreasing (you 'll see a changing 
       PWM waveform)
     - 5 second of the constant PWM but it will be enabled and disabled each
       second (so PWM on and PWM off...PWM on and PWM off...)
       
    SPECIAL NOTES
    -------------
    - write a large value to the PWM doesn't hurt it you just get "clipping"
      or a squared off wave.  So this wasn't tested/tried here.

CHANGE HISTORY :
$Log: PwmDrvTest.c $

    ***********************************************
    Revision: NovaPfrmB_Local_HorstM/4
    User: HorstM     Date: 03/01/05  Time: 05:17PM
    using latest isr arrangement

    

    ***********************************************
    Revision: NovaPfrmB_Local_HorstM/3
    User: HorstM     Date: 02/21/05  Time: 12:54AM
    changed to use rtos and new (timer) isr structure so a properly functioning
    timer can be used to test & operate the pwm's
    

    ***********************************************
    Revision: NovaPfrmB_Local_HorstM/2
    User: HorstM     Date: 02/17/05  Time: 07:31PM
    changed to use rtos and new isr structure
    

    ***********************************************
    Revision: NovaPfrmB_Local_HorstM/1
    User: HorstM     Date: 02/12/05  Time: 01:18AM
    removed dead code
    

    ***********************************************
    Revision: NovaPfrmB_HorstM/4
    User: HorstM     Date: 01/20/05  Time: 09:40PM
    removed peripheral interrupt and output pin enabling
    and moved global interrupt enable
    

    ***********************************************
    Revision: NovaPfrmB_HorstM/3
    User: HorstM     Date: 01/20/05  Time: 06:07PM
    removed interrupt enabling (now handled by timer driver)
    
    

    ***********************************************
    Revision: NovaPfrmB_HorstM/2
    User: HorstM     Date: 01/20/05  Time: 05:36PM
    changed TMRDRV_teFN element in fan timer object to
    work with revised timer driver
    

==============================================================================*/


/*==============================================================================
                              Includes
==============================================================================*/
#include "Pwmdrv.h"			//Package to be tested
#include "PwmDrvC28xOpt.h"	//Part of the package to be tested
#include "Device.h"			//Some device specific stuff
#include "DeviceMacro.h"
#include "config.h"
#include "Tmrdrv.h"			//Used in conjunction with pwm
#include "init.h"           //for sysinit and clockspeed functions
#include "includes.h"
#include "uCOS_II.h"        // for OSTaskDel
#include "isr.h"        // isr declarations
#include "PieVect.h"    // for PIE vector table initialization
#include "xassert.h"		//For asserts

/*==============================================================================
                              external functions
==============================================================================*/
extern void DSP28x_usDelay( uint32 Count );

/*==============================================================================
                              Defines
==============================================================================*/
#define PWM_mPIN_TOGGLE()   ( GpioDataRegs.GPFTOGGLE.bit.GPIOF14 = 1 )

// ---------------------------------------------------------------------------
// Delay microsecond macros
// ---------------------------------------------------------------------------
//
// PWM_mCPU_RATE is the # of nanoseconds per clock cycle, so for example:
//
// System when MIPS = 150MHz
//
// PWM_mCPU_RATE = 1 / 150MHz = 6.6667nS/clock cycle
//
// This value is then passed into the DSP28x_usDelay function for processing
#define PWM_mCPU_RATE   ( (uint32)( ( 1 / (float) CTRL_CPU_MAXSPEED ) \
                              * (float)1.0E9 ) )
// This is a formula derived by TI, They convert the input value into
// nano-seconds using:
//
// ( long double ) A * 1000L
//
// Then they find the number of clock cycles required:
//
// ( long double ) A * 1000L / ( long double ) PWM_mCPU_RATE
//
// According the ASM function created by TI, there is 9/10 of a cycle of
// overhead for each loop and each run through the loop takes 5 cycles
//
// ( ( (long double)A * 1000L / (long double)PWM_mCPU_RATE ) - 9.0L ) / 5.0L
//
#define PWM_mDELAY_US(A)  DSP28x_usDelay( ( ( ( (uint32)A * 1000.0L ) \
                                          / PWM_mCPU_RATE )   \
                                          - 9.0L ) / 5.0L )

//Delay using ns unit
#define PWM_mDELAY_NS(A)  DSP28x_usDelay( ( ( (uint32)A  \
                                          / PWM_mCPU_RATE )   \
                                          - 9.0L ) / 5.0L )
/*==============================================================================
                              Defines
==============================================================================*/
//This test code requires NDEBUG to NOT BE DEFINED so asserts can be used
//to track issues captured by the tests.
#ifdef NDEBUG
#undef NDEBUG	
#endif

// OS stuff
#define  TASK_STK_SIZE     256
#define  START_TASK_PRIO   11
#define  EEP_TASK_PRIO     10
#define  START_TASK_ID     START_TASK_PRIO

#define TEST_PASS_SPEED ( 1000000 )	// 1,000,000 us (1sec)
#define TEST_FAIL_SPEED ( 500000  )	// 500,000 us (0.5sec)

#define DESIRED_DEAD_BAND_NS    ( 1000  ) //Dead band desired in nanoseoncds
#define DESIRED_PWM_PRD         ( 12000 ) //In Hz
#define DESIRED_FAN_PRD         ( 20000 ) //In Hz
#define UP_DOWN_CNT_LMT			( 10000	) //Used in loops for playing w/ the PWM
#define PWM_DFLT_TIMER_PRD      ( 10000 ) //10Khz timer period

/*==============================================================================
                            Type Definitions
==============================================================================*/


/*==============================================================================
                                Enums
==============================================================================*/
typedef enum PWMDRV_eINDEX
{
    PWMDRV_ePWM_1 = 0,
    PWMDRV_ePWM_2,
    PWMDRV_ePWM_3,
    PWMDRV_ePWM_4,
    PWMDRV_ePWM_5,
    PWMDRV_ePWM_6,
    PWMDRV_ePWM_MAX
}PWMDRV_teINDEX;

/*=============================================================================
                              Structures
==============================================================================*/
//EVA PWM PAIRS
PWMDRV_tzDATA   tzEvaPwm1;
PWMDRV_tzDATA   tzEvaPwm2;
PWMDRV_tzDATA   tzEvaPwm3;
//EVB PWM PAIRS
PWMDRV_tzDATA   tzEvbPwm4;
PWMDRV_tzDATA   tzEvbPwm5;
PWMDRV_tzDATA   tzEvbPwm6;

PWMDRV_tzDATA *pwms[] = { &tzEvaPwm1,
                          &tzEvaPwm2,
                          &tzEvaPwm3,
                          &tzEvbPwm4,
                          &tzEvbPwm5,
                          &tzEvbPwm6 };

PWMDRV_tzDATA *ptzBCPwm1;      // pointer to bridge control pwm - pair 1
PWMDRV_tzDATA *ptzBCPwm2;      // pointer to bridge control pwm - pair 2
PWMDRV_tzDATA *ptzFanPwm;      // pointer to fan control pwm

//The user/target specific options.
PWMDRV_tzOPTIONS_TI_C28X tzBCPwmOptions;
PWMDRV_tzOPTIONS_TI_C28X tzFanPwmOptions;

TMRDRV_tzTIMER tzBCTimer = { TMRDRV_ID_DFLT        , // ucTimerID
                             DESIRED_PWM_PRD       , // ulTimerFreq
                             TMRDRV_DUTY_DFLT      , // uiTimerDutyPct
                             TMRDRV_eINT_PRD       , // teTimerInt
                             TMRDRV_eCONTUPDOWN    , // teTimerCountMode
                             TMRDRV_eADC_SEPERATE  , // teTimerAdcMode
                             TMRDRV_eNO_OUTPUT     , // teTimerCompOutput
                             TMRDRV_eEMU_STOP_IMM  , // teTimerEmu
                             TMRDRV_eNO_COMP       , // teTimerCompMode
                             TMRDRV_eNO_SYN        , // teTimerSyn
                             TMRDRV_ID_DFLT        , // uiTimerIDSyn
                             TMRDRV_eINTERNAL      , // teTimerClock
                             TMRDRV_ePWM           , // teTimeFunc
                             TMRDRV_INTERUPT_DFLT    // uiTimerInterrupt
                            };
                    
TMRDRV_tzTIMER tzFanTimer = { TMRDRV_ID_DFLT        , // ucTimerID
                              DESIRED_FAN_PRD       , // ulTimerFreq
                              TMRDRV_DUTY_DFLT      , // uiTimerDutyPct
                              TMRDRV_eINT_PRD       , // teTimerInt
                              TMRDRV_eCONTUPDOWN    , // teTimerCountMode
                              TMRDRV_eADC_SEPERATE  , // teTimerAdcMode
                              TMRDRV_eNO_OUTPUT     , // teTimerCompOutput
                              TMRDRV_eEMU_STOP_IMM  , // teTimerEmu
                              TMRDRV_eNO_COMP       , // teTimerCompMode
                              TMRDRV_eNO_SYN        , // teTimerSyn
                              TMRDRV_ID_DFLT        , // uiTimerIDSyn
                              TMRDRV_eINTERNAL      , // teTimerClock
                              TMRDRV_eFAN           , // teTimeFunc
                              TMRDRV_INTERUPT_DFLT    // uiTimerInterrupt
                             };
                    
TMRDRV_tzTIMER *ptzBCTimer = &tzBCTimer;
TMRDRV_tzTIMER *ptzFanTimer = &tzFanTimer;


/*==============================================================================
                           Local/Private Constants
==============================================================================*/


/*==============================================================================
                           Local/Private Variables
==============================================================================*/
//if FALSE and an assert happens it means weren't testing for that we new would
//cause an assert.  So FALSE a "real" assert wile TRUE would mean we are
//intentially causing an assert.
static tucBOOL AssertTest = FALSE;
static uint16  AssertCnt  = 0;
static uchar8  PwmIndex   = PWMDRV_ePWM_1;
uint16 errvalue = 0;
uint16 loopcount = 0;

#pragma DATA_SECTION( TaskStartStk, ".stack" );
OS_STK  TaskStartStk[ TASK_STK_SIZE ] = { 0 };
/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/
void    TEST_fnTestTask( void *pvData );
void    TEST_fnSetupDevice( void );
void    TEST_fnInit( void );
tucBOOL TEST_fnSetup( void );
void    TEST_fnStartPwm( PWMDRV_tzDATA * ptzPwm );
void    TEST_fnStopPwm( void );
void    TEST_fnDoPinToggle( uint32 Speed );

void TEST_fnLimitsAndAsserts( void );
void TEST_fnConstantPwming( void );
void TEST_fnVariablePwming( void );
void TEST_fnConstantPwmingDis( void );
void TEST_fnChangeOptions( void );

// misc functions
void SYS_fnAbort( char *Warning );

//pseudo-isr's
void DIGCTRL_fnIsrPwm( void );
void SWBASE_fnIsrSw( void );
void FAN_fnIsrPwm( void );

// isr stubs (declarations of...)
void ACOL_fnSetPkOvrI( void );
void CANDRV_fnECAN0INTIsr( void );
void CANDRV_fnECAN1INTIsr( void );
void SCIDRV_fnRxIsr( void );
void SCIDRV_fnTxIsr( void );
void SPIDRV_fnRxIsr( void );
void SPI_fnTxIsr( void );
void ZEROX_fnISRL1( void );
void ZEROX_fnISRL2( void );
/*==============================================================================
                           Function Definitions
==============================================================================*/

/*******************************************************************************

FUNCTION NAME: 
    main
  
PURPOSE: 
    Start of the program

INPUTS: 
    None

OUTPUTS:
    None

NOTES:
    None

VERSION HISTORY:

Version: 1.00  Date: May 16, 2003  By: John Bellini
    - Created

*******************************************************************************/
void main( void )
{
    //¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
    //Test and Device Setup
    //¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
    TEST_fnSetupDevice();

    // GEL script label
    _IntroBreak:
    ;

    //Initialize the RTOS
    OSInit( );

    // Create the test task
    //
    OSTaskCreateExt( TEST_fnTestTask,
                    ( void * )0,
                    &TaskStartStk[ 0 ],
                    START_TASK_PRIO,
                    START_TASK_ID,
                    &TaskStartStk[ TASK_STK_SIZE - 1 ],
                    TASK_STK_SIZE,
                    ( void * )0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR );

    // Start multitasking
    //
    OSStart( );
}

/*******************************************************************************

FUNCTION NAME: 
    TEST_fnTestTask
  
PURPOSE: 

INPUTS: 

OUTPUTS:

NOTES:

VERSION HISTORY:

Version 1.00    Date: DEC-3-2004     By: Horst Maurer
    - Created

*******************************************************************************/
void TEST_fnTestTask( void *pvData )
{
	//The test loop starts here.
    while( errvalue == 0 )
    {
        // must be called after pwm dtor called to re-init pwm id's
        TEST_fnInit();

        TEST_fnSetup();

    	TEST_fnChangeOptions();

        //Test the limits and asserts for the target package under test.
        TEST_fnLimitsAndAsserts();

    	//5 seconds of constant output on all enabled PWM pairs in EVA
    	TEST_fnConstantPwming();

    	//5 seconds of variable Pwming on all enabled PWM pairs in EVA
    	TEST_fnVariablePwming();

    	//Disable one PWM pair and enable another, then switch
    	//Test the and enable disable functionality.
		TEST_fnConstantPwmingDis();

    	//If we get here, then test is working.
        TEST_fnDoPinToggle( TEST_PASS_SPEED );

        // kill all test objects so they can be recycled
        TEST_fnStopPwm();

        loopcount++;

        // GEL script label
        _MainBreak:
        ;
    }
}

/*******************************************************************************

FUNCTION NAME: 
    TEST_fnSetupDevice
  
PURPOSE: 
    To setup the target

INPUTS: 
    None

OUTPUTS:
    None

NOTES:
    None

VERSION HISTORY:

Version: 1.00  Date: May 16, 2003  By: John Bellini
    - Created

Version: 1.10  Date: JAN-20-2005  By: Horst Maurer
    - removed peripheral interrupt enabling
    - added global interrupt enabling

*******************************************************************************/
void TEST_fnSetupDevice( void )
{
    // call the init package to initialize the system
    INIT_fnSysInit();
    
	// Disable and clear all CPU interrupts
   	DEVICE_mGLOBAL_INT_DIS();
   	DEVICE_mALL_INT_DIS();
   	DEVICE_mALL_INT_FLAG_CLEAR();

    // Initialize PIE control registers to their default state:
    // This function is found in the DSP281x_PieCtrl.c file.
    InitPieCtrl();

    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    // This will populate the entire table, even if the interrupt
    // is not used in this example.  This is useful for debug purposes.
    // The shell ISR routines are found in DSP281x_DefaultIsr.c.
    // This function is found in DSP281x_PieVect.c.
    PIE_fnInitPieVectTable();

    // must allow access to the EALLOW protected registers before we can do
    // anything useful.
   	EALLOW;	// This is needed to write to EALLOW protected registers

	// Interrupts that are used in this example are re-mapped to
	// ISR functions found within this file.  
	// set up the IO to blink a LED:
    // set up pin 14 (XF_GPIO pin) to be an I/O pin
    GpioMuxRegs.GPFMUX.bit.XF_GPIOF14 = 0;
    // configure the direction of pin 14 (XF_GPIO pin) to be an output
    GpioMuxRegs.GPFDIR.bit.GPIOF14 = 1;

    // Enable the PIE
    PieCtrlRegs.PIECRTL.bit.ENPIE = F28X_PIECTRL_ENPIE_TRUE;

    // enable EV clocks
    SysCtrlRegs.PCLKCR.bit.EVBENCLK   = F28X_PCLKCR_CLK_ON;
    SysCtrlRegs.PCLKCR.bit.EVAENCLK   = F28X_PCLKCR_CLK_ON;

    // enable interrupts globally
    DEVICE_mGLOBAL_INT_EN();

    // disable write access to the protected registers.
    EDIS;
}

/*******************************************************************************

FUNCTION NAME: 
    TEST_fnInit
  
PURPOSE: 
    Initialize the pwm's to be tested

INPUTS: 
    None

OUTPUTS:
    None

NOTES:
    None

VERSION HISTORY:

Version: 1.00  Date: May 21, 2003  By: John Bellini
    - Created

*******************************************************************************/
void TEST_fnInit( void )
{
    // initialize the PwmId's, as is now required to support multiple clients
    tzEvaPwm1.tePwmId = ePWM_ID_1_2;
    tzEvaPwm2.tePwmId = ePWM_ID_3_4;
    tzEvaPwm3.tePwmId = ePWM_ID_5_6;
    tzEvbPwm4.tePwmId = ePWM_ID_7_8;
    tzEvbPwm5.tePwmId = ePWM_ID_9_10;
    tzEvbPwm6.tePwmId = ePWM_ID_11_12;
}

/*******************************************************************************

FUNCTION NAME: 
    TEST_fnSetup
  
PURPOSE: 
    To setup the test

INPUTS: 
    None

OUTPUTS:
    None

NOTES:
    None

VERSION HISTORY:

Version: 1.00  Date: May 21, 2003  By: John Bellini
    - Created

Version: 1.10  Date: JAN-20-2005  By: Horst Maurer
    - removed pwm output pin enabling and global interrupt enabling

*******************************************************************************/
tucBOOL TEST_fnSetup( void )
{
    //Setup the PWM's to be tested
    // H-bridge control - 2 pairs
    //Setup target specific options
    tzBCPwmOptions.teCompEn       = PWMDRV_eCOMP_EN_ON;
    tzBCPwmOptions.teCompReload   = PWMDRV_eCOMP_RELOAD_UF_OR_PRD;
    tzBCPwmOptions.teActionReload = PWMDRV_eACTION_RELOAD_UF;
	tzBCPwmOptions.tePin1State    = PWMDRV_ePOL_ACTIVE_HI;
	tzBCPwmOptions.tePin2State    = PWMDRV_ePOL_ACTIVE_LOW;
    tzBCPwmOptions.teDbTmrEn      = PWMDRV_eDB_TMR_EN;
    tzBCPwmOptions.uiDesiredDbNs  = DESIRED_DEAD_BAND_NS;

    ptzBCPwm1 = pwms[ PwmIndex ];
    //Assign the options struct to the users struct.
    ptzBCPwm1->tePwmType = PWMDRV_eTYPE_SYMMETRIC;
    ptzBCPwm1->ptzPwmOpt = &tzBCPwmOptions;
    ptzBCPwm1->uiDutyCycle = 0;
    ptzBCPwm1->ulDesiredFreqHz = DESIRED_PWM_PRD;

    ptzBCTimer->teTimerCountMode = TMRDRV_eCONTUPDOWN;

    TEST_fnStartPwm( ptzBCPwm1 );

    ptzBCPwm2 = pwms[ PwmIndex + 1 ];
    //Assign the options struct to the users struct.
    ptzBCPwm2->tePwmType = PWMDRV_eTYPE_SYMMETRIC;
    ptzBCPwm2->ptzPwmOpt = &tzBCPwmOptions;
    ptzBCPwm2->uiDutyCycle = 0;
    ptzBCPwm2->ulDesiredFreqHz = DESIRED_PWM_PRD;
    
    TEST_fnStartPwm( ptzBCPwm2 );

    //Create the timer for the pwms
    if( TMRDRV_fnCtor( ptzBCTimer ) == eSTATUS_ERR )
    {
        errvalue |= 0x0001;
    }

    //enable timer interrupts
    if( TMRDRV_fnEnInt( ptzBCTimer ) == eSTATUS_ERR )
    {
        errvalue |= 0x0002;
    }

    //Start the PWM timer.
    if( TMRDRV_fnStart( ptzBCTimer ) == eSTATUS_ERR )
    {
        errvalue |= 0x0004;
    }

    // fan control - 1 pair
    //Setup target specific options
    tzFanPwmOptions.teCompEn       = PWMDRV_eCOMP_EN_ON;
    tzFanPwmOptions.teCompReload   = PWMDRV_eCOMP_RELOAD_UF_OR_PRD;
    tzFanPwmOptions.teActionReload = PWMDRV_eACTION_RELOAD_UF;
	tzFanPwmOptions.tePin1State    = PWMDRV_ePOL_ACTIVE_HI;
    tzFanPwmOptions.teDbTmrEn      = PWMDRV_eDB_TMR_DIS;
    tzFanPwmOptions.uiDesiredDbNs  = 0;

    ptzFanPwm = pwms[ PwmIndex + 3 ];
    //Assign the options struct to the users struct.
    ptzFanPwm->tePwmType = PWMDRV_eTYPE_SYMMETRIC;
    ptzFanPwm->ptzPwmOpt = &tzFanPwmOptions;
    ptzFanPwm->uiDutyCycle = 0;
    ptzFanPwm->ulDesiredFreqHz = DESIRED_FAN_PRD;
    
    ptzFanTimer->teTimerCountMode = TMRDRV_eCONTUPDOWN;

    TEST_fnStartPwm( ptzFanPwm );

    //Create the timer for the pwms
    if( TMRDRV_fnCtor( ptzFanTimer ) == eSTATUS_ERR )
    {
        errvalue |= 0x0001;
    }

    //enable timer interrupts
    if( TMRDRV_fnEnInt( ptzFanTimer ) == eSTATUS_ERR )
    {
        errvalue |= 0x0002;
    }

    //Start the PWM timer.
    if( TMRDRV_fnStart( ptzFanTimer ) == eSTATUS_ERR )
    {
        errvalue |= 0x0004;
    }

	return( TRUE );
}

/*******************************************************************************

FUNCTION NAME: 
    TEST_fnStartPwm
  
PURPOSE: 
    initialize state variables for a pwm

INPUTS: 
    pointer to pwm struct

OUTPUTS:
    None

NOTES:
    None

VERSION HISTORY:

Version: 1.00  Date: DEC-21-2004  By: John Bellini
    - Created

*******************************************************************************/
void TEST_fnStartPwm( PWMDRV_tzDATA * ptzPwm )
{
    if( PWMDRV_fnCtor( ptzPwm ) == eSTATUS_ERR )
    {
        errvalue |= 0x0008;
    }
    
    if( PWMDRV_fnSetDeadBand( ptzPwm ) == eSTATUS_ERR )
    {
        errvalue |= 0x0010;
    }
    
    //Enable the PWM.  The fn's always return OKAY so no need to check
    //the return value.
    (void)PWMDRV_fnEn( ptzPwm );
}

/*******************************************************************************

FUNCTION NAME: 
    TEST_fnStopPwm
  
PURPOSE: 
    terminate this test by destroying all used objects

INPUTS: 
    pointer to pwm struct

OUTPUTS:
    None

NOTES:
    None

VERSION HISTORY:

Version: 1.00  Date: DEC-21-2004  By: John Bellini
    - Created

*******************************************************************************/
void TEST_fnStopPwm( void )
{
    // destroy the timers used so they can be recycled
    TMRDRV_fnDtor( ptzBCTimer );
    TMRDRV_fnDtor( ptzFanTimer );

    // destroy the pwm's used so they can be recycled
    PWMDRV_fnDtor( ptzBCPwm1 );
    PWMDRV_fnDtor( ptzBCPwm2 );
    PWMDRV_fnDtor( ptzFanPwm );
}

/*******************************************************************************

FUNCTION NAME: 
    TEST_fnChangeOptions
  
PURPOSE: 
    To change the setup of the PWM via its API to make sure its API works

INPUTS: 
    None

OUTPUTS:
    None

NOTES:
    None

VERSION HISTORY:

Version: 1.00  Date: Nov 5, 2003  By: John Bellini
    - Created
    
Version: 1.10  Date: Nov 6, 2003  By: John Bellini
    - Updated so all options for pin actions are tested.

*******************************************************************************/
void TEST_fnChangeOptions( void )
{
    static uint16 Tgl = 0;
    
    switch( Tgl )
    {
	    case 0:
	    tzBCPwmOptions.tePin1State = PWMDRV_ePOL_ACTIVE_HI;
	    tzBCPwmOptions.tePin2State = PWMDRV_ePOL_ACTIVE_LOW;
	    break;
	    
	    case 1:
	    tzBCPwmOptions.tePin1State = PWMDRV_ePOL_ACTIVE_LOW;
	    tzBCPwmOptions.tePin2State = PWMDRV_ePOL_ACTIVE_HI;
	    break;
	    
	    case 2:
	    tzBCPwmOptions.tePin1State = PWMDRV_ePOL_ACTIVE_HI;
	    tzBCPwmOptions.tePin2State = PWMDRV_ePOL_ACTIVE_HI;
	    break;
	    
        case 3:
	    tzBCPwmOptions.tePin1State = PWMDRV_ePOL_ACTIVE_LOW;
	    tzBCPwmOptions.tePin2State = PWMDRV_ePOL_ACTIVE_LOW;
	    break;
	    
	    case 4:
	    tzBCPwmOptions.tePin1State = PWMDRV_ePOL_ACTIVE_HI;
	    tzBCPwmOptions.tePin2State = PWMDRV_ePOL_FORCE_LOW;
	    break;
	    
	    case 5:
	    tzBCPwmOptions.tePin1State = PWMDRV_ePOL_ACTIVE_HI;
	    tzBCPwmOptions.tePin2State = PWMDRV_ePOL_FORCED_HI;
	    break;
	    
	    case 6:
	    tzBCPwmOptions.tePin1State = PWMDRV_ePOL_ACTIVE_LOW;
	    tzBCPwmOptions.tePin2State = PWMDRV_ePOL_FORCE_LOW;
	    break;
	    
	    case 7:
	    tzBCPwmOptions.tePin1State = PWMDRV_ePOL_ACTIVE_LOW;
	    tzBCPwmOptions.tePin2State = PWMDRV_ePOL_FORCED_HI;
	    break;
    }

    //Cycle through the number of available configurations of options.    
    ++Tgl;
    if( Tgl >= 8 )
    {
       Tgl = 0;
    }
	
	//Make sure the assignment to the options struct is assigned to the 
	//users struct.
    ptzBCPwm1->ptzPwmOpt = &tzBCPwmOptions;
    ptzBCPwm2->ptzPwmOpt = &tzBCPwmOptions;
	PWMDRV_fnSetOpt( ptzBCPwm1 );
	PWMDRV_fnSetOpt( ptzBCPwm2 );
    // need to enable pwm's again after call to setOpt
    PWMDRV_fnEn( ptzBCPwm1 );
	PWMDRV_fnEn( ptzBCPwm2 );
}

/*******************************************************************************

FUNCTION NAME: 
    TEST_fnLimitsAndAsserts
  
PURPOSE: 
    

INPUTS: 
    None

OUTPUTS:
    None

NOTES:
    None

VERSION HISTORY:

Version: 1.00  Date: DEC-21-2004  By: John Bellini
    - Created

*******************************************************************************/
void TEST_fnLimitsAndAsserts( void )
{
    PWMDRV_tzDATA *ScratchPwm = ( PWMDRV_tzDATA *)0;
    volatile tuiSTATUS Status;              //Used to peek at the return value.
    
    //The following tests cause asserts so we need to NOT loop for verification
    AssertTest = TRUE;
    
    //-------------------------------------------------------------------------    
    //Setup to call the ctor with a null pointer.
    //This verifies that the null pointer is caught (assert #1), the TYPE of
    //PWMing desired is INVALID (assert #2) and then verifies that you get a 
    //line of code that you should never get to (assuming asserts never return)
    //(assert #3).
    AssertCnt  = 0;
    ScratchPwm = ( void *)0;
    Status = PWMDRV_fnCtor( ScratchPwm );
    AssertTest = FALSE;
    assert( AssertCnt );
    AssertTest = TRUE;
    
    //Another null pointer verification
    AssertCnt  = 0;
    ScratchPwm = ( void *)0;
    PWMDRV_fnUpdateDutyCycle( ScratchPwm );
    AssertTest = FALSE;
    assert( AssertCnt );
    AssertTest = TRUE;
    
    //-------------------------------------------------------------------------
    //Test Dead Band setting limits:  
    //
    //Since this causes an assert, we setup so we don't stay in the assert,
    //but can still tell it happened.
    AssertCnt  = 0;
    tzBCPwmOptions.uiDesiredDbNs = PWMDRV_mMAX_DEADBAND_NS() + 1;	//Over limit dead band
    PWMDRV_fnSetDeadBand( ptzBCPwm1 );
    //Make sure we asserted by the count NOT being zero.
    AssertTest = FALSE;
    assert( AssertCnt );
    AssertTest = TRUE;
    
    AssertCnt  = 0;
    tzBCPwmOptions.uiDesiredDbNs = 65535;	//Way over limit dead band
    PWMDRV_fnSetDeadBand( ptzBCPwm1 );
    //Make sure we asserted.  If we did, AssertTest will be FALSE.
    AssertTest = FALSE;
    assert( AssertCnt );
    AssertTest = TRUE;

    AssertCnt  = 0;
    ScratchPwm = ( void *)0;
    Status = PWMDRV_fnSetOpt( ScratchPwm );
    AssertTest = FALSE;
    assert( AssertCnt );
    //Better not pass with a NULL pointer.
    if( Status != eSTATUS_ERR )
    {
        errvalue |= 0x0020;
    }
}

/*******************************************************************************

FUNCTION NAME: 
    TEST_fnConstantPwming
  
PURPOSE: 
    

INPUTS: 
    None

OUTPUTS:
    None

NOTES:
    None

VERSION HISTORY:

Version: 1.00  Date: DEC-21-2004  By: John Bellini
    - Created

*******************************************************************************/
void TEST_fnConstantPwming( void )
{
	uint16 Cnt;
	
	//This will have to be measured on the scope to verify its time.
	//The fn PWMDRV_fnSetDeadBand also ensures the dead band tmr is enabled
	tzBCPwmOptions.uiDesiredDbNs = PWMDRV_mMAX_DEADBAND_NS();	//Large dead band
	PWMDRV_fnSetDeadBand( ptzBCPwm1 );
	
	//Do this for 5 seconds
	for( Cnt = 0; Cnt < 5; Cnt++ )
	{
		ptzBCPwm1->uiDutyCycle = ( uint16 )(( EvaRegs.T1PR / 100 ) * ( 10 * Cnt + 50 ));
		ptzBCPwm2->uiDutyCycle = ( uint16 )(( EvaRegs.T1PR / 100 ) * ( 5 * Cnt + 50 ));
		ptzFanPwm->uiDutyCycle = ( uint16 )(( EvbRegs.T3PR / 100 ) * ( 10 * Cnt + 50 ));
	    PWMDRV_fnUpdateDutyCycle( ptzBCPwm1 );
	    PWMDRV_fnUpdateDutyCycle( ptzBCPwm2 );
	    PWMDRV_fnUpdateDutyCycle( ptzFanPwm );
	    TEST_fnDoPinToggle( TEST_PASS_SPEED );
	}
}

/*******************************************************************************

FUNCTION NAME: 
    TEST_fnVariablePwming
  
PURPOSE: 
    

INPUTS: 
    None

OUTPUTS:
    None

NOTES:
    None

VERSION HISTORY:

Version: 1.00  Date: DEC-21-2004  By: John Bellini
    - Created

*******************************************************************************/
void TEST_fnVariablePwming( void )
{
	uint16 Cnt;
	uint16 Cnt1;

	//This will have to be measured on the scope to verify its time.
	//The fn PWMDRV_fnSetDeadBand also ensures the dead band tmr is enabled
	tzBCPwmOptions.uiDesiredDbNs = 3000;	//"Normal" dead band
	PWMDRV_fnSetDeadBand( ptzBCPwm1 );
	
	for( Cnt1 = 0; Cnt1 < 4; Cnt1++ )
	{
		//Ramp up the PWM duty cycle
		//This takes about 400 - 500us
		for( Cnt = 0; Cnt < 40; Cnt++ )
		{
			ptzBCPwm1->uiDutyCycle = ( uint16 )(( EvaRegs.T1PR / 100 ) * ( Cnt + 50 ));
			ptzBCPwm2->uiDutyCycle = ( uint16 )(( EvaRegs.T1PR / 100 ) * ( Cnt + 50 ));
			ptzFanPwm->uiDutyCycle = ( uint16 )(( EvbRegs.T3PR / 100 ) * ( Cnt + 50 ));
		    PWMDRV_fnUpdateDutyCycle( ptzBCPwm1 );
		    PWMDRV_fnUpdateDutyCycle( ptzBCPwm2 );
		    PWMDRV_fnUpdateDutyCycle( ptzFanPwm );
		    //A bit of a delay between each count

		    PWM_mDELAY_US(62500);
		}
		
		//Ramp down the PWM duty cycle
		//This takes about 400 - 500us
		for( Cnt = 40; Cnt > 0; Cnt-- )
		{
			ptzBCPwm1->uiDutyCycle = ( uint16 )(( EvaRegs.T1PR / 100 ) * ( Cnt + 50 ));
			ptzBCPwm2->uiDutyCycle = ( uint16 )(( EvaRegs.T1PR / 100 ) * ( Cnt + 50 ));
			ptzFanPwm->uiDutyCycle = ( uint16 )(( EvbRegs.T3PR / 100 ) * ( Cnt + 50 ));
		    PWMDRV_fnUpdateDutyCycle( ptzBCPwm1 );
		    PWMDRV_fnUpdateDutyCycle( ptzBCPwm2 );
		    PWMDRV_fnUpdateDutyCycle( ptzFanPwm );

		    //A bit of a delay between each count
		    PWM_mDELAY_US(62500);
		}
		
		TEST_fnDoPinToggle( TEST_PASS_SPEED );
	}
}

/*******************************************************************************

FUNCTION NAME: 
    TEST_fnConstantPwmingDis
  
PURPOSE: 
    

INPUTS: 
    None

OUTPUTS:
    None

NOTES:
    None

VERSION HISTORY:

Version: 1.00  Date: DEC-21-2004  By: John Bellini
    - Created

*******************************************************************************/
void TEST_fnConstantPwmingDis( void )
{
	uint16 Cnt;
	tucBOOL Switch = FALSE;
		
	//This will have to be measured on the scope to verify its time.
	//The fn PWMDRV_fnSetDeadBand also ensures the dead band tmr is enabled
	tzBCPwmOptions.uiDesiredDbNs = 1000;	//Small dead band
	PWMDRV_fnSetDeadBand( ptzBCPwm1 );
	
	//Do this for 5 seconds
	for( Cnt = 0; Cnt < 5; Cnt++ )
	{
		if( Switch == FALSE )
		{
			//Disable 1, Enable 2
			PWMDRV_fnDis( ptzBCPwm1 );
			PWMDRV_fnEn(  ptzBCPwm2 );		
		}
		else
		{
			//Disable 2, Enable 1
			PWMDRV_fnDis( ptzBCPwm2 );
			PWMDRV_fnEn(  ptzBCPwm1 );		
		}
		
		ptzBCPwm1->uiDutyCycle = ( uint16 )(( EvaRegs.T1PR / 100 ) * ( 10 * Cnt + 50 ));
		ptzBCPwm2->uiDutyCycle = ( uint16 )(( EvaRegs.T1PR / 100 ) * ( 10 * Cnt + 50 ));
		ptzFanPwm->uiDutyCycle = ( uint16 )(( EvbRegs.T3PR / 100 ) * ( 10 * Cnt + 50 ));

	    PWMDRV_fnUpdateDutyCycle( ptzBCPwm1 );
	    PWMDRV_fnUpdateDutyCycle( ptzBCPwm2 );
	    PWMDRV_fnUpdateDutyCycle( ptzFanPwm );
	    TEST_fnDoPinToggle( TEST_PASS_SPEED );
	    Switch ^= TRUE;
	}
	
	//Make sure the PWM's are enabled again.
	PWMDRV_fnEn(  ptzBCPwm1 );
	PWMDRV_fnEn(  ptzBCPwm2 );
}

/*******************************************************************************

FUNCTION NAME: 
    SYS_fnAbort
  
PURPOSE: 
    

INPUTS: 
    None

OUTPUTS:
    None

NOTES:
    None

VERSION HISTORY:

Version: 1.00  Date: DEC-21-2004  By: John Bellini
    - Created

*******************************************************************************/
void SYS_fnAbort( char *Warning )
{
    //GP counter to see if we get here or not in case the loop is bypassed with
    //AssertTest == TRUE.
    ++AssertCnt;
    
    //If the test didn't intentially create an assert, then we stay here
    //to capture the problem
    if( AssertTest == FALSE )
    {
		while( TRUE )
		{
	    	TEST_fnDoPinToggle( (uint32)TEST_FAIL_SPEED );    
	    }
    }
}

/*******************************************************************************

FUNCTION NAME: 
    TEST_fnDoPinToggle
  
PURPOSE: 
    

INPUTS: 
    None

OUTPUTS:
    None

NOTES:
    None

VERSION HISTORY:

Version: 1.00  Date: DEC-21-2004  By: John Bellini
    - Created

*******************************************************************************/
void TEST_fnDoPinToggle( uint32 Speed )
{
	PWM_mDELAY_US(Speed);
    PWM_mPIN_TOGGLE();
}

void DIGCTRL_fnIsrPwm( void )
{
    EvaRegs.EVAIFRA.bit.T1PINT |= 1;
}

void SWBASE_fnIsrSw( void )
{
    EvaRegs.EVAIFRB.bit.T2PINT |= 1;
}

void FAN_fnIsrPwm( void )
{
    EvbRegs.EVBIFRA.bit.T3PINT |= 1;
}

void pwm_fnPhantomIsr( void )
{
	// halt the processor
     asm(" ESTOP0");
     for(;;);
}

void ACOL_fnSetPkOvrI( void ) { pwm_fnPhantomIsr(); }
void CANDRV_fnECAN0INTIsr( void ) { pwm_fnPhantomIsr(); }
void CANDRV_fnECAN1INTIsr( void ) { pwm_fnPhantomIsr(); }
void SCIDRV_fnRxIsr( void ) { pwm_fnPhantomIsr(); }
void SCIDRV_fnTxIsr( void ) { pwm_fnPhantomIsr(); }
void SPIDRV_fnRxIsr( void ) { pwm_fnPhantomIsr(); }
void SPI_fnTxIsr( void ) { pwm_fnPhantomIsr(); }
void ZEROX_fnISRL1( void ) { pwm_fnPhantomIsr(); }
void ZEROX_fnISRL2( void ) { pwm_fnPhantomIsr(); }
