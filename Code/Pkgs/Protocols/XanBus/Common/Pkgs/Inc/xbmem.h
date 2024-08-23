/*==============================================================================
Copyright 2003 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by 
copyright. The reproduction, in whole or in part, by anyone without the written 
approval of Xantrex is prohibited.

FILE NAME:  
    xbmem.h             - Interface to XBMEM package

PURPOSE:
    This file provide the interface to xbmem.c

FUNCTION(S):
    XBMEM_fnCtor        - Creator for XanBus Memory heap
    XBMEM_fnMalloc      - Allocate a block of memory from the heap
    XBMEM_fnFree        - Free a block of memory back to the heap
   
NOTES:

CHANGE HISTORY:
$Log: xbmem.h $

    ***********************************************
    Revision: XanBus_HollyZ/1
    User: HollyZ     Date: 06/30/05  Time: 04:52PM
    Added function prototype for XBMEM_fnDtor()

    ***********************************************
    Revision: GateWayPfrmANMEA_DaleM/1
    User: DaleM     Date: 02/10/05  Time: 07:23PM
    Cleaned up old history list
    
    
    
==============================================================================*/

#ifndef XBMEM_INCL
#define XBMEM_INCL


/*==============================================================================
                              Includes
==============================================================================*/


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

extern tucBOOL XBMEM_fnCtor( void *pvHeapStart,    // Pointer to memory for heap
                             uint16 uiHeapSize );  // Number of bytes for heap
extern void XBMEM_fnDtor( void );            
extern void *XBMEM_fnMalloc( uint16 NumBytes ); // Number of bytes to allocate

extern void XBMEM_fnFree( void *pvMemBlock );   // Memory pointer to release
extern void XBMEM_fnSetPriorityDobStatus( uchar8 status );
extern uchar8 XBMEM_fnGetPriorityDobStatus( void );
extern uint16 XBMEM_fnGetFreeBytes( void );

#endif  // XBMEM_INCL
