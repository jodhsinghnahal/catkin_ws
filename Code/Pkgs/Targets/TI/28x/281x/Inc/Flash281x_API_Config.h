// TI File $Revision:: 1    $
//###########################################################################
//
// FILE:  Flash281x_API_Config.h
//
// TITLE: F281x Flash Algo's - User Settings
//
// NOTE:  This file contains user defined settings that
//        are used by the F281x Flash APIs.
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  1.00| 10 Dec 2003 | TI   | TMS Release
//###########################################################################

#ifndef FLASH281X_API_CONFIG_H
#define FLASH281X_API_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

// Variables that can be configured by the user. 

/*-----------------------------------------------------------------------------
   1. Ensure the proper processor type is defined in your project settings  
-----------------------------------------------------------------------------*/

#ifdef  PROCESSOR_F2810
#define FLASH_F2810   1
#else
#define FLASH_F2810   0 
#endif

#ifdef  PROCESSOR_F2811
#define FLASH_F2811   1
#else
#define FLASH_2811    0
#endif

#ifdef PROCESSOR_F2812
#define FLASH_F2812   1
#else
#define FLASH_F2812   0
#endif

/*-----------------------------------------------------------------------------
   2. Specify the clock rate of the CPU (SYSCLKOUT) in nS.

      Take into account the input clock frequency and the PLL multiplier
      selected in step 1.
 
      Use one of the values provided, or define your own.
      The trailing L is required tells the compiler to treat 
      the number as a 64-bit value.  

      Only one statement should be uncommented.

      Example:  CLKIN is a 30MHz crystal. 
 
                In step 1 the user specified PLLCR = 0xA for a 
                150Mhz CPU clock (SYSCLKOUT = 150MHz).  

                In this case, the CPU_RATE will be 6.667L
                Uncomment the line:  #define CPU_RATE  6.667L   
-----------------------------------------------------------------------------*/

#define CPU_RATE    6.667L   // for a 150MHz CPU clock speed (SYSCLKOUT)
//#define CPU_RATE    7.143L   // for a 140MHz CPU clock speed (SYSCLKOUT)
//#define CPU_RATE    8.333L   // for a 120MHz CPU clock speed (SYSCLKOUT)
//#define CPU_RATE   10.000L   // for a 100MHz CPU clock speed (SYSCLKOUT)
//#define CPU_RATE   13.330L   // for a 75MHz CPU clock speed (SYSCLKOUT)
//#define CPU_RATE   20.000L   // for a 50MHz CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE   33.333L   // for a 30MHz CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE   41.667L   // for a 24MHz CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE   50.000L   // for a 20MHz CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE   66.667L   // for a 15MHz CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE  100.000L   // for a 10MHz CPU clock speed  (SYSCLKOUT)

//----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// **** DO NOT modify the code below this line ****
//-----------------------------------------------------------------------------
#define SCALE_FACTOR  1048576.0L*( (200L/CPU_RATE) )  // IQ20


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif // -- end FLASH281X_API_CONFIG_H 
