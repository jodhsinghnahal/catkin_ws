;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
; Copyright 2004 Xantrex International.  All rights reserved.
;
; This source file is proprietary to Xantrex International. and protected by
; copyright. The reproduction, in whole or in part, by anyone without the written 
; approval of Xantrex is prohibited. 
; 
;FILE NAME:  FlashKey.asm
;
;PURPOSE:
;  This file specifies the key value inserted into the flash security registers
;  that protect the TI TMS320LF28x DSP's on-board flash.
;
;NOTES:
;	Set this value to all zeros or all F's to disable security.
;
;	CAUTION: If the security is enabled, not knowing the value of the key will
;			prevent the part from being reprogrammed.  It should only be used
;			for production releases.
;
;CHANGE HISTORY:
$Log: FlashKey.asm $

    ;   ***********************************************
    ;   Revision: FieldPrgm_BaldeeshK/1
    ;   User: BaldeeshK     Date: 12/08/04  Time: 12:24AM
    ;   - Flashdriver test files
    ;   
;   
;   
;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

          .global _PRG_key0
          .global _PRG_key1
          .global _PRG_key2
          .global _PRG_key3
          .global _PRG_key4
          .global _PRG_key5
          .global _PRG_key6
          .global _PRG_key7

; For erased flash the password locations will all be 0xFFFF       
           .text                         
_PRG_key0  .word 0xFFFF  ; PSWD bits 15-0
_PRG_key1  .word 0xFFFF  ; PSWD bits 31-16
_PRG_key2  .word 0xFFFF  ; PSWD bits 47-32
_PRG_key3  .word 0xFFFF  ; PSWD bits 63-48
_PRG_key4  .word 0xFFFF  ; PSWD bits 79-64   
_PRG_key5  .word 0xFFFF  ; PSWD bits 95-80   
_PRG_key6  .word 0xFFFF  ; PSWD bits 111-96   
_PRG_key7  .word 0xFFFF  ; PSWD bits 127-112   
