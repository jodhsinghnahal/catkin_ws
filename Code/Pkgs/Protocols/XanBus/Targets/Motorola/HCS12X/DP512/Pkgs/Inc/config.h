/*==============================================================================
Copyright 2005 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by 
copyright. The reproduction, in whole or in part, by anyone without the written 
approval of Xantrex is prohibited.

FILE NAME:  
    config.h

PURPOSE:
    The file includes system/hardware level defines that may be specific to a 
    particular project. 

FUNCTION(S):
     none
   
    local:
     none
   
NOTES:

    
HISTORY:
$Log: config.h $

    ***********************************************
    Revision: XanBus_DaleM/2
    User: DaleM     Date: 02/23/05  Time: 01:31AM
    Needed to add MANUFACTURER_MOTOROLA for candrv.h.

    
    
    ************************************************
    User: Dalem        Date: 010705   Time: 2:302p
    Created for use by XanBus library
    
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

#define CTRL_CONFIG_MANUFACTURER ( MANUFACTURER_MOTOROLA )

#define XT_BITS_PER_BYTE        ( 8 )

// Macro for converting value to an 8 bit value
#define XT_mMK_BYTE(x)          ( x )

/*==============================================================================
                              Includes
==============================================================================*/

#endif  // CONFIG_INCL
