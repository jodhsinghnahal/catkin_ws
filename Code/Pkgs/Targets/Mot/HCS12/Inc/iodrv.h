/*=============================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International and protected by 
copyright. The reproduction, in whole or in part, by anyone without the written 
approval of Xantrex is prohibited.

FILE NAME:  
    IoDrv.h

PURPOSE:
    Package header and definitions for the I/O driver

FUNCTION(S):
    IODRV_fnSetPin  - Set a pin to a new state
    IODRV_fnGetPin  - Get the state of a pin
    IODRV_fnSetPort - Set a whole port
    IODRV_fnGetPort - Get a whole port
   
NOTES:   
    IODrvMap.h is the target specifics.  This allows for this interface to be
    generic.  tuiPORT and tuiPIN are defined there to allow for smaller or
    larger port sizes which can vary from target to target.

CHANGE HISTORY:
$History: iodrv.h $
    
    *****************  Version 6  *****************
    User: Ronm         Date: 3/30/04    Time: 3:10p
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/Baseline/HCS12/Packages/Inc
    - PRN 1387: updated copyright notice
    
    *****************  Version 5  *****************
    User: Alant        Date: 12/09/03   Time: 10:27a
    Updated in $/PD/Network_Extensions/Code/Common/HAL/Test/DIO
    - Made changes to match coding standard:
      - Renamed enum values to IODRV_e
      - Portmap.h renamed to IoDrvMap.h
      - tuiPORT and tuiPIN now have IODRVMAP_ package prefix
    
    *****************  Version 4  *****************
    User: Johnb        Date: 6/17/03    Time: 5:14p
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/Common/Services/IO/Test
    Updated headers and comments throughout.
    Removed include file LIB_stddefs.h
    Added IODRV_ package prefix where needed.
    Moved Device defines to portmap.h
    Moved tuiPORT and tuiPIN to portmap.h
    
    *****************  Version 3  *****************
    User: Karli        Date: 6/04/03    Time: 2:51p
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/Common/Inc
    Add Network Power Pin
    
    *****************  Version 2  *****************
    User: Johnb        Date: 4/14/03    Time: 5:03p
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/Common/Services/IO/Test
    Updated port and pin allocations for REMOTESD_PORT, REMOTESD_PIN,
    EXTERNSD_PORT, and  EXTERNSD_PIN.
    Updated comments.
    Updated package name prefix from IO to IODRV.
    
    *****************  Version 1  *****************
    User: Jasonc       Date: 3/13/03    Time: 3:51p
    Created in $/PD/Inverter_Charger/Nova_Series/Code/Common/Inc
    Created
    

==============================================================================*/

#ifndef IODRV_H_INCL
#define IODRV_H_INCL

/*==============================================================================
                              Includes
==============================================================================*/
#include "LIB_stdtype.h"	// Requires and architecture defn. in the compiler
#include "iodrvmap.h"       // Processor specific port and pin mappings

/*=============================================================================
                              Macros
==============================================================================*/

// Test pin specified by msk.  Returns true if pin high, false otherwise
#define IODRV_mPIN_TEST( dest, msk )   ( ( dest & msk ) == ( msk ) )

// Set pin high specified by msk in dest
#define IODRV_mPIN_SET(  dest, msk )   ( ( dest ) |= ( msk ) )

// Set pin low specified by msk in dest
#define IODRV_mPIN_CLR(  dest, msk )   ( ( dest ) &= ( ~msk ) )


/*==============================================================================
                              Defines
==============================================================================*/
//None

/*==============================================================================
                            Type Definitions
==============================================================================*/
//None

/*==============================================================================
                                Enums
==============================================================================*/
// Possible pin states, either low or high
typedef enum ePIN_STATE
{
    IODRV_ePIN_LOW = 0,
    IODRV_ePIN_HIGH
    
} IODRV_tePIN_STATE;

/*==============================================================================
                          External/Public Constants
==============================================================================*/
//None

/*==============================================================================
                          External/Public Variables
==============================================================================*/
//None

/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/
// Set a pin either high or low
void IODRV_fnSetPin( IODRVMAP_tuiPORT  uiPort,               // The port
                     IODRVMAP_tuiPIN   uiPin,                // The pin
                     IODRV_tePIN_STATE eState );             // New pin state
// Get a pin's state
IODRV_tePIN_STATE IODRV_fnGetPin( IODRVMAP_tuiPORT uiPort,   // The port
                                  IODRVMAP_tuiPIN  uiPin );  // The pin
                                
//Set an entire port
void IODRV_fnSetPort( IODRVMAP_tuiPORT uiPort ,              // Port Effected.
                      IODRVMAP_tuiPORT uiVal  );             // Value to write
//Get an entire port
IODRVMAP_tuiPORT IODRV_fnGetPort( IODRVMAP_tuiPORT uiPort ); // The port

#endif  // IODRV_H_INCL