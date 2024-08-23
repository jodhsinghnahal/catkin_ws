/*=============================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME: spiclienttest.c 

PURPOSE:
   spiclient unit test  

FUNCTION(S):
   main - the root C function 

NOTES:
    This unit test is using ezDSP board. No extra circuit is necessary.
    If the unit test is passed, the DS2 LED will flashes. 

$Log: spiclienttest.c $

   ***********************************************
   Revision: NovaPfrmB_TrevorM/1
   User: TrevorM     Date: 03/11/05  Time: 11:45PM
   modified to use SPI driver handle
    
    *****************  Version 1  *****************
    User: Yingrand     Date: 11/03/04   Time: 2:39p
    Created in $/PD/Inverter_Charger/Nova_Series/Code/Baseline/C28x/Packages/Services/HALSVC/SPICLIENT/Test
    

==============================================================================*/


/*==============================================================================
                              Includes
==============================================================================*/
#include "spidrv.h"
#include "includes.h"       //RTOS header file
#include "LIB_stddefs.h"
#include "LIB_stdtype.h"
#include "Device.h"         // TI device headers
#include "devicemacro.h"
#include "init.h"           // Public access to init functions
#include "xassert.h"

/*=============================================================================
                              Defines
=============================================================================*/
#define  TASK_STK_SIZE   256 
#define  START_TASK_PRIO   0
#define  START_TASK_ID   START_TASK_PRIO

#define TEST_mPIN_TOGGLE()   ( GpioDataRegs.GPFTOGGLE.bit.GPIOF14 = 1 )
                    
//test default configuration
#define SPITEST_BIT_RATE        ( 2000000L )
#define SPITEST_CHARLENGH_DFLT  ( 8 )

#define SPIDRV_LOCK_TIMEOUT     ( OS_TICKS_PER_SEC / 5 )

/*==============================================================================
                           Function Prototypes
==============================================================================*/
void SPIDRV_fnOSIsr( void );
void fnTestTask( void *pdata );
void TEST_fnDoPinToggle( void );

/*==============================================================================
                            File Variables
==============================================================================*/
#pragma DATA_SECTION( TaskStartStk, ".stack" );
OS_STK  TaskStartStk[ TASK_STK_SIZE ] = { 0 };

/*==============================================================================
                           Function Definitions
==============================================================================*/

// ISR stubs
void SCI_fnRxIsr(void){}
void ZEROX_fnISRL1(void){}
void ZEROX_fnISRL2(void){}
void SCI_fnTxIsr(void){}
void ACOL_fnSetPkOvrI(void){}
void SPI_fnRxIsr(void){}
void SPI_fnTxIsr(void){}
void CANDRV_fnECAN0INTIsr(void){}
void CANDRV_fnECAN1INTIsr(void){}
void DIGCTRL_fnIsrPwm(void){}
void SWBASE_fnIsrSw(void){}
void SCIDRV_fnRxIsr(void){}
void SCIDRV_fnTxIsr(void){}

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

Version: 0.10    Date: OCT-24-2001   By: Karl Isham
   - creation of stub

Version: 0.20    Date: DEC-03-2002  By: Karl Isham
   - Changed to have RTOS blink the LED for second architectural milestone. 
   

   
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

Function NAME: fnTestTask

PURPOSE:
   This entry point for the Test program task

INPUTS:
   void *pvData - task specific data sent when this task was created.
   
OUTPUTS:
   None
   
NOTES: 
   
*******************************************************************************/
void  fnTestTask( void *pvData )
{
    SpiDrvHandle hSPI;
    
    uint16 RxData;
    
    //enable global interrupt
    DEVICE_mGLOBAL_INT_EN();
       
    CpuTimer2Regs.TCR.bit.TSS = 0; // start timer
    
    if( SPIDRV_fnCtor() != eSTATUS_OK )
    {
        assert( 0 );
    }
    
    // create a handle to use to access the SPI driver
    if( (hSPI = SPIDRV_fnCreateHandle()) == NULL )
    {
        assert( 0 );
    }

    // get an exclusive lock to the SPI device
    if( SPIDRV_fnAcquireLock( hSPI, SPIDRV_LOCK_TIMEOUT ) != eSTATUS_OK )
    {
        assert( 0 );
    }
    
    // set up the SPI driver configuration
    if( SPIDRV_fnSetCfg(hSPI,
                        SPIDRV_eMASTER, 
                        SPIDRV_eRISING_EDGE,
                        SPIDRV_eWITHOUT_DELAY,
                        SPITEST_BIT_RATE,
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

    OSTimeDly( 3 );          

    while( TRUE ) 
    {
        if( SPIDRV_fnWrite( hSPI, 
                            0x28 ) != eSTATUS_OK )
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
                                   0x28,
                                   &RxData ) != eSTATUS_OK )
        {
            assert( 0 );
        } 
        assert( RxData == 0x28 );

        //######################################################
        //Flash LED to indicate the result of the unit test Done
        OSTimeDly( 100 );
        TEST_fnDoPinToggle();
    }
}

/*******************************************************************************

FUNCTION NAME: SYS_fnAbort

PURPOSE:
   

INPUTS:
   none
   
OUTPUTS:
   none
   
NOTES:
    
    
VERSION HISTORY:
 

Version: 1.00  Date: July 31, 2003  By: Ron Mok
    - Created    
   
*******************************************************************************/
void SYS_fnAbort( char *Warning )
{
    // loop forever to allow developer to examine the system
    //
    while( TRUE ) { ; } 
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
void TEST_fnDoPinToggle( void )
{
    TEST_mPIN_TOGGLE();
}

