/*=============================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME: eedrvmain.c 

PURPOSE:
    unit test code for eeprom driver

FUNCTION(S):

NOTES:
    Only run one error test case at a time.
    
    Change bytes to be written to the EEDRV between swtiching ERR tests.


CHANGE HISTORY:
$Log: eedrvmain.c $

    ***********************************************
    Revision: NovaPfrmB_TrevorM/4
    User: TrevorM     Date: 03/19/05  Time: 12:49AM
    Converted test to use multiple tasks with simultaneous use of the SPI 

    
   ***********************************************
   Revision: NovaPfrmB_TrevorM/3
   User: TrevorM     Date: 03/12/05  Time: 01:05AM
   modified to use SPI driver handle

    ***********************************************
    Revision: NovaPfrmB_TrevorM/2
    User: TrevorM     Date: 03/02/05  Time: 11:00PM
    updated for use with sysclk to calculate the SPI prescalar value

    

    ***********************************************
    Revision: NovaPfrmB_TrevorM/1
    User: TrevorM     Date: 03/02/05  Time: 09:46PM
    Modified to calculate SPI Baud prescalar at run time

    

    ***********************************************
    Revision: NovaPfrmB_JohnB/1
    User: JohnB     Date: 01/11/05  Time: 07:06PM
    Updated to coding standard. All associated files and
    test updated to accomodate coding standard update.
    
    

    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/1
    User: BaldeeshK     Date: 12/09/04  Time: 12:51AM
    - Initial add to Accurev for eedrv test files
    
    
    
==============================================================================*/


/*==============================================================================
                              Includes
==============================================================================*/
#include "eedrv.h"
#include "eedevice.h"
#include "spiclient.h"
#include "LIB_stddefs.h"
#include "LIB_stdtype.h"
#include "Device.h"         // TI device headers
#include "devicemacro.h"
#include "xassert.h"
#include "PIEVect.h"
#include "init.h"           // Public access to init functions
#include "sysclk.h"            // for system clock timing
#include "spidrv.h"
#include "task.h"
#include "includes.h"
#include "iosvc.h"
#include "iosvcdevice.h"
#include <string.h>         // for memset, memcpy, etc
#include <errno.h>          // driver level error codes

/*=============================================================================
                              Defines
=============================================================================*/
//This test code requires NDEBUG to NOT BE DEFINED so asserts can be used
//to track issues captured by the tests.
#ifdef NDEBUG
#undef NDEBUG
#endif

#define TEST_PASS_SPEED ( 500000 )    //Speed to blink Das LED if pass (1sec)
#define TEST_FAIL_SPEED ( 250000  )   //Speed to blink Das LED if fail (0.5sec)


#define TEST_mPIN_TOGGLE()   ( GpioDataRegs.GPFTOGGLE.bit.GPIOF14 = 1 )
#define EETEST_WAIT_TIME_US  ( 50000 )


// ---------------------------------------------------------------------------
// Delay microsecond macros
// ---------------------------------------------------------------------------
//
// DEVICE_mCPU_RATE is the # of nanoseconds per clock cycle, so for example:
//
// System when MIPS = 150MHz
//
// DEVICE_mCPU_RATE = 1 / 150MHz = 6.6667nS/clock cycle
//
// This value is then passed into the DSP28x_usDelay function for processing
#define DEVICE_mCPU_RATE   ( (long double)( ( 1 / (float) CTRL_CPU_MAXSPEED ) \
                              * (float)1.0E9 ) )
// This is a formula derived by TI, They convert the input value into
// nano-seconds using:
//
// ( long double ) A * 1000L
//
// Then they find the number of clock cycles required:
//
// ( long double ) A * 1000L / ( long double ) DEVICE_mCPU_RATE
//
// According the ASM function created by TI, there is 9/10 of a cycle of
// overhead for each loop and each run through the loop takes 5 cycles
//
// ( ( (long double)A * 1000L / (long double)DEVICE_mCPU_RATE ) - 9.0L ) / 5.0L
//
#define DEVICE_mDELAY_US(A)  DSP28x_usDelay( ( ( ( (long double) A * 1000.0L ) \
                                          / (long double) DEVICE_mCPU_RATE )   \
                                          - 9.0L ) / 5.0L )

//Delay using ns unit
#define DEVICE_mDELAY_NS(A)  DSP28x_usDelay( ( ( (long double) A  \
                                          / (long double) DEVICE_mCPU_RATE )   \
                                          - 9.0L ) / 5.0L )

// task definitions                                          
#define START_TASK_PRIO       ( 0 )
#define START_TASK_STK_SIZE   256
#define START_TASK_ID         START_TASK_PRIO

#define FLASHER_TASK_PRIO     ( 1 )
#define FLASHER_TASK_STK_SIZE 256
#define FLASHER_TASK_ID       FLASHER_TASK_PRIO

#define TEST_TASK_PRIO        ( 2 )
#define TEST_TASK_STK_SIZE    256
#define TEST_TASK_ID          TEST_TASK_PRIO

/*==============================================================================
                           Function Prototypes
==============================================================================*/
//Supporting functions for this test.
void TEST_fnDoPinToggle( uint32 Speed );
void TEST_fnEEDrv( void );
void TEST_fnLimitsAndAsserts( void );
void main_fnStartTask( void *pdata );
void main_fnFlasherTask( void *pdata );
void main_fnTestTask( void *pdata );
void SYS_fnAbort( char *Warning );
void SYS_fnResetWatchdog( void );

/*==============================================================================
                            File Variables
==============================================================================*/
//if FALSE and an assert happens it means weren't testing for that we new would
//cause an assert.  So FALSE a "real" assert wile TRUE would mean we are
//intentially causing an assert.
static tucBOOL AssertTest = FALSE;
static uint16  AssertCnt  = 0;

// EEPROM Handle
SpiDrvHandle hEeprom;

// task stacks
#pragma DATA_SECTION(StartTaskStk, ".stack")
OS_STK StartTaskStk[ START_TASK_STK_SIZE ] = {0};

#pragma DATA_SECTION(FlasherTaskStk, ".stack")
OS_STK FlasherTaskStk[ FLASHER_TASK_STK_SIZE ] = {0};

#pragma DATA_SECTION(TestTaskStk, ".stack")
OS_STK TestTaskStk[ TEST_TASK_STK_SIZE ] = {0};

static uint16 ReadBuffer[ EEDRV_PAGE_SIZE ];

// add all variables to the watch list
static uint16 StartAdxByte        = 0x4502;
static uint16 UpperHalfAdxByte    = 0x6873;
static uint16 UpperQuarterAdxByte = 0x402A;
static uint16 LastAdxByte         = 0x5A5A;
    
static uint16 MinPageAdx        = EEDRV_UPPER_QUARTER_STARTING_ADX + 30;

static uint16 ReadStartAdxByte        = 0xFFFF;
static uint16 ReadUpperHalfAdxByte    = 0xFFFF;
static uint16 ReadUpperQuarterAdxByte = 0xFFFF;
static uint16 ReadLastAdxByte         = 0xFFFF;
    
static uint16 StartAdxPage[]        = { 0x1410, 0x7424, 0x890B, 0xF6EF };
static uint16 UpperHalfAdxPage[]    = { 0xA32A, 0xA5A5 };
static uint16 UpperQuarterAdxPage[] = { 0x0020, 0x0400, 0x10F1 };
static uint16 LastAdxPage[]         = { 0xF2FE, 0x00FF };
static uint16 MinPage[]             = { 0xA04F };
    
static uint16 ReadStartAdxPage[ sizeof( StartAdxPage ) ];
static uint16 ReadUpperHalfAdxPage[ sizeof( UpperHalfAdxPage ) ];
static uint16 ReadUpperQuarterAdxPage[ sizeof( UpperQuarterAdxPage ) ];
static uint16 ReadLastAdxPage[ sizeof( LastAdxPage ) ];
static uint16 ReadMinPage[ sizeof( MinPage ) ];

static uint16 EeAdx;
static uint16 EmptyValue[ EEDRV_PAGE_SIZE ];


/*==============================================================================
                           Function Definitions
==============================================================================*/

// ISR stubs
void ZEROX_fnISRL1(void){}
void ZEROX_fnISRL2(void){}
void ACOL_fnSetPkOvrI(void){}
void DIGCTRL_fnIsrPwm(void){}
void SWBASE_fnIsrSw(void){}

// other stubs
void SYS_fnResetWatchdog(void){}

/*******************************************************************************

FUNCTION NAME: Main

PURPOSE:
   C root function call

INPUTS:
   none

OUTPUTS:
   none

NOTES:

VERSION HISTORY:

*******************************************************************************/

void main( void )
{
    // system initialization
    //
    INIT_fnSysInit( );

    //This must be called prior to OSInit();
    PIE_fnInitPieVectTable();

    // Disable and clear all CPU interrupts
    DEVICE_mGLOBAL_INT_DIS();
    DEVICE_mALL_INT_DIS();
    DEVICE_mALL_INT_FLAG_CLEAR();

    // Interrupts that are used in this example are re-mapped to
    // ISR functions found within this file.  
    EALLOW; // This is needed to write to EALLOW protected registers

    // set up the IO to blink a LED
    // must allow access to the EALLOW protected registers before we can do
    // anything useful.
    // set up pin 14 (XF_GPIO pin) to be an I/O pin
    GpioMuxRegs.GPFMUX.bit.XF_GPIOF14 = 0;
    // configure the direction of pin 14 (XF_GPIO pin) to be an output
    GpioMuxRegs.GPFDIR.bit.GPIOF14 = 1;

    // disable write access to the protected registers.
    EDIS;

    //Initialize the RTOS
    OSInit( );                                         

    // create the start task 
    (void)OSTaskCreateExt( main_fnStartTask,
                          ( void * )0,
                          &StartTaskStk[ 0 ],
                          START_TASK_PRIO,
                          START_TASK_ID,
                          &StartTaskStk[ START_TASK_STK_SIZE-1],
                          START_TASK_STK_SIZE,
                          ( void * )0,
                          OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR );
    
    //enable global interrupt
    DEVICE_mGLOBAL_INT_EN();
    
    // start the OS timer           
    CpuTimer2Regs.TCR.bit.TSS = 0;

    // Start multitasking               
    OSStart( );

}

/*******************************************************************************

FUNCTION NAME: main_fnStartTask

PURPOSE:
   Start all other application tasks.

INPUTS:
   void *pvData - task specific data sent when this task was created.
   
OUTPUTS:
   none
   
NOTES:
    
    
VERSION HISTORY:
 

Version: 1.00  Date: Mar 16, 2005  By: Trevor Monk
    - Created    
   
*******************************************************************************/
void main_fnStartTask( void *pdata )
{
    pdata = pdata;
    
    // create the SPI driver
    if( SPIDRV_fnCtor() != eSTATUS_OK )
    {
        assert( 0 );
    }


    // create the IOSVC
    IOSVC_fnCtor();
    
    // create the test task
    (void)OSTaskCreateExt( main_fnTestTask,
                          ( void * )0,
                          &TestTaskStk[ 0 ],
                          TEST_TASK_PRIO,
                          TEST_TASK_ID,
                          &TestTaskStk[ TEST_TASK_STK_SIZE-1],
                          TEST_TASK_STK_SIZE,
                          ( void * )0,
                          OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR );
    
    // create the flasher task
    (void)OSTaskCreateExt( main_fnFlasherTask,
                          ( void * )0,
                          &FlasherTaskStk[ 0 ],
                          FLASHER_TASK_PRIO,
                          FLASHER_TASK_ID,
                          &FlasherTaskStk[ FLASHER_TASK_STK_SIZE-1],
                          FLASHER_TASK_STK_SIZE,
                          ( void * )0,
                          OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR );
        
    // waste some time
    while( TRUE )
    {
        OSTimeDly( OS_TICKS_PER_SEC );
    }
}

/*******************************************************************************

FUNCTION NAME: main_fnFlasherTask

PURPOSE:
   Flash an LED using IOSVC

INPUTS:
   void *pvData - task specific data sent when this task was created.
   
OUTPUTS:
   none
   
NOTES:
    
    
VERSION HISTORY:
 

Version: 1.00  Date: Mar 16, 2005  By: Trevor Monk
    - Created    
   
*******************************************************************************/
void main_fnFlasherTask( void *pdata )
{
    pdata = pdata;

    while( TRUE )
    {
        //If we get here, then test is working.
        IOSVC_fnSet( IOSVCDEVICE_eLED_FAULT, IOSVC_ePIN_HIGH );
        OSTimeDly( 10 );
        IOSVC_fnSet( IOSVCDEVICE_eLED_FAULT, IOSVC_ePIN_LOW );
        OSTimeDly( 10 );
    }
}

/*******************************************************************************

FUNCTION NAME: main_fnTestTask

PURPOSE:
   Perform the EEPROM tests

INPUTS:
   void *pvData - task specific data sent when this task was created.
   
OUTPUTS:
   none
   
NOTES:
    
    
VERSION HISTORY:
 

Version: 1.00  Date: Mar 16, 2005  By: Trevor Monk
    - Created    
   
*******************************************************************************/
void main_fnTestTask( void *pdata )
{
    pdata = pdata;
    
    while( TRUE )
    {
        //The test for the driver.
        TEST_fnEEDrv();

        //Test the boundries and then some.
        TEST_fnLimitsAndAsserts();
    }
}

/*******************************************************************************

Function NAME: TEST_fnEEDrv

PURPOSE:
   EE general operation test cases

INPUTS:
   void *pvData - task specific data sent when this task was created.
   
OUTPUTS:
   None
   
NOTES: 
   
*******************************************************************************/
void TEST_fnEEDrv( void )
{
    memset( ReadStartAdxPage, EEDRV_EMPTY_VALUE, sizeof( StartAdxPage ) );
    memset( ReadUpperHalfAdxPage, EEDRV_EMPTY_VALUE, sizeof( UpperHalfAdxPage ) );
    memset( ReadUpperQuarterAdxPage, EEDRV_EMPTY_VALUE, sizeof( UpperQuarterAdxPage ) );
    memset( ReadLastAdxPage, EEDRV_EMPTY_VALUE, sizeof( LastAdxPage ) );
    memset( ReadBuffer, EEDRV_EMPTY_VALUE, sizeof( ReadBuffer ) );
    memset( EmptyValue, EEDRV_EMPTY_VALUE, sizeof( EmptyValue ) );

    if( EEDRV_fnCtor() != eSTATUS_OK )
    {
        assert( 0 );
    }

        
    if( EEDRV_fnIsBlank() != TRUE )
    {
        for( EeAdx = EEDRV_STARTING_ADX; 
             EeAdx < EEDRV_LAST_ADX; 
             EeAdx += EEDRV_PAGE_ADX_SPAN )
        {
            
            EEDRV_fnWritePage( EeAdx, 
                               EmptyValue,
                               EEDRV_PAGE_SIZE );
            DEVICE_mDELAY_US( EETEST_WAIT_TIME_US );
        
            if( EEDRV_fnIsReady() != TRUE )
            {
                assert( 0 );
            }
        }
    }
    if( EEDRV_fnIsBlank() != TRUE )
    {
        assert( 0 );
    }

// WRITE A BYTE ////////////////////////////////////////////////////////////////////////////////
    if( EEDRV_fnWriteChar( EEDRV_STARTING_ADX, StartAdxByte ) != eSTATUS_OK )
    {
        assert( 0 );
    }
    DEVICE_mDELAY_US( EETEST_WAIT_TIME_US );

    if( EEDRV_fnIsReady() != TRUE )
    {
        assert( 0 );
    }
    
    if( EEDRV_fnWriteChar( EEDRV_UPPER_HALF_STARTING_ADX, UpperHalfAdxByte ) != eSTATUS_OK )
    {
        assert( 0 );
    }
    DEVICE_mDELAY_US( EETEST_WAIT_TIME_US );

    if( EEDRV_fnIsReady() != TRUE )
    {
        assert( 0 );
    }
    
    if (EEDRV_fnWriteChar( EEDRV_UPPER_QUARTER_STARTING_ADX, UpperQuarterAdxByte ) != eSTATUS_OK )
    {
        assert( 0 );
    }
    DEVICE_mDELAY_US( EETEST_WAIT_TIME_US );

    if( EEDRV_fnIsReady() != TRUE )
    {
        assert( 0 );
    }
        
#if( CHAR_BIT == 8 )
    if( EEDRV_fnWriteChar( EEDRV_LAST_ADX, LastAdxByte ) != eSTATUS_OK )
    {
        assert( 0 );
    }
        
#elif( CHAR_BIT == 16 )
// must subtract 1 from the last adx since we can't write 16 bits to the last 8 bits
    if( EEDRV_fnWriteChar( EEDRV_LAST_ADX - 1, LastAdxByte ) != eSTATUS_OK )
    {
        assert( 0 );
    }
    DEVICE_mDELAY_US( EETEST_WAIT_TIME_US );

    if( EEDRV_fnIsReady() != TRUE )
    {
        assert( 0 );
    }

#endif        
        
// READ A BYTE ////////////////////////////////////////////////////////////////////////////////
// Place a break point at every comparison. Compare vars.

    EEDRV_fnReadChar( EEDRV_UPPER_HALF_STARTING_ADX, &ReadUpperHalfAdxByte );
    if( ReadUpperHalfAdxByte != UpperHalfAdxByte )
    {
        assert( 0 );
    }

#if( CHAR_BIT == 8 )
    EEDRV_fnReadChar( EEDRV_LAST_ADX, &ReadLastAdxByte );
    if( ReadLastAdxByte != LastAdxByte )
    {
        assert( 0 );
    }
#elif( CHAR_BIT == 16 )
    EEDRV_fnReadChar( EEDRV_LAST_ADX - 1, &ReadLastAdxByte );
    if( ReadLastAdxByte != LastAdxByte )
    {
        assert( 0 );
    }
#endif

    EEDRV_fnReadChar( EEDRV_UPPER_QUARTER_STARTING_ADX, &ReadUpperQuarterAdxByte );
    if( ReadUpperQuarterAdxByte != UpperQuarterAdxByte )
    {
        assert( 0 );
    }

    EEDRV_fnReadChar( EEDRV_STARTING_ADX, &ReadStartAdxByte );
    if( ReadStartAdxByte != StartAdxByte )
    {
        assert( 0 );
    }


// WRITE A PAGE and check ready function /////////////////////////////////////////////////////
    if( EEDRV_fnWritePage( MinPageAdx,          
                            (void * ) MinPage,       
                            sizeof( MinPage ) ) != eSTATUS_OK )
    {
        assert( 0 );
    }
    DEVICE_mDELAY_US( EETEST_WAIT_TIME_US );

    if( EEDRV_fnIsReady() != TRUE )
    {
        assert( 0 );
    }

    if( EEDRV_fnWritePage( EEDRV_STARTING_ADX,          
                            (void * ) StartAdxPage,       
                            sizeof( StartAdxPage ) ) != eSTATUS_OK )
    {
        assert( 0 );
    }

    DEVICE_mDELAY_US( EETEST_WAIT_TIME_US );

    if( EEDRV_fnIsReady() != TRUE )
    {
        assert( 0 );
    }

    if( EEDRV_fnWritePage( EEDRV_UPPER_HALF_STARTING_ADX,          
                            (void * ) UpperHalfAdxPage,       
                            sizeof( UpperHalfAdxPage ) ) != eSTATUS_OK )
    {
        assert( 0 );
    }

    DEVICE_mDELAY_US( EETEST_WAIT_TIME_US );

    if( EEDRV_fnIsReady() != TRUE )
    {
        assert( 0 );
    }

#if( CHAR_BIT == 8 )
    if( EEDRV_fnWritePage( EEDRV_LAST_ADX  - sizeof( LastAdxPage ),          
                            (void * ) LastAdxPage,       
                            sizeof( LastAdxPage ) ) != eSTATUS_OK )
    {
        assert( 0 );
    }
#elif( CHAR_BIT == 16 )        
    if( EEDRV_fnWritePage( EEDRV_LAST_ADX - 1 - sizeof( LastAdxPage ),          
                            (void * ) LastAdxPage,       
                            sizeof( LastAdxPage ) ) != eSTATUS_OK )
    {
        assert( 0 );
    }

#endif

    DEVICE_mDELAY_US( EETEST_WAIT_TIME_US );

    if( EEDRV_fnIsReady() != TRUE )
    {
        assert( 0 );
    }

    if( EEDRV_fnWritePage( EEDRV_UPPER_QUARTER_STARTING_ADX,          
                            (void * ) UpperQuarterAdxPage,       
                            sizeof( UpperQuarterAdxPage ) ) != eSTATUS_OK )
    {
        assert( 0 );
    }
    DEVICE_mDELAY_US( EETEST_WAIT_TIME_US );

    if( EEDRV_fnIsReady() != TRUE )
    {
        assert( 0 );
    }


// READ A PAGE /////////////////////////////////////////////////////////////////////////////////
// if the read value is not the same as the written value assert is called.

#if( CHAR_BIT == 8 )
    EEDRV_fnReadPage( EEDRV_LAST_ADX - sizeof( ReadLastAdxPage ),
                       (void *) ReadLastAdxPage,
                       sizeof( ReadLastAdxPage ) );
#elif( CHAR_BIT == 16 )                           
    EEDRV_fnReadPage( EEDRV_LAST_ADX - 1 - sizeof( ReadLastAdxPage ),
                       (void *) ReadLastAdxPage,
                       sizeof( ReadLastAdxPage ) );

#endif
    if( memcmp( ReadLastAdxPage, 
                LastAdxPage, 
                sizeof( ReadLastAdxPage ) ) != 0 )  
    {
        assert( 0 );
    }                        
    
    EEDRV_fnReadPage( EEDRV_UPPER_HALF_STARTING_ADX,
                       (void *) ReadUpperHalfAdxPage,
                       sizeof( ReadUpperHalfAdxPage ) );
    if( memcmp( ReadUpperHalfAdxPage,
                UpperHalfAdxPage, 
                sizeof( ReadUpperHalfAdxPage ) ) != 0 )  
    {
        assert( 0 );
    }                        
    
    EEDRV_fnReadPage( EEDRV_UPPER_QUARTER_STARTING_ADX,
                       (void *) ReadUpperQuarterAdxPage,
                       sizeof( ReadUpperQuarterAdxPage ) );
    if( memcmp( ReadUpperQuarterAdxPage, 
                UpperQuarterAdxPage, 
                sizeof( ReadUpperQuarterAdxPage ) ) != 0 )  
    {
        assert( 0 );
    }              

    EEDRV_fnReadPage( MinPageAdx,
                       (void *) ReadMinPage,
                       sizeof( ReadMinPage ) );
    if( memcmp( ReadMinPage, 
                MinPage, 
                sizeof( ReadMinPage ) ) != 0 )  
    {
        assert( 0 );
    }                        

    EEDRV_fnReadPage( EEDRV_STARTING_ADX,
                       (void *) ReadStartAdxPage,
                       sizeof( ReadStartAdxPage ) );
    if( memcmp( ReadStartAdxPage, 
                StartAdxPage, 
                sizeof( ReadStartAdxPage ) ) != 0 )  
    {
        assert( 0 );
    }                        

}


/*******************************************************************************

Function NAME:
    TEST_fnLimitsAndAsserts

PURPOSE:
    Test limits and handle assertions to test specific asserted items.

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    None

*******************************************************************************/
void TEST_fnLimitsAndAsserts( void )
{
    volatile uint16 Scratch;

    AssertTest = TRUE;

    // try and read beyond the end of the EEDRV.
    AssertCnt = 0;
    Scratch = EEDRV_fnReadChar( EEDRV_LAST_ADX + 1, 
                                &ReadLastAdxByte );
    assert( AssertCnt );

    // try and write beyond the end of the EEDRV        
    AssertCnt = 0;
    Scratch = EEDRV_fnWriteChar( EEDRV_LAST_ADX + 1, 
                                 0xFFFF ); 
    assert( AssertCnt );

    //test reading a page beyond the EE size        
    AssertCnt = 0;
    Scratch = EEDRV_fnReadPage( EEDRV_LAST_ADX + 1,
                                ( void *) ReadLastAdxPage,
                                sizeof( ReadLastAdxPage ) ); 
    assert( AssertCnt );

    // test writing a page beyond the EE size        
    AssertCnt = 0;
    Scratch = EEDRV_fnWritePage( EEDRV_LAST_ADX + 1,          
                                ( void * ) LastAdxPage,       
                                sizeof( LastAdxPage ) ); 
    assert( AssertCnt );

    // test reading across a page boudary        
    AssertCnt = 0;
    Scratch = EEDRV_fnReadPage( EEDRV_UPPER_HALF_STARTING_ADX - 1,
                                (void *) ReadUpperHalfAdxPage,
                                sizeof( ReadUpperHalfAdxPage ) ); 
    assert( AssertCnt );

    // test writing across a page boundary        
    AssertCnt = 0;
    Scratch = EEDRV_fnWritePage( EEDRV_UPPER_QUARTER_STARTING_ADX - 1,          
                                (void * ) UpperQuarterAdxPage,       
                                sizeof( UpperQuarterAdxPage ) ); 
    assert( AssertCnt );

    // test reading beyond the page size        
    AssertCnt = 0;
    Scratch = EEDRV_fnReadPage( EEDRV_UPPER_HALF_STARTING_ADX - 1,
                                (void *) ReadUpperHalfAdxPage,
                                EEDRV_PAGE_SIZE + 1 ); 
    assert( AssertCnt );

    // test writing beyond the page size        
    AssertCnt = 0;
    Scratch = EEDRV_fnWritePage( EEDRV_UPPER_QUARTER_STARTING_ADX,          
                                 (void * ) UpperQuarterAdxPage,       
                                 EEDRV_PAGE_SIZE + 1 ); 
    assert( AssertCnt );
    // test reading under the minimum page size        
    AssertCnt = 0;
    Scratch = EEDRV_fnReadPage( EEDRV_UPPER_HALF_STARTING_ADX - 1,
                                (void *) ReadUpperHalfAdxPage,
                                EEDRV_MIN_PAGE_SIZE - 1 ); 
    assert( AssertCnt );

    // test writing under the minimum page size        
    AssertCnt = 0;
    Scratch = EEDRV_fnWritePage( EEDRV_UPPER_QUARTER_STARTING_ADX,          
                                 (void * ) UpperQuarterAdxPage,       
                                 EEDRV_MIN_PAGE_SIZE - 1 ); 
    assert( AssertCnt );

    //dtor
    AssertCnt = 0;
    EEDRV_fnDtor();
    Scratch = EEDRV_fnReadPage( EEDRV_UPPER_HALF_STARTING_ADX - 1,
                                (void *) ReadUpperHalfAdxPage,
                                EEDRV_MIN_PAGE_SIZE - 1 );
    assert( AssertCnt );
     
}

/*******************************************************************************

Function NAME:
    SYS_fnAbort

PURPOSE:
    Debugging feature, checking point failed.

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    None

*******************************************************************************/
void SYS_fnAbort( char *Warning )
{
    Warning = Warning;
    
    //GP counter to see if we get here or not in case the loop is bypassed with
    //AssertTest == TRUE.
    ++AssertCnt;

    //If the test didn't intentially create an assert, then we stay here
    //to capture the problem
    if( AssertTest == FALSE )
    {
        while( TRUE )
        {
            TEST_mPIN_TOGGLE();
            OSTimeDly( 10 );
        }
    }
}

