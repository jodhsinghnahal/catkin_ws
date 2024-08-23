;/*==============================================================================
;Copyright © 2005 Xantrex International

;This file is the property of Xantrex International and shall not be reproduced,
;copied, or used as the basis for the manufacture or sale of equipment without
;the express written permission of Xantrex International.

;FILE NAME:
;    isr_a.h
;
;PURPOSE:
;    To provide macros that will align the stack pointer with in the Interrupt
;    Service Rountine.
;
;FUNCTIONS:
;
;NOTES:
;  None
;
;CHANGE HISTORY:
; $Log: Isr_a.h $

;     ***********************************************
;     Revision: SurveyorPfrmA_JeffF/1
;     User: JeffF     Date: 02/13/06  Time: 06:59PM
;     Ran the copyright updater script to bring the copyright notice on all project
;     files up to date.
;
;
;==============================================================================*/


;/*==============================================================================
;                              Defines
;==============================================================================*/

;/******************************************************************************
;
;Function NAME: ISR_mALIGN_INT_SP
;
;PURPOSE:
;    Aligns the stack pointer within an interrupt service routine.
;    This is required by the compiler before calling functions.
;
;INPUTS:
;     none
;
;OUTPUTS:
;     none
;
;NOTES:
;                       
;    C compiler expects interrupts to 32-bit align the stack
;    Ref SPRU514 sec 7.3.5
;    Normally the compiler would do this alignment before the auto context
;    save.  Since the RTOS' first task when entering an interrupt
;    is to save registers and stack pointer, manipulating the SP before
;    hand would result in a corrupt stack.  Hence the alignment should only
;    be done after all registers are saved (automatic and manual)
;
;    RECOMMENDED USAGE:    
;     1.  Try not to use ALIGN_INT_SP before SP is saved.
;     2.  Always call ALIGN_INT_SP before a function call (LCR)
;     3.  If a function call is done before saving of SP, then wrap function
;          call with ALIGN_INT_SP and UNDO_ALIGN_INT_SP.
;     4.  Try not to use ALIGN_INT_SP_UNDO after registers are stored.
;     5.  Always match ALIGN_INT_SP_UNDO macros with ALIGN_INT_SP
;     6.  If a funciton call (LCR) is done after restoration of registers, then
;          wrap the call with ALIGN_INT_SP and UNDO_ALIGN_INT_SP
;
;    Ie:     ISR:                                 ISR:
;             Save Reg                             Save Reg
;             if( Nesting == 1)                    ALIGN_INT_SP
;              Save SP                             LCR OSIntEnter
;             ALIGN_INT_SP                         ALIGN_INT_SP_UNDO
;             ...                                  if( Nesting == 1 )
;             Isr code                              Save SP
;             LCR FunctionCall                     ALIGN_INT_SP
;             ...                                  ...
;             ALIGN_INT_SP_UNDO                    Isr Code
;             Restore Reg                          ...
;             Return from interrupt                LCR OSIntExit
;                                                  ALIGN_INT_SP_UNDO
;                                                  Restore Reg
;                                                  Return from interrupt
;
;VERSION HISTORY:
;
;Version: 00.01   Date: 14-Sep-2003    By: Huey Duong
;   - Created
;Version: 01.00   Date: 20-MAY-2005    By: Wayland Yu
;   - Port to the TI 280 project
;
;******************************************************************************/
ISR_mALIGN_INT_SP .macro
    ; TBD PAGE0 is a requirement of SPRU514 Sec7.2.2 but do not know why
    ; OVM and AMODE are cleared.
    ; It also expects PM to be cleared but is not done so by the compiler. 
    ASP
    CLRC   OVM, PAGE0
    CLRC   AMODE
    .endm
    
;/******************************************************************************
;
;Function NAME: ISR_mALIGN_INT_SP_UNDO
;
;PURPOSE:
;    Undo the alignment of stack pointer in an interrupt service routine
;
;INPUTS:
;     none
;
;OUTPUTS:
;     none
;
;NOTES:
;                       
;    C compiler expects interrupts to 32-bit align the stack
;    Ref SPRU514 sec 7.3.5
;    Normally the compiler would do this alignment before the auto context
;    save.  Since the RTOS' first task when entering an interrupt
;    is to save registers and stack pointer, manipulating the SP before
;    hand would result in a corrupt stack.  Hence the alignment should only
;    be done after all registers are saved (automatic and manual)
;
;    RECOMMENDED USAGE:    
;     1.  Try not to use ALIGN_INT_SP before SP is saved.
;     2.  Always call ALIGN_INT_SP at least once before a function call (LCR)
;     3.  If a function call is done before saving of SP, then wrap function
;          call with ALIGN_INT_SP and UNDO_ALIGN_INT_SP.
;     4.  Try not to use ALIGN_INT_SP_UNDO after registers are stored.
;     5.  Always match ALIGN_INT_SP_UNDO macros with ALIGN_INT_SP
;     6.  If a funciton call (LCR) is done after restoration of registers, then
;          wrap the call with ALIGN_INT_SP and UNDO_ALIGN_INT_SP
;
;    Ie:     ISR:                                 ISR:
;             Save Reg                             Save Reg
;             if( Nesting == 1)                    ALIGN_INT_SP
;              Save SP                             LCR OSIntEnter
;             ALIGN_INT_SP                         ALIGN_INT_SP_UNDO
;             ...                                  if( Nesting == 1 )
;             Isr code                              Save SP
;             LCR FunctionCall                     ALIGN_INT_SP
;             ...                                  ...
;             ALIGN_INT_SP_UNDO                    Isr Code
;             Restore Reg                          ...
;             Return from interrupt                LCR OSIntExit
;                                                  ALIGN_INT_SP_UNDO
;                                                  Restore Reg
;                                                  Return from interrupt
;
;VERSION HISTORY:
;
;Version: 00.01   Date: 14-Sep-2003    By: Huey Duong
;   - Created
;Version: 01.00   Date: 20-MAY-2005    By: Wayland Yu
;   - Port to the TI 280 project
;
;******************************************************************************/
ISR_mALIGN_INT_SP_UNDO .macro
    ; Undo stack alignment using the SPA bit in the ST1 register
    NASP
    .endm
    
