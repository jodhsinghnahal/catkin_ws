/*==============================================================================
Copyright 2005 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by 
copyright. The reproduction, in whole or in part, by anyone without the written 
approval of Xantrex is prohibited.

FILE NAME:  
    SysClk.h

PURPOSE:
    Provide access to set and retreive system clock speeds.

FUNCTION(S):
    SYSCLK_fnGetHiSpeedPeriphClk - Get high speed peripheral clock
    SYSCLK_fnSetHiSpeedPeriphClk - Set high speed peripheral clock
    SYSCLK_fnGetLoSpeedPeriphClk - Get low speed peripheral clock
    SYSCLK_fnSetLoSpeedPeriphClk - Set low speed peripheral clock
    SYSCLK_fnGetSysClk           - Get system clock
    SYSCLK_fnSetSysClk           - Set system clock
   
NOTES:
    
CHANGE HISTORY:
$Log: SysClk.h $

    ***********************************************
    Revision: NovaPfrmB_JohnB/2
    User: JohnB     Date: 01/24/05  Time: 09:16PM
    Updated header based on latest standards.
    

    ***********************************************
    Revision: NovaPfrmB_HorstM/2
    User: HorstM     Date: 01/13/05  Time: 09:16PM
    moved to new location

==============================================================================*/

#ifndef SYSCLK_H_INCL
#define SYSCLK_H_INCL


/*==============================================================================
                              Includes
==============================================================================*/
#include "LIB_stdtype.h"  // Standard types


/*==============================================================================
                              Defines
==============================================================================*/


/*==============================================================================
                            Type Definitions
==============================================================================*/


/*==============================================================================
                                Enums
==============================================================================*/


/*=============================================================================
                              Structures
==============================================================================*/


/*==============================================================================
                          External/Public Constants
==============================================================================*/


/*==============================================================================
                          External/Public Variables
==============================================================================*/


/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/

// Returns the Hi Speed Peripheral clock speed in hertz
uint32 SYSCLK_fnGetHiSpeedPeriphClk( void );

// Sets the Hi Speed Peripheral clock speed in hertz
void SYSCLK_fnSetHiSpeedPeriphClk( uint32 ClkHz );

// Returns the Low Speed Peripheral clock speed in hertz
uint32 SYSCLK_fnGetLoSpeedPeriphClk( void );

// Sets the Low Speed Peripheral clock speed in hertz
void SYSCLK_fnSetLoSpeedPeriphClk( uint32 ClkHz );

// Returns the System clock speed in hertz
uint32 SYSCLK_fnGetSysClk( void );

// Sets the System clock speed in hertz
void SYSCLK_fnSetSysClk( uint32 ClkHz );


#endif  // SYSCLK_H_INCL
