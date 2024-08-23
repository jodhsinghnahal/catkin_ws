/*==============================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:
    config.h

PURPOSE:
    Allow portability between different hardware setups based on a family of
    CPU's.  Allows for the software to built based on a specific configuration
    usually based on hardware.

FUNCTION(S):
    None

NOTES:
    One of the CTRL options needs to be defined by the user either in there
    project setup or by a #define.

    Only 1 option must be defined either in your project options or by a
    #define somewhere in your code.  If you need more options, just add them,
    don't change the ones that already exist.
      For example:
        CTRL_CONFIG_OPTION_0
        CTRL_CONFIG_OPTION_1
        CTRL_CONFIG_OPTION_2
        CTRL_CONFIG_OPTION_3
        CTRL_CONFIG_OPTION_4
        CTRL_CONFIG_OPTION_5
        CTRL_CONFIG_OPTION_6
        CTRL_CONFIG_OPTION_7

CHANGE HISTORY:
$Log: config.h $

    ***********************************************
    Revision: NovaPfrmB_Karli/1
    User: Karli     Date: 02/19/05  Time: 02:03AM
    Modified PLL multiplier on CTRL_CONFIG_OPTION_7 (for the ACB) to 5 to reflect
    actual desired clock rate.
    

    ***********************************************
    Revision: NovaPfrmB_TrevorM/2
    User: TrevorM     Date: 02/15/05  Time: 09:51PM
    Modified PLL multiplier on CTRL_CONFIG_OPTION_6 (for the eZDSP environment)
    to 5 to run the development board at the same clock speed as the ACB.
    

    ***********************************************
    Revision: NovaPfrmB_TrevorM/1
    User: TrevorM     Date: 02/09/05  Time: 03:42PM
    Added CTRL_EXTMEM_SPEED #define to CTRL_CONFIG_OPTION_6
    to support external memory address wait state configuration
    on the eZDSP development kit
    

    ***********************************************
    Revision: NovaPfrmB_HorstM/2
    User: HorstM     Date: 02/03/05  Time: 12:50AM
    added defines for 25xx320 & 25xx640 eeprom devices;
    added 25xx640 to CTRL_CONFIG_OPTION_6 for eedevice
    for FBT development
    

    ***********************************************
    Revision: NovaPfrmB_JohnB/3
    User: JohnB     Date: 01/24/05  Time: 09:14PM
    Updated for use with SYSCLK package. Removed INIT_fnGetSysClk()
    functionality.


    ***********************************************
    Revision: NovaPfrmB_HorstM/1
    User: HorstM     Date: 01/19/05  Time: 11:04PM
    changed CTRL_CONFIG_PLL from 5 back to 3 for use on
    the ezDSP board


    ***********************************************
    Revision: NovaPfrmB_JohnB/2
    User: JohnB     Date: 01/14/05  Time: 12:58AM
    Updated PLL from 3 to 5 to make 150MHz. Removed file
    version history.


==============================================================================*/

#ifndef CONFIG_INCL
#define CONFIG_INCL


/*==============================================================================
                              Includes
==============================================================================*/
#include "LIB_stddefs.h"    //Standard defintions for all projects
#include "Manufacturers.h"  //List of device manufacturers

/*==============================================================================
                              Defines
==============================================================================*/
// All configurations in this file are by TI.
#define CTRL_CONFIG_MANUFACTURER ( MANUFACTURER_TEXAS_INSTRUMENTS )


// Define the possible CPUs for TMS320C24x DSP Generation
#define CTRL_CPU_TI_INVL            ( 0  )//Invalid selection
#define CTRL_CPU_TI_SM320F240       ( 1  )
#define CTRL_CPU_TI_SMJ320F240      ( 2  )
#define CTRL_CPU_TI_TMS320C242      ( 3  )
#define CTRL_CPU_TI_TMS320F240      ( 4  )
#define CTRL_CPU_TI_TMS320F241      ( 5  )
#define CTRL_CPU_TI_TMS320F243      ( 6  )
#define CTRL_CPU_TI_TMS320LC2401A   ( 7  )
#define CTRL_CPU_TI_TMS320LC2402A   ( 8  )
#define CTRL_CPU_TI_TMS320LC2404A   ( 9  )
#define CTRL_CPU_TI_TMS320LC2406A   ( 10 )
#define CTRL_CPU_TI_TMS320LF2401A   ( 11 )
#define CTRL_CPU_TI_TMS320LF2402A   ( 12 )
#define CTRL_CPU_TI_TMS320LF2403A   ( 13 )
#define CTRL_CPU_TI_TMS320LF2406A   ( 14 )
#define CTRL_CPU_TI_TMS320LF2407A   ( 15 )

#define CTRL_CPU_TI_TMS320LF2812A_EZDSP ( 20 ) //For the 2812 eZdsp
#define CTRL_CPU_TI_TMS320LF2812A       ( 21 )

// Define the possible EEPROMs for control board
#define CTRL_EEDRV_INVL                  ( 0 )// Invalid selection
#define CTRL_EEDRV_MICROCHIP_MC25LC040   ( 1 )
#define CTRL_EEDRV_MICROCHIP_MC25AA080   ( 2 )
#define CTRL_EEDRV_MICROCHIP_MC25AA160   ( 3 )
#define CTRL_EEDRV_MICROCHIP_MC25AA320   ( 4 )
#define CTRL_EEDRV_MICROCHIP_MC25AA640   ( 5 )

/*^^^^^^^^^^^^
Setup Selction
^^^^^^^^^^^^*/
// Define the macros for program use based on board configurations
#if( defined( CTRL_CONFIG_OPTION_0 ) )
// Texas Instruments TMS320LF2407A EVM configuration
#define CTRL_CONFIG_CPU     ( CTRL_CPU_TI_TMS320LF2407A )//40 MIPs
#define CTRL_CONFIG_CRYSTAL ( 10000000L                 )//Hz
#define CTRL_CONFIG_EEDRV   ( CTRL_EEDRV_INVL           )
#define CTRL_CPU_MAXSPEED   ( 40000000L                 )//40 MIPs
#define CTRL_CPU_NAME       ( "TMS320LF2407A-eZdsp"     )

#elif( defined( CTRL_CONFIG_OPTION_1 ) )
// Nova Control Board configuration
#define CTRL_CONFIG_CPU     ( CTRL_CPU_TI_TMS320LF2407A      )//40 MIPs
#define CTRL_CONFIG_CRYSTAL ( 10000000L                      )//Hz
#define CTRL_CONFIG_EEDRV   ( CTRL_EEDRV_MICROCHIP_MC25AA160 )
#define CTRL_CPU_MAXSPEED   ( 40000000L                      )//40 MIPs
#define CTRL_CPU_NAME       ( "TMS320LF2407A-CtrlBrd-Rev5"   )

#elif( defined( CTRL_CONFIG_OPTION_2 ) )
// Nova Control Board configuration
#define CTRL_CONFIG_CPU     ( CTRL_CPU_TI_TMS320LF2407A      )//40 MIPS
#define CTRL_CONFIG_CRYSTAL ( 10000000L                      )//Hz
#define CTRL_CONFIG_EEDRV   ( CTRL_EEDRV_MICROCHIP_MC25AA160 )
#define CTRL_CPU_MAXSPEED   ( 40000000L                      )
#define CTRL_CPU_NAME       ( "TMS320LF2407A-CtrlBrd-Rev7"   )

#elif( defined( CTRL_CONFIG_OPTION_3 ) )
// Nova Control Board configuration
#define CTRL_CONFIG_CPU     ( CTRL_CPU_TI_TMS320LF2407A      )//40 MIPS
#define CTRL_CONFIG_CRYSTAL ( 10000000L                      )//Hz
#define CTRL_CONFIG_EEDRV   ( CTRL_EEDRV_MICROCHIP_MC25AA160 )
#define CTRL_CPU_MAXSPEED   ( 40000000L                      )
#define CTRL_CPU_NAME       ( "TMS320LF2407A-CtrlBrd-Rev8"   )

#elif( defined( CTRL_CONFIG_OPTION_4 ) )
// Nova Control Board configuration considered as C-Model boards
#define CTRL_CONFIG_CPU     ( CTRL_CPU_TI_TMS320LF2407A      )//40 MIPS
#define CTRL_CONFIG_CRYSTAL ( 10000000L                      )//Hz
#define CTRL_CONFIG_EEDRV   ( CTRL_EEDRV_MICROCHIP_MC25AA160 )
#define CTRL_CPU_MAXSPEED   ( 40000000L                      )
#define CTRL_CPU_NAME       ( "TMS320LF2407A-CtrlBrd-RevA"   )

#elif( defined( CTRL_CONFIG_OPTION_5 ) )
// Nova Control Board configuration considered as C-Model
#define CTRL_CONFIG_CPU     ( CTRL_CPU_TI_TMS320LF2407A      )//40 MIPS
#define CTRL_CONFIG_CRYSTAL ( 10000000L                      )//Hz
#define CTRL_CONFIG_EEDRV   ( CTRL_EEDRV_MICROCHIP_MC25AA160 )
#define CTRL_CPU_MAXSPEED   ( 40000000L                      )
#define CTRL_CPU_NAME       ( "TMS320LF2407A-CtrlBrd-RevA-CC02" )

#elif( defined( CTRL_CONFIG_OPTION_6 ) )
//Texas Instruments TMS320LF2812A EVM configuration (AKA the eZdsp)
//This setup is specific for te eZdsp from  Spectrum Digital
#define CTRL_CONFIG_CPU     ( CTRL_CPU_TI_TMS320LF2812A_EZDSP )
#define CTRL_CONFIG_CRYSTAL ( 30000000L                       )//Hz
#define CTRL_CONFIG_PLL     ( 5L                              )//PLL multiplier
#define CTRL_CONFIG_EEDRV   ( CTRL_EEDRV_MICROCHIP_MC25AA640                 )
#define CTRL_CPU_MAXSPEED   ( CTRL_CONFIG_CRYSTAL * CTRL_CONFIG_PLL )
#define CTRL_CPU_NAME       ( "TMS320LF2812A-eZdsp"           )
#define CTRL_EXTMEM_SPEED   ( MEM_SPEED_10_NS )

#elif( defined( CTRL_CONFIG_OPTION_7 ) )
// Nova Advanced Control Board configuration.
// 
#define CTRL_CONFIG_CPU     ( CTRL_CPU_TI_TMS320LF2812A      )
#define CTRL_CONFIG_CRYSTAL ( 30000000L                      )//Hz
#define CTRL_CONFIG_PLL     ( 5L                             )//PLL multiplier
#define CTRL_CONFIG_EEDRV   ( CTRL_EEDRV_MICROCHIP_MC25AA160 )
#define CTRL_CPU_MAXSPEED   ( CTRL_CONFIG_CRYSTAL * CTRL_CONFIG_PLL )
#define CTRL_CPU_NAME       ( "TMS320LF2812A-CtrlBrd-RevA"   )
#else
// No configuration so show an error msg at compile time.
#error No configuration selected (config.h)
#endif

//The speed must be a multiple of the crystal speed since the PLL
//accepts whole numbers only.
#if( ( CTRL_CPU_MAXSPEED % CTRL_CONFIG_CRYSTAL ) != 0 )
#error CTRL_CPU_MAXSPEED must be multiple of CTRL_CONFIG_CRYSTAL
#endif

#define XT_BITS_PER_BYTE        ( 8 )

// Macro for converting value to an 8 bit value
#define XT_mMK_BYTE(x)          ( ( x ) & 0xFF )

/*==============================================================================
                            Type Definitions
==============================================================================*/


/*=============================================================================
                              Structures
==============================================================================*/

#endif //CONFIG_INCL
