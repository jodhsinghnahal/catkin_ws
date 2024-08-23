/*==============================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:
    Testmain.c

PURPOSE:
    To test the XanBus message handling.

FUNCTION(S):
    main                    - Entry point for XanBus test
    SYS_fnAbort             - Used by assert function

    local:
        fnInstallInterrupts     - Install interrupt handlers
        fnInitCpuTimers         - Set CPU timer registers to known off state
        fnConfigCpuTimers       - Configure a CPU timer for use
        main_fnTimerInit        - Set up timer for XanBus use
        main_fnXanBusInit       - Do what we need to ready XanBus
        main_fnXanBusLoop       - Loop for XanBus in lieu of multi-tasker

    interrupt:
        ISRTimer2               - Handle timer 2 interrupt
        ISRCAN0                 - Handle CAN 0 interrupt
        ISRCAN1                 - Handle CAN 1 interrupt

NOTES:

    HOW TO USE THIS
    ---------------
        This test is run on eZdsp board for the F2812 chip. 
        If the test is underway the LED will flash at a slow rate (once every
        2 seconds), if the test passes it will flash moderately faster, and if
        the test fails it will flash very fast.
        
        For this test, you need the following:
        
        Canalyzer
        A CAN tranceiver hooked up with the following pins:  
                Tx, Rx, 3.3V, 5 V, and Common
        
        XanBusTest.can
        XanBusTest.cfg
        
        Load these files into CANAnalyer, and ensure that a node is set
        for XanBusTest.can, so it can automatically respond to messages sent
        by this test.
        
        In addition, ensure that your paths are correct as defined in your
        macro.ini file located in your compiler's directory.

CHANGE HISTORY :
$Log: TestMain.c $

    ***********************************************
    Revision: XanBus_HollyZ/1
    User: HollyZ     Date: 04/20/05  Time: 11:27PM
    CAN interrupt handlers' name changed


==============================================================================*/


/*==============================================================================
                              Includes
==============================================================================*/

//#include <string.h>
#include "LIB_stddefs.h"
#include "Device.h"         // TI device headers
#include "devicemacro.h"    // for interrupt control
#include "init.h"           // Public access to init functions
#include "xbudefs.h"        // XanBus message interface
#include "xbgdefs.h"        // XanBus application interface
#include "candrv.h"         // for the CAN ISRs
#include "xassert.h"        // for debugging

/*==============================================================================
                              Defines
==============================================================================*/

// Set blink rate to use if assert occurs - very fast
#define ASSERT_FAIL_SPEED           ( 150000 )

// Size of the XanBus heap
#ifndef XB_HEAP_SIZE
#define XB_HEAP_SIZE                ( 2000 )
#endif

// Rate at which the XanBus tick being called
#define MAIN_TICKS_PER_SEC          ( 100 )
                            
// RTI Event Counter Macros
//
// Decrement Counter
#define mTMR_EV_DEC_CNT()           DEVICE_mGLOBAL_INT_DIS();   \
                                    --uiTimerEvent;             \
                                    DEVICE_mGLOBAL_INT_EN()

// Increment Counter
#define mTMR_EV_INC_CNT( count )    DEVICE_mGLOBAL_INT_DIS();   \
                                    uiTimerEvent += count;      \
                                    DEVICE_mGLOBAL_INT_EN()

// Clear Counter
#define mTMR_EV_CLR_CNT()           DEVICE_mGLOBAL_INT_DIS();   \
                                    uiTimerEvent = 0;           \
                                    DEVICE_mGLOBAL_INT_EN()

// Set Counter
#define mTMR_EV_SET_CNT( count )    DEVICE_mGLOBAL_INT_DIS();   \
                                    uiTimerEvent = count;       \
                                    DEVICE_mGLOBAL_INT_EN()

// Get Counter value
#define mTMR_EV_GET_CNT( count )    DEVICE_mGLOBAL_INT_DIS();   \
                                    count = uiTimerEvent;       \
                                    DEVICE_mGLOBAL_INT_EN()
                                    
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

// Heap supplied for XanBus memory heap
static uchar8 aucXbHeap[ XB_HEAP_SIZE ];

// Used to interface to a hardware timer
static uint16 volatile uiTimerEvent;    // Tick event flag or counter

struct CPUTIMER_VARS CpuTimer0;
struct CPUTIMER_VARS CpuTimer1;
struct CPUTIMER_VARS CpuTimer2;

/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/

static void fnInstallInterrupts( void );
void fnInitCpuTimers(void);
void fnConfigCpuTimer(struct CPUTIMER_VARS *Timer, float Freq, float Period);
static void main_fnTimerInit( void );
static void main_fnXanBusInit( void );
static void main_fnXanBusLoop( void );

/*==============================================================================
                         Interrupt Service Routines
==============================================================================*/

extern void CANDRV_fnECAN1INTIsr( void );
extern void CANDRV_fnECAN0INTIsr( void );

interrupt void ISRTimer2(void)
{
    uiTimerEvent++;
}

interrupt void ISR_fnECAN0INT(void)
{
    CANDRV_fnECAN0INTIsr();

    IER |= 0x0100;
    EINT;
}

interrupt void ISR_fnECAN1INT(void)
{
    CANDRV_fnECAN1INTIsr();

    IER |= 0x0100;
    EINT;
}


/*==============================================================================
                           Function Definitions
==============================================================================*/

/*******************************************************************************

Function NAME:
    fnInstallInterrupts     - Install interrupt handlers

PURPOSE:
    This function is used to collect together and install all the interrupts
    handlers from one place in the code.

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    None

*******************************************************************************/

static void fnInstallInterrupts( void )
{
    // Allow access to the EALLOW protected registers
    EALLOW;

    // Install primary interrupt service routines
    PieVectTable.TINT2 = &ISRTimer2;
//    PieVectTable.ECAN0INTA = &ISRCAN0;
//    PieVectTable.ECAN1INTA = &ISRCAN1;

    // Disable access to the protected registers.
    EDIS;
}

/*******************************************************************************

Function NAME:
    fnInitCpuTimers         - Set CPU timer registers to known off state

PURPOSE:
    Initialize cpu timer registers

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    None

*******************************************************************************/

void fnInitCpuTimers( void )
{
    // Tell CPU to allow setting of controlled registers
	EALLOW;

	// Initialize address pointers to respective timer registers:
	// CpuTimerx Time support variables roughed out (externed) in 
	// DSP28_CpuTimers.h and CpuTimerxRegs are register variables 
	// used to directly access the timer registers.
	CpuTimer0.RegsAddr = &CpuTimer0Regs;
	CpuTimer1.RegsAddr = &CpuTimer1Regs;
	CpuTimer2.RegsAddr = &CpuTimer2Regs;

	// Initialize timer period to maximum:	
	CpuTimer0Regs.PRD.all  = 0xFFFFFFFF;
	CpuTimer1Regs.PRD.all  = 0xFFFFFFFF;
	CpuTimer2Regs.PRD.all  = 0xFFFFFFFF;

	// Initialize pre-scale counter to divide by 1 (SYSCLKOUT):	
	CpuTimer0Regs.TPR.all  = 0;
	CpuTimer0Regs.TPRH.all = 0;
	
	// Make sure timers are stopped:
	CpuTimer0Regs.TCR.bit.TSS = 1;             
	CpuTimer1Regs.TCR.bit.TSS = 1;             
	CpuTimer2Regs.TCR.bit.TSS = 1;             

	// Reload all counter registers with period values:
	CpuTimer0Regs.TCR.bit.TRB = 1;             
	CpuTimer1Regs.TCR.bit.TRB = 1;             
	CpuTimer2Regs.TCR.bit.TRB = 1;             
	
	// Reset interrupt counters:
	CpuTimer0.InterruptCount = 0;
	CpuTimer1.InterruptCount = 0;
	CpuTimer2.InterruptCount = 0;

   	// Disable write access to the protected registers.
	EDIS;
} // fnInitCpuTimers

/*******************************************************************************

Function NAME:
    fnConfigCpuTimers       - Configure a CPU timer for use

PURPOSE:
    

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    None

*******************************************************************************/

void fnConfigCpuTimer(struct CPUTIMER_VARS *Timer, float Freq, float Period)
{
	Uint32 	temp;
	
    // Tell CPU to allow setting of controlled registers
	EALLOW;

	// Initialize timer period:	
	Timer->CPUFreqInMHz = Freq;
	Timer->PeriodInUSec = Period;
	temp = (long) (Freq * Period);
	Timer->RegsAddr->PRD.all = temp;

	// Set pre-scale counter to divide by 1 (SYSCLKOUT):	
	Timer->RegsAddr->TPR.all  = 0;
	Timer->RegsAddr->TPRH.all  = 0;
	
	// Initialize timer control register:
	Timer->RegsAddr->TCR.bit.TSS = 1;      // 1 = Stop timer, 0 = Start/Restart Timer 
	Timer->RegsAddr->TCR.bit.TRB = 1;      // 1 = reload timer
	Timer->RegsAddr->TCR.bit.SOFT = 1;
	Timer->RegsAddr->TCR.bit.FREE = 1;     // Timer Free Run
	Timer->RegsAddr->TCR.bit.TIE = 1;      // 0 = Disable/ 1 = Enable Timer Interrupt
	
	// Reset interrupt counter:
	Timer->InterruptCount = 0;

   	// Disable write access to the protected registers.
	EDIS;
}

/*******************************************************************************

Function NAME:
    main_fnTimerInit        - Set up timer for XanBus use

PURPOSE:
    Set up hardware timer for XanBus tick events

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    None

*******************************************************************************/

static void main_fnTimerInit( void )
{
    // Tell CPU to allow setting of controlled registers
	EALLOW;

	// Remap timer2 ISR to the one we define.  This replaces the ISR located in
	// DSP28_DefaultIsr.c
	PieVectTable.TINT2 = &ISRTimer2;

   	// Disable write access to the protected registers.
	EDIS;

	// Initialize the timers to known values.
	fnInitCpuTimers();
	
	// Configure CPU-Timer 2 to interrupt every second:
	fnConfigCpuTimer( &CpuTimer2,
	                  (CTRL_CPU_MAXSPEED/1000000), // CPU Freq in MHz
	                  10000 );	           // Period in uSeconds (10 mSecond)

 	StartCpuTimer2();

    // Enable INT14 which is connected to CPU-Timer 2:
	IER |= M_INT14;
}

/*******************************************************************************

Function NAME:
    main_fnXanBusInit       - Do what we need to ready XanBus

PURPOSE:
    Do the things you need to do to get XanBus ready to run

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    None

*******************************************************************************/

static void main_fnXanBusInit( void )
{
    // Supply memory heap to Xantrex CAN package (shared by network tasks)
    if ( XBMEM_fnCtor( aucXbHeap,
                       XB_HEAP_SIZE ) == FALSE )
    {
        assert( FALSE );
    }

    // Init ISO Addr package with first four bytes of serial ID as random seed
    XBADDR_fnCtor( (uint32) 0x200 );

    // Initialize the XanBus interface
    if ( !XBINIT_fnInit( MAIN_TICKS_PER_SEC ) )
    {
        XBPLATFORM_fnCheckErrors();
        assert( FALSE );
    }       

    // Start address claiming
    XBINIT_fnNetCtrl( XB_eNET_CTRL_POWERUP );
}

/*******************************************************************************

Function NAME:
    main_fnXanBusLoop       - Loop for XanBus in lieu of multi-tasker

PURPOSE:
    This is the XanBus loop: wait for timer event then call XanBus tick

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    None

*******************************************************************************/

static void main_fnXanBusLoop( void )
{
    tucBOOL tucOneTime;
    uint16  uiFlashCount;

    tucOneTime = FALSE;
    uiFlashCount = 0;

    // Clear any timer event
    mTMR_EV_CLR_CNT();

    // Enable interrupts
    DEVICE_mGLOBAL_INT_EN();

    // No multitasker code, loop forever waiting for timer events
    for ( ; ; )
    {
        // Get timer event count
        mTMR_EV_GET_CNT( uiTimerEvent );
        
        // If timer event
        if ( uiTimerEvent != 0 )
        {
            uiFlashCount++;

            // Call CAN stack tick processing
            XBINIT_fnTick();

            // Check for unreported errors
            XBPLATFORM_fnCheckErrors();

            // Check for bus active
            if ( tucOneTime == FALSE )
            {
                // If we are now on line
                if ( XBADDR_fnOnLine() != FALSE )
                {
                    // Start the test state machine
                    TESTMSGS_fnStart();

                    // Set to bus ready
                    tucOneTime = TRUE;
                }
            }

            // Count off time event
            mTMR_EV_DEC_CNT();

            // Check result of test
            switch ( TESTMSGS_fnResult() )
            {
                case 2:     // Done, success
                    if ( uiFlashCount < 80 )
                        continue;
                    break;
                case 1:     // Failure
                    if ( uiFlashCount < 10 )
                        continue;
                    break;
                case 0:     // Not finished yet
                default:
                    if ( uiFlashCount < 200 )
                        continue;
                    break;
            }

            GpioDataRegs.GPFTOGGLE.bit.GPIOF14 = 1;
            uiFlashCount = 0;
        }
    }
}

/*******************************************************************************

Function NAME:
    SYS_fnAbort             - Used by assert function

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
    uint32 ulLEDCount;

    // Forever
    for ( ; ; )
    {
        // Delay
        for( ulLEDCount = 0; ulLEDCount < ASSERT_FAIL_SPEED; ulLEDCount++ )
            ;

        // Toggle output bit
        GpioDataRegs.GPFTOGGLE.bit.GPIOF14 = 1;
    }
}

/*******************************************************************************

FUNCTION NAME:
    main                    - Entry point for XanBus test

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

    // Call the system initialization to set up hardware registers
    INIT_fnSysInit();
	
    // Set up a hardware timer for XanBus Ticks
    main_fnTimerInit();

	// set up the vectoring table for the interrupts in the system
	// this maps the ISR vectors to the functions stored in DSP_DefaultIsr.c
    InitPieVectTable();
	
    // Install primary interrupt handlers for hardware that I use
    fnInstallInterrupts();

    // Enable PIE and CPU interrupts
    EnableInterrupts();

    // Initialize XanBus
    main_fnXanBusInit();

    // Do the XanBus loop
    main_fnXanBusLoop();

    // Catch here if loop should end
    SYS_fnAbort( "" );
}

