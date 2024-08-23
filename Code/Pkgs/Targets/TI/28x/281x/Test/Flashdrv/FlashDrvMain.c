/*=============================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by 
copyright. The reproduction, in whole or in part, by anyone without the written 
approval of Xantrex is prohibited.


FILE NAME:  
    FlashDrvMain.c

PURPOSE:
    Tests the driver that accesses on-chip Flash memory on the
    TI TMS320LF281x DSP.

FUNCTION(S):
    main        - Creates, exercises, then destroys the flash driver.


NOTES:
    Texas Instruments c28x Test:
    ---------------------------
        Hardware Required:
            TI F2812 ezDSP Emulator Board.
            
        Hardware Setup:
            Set the ezDSP to run in Flash mode (JP1 to position 1-2).
            
        Software Required:
            Code Composer Studio V. 2.20 or above
            The Flash Plugin Utility

    Tests:
    -----
    This test file runs MAX_TEST_ITERATIONS times with a slow flashing of GPIOF14
    (EzDSP DS2) once per test completed.  It will appear random, anywhere
    from once a second to once every 2 or 3 seconds depending on the length of
    the test. If an error occurs, the LED will steadily flash more quickly 
    ( approx. 10 Hz ).  Stopping the emulator should place you at the location 
    just after the operation which caused the error.
    
    To run the test, open the project file, and go to the Tools Menu in CCS.  
    Select F2xx On-Chip Flash Programmer.  Then select Execute Operation to 
    download the firmware into flash, and click Run to start the test.
    
    Also, you must include the appropriate flash utility libray in your project.
    For example, if you are testing on a F2812, include Flash2812_API_V100.lib.
    In addition, in your project options file, include what your processor 
    target is.  Ie, use either PROCESSOR_F2810, PROCESSOR_F2811 or PROCESSOR_F2812.
    
    This test does not test external flash( if any exist ).
    
    ** Remember to change JP1 for these tests. **
 
    Successful Test:
    ---------------
    The test will run for approx. 3 minutes.  Whether successful or not, the
    LED will flash at the conclusion of the test.  Stop execution and view
    where the PC is.  If it is located at the end of Main (following the
    ** Test Completed Successfully ** comment) the test was successful.
 
 
CHANGE HISTORY:  
$Log: FlashDrvMain.c $

    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/1
    User: BaldeeshK     Date: 01/24/05  Time: 05:31PM
    - Updated test due to code review changes
    

    ***********************************************
    Revision: FieldPrgm_BaldeeshK/1
    User: BaldeeshK     Date: 12/08/04  Time: 12:26AM
    - FlashDrv Test files
    
    
==============================================================================*/


/*==============================================================================
                              Includes
==============================================================================*/
#include "FlashDrv.h"
#include "Config.h"
#include "device.h"
#include "LIB_stddefs.h"
#include "LIB_stdtype.h"
#include "xassert.h"
#include "devicemacro.h"
#include "xassert.h"
#include "init.h" 
#include <errno.h>

/*==============================================================================
                              Defines
==============================================================================*/
//This test code requires NDEBUG to NOT BE DEFINED so asserts can be used
//to track issues captured by the tests.
#ifdef NDEBUG
#undef NDEBUG
#endif


#define TEST_PASS_SPEED ( 6500000 )    // Speed to blink for a pass ~1sec
#define TEST_FAIL_SPEED ( 250000  )    // Speed to blink for a fail much less 
                                       // than 1 second

// Number of times to perform all tests.
//
#define MAX_TEST_ITERATIONS     ( 5 )

#define mERROR_STOP()       flashtest_ErrorFlag = TRUE;   \
                            while ( TRUE )\
                            {\
                                for( ulLEDCount = 0; ulLEDCount < TEST_FAIL_SPEED; ulLEDCount++ );\
                                GpioDataRegs.GPFTOGGLE.bit.GPIOF14 = 1;\
                            }
                            
// Setup Hardware Init Macros for Error/Done LED
#define mTOGGLE_LED()           GpioDataRegs.GPFTOGGLE.bit.GPIOF14 = 1
#define mLED_IO_SETUP()             EALLOW; \
                                    GpioMuxRegs.GPFMUX.bit.XF_GPIOF14 = 0; \
                                    GpioMuxRegs.GPFDIR.bit.GPIOF14 = 1;   \
                                    EDIS 

#define TEST_ARRAY_SIZE     ( 128 )



#define PROTECTED_SECTOR ( FLASHDRV_SECURE_SECTORS )

// sector locations:  should match those in flash sector map, but are repeated
// here for test purposes
#define FLASH_SECTOR_1_START   ( 0x003D8000L )
#define FLASH_SECTOR_1_END     ( 0x003D9FFFL )
#define FLASH_SECTOR_2_START   ( 0x003DA000L ) 
#define FLASH_SECTOR_2_END     ( 0x003DBFFFL )
#define FLASH_SECTOR_3_START   ( 0x003DC000L )
#define FLASH_SECTOR_3_END     ( 0x003DFFFFL )
#define FLASH_SECTOR_4_START   ( 0x003E0000L )
#define FLASH_SECTOR_4_END     ( 0x003E3FFFL )
#define FLASH_SECTOR_5_START   ( 0x003E4000L )
#define FLASH_SECTOR_5_END     ( 0x003E7FFFL )
#define FLASH_SECTOR_6_START   ( 0x003E8000L )
#define FLASH_SECTOR_6_END     ( 0x003EBFFFL )
#define FLASH_SECTOR_7_START   ( 0x003EC000L )
#define FLASH_SECTOR_7_END     ( 0x003EFFFFL )
#define FLASH_SECTOR_8_START   ( 0x003F0000L )
#define FLASH_SECTOR_8_END,    ( 0x003F3FFFL )
#define FLASH_SECTOR_9_START   ( 0x003F4000L )
#define FLASH_SECTOR_9_END     ( 0x003F5FFFL )
#define FLASH_SECTOR_10_START  ( 0x003F6000L )
#define FLASH_SECTOR_10_END    ( 0x003F7F7FL )


// protected sectors
#define PROTECTED_SECTOR_START ( FLASH_SECTOR_10_START )
#define PROTECTED_SECTOR_END   ( FLASH_SECTOR_10_END )


/*==============================================================================
                            Type Definitions
==============================================================================*/

/*==============================================================================
                                Enums
==============================================================================*/
typedef enum FLASHTESTS_eTESTS
{
    FLASHTEST_eRANGE_ERASE,
    FLASHTEST_ePROG,
    FLASHTEST_eCROSS_BOUNDARY_PROGRAMMING,
    FLASHTEST_eERROR_CODE_TEST,
    FLASHTEST_eEXT_ERASE,
    FLASHTEST_eEXT_PROG, 
    FLASHTEST_eMAX_NO_TESTS
} FLASHTEST_teTESTS ;

/*=============================================================================
                              Structures
==============================================================================*/

/*==============================================================================
                           Local/Private Constants
==============================================================================*/

/*==============================================================================
                           Local/Private Variables
==============================================================================*/
static tucBOOL flashtest_ErrorFlag = FALSE;
static uint32 ulLEDCount;
static uint16 AssertCnt;
static tucBOOL AssertTest;

/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/
void flashtest_fnInit( void );
void flashtest_fnEraseRange( void );
void flashtest_fnErrorCodes( void );
void flashtest_fnCrossBoundaryProg( void );
void flashtest_fnProgram( void );


//Supporting functions for this test.
void TEST_fnStop( uint32 ulTime );

/*==============================================================================
                              Macro Definitions
==============================================================================*/


/*==============================================================================
                             Function Definitions
==============================================================================*/




/*******************************************************************************

FUNCTION NAME: 
    void main( void )
    
PURPOSE: 
    Flash driver test entry point.

INPUTS: 
    none                             
    
OUTPUTS:
    none

NOTES:
    
   
VERSION HISTORY:
Version: 0.01  Date: Feb-26-2003  By: Alan Travelbea
    - Created.
Version: 0.10  Date: Mar-19-2003  By: Alan Travelbea
    - Moved tests to functions.

*******************************************************************************/

void main( void )
{
    FLASHTEST_teTESTS eCurrentTest = FLASHTEST_eRANGE_ERASE;
    uint16 TestRound;

    // Initialize Timer and LED
    flashtest_fnInit();
    
    if( FLASHDRV_fnCtor( ) != eSTATUS_OK )
    {
        mERROR_STOP();
    }
    
    // Begin Tests
    //
    for( TestRound = 0; TestRound < MAX_TEST_ITERATIONS; ++TestRound )
    {
        eCurrentTest = FLASHTEST_eRANGE_ERASE;
        while( eCurrentTest < FLASHTEST_eMAX_NO_TESTS )
        {
            switch( eCurrentTest )
            {
        
                case FLASHTEST_eRANGE_ERASE:
                    flashtest_fnEraseRange();
                    break;

                case FLASHTEST_ePROG:
                    flashtest_fnProgram();
                    break;
        
                case FLASHTEST_eCROSS_BOUNDARY_PROGRAMMING:
                    flashtest_fnEraseRange();
                    flashtest_fnCrossBoundaryProg();
                    break;
        
                case FLASHTEST_eERROR_CODE_TEST:
                    flashtest_fnEraseRange();
                    flashtest_fnErrorCodes();
                    break;
    
                default:
                    break;
                    
            } // switch( eCurrentTest )
    
            // Toggle bit to show test is continuing...
            mTOGGLE_LED();
            ++eCurrentTest;
        }
    }
    
    // test the error codes
    flashtest_fnErrorCodes();
                                        
    FLASHDRV_fnDtor(); 
    
    // ** Test Completed Successfully **
    //
    while( TRUE )
    {
        //If we get here, then test is working.
        TEST_fnStop( (uint32)TEST_PASS_SPEED );
    }
}


/*******************************************************************************

FUNCTION NAME: 
    flashtest_fnInit
    
PURPOSE: 
    Inits hardware for the Flash driver test:
        Timer 1: Enables overflow interrupt
        Sets

INPUTS: 
    none                             
    
OUTPUTS:
    none
   
VERSION HISTORY:

Version: 1.00  Date: SEP-22-2003  By: Alan Travelbea
    - Created.

*******************************************************************************/
void flashtest_fnInit( void )
{

    //¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
    //Test and Device Setup
    //¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
    INIT_fnSysInit();   // Init the system
    
    
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
    InitPieVectTable();
    
    // Setup the LED's so that that general IO pin 14 can be used to indiciate
    // test success or failure
    mLED_IO_SETUP();
    
}
    


/*******************************************************************************

FUNCTION NAME: 
    void flashtest_fnEraseRange( void )
    
PURPOSE: 
    Tests the functionality of the EraseRange function.

INPUTS: 
    none                             
    
OUTPUTS:
    none
   
VERSION HISTORY:

Version: 0.01  Date: Mar 19, 2003  By: Alan Travelbea
    - Created.

*******************************************************************************/
void flashtest_fnEraseRange( void )
{
    uint32 Adx;
    
    // Start Driver 
    FLASHDRV_fnCtor();
    
    // Erase sector J 
    if( FLASHDRV_fnEraseRange( FLASH_SECTOR_1_START, 
                            FLASH_SECTOR_1_END ) 
                            != eSTATUS_OK )
    {
        mERROR_STOP();
    }

    // Erase sector I, H, G, and F
    if( FLASHDRV_fnEraseRange( FLASH_SECTOR_2_START,
                            FLASH_SECTOR_5_END )
                            != eSTATUS_OK )
    {
        mERROR_STOP();
    }

    for( Adx = FLASH_SECTOR_1_START; Adx < FLASH_SECTOR_5_END; ++Adx )
    {
        if( *( ( uint16* )( Adx ) ) != (uint16)0xFFFF )
        {
            mERROR_STOP();
        }
    }

}


/*******************************************************************************

FUNCTION NAME: 
    void flashtest_fnErrorCodes( void )
    
PURPOSE: 
    Tests the driver's error codes

INPUTS: 
    none                             
    
OUTPUTS:
    none
   
VERSION HISTORY:

Version: 0.01  Date: Mar 19, 2003  By: Alan Travelbea
    - Created.

*******************************************************************************/
void flashtest_fnErrorCodes( void )
{
    uint16 LargeTestArray[ TEST_ARRAY_SIZE ];
    uint16 ErrorCodes[ 10 ];


    // Start Driver 
    FLASHDRV_fnCtor();
    

    if(( FLASHDRV_fnEraseRange( 0xFFFFFFFE,
                                0xFFFFFFFF ) != eSTATUS_ERR )
            && ( errno != FLASHDRV_eERRNO_ILLEGAL_ADX ))
    {
        mERROR_STOP();
    }
    
    if(( FLASHDRV_fnProgramBlock( (uint32)0x1FFFF, 
                              ErrorCodes, 
                              1 ) 
                              != eSTATUS_ERR )
            && ( errno != FLASHDRV_eERRNO_ILLEGAL_ADX ))
    {
        mERROR_STOP();
    }

    // ** Test Protected sector error **
    if(( FLASHDRV_fnEraseRange( PROTECTED_SECTOR_START,
                                PROTECTED_SECTOR_START + 0x3FF ) != eSTATUS_ERR )
            && ( errno != FLASHDRV_eERRNO_PROT_SECTOR ))
    {
        mERROR_STOP();
    }

    if(( FLASHDRV_fnProgramBlock( PROTECTED_SECTOR_START + 0x50, 
                              LargeTestArray, 
                              TEST_ARRAY_SIZE ) 
                              != eSTATUS_ERR )
            && ( errno != FLASHDRV_eERRNO_PROT_SECTOR ))
    {
        mERROR_STOP();
    }
    
}


/*******************************************************************************

FUNCTION NAME: 
    void flashtest_fnCrossBoundaryProg( void )
    
PURPOSE: 
    Tests the functionality of the ProgramBlock function when the data to write
    crosses a sector boundary.  A write across sectors 2 and 3 is tested.

INPUTS: 
    none                             
    
OUTPUTS:
    none
   
VERSION HISTORY:

Version: 0.01  Date: Mar 19, 2003  By: Alan Travelbea
    - Created.

*******************************************************************************/
void flashtest_fnCrossBoundaryProg( void )
{
    uint32 Adx;
    uint16 Counter;
    uint16 LargeTestArray[ TEST_ARRAY_SIZE ];
    #define FDM_CBP_START   ( FLASH_SECTOR_2_START - 0x10 )
    #define FDM_CBP_END     ( FLASH_SECTOR_2_START + TEST_ARRAY_SIZE - 0x10 )
    #define FDM_CBP_PROGVAL ( 0xC3A5 )
    
    FLASHDRV_fnCtor();
    
    for ( Counter = 0; Counter < TEST_ARRAY_SIZE ; ++Counter )
    {
        LargeTestArray[ Counter ] = FDM_CBP_PROGVAL; 
    }
    
    if( FLASHDRV_fnProgramBlock( FDM_CBP_START, 
                              LargeTestArray, 
                              TEST_ARRAY_SIZE ) 
                              != eSTATUS_OK )
    {
        mERROR_STOP();
    }
    for( Adx = FDM_CBP_START; Adx < FDM_CBP_END; ++Adx )
    {
        if( *( ( uint16* )( Adx ) ) != FDM_CBP_PROGVAL )
        {
            mERROR_STOP();
        }
    }
    if( *( ( uint16* )( Adx ) ) == FDM_CBP_PROGVAL )
    {   
        // Programming went past desired end address.
        mERROR_STOP();
    }
}


/*******************************************************************************

FUNCTION NAME: 
    void flashtest_fnProgram( void )
    
PURPOSE: 
    Tests the functionality of the ProgramBlock function.

INPUTS: 
    none                             
    
OUTPUTS:
    none
   
VERSION HISTORY:

Version: 0.01  Date: Mar 19, 2003  By: Alan Travelbea
    - Created.

*******************************************************************************/
void flashtest_fnProgram( void )
{
    uint32 Adx;
    uint16 LargeTestArray[ TEST_ARRAY_SIZE ];
    uint16 iCounter;
    FLASHDRV_tzSECTORINFO SectorInfo;
    #define FLASHDRV_SECTOR1_PROGVAL    0x5555
    #define FLASHDRV_SECTOR2_PROGVAL    0xAAAA
    #define FLASHDRV_SECTOR3_PROGVAL    0x1234
    

    // Test: Program Sectors 1, 2 & 3
    //
    // Manually examine the sectors to ensure that the test string has
    //  been written correctly.  All sectors should be filled with 0x5555.
    
    // Start Driver 
    FLASHDRV_fnCtor();
    
    // Test Sector 1
    //
    for ( iCounter = 0; iCounter < TEST_ARRAY_SIZE ; ++iCounter )
    {
        LargeTestArray[ iCounter ] = FLASHDRV_SECTOR1_PROGVAL; 
    }
    
    if( FLASHDRV_fnDetermineSector( FLASH_SECTOR_1_START, &SectorInfo ) != eSTATUS_OK )
    {
        mERROR_STOP();
    }
    
    for( iCounter = 0; 
         iCounter < ( SectorInfo.ulEnd - SectorInfo.ulStart );
         iCounter += TEST_ARRAY_SIZE )
    {
        if( FLASHDRV_fnProgramBlock( SectorInfo.ulStart + iCounter, 
                              LargeTestArray, 
                              TEST_ARRAY_SIZE )
                              != eSTATUS_OK )
        {
            mERROR_STOP();
        }

    }
    for( Adx = FLASH_SECTOR_1_START; Adx < FLASH_SECTOR_1_END; ++Adx )
    {
        if( *( ( uint16* )( Adx ) ) != FLASHDRV_SECTOR1_PROGVAL )
        {
            mERROR_STOP();
        }
    }
    
    
    // Test Sector 2
    //
    if( FLASHDRV_fnDetermineSector( FLASH_SECTOR_2_START, &SectorInfo ) != eSTATUS_OK )
    {
        mERROR_STOP();
    }

    for ( iCounter = 0; iCounter < TEST_ARRAY_SIZE ; ++iCounter )
    {
        LargeTestArray[ iCounter ] = FLASHDRV_SECTOR2_PROGVAL; 
    }
    
    for( iCounter = 0; 
         iCounter < ( SectorInfo.ulEnd - SectorInfo.ulStart );
         iCounter += TEST_ARRAY_SIZE )
    {
        if( FLASHDRV_fnProgramBlock( SectorInfo.ulStart + iCounter, 
                              LargeTestArray, 
                              TEST_ARRAY_SIZE )
                              != eSTATUS_OK )
        {
            mERROR_STOP();
        }
    }
    for( Adx = FLASH_SECTOR_2_START; Adx < FLASH_SECTOR_2_END; ++Adx )
    {
        if( *( ( uint16* )( Adx ) ) != FLASHDRV_SECTOR2_PROGVAL )
        {
            mERROR_STOP();
        }
    }


    // Test Sector 3
    //
    for ( iCounter = 0; iCounter < TEST_ARRAY_SIZE ; ++iCounter )
    {
        LargeTestArray[ iCounter ] = FLASHDRV_SECTOR3_PROGVAL; 
    }

    if( FLASHDRV_fnDetermineSector( FLASH_SECTOR_3_START, &SectorInfo ) != eSTATUS_OK )
    {
        mERROR_STOP();
    }   
    
    for( iCounter = 0; 
         iCounter < ( SectorInfo.ulEnd - SectorInfo.ulStart );
         iCounter += TEST_ARRAY_SIZE )
    {
        if( FLASHDRV_fnProgramBlock( SectorInfo.ulStart + iCounter, 
                              LargeTestArray, 
                              TEST_ARRAY_SIZE )!= eSTATUS_OK )
        {
            mERROR_STOP();
        }
    }
    for( Adx = FLASH_SECTOR_3_START; Adx < FLASH_SECTOR_3_END; ++Adx )
    {
        if( *( ( uint16* )( Adx ) ) != FLASHDRV_SECTOR3_PROGVAL )
        {
            mERROR_STOP();
        }
    }
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
            TEST_fnStop( (uint32)TEST_FAIL_SPEED );
        }
    }
}

/*******************************************************************************

Function NAME:
    TEST_fnStop

PURPOSE:
    flash test indication led with given rate.

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    None

*******************************************************************************/
void TEST_fnStop( uint32 ulTime )
{
    uint32 Count;
    
    while ( TRUE )
    {
        for( Count = 0; Count < ulTime; Count++ )
        {
            
        }
        GpioDataRegs.GPFTOGGLE.bit.GPIOF14 = 1;
    };
}
