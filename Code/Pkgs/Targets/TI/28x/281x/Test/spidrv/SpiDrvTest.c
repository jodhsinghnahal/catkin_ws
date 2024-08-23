/*==============================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:
    SpiDrvTest.c

PURPOSE:
    To test the SPI driver (SPIDRV) package.

FUNCTION(S):
    TEST_fnDoPinToggle - Toggles a pin
    TEST_fnSpiDrv      - Test the driver

    SPICLIENT_fnCtor -  SPI client construction
    SPICLIENT_fnSignalStartTxRx - indicating a start of communication process
    SPICLIENT_fnWaitForTxRxComplete - waiting for communication to finish
    SPICLIENT_fnTxRxComplete - Complete a spi communication process
    SPICLIENT_fnDtor - SPI client destruction
    SPICLIENT_fnAcquireLock - acquire an exclusive lock to the SPI driver
    SPICLIENT_fnReleaseLock - release an exclusive lock to the SPI driver

    local:
        -

NOTES:

    HOW TO USE THIS
    ---------------
        This test is run on eZdsp board. 
        If the test is pass, led on the eZdsp will flash at rate about 1Hz.
            If the test is failed, the Led will flash at about 2Hz.

CHANGE HISTORY :
$Log: SpiDrvTest.c $

   ***********************************************
   Revision: NovaPfrmB_TrevorM/1
   User: TrevorM     Date: 03/11/05  Time: 07:44PM
   Modified to support SPI driver handles and exclusive access modes

    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/1
    User: BaldeeshK     Date: 12/08/04  Time: 05:01PM
    - Initial version for Accurev
    
    

==============================================================================*/


/*==============================================================================
                              Includes
==============================================================================*/
#include "spidrv.h"
#include "spiclient.h"
#include "LIB_stddefs.h"
#include "LIB_stdtype.h"
#include "Device.h"         // TI device headers
#include "devicemacro.h"
#include "xassert.h"
#include "init.h"           // Public access to init functions
#include <errno.h>

/*==============================================================================
                              Defines
==============================================================================*/
//This test code requires NDEBUG to NOT BE DEFINED so asserts can be used
//to track issues captured by the tests.
#ifdef NDEBUG
#undef NDEBUG
#endif

#define TEST_PASS_SPEED ( 500000 )    //Speed to blink Das LED if pass (1sec)
#define TEST_FAIL_SPEED ( 250000  )   //Speed to blink Das LED if fail (0.5sec)


#define TEST_mPIN_TOGGLE()   ( GpioDataRegs.GPFTOGGLE.bit.GPIOF14 = 1 )

//test default configuration
#define SPITEST_BIT_RATE_HZ    ( 1000000)
#define SPITEST_CHARLENGH_DFLT  ( 8 )

//define a few test value
#define TEST_DATA1  ( 0x1055 )
#define RECV_DATA1  ( 0x0055 )
#define TEST_DATA2  ( 0x1035 )
#define RECV_DATA2  ( 0x0035 )
#define TEST_DATA3  ( 0x1015 )
#define RECV_DATA3  ( 0x0015 )
#define TEST_DATA4  ( 0x1025 )
#define RECV_DATA4  ( 0x0025 )
#define TEST_DATA5  ( 0x1045 )
#define RECV_DATA5  ( 0x1045 )
#define RECV_DATA5_A  	( 0x0045 )
#define TEST_DATA6  ( 0x1065 )
#define RECV_DATA6  ( 0x0065 )
#define TEST_DATA7  ( 0x1085 )
#define RECV_DATA7  ( 0x0085 )
#define TEST_DATA8  ( 0x10A5 )
#define RECV_DATA8  ( 0x00A5 )
#define TEST_DATA9  ( 0x10A1 )
#define RECV_DATA9  ( 0x00A1 )
#define TEST_DATA10 ( 0x10A2 )
#define RECV_DATA10 ( 0x00A2 )
#define TEST_DATA11 ( 0x10A3 )
#define RECV_DATA11 ( 0x00A3 )
#define TEST_DATA12 ( 0x10A4 )
#define RECV_DATA12 ( 0x00A4 )

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

// Flag to synchronize tx/rx
static uint16 SpiClientFlag;

// simulated locking 
static tucBOOL Locked = FALSE;

/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/
//Supporting functions for this test.
void TEST_fnDoPinToggle( uint32 Speed );
void TEST_fnSpiDrv( void );
void TEST_fnLimitsAndAsserts( void );
interrupt void SPIDRV_fnOSIsr( void );

tuiSTATUS SPICLIENT_fnAcquireLock( sint16 siWaitTicks );
void SPICLIENT_fnReleaseLock( void );

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

	// Interrupts that are used in this example are re-mapped to
	// ISR functions found within this file.  
   	EALLOW;	// This is needed to write to EALLOW protected registers
   	PieVectTable.SPIRXINTA = &SPIDRV_fnOSIsr;

	// set up the IO to blink a LED
    // must allow access to the EALLOW protected registers before we can do
    // anything useful.
    // set up pin 14 (XF_GPIO pin) to be an I/O pin
    GpioMuxRegs.GPFMUX.bit.XF_GPIOF14 = 0;
    // configure the direction of pin 14 (XF_GPIO pin) to be an output
    GpioMuxRegs.GPFDIR.bit.GPIOF14 = 1;

    // disable write access to the protected registers.
    EDIS;

    //enable global interrupt
    DEVICE_mGLOBAL_INT_EN();

	// create the SPI driver
	if( SPIDRV_fnCtor() != eSTATUS_OK )
    {
        assert( 0 );
    }
    
    //The test for the driver.
    TEST_fnSpiDrv();

    //Test the boundries and then some.
    TEST_fnLimitsAndAsserts();

    //Test Loop
    while( TRUE )
    {
        //If we get here, then test is working.
        TEST_fnDoPinToggle( (uint32)TEST_PASS_SPEED );
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
    SpiDrvHandle hSPI;
    
    // get a handle to the SPI driver
    if( ( hSPI = SPIDRV_fnCreateHandle() ) == NULL )
    {
        assert( 0 );
    }

    AssertTest = TRUE;

    //Try to access a value just after the last one allowed.
    AssertCnt = 0;
    Scratch = SPIDRV_fnSetCfg( hSPI,
                               SPIDRV_eCTRL_MODE_INVL, 
                               SPIDRV_eRISING_EDGE,
                               SPIDRV_eWITHOUT_DELAY,
                               SPITEST_BIT_RATE_HZ,
                               SPITEST_CHARLENGH_DFLT );
    assert( AssertCnt );

    AssertCnt = 0;
    Scratch = SPIDRV_fnSetCfg( hSPI,
                               SPIDRV_eMASTER, 
                               SPIDRV_eCLK_POL_INVL,
                               SPIDRV_eWITHOUT_DELAY,
                               SPITEST_BIT_RATE_HZ,
                               SPITEST_CHARLENGH_DFLT );
    assert( AssertCnt );

    AssertCnt = 0;
    Scratch = SPIDRV_fnSetCfg( hSPI,
                               SPIDRV_eMASTER, 
                               SPIDRV_eRISING_EDGE,
                               SPIDRV_eCLK_PHS_INVL,
                               SPITEST_BIT_RATE_HZ,
                               SPITEST_CHARLENGH_DFLT );
    assert( AssertCnt );

    AssertCnt = 0;
    Scratch = SPIDRV_fnSetCfg( hSPI,
                               SPIDRV_eMASTER, 
                               SPIDRV_eRISING_EDGE,
                               SPIDRV_eWITHOUT_DELAY,
                               2,
                               SPITEST_CHARLENGH_DFLT );
    assert( AssertCnt );

    AssertCnt = 0;
    Scratch = SPIDRV_fnSetCfg( hSPI,
                               SPIDRV_eMASTER, 
                               SPIDRV_eRISING_EDGE,
                               SPIDRV_eWITHOUT_DELAY,
                               60000000L,
                               SPITEST_CHARLENGH_DFLT );
    assert( AssertCnt );

    AssertCnt = 0;
    Scratch = SPIDRV_fnSetCfg( hSPI, 
                               SPIDRV_eMASTER, 
                               SPIDRV_eRISING_EDGE,
                               SPIDRV_eWITHOUT_DELAY,
                               SPITEST_BIT_RATE_HZ,
                               0 );
    assert( AssertCnt );

    AssertCnt = 0;
    Scratch = SPIDRV_fnSetCfg( hSPI,
                               SPIDRV_eMASTER, 
                               SPIDRV_eRISING_EDGE,
                               SPIDRV_eWITHOUT_DELAY,
                               SPITEST_BIT_RATE_HZ,
                               17 );
    assert( AssertCnt );

    AssertTest = FALSE;
}


/*******************************************************************************

Function NAME:
    TEST_fnSpiDrv

PURPOSE:
    To test the SPI driver.

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    None

*******************************************************************************/
void TEST_fnSpiDrv( void )
{
    SpiDrvHandle hSPI;
    SpiDrvHandle hSPI2;
        
    SPIDRV_teCTRL_MODE 	CtrlMode;
    SPIDRV_teCLK_POL	ClkPol;
    SPIDRV_teCLK_PHS	ClkPhs;        
    uint32 		    	BitRate;
    uint16 				Length;
    uint16 				RxData;

    // get a handle to the SPI driver
    if( ( hSPI = SPIDRV_fnCreateHandle() ) == NULL )
    {
        assert( 0 );
    }

    // get an exclusive lock to the SPI driver
    if( SPIDRV_fnAcquireLock( hSPI, 
                          SPIDRV_LOCK_NOWAIT ) != eSTATUS_OK )
    {
        assert( 0 );
    }
                          
    //-------------------------------------------------------------------------
    //Test 1: Default configuration and normal operation case
    if( SPIDRV_fnSetCfg( hSPI, 
                     SPIDRV_eMASTER, 
                     SPIDRV_eRISING_EDGE,
                     SPIDRV_eWITHOUT_DELAY,
                     SPITEST_BIT_RATE_HZ,
                     SPITEST_CHARLENGH_DFLT ) != eSTATUS_OK )
    {
        assert( 0 );
    }

    //Set to loop back mode for unit testing
    SpiaRegs.SPICCR.bit.SPILBK = 1;
    
    if( SPIDRV_fnEnTx( hSPI ) != eSTATUS_OK )
    {
        assert( 0 );
    }
    
    if( SPIDRV_fnEnInt( hSPI ) != eSTATUS_OK )
    {
        assert( 0 );
    }

    SPIDRV_fnGetCfg( hSPI,
                     &CtrlMode,  
                     &ClkPol,          
                     &ClkPhs,          
                     &BitRate,
                     &Length );
                           
    assert( CtrlMode == SPIDRV_eMASTER );
    assert( ClkPol == SPIDRV_eRISING_EDGE );
    assert( ClkPhs == SPIDRV_eWITHOUT_DELAY );
    assert( BitRate == SPITEST_BIT_RATE_HZ );
    assert( Length == SPITEST_CHARLENGH_DFLT );

	if( SPIDRV_fnWrite( hSPI, 
                        TEST_DATA1 ) != eSTATUS_OK )
	{
	    assert( 0 );
	} 

	if( SPIDRV_fnRead( hSPI, 
                       &RxData ) != eSTATUS_OK )
	{
	    assert( 0 );
	} 

	assert( RxData == 0 );
	
	if( SPIDRV_fnWriteAndRead( hSPI, 
                               TEST_DATA1,
	                           &RxData ) != eSTATUS_OK )
	{
	    assert( 0 );
	} 

	assert( RxData == RECV_DATA1 );


    //-------------------------------------------------------------------------
    //Test 2: different polarity configure and normal operation case
    if( SPIDRV_fnSetCfg( hSPI,
                         SPIDRV_eMASTER, 
                         SPIDRV_eFALLING_EDGE,
                         SPIDRV_eWITHOUT_DELAY,
                         SPITEST_BIT_RATE_HZ,
                         SPITEST_CHARLENGH_DFLT ) != eSTATUS_OK )
    {
        assert( 0 );
    }                         

	if( SPIDRV_fnWrite( hSPI, 
                        TEST_DATA2 ) != eSTATUS_OK )
	{
	    assert( 0 );
	} 
    
	if( SPIDRV_fnRead( hSPI, 
                       &RxData ) != eSTATUS_OK )
	{
	    assert( 0 );
	}
     
	assert( RxData == 0 );
	
	if( SPIDRV_fnWriteAndRead( hSPI,
                               TEST_DATA2,
	                           &RxData ) != eSTATUS_OK )
	{
	    assert( 0 );
	}
     
	assert( RxData == RECV_DATA2 );

    //-------------------------------------------------------------------------
    //Test 3: different phase configure and normal operation case
    if( SPIDRV_fnSetCfg( hSPI,
                         SPIDRV_eMASTER, 
                         SPIDRV_eRISING_EDGE,
                         SPIDRV_eWITH_DELAY,
                         SPITEST_BIT_RATE_HZ,
                         SPITEST_CHARLENGH_DFLT ) != eSTATUS_OK )
    {
        assert( 0 );
    }                     

	if( SPIDRV_fnWrite( hSPI,
                        TEST_DATA3 ) != eSTATUS_OK )
	{
	    assert( 0 );
	} 
    
	if( SPIDRV_fnRead( hSPI, 
                       &RxData ) != eSTATUS_OK )
	{
	    assert( 0 );
	}
     
	assert( RxData == 0 );
	
	if( SPIDRV_fnWriteAndRead( hSPI,
                               TEST_DATA3,
	                           &RxData ) != eSTATUS_OK )
	{
	    assert( 0 );
	} 
    
	assert( RxData == RECV_DATA3 );

    //-------------------------------------------------------------------------
    //Test 4: different BitRate configure and normal operation case
    if( SPIDRV_fnSetCfg( hSPI,
                     SPIDRV_eMASTER, 
                     SPIDRV_eRISING_EDGE,
                     SPIDRV_eWITH_DELAY,
                     4000000L,
                     SPITEST_CHARLENGH_DFLT ) != eSTATUS_OK )
    {
        assert( 0 );
    }                     

	if( SPIDRV_fnWrite( hSPI,
                        TEST_DATA4 ) != eSTATUS_OK )
	{
	    assert( 0 );
	} 

	if( SPIDRV_fnRead( hSPI,
                       &RxData ) != eSTATUS_OK )
	{
	    assert( 0 );
	} 

	assert( RxData == 0 );
	
	if( SPIDRV_fnWriteAndRead( hSPI,
                               TEST_DATA4,
	                           &RxData ) != eSTATUS_OK )
	{
	    assert( 0 );
	} 

	assert( RxData == RECV_DATA4 );

    //-------------------------------------------------------------------------
    //Test 5: different char length configure and normal operation case
    if( SPIDRV_fnSetCfg( hSPI, 
                         SPIDRV_eMASTER, 
                         SPIDRV_eRISING_EDGE,
                         SPIDRV_eWITH_DELAY,
                         SPITEST_BIT_RATE_HZ,
                         16 ) != eSTATUS_OK )
    {
        assert( 0 );
    }                                 

	if( SPIDRV_fnWrite( hSPI, 
                        TEST_DATA5 ) != eSTATUS_OK )
	{
	    assert( 0 );
	} 

	if( SPIDRV_fnRead( hSPI, 
                       &RxData ) != eSTATUS_OK )
	{
	    assert( 0 );
	} 
	assert( RxData == 0 );
	
	if( SPIDRV_fnWriteAndRead( hSPI, 
                               TEST_DATA5,
	                           &RxData ) != eSTATUS_OK )
	{
	    assert( 0 );
	} 
	assert( RxData == RECV_DATA5 );

	if( SPIDRV_fnSetCharLength( hSPI,
                                SPITEST_CHARLENGH_DFLT ) != eSTATUS_OK )
    {
        assert( 0 );
    }                           
    
	if( SPIDRV_fnWriteAndRead( hSPI, 
                               TEST_DATA5,
                               &RxData ) != eSTATUS_OK )
	{
	    assert( 0 );
	} 
	assert( RxData == RECV_DATA5_A );
	
    //-------------------------------------------------------------------------
    //Test 6: communication when transmission is disabled
    if( SPIDRV_fnSetCfg( hSPI,
                     SPIDRV_eMASTER, 
                     SPIDRV_eRISING_EDGE,
                     SPIDRV_eWITH_DELAY,
                     SPITEST_BIT_RATE_HZ,
                     SPITEST_CHARLENGH_DFLT ) != eSTATUS_OK )
    {
        assert( 0 );
    }
                     
    if( SPIDRV_fnDisTx( hSPI ) != eSTATUS_OK )
    {
        assert( 0 );
    } 

	if( SPIDRV_fnWrite( hSPI, 
                        TEST_DATA6 ) != eSTATUS_OK )
	{
	    assert( 0 );
	} 
	if( SPIDRV_fnRead( hSPI, 
                       &RxData ) != eSTATUS_OK )
	{
	    assert( 0 );
	} 
	assert( RxData == 0 );
	
	if( SPIDRV_fnWriteAndRead( hSPI,
                               TEST_DATA6,
	                           &RxData ) != eSTATUS_OK )
	{
	    assert( 0 );
	} 
	assert( RxData == RECV_DATA6 );


    //-------------------------------------------------------------------------
    //Test 7: communication when interrupt is disabled
    if( SPIDRV_fnSetCfg( hSPI,
                         SPIDRV_eMASTER, 
                         SPIDRV_eRISING_EDGE,
                         SPIDRV_eWITH_DELAY,
                         SPITEST_BIT_RATE_HZ,
                         SPITEST_CHARLENGH_DFLT ) != eSTATUS_OK )
    {
        assert( 0 );
    }
                             
    if( SPIDRV_fnEnTx( hSPI ) != eSTATUS_OK )
    {
        assert( 0 );
    }
    
    if( SPIDRV_fnDisInt( hSPI ) != eSTATUS_OK )
    {
        assert( 0 );
    }

	if( SPIDRV_fnWrite( hSPI, 
                        TEST_DATA7 ) != eSTATUS_OK )
	{
	    //check event polling
	    if( SPIDRV_fnCheckEvent( hSPI ) == SPIDRV_eRX_FULL )
	    {
	    	RxData = SpiaRegs.SPIRXBUF;
	    }
	    else
	    {
	    	assert( 0 );
	    }
	    	
	} 
	else
	{
		assert( 0 );
	}
	
	if( SPIDRV_fnRead( hSPI, 
                       &RxData ) != eSTATUS_OK )
	{
	    //check event polling
	    if( SPIDRV_fnCheckEvent( hSPI ) == SPIDRV_eRX_FULL )
	    {
	    	RxData = SpiaRegs.SPIRXBUF;
	    }
	    else
	    {
	    	assert( 0 );
	    }
	    	
	} 
	else
	{
		assert( 0 );
	}
	assert( RxData == 0 );
	
	if( SPIDRV_fnWriteAndRead( hSPI, 
                               TEST_DATA7,
                               &RxData ) != eSTATUS_OK )
	{
	    //check event polling
	    if( SPIDRV_fnCheckEvent( hSPI ) == SPIDRV_eRX_FULL )
	    {
	    	RxData = SpiaRegs.SPIRXBUF;
	    }
	    else
	    {
	    	assert( 0 );
	    }
	    	
	} 
	else
	{
		assert( 0 );
	}
	assert( RxData == RECV_DATA7 );

    //-------------------------------------------------------------------------
    //Test 8: Communication when SPI is software reset
    if( SPIDRV_fnSetCfg( hSPI, 
                     SPIDRV_eMASTER, 
                     SPIDRV_eRISING_EDGE,
                     SPIDRV_eWITH_DELAY,
                     SPITEST_BIT_RATE_HZ,
                     SPITEST_CHARLENGH_DFLT ) != eSTATUS_OK )
    {
        assert( 0 );
    }               
    
    if( SPIDRV_fnEnInt( hSPI ) != eSTATUS_OK )
    {
        assert( 0 );
    }

    if( SPIDRV_fnReset( hSPI ) != eSTATUS_OK )
    {
        assert( 0 );
    }   

	if( SPIDRV_fnWrite( hSPI,
                        TEST_DATA8 ) == eSTATUS_OK )
	{
	    assert( 0 );
	} 
	if( SPIDRV_fnRead( hSPI,
                       &RxData ) == eSTATUS_OK )
	{
	    assert( 0 );
	} 
	
	if( SPIDRV_fnWriteAndRead( hSPI,
                               TEST_DATA8,
	                           &RxData ) == eSTATUS_OK )
	{
	    assert( 0 );
	} 

    if( SPIDRV_fnStart( hSPI ) != eSTATUS_OK )
    {
        assert( 0 );        
    }
    
	if( SPIDRV_fnWrite( hSPI, 
                        TEST_DATA8 ) != eSTATUS_OK )
	{
	    assert( 0 );
	} 
	if( SPIDRV_fnRead( hSPI,
                       &RxData ) != eSTATUS_OK )
	{
	    assert( 0 );
	} 
	assert( RxData == 0 );
	
	if( SPIDRV_fnWriteAndRead( hSPI,
                               TEST_DATA8,
                               &RxData ) != eSTATUS_OK )
	{
	    assert( 0 );
	} 
	assert( RxData == RECV_DATA8 );

    //-------------------------------------------------------------------------
    //Test 9: Verify locking mechanism

    if( ( hSPI2 = SPIDRV_fnCreateHandle() ) == NULL )
    {
        assert( 0 );
    }
    
    // now try to lock the SPI device (which is already locked)
    if( SPIDRV_fnAcquireLock( hSPI2,
                              SPIDRV_LOCK_NOWAIT ) == eSTATUS_OK )
    {
        assert( 0 );
    }
    
    // release the lock to the SPI driver
    SPIDRV_fnReleaseLock( hSPI );
    
    // delete the SPI SpiDrvHandles
    if( SPIDRV_fnDeleteHandle( hSPI ) != eSTATUS_OK )
    {
        assert( 0 );
    }
        
    if( SPIDRV_fnDeleteHandle( hSPI2 ) != eSTATUS_OK )
    {
        assert( 0 );
    }
    
} // TEST_fnSpiDrv()

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

FUNCTION NAME: SPICLIENT_fnCtor

  
PURPOSE: Setup and initialize the tx/rx signal synchronization.


INPUTS: 
    none

OUTPUTS: 
    tuiSTATUS

NOTES:


VERSION HISTORY:

Version: 1.00  Date: May 31 2003  By: Ron Mok
    - Created.

*******************************************************************************/
tuiSTATUS SPICLIENT_fnCtor( void )
{
    
    SpiClientFlag = 1; // indicating SPI is ready to use

	return( eSTATUS_OK );
}


/*******************************************************************************

FUNCTION NAME: SPICLIENT_fnDtor

  
PURPOSE: destroy the spi tx/rx sync.


INPUTS: 
    none.
    
OUTPUTS:
    tuiSTATUS - eSTATUS_ERR if the pkg could not be destroyed. 
              - eSTATUS_ERR if pkg is destroyed.

NOTES:
    None.

VERSION HISTORY:

Version: 1.00  Date: Sept. 20, 2004  By: Yingran Duan
    - Created.

*******************************************************************************/
#ifdef GLBL_DTOR_EN
tuiSTATUS SPICLIENT_fnDtor( void )
{
    tuiSTATUS tuiStatus = eSTATUS_OK;

    // see if SPI work is all done
    if( SpiClientFlag != 1 )
    {
        //there is still a task using SPI, so don't dtor yet
        mWRITE_STATUS_AND_ERRNO( SPIDRV_eERRNO_DTOR );
    }

    return( tuiStatus );
}
#endif // #ifdef GLBL_DTOR_EN
                                          
                                                                    
/*******************************************************************************

FUNCTION NAME: SPICLIENT_fnSignalStartTxRx

  
PURPOSE: Signal the beginning of tx/rx.


INPUTS: 
    none
    
OUTPUTS: 
    tuiSTATUS

NOTES:
    None.

VERSION HISTORY:

Version: 1.00  Date: Setp. 20, 2004  By: Yingran Duan
    - Created.

*******************************************************************************/
tuiSTATUS SPICLIENT_fnSignalStartTxRx( void )
{
  
    // check to see if the SPI is free ( 1 = available, 0 = not avail. )
    if( SpiClientFlag == 1 )
    {
        return( eSTATUS_OK );
    }

    return( eSTATUS_ERR );
}


/*******************************************************************************

FUNCTION NAME: SPICLIENT_fnWaitForTxRxComplete

  
PURPOSE: Provide a means for the spi driver to wait for the tx/rx to complete.


INPUTS: 
    none
    
OUTPUTS:
     tuiSTATUS - eSTATUS_ERR if timeout or other error occurred
               - eSTATUS_OK  if tx/rx completed in time


NOTES:
    None.

VERSION HISTORY:

Version: 1.00  Date: Sept. 20, 2004  By: Yingran Duan
    - Created.

*******************************************************************************/
tuiSTATUS SPICLIENT_fnWaitForTxRxComplete( void )
{
    uint16 LoopCnt = 0;
    //take the spi 
    SpiClientFlag -= 1;
    
    while( SpiClientFlag == 0 )
    {
        LoopCnt++;
        if( LoopCnt == 0xFFFF )
        {
            SPICLIENT_fnTxRxComplete();
            return( eSTATUS_ERR );
        }
    }

    return( eSTATUS_OK );
}


/*******************************************************************************

FUNCTION NAME: SPICLIENT_fnTxRxComplete

  
PURPOSE: let the spi drv know that the tx/rx has been completed successfully.


INPUTS: 
    none
    
OUTPUTS: 
    none

NOTES:
    None.

VERSION HISTORY:

Version: 1.00  Date: Sept. 20, 2004  By: Yingran Duan
    - Created.

*******************************************************************************/
void SPICLIENT_fnTxRxComplete( void )
{
    // spi communication donw so we know that the tx/rx was ok
    SpiClientFlag++;
}

/*******************************************************************************

FUNCTION NAME: SPICLIENT_fnAcquireLock

  
PURPOSE: allow a driver to obtain exclusive access to the SPI hardware


INPUTS: 
    siWaitTicks : maximum number of ticks to wait for the SPI hardware 
                  to become available.  This stub ignores this argument
    
OUTPUTS: 
    always returns eSTATUS_OK

NOTES:
    None.

VERSION HISTORY:

Version: 1.00  Date: Mar. 10, 2005  By: Trevor Monk
    - Created.

*******************************************************************************/
tuiSTATUS SPICLIENT_fnAcquireLock( sint16 siWaitTicks )
{
    if( Locked )
    {
        return( eSTATUS_ERR );
    }
    
    Locked = TRUE;
    
    return( eSTATUS_OK );
}

/*******************************************************************************

FUNCTION NAME: SPICLIENT_fnReleaseLock

  
PURPOSE: allow a driver to release exclusive access to the SPI hardware


INPUTS: 
    None
    
OUTPUTS: 
    None

NOTES:
    None.

VERSION HISTORY:

Version: 1.00  Date: Mar. 10, 2005  By: Trevor Monk
    - Created.

*******************************************************************************/
void SPICLIENT_fnReleaseLock( void )
{
    Locked = FALSE;
}

/*******************************************************************************

FUNCTION NAME: SPIDRV_fnOSIsr

  
PURPOSE: ISR interface for SPIDRV ISR, the interface is for further OS 
			combination

INPUTS: 
    none
    
OUTPUTS: 
    none

NOTES:
    None.

VERSION HISTORY:

Version: 1.00  Date: Oct. 28, 2004  By: Yingran Duan
    - Created.

Version: 1.10  Date: Mar. 10, 2005  By: Trevor Monk
    - updated to support function name change in SPIDRV.c
    
*******************************************************************************/
interrupt void SPIDRV_fnOSIsr( void )
{
	
	SPIDRV_fnRxIsr();
}
