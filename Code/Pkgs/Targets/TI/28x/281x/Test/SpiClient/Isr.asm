;*==============================================================================
;Copyright 2004 Xantrex Technology Inc.  All rights reserved.
;
;This source file is proprietary to Xantrex Technology Inc. and protected by 
;copyright. The reproduction, in whole or in part, by anyone without the written
;approval of Xantrex is prohibited.
;
;FILE NAME: Isr.asm
;
;PURPOSE:
;    This file contains the interrupt service routine that works with RTOS,
;	 	with stack/task switching mechanism.
;
;
;FUNCTION(S):
;    OSCtxSw        - context switch at task level (from a trap )
;    OSIntCtxSw     - context switch at interrupt level (from the Timer 
;                     interrupt )
;    OSStartHighRdy - switch to the highest priority task and run it
;
;NOTES:
;	 None
;
;CHANGE HISTORY:
;$History: Isr.asm $
;   
;   *****************  Version 1  *****************
;   User: Yingrand     Date: 11/03/04   Time: 2:39p
;   Created in $/PD/Inverter_Charger/Nova_Series/Code/Baseline/C28x/Packages/Services/HALSVC/SPICLIENT/Test
;   
;
;=============================================================================*/

;/*=============================================================================
;                              Includes
;=============================================================================*/
	.include OS_C28x_a.h


	.global  _SPIDRV_fnIsr       ; c coded ISR
    .global  _SPIDRV_fnOSIsr    ; Assemble the following code into the .text
     
;/*=============================================================================
;                              Definitions
;=============================================================================*/

;/*=============================================================================
;                           Function Definitions
;=============================================================================*/

    ; Assemble the following code into the .text section
    .text 

;/******************************************************************************
;
;FUNCTION NAME: 
;    _SPIDRV_fnOSIsr
;
;PURPOSE:
;    assembly code that handling context switch during SPI interrupt
;
;INPUTS:
;    none
;
;OUTPUTS:
;    none
;
;NOTES:
;
;VERSION HISTORY:
;
;Version: 1.00    Date: Oct. 27, 2004   By: Yingran Duan
;    - Created
;
;******************************************************************************/
_SPIDRV_fnOSIsr:

    ; Save processor registers
    ; Call OSIntEnter() or increment OSIntNesting
    ; if( OSIntNesting == 1)
    ;    OSTCBCur->OSTCBStkPtr = Stack Pointer
    OSC28XA_mHW_INT_ENTER
   
    ; Call SPI ISR service routine
    LCR    _SPIDRV_fnIsr
    
    ; Clear interrupting device
    ; NOTE: This interrupt does not go through PIE, hence it is automatically
    ;       cleared

    ; OSIntExit();
    ; Restore processor registers;
    ; Execute a return from interrupt instruction
    OSC28XA_mHW_INT_EXIT

;/******************************************************************************
;                           End of Assembly File
;******************************************************************************/
	.end
	
