/*==============================================================================
Copyright © 2005 Xantrex International

This file is the property of Xantrex International and shall not be reproduced,
copied, or used as the basis for the manufacture or sale of equipment without
the express written permission of Xantrex International.

FILE NAME:
    devicemacro.h

PURPOSE:
    Central Repository for macros

FUNCTION(S):

NOTES:

CHANGE HISTORY:
$Log: DeviceMacro.h $

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/3
    User: JeffF     Date: 02/13/06  Time: 06:59PM
    Ran the copyright updater script to bring the copyright notice on all project
    files up to date.

    ***********************************************
    Revision: SurveyorPfrmA_TestBench_JeffF/1
    User: JeffF     Date: 01/26/06  Time: 11:46PM
    Removed reset watchdog macro because it was designed for an internal watchdog
    which this project does not use.

    ***********************************************
    Revision: SurveyorPfrmA_JosephineT/1
    User: JosephineT     Date: 07/28/05  Time: 07:33PM
    - Added macro DEVICE_mWD_RST so that the watchdog can be reset from a task.
    This is needed by the functions NVMEM_fnEraseAll() and NVMEM_fnEraseData().
    These functions take a long time to complete and at the end, the micro
    is reset.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/2
    User: JeffF     Date: 05/26/05  Time: 09:14PM
    Added macro for calling OSTimeDly in terms of milliseconds instead of RTOS
    ticks.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/1
    User: JeffF     Date: 04/28/05  Time: 08:47PM
    Began integration of ADC and PWM into project

    ***********************************************
    Revision: NovaPfrmB_TrevorM/1
    User: TrevorM     Date: 03/02/05  Time: 06:04PM
    Added macros for DEVICE_mRESET, and DEVICE_mSHUTDOWN

    

    ***********************************************
    Revision: NovaPfrmB_Karli/1
    User: Karli     Date: 02/19/05  Time: 01:45AM
    Added macros for INT13 and INT14 (CPU timers 1 and 2).
    

    ***********************************************
    Revision: NovaPfrmB_JohnB/4
    User: JohnB     Date: 01/28/05  Time: 09:53PM
    Changed DEVICE_INT_KW to DEVICE_INT_FN
    

    ***********************************************
    Revision: NovaPfrmB_JohnB/3
    User: JohnB     Date: 01/24/05  Time: 09:14PM
    Updated for use with SYSCLK package. Removed INIT_fnGetSysClk()
    functionality.
    

    ***********************************************
    Revision: NovaPfrmB_JohnB/2
    User: JohnB     Date: 01/21/05  Time: 07:49PM
    SWBASE unit test updates. Main changes are to interrupt
    handling.
    

    ***********************************************
    Revision: Xantrex1_JohnB/4
    User: JohnB     Date: 01/13/05  Time: 10:38PM
    merged
    

    ***********************************************
    Revision: Xantrex1_JohnB/3
    User: JohnB     Date: 01/13/05  Time: 10:36PM
    Can't revert to back due this file being in the basis
    stream.  Merging back the time delay macro and defines
    into this file.
    

==============================================================================*/

#ifndef DEVICEMACRO_H
#define DEVICEMACRO_H


/*==============================================================================
                              Includes
==============================================================================*/
#include "config.h"
#include "Device.h"

/*==============================================================================
                              Defines
==============================================================================*/

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

// This define will convert a desired delay in milli seconds into RTOS ticks
// and call the RTOS time delay function.
#define DEVICE_mOSTIME_DELAY_MS( DelayMs )                        \
            OSTimeDly( ( uint16 )( ( ( uint32 )DelayMs 			  \
                                   * ( uint32 )OS_TICKS_PER_SEC ) \
                                   / 1000L ) );

// ---------------------------------------------------------------------------
// protected registers operation
// access enabled
#define DEVICE_mACCESS_EN()  EALLOW

// access disabled
#define DEVICE_mACCESS_DIS()  EDIS

// ---------------------------------------------------------------------------
//interrupt operation
#define DEVICE_INT_FN             interrupt //keyword for interupts
#define DEVICE_mGLOBAL_INT_DIS()  DINT      //disable
#define DEVICE_mGLOBAL_INT_EN()   EINT      //enable
#define DEVICE_mGLOBAL_RTINT_EN() DRTM      //global realtime int                                    	 

//reset
#define DEVICE_mCPU_RST()           asm(" TRAP #0");

//shutdown
#define DEVICE_mSHUTDOWN()        ESTOP0

//Disable all interrupts
#define DEVICE_mALL_INT_DIS()     ( IER = 0x0000 )

//Enable all interrupts.  
#define DEVICE_mALL_INT_EN()      ( IER = 0x2FFF )

//Clear all interrupt flags
#define DEVICE_mALL_INT_FLAG_CLEAR()     ( IFR = 0 )

//IER operations
#define IER_INT1_EN     ( 0x0001 )
#define IER_INT2_EN     ( 0x0002 )
#define IER_INT3_EN     ( 0x0004 )
#define IER_INT4_EN     ( 0x0008 )
#define IER_INT5_EN     ( 0x0010 )
#define IER_INT6_EN     ( 0x0020 )
#define IER_INT7_EN     ( 0x0040 )
#define IER_INT8_EN     ( 0x0080 )
#define IER_INT9_EN     ( 0x0100 )
#define IER_INT10_EN    ( 0x0200 )
#define IER_INT11_EN    ( 0x0400 )
#define IER_INT12_EN    ( 0x0800 )
#define IER_INT13_EN    ( 0x1000 )
#define IER_INT14_EN    ( 0x2000 )

//IFR
#define IFR_INT1_EN     ( 0x0001 )
#define IFR_INT2_EN     ( 0x0002 )
#define IFR_INT3_EN     ( 0x0004 )
#define IFR_INT4_EN     ( 0x0008 )
#define IFR_INT5_EN     ( 0x0010 )
#define IFR_INT6_EN     ( 0x0020 )
#define IFR_INT7_EN     ( 0x0040 )
#define IFR_INT8_EN     ( 0x0080 )
#define IFR_INT9_EN     ( 0x0100 )
#define IFR_INT10_EN    ( 0x0200 )
#define IFR_INT11_EN    ( 0x0400 )
#define IFR_INT12_EN    ( 0x0800 )
#define IFR_INT13_EN    ( 0x1000 )
#define IFR_INT14_EN    ( 0x2000 )

//IER acknowledge
// TBD  What are these acks for?
#define IER_INT1_ACK        ( 0x0001 )
#define IER_INT2_ACK        ( 0x0002 )
#define IER_INT3_ACK        ( 0x0004 )
#define IER_INT4_ACK        ( 0x0008 )
#define IER_INT5_ACK        ( 0x0010 )
#define IER_INT6_ACK        ( 0x0020 )
#define IER_INT7_ACK        ( 0x0040 )
#define IER_INT8_ACK        ( 0x0080 )
#define IER_INT9_ACK        ( 0x0100 )
#define IER_INT10_ACK       ( 0x0200 )
#define IER_INT11_ACK       ( 0x0400 )
#define IER_INT12_ACK       ( 0x0800 )

//interrupt 1 operation
#define DEVICE_mINT1_EN()      ( IER |=  IER_INT1_EN ) //enable
#define DEVICE_mINT1_DIS()     ( IER &= ~IER_INT1_EN ) //disable
#define DEVICE_mINT1_CLR()     ( IFR &= ~IFR_INT1_EN )  //clear

//interrupt 2 operation
#define DEVICE_mINT2_EN()      ( IER |=  IER_INT2_EN ) //enable
#define DEVICE_mINT2_DIS()     ( IER &= ~IER_INT2_EN ) //disable
#define DEVICE_mINT2_CLR()     ( IFR &= ~IFR_INT2_EN )  //clear

//interrupt 3 operation
#define DEVICE_mINT3_EN()      ( IER |=  IER_INT3_EN ) //enable
#define DEVICE_mINT3_DIS()     ( IER &= ~IER_INT3_EN ) //disable
#define DEVICE_mINT3_CLR()     ( IFR &= ~IFR_INT3_EN )  //clear

//interrupt 4 operation
#define DEVICE_mINT4_EN()      ( IER |=  IER_INT4_EN ) //enable
#define DEVICE_mINT4_DIS()     ( IER &= ~IER_INT4_EN ) //disable
#define DEVICE_mINT4_CLR()     ( IFR &= ~IFR_INT4_EN )  //clear

//interrupt 5 operation
#define DEVICE_mINT5_EN()      ( IER |=  IER_INT5_EN ) //enable
#define DEVICE_mINT5_DIS()     ( IER &= ~IER_INT5_EN ) //disable
#define DEVICE_mINT5_CLR()     ( IFR &= ~IFR_INT5_EN )  //clear

//interrupt 6 operation
#define DEVICE_mINT6_EN()      ( IER |=  IER_INT6_EN ) //enable
#define DEVICE_mINT6_DIS()     ( IER &= ~IER_INT6_EN ) //disable
#define DEVICE_mINT6_CLR()     ( IFR &= ~IFR_INT6_EN )  //clear

//interrupt 7 operation
#define DEVICE_mINT7_EN()      ( IER |=  IER_INT7_EN ) //enable
#define DEVICE_mINT7_DIS()     ( IER &= ~IER_INT7_EN ) //disable
#define DEVICE_mINT7_CLR()     ( IFR &= ~IFR_INT7_EN )  //clear

//interrupt 8 operation
#define DEVICE_mINT8_EN()      ( IER |=  IER_INT8_EN ) //enable
#define DEVICE_mINT8_DIS()     ( IER &= ~IER_INT8_EN ) //disable
#define DEVICE_mINT8_CLR()     ( IFR &= ~IFR_INT8_EN )  //clear

//interrupt 9 operation
#define DEVICE_mINT9_EN()      ( IER |=  IER_INT9_EN ) //enable
#define DEVICE_mINT9_DIS()     ( IER &= ~IER_INT9_EN ) //disable
#define DEVICE_mINT9_CLR()     ( IFR &= ~IFR_INT9_EN )  //clear

//interrupt 10 operation
#define DEVICE_mINT10_EN()      ( IER |=  IER_INT10_EN ) //enable
#define DEVICE_mINT10_DIS()     ( IER &= ~IER_INT10_EN ) //disable
#define DEVICE_mINT10_CLR()     ( IFR &= ~IFR_INT10_EN )  //clear

//interrupt 11 operation
#define DEVICE_mINT11_EN()      ( IER |=  IER_INT11_EN ) //enable
#define DEVICE_mINT11_DIS()     ( IER &= ~IER_INT11_EN ) //disable
#define DEVICE_mINT11_CLR()     ( IFR &= ~IFR_INT11_EN )  //clear

//interrupt 12 operation
#define DEVICE_mINT12_EN()      ( IER |=  IER_INT12_EN ) //enable
#define DEVICE_mINT12_DIS()     ( IER &= ~IER_INT12_EN ) //disable
#define DEVICE_mINT12_CLR()     ( IFR &= ~IFR_INT12_EN )  //clear

//interrupt 13 operation
#define DEVICE_mINT13_EN()      ( IER |=  IER_INT13_EN ) //enable
#define DEVICE_mINT13_DIS()     ( IER &= ~IER_INT13_EN ) //disable
#define DEVICE_mINT13_CLR()     ( IFR &= ~IFR_INT13_EN )  //clear

//interrupt 14 operation
#define DEVICE_mINT14_EN()      ( IER |=  IER_INT14_EN ) //enable
#define DEVICE_mINT14_DIS()     ( IER &= ~IER_INT14_EN ) //disable
#define DEVICE_mINT14_CLR()     ( IFR &= ~IFR_INT14_EN )  //clear

//Combined INT disables
#define DEVICE_mINT3456_DIS()   ( IER &= ~( IER_INT3_EN |    \
                                            IER_INT4_EN |    \
                                            IER_INT5_EN |    \
                                            IER_INT6_EN ) )

#define DEVICE_mINT456_DIS()    ( IER &= ~( IER_INT4_EN |    \
                                            IER_INT5_EN |    \
                                            IER_INT6_EN ) )

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
                          External/Public Constants
==============================================================================*/


/*==============================================================================
                          External/Public Variables
==============================================================================*/


/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/
extern void DSP28x_usDelay( uint32 Count );

#endif
