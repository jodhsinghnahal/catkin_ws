/*==============================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:
    CanDrvTest.c

PURPOSE:
    To test the CAN driver (CANDRV) package.

FUNCTION(S):
    TEST_fnCANDrv      - Test the driver

    local:
        -

NOTES:

    HOW TO USE THIS
    ---------------
        This test is run on eZdsp board for the F2812 chip. 
        If the test passes, it will flash slow, otherwise it will flash fast.
        
        For this test, you need the following:
        
        Canalyzer
        A CAN tranceiver hooked up with the following pins:  
                Tx, Rx, 3.3V, 5 V, and Common
        
        Ticantest.can
        Ticantest.cfg
        
        Load these files into CANAnalyer, and ensure that a node is set
        for TIcanttest.can, so it can automatically respond to messages sent
        by this test.
        
        In addition, ensure that your paths are correct as defined in your
        macro.ini file located in your compiler's directory.
        
        This test takes about 5 min to run....

CHANGE HISTORY :
$Log: CanDrvTest.c $

    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/2
    User: BaldeeshK     Date: 02/24/05  Time: 09:21PM
    - Modified to test more than one buffer sending at a time and checking if correct
    interrupts are being called

    


==============================================================================*/


/*==============================================================================
                              Includes
==============================================================================*/
#include <string.h>
#include "candrv.h"
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

#define TEST_PASS_SPEED ( 1000000 )    // Speed to blink for a pass ~1sec
#define TEST_FAIL_SPEED ( 150000 )    // Speed to blink for a fail much less 
                                      // than 1 second
                                      
#define TEST_NUM_TEST_LOOPS ( 5 ) 

#define TEST_NUM_TX_INTERRUPTS ( ( TEST_NUM_TEST_LOOPS * ( sizeof( atzTxFrames ) / sizeof( atzTxFrames[ 0 ] ) ) * 3 ) \
                                 + ( CANDRV_TX_BUFFERS * 2 ) )
                                 
                                 
                                 
#define TEST_NUM_RX_INTERRUPTS ( ( TEST_NUM_TEST_LOOPS * ( sizeof( atzRxFrames )/ sizeof( atzRxFrames[ 0 ] ) ) * 3 ) \
                                 + ( CANDRV_TX_BUFFERS * 2 * 4 ) )
								

// flash the LED at a fail (fast) speed
#define mERROR_STOP()       while ( TRUE )\
                            {\
                                for( ulLEDCount = 0; ulLEDCount < TEST_FAIL_SPEED; ulLEDCount++ );\
                                GpioDataRegs.GPFTOGGLE.bit.GPIOF14 = 1;\
                            }
                            
// flash the LED at a pass (slow) speed 
#define mPASS()             while ( TRUE )\
                            {\
                                for( ulLEDCount = 0; ulLEDCount < TEST_PASS_SPEED; ulLEDCount++ );\
                                GpioDataRegs.GPFTOGGLE.bit.GPIOF14 = 1;\
                            }
                            
// flash the LED at a pass (slow) speed 
#define mDELAY()            for( ulDelayCount1 = 0; ulDelayCount1 < 5000000; ulDelayCount1++ ) //\
                                DEVICE_mUS_DELAY( 65000 );
                            
// Setup Hardware Init Macros for Error/Done LED
#define mTOGGLE_LED()           GpioDataRegs.GPFTOGGLE.bit.GPIOF14 = 1
#define mLED_IO_SETUP()             EALLOW; \
                                    GpioMuxRegs.GPFMUX.bit.XF_GPIOF14 = 0; \
                                    GpioMuxRegs.GPFDIR.bit.GPIOF14 = 1;   \
                                    EDIS 


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
static uint32 ulLEDCount;
static uint32 ulDelayCount1;

static uchar8 ucRxCount = 0;
static uchar8 HandleMask = 0;
static uint16 ucTxIntCount = 0;
static uint16 ucRxIntCount = 0;
static uint16 ucErrIntCount = 0;

static tucBOOL tucRxTest = TRUE;


CANDRV_tzCAN_FRAME atzTxFrames[] = 
{
    {
        { 0x80, 0x00, 0x00, 0x01 },
        8,
        { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 }
    
    
    },

    {
        { 0x80, 0xAA, 0x55, 0x33 },
        8,
        { 0x11, 0x22, 0x33, 0x44, 0x77, 0x99, 0xAA, 0xEE }  
    
    },

    {
        { 0x80, 0x12, 0x34, 0x56 },
        8,
        { 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10 }  
    
    },

    {
        { 0x80, 0xEE, 0x44, 0x66 },
        8,
        { 0x34, 0x37, 0x02, 0x76, 0x45, 0x15, 0x75, 0x61 }  

    },

    {
        { 0x84, 0x22, 0x17, 0x64 },
        8,
        { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 }  

    }

};


// frames we expect to get back
CANDRV_tzCAN_FRAME atzRxFrames[] = 
{
    {
        { 0x80, 0xA2, 0x45, 0xC7 },
        8,
        { 0xBA, 0x7B, 0xBE, 0xC0, 0x2C, 0x75, 0x58, 0x60 }
    
    
    },

    {
        { 0x80, 0xA4, 0xCD, 0x44 },
        8,
        { 0xCA, 0xEB, 0x2F, 0x06, 0x8C, 0x4D, 0xF6, 0xCA }  
    
    },

    {
        { 0x80, 0x24, 0x6D, 0x8F },
        8,
        { 0x7D, 0xBB, 0x3D, 0x70, 0x95, 0x03, 0x26, 0x71 }  
    
    },

    {
        { 0x80, 0xAB, 0x2D, 0xD2 },
        8,
        { 0x3D, 0x92, 0x84, 0x6B, 0x39, 0xC0, 0x94, 0xA2 }  

    },

    {
        { 0x84, 0xE1, 0xB6, 0x5D },
        8,
        { 0xCC, 0xFE, 0xEF, 0x70, 0x8D, 0xDE, 0xB6, 0x28 }  

    },

    {
        { 0x80, 0xB7, 0xE6, 0x60 },
        8,
        { 0x33, 0x1B, 0x5C, 0x2B, 0xB3, 0x3F, 0x7B, 0x23 }
    
    
    },

    {
        { 0x80, 0x72, 0x7B, 0x06 },
        8,
        { 0xE1, 0xB6, 0x5D, 0xAB, 0x2D, 0xD2, 0xB4, 0x77 }  
    
    },

    {
        { 0x80, 0xFB, 0x73, 0x9F },
        8,
        { 0x33, 0x1B, 0x5C, 0x2B, 0xB3, 0x3F, 0x7B, 0x23 }  
    
    },

    {
        { 0x80, 0x6A, 0x64, 0x0A },
        8,
        { 0x98, 0x2E, 0x0C, 0x26, 0xF1, 0xDA, 0xE5, 0x1D }  

    },

    {
        { 0x84, 0x0F, 0x47, 0x68 },
        8,
        { 0xA3, 0x25, 0x05, 0xF0, 0x89, 0x98, 0xB4, 0x3F }  

    },
    {
        { 0x80, 0xF1, 0x4B, 0x13 },
        8,
        { 0x85, 0x9C, 0x73, 0x9B, 0xE7, 0x45, 0xB0, 0xB0 }
    
    
    },

    {
        { 0x80, 0x13, 0xF9, 0x2D },
        8,
        { 0xC5, 0x6A, 0xB8, 0xDA, 0x37, 0xB4, 0xC1, 0xE3 }  
    
    },

    {
        { 0x80, 0xF3, 0x3A, 0xAF },
        8,
        { 0x02, 0x9D, 0xBA, 0x52, 0xDB, 0xF1, 0xC6, 0xBE }  
    
    },

    {
        { 0x80, 0xEF, 0x44, 0x66 },
        8,
        { 0x4A, 0xB0, 0xC5, 0xA0, 0x50, 0x8D, 0x6B, 0xA5}   

    },

    {
        { 0x80, 0x23, 0x17, 0x64 },
        8,
        { 0xB5, 0x78, 0x4F, 0xA2, 0x84, 0x03, 0x82, 0x1E }  

    },
    {
        { 0x80, 0x04, 0x20, 0x56 },
        8,
        { 0xF8, 0x37, 0x12, 0xBC, 0xCB, 0x90, 0xE2, 0x86 }
    
    
    },

    {
        { 0x80, 0x55, 0xA1, 0xB2 },
        8,
        { 0xC3, 0x3D, 0x4E, 0x5F, 0xFE, 0x56, 0x76, 0xB3 }  
    
    },

    {
        { 0x80, 0x54, 0x65, 0x67 },
        8,
        { 0xEF, 0xDA, 0xC5, 0x44, 0x62, 0x74, 0x13, 0x12 }  
    
    },

    {
        { 0x80, 0xEA, 0x44, 0x66 },
        8,
        { 0x34, 0x37, 0x02, 0x76, 0x45, 0x15, 0x75, 0x61 }  

    },

    {
        { 0x81, 0x20, 0x64, 0xF0 },
        8,
        { 0xBA, 0x3B, 0x47, 0x9B, 0x6E, 0x2D, 0xF4, 0x6A }  

    }

};

/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/
//Supporting functions for this test.
void TEST_fnDoPinToggle( uint32 Speed );
void TEST_fnCANDrv( void );
void TEST_fnLimitsAndAsserts( void );
void TEST_fnMultiTx( void );
extern void CANDRV_fnECAN0INTIsr( void );
extern void CANDRV_fnECAN1INTIsr( void );

interrupt void CANDRV_fnINT1ISR( void );
interrupt void CANDRV_fnINT0ISR( void );

void CANDRVTEST_fnTxCBack( uchar8 ucChannel );
void CANDRVTEST_fnRxCBack( uchar8 ucChannel );
void CANDRVTEST_fnOvrFlowCBack( uchar8 ucChannel );
void CANDRVTEST_fnTxAbortCBack( uchar8 ucChannel );
void CANDRVTEST_fnWakeUpCBack( uchar8 ucChannel );
void CANDRVTEST_fnStatusCBack( uchar8 ucChannel );

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

    INIT_fnSysInit();
    EALLOW;


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
    EALLOW; // This is needed to write to EALLOW protected registers
    PieVectTable.ECAN0INTA = &CANDRV_fnINT0ISR;
    PieVectTable.ECAN1INTA = &CANDRV_fnINT1ISR;
    

    // set up the IO to blink a LED
    // must allow access to the EALLOW protected registers before we can do
    // anything useful.
    // set up pin 14 (XF_GPIO pin) to be an I/O pin
    GpioMuxRegs.GPFMUX.bit.XF_GPIOF14 = 0;
    // configure the direction of pin 14 (XF_GPIO pin) to be an output
    GpioMuxRegs.GPFDIR.bit.GPIOF14 = 1;

    // disable write access to the protected registers.
    EDIS;
    
    CANDRV_fnInit( 0,
                   CANDRV_eBIT_RATE_250KBPS,
                   CANDRV_eFILTER_PASS_ALL );
                   
    CANDRV_fnInitCbList();

    CANDRV_fnInstallCb( 0,CANDRV_eCB_RECEIVE, &CANDRVTEST_fnRxCBack );
    CANDRV_fnInstallCb( 0,CANDRV_eCB_OVERFLOW, &CANDRVTEST_fnOvrFlowCBack );
    CANDRV_fnInstallCb( 0,CANDRV_eCB_TXOK, &CANDRVTEST_fnTxCBack );
    CANDRV_fnInstallCb( 0,CANDRV_eCB_TXABORT, &CANDRVTEST_fnTxAbortCBack );
    CANDRV_fnInstallCb( 0,CANDRV_eCB_WAKEUP, &CANDRVTEST_fnWakeUpCBack );
    CANDRV_fnInstallCb( 0,CANDRV_eCB_STATUS, &CANDRVTEST_fnStatusCBack );

    //enable global interrupt
    DEVICE_mGLOBAL_INT_EN();
    tucRxTest = FALSE;

    TEST_fnMultiTx(); 
    // we have to delay to make sure all the Frames are sent 
    // out before we compare the value    
    mDELAY();
    mDELAY();
    
	if( HandleMask != 0x007F )
	{
	    mERROR_STOP();
	}
	
	// reset the Handle mask
	HandleMask = 0;

    tucRxTest = TRUE;
    //The test for the driver.
    TEST_fnCANDrv();
    
    CANDRV_fnDisable( 0 );
    CANDRV_fnEnable( 0 );
    
    TEST_fnCANDrv();
    
    CANDRV_fnSleep( 0, FALSE );
    CANDRV_fnWakeUp( 0 );
    
    TEST_fnCANDrv();
    
    tucRxTest = FALSE;

    TEST_fnMultiTx();
    
    // we have to delay to make sure all the Frames are sent 
    // out before we compare the value
    mDELAY();
    mDELAY();
    
	if( HandleMask != 0x007F )
	{
	    mERROR_STOP();
	}
	
	if( ucRxIntCount != TEST_NUM_RX_INTERRUPTS )
	{
		mERROR_STOP();
	}
	
	if( ucTxIntCount != TEST_NUM_TX_INTERRUPTS )
	{
		mERROR_STOP();
	}
   
   
    AssertTest = TRUE;    
       
    //Test the boundries and then some.
    TEST_fnLimitsAndAsserts();
     
    if( AssertCnt != 14 )
    {
        mERROR_STOP();
    }
    mDELAY();
    mPASS();
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

    CANDRV_fnEnable( 1 );
    CANDRV_fnDisable( 1 );
    CANDRV_fnReceive( 1,
                      NULL );
    CANDRV_fnTransmitOk( 1 );
    CANDRV_fnTransmit( 1, NULL, NULL );
    CANDRV_fnAbort( 1, 0 );
    CANDRV_fnTxStatus( 1, NULL );
    CANDRV_fnRxStatus( 1, NULL );
    CANDRV_fnInstallCb( 1, CANDRV_eCB_RECEIVE, NULL );

}


/*******************************************************************************

Function NAME:
    TEST_fnCANDRV

PURPOSE:
    To test the CAN driver.

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    None

*******************************************************************************/
void TEST_fnCANDrv( void )
{
    uchar8 ucIndex  = 0;
    uchar8 ucLoopCount;
    uchar8 ucHandle;
    
    ucRxCount = 0;
    
    for( ucLoopCount = 0;  ucLoopCount < TEST_NUM_TEST_LOOPS; ucLoopCount++ )
    {
    
        
       for( ucIndex = 0;
             ucIndex < sizeof( atzTxFrames )/sizeof( atzTxFrames[ 0 ] );
             ucIndex++ )
        {
    
            CANDRV_fnTransmit( 0,
                               &atzTxFrames[ ucIndex ],
                               &ucHandle );
                       
            mDELAY();
            ucHandle = 5;
            
        }
        

    }

} 

/*******************************************************************************

Function NAME:
    TEST_fnCANDRV

PURPOSE:
    To test the CAN driver.

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    None

*******************************************************************************/
void TEST_fnMultiTx( void )
{

	uchar8 Count;
	uchar8 ucHandle2;
	
	for( Count = 0; Count < CANDRV_TX_BUFFERS; Count++ )
	{
        CANDRV_fnTransmit( 0,
                           &atzTxFrames[ 0 ],
                           &ucHandle2 );
                       
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
        mERROR_STOP();
    }
}


/*******************************************************************************

FUNCTION NAME: CANDRVTEST_fnTxCBack

  
PURPOSE: 

INPUTS: 
    none
    
OUTPUTS: 
    none

NOTES:
    None.

VERSION HISTORY:

Version: 1.00  Date: Oct. 28, 2004  By: 
    - Created.

*******************************************************************************/
void CANDRVTEST_fnTxCBack( uchar8 ucChannel )
{
	uchar8 Test;
	
    Test = CANDRV_fnGetHandle( 0 );
    
    HandleMask |= 1 << Test;

}

/*******************************************************************************

FUNCTION NAME: CANDRVTEST_fnRxCBack

  
PURPOSE: 

INPUTS: 
    none
    
OUTPUTS: 
    none

NOTES:
    None.

VERSION HISTORY:

Version: 1.00  Date: Oct. 28, 2004  By: 
    - Created.

*******************************************************************************/
void CANDRVTEST_fnRxCBack( uchar8 ucChannel )
{
    CANDRV_tzCAN_FRAME tzFrame;
    uchar8 ucCount;

    if( CANDRV_fnReceive( 0,
                          &tzFrame ) != CANDRV_eRET_SUCCESS )
    {
        mERROR_STOP();
    
    }
    
    if( tucRxTest != FALSE )
    {    
	    if( 
	        ( tzFrame.tzCanId.ucId28_24 != atzRxFrames[ ucRxCount ].tzCanId.ucId28_24 ) ||
	        ( tzFrame.tzCanId.ucId23_16 != atzRxFrames[ ucRxCount ].tzCanId.ucId23_16 ) ||
	        ( tzFrame.tzCanId.ucId15_8 != atzRxFrames[ ucRxCount ].tzCanId.ucId15_8 ) ||
	        ( tzFrame.tzCanId.ucId07_00 != atzRxFrames[ ucRxCount ].tzCanId.ucId07_00 ) )
	    {
	        mERROR_STOP();
	    }
	    
	    for( ucCount = 0; ucCount < 8; ucCount++ )
	    {
	        if( tzFrame.aucData[ ucCount ] != atzRxFrames[ ucRxCount ].aucData[ ucCount ] )
	        {
	            mERROR_STOP();
	        }
	    }
	
	    ucRxCount++;
    }
    
    if( ucRxCount >= sizeof( atzRxFrames ) / sizeof( atzRxFrames[0] ) )
    {
    	ucRxCount = 0;
    }

}

/*******************************************************************************

FUNCTION NAME: CANDRVTEST_fnOvrFlowCBack

  
PURPOSE: 

INPUTS: 
    none
    
OUTPUTS: 
    none

NOTES:
    None.

VERSION HISTORY:

Version: 1.00  Date: Oct. 28, 2004  By: 
    - Created.

*******************************************************************************/
void CANDRVTEST_fnOvrFlowCBack( uchar8 ucChannel )
{
}

/*******************************************************************************

FUNCTION NAME: CANDRVTEST_fnTxAbortCBack

  
PURPOSE: 

INPUTS: 
    none
    
OUTPUTS: 
    none

NOTES:
    None.

VERSION HISTORY:

Version: 1.00  Date: Oct. 28, 2004  By: 
    - Created.

*******************************************************************************/
void CANDRVTEST_fnTxAbortCBack( uchar8 ucChannel )
{
}

/*******************************************************************************

FUNCTION NAME: CANDRVTEST_fnWakeUpCBack

  
PURPOSE: 

INPUTS: 
    none
    
OUTPUTS: 
    none

NOTES:
    None.

VERSION HISTORY:

Version: 1.00  Date: Oct. 28, 2004  By: 
    - Created.

*******************************************************************************/
void CANDRVTEST_fnWakeUpCBack( uchar8 ucChannel )
{
}

/*******************************************************************************

FUNCTION NAME: CANDRVTEST_fnStatusCBack

  
PURPOSE: 

INPUTS: 
    none
    
OUTPUTS: 
    none

NOTES:
    None.

VERSION HISTORY:

Version: 1.00  Date: Oct. 28, 2004  By: 
    - Created.

*******************************************************************************/
void CANDRVTEST_fnStatusCBack( uchar8 ucChannel )
{
}



/*******************************************************************************

FUNCTION NAME: CANDRV_fnINT1ISR

  
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

*******************************************************************************/
interrupt void CANDRV_fnINT1ISR( void )
{
    ucRxIntCount++;
    CANDRV_fnECAN1INTIsr();
    IER |= 0x0100;					// Enable INT9 
    EINT;


}

/*******************************************************************************

FUNCTION NAME: CANDRV_fnINT0ISR

  
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

*******************************************************************************/
interrupt void CANDRV_fnINT0ISR( void )
{
    if( ECanaRegs.CANGIF0.bit.GMIF0 == 1 )
    {
        ucTxIntCount++;
    }
    else
    {
    	ucErrIntCount++;
    }
    CANDRV_fnECAN0INTIsr(); 
    IER |= 0x0100;					// Enable INT9 
    EINT;

}
