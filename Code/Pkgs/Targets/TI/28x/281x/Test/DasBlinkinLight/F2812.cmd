/*
//
//      TMDX BETA RELEASE
//      Intended for product evaluation purposes
//
//###########################################################################
//
// FILE:	F2812.cmd
//
// TITLE:	Linker Command File For F2812 Device
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  0.51| 01 May 2002 | L.H. | EzDSP Alpha Release
//  0.56| 20 May 2002 | L.H. | Peripheral frame 1 & 2 names were swapped
//  0.58| 18 Jun 2002 | L.H. | .reset and vectors section are set to type = DSECT
//      |             |      | IQ math table in ROM is set to type = NOLOAD
//      |             |      | Added codestart section to redirect execution after
//      |             |      | a from boot to flash
//      |             |      | Fixed FLASHA length
//      |             |      | CPU Timer1 and CPU Timer2 are reserved for BIOS/RTOS
//      |             |      | Added XINTF Zones
//  0.59| 20 Aug 2002 | T.A. | Added CPU timer 2 back to support Blink.
//###########################################################################
*/

/* Define the memory block start/length for the F2812  
   PAGE 0 will be used to organize program sections
   PAGE 1 will be used to organize data sections

   Notes: 
         Memory blocks on F2812 are uniform (ie same
         physical memory) in both PAGE 0 and PAGE 1.  
         That is the same memory region should not be
         defined for both PAGE 0 and PAGE 1.
         Doing so will result in corruption of program 
         and/or data. 
*/

MEMORY
{
PAGE 0:    /* Program Memory */
           /* Memory (RAM/FLASH/OTP) blocks can be moved to PAGE1 for data allocation */

   ZONE0       : origin = 0x002000, length = 0x002000     /* XINTF zone 0 */
   ZONE1       : origin = 0x004000, length = 0x002000     /* XINTF zone 1 */
   RAML0       : origin = 0x008000, length = 0x001000     /* on-chip RAM block L0 */
   ZONE2       : origin = 0x080000, length = 0x080000     /* XINTF zone 2 */
   ZONE6       : origin = 0x100000, length = 0x080000     /* XINTF zone 6 */
   OTP         : origin = 0x3D7800, length = 0x000800     /* on-chip OTP */
   FLASHJ      : origin = 0x3D8000, length = 0x002000     /* on-chip FLASH */
   FLASHI      : origin = 0x3DA000, length = 0x002000     /* on-chip FLASH */
   FLASHH      : origin = 0x3DC000, length = 0x004000     /* on-chip FLASH */
   FLASHG      : origin = 0x3E0000, length = 0x004000     /* on-chip FLASH */
   FLASHF      : origin = 0x3E4000, length = 0x004000     /* on-chip FLASH */
   FLASHE      : origin = 0x3E8000, length = 0x004000     /* on-chip FLASH */
   FLASHD      : origin = 0x3EC000, length = 0x004000     /* on-chip FLASH */
   FLASHC      : origin = 0x3F0000, length = 0x004000     /* on-chip FLASH */
   FLASHA      : origin = 0x3F6000, length = 0x001FF6     /* on-chip FLASH */
   BEGIN       : origin = 0x3F7FF6, length = 0x000002     /* Part of FLASHA.  Used for "boot to Flash" bootloader mode. */
/* ZONE7       : origin = 0x3FC000, length = 0x003FC0     /* XINTF zone 7 available if MP/MCn=1 */ 
   ROM         : origin = 0x3FF000, length = 0x000FC0     /* Boot ROM available if MP/MCn=0 */
   RESET       : origin = 0x3FFFC0, length = 0x000002     /* part of boot ROM (MP/MCn=0) or XINTF zone 7 (MP/MCn=1) */
   VECTORS     : origin = 0x3FFFC2, length = 0x00003E     /* part of boot ROM (MP/MCn=0) or XINTF zone 7 (MP/MCn=1) */

PAGE 1 :   /* Data Memory */
           /* Memory (RAM/FLASH/OTP) blocks can be moved to PAGE0 for program allocation */
           /* Registers remain on PAGE1                                                  */

   RAMM0       : origin = 0x000000, length = 0x000400     /* on-chip RAM block M0 */
   RAMM1       : origin = 0x000400, length = 0x000400     /* on-chip RAM block M1 */
   DEV_EMU     : origin = 0x000880, length = 0x000180     /* Device emulation registers */
   FLASH_REGS  : origin = 0x000A80, length = 0x000060     /* FLASH registers */
   CSM         : origin = 0x000AE0, length = 0x000010     /* Code security module registers */
   XINTF       : origin = 0x000B20, length = 0x000020     /* External interface registers */
   CPU_TIMER0  : origin = 0x000C00, length = 0x000008     /* CPU Timer0 registers (CPU Timer1 and Timer2 are reserved for BIOS)*/
   CPU_TIMER2  : origin = 0x000C10, length = 0x000008     /* CPU Timer2 needed for this example */
   PIE_CTRL    : origin = 0x000CE0, length = 0x000020     /* PIE control registers */
   PIE_VECT    : origin = 0x000D00, length = 0x000100     /* PIE vector table */
   ECAN_A      : origin = 0x006000, length = 0x000100     /* eCAN registers */
   ECAN_AMBOX  : origin = 0x006100, length = 0x000100     /* eCAN mailboxes */
   SYSTEM      : origin = 0x007010, length = 0x000020     /* System control registers */
   SPI_A       : origin = 0x007040, length = 0x000010     /* SPI registers */
   SCI_A       : origin = 0x007050, length = 0x000010     /* SCI-A registers */
   XINTRUPT    : origin = 0x007070, length = 0x000010     /* External interrupt registers */
   GPIOMUX     : origin = 0x0070C0, length = 0x000020     /* GPIO mux registers */
   GPIODAT     : origin = 0x0070E0, length = 0x000020     /* GPIO data registers */
   ADC         : origin = 0x007100, length = 0x000020     /* ADC registers */
   EV_A        : origin = 0x007400, length = 0x000040     /* Event Manager A registers */
   EV_B        : origin = 0x007500, length = 0x000040     /* Event Manager B registers */
   SCI_B       : origin = 0x007750, length = 0x000010     /* SCI-B registers */
   MCBSP_A     : origin = 0x007800, length = 0x000040     /* McBSP registers */
   RAML1       : origin = 0x009000, length = 0x001000     /* on-chip RAM block L1 */
   FLASHB      : origin = 0x3F4000, length = 0x002000     /* on-chip FLASH */
   CSM_PWL     : origin = 0x3F7FF8, length = 0x000008     /* CSM password locations in FLASHA */
   RAMH0       : origin = 0x3F8000, length = 0x002000     /* on-chip RAM block H0 */
}

/* Allocate sections to memory blocks.
   Note:
         codestart user defined section in DSP28_CodeStartBranch.asm used to redirect code 
                   execution when booting to flash
         ramfuncs  user defined section to store functions that will be copied from Flash into RAM
*/ 
 
SECTIONS
{
 
   /* Allocate program areas: */
   .cinit              : > FLASHJ      PAGE = 0
   .text               : > FLASHJ      PAGE = 0
   codestart           : > BEGIN       PAGE = 0
   ramfuncs            : LOAD = FLASHD, RUN = RAML0, PAGE = 0


   /* Allocate uninitalized data sections: */
   .stack              : > RAMM0       PAGE = 1
   .bss                : > RAML1       PAGE = 1
   .ebss               : > RAML1       PAGE = 1
   .sysmem             : > RAMH0       PAGE = 1
   .esysmem            : > RAMH0       PAGE = 1

   /* Initalized sections go in Flash */
   .const              : > FLASHB      PAGE = 1
   .econst             : > FLASHB      PAGE = 1      

   /* Allocate IQ math areas: */
   IQmath              : > FLASHC      PAGE = 0                  /* Math Code */
   IQmathTables        : > ROM         PAGE = 0, TYPE = NOLOAD   /* Math Tables In ROM */

   /* .reset indicates the start of _c_int00 for C Code.  
   /* When using the boot ROM this section and the CPU vector
   /* table is not needed.  Thus the default type is set to 
   /* DESECT */ 
   .reset              : > RESET,      PAGE = 0, TYPE = DSECT
   vectors             : > VECTORS     PAGE = 0, TYPE = DSECT

/* ------------------------------------------------------------- */
/* The following allocations are required for the DSP28 Header file
   examples.  Each allocation maps a structure defined in the DSP28 
   header files to the memory location of those registers.  
*/
      
   /* Allocate Peripheral Frame 0 Register Structures:   */
   DevEmuRegsFile      : > DEV_EMU     PAGE = 1
   FlashRegsFile       : > FLASH_REGS  PAGE = 1
   CsmRegsFile         : > CSM         PAGE = 1
   XintfRegsFile       : > XINTF       PAGE = 1
   CpuTimer0RegsFile   : > CPU_TIMER0  PAGE = 1      
   CpuTimer2RegsFile   : > CPU_TIMER2  PAGE = 1
   PieCtrlRegsFile     : > PIE_CTRL    PAGE = 1      
   PieVectTable        : > PIE_VECT    PAGE = 1

   /* Allocate Peripheral Frame 2 Register Structures:   */
   ECanaRegsFile       : > ECAN_A      PAGE = 1   
   ECanaMboxesFile     : > ECAN_AMBOX  PAGE = 1

   /* Allocate Peripheral Frame 1 Register Structures:   */
   SysCtrlRegsFile     : > SYSTEM      PAGE = 1
   SpiaRegsFile        : > SPI_A       PAGE = 1
   SciaRegsFile        : > SCI_A       PAGE = 1
   XIntruptRegsFile    : > XINTRUPT    PAGE = 1
   GpioMuxRegsFile     : > GPIOMUX     PAGE = 1
   GpioDataRegsFile    : > GPIODAT     PAGE = 1
   AdcRegsFile         : > ADC         PAGE = 1
   EvaRegsFile         : > EV_A        PAGE = 1
   EvbRegsFile         : > EV_B        PAGE = 1
   ScibRegsFile        : > SCI_B       PAGE = 1
   McbspaRegsFile      : > MCBSP_A     PAGE = 1

   /* CSM Password Locations */
   CsmPwlFile          : > CSM_PWL     PAGE = 1

}
