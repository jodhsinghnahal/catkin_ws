/*==============================================================================
Copyright 2005 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International and protected by 
copyright. The reproduction, in whole or in part, by anyone without the written 
approval of Xantrex is prohibited.

FILE NAME: sciio.h

PURPOSE:
    To provide a means of interfacing between the user and the product. 
    
FUNCTION(S):
    SCIIO_fnCtor - Initialize the SCI I/O module
    
NOTES:
    This module registers a new IO driver with the stdio subsystem via the
    add_device function.  This module also implements a number of standard
    input/output callback functions which read/write or otherwise access
    the lower level SCI driver.  This abstracts the SCIDRV functionality,
    and lets the application layer perform device input/output using the
    stdio functions.
    
CHANGE HISTORY :
$Log: sciio.h $
    
==============================================================================*/
#ifndef SCIIO_H
#define SCIIO_H

/*==============================================================================
                              Includes
==============================================================================*/
#include "LIB_stddefs.h"  // Standard library definitions
#include <stdio.h>        // for FILE handle definition
#include "scidrv.h"       // for SCIDRV enumerations

/*==============================================================================
                            Type Definitions
==============================================================================*/

/*==============================================================================
                                Enums
==============================================================================*/

/*==============================================================================
                              Defines
==============================================================================*/

/*=============================================================================
                              Structures
==============================================================================*/

/*==============================================================================
                          External/Public Constants
==============================================================================*/

/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/
// Set up the command package. This Ctor calls the SCI Ctor and starts 
// a task to monitor the SCI for incoming commands.
tuiSTATUS SCIIO_fnCtor( SCIDRV_teBAUD_RATE BaudRate,
                    SCIDRV_tePARITY    Parity,
                    SCIDRV_teDATA_BITS DataBits,
                    SCIDRV_teSTOP_BITS StopBits, 
                    FILE **ppInput,
                    FILE **ppOutput);
                                       
#endif // SCIIO_H

