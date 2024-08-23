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
    Revision: NovaPfrmB_BaldeeshK/2
    User: BaldeeshK     Date: 12/08/04  Time: 05:11PM
    - Initial add to Accurev of spi driver test files
    
    
    *****************  Version 25  *****************
    User: Johnb        Date: 6/16/04    Time: 3:05p
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/Baseline/C24x/Inc
    Added configuration options 6 and 7 for the 28x implementation.
    Added a PLL multiplied for the 6 and 7th options.
    Added an error check for the verification on xtal speed and max cpu
    speed
    
    *****************  Version 24  *****************
    User: Johnb        Date: 4/08/04    Time: 3:04p
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/Baseline/C24x/Packages/FilterScale/Test/FilterScale
    PRN1163
    - updated copy right
    - added option 5
    
    *****************  Version 23  *****************
    User: Johnb        Date: 1/05/04    Time: 12:09p
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/Baseline/C24x/Inc
    Changed "TMS320LF2407A-CtrlBrd-Rev9" to "TMS320LF2407A-CtrlBrd-RevA"
    
    *****************  Version 22  *****************
    User: Johnb        Date: 12/12/03   Time: 6:12p
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/Baseline/C24x/Inc
    Updated config option 4 with details.
    
    *****************  Version 21  *****************
    User: Peteran      Date: 10/25/03   Time: 10:52a
    Updated in $/PD/Common/Targets/TI/c24x/HAL/Test/Init
    Setup of CONTROL_CONFIG_OPTION_3 (PRN 431)
    
    *****************  Version 20  *****************
    User: Alant        Date: 16/09/03   Time: 2:45p
    Updated in $/PD/Common/FieldReprogram/Loader/Code/TI/c24x/BootBlock/Test
    - Replaced incorrect version with previous.
    
    *****************  Version 18  *****************
    User: Peteran      Date: 9/12/03    Time: 9:36a
    Updated in $/PD/Common/Targets/TI/c24x/HAL/Test/Init
    Updated CPU name in CTRL_CONFIG_OPTION2  to "TMS320LF2407A-CtrlBrd-Rev7
    (PRN241)
    
    *****************  Version 17  *****************
    User: Johnb        Date: 7/04/03    Time: 4:58p
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/Common/Inc
    Removed unused defines.
    Updated CTRL_EEDRV_MICROCHIP_INVL to CTRL_EEDRV_INVL.
    Added "L" to big numbers that needed it.
    Updated the "vanity" level of the file.
    
    *****************  Version 16  *****************
    User: Johnb        Date: 6/24/03    Time: 5:26p
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/Common/DigCtrl/InvCtrl/Test/SineWaveGen
    Changed ALL occurances of EEPROM to EEDRV (fn, defines, variables, etc)
    
    *****************  Version 15  *****************
    User: Johnb        Date: 6/12/03    Time: 4:39p
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/Common/Inc
    Added config option parameters.
    Added Rev to CPU name.
    
    *****************  Version 13  *****************
    User: Karli        Date: 4/25/03    Time: 11:24a
    Updated in $/PD/Common/Targets/TI/c24x/HAL/Inc
    Redo CTRL_CONFIG_CPU as defines rather than enums
    
    *****************  Version 12  *****************
    User: Karli        Date: 4/24/03    Time: 8:26p
    Updated in $/PD/Common/Targets/Microchip/25AAxxx/Test/ezDSP
    Add definitions for EEPROM device.
    
    *****************  Version 11  *****************
    User: Alant        Date: 3/04/03    Time: 10:13a
    Updated in $/PD/Common/Targets/TI/c24x/HAL/Test/BootBlock
    - Renamed manufacturer enum to define to be ANSI compliant.
    
    *****************  Version 10  *****************
    User: Alant        Date: 31/03/03   Time: 2:53p
    Updated in $/PD/Common/Targets/TI/c24x/HAL/Inc
    - Added global define of the manufacturer as TI using the
    Manufacturer.H include file.
    
    *****************  Version 9  *****************
    User: Jasonc       Date: 2/28/03    Time: 5:28p
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/RV_Series/R3000/Inc
    Fixed defines

Date:   Nov 1 2002   By: John Bellini
    - Borrowed from XDI project.
    - Changed for Nova compatibility
    
Date:  Nov 20 2002  By: John Bellini
    - Created generic options that are now required to be defined by the user.
    
Date:  DEC-20-2002  By: Karl Isham
    - Added CPU name string for OS View and disabled all options execpt 0 

Date:  Jan 16 2003  By: Ron Mok
    - If a control configuration is not selected, an error msg is displayed at
      compile time instead of defaulting to a pre-defined configuration. 


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
#define CTRL_CONFIG_PLL     ( 3                               )//PLL multiplier
#define CTRL_CONFIG_EEDRV   ( CTRL_EEDRV_INVL                 )
#define CTRL_CPU_MAXSPEED   ( CTRL_CONFIG_CRYSTAL * CTRL_CONFIG_PLL )
#define CTRL_CPU_NAME       ( "TMS320LF2812A-eZdsp"           )


#elif( defined( CTRL_CONFIG_OPTION_7 ) )
// Nova Advanced Control Board configuration.  
// Requires 80Mips minimum.
#define CTRL_CONFIG_CPU     ( CTRL_CPU_TI_TMS320LF2812A      )
#define CTRL_CONFIG_CRYSTAL ( 30000000L                      )//Hz
#define CTRL_CONFIG_PLL     ( 3                              )//PLL multiplier
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


/*==============================================================================
                            Type Definitions
==============================================================================*/


/*=============================================================================
                              Structures
==============================================================================*/

#endif //CONFIG_INCL
