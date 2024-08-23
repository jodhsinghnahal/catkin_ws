/*=============================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME: TmrdrvTest.c

PURPOSE:
    timer driver unit test 

FUNCTION(S):
    main
    fnInitTest          // initialize the ezDSP board for testing
    fnTestTimer         // test an EV timer
    fnTestTimerSync     // test EV timer sync functionality
    
NOTES:
    * errvalue should be zero at any normal condition
    errvalue    1 --- main initialization error
                2 --- fnCtor error
                3 --- fnEnInt error             
                4 --- fnInit error              
                5 --- fnGetStatus error
                6 --- fnGetStatus error, data not correct
                7 --- fnSetPeriod error
                8 --- fnSetCompare error
                9 --- fnStart error
                10 --- fnStop error
                11--- fnHold error
                12--- fnResume error
                13 --- fnDisInt error
                14 --- fnClrIntFlag error
                25 --- fnDtor error
                
CHANGE HISTORY:
$Log: TmrdrvTest.c $

    ***********************************************
    Revision: NovaPfrmB_Local_HorstM/4
    User: HorstM     Date: 03/01/05  Time: 05:13PM
    using latest isr arrangement

    

    ***********************************************
    Revision: NovaPfrmB_Local_HorstM/3
    User: HorstM     Date: 02/21/05  Time: 12:58AM
    changes to accomodate changes to timer driver (code review) and use rtos and
    new isr structure
    

    ***********************************************
    Revision: NovaPfrmB_Local_HorstM/2
    User: HorstM     Date: 02/17/05  Time: 07:33PM
    changed interrupt keyword to defined constant DEVICE_INT_FN, added a couple
    of calls to system init functions in ctor
    

    ***********************************************
    Revision: NovaPfrmB_Local_HorstM/1
    User: HorstM     Date: 02/15/05  Time: 12:21AM
    unit test now uses RTOS and isr.asm for interrupts; changes required to run
    test as OS task
    

    ***********************************************
    Revision: NovaPfrmB_HorstM/4
    User: HorstM     Date: 01/19/05  Time: 11:50PM
    modified to use updated timer driver
    

    ***********************************************
    Revision: NovaPfrmB_HorstM/3
    User: HorstM     Date: 01/05/05  Time: 12:55AM
    re-defined delay macros here
    

    ***********************************************
    Revision: NovaPfrmB_HorstM/2
    User: HorstM     Date: 12/08/04  Time: 08:46PM
    latest version to work with Accurev project structure
    

    ***********************************************
    Revision: NovaPfrmB_HorstM/1
    User: HorstM     Date: 12/07/04  Time: 09:37PM
    fixed header tags/comments;
    modified code so entire file can be compiled
    and linked/loaded into target without using com-
    pile switches; re-initialize timer objects
    before each use.
    
    
Date:  DEC-3-2004 - Horst Maurer
    -   Created
    
==============================================================================*/


/*==============================================================================
                              Includes
==============================================================================*/
#include "Tmrdrv.h"
#include "Device.h"
#include "devicemacro.h"
#include "config.h"
#include "init.h"                   //uint initial
#include "includes.h"
#include "uCOS_II.h"        // for OSTaskDel
#include "isr.h"        // isr declarations
#include "PieVect.h"    // for PIE vector table initialization
#include <errno.h>

/*==============================================================================
                              macros
==============================================================================*/
#define TEST_mPIN_TOGGLE()   ( GpioDataRegs.GPFTOGGLE.bit.GPIOF14 = 1 )

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
#define TEST_mCPU_RATE   ( (long double)( ( 1 / (float) CTRL_CPU_MAXSPEED ) \
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
#define TEST_mDELAY_US(A)  DSP28x_usDelay( ( ( ( (long double) A * 1000.0L ) \
                                          / (long double) TEST_mCPU_RATE )   \
                                          - 9.0L ) / 5.0L )

//Delay using ns unit
#define TEST_mDELAY_NS(A)  DSP28x_usDelay( ( ( (long double) A  \
                                          / (long double) TEST_mCPU_RATE )   \
                                          - 9.0L ) / 5.0L )

/*==============================================================================
                              defines
==============================================================================*/
// OS stuff
#define  TASK_STK_SIZE     256
#define  START_TASK_PRIO   11
#define  EEP_TASK_PRIO     10
#define  START_TASK_ID     START_TASK_PRIO

#define TEST_NO_ERR     ( 0 )
//LED flash counter limit
#define CNT_LIM             ( 10 )    
/*==============================================================================
                           Function Prototypes
==============================================================================*/
// test functions
void fnTestTask( void *pvData );
void fnInitTest( void );
void fnInitTimer( TMRDRV_tzTIMER* tmr );
tuiSTATUS fnTestTimer( uint16 timer, uint16 compMode, uint16 intType );
tuiSTATUS fnTestTimerSync( uint16 pair, uint16 synType );

// misc functions
void SYS_fnAbort( char *warning );

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
                            File Variables
==============================================================================*/
TMRDRV_tzTIMER tzTimer_1;
TMRDRV_tzTIMER tzTimer_2;
TMRDRV_tzTIMER tzTimer_3;
TMRDRV_tzTIMER tzTimer_4;
TMRDRV_tzTIMER tzTimerTemp;
uint16 errvalue = 0;
uint16 uiTimeDelay = 1000;
uint16 Counter = 0;
TMRDRV_tzPRD tzTimerPrd = { 0, 0 , 0};

#pragma DATA_SECTION( TaskStartStk, ".stack" );
OS_STK  TaskStartStk[ TASK_STK_SIZE ] = { 0 };
/*==============================================================================
                           Function Definitions
==============================================================================*/
/******************************************************************************\

FUNCTION NAME: main

PURPOSE:
    C root function call

INPUTS:
   none

OUTPUTS:
   none

NOTES:
    the following timer driver features can be tested with this tester:
    (X-->test included)
     x  counting mode
     x  syncronization
     x  interrupts
     x  compare
        compare reload
        compare output
        ADC
        clock

    following timer functions are tested with this module:
    X   TMRDRV_fnCtor               - create timer instances
    X   TMRDRV_fnDtor               - destroy timer instances
    X   TMRDRV_fnStart              - Start the timer
    X   TMRDRV_fnStop               - Stop the timer
    X   TMRDRV_fnHold               - Hold the timer
    X   TMRDRV_fnResume             - resume the timer from hold
    X   TMRDRV_fnSet                - Set timer control registers 
    X   TMRDRV_fnSetPrd          - Set a timer period value
    X   TMRDRV_fnSetPrdFast         - set timer specific period registers
    X   TMRDRV_fnSetCmp         - Update a timer compare value
        TMRDRV_fnSetDelay           - set timer compare value with delay time
    X   TMRDRV_fnGetStatus          - Returns the status of the timer.
    X   TMRDRV_fnGetPrd          - return timer period
    X   TMRDRV_fnEnInt              - enable a interrupt
    X   TMRDRV_fnDisInt             - disable a interrupt
        TMRDRV_fnDisIntAll          - disable all interrupts
        TMRDRV_fnClrIntFlag         - clear a interrupt flag
        TMRDRV_fnClrIntFlagAll      - clear all timer interrupt flag
        TMRDRV_fnReset       - reset timer registers
    X   TMRDRV_fnGetClkSpeed        - get the clock speed for this device

VERSION HISTORY:

Version 1.00    Date: DEC-3-2004     By: Horst Maurer
    - Created

******************************************************************************/
void main( void )
{
    // initialize the system
    fnInitTest();

    //Initialize the RTOS
    OSInit( );

    // Create the test task
    //
    OSTaskCreateExt( fnTestTask,
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
    fnTestTask
  
PURPOSE: 

INPUTS: 

OUTPUTS:

NOTES:

VERSION HISTORY:

Version 1.00    Date: DEC-3-2004     By: Horst Maurer
    - Created

*******************************************************************************/
void fnTestTask( void *pvData )
{
    tuiSTATUS tuiStatus = eSTATUS_OK;
    TMRDRV_teFN eTmr, eTmrErr;
    TMRDRV_teCOMP_MODE eCmp, eCmpErr;
    TMRDRV_teINT eInt, eIntErr;
    TMRDRV_teSYN eSyn, eSynErr;

    // compare mode should be 0 (no compare) unless interrupt type is 2 (CInt)

    /*
    // timer
    TMRDRV_ePWM,                 // PWM for H-bridge control
    TMRDRV_eSW,                  // sine wave reference
    TMRDRV_eFAN,                 // PWM for fan control
    TMRDRV_eCAP                  // capture driver

    // compare reload mode
    // (when compare register is loaded with value in shadow register)
    TMRDRV_eNO_COMP = 0,
    TMRDRV_eZERO_RELOAD,         //compare reload at counter==0
    TMRDRV_eEQUAL_RELOAD,        //compare reload at counter==O or period
    TMRDRV_eIMM_RELOAD           //compare reload immediatly

    // interrupt type
    TMRDRV_eNO_INT = 0,          //no interrupt
    TMRDRV_eINT_PRD = 1,             //period interrupt
    TMRDRV_eINT_COMPARE = 2,         //compare match interrupt
    TMRDRV_eINT_UNDERFLOW = 3,       //counter underflow interrupt
    TMRDRV_eINT_OVERFLOW = 4         //counter overflow interrupt
    */

    while( errvalue == 0 )
    {
        for( eTmr = TMRDRV_ePWM; eTmr <= TMRDRV_eCAP; eTmr++ )
        {
            // test the period interrupts
            eInt = TMRDRV_eINT_PRD;
            eCmp = TMRDRV_eNO_COMP;
            tuiStatus = fnTestTimer( eTmr, eCmp, eInt );
            if( tuiStatus != eSTATUS_OK ) break;

            // test the compare interrupts
            eInt = TMRDRV_eINT_COMPARE;
            // with all the different compare register reload conditions
            eCmp = TMRDRV_eIMM_RELOAD;
            tuiStatus = fnTestTimer( eTmr, eCmp, eInt );
            if( tuiStatus != eSTATUS_OK ) break;

            eCmp = TMRDRV_eZERO_RELOAD;
            tuiStatus = fnTestTimer( eTmr, eCmp, eInt );
            if( tuiStatus != eSTATUS_OK ) break;

            eCmp = TMRDRV_eEQUAL_RELOAD;
            tuiStatus = fnTestTimer( eTmr, eCmp, eInt );
            if( tuiStatus != eSTATUS_OK ) break;

            // test the underflow interrupts
            eInt = TMRDRV_eINT_UNDERFLOW;
            eCmp = TMRDRV_eNO_COMP;
            tuiStatus = fnTestTimer( eTmr, eCmp, eInt );
            if( tuiStatus != eSTATUS_OK ) break;

            // test the overflow interrupts
            eInt = TMRDRV_eINT_OVERFLOW;
            eCmp = TMRDRV_eNO_COMP;
            tuiStatus = fnTestTimer( eTmr, eCmp, eInt );
            if( tuiStatus != eSTATUS_OK ) break;

            // GEL script label
            _TopBreakHere:
            ;
        }

        if( tuiStatus != eSTATUS_OK )
        {
            // an error ocurred during the last timer test
            eTmrErr = eTmr;
            eCmpErr = eCmp;
            eIntErr = eInt;
            break;
        }

        // sync testing
        for( eSyn = TMRDRV_eSYN_PERIOD; eSyn <= TMRDRV_eSYN_PRD_ST; eSyn++ )
        {
            eTmr = TMRDRV_ePWM;
            tuiStatus = fnTestTimerSync( eTmr, eSyn );
            if( tuiStatus != eSTATUS_OK ) break;

            eTmr = TMRDRV_eFAN;
            tuiStatus = fnTestTimerSync( eTmr, eSyn );
            if( tuiStatus != eSTATUS_OK ) break;

            // GEL script label
            _TopBreakHere2:
            ;
        }

        if( tuiStatus != eSTATUS_OK )
        {
            // an error ocurred during the last timer sync test
            eTmrErr = eTmr;
            eSynErr = eSyn;
            break;
        }
    }

    // GEL script label
    _ErrBreakHere:
    ;
}

/*******************************************************************************

FUNCTION NAME: 
    fnInitTest
  
PURPOSE: 
    initializes the F28x exDSP system for testing the timer driver

INPUTS: 
    none

OUTPUTS:
    none

NOTES:

VERSION HISTORY:

Version 1.00    Date: DEC-3-2004     By: Horst Maurer
    - Created

*******************************************************************************/
void fnInitTest( void)
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

    // NOTE:
    // Enable of Interrupts at the CPU level happens after the timer is set up

}

/*******************************************************************************

FUNCTION NAME: 
    fnTestTimer
  
PURPOSE: 
    runs a series of tests on the specified timer

INPUTS: 
    timer number (1, 2, 3, or 4)

OUTPUTS:
    error code

NOTES:

VERSION HISTORY:

Version 1.00    Date: DEC-3-2004     By: Horst Maurer
    - Created

*******************************************************************************/
tuiSTATUS fnTestTimer( uint16 timer, uint16 compMode, uint16 intType )
{
    TMRDRV_tzTIMER *ptzTimer;
    tuiSTATUS tuiStatus = eSTATUS_OK;
    uint16 i;
    uint16 a, b;

    switch( timer )
    {
        case TMRDRV_ePWM:
            fnInitTimer( &tzTimer_1 );
            ptzTimer = &tzTimer_1;
            break;

        case TMRDRV_eSW:
            fnInitTimer( &tzTimer_2 );
            ptzTimer = &tzTimer_2;
            break;

        case TMRDRV_eFAN:
            fnInitTimer( &tzTimer_3 );
            ptzTimer = &tzTimer_3;
            break;

        case TMRDRV_eCAP:
            fnInitTimer( &tzTimer_4 );
            ptzTimer = &tzTimer_4;
            break;

        default:
            return( eSTATUS_ERR );
    }

    /* timer can be one of:
    TMRDRV_ePWM = 1              // PWM for H-bridge control
    TMRDRV_eSW,                  // sine wave reference
    TMRDRV_eFAN,                 // PWM for fan control
    TMRDRV_eCAP                  // capture driver */
    ptzTimer->teTimeFunc = ( TMRDRV_teFN )timer;

    /* interrupt mode can be one of:
    TMRDRV_eINT_PRD = 1,         //period interrupt
    TMRDRV_eINT_COMPARE,         //compare match interrupt
    TMRDRV_eINT_UNDERFLOW,       //counter underflow interrupt
    TMRDRV_eINT_OVERFLOW         //counter overflow interrupt */
    ptzTimer->teTimerInt = ( TMRDRV_teINT )intType;

    /* compare mode can be one of:
    TMRDRV_eNO_COMP = 0,
    TMRDRV_eZERO_RELOAD,         //compare reload at counter==0
    TMRDRV_eEQUAL_RELOAD,        //compare reload at counter==O or period
    TMRDRV_eIMM_RELOAD           //compare reload immediatly */
    ptzTimer->teTimerCompMode = ( TMRDRV_teCOMP_MODE )compMode;

    /* counting mode can be one of:
    TMRDRV_eSTOPHOLD = 0,        //counter hold
    TMRDRV_eCONTUPDOWN,          //continuous up and down
    TMRDRV_eCONTUP,              //continuous up
    TMRDRV_eDIRUPDOWN,           //directional up and down */
    ptzTimer->teTimerCountMode = TMRDRV_eCONTUP;

    tuiStatus = TMRDRV_fnCtor( ptzTimer );
    if( tuiStatus == eSTATUS_ERR )
    {
        errvalue = 2;
        return( tuiStatus );
    }
    
    ptzTimer->ulTimerFreq = 30;   // 30 Hz.
    ptzTimer->uiTimerDutyPct = 5000;   // 50%

    // set the period register of the timer
    tuiStatus = TMRDRV_fnSetPrd( ptzTimer );
    if( tuiStatus == eSTATUS_ERR )
    {
        errvalue = 7;
        return( tuiStatus );
    }

    // ************************ BENIGN TEST SECTION ****************************
    // *************************************************************************
    // If both TMRDRV_fnGetPrd and TMRDRV_fnSetPrdFast are working properly 
    // and the timer is properly defined (including the period and prescaler),
    // the net effect of the next two function calls will be nothing.
    // Here we just read the values from the timer, and then write them back:

    // another benign function which just loads the period and prescaler
    // of the timer into a temporary variable (tzTimerPrd)
    tuiStatus = TMRDRV_fnGetPrd( ptzTimer, &tzTimerPrd );
    if( tuiStatus == eSTATUS_ERR )
    {
        errvalue = 15;
        return( tuiStatus );
    }

    // directly calls the function that does a quick set of the timer period
    // and prescaler from the values just retrieved in the preceeding call
    // to TMRDRV_fnGetPrd.  
    if( TMRDRV_fnSetPrdFast( ptzTimer, &tzTimerPrd ) == eSTATUS_ERR )
    {
        errvalue = 20;
        return( tuiStatus );
    }

    //test TMRDRV_fnGetStatus function
    // this is a rather benign function that simply checks
    // the 'timer used' bit and returns false if it's not set
    // and sets the errno
    tuiStatus = TMRDRV_fnGetStatus( ptzTimer );
    if( tuiStatus == eSTATUS_ERR )
    {
        errvalue = 5;
        return( tuiStatus );
    }
    // *************************************************************************
    // ********************** END BENIGN TEST SECTION **************************

    // special settings for certain tests
    if( ptzTimer->teTimerInt == TMRDRV_eINT_COMPARE )
    {
        // set the timer compare register to (period * duty-cycle)
        tuiStatus = TMRDRV_fnSetCmp( ptzTimer );
        if( tuiStatus == eSTATUS_ERR )
        {
            errvalue = 8;
            return( tuiStatus );
        }
    }

    if( ptzTimer->teTimerInt == TMRDRV_eINT_OVERFLOW )
    {
        // set period register to 0xFFFF:
        // get current Period info
        tuiStatus = TMRDRV_fnGetPrd( ptzTimer, &tzTimerPrd );
        if( tuiStatus == eSTATUS_ERR )
        {
            errvalue = 15;
            return( tuiStatus );
        }

        tzTimerPrd.uiPeriod = 0xFFFF;
        // directly calls the function that does a quick set of the timer period
        // and prescaler from the values just retrieved in the preceeding call
        // to TMRDRV_fnGetPrd.  
        if( TMRDRV_fnSetPrdFast( ptzTimer, &tzTimerPrd ) == eSTATUS_ERR )
        {
            errvalue = 20;
            return( tuiStatus );
        }
    }

    //clear the timer counter and initialize it's control registers
    tuiStatus = TMRDRV_fnSet( ptzTimer );
    if( tuiStatus == eSTATUS_ERR )
    {
        errvalue = 4;
        return( tuiStatus );
    }

    // start the timer!
    tuiStatus = TMRDRV_fnStart( ptzTimer );
    if( tuiStatus == eSTATUS_ERR )
    {
        errvalue = 9;
        return( tuiStatus );
    }

    // enable this particular type of interrupt
    tuiStatus = TMRDRV_fnEnInt( ptzTimer );
    if( tuiStatus == eSTATUS_ERR )
    {
        errvalue = 3;
        return( tuiStatus );
    }
    
    //enable global interrupt
    EINT;

    // go through some cycles of stopping and restarting the timer
    // so we can see the LED go from blinking to static to blinking a few times
    for( i = 0; i < 1; i++ )
    {
        // BLINK THE LED FOR TWO SECONDS
        
        // wait a while to allow the timer to run and interrupts to occur
        // so we can see the LED blinkin'
        for( a = 0 ; a < 2 ; a++ )
        {
            for( b = 0; b < 1000; b++ )
            {
                TEST_mDELAY_US( 1000 );     // one milli-second delay
            }
        }

        // stop the timer but don't clear the count
        // only here to detect problem with this function
        tuiStatus = TMRDRV_fnHold( ptzTimer );
        if( tuiStatus == eSTATUS_ERR )
        {
            errvalue = 11;
            return( tuiStatus );
        }

        // STOP BLINKING THE LED FOR ONE SECOND
        
        // wait a while so we can see the LED stop blinking briefly 
        // before it resumes
        for( a = 0 ; a < 1 ; a++ )
        {
            for( b = 0; b < 1000; b++ )
            {
                TEST_mDELAY_US( 1000 );     // one milli-second delay
            }
        }

        // opposite of TMRDRV_fnHold; just re-enables the timer
        tuiStatus = TMRDRV_fnResume( ptzTimer );
        if( tuiStatus == eSTATUS_ERR )
        {
            errvalue = 12;
            return( tuiStatus );
        }

        // BLINK THE LED FOR TWO MORE SECONDS
        
        // wait a while to allow the timer to run and interrupts to occur
        // so we can see the LED blinkin'
        for( a = 0 ; a < 2 ; a++ )
        {
            for( b = 0; b < 1000; b++ )
            {
                TEST_mDELAY_US( 1000 );     // one milli-second delay
            }
        }

        // stop the timer but don't clear the count
        // only here to detect problem with this function
        tuiStatus = TMRDRV_fnHold( ptzTimer );
        if( tuiStatus == eSTATUS_ERR )
        {
            errvalue = 11;
            return( tuiStatus );
        }

        // STOP BLINKING THE LED FOR ONE SECOND
        
        // wait a while so we can see the LED stop blinking briefly 
        // before it resumes
        for( a = 0 ; a < 1 ; a++ )
        {
            for( b = 0; b < 1000; b++ )
            {
                TEST_mDELAY_US( 1000 );     // one milli-second delay
            }
        }

        // opposite of TMRDRV_fnHold; just re-enables the timer
        tuiStatus = TMRDRV_fnResume( ptzTimer );
        if( tuiStatus == eSTATUS_ERR )
        {
            errvalue = 12;
            return( tuiStatus );
        }

        // BLINK THE LED FOR TWO MORE SECONDS
        
        // wait a while to allow the timer to run and interrupts to occur
        // so we can see the LED blinkin'
        for( a = 0 ; a < 2 ; a++ )
        {
            for( b = 0; b < 1000; b++ )
            {
                TEST_mDELAY_US( 1000 );     // one milli-second delay
            }
        }

        // stop the timer but don't clear the count
        // pause to see break between tests
        tuiStatus = TMRDRV_fnHold( ptzTimer );
        if( tuiStatus == eSTATUS_ERR )
        {
            errvalue = 11;
            return( tuiStatus );
        }

        // wait a while so we can see the LED stop blinking briefly 
        // before it resumes
        for( a = 0 ; a < 2 ; a++ )
        {
            for( b = 0; b < 1000; b++ )
            {
                TEST_mDELAY_US( 1000 );     // one milli-second delay
            }
        }
    }

    // disable global interrupts
    DINT;

    //disable this timer's (current) interrupt type
    tuiStatus = TMRDRV_fnDisInt( ptzTimer );
    if( tuiStatus == eSTATUS_ERR )
    {
        errvalue = 13;
        return( tuiStatus );
    }

    // stop the timer (via call to TMRDRV_fnHold), and 
    // clear the timer count register
    tuiStatus = TMRDRV_fnStop( ptzTimer );
    if( tuiStatus == eSTATUS_ERR )
    {
        errvalue = 10;
        return( tuiStatus );
    }

    // GEL script label
    _TimerBreakHere:

    // destroy this timer so it can be recycled for the next test
    tuiStatus = TMRDRV_fnDtor( ptzTimer );
    if( tuiStatus == eSTATUS_ERR )
    {
        errvalue = 25;
        return( tuiStatus );
    }
    
    return( tuiStatus );

    /*

    // this just sets the timer compare register (previously set with
    // TMRDRV_fnSetCmp) to the number specified by uiTimeDelay (1000)
    tuiStatus = TMRDRV_fnSetDelay( ptzTimer, uiTimeDelay );
    if( tuiStatus == eSTATUS_ERR )
    {
        errvalue = 16;
        return( tuiStatus );
    }*/
}

/*******************************************************************************

FUNCTION NAME: 
    fnTestTimerSync
  
PURPOSE: 
    runs sync tests on the specified timer pair

INPUTS: 
    timer master (1 = 1 & 3; 2 = 2 & 4)

OUTPUTS:
    error code

NOTES:

VERSION HISTORY:

Version 1.00    Date: DEC-3-2004     By: Horst Maurer
    - Created

*******************************************************************************/
tuiSTATUS fnTestTimerSync( uint16 master, uint16 synType )
{
    TMRDRV_tzTIMER *ptzTimerMaster;
    TMRDRV_tzTIMER *ptzTimerSlave;
    tuiSTATUS tuiStatus = eSTATUS_OK; 
    uint16 i;
    uint16 a, b;

    switch( master )
    {
        case 1:
            fnInitTimer( &tzTimer_1 );
            fnInitTimer( &tzTimer_2 );
            ptzTimerMaster = &tzTimer_1;
            ptzTimerSlave = &tzTimer_2;
            break;

        case 3:
            fnInitTimer( &tzTimer_3 );
            fnInitTimer( &tzTimer_4 );
            ptzTimerMaster = &tzTimer_3;
            ptzTimerSlave = &tzTimer_4;
            break;

        default:
            return( eSTATUS_ERR );
    }

    /* sync mode can be one of:
    TMRDRV_eNO_SYN = 0,          //no synchronize
    TMRDRV_eSYN_PERIOD,          //synchronize prd
    TMRDRV_eSYN_START,           //synchronize starting
    TMRDRV_eSYN_PRD_ST           //synchronixe period and starting */
    ptzTimerSlave->teTimerSyn = ( TMRDRV_teSYN )synType;

    // timer function
    ptzTimerMaster->teTimeFunc = ( TMRDRV_teFN )master;
    ptzTimerSlave->teTimeFunc = ( TMRDRV_teFN )( master + 1 );

    // counting mode
    ptzTimerMaster->teTimerCountMode = TMRDRV_eCONTUP;    

    // create the two timers
    tuiStatus = TMRDRV_fnCtor( ptzTimerMaster );
    if( tuiStatus == eSTATUS_ERR )
    {
        errvalue = 2;
        return( tuiStatus );
    }
    tuiStatus = TMRDRV_fnCtor( ptzTimerSlave );
    if( tuiStatus == eSTATUS_ERR )
    {
        errvalue = 2;
        return( tuiStatus );
    }

    // set the period & duty cycle
    ptzTimerMaster->ulTimerFreq = 30;
    ptzTimerMaster->uiTimerDutyPct = 5000;
    ptzTimerSlave->ulTimerFreq = 20;
    ptzTimerSlave->uiTimerDutyPct = 5000;
    tuiStatus = TMRDRV_fnSetPrd( ptzTimerMaster );
    if( tuiStatus == eSTATUS_ERR )
    {
        errvalue = 7;
        return( tuiStatus );
    }

    //define syncronized timer
    ptzTimerSlave->uiTimerIDSyn = ptzTimerMaster->ucTimerID;

    // compare mode
    ptzTimerSlave->teTimerCompMode = TMRDRV_eZERO_RELOAD;

    // enable interrupts in the slave timer
    // since we are testing syncronization, we want to make sure
    // that the slave timer is syncing to the master by verifying
    // that interrupts occur in the slave due to 'events' in the master
    // enable compare interrupts in slave timer
    ptzTimerSlave->teTimerInt = TMRDRV_eINT_COMPARE;
    tuiStatus = TMRDRV_fnEnInt( ptzTimerSlave );
    // enable period interrupts in slave timer
    ptzTimerSlave->teTimerInt = TMRDRV_eINT_PRD;
    tuiStatus = TMRDRV_fnEnInt( ptzTimerSlave );
    // enable global interrupts
    EINT;

    //clear the timer counter and initialize it's control registers
    tuiStatus = TMRDRV_fnSet( ptzTimerMaster );
    if( tuiStatus == eSTATUS_ERR )
    {
        errvalue = 4;
        return( tuiStatus );
    }
    tuiStatus = TMRDRV_fnSet( ptzTimerSlave );
    if( tuiStatus == eSTATUS_ERR )
    {
        errvalue = 4;
        return( tuiStatus );
    }

    //start the timers
    tuiStatus = TMRDRV_fnStart( ptzTimerSlave );
    if( tuiStatus == eSTATUS_ERR )
    {
        errvalue = 9;
        return( tuiStatus );
    }
    tuiStatus = TMRDRV_fnStart( ptzTimerMaster );
    if( tuiStatus == eSTATUS_ERR )
    {
        errvalue = 9;
        return( tuiStatus );
    }

    // Main Loop, turn on and off timer periodically
    for( i = 0; i < 1; i++ )
    {
        // BLINK THE LED FOR TWO SECONDS
        
        // wait a while to allow the timer to run and interrupts to occur
        // so we can see the LED blinkin'
        for( a = 0 ; a < 2 ; a++ )
        {
            for( b = 0; b < 1000; b++ )
            {
                TEST_mDELAY_US( 1000 );     // one milli-second delay
            }
        }

        // stop the master timer but don't clear the count
        tuiStatus = TMRDRV_fnHold( ptzTimerMaster );
        if( tuiStatus == eSTATUS_ERR )
        {
            errvalue = 11;
            return( tuiStatus );
        }

        // STOP BLINKING THE LED FOR ONE SECOND
        
        // wait a while so we can see the LED stop blinking briefly 
        // before it resumes
        for( a = 0 ; a < 1 ; a++ )
        {
            for( b = 0; b < 1000; b++ )
            {
                TEST_mDELAY_US( 1000 );     // one milli-second delay
            }
        }

        // opposite of TMRDRV_fnHold; just re-enables the timer
        tuiStatus = TMRDRV_fnResume( ptzTimerMaster );
        if( tuiStatus == eSTATUS_ERR )
        {
            errvalue = 12;
            return( tuiStatus );
        }

        // BLINK THE LED FOR TWO MORE SECONDS
        
        // wait a while to allow the timer to run and interrupts to occur
        // so we can see the LED blinkin'
        for( a = 0 ; a < 2 ; a++ )
        {
            for( b = 0; b < 1000; b++ )
            {
                TEST_mDELAY_US( 1000 );     // one milli-second delay
            }
        }

        // stop the timer but don't clear the count
        // only here to detect problem with this function
        tuiStatus = TMRDRV_fnHold( ptzTimerMaster );
        if( tuiStatus == eSTATUS_ERR )
        {
            errvalue = 11;
            return( tuiStatus );
        }

        // STOP BLINKING THE LED FOR ONE SECOND
        
        // wait a while so we can see the LED stop blinking briefly 
        // before it resumes
        for( a = 0 ; a < 1 ; a++ )
        {
            for( b = 0; b < 1000; b++ )
            {
                TEST_mDELAY_US( 1000 );     // one milli-second delay
            }
        }

        // opposite of TMRDRV_fnHold; just re-enables the timer
        tuiStatus = TMRDRV_fnResume( ptzTimerMaster );
        if( tuiStatus == eSTATUS_ERR )
        {
            errvalue = 12;
            return( tuiStatus );
        }

        // BLINK THE LED FOR TWO MORE SECONDS
        
        // wait a while to allow the timer to run and interrupts to occur
        // so we can see the LED blinkin'
        for( a = 0 ; a < 2 ; a++ )
        {
            for( b = 0; b < 1000; b++ )
            {
                TEST_mDELAY_US( 1000 );     // one milli-second delay
            }
        }

        // stop the timer but don't clear the count
        // pause to see break between tests
        tuiStatus = TMRDRV_fnHold( ptzTimerMaster );
        if( tuiStatus == eSTATUS_ERR )
        {
            errvalue = 11;
            return( tuiStatus );
        }

        // wait a while so we can see the LED stop blinking briefly 
        // before it resumes
        for( a = 0 ; a < 2 ; a++ )
        {
            for( b = 0; b < 1000; b++ )
            {
                TEST_mDELAY_US( 1000 );     // one milli-second delay
            }
        }
    }

    // disable global interrupts
    DINT;

    //disable this timer's (current) interrupt type
    tuiStatus = TMRDRV_fnDisInt( ptzTimerSlave );
    if( tuiStatus == eSTATUS_ERR )
    {
        errvalue = 13;
        return( tuiStatus );
    }

    // stop the timer (via call to TMRDRV_fnHold), and 
    // clear the timer count register
    tuiStatus = TMRDRV_fnStop( ptzTimerMaster );
    if( tuiStatus == eSTATUS_ERR )
    {
        errvalue = 10;
        return( tuiStatus );
    }

    // stop the timer (via call to TMRDRV_fnHold), and 
    // clear the timer count register
    tuiStatus = TMRDRV_fnStop( ptzTimerSlave );
    if( tuiStatus == eSTATUS_ERR )
    {
        errvalue = 10;
        return( tuiStatus );
    }

    // destroy this timer so it can be recycled for the next test
    tuiStatus = TMRDRV_fnDtor( ptzTimerMaster );
    if( tuiStatus == eSTATUS_ERR )
    {
        errvalue = 25;
        return( tuiStatus );
    }
    
    // destroy this timer so it can be recycled for the next test
    tuiStatus = TMRDRV_fnDtor( ptzTimerSlave );
    if( tuiStatus == eSTATUS_ERR )
    {
        errvalue = 25;
        return( tuiStatus );
    }
    
    // GEL script label
    _SyncBreakHere:

    return( tuiStatus );
}

/*******************************************************************************

FUNCTION NAME: 
    fnInitTimer
  
PURPOSE: 
    initialize a timer to the defaults

INPUTS: 
    pointer to timer struct

OUTPUTS:
    none

NOTES:

VERSION HISTORY:

Version: 1.00  Date: DEC-6-2004  By: Horst Maurer
    - create

*******************************************************************************/
void fnInitTimer( TMRDRV_tzTIMER* tmr )
{
    tmr->ucTimerID = TMRDRV_ID_DFLT;
    tmr->ulTimerFreq = TMRDRV_PRD_DFLT;
    tmr->uiTimerDutyPct = TMRDRV_DUTY_DFLT;
    tmr->teTimerInt = TMRDRV_NO_INT;
    tmr->teTimerCountMode = TMRDRV_MODE_DFLT;
    tmr->teTimerAdcMode = TMRDRV_ADC_DFLT;
    tmr->teTimerCompOutput = TMRDRV_OUTPUT_DFLT;
    tmr->teTimerEmu = TMRDRV_EMU_DFLT;
    tmr->teTimerCompMode = TMRDRV_COMP_DFLT;
    tmr->teTimerSyn = TMRDRV_SYN_DFLT;
    tmr->uiTimerIDSyn = TMRDRV_ID_DFLT;
    tmr->teTimerClock = TMRDRV_CLOCK_DFLT;
    tmr->teTimeFunc = TMRDRV_FN_DFLT;
    tmr->uiTimerInterrupt = TMRDRV_INTERUPT_DFLT;
}

/*******************************************************************************

FUNCTION NAME: 
    SYS_fnAbort
  
PURPOSE: 
    replace function to avoid include xassert in this unit test

INPUTS: 
    char *

OUTPUTS:
    none

NOTES:
    forever loop

VERSION HISTORY:

Version: 0.10  Date: Oct-31-2002  By: Yingran Duan
    - create
Version: 1.00  Date: Aug. 18, 2003  By: Yingran Duan
    - standarize the function header

*******************************************************************************/
void SYS_fnAbort( char *warning )
{
    while( TRUE )
    {
        //TEST_mDELAY_US( TEST_FAIL_SPEED );
        //TEST_fnDoPinToggle( TEST_FAIL_SPEED );
    }
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

void TMRDRV_fnT1Cint( void )
{
    EvaRegs.EVAIFRA.bit.T1CINT |= 1;
}

void TMRDRV_fnT1UFint( void )
{
    EvaRegs.EVAIFRA.bit.T1UFINT |= 1;
}

void TMRDRV_fnT1OFint( void )
{
    EvaRegs.EVAIFRA.bit.T1OFINT |= 1;
}

void TMRDRV_fnT2Cint( void )
{
    EvaRegs.EVAIFRB.bit.T2CINT |= 1;
}

void TMRDRV_fnT2UFint( void )
{
    EvaRegs.EVAIFRB.bit.T2UFINT |= 1;
}

void TMRDRV_fnT2OFint( void )
{
    EvaRegs.EVAIFRB.bit.T2OFINT |= 1;
}

void TMRDRV_fnT3Cint( void )
{
    EvbRegs.EVBIFRA.bit.T3CINT |= 1;
}

void TMRDRV_fnT3UFint( void )
{
    EvbRegs.EVBIFRA.bit.T3UFINT |= 1;
}

void TMRDRV_fnT3OFint( void )
{
    EvbRegs.EVBIFRA.bit.T3OFINT |= 1;
}

void TMRDRV_fnT4Pint( void )
{
    EvbRegs.EVBIFRB.bit.T4PINT |= 1;
}

void TMRDRV_fnT4Cint( void )
{
    EvbRegs.EVBIFRB.bit.T4CINT |= 1;
}

void TMRDRV_fnT4UFint( void )
{
    EvbRegs.EVBIFRB.bit.T4UFINT |= 1;
}

void TMRDRV_fnT4OFint( void )
{
    EvbRegs.EVBIFRB.bit.T4OFINT |= 1;
}

void tmr_fnPhantomIsr( void )
{
	// halt the processor
     asm(" ESTOP0");
     for(;;);
}

void ACOL_fnSetPkOvrI( void ) { tmr_fnPhantomIsr(); }
void CANDRV_fnECAN0INTIsr( void ) { tmr_fnPhantomIsr(); }
void CANDRV_fnECAN1INTIsr( void ) { tmr_fnPhantomIsr(); }
void SCIDRV_fnRxIsr( void ) { tmr_fnPhantomIsr(); }
void SCIDRV_fnTxIsr( void ) { tmr_fnPhantomIsr(); }
void SPIDRV_fnRxIsr( void ) { tmr_fnPhantomIsr(); }
void SPI_fnTxIsr( void ) { tmr_fnPhantomIsr(); }
void ZEROX_fnISRL1( void ) { tmr_fnPhantomIsr(); }
void ZEROX_fnISRL2( void ) { tmr_fnPhantomIsr(); }
