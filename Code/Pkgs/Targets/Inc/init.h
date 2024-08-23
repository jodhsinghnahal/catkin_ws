/*=============================================================================
Copyright 2004 Xantrex Technology Inc.  All rights reserved.

This source file is proprietary to Xantrex Technology Inc. and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:  init.h
PURPOSE:
    Header file for init.c

FUNCTION(S):
    INIT_fnSysInit - public DSP initialization function
    INIT_fnInitIOConfig( void )
    INIT_fnEnableInterrupts( void ) - Public Interrupt Enable Function
    
NOTES:

CHANGE HISTORY:
$Log: init.h $

    ***********************************************
    Revision: NovaPfrmB_HorstM/2
    User: HorstM     Date: 01/26/05  Time: 07:54PM
    removed declaration of INIT_fnGetClkSpeed
    

    ***********************************************
    Revision: NovaPfrmB_HorstM/1
    User: HorstM     Date: 12/07/04  Time: 09:13PM
    added new function(s)
    
    
Date:  JUN-8-2004 - Brent Tokarchuk
    -   Created

=============================================================================*/

#ifndef INIT_INCL
#define INIT_INCL

/*=============================================================================
                              Includes
=============================================================================*/
#include "LIB_stdtype.h"	//Requres and architecture defintion in the compiler
#include "LIB_stddefs.h"    //Standard defintions for all projects

/*=============================================================================
                              Defines
=============================================================================*/


/*=============================================================================
                            Type Definitions
=============================================================================*/


/*=============================================================================
                              Structures
=============================================================================*/


/*=============================================================================
                                Enums
=============================================================================*/

/*=============================================================================
                               Constants
=============================================================================*/


/*=============================================================================
                               Variables
=============================================================================*/


/*=============================================================================
                            Function Protoypes
=============================================================================*/

void INIT_fnSysInit( void );
void INIT_fnInitIOConfig();
void INIT_fnEnableInterrupts( void );

#endif //INIT_INCL
