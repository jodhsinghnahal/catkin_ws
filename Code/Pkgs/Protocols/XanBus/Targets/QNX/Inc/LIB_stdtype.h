/*==============================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International and protected by 
copyright. The reproduction, in whole or in part, by anyone without the written 
approval of Xantrex is prohibited.

FILE NAME:  
    LIB_stdtype.h

PURPOSE:
    Architecture independant data definitions

FUNCTION(S):
    None

NOTES:    
    The user must predefine the preprocessor symbol _xxBIT_ARCHITECTURE
    in the project to the appropriate value for the target.

CHANGE HISTORY:
$Log: LIB_stdtype.h $

    ***********************************************
    Revision: Xantrex1_JohnB/4
    User: JohnB     Date: 12/06/04  Time: 06:33PM
    Updated header information.
    

==============================================================================*/

#ifndef LIB_STDTYPE_H
#define LIB_STDTYPE_H


/*==============================================================================
                              Includes
==============================================================================*/

// Use the generated config.h to determine the cpu architecture
#include <config.h>

#if ( SIZEOF_INT == 1 )
#define _8BIT_ARCHITECTURE
#elif ( SIZEOF_INT == 2 )
#define _16BIT_ARCHITECTURE
#elif ( SIZEOF_INT == 4 )
#define _32BIT_ARCHITECTURE
#endif

//Define types based on processor architecture
#if defined( _8BIT_ARCHITECTURE )
#include "LIB_stdtype8.h"

#elif defined( _16BIT_ARCHITECTURE )
#include "LIB_stdtype16.h"

#elif defined(  _32BIT_ARCHITECTURE )
#include "LIB_stdtype32.h"

#else
#error ARCHITECTURE not defined
#endif


#endif // LIB_STDTYPE_INCL
