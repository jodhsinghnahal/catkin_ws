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
    F2812_EzDSP_RAM_lnk.cmd

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
 
    Boot sequence:
    1.  VMAP  = 1 default
        ENPIE = 0 default
        M0M1MAP = 1 default
        MP/NOT_MC = low
        ( ref TI's SPRU078.pdf pg 6-7 )
        This will configure the vector table to be mapped at 0x3FFFC0 - 3FFFFF
        of the Boot ROM section
        
    2.  On reset, the reset vector ( 0x3FFFC0 - 0x3FFFC1 ) will be copied to
        the PC.
        
    3.  It will then run the InitBoot function in Boot ROM ( 0x3FF000 )
    
    4.  InitBoot looks at GPIOF4, 12, 3 and 2 pins for boot mode during reset.
        Ensure GPIOF4=0, GPIOF12=0, GPIOF3=1 and GPIO2=0 to jump into H0 SARAM
        @ 0x3F8000.
        ( ref TI's SPRU095.pdf pg 12 )
        
    5.  The program code should write a "LB _c_int00" instruction at 0x3F8000.
    
    6.  _c_int00 initializes the C environment and calls main()
        ( ref TI's SPRU514.pdf sec 4.5.2 )

CHANGE HISTORY:
$Log: F2812_EzDSP_RAM_lnk.cmd $

   ***********************************************
   Revision: NovaPfrmB_TrevorM/5
   User: TrevorM     Date: 03/16/05  Time: 06:58PM
   Fixed mistake in definition of ZONE6 external program RAM

    ***********************************************
    Revision: NovaPfrmB_TrevorM/4
    User: TrevorM     Date: 02/18/05  Time: 05:28PM
    Consolidated memory segments.  Placed all data in internal memory allowing for
    more code space in external memory.  Moved the stack to the larger RAML segment.
    
    

    ***********************************************
    Revision: NovaPfrmB_TrevorM/3
    User: TrevorM     Date: 02/11/05  Time: 06:30PM
    Placed file scope variables into external RAM
    

    ***********************************************
    Revision: NovaPfrmB_TrevorM/2
    User: TrevorM     Date: 02/11/05  Time: 12:17AM
    Created
    
    
==============================================================================*/

/*** Continue with main section linkage ***/
MEMORY
{
PAGE 0:    /* Program Memory */
    RAMH0_START : origin = 0x3F8000, length = 0x000002  /* OnChip H0 SARAM : program start vector for DEBUG        */ 
    ZONE6       : origin = 0x100000, length = 0x010000  /* XINTF Zone 6    : program code for DEBUG                */
    RAML_CODE   : origin = 0x008000, length = 0x000400  /* Secure RAM program code */
    FLASH       : origin = 0x3D8000, length = 0x020000  /* 128K x 16 on-chip flash */
    RESET       : origin = 0x3FFFC0, length = 0x000002  /* OnChip Boot ROM (MP/MCn=0) : factory coded reset vector */ 
                                                        /* XINTF Zone 7    (MP/MCn=1) : user coded reset vector    */

PAGE 1 :   /* Data Memory */
    RAMM        : origin = 0x000040, length = 0x0007C0  /* OnChip M0/M1 RAM                                        */
    RAML        : origin = 0x008400, length = 0x001C00  /* OnChip L0/L1 RAM                                        */
    RAMH0       : origin = 0x3F8002, length = 0x001FFE  /* OnChip H0 RAM                                           */
}

 
SECTIONS
{
    /*** Compiler Required Sections ***/

    /* Program memory sections */

    .text     : > ZONE6, PAGE = 0               /* program code.                               */
    .switch   : > ZONE6, PAGE = 0               /* switch statements                           */
    .cinit    : > ZONE6, PAGE = 0               /* initialization data for global vars         */
    .reset    : > RESET,  PAGE = 0, TYPE = DSECT /* reset vector. (used only when MP/MCn=1)     */

    /* Data Memory sections */

    /* certain sections should be empty for large memory models */
    .const    :               > RAMH0,  PAGE = 1 /* constants ( !!!ensure section is empty )     */
    .econst   :               > RAMH0,  PAGE = 1 /* constants                                    */
    .stack    :               > RAML,   PAGE = 1 /* stack !!!DO NOT MOVE THIS BEYOND 0xFFFF!!!   */ 
    .bss      : fill = 0x0000 > RAMH0,  PAGE = 1 /* global variables ( ensure section is empty ) */
    .ebss     : fill = 0x0000 > RAMH0,  PAGE = 1 /* global variables                             */
    .sysmem   : fill = 0x0000 > RAMH0, PAGE = 1 /* heap ( ensure section is empty )             */
    .esysmem  : fill = 0x0000 > RAMH0, PAGE = 1 /* heap                                         */
    
    /* User Defined Sections */

    codestart :               > RAMH0_START, PAGE = 0       /* start vector ( entry.asm )                   */
    flash     :               > FLASH, PAGE = 0             /* on-chip flash */
    ramfuncs  : LOAD = ZONE6, PAGE = 0                      /* fast RAM code */
                RUN = RAML_CODE, PAGE = 0
                RUN_START (_ramfuncs_runstart),
                LOAD_START (_ramfuncs_loadstart),
                LOAD_END (_ramfuncs_loadend)
}