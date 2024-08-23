/*==============================================================================
Copyright 2005 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:
    CapDrvTest.c

PURPOSE:
    To test the capture driver.

FUNCTION(S):
    TEST_fnInitGPIO - Initialize GPIO specifics as required by the test cases.
    TEST_fnDoPinToggle - Toggles a pin
    SYS_fnAbort        - handling on assert
    CAPDRV_fnIsr1      - Interrup Service Routine for capture 4
    CAPDRV_fnIsr2      - Interrup Service Routine for capture 5
    TestInitial        - Initialization

    local:
        -

NOTES:

    HOW TO USE THIS
    ---------------
        This test is run on eZdsp board. 
        If the test is pass, led on the eZdsp will flash at rate about 1Hz.
        If the test is failed, the Led will flash at about 2Hz.
        If the test is failed with a timeout error, the Led will flash at about 4Hz.

    I ran this test using a eZdsp F2812 sim board and using a function generator to 
    generate 60Hz pulse signal as capture input. Just set breakpoints in interrupt 
    service routine CAPDRV_fnIsr1() and CAPDRV_fnIsr2() and put all the vars in the
    watch window. There is a GEL file for this project. If you load GEL file, select menu
    item GEL-> CapDrv Unit Test -> Run, the project will be recompiled and vars will be 
    loaded to watch window automatically.

CHANGE HISTORY :
$Log: CapDrvTest.c $

    ***********************************************
    Revision: NovaPfrmB_HongY/4
    User: HongY     Date: 02/01/05  Time: 09:26PM
    updated because of the change of database structure
    
    

    ***********************************************
    Revision: NovaPfrmB_HongY/3
    User: HongY     Date: 01/20/05  Time: 09:13PM
    Changed according to code review and code standard
    
    

    ***********************************************
    Revision: NovaPfrmB_HongY/2
    User: HongY     Date: 12/08/04  Time: 05:15PM
    return count to zero to avoid beyond the boundary of the arrays
    

    ***********************************************
    Revision: NovaPfrmB_HongY/1
    User: HongY     Date: 12/07/04  Time: 11:30PM
    improved the structure of the test file
    
 
    *****************  Version 1  *****************
    User: Hongy        Date: 6/12/04    Time: 1:48p
    Created in $/PD/Common/Targets/TI/c28x/HAL/Test/CAPDRV
    
    *****************  Version 1  *****************
    User: Hongy        Date: 6/12/04    Time: 1:47p
    Created in $/PD/Common/Targets/TI/c28x/HAL/Test/TEST/CapDrv
    
    *****************  Version 1  *****************
    User: Hongy        Date: 6/12/04    Time: 1:44p
    Created in $/PD/Common/Targets/TI/c28x/HAL/Test/CapDrv/CAPDRV
    
==============================================================================*/


/*==============================================================================
                              Includes
==============================================================================*/
#include "LIB_stddefs.h"
#include "LIB_stdtype.h"
#include "Device.h"         // TI device headers
#include "devicemacro.h"
#include "xassert.h"
#include "init.h"           // Public access to init functions
#include <errno.h>
#include "Capdrv.h"
#include "Tmrdrv.h"

/*==============================================================================
                              Defines
==============================================================================*/
//This test code requires NDEBUG to NOT BE DEFINED so asserts can be used
//to track issues captured by the tests.
#ifdef NDEBUG
#undef NDEBUG
#endif

#define TEST_PASS_SPEED        ( 500000L )    //Speed to blink Das LED if pass (1sec)
#define TEST_FAIL_SPEED        ( 250000L  )   //Speed to blink Das LED if fail (0.5sec)
#define TEST_TIMEOUT_ERR_SPEED ( 125000L  )   //Speed to blink Das LED if timeout error (0.25sec)


#define TEST_mPIN_TOGGLE()  ( GpioDataRegs.GPFTOGGLE.bit.GPIOF14 = 1 )

#define mSTART_ASSERT_LOG()   AssertTest = TRUE; AssertCnt = 0;
#define mSTOP_ASSERT_LOG()    AssertTest = FALSE;
#define mVERIFY_ASSERTION()   assert( AssertCnt );

#define TEST_CAP (2)   //define which capture you want to test. 1: cap4; 2:cap5

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
                           Local/Private Variables
==============================================================================*/
//if FALSE and an assert happens it means weren't testing for that we new would
//cause an assert.  So FALSE a "real" assert wile TRUE would mean we are
//intentially causing an assert.
static tucBOOL AssertTest = FALSE;
static uint16  AssertCnt  = 0;
static CAPDRV_tzOBJ tzZeroX1Cap;
static CAPDRV_tzOBJ tzZeroX2Cap;

#define TEST_TIMES 16
static uint32 uiArray[TEST_TIMES]={0};
static uint8 uiFifostatus[TEST_TIMES]={0};
static uint32 uiArray2[TEST_TIMES]={0};
static uint8 uiFifostatus2[TEST_TIMES]={0};

/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/
//Supporting functions for this test.
void TEST_fnInitGPIO(void);
void TEST_fnDoPinToggle( uint32 uiTime );
interrupt void CAPDRV_fnIsr1( void );
interrupt void CAPDRV_fnIsr2( void );
tuiSTATUS TestInitial(void);
void SYS_fnAbort( char *Warning );

/*==============================================================================
                            File Variables
==============================================================================*/

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

*******************************************************************************/
void main( void )
{
    //¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
    //Test and Device Setup
    //¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
   
    // system initialization
    INIT_fnSysInit( );
    // Disable and clear all CPU interrupts
    DEVICE_mGLOBAL_INT_DIS();
    DEVICE_mALL_INT_DIS();
    DEVICE_mALL_INT_FLAG_CLEAR();

    //initial GPIO F14 for LED blink
    TEST_fnInitGPIO();

    // Enable global interrupts
    DEVICE_mGLOBAL_INT_EN(); 
    
    // Interrupts that are used in this example are re-mapped to
    // ISR functions found within this file.  
    // ensure the CAP int can be enabled. 
    DEVICE_mACCESS_EN();
    PieVectTable.CAPINT4 = &CAPDRV_fnIsr1;  // CAPINT4 ISR
#if (TEST_CAP == 2)
    PieVectTable.CAPINT5 = &CAPDRV_fnIsr2;  // CAPINT5 ISR
#endif
    DEVICE_mACCESS_DIS();

    //initialize timer, capture
    TestInitial();

    // Re-enable scheduler to allow other tasks to run
    
    while( TRUE )
    {
        TEST_fnDoPinToggle(TEST_PASS_SPEED);
    }
    
}

/*******************************************************************************

Function NAME: TestInitial

PURPOSE:
   This entry point for the Test program task

INPUTS:
   void 
   
OUTPUTS:
   None

NOTES: 
    
*******************************************************************************/
tuiSTATUS TestInitial(void)
{
    //create CAP instance; Timer instance will be created by CAPDRV_fnCtor()

    //create capture instance for line1 or channal 1
    tzZeroX1Cap.teCapFunc   = CAPDRV_eFN5;
    tzZeroX1Cap.uiCapCnt    = 0;
    tzZeroX1Cap.uiRiseFall  = 0;
    tzZeroX1Cap.teCapEdge   = CAPDRV_eRISE_FALL;
    tzZeroX1Cap.teCapReady  = CAPDRV_eDATA_INVALID;
    if( CAPDRV_fnCtor( &tzZeroX1Cap ) == eSTATUS_ERR )
    {
        assert( 0 );
        return( eSTATUS_ERR );
    }
    
    //start capture
    if( CAPDRV_fnStart( &tzZeroX1Cap ) == eSTATUS_ERR )
    {
        assert( 0 );
        return( eSTATUS_ERR );
    }
    
    //enable interrupt
    if( CAPDRV_fnEnInt( &tzZeroX1Cap ) == eSTATUS_ERR )
    {
        assert( 0 );
        return( eSTATUS_ERR );
    } 

#if (TEST_CAP == 2) 
    //create capture instance for line2 or channal 2
    tzZeroX2Cap.teCapFunc     = CAPDRV_eFN6;
    tzZeroX2Cap.uiCapCnt      = 0;
    tzZeroX2Cap.uiRiseFall    = 0;
    tzZeroX2Cap.teCapEdge     = CAPDRV_eRISE;
    tzZeroX2Cap.teCapReady    = CAPDRV_eDATA_INVALID;
    if( CAPDRV_fnCtor( &tzZeroX2Cap ) == eSTATUS_ERR )
    {
        assert( 0 );
        return( eSTATUS_ERR );
    }
    
    //start capture
    if( CAPDRV_fnStart( &tzZeroX2Cap ) == eSTATUS_ERR )
    {
        assert( 0 );
        return( eSTATUS_ERR );
    }
    
    //enable interrupt
    if( CAPDRV_fnEnInt( &tzZeroX2Cap ) == eSTATUS_ERR )
    {
        assert( 0 );
        return( eSTATUS_ERR );
    }     
#endif

    return( eSTATUS_OK );

} // end of TestInitial()

/*******************************************************************************

FUNCTION NAME: 
    CAPDRV_fnIsr1 
     
PURPOSE: 
    capture interrupt service routine

INPUTS: 
    none

OUTPUTS:
    void

NOTES:
    get captured data


*******************************************************************************/
interrupt void CAPDRV_fnIsr1( void )
{
    static uint16 count = 0;
        
    DEVICE_mGLOBAL_INT_DIS(); 

    //get captured timer counter
    ( void )CAPDRV_fnGetCnt( &tzZeroX1Cap ); 

    uiArray[count] = tzZeroX1Cap.uiCapCnt;
    uiFifostatus[count] = EvbRegs.CAPFIFOB.bit.CAP4FIFO;
    count++;
    if (count >= TEST_TIMES)
    {
       count = 0; 
    }

    //clear flag
    ( void )CAPDRV_fnClrIntFlag( &tzZeroX1Cap );
    
    DEVICE_mGLOBAL_INT_EN(); 

}  //end of  CAPDRV_fnIsr1

/*******************************************************************************

FUNCTION NAME: 
    CAPDRV_fnIsr2 
     
PURPOSE: 
    capture interrupt service routine2

INPUTS: 
    none

OUTPUTS:
    void

NOTES:
    get captured data


*******************************************************************************/
interrupt void CAPDRV_fnIsr2( void )
{
    static uint16 count = 0;
        
    DEVICE_mGLOBAL_INT_DIS(); 

    //get captured timer counter
    ( void )CAPDRV_fnGetCnt( &tzZeroX2Cap ); 

    uiArray2[count] = tzZeroX2Cap.uiCapCnt;
    uiFifostatus2[count] = EvbRegs.CAPFIFOB.bit.CAP5FIFO;
    count++;
    if (count >= TEST_TIMES)
    {
       count = 0; 
    }

    //clear flag
    ( void )CAPDRV_fnClrIntFlag( &tzZeroX2Cap );
    
    DEVICE_mGLOBAL_INT_EN(); 

}  //end of  CAPDRV_fnIsr2

/*******************************************************************************

FUNCTION NAME: 
    TEST_fnInitGPIO
  
PURPOSE: 
    Initialize GPIO specifics as required by the test cases.

INPUTS: 
    None

OUTPUTS:
    None

NOTES:
    None

VERSION HISTORY:

Version: 1.00    Date: June 16, 2004    By: Hong Yin
    - Ported from blinking led project created by Brent.

*******************************************************************************/
void TEST_fnInitGPIO(void)
{
    // must allow access to the EALLOW protected registers before we can do 
    // anything useful.
    EALLOW;
    
    // set up pin 14 (XF_GPIO pin) to be an I/O pin
    GpioMuxRegs.GPFMUX.bit.XF_GPIOF14 = 0;
    // configure the direction of pin 14 (XF_GPIO pin) to be an output
    GpioMuxRegs.GPFDIR.bit.GPIOF14 = 1;
    // configure the default for the pin to be low
    GpioDataRegs.GPFDAT.bit.GPIOF14 = 0;
    
    // disable write access to the protected registers.
    EDIS;
}
/*******************************************************************************

Function NAME:
    TEST_fnDoPinToggle

PURPOSE:
    flash test indication led with given rate.

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    None

*******************************************************************************/
void TEST_fnDoPinToggle( uint32 uiTime )
{
    DEVICE_mDELAY_US( uiTime );
    TEST_mPIN_TOGGLE();
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


