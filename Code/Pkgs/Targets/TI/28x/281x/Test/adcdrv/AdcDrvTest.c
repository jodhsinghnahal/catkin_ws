/*==============================================================================
Copyright © 2004 Xantrex Technology, Inc. All rights reserved.

This source file and the information contained in it are confidential and
proprietary to Xantrex Technology, Inc. The reproduction or disclosure, in
whole or in part, to anyone outside of Xantrex without the written approval of
a Xantrex officer under a Non-Disclosure Agreement, or to any employee of
Xantrex who has not previously obtained written authorization for access from
the individual responsible for the source code, will have a significant
detrimental effect on Xantrex and is expressly prohibited.

FILE NAME:
    AdcDrvTest.c

PURPOSE:
    To test the ADC driver (ADCDRV) package.

FUNCTION(S):
    TEST_fnDoPinToggle - Toggles a pin
    TEST_fnAdcDrv      - Test the driver

    local:
        -

NOTES:

    HOW TO USE THIS
    ---------------

    What you will need is a hardware setup that can provide signals to each of
    the ADC channels.  I did this with 16 pots wired up to each channel.

        3.3V
        ___
         |
         |
         |
         |
         \ (Potentiometer)
         /
         \----------------------o Center of Pot To Target ADC channel
         /
         |
         |
         |
         |
         0V

    On the EzDSP 2812 Eval board it is necessary to jumper pins 17 and 18
    on P9 together, otherwise the internal VREF will have no ground
    reference and all channels will read 0.

    Assuming you loaded the workspace for this project, the GEL file should
    run at start up and setup all the required breakpoints.  It should also,
    build the project, load it into the dev board and run it.
    
    If it doesn't:
    
    Rebuild and load the project, then ensure that AdcDrvTest.gel is loaded 
    and that the conditional breakpoints are set at the _BreakHere label in 
    AdcDrvTest.c.  If not, excecute the menu option 
    GEL>>ADC Unit Test>>Setup_Breakpoints.  This will load all the required 
    conditional breakpoints into the code so that when you execute you will 
    get the realtime output for the ADC channels.

    You should see two GEL windows now at the bottom. "Run" the application
    and resize the windows so you can see what's happening.

    Adjust all the pots across the range of 0 to 3.0V and verify the readings
    traverse the entire range of the ADC.  Watch the values in the watch
    window to see them change.

    The GEL script is set up so that you can easily verify that the channel
    sequence that is stored in the sequence registers can be verified to be
    correct.  When the GEL is running you will see which channel the device
    thinks that a particular register is mapped to.  Verify that this is
    correct.

    This unit test assumes the driver is in Cascaded mode and all 16 channels
    are active.

    Since the setup of the driver is hard coded, different setups of the ADC
    module are not tested.

CHANGE HISTORY :
$History: AdcDrvTest.c $
    
    *****************  Version 3  *****************
    User: Brentt       Date: 6/23/04    Time: 12:03p
    Updated in $/PD/Common/Targets/TI/c28x/HAL/Test/adcdrv
    - More commenting changes to reflect improvments in the GEL file

    *****************  Version 2  *****************
    User: Brentt       Date: 6/23/04    Time: 10:49a
    Updated in $/PD/Common/Targets/TI/c28x/HAL/Test/adcdrv
    - Updated the file header information with notes on setting up the
    EzDSP board for this unit test.

    *****************  Version 1  *****************
    User: Brentt       Date: 6/22/04    Time: 2:46p
    Created in $/PD/Common/Targets/TI/c28x/HAL/Test/adcdrv

==============================================================================*/


/*==============================================================================
                              Includes
==============================================================================*/
#include "adcdrv.h"
#include "LIB_stddefs.h"
#include "LIB_stdtype.h"
#include "Device.h"     // TI device headers
#include "devicemacro.h"
#include "xassert.h"
#include "init.h"               // Public access to init functions
/*==============================================================================
                              Defines
==============================================================================*/
//This test code requires NDEBUG to NOT BE DEFINED so asserts can be used
//to track issues captured by the tests.
#ifdef NDEBUG
#undef NDEBUG
#endif

#define TEST_PASS_SPEED ( 7000 )    //Speed to blink Das LED if pass (1sec)
#define TEST_FAIL_SPEED ( 3000  )   //Speed to blink Das LED if fail (0.5sec)


#define TEST_NUM_MAX_ADC_CHNLS  ( 16 )

#define TEST_mPIN_TOGGLE()   ( GpioDataRegs.GPFTOGGLE.bit.GPIOF14 = 1 )

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


/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/
//Supporting functions for this test.
void    TEST_fnDoPinToggle( uint32 Speed );
void    TEST_fnAdcDrv( void );
void    TEST_fnLimitsAndAsserts( void );
void    TEST_fnInitGPIO(void);

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

Version: 1.10  Date: June 21, 2004  By: Brent Tokarchuk
    - Updated to use the INIT package for initialization.

*******************************************************************************/
void main( void )
{
    //¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
    //Test and Device Setup
    //¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
    INIT_fnSysInit();   // Init the system

    TEST_fnInitGPIO();  // set up the IO to blink a LED

    ADCDRV_fnCtor();

    //Test Loop
    while( TRUE )
    {
        //Test the boundries and then some.
        TEST_fnLimitsAndAsserts();

        //The test for the driver.
        TEST_fnAdcDrv();

        //If we get here, then test is working.
        TEST_fnDoPinToggle( (uint32)TEST_PASS_SPEED );
    }
}

/*******************************************************************************

Function NAME:
    TEST_fnInitGPIO

PURPOSE:
    Sets up the IO to be capable of blinking a LED on the F2812 EzDSP board.

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    None

VERSION HISTORY:

Version: 1.00  Date: June 14, 2004  By: Brent Tokarchuk
    - Created

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

    // disable write access to the protected registers.
    EDIS;

} // TEST_fnInitGPIO

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

    //Try to access a value just after the last one allowed.
    AssertCnt = 0;
    Scratch = ADCDRV_fnGetResultReg(  ADCDRV_eRESULT_REG_15 + 1 );
    assert( AssertCnt );

    AssertCnt = 0;
    Scratch = ADCDRV_fnGetResultChnl( ADCDRV_eCHNL_15       + 1 );
    assert( AssertCnt );

    AssertTest = FALSE;
}


/*******************************************************************************

Function NAME:
    TEST_fnAdcDrv

PURPOSE:
    To test the adc driver.

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    Make sure that the AdcDrvTest.gel GEL file is loaded and that you have
    run "Setup_Breakpoints".  This will let you monitor all the relevant data

Version: 1.00  Date: May 16, 2003  By: John Bellini
    - Created

Version: 1.10  Date: June 21, 2004  By: Brent Tokarchuk
    - Made some big changes to account for the fact that extensive GEL scripting
      is now being used to help run the unit test.

*******************************************************************************/
void TEST_fnAdcDrv( void )
{
    uint16 Cnt = 0;                                  // GP counter
    uint16 AdcSeqCnt;                                // ADC seq count value
    volatile uint16 Result;                          // GP result container
    uint16 AdResultRegs[ TEST_NUM_MAX_ADC_CHNLS ];   // Result from result regs.
    uint16 AdResultChnl[ TEST_NUM_MAX_ADC_CHNLS ];   // Result from channels.
    uint16 FunctionResult[ TEST_NUM_MAX_ADC_CHNLS ]; // Result from using function call
    uint16 MacroResult[ TEST_NUM_MAX_ADC_CHNLS ];    // Result from using macro

    //Call the construct everytime to make sure calling it more than once does
    //not bugger things up.  Results should still be valid at the end of this
    //function.
    ADCDRV_fnCtor();

    //Do a conversion
    //Delay until the converstions are done.
    //Then get the results
    ADCDRV_fnStartConv();

    //-------------------------------------------------------------------------
    //Loop until all the channels have been read.
    //If you never exit this loop, then the ADC is not running.
    do
    {
        //GP counter to determine if we've been in this loop too long.
        ++Cnt;
        AdcSeqCnt = ADCDRV_mGET_SEQ_CNT();

        //If we loop here more than 16*3 times (# of ADC channels times 3),
        //then the ADC driver or module is either not running, or running
        //too slow.
        if( Cnt > (TEST_NUM_MAX_ADC_CHNLS*3) )
        {
            assert( 0 );
        }

    }
    while( AdcSeqCnt > 0 );

    //-------------------------------------------------------------------------
    //Get the results from the last AD sampling.
    for( Cnt = 0; Cnt < TEST_NUM_MAX_ADC_CHNLS; Cnt++ )
    {
        //The intermidiate assignment to Result is in case you need to look
        //at each value as it is retrieved.
        //
        //Get the value from the result register which can have different
        //results based on the sequence order in which the channels are read.
        Result = ADCDRV_fnGetResultReg( Cnt );
        AdResultRegs[ Cnt ] = Result;

        //Get the value from the desired channel the value is attached to.
        Result = ADCDRV_fnGetResultChnl( Cnt );
        AdResultChnl[ Cnt ] = Result;
    }

    // The order is not "in-order" to make sure there is no dependancy on
    // how the values are read.  Doing it this way prevents us from using
    // a loop like for AdResultChnl and AdResultRegs above.
    FunctionResult[15] = ADCDRV_fnGetResultReg( ADCDRV_eRESULT_REG_15 );
    MacroResult[15] = ADCDRV_mGET_RESULT_REG( ADCDRV_eRESULT_REG_15 );

    FunctionResult[1] = ADCDRV_fnGetResultReg( ADCDRV_eRESULT_REG_1 );
    MacroResult[1] = ADCDRV_mGET_RESULT_REG( ADCDRV_eRESULT_REG_1 );

    FunctionResult[14] = ADCDRV_fnGetResultReg( ADCDRV_eRESULT_REG_14 );
    MacroResult[14] = ADCDRV_mGET_RESULT_REG( ADCDRV_eRESULT_REG_14 );

    FunctionResult[2] = ADCDRV_fnGetResultReg( ADCDRV_eRESULT_REG_2 );
    MacroResult[2] = ADCDRV_mGET_RESULT_REG( ADCDRV_eRESULT_REG_2 );

    FunctionResult[13] = ADCDRV_fnGetResultReg( ADCDRV_eRESULT_REG_13 );
    MacroResult[13] = ADCDRV_mGET_RESULT_REG( ADCDRV_eRESULT_REG_13 );

    FunctionResult[3] = ADCDRV_fnGetResultReg( ADCDRV_eRESULT_REG_3 );
    MacroResult[3] = ADCDRV_mGET_RESULT_REG( ADCDRV_eRESULT_REG_3 );

    FunctionResult[12] = ADCDRV_fnGetResultReg( ADCDRV_eRESULT_REG_12 );
    MacroResult[12] = ADCDRV_mGET_RESULT_REG( ADCDRV_eRESULT_REG_12 );

    FunctionResult[4] = ADCDRV_fnGetResultReg( ADCDRV_eRESULT_REG_4 );
    MacroResult[4] = ADCDRV_mGET_RESULT_REG( ADCDRV_eRESULT_REG_4 );

    FunctionResult[11] = ADCDRV_fnGetResultReg( ADCDRV_eRESULT_REG_11 );
    MacroResult[11] = ADCDRV_mGET_RESULT_REG( ADCDRV_eRESULT_REG_11 );

    FunctionResult[5] = ADCDRV_fnGetResultReg( ADCDRV_eRESULT_REG_5 );
    MacroResult[5] = ADCDRV_mGET_RESULT_REG( ADCDRV_eRESULT_REG_5 );

    FunctionResult[10] = ADCDRV_fnGetResultReg( ADCDRV_eRESULT_REG_10 ) ;
    MacroResult[10] = ADCDRV_mGET_RESULT_REG( ADCDRV_eRESULT_REG_10 );

    FunctionResult[6] = ADCDRV_fnGetResultReg( ADCDRV_eRESULT_REG_6 );
    MacroResult[6] = ADCDRV_mGET_RESULT_REG( ADCDRV_eRESULT_REG_6 );

    FunctionResult[9] = ADCDRV_fnGetResultReg( ADCDRV_eRESULT_REG_9 );
    MacroResult[9] = ADCDRV_mGET_RESULT_REG( ADCDRV_eRESULT_REG_9 );

    FunctionResult[7] = ADCDRV_fnGetResultReg( ADCDRV_eRESULT_REG_7 );
    MacroResult[7] = ADCDRV_mGET_RESULT_REG( ADCDRV_eRESULT_REG_7 );

    FunctionResult[8] = ADCDRV_fnGetResultReg( ADCDRV_eRESULT_REG_8 );
    MacroResult[8] = ADCDRV_mGET_RESULT_REG( ADCDRV_eRESULT_REG_8 );

    FunctionResult[0] = ADCDRV_fnGetResultReg( ADCDRV_eRESULT_REG_0 );
    MacroResult[0] = ADCDRV_mGET_RESULT_REG( ADCDRV_eRESULT_REG_0 );

    // This label is referenced by the GEL script so that it knows where
    // to put all the conditional breakpoints that give us our real time
    // ADC readings
    _BreakHere:
} // TEST_fnAdcDrv()

//¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤
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

//¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤
void TEST_fnDoPinToggle( uint32 uiTime )
{
    DEVICE_mDELAY_US( uiTime );
    TEST_mPIN_TOGGLE();
}
