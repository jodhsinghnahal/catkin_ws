/*==============================================================================
Copyright © 2005 Xantrex Technology, Inc. All rights reserved.

This source file and the information contained in it are confidential and 
proprietary to Xantrex Technology, Inc. The reproduction or disclosure, in 
whole or in part, to anyone outside of Xantrex without the written approval of 
a Xantrex officer under a Non-Disclosure Agreement, or to any employee of 
Xantrex who has not previously obtained written authorization for access from 
the individual responsible for the source code, will have a significant 
detrimental effect on Xantrex and is expressly prohibited.

FILE NAME:  
    F2812_EzDSP_FLASH_lnk.cmd

PURPOSE:
    Debug configuration linker command file used to boot into H0 SARAM and 
    eventually into external program space for debugging.
    
    This file is used by Code Composer to configure system memory by
    allocating output sections eficiently into the memory map.
    
    Controls memory configuration, output section definition and address 
    binding.
        
FUNCTION(S):
    none
   
NOTES:

CHANGE HISTORY:
$Log:  $

==============================================================================*/

/*** Continue with main section linkage ***/
MEMORY
{
PAGE 0:    /* Program Memory */
    FLASH_CODE  : origin = 0x3D8000, length = 0x01E000  /* OnChip Flash    : program code for RELEASE              */
    FLASH_DATA  : origin = 0x3F6000, length = 0x001FF6  /* OnChip Flash    :                                       */
    FLASH_START : origin = 0x3F7FF6, length = 0x000002  /* OnChip Flash    : program start vector for RELEASE      */
   	RAML_CODE	: origin = 0x008000, length = 0x000400	/* Secure RAM program code */
    RESET       : origin = 0x3FFFC0, length = 0x000002  /* OnChip Boot ROM (MP/MCn=0) : factory coded reset vector */ 
                                                        /* XINTF Zone 7    (MP/MCn=1) : user coded reset vector    */

PAGE 1 :   /* Data Memory */
    RAMM        : origin = 0x000040, length = 0x0007C0  /* OnChip M0/M1 RAM                                        */
    RAML        : origin = 0x008400, length = 0x001C00  /* OnChip L0/L1 RAM                                        */
    RAMH0       : origin = 0x3F8002, length = 0x001FFE  /* OnChip H0 RAM                                           */
    FLASH_DATA  : origin = 0x3F6000, length = 0x001FF6  /* OnChip Flash    :                            */
	ZONE6       : origin = 0x108000, length = 0x010000  /* XINTF Zone 6    : external RAM                          */    
}

 
SECTIONS
{
    /*** Compiler Required Sections ***/

    /* Program memory sections */

    .text     : > FLASH_CODE, PAGE = 0                  /* program code.                               */
    .switch   : > FLASH_CODE, PAGE = 0                  /* switch statements                           */
    .cinit    : > FLASH_CODE, PAGE = 0                  /* initialization data for global vars         */
    .reset    : > RESET,  PAGE = 0, TYPE = DSECT /* reset vector. (used only when MP/MCn=1)     */

    /* Data Memory sections */

    /* certain sections should be empty for large memory models */
    .const    :               > RAMH0,  PAGE = 1 /* constants ( !!!ensure section is empty )     */
    .econst   : LOAD = FLASH_DATA,      PAGE = 0 /* constants ( load and run out of flash )      */
                RUN  = FLASH_DATA,      PAGE = 1

    .stack    :               > RAML,   PAGE = 1 /* stack !!!DO NOT MOVE THIS BEYOND 0xFFFF!!!   */ 
    .bss      : fill = 0x0000 > RAMH0,  PAGE = 1 /* global variables ( ensure section is empty ) */
    .ebss     : fill = 0x0000 > RAMH0,  PAGE = 1 /* global variables                             */
    .sysmem   : fill = 0x0000 > RAMH0, PAGE = 1 /* heap ( ensure section is empty )             */
    .esysmem  : fill = 0x0000 > RAMH0, PAGE = 1 /* heap                                         */
    
    /* User Defined Sections */

    codestart : > FLASH_START,          PAGE = 0     /* start vector ( entry.asm )                   */
    ramfuncs  : LOAD = FLASH_CODE, PAGE = 0                 /* fast RAM code */
                RUN = RAML_CODE, PAGE = 0
                RUN_START (_ramfuncs_runstart),
                LOAD_START (_ramfuncs_loadstart),
                LOAD_END (_ramfuncs_loadend)
}