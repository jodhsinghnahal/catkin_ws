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
;    This file contains the interrupt service routine that works with RTOS.
;
;
;FUNCTION(S):
;
;NOTES:
;	 None
;
;CHANGE HISTORY:
;$History: Isr.asm $
;   
;   *****************  Version 1  *****************
;   User: Hollyz       Date: 11/18/04   Time: 2:35p
;   Created in $/PD/Inverter_Charger/Nova_Series/Code/Baseline/C28x/Packages/Services/HALSVC/SciClient/Test
;   Created for SCI unit test
  

;=============================================================================*/

;/*=============================================================================
;                              Includes
;=============================================================================*/
	.include OS_C28x_a.h

	.global  _SCIDRV_fnSciTxInterrupt       ; C coded Transmission ISR
	.global  _SCIDRV_fnSciRxInterrupt       ; C coded Receiving ISR  
    .global  _SCIDRV_fnOSTxIsr    
    .global  _SCIDRV_fnOSRxIsr    
    
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
;    _SCIDRV_fnOSIsr
;
;PURPOSE:
;    assembly code that handling SCI Tx interrupt
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
;Version: 1.00    Date: Nov-17-2004   By: Holly Zhou
;    - Created
;
;******************************************************************************/
_SCIDRV_fnOSTxIsr:

    ; Save processor registers
    ; Call OSIntEnter() or increment OSIntNesting
    ; if( OSIntNesting == 1)
    ;    OSTCBCur->OSTCBStkPtr = Stack Pointer
    OSC28XA_mHW_INT_ENTER
   
    ; Call SPI ISR service routine
    LCR    _SCIDRV_fnSciTxInterrupt
    
    ; Clear interrupting device
    ; NOTE: This interrupt does not go through PIE, hence it is automatically
    ;       cleared

    ; OSIntExit();
    ; Restore processor registers;
    ; Execute a return from interrupt instruction
    OSC28XA_mHW_INT_EXIT
    
;/******************************************************************************
;
;FUNCTION NAME: 
;    _SCIDRV_fnOSIsr
;
;PURPOSE:
;    assembly code that handling SCI Rx interrupt
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
;Version: 1.00    Date: Nov-17-2004   By: Holly Zhou
;    - Created
;
;******************************************************************************/
_SCIDRV_fnOSRxIsr:

    ; Save processor registers
    ; Call OSIntEnter() or increment OSIntNesting
    ; if( OSIntNesting == 1)
    ;    OSTCBCur->OSTCBStkPtr = Stack Pointer
    OSC28XA_mHW_INT_ENTER
   
    ; Call SPI ISR service routine
    LCR    _SCIDRV_fnSciRxInterrupt
    
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
	
