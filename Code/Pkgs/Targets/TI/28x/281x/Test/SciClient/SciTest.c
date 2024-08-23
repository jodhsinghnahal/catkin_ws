/*=============================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME: 
   	scitest.c 

PURPOSE:
   	SCI unit test on F2812 eZdsp 

FUNCTION(S):
	main - the root C function 

NOTES:
    This unit test uses EzDSP board. The PC is connected to the DSP SCI port via
    a RS232 adapter. The SCI adapter must translate the RS232 signal to 3.3VDC max
    SCIRXD signal on the ExDsp.
    The DS2 LED flashes to indicate CPU activity, its rate also reflects the 
    receive timeout settings. 

==============================================================================*/


/*==============================================================================
                              Includes
==============================================================================*/
#include "scidrv.h"			// for SCI driver
#include "sciclient.h"		// for SCI client
#include "includes.h"       // RTOS header file
#include "LIB_stddefs.h"
#include "LIB_stdtype.h"
#include "Device.h"         // TI device headers
#include "devicemacro.h"	// for device micros
#include "init.h"           // Public access to init functions

#include "xassert.h"		// for assert

/*=============================================================================
                              Defines
=============================================================================*/
#define  TASK_STK_SIZE     256 
#define  START_TASK_PRIO   0 
#define  START_TASK_ID     START_TASK_PRIO

#define TEST_mPIN_TOGGLE()   ( GpioDataRegs.GPFTOGGLE.bit.GPIOF14 = 1 )
                    
#define SCI_TIMEOUT_1000MS   (  OS_TICKS_PER_SEC * 1 )      // 1 character per second
#define SCI_TIMEOUT_200MS    (  OS_TICKS_PER_SEC * 0.2 )    // 5 character per second

#define CR				   	 (  10 )
#define LF             		 (  13 )


/*==============================================================================
                           Function Prototypes
==============================================================================*/
void SCIDRV_fnTestTask( void *pdata );

/*==============================================================================
                            File Variables
==============================================================================*/
#pragma DATA_SECTION( TaskStartStk, ".stack" );
OS_STK  TaskStartStk[ TASK_STK_SIZE ] = { 0 };

/*==============================================================================
                           Function Definitions
==============================================================================*/


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

Version: 1.00    Date: Nov-17-2004   By: Holly Zhou
   - created for SCI unit test
   
*******************************************************************************/
void main( void )
{
    
    // system initialization
    //
    INIT_fnSysInit( );

	// Disable and clear all CPU interrupts
	//
   	DEVICE_mGLOBAL_INT_DIS();
   	DEVICE_mALL_INT_DIS();
   	DEVICE_mALL_INT_FLAG_CLEAR();
   	
 	// set up the IO to blink a LED
    // must allow access to the EALLOW protected registers before we can do
    // anything useful.
    EALLOW;	// This is needed to write to EALLOW protected registers

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
    OSTaskCreateExt( SCIDRV_fnTestTask,
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
void  SCIDRV_fnTestTask( void *pvData )
{
    uchar8 Character;       // Character received
    uchar8 Status;          // receiver status
    uint16 TimeOut;         // timeout for receiving a character 
       
    CpuTimer2Regs.TCR.bit.TSS = 0; // start timer
    
    // SCI initialization
    SCIDRV_fnCtor( SCIDRV_eBAUD_115200, 
                  SCIDRV_ePARITY_NONE, 
                  SCIDRV_eDATA_BITS_8,
                  SCIDRV_eSTOP_BITS_1 
                 );  
                    
    // Set the SCI driver timout
    TimeOut = SCI_TIMEOUT_1000MS; 
    SCIDRV_fnSetTimeout( TimeOut ); 
    
    // Enable global interrupt
    DEVICE_mGLOBAL_INT_EN();  

    // Start SCI test task loop
    while( TRUE ) 
    {  
       Status = SCIDRV_fnReceiveByte( &Character );
       
       if( Status == SCIDRV_eOK )
       {
          //If data has been received then echo 
           SCIDRV_fnSendByte( Character );
           
           // When key '!' is pressed...
           if( Character == '!' )
           {
               // toggle the TimeOut between 1 per second and 5 per second
               if( TimeOut == SCI_TIMEOUT_1000MS )
               {
                   TimeOut = SCI_TIMEOUT_200MS;
                   SCIDRV_fnSetTimeout( TimeOut );
               }
               else
               {
                   TimeOut = SCI_TIMEOUT_1000MS;
                   SCIDRV_fnSetTimeout( TimeOut );
               } 
           }       
       }
       // Error condition is detected
       else if ( Status == SCIDRV_eERROR )
       {
	    	OSTimeDly( OS_TICKS_PER_SEC );  
  	 		OSTimeDly( OS_TICKS_PER_SEC );

    		SCIDRV_fnSendByte( CR );
	    	SCIDRV_fnSendByte( LF );
	    	SCIDRV_fnSendByte( 'E' );
    		SCIDRV_fnSendByte( 'r' );
        	SCIDRV_fnSendByte( 'r' );
    		SCIDRV_fnSendByte( 'o' );
    		SCIDRV_fnSendByte( 'r' );
	    	SCIDRV_fnSendByte( CR );
	    	SCIDRV_fnSendByte( LF );  
       }    
       // Break is detected
       else if( Status == SCIDRV_eBREAK )
       {
       
    		OSTimeDly( OS_TICKS_PER_SEC );  
   			OSTimeDly( OS_TICKS_PER_SEC );   
              	
    		SCIDRV_fnSendByte( CR );
	    	SCIDRV_fnSendByte( LF );  
    		SCIDRV_fnSendByte( 'B' );
    		SCIDRV_fnSendByte( 'r' );
    		SCIDRV_fnSendByte( 'e' );
    		SCIDRV_fnSendByte( 'a' );
    		SCIDRV_fnSendByte( 'k' );
    		SCIDRV_fnSendByte( CR );
	    	SCIDRV_fnSendByte( LF );
       }
       
       // Show CPU activity by blinking DS2 on eZdsp  
	   TEST_mPIN_TOGGLE();
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

