/*==============================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International and protected by 
copyright. The reproduction, in whole or in part, by anyone without the written 
approval of Xantrex is prohibited.

FILE NAME:  
    xassert.h

PURPOSE:
    To perform an assert operation that can work in an embedded system.

NOTES:
    The name of the assert define is NOT xassert as per the file name to
    maintain portability to any other system that uses the ANSI assert.

CHANGE HISTORY:
$Log: xassert.h $

    ***********************************************
    Revision: XanBus_HollyZ/1
    User: HollyZ     Date: 10/11/07  Time: 12:02AM
    XanBus Library V1.04.00 BN0727

    ***********************************************
    Revision: Xantrex1_JohnB/6
    User: JohnB     Date: 12/06/04  Time: 06:33PM
    Updated header information.
    

==============================================================================*/

#ifndef XASSERT_H
#define XASSERT_H


/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/
void SYS_fnAbort( char *warning ); // must implement in project specific code

/*==============================================================================
                              Macros
==============================================================================*/
#define _STR(x)  __STR(x)
#define __STR(x) #x

#if defined( NDEBUG )
#define assert(_ignore) ((void)0)
#else
#define assert(_expr) ((_expr)?(void)0:(void)SYS_fnAbort(0))

#endif

#endif  // XASSERT_INCL 
