/*==============================================================================
Copyright 2005 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:  
    spiclient.h

PURPOSE:
    Provide public access to operating system specific SPI functions.

FUNCTION(S):
    SPICLIENT_fnCtor            - SPI client construction
    SPICLIENT_fnSignalStartTxRx - indicating a start of communication process
    SPICLIENT_fnWaitForTxRxComplete - waiting for communication process 
                                        to finish
    SPICLIENT_fnTxRxComplete    - Complete a spi communication process
    SPICLIENT_fnDtor            - SPI client destruction
    SPICLIENT_fnAcquire         - Acquire exclusive access to the SPI hardware
    SPICLIENT_fnRelease         - Release exclusive access to the SPI hardware
                   
    local:
        None
           
NOTES:
        None

CHANGE HISTORY:
$Log: spiclient.h $

   ***********************************************
   Revision: NovaPfrmB_TrevorM/1
   User: TrevorM     Date: 03/11/05  Time: 11:51PM
   modified to use SPI driver handle

    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/2
    User: BaldeeshK     Date: 02/04/05  Time: 10:56PM
    - Code review changes 04/02/2005.  Added comments and
    changed #ifndef label
    

    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/1
    User: BaldeeshK     Date: 12/08/04  Time: 05:01PM
    - Initial version for Accurev
    
    
   
==============================================================================*/
#ifndef SPICLIENT_H
#define SPICLIENT_H


/*==============================================================================
                              Includes
==============================================================================*/
#include "LIB_stdtype.h"    //Requres and architecture defintion in the compiler
#include "LIB_stddefs.h"    //Standard defintions for all projects

 
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
                          External/Public Variables
==============================================================================*/


/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/
// SPICLIENT_fnCtor
//       SPI client construction
//
//Input: none
//Output: tuiSTATUS
tuiSTATUS SPICLIENT_fnCtor( void );

// SPICLIENT_fnSignalStartTxRx
//       indicating a start of communication process
//
//Input: none
//Output: tuiSTATUS
tuiSTATUS SPICLIENT_fnSignalStartTxRx( void );

// SPICLIENT_fnWaitForTxRxComplete
//       waiting for communication process to finish
//
//Input: none
//Output: tuiSTATUS
tuiSTATUS SPICLIENT_fnWaitForTxRxComplete( void );

// SPICLIENT_fnTxRxComplete
//       Complete a spi communication process
//
//Input: none
//Output: none
void SPICLIENT_fnTxRxComplete( void );

// SPICLIENT_fnDtor
//       SPI client destruction
//
//Input: none
//Output: tuiSTATUS
tuiSTATUS SPICLIENT_fnDtor( void );

// SPICLIENT_fnAcquireLock
//      SPI client acquire exclusive access
//
//Input: siWaitTicks, number of ticks to wait for exclusive access
//                    0 = wait forever
//                    negative => try to acquire without blocking
//Output: tuiStatus - eSTATUS_ERR - a timeout or other error occurred
//                  - eSTATUS_OK - access to the SPI hardware has been granted
tuiSTATUS SPICLIENT_fnAcquireLock( sint16 siWaitTicks );

// SPICLIENT_fnReleaseLock
//      SPI client release exclusive access
//Input: none
//Output: none
void SPICLIENT_fnReleaseLock( void );
 
#endif  // SPICLIENT_H

