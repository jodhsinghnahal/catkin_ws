/*==============================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:  
    main.c

PURPOSE:
    Blinking LED entry point and setup.  

FUNCTION(S):

   
NOTES:
    None
    
CHANGE HISTORY:
$Log: main.c $

    ***********************************************
    Revision: NovaPfrmB_JohnB/3
    User: JohnB     Date: 12/07/04  Time: 09:22PM
    Checking Time syncronization.
    

    ***********************************************
    Revision: NovaPfrmB_JohnB/2
    User: JohnB     Date: 12/07/04  Time: 09:21PM
    Removed dead code.
    Added file header.
    
    
==============================================================================*/

#include "main.h"
#include "DSP281x_Device.h"

void fnInitSystem(void)
{
	Uint16 i;
	
	// must allow access to the EALLOW protected registers before we can do 
	// anything useful.
	EALLOW;
	
	// Disable watchdog module and follows the required formatting for the
	// rest of the register.
   	SysCtrlRegs.WDCR = 0x0068;
   	
   	// Initalize PLL to CLKIN = (OSCCLK * 10.0)/2
   	SysCtrlRegs.PLLCR.all = 0x000A;
   	// Wait for PLL to lock
   	for(i = 0; i < 5000; i++){}
   	
   	// HISPCP/LOSPCP prescale register settings, normally it will be set to 
   	// default values, but this ensures that it is indeed set to defaults.
   	SysCtrlRegs.HISPCP.all = 0x0001;
   	SysCtrlRegs.LOSPCP.all = 0x0002;
   	
   	// now we must enable the timers to the appropriate peripherals
   	// enable Event Manager A clock
   	SysCtrlRegs.PCLKCR.bit.EVAENCLK=1;
   	// enable Event Manager B clock
   	SysCtrlRegs.PCLKCR.bit.EVBENCLK=1;   	
   
   	// disable write access to the protected registers.
	EDIS;
} // fnInitSystem

void fnInitGPIO(void)
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
} // fnInitGPIO

void fnInitPIECtrl(void)
{
	// Disable PIE:
	PieCtrlRegs.PIECRTL.bit.ENPIE = 0;

	// Disable all Peripheral Interrupts by clearing all PIEIER registers:
	PieCtrlRegs.PIEIER1.all = 0;
	PieCtrlRegs.PIEIER2.all = 0;
	PieCtrlRegs.PIEIER3.all = 0;	
	PieCtrlRegs.PIEIER4.all = 0;
	PieCtrlRegs.PIEIER5.all = 0;
	PieCtrlRegs.PIEIER6.all = 0;
	PieCtrlRegs.PIEIER7.all = 0;
	PieCtrlRegs.PIEIER8.all = 0;
	PieCtrlRegs.PIEIER9.all = 0;
	PieCtrlRegs.PIEIER10.all = 0;
	PieCtrlRegs.PIEIER11.all = 0;
	PieCtrlRegs.PIEIER12.all = 0;

	// Clear all flagged interrupts by clearing PIEIFR registers:
	PieCtrlRegs.PIEIFR1.all = 0;
	PieCtrlRegs.PIEIFR2.all = 0;
	PieCtrlRegs.PIEIFR3.all = 0;	
	PieCtrlRegs.PIEIFR4.all = 0;
	PieCtrlRegs.PIEIFR5.all = 0;
	PieCtrlRegs.PIEIFR6.all = 0;
	PieCtrlRegs.PIEIFR7.all = 0;
	PieCtrlRegs.PIEIFR8.all = 0;
	PieCtrlRegs.PIEIFR9.all = 0;
	PieCtrlRegs.PIEIFR10.all = 0;
	PieCtrlRegs.PIEIFR11.all = 0;
	PieCtrlRegs.PIEIFR12.all = 0;

	// Enable PIE:
	PieCtrlRegs.PIECRTL.bit.ENPIE = 1;
	PieCtrlRegs.PIEACK.all = 0xFFFF;
}

void fnInitCpuTimers(void)
{
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
} // fnInitCpuTimers

void fnConfigCpuTimer(struct CPUTIMER_VARS *Timer, float Freq, float Period)
{
	Uint32 	temp;
	
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
}

interrupt void ISRTimer2(void)
{
	GpioDataRegs.GPFTOGGLE.bit.GPIOF14 = 1; 
}

void main(void) 
{
	// Initialize basic system registers and stuff.
	fnInitSystem();
	
	// set up PORTF to do the right things for our flashing LED stuff :)
	fnInitGPIO();
	
	// Clear any outstanding interrupts, disable all of them except the PIE
	fnInitPIECtrl();

	// set up the vectoring table for the interrupts in the system
	// this maps the ISR vectors to the functions stored in DSP_DefaultIsr.c
	InitPieVectTable();
	
	// Initialize the timers to known values.
	fnInitCpuTimers();
	
	// must allow access to the EALLOW protected registers before we can do 
	// anything useful to the system registers.
	EALLOW;
	// remap timer2 ISR to the one we define.  This replaces the ISR located in
	// DSP28_DefaultIsr.c
	PieVectTable.TINT2 = &ISRTimer2;
   	// disable write access to the protected registers.
	EDIS;

	// Configure CPU-Timer 2 to interrupt every second:
	fnConfigCpuTimer(&CpuTimer2, 90, 1000000);	// 100MHz CPU Freq, 1 second Period (in uSeconds)
 	StartCpuTimer2();

    // Enable INT14 which is connected to CPU-Timer 2:
	IER |= M_INT14;

    // Enable global Interrupts and higher priority real-time debug events:
	
	EINT;   // Enable Global interrupt INTM
	ERTM;	// Enable Global realtime interrupt DBGM
	
	while( TRUE)
	{
	} // while
} // main
