/*==============================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International and protected by 
copyright. The reproduction, in whole or in part, by anyone without the written 
approval of Xantrex is prohibited.

FILE NAME:  
    LIB_stddefs.h

PURPOSE:
    This is a header file containing all the data definitions to be used 
    as common entities in all Xantrex projects.

FUNCTION(S):
    None

NOTES:
    Depnding on the architecture of the device, one of the following
    MUST be defined for this file the compile properly:
    
     _8BIT_ARCHITECTURE
    _16BIT_ARCHITECTURE
    _32BIT_ARCHITECTURE

CHANGE HISTORY:
$Log: LIB_stddefs.h $

    ***********************************************
    Revision: Xantrex1_JohnB/4
    User: JohnB     Date: 12/06/04  Time: 06:33PM
    Updated header information.
    

==============================================================================*/
 
#ifndef LIB_STDDEFS_H
#define LIB_STDDEFS_H

/*==============================================================================
                              Includes
==============================================================================*/
#include "LIB_stdtype.h"    //Requres and architecture defintion in the compiler


/*==============================================================================
                              Defines
==============================================================================*/

#if defined(__HIWARE__)         // If compiler is for HCS12
#if defined(__BIG_ENDIAN__)     // If compiler uses integers in Big Endian order
#define XT_BIG_ENDIAN           // Define Xantrex Big Endian
#endif
#endif

#ifndef NULL
#define NULL                   ((void *) 0)
#endif

#ifndef FALSE
#define FALSE  0
#endif

#ifndef TRUE
#define TRUE   ( !FALSE )
#endif

#define  OFF    0
#define  ON     ( !OFF )

#define  NO     0
#define  YES    ( !NO )

//BIT positions
#define XT_BITPOS0  0
#define XT_BITPOS1  1
#define XT_BITPOS2  2
#define XT_BITPOS3  3
#define XT_BITPOS4  4
#define XT_BITPOS5  5
#define XT_BITPOS6  6
#define XT_BITPOS7  7
#define XT_BITPOS8  8
#define XT_BITPOS9  9
#define XT_BITPOS10 10
#define XT_BITPOS11 11
#define XT_BITPOS12 12
#define XT_BITPOS13 13
#define XT_BITPOS14 14
#define XT_BITPOS15 15
#define XT_BITPOS16 16
#define XT_BITPOS17 17
#define XT_BITPOS18 18
#define XT_BITPOS19 19
#define XT_BITPOS20 20
#define XT_BITPOS21 21
#define XT_BITPOS22 22
#define XT_BITPOS23 23
#define XT_BITPOS24 24
#define XT_BITPOS25 25
#define XT_BITPOS26 26
#define XT_BITPOS27 27
#define XT_BITPOS28 28
#define XT_BITPOS29 29
#define XT_BITPOS30 30
#define XT_BITPOS31 31


#if defined( _8BIT_ARCHITECTURE )
#define XT_BIT0   0x01
#define XT_BIT1   0x02
#define XT_BIT2   0x04
#define XT_BIT3   0x08
#define XT_BIT4   0x10
#define XT_BIT5   0x20
#define XT_BIT6   0x40
#define XT_BIT7   0x80

#elif defined( _16BIT_ARCHITECTURE )
#define XT_BIT0   0x0001
#define XT_BIT1   0x0002
#define XT_BIT2   0x0004
#define XT_BIT3   0x0008
#define XT_BIT4   0x0010
#define XT_BIT5   0x0020
#define XT_BIT6   0x0040
#define XT_BIT7   0x0080
#define XT_BIT8   0x0100
#define XT_BIT9   0x0200
#define XT_BIT10  0x0400
#define XT_BIT11  0x0800
#define XT_BIT12  0x1000
#define XT_BIT13  0x2000
#define XT_BIT14  0x4000
#define XT_BIT15  0x8000

#elif defined( _24BIT_ARCHITECTURE )
#define XT_BIT0   0x000001
#define XT_BIT1   0x000002
#define XT_BIT2   0x000004
#define XT_BIT3   0x000008
#define XT_BIT4   0x000010
#define XT_BIT5   0x000020
#define XT_BIT6   0x000040
#define XT_BIT7   0x000080
#define XT_BIT8   0x000100
#define XT_BIT9   0x000200
#define XT_BIT10  0x000400
#define XT_BIT11  0x000800
#define XT_BIT12  0x001000
#define XT_BIT13  0x002000
#define XT_BIT14  0x004000
#define XT_BIT15  0x008000
#define XT_BIT16  0x010000
#define XT_BIT17  0x020000
#define XT_BIT18  0x040000
#define XT_BIT19  0x080000
#define XT_BIT20  0x100000
#define XT_BIT21  0x200000
#define XT_BIT22  0x400000
#define XT_BIT23  0x800000

#elif defined( _32BIT_ARCHITECTURE )
#define XT_BIT0   0x00000001
#define XT_BIT1   0x00000002
#define XT_BIT2   0x00000004
#define XT_BIT3   0x00000008
#define XT_BIT4   0x00000010
#define XT_BIT5   0x00000020
#define XT_BIT6   0x00000040
#define XT_BIT7   0x00000080
#define XT_BIT8   0x00000100
#define XT_BIT9   0x00000200
#define XT_BIT10  0x00000400
#define XT_BIT11  0x00000800
#define XT_BIT12  0x00001000
#define XT_BIT13  0x00002000
#define XT_BIT14  0x00004000
#define XT_BIT15  0x00008000
#define XT_BIT16  0x00010000
#define XT_BIT17  0x00020000
#define XT_BIT18  0x00040000
#define XT_BIT19  0x00080000
#define XT_BIT20  0x00100000
#define XT_BIT21  0x00200000
#define XT_BIT22  0x00400000
#define XT_BIT23  0x00800000
#define XT_BIT24  0x01000000
#define XT_BIT25  0x02000000
#define XT_BIT26  0x04000000
#define XT_BIT27  0x08000000
#define XT_BIT28  0x10000000
#define XT_BIT29  0x20000000
#define XT_BIT30  0x40000000
#define XT_BIT31  0x80000000

#else
#error ARCHITECTURE not defined
#endif  //ifdef _8BIT_ARCHITECTURE

/*==============================================================================
                            Type Definitions
==============================================================================*/
//"Status" codes for returns and/or general purpose use.  This idea is that in
//general, a fn can return a value as per eSTATUS.  If the value returned is
//eSTATUS_OK, all is good.  If the value returned is eSTATUS_ERR, then the
//receiver can look at an error variable that details the error.

#if defined( _8BIT_ARCHITECTURE )
typedef uint8  tuiSTATUS;

#elif defined( _16BIT_ARCHITECTURE )
typedef uint16 tuiSTATUS;

#elif defined( _32BIT_ARCHITECTURE )
typedef uint32 tuiSTATUS;

#else
#error ARCHITECTURE not defined

#endif  //ifdef _8BIT_ARCHITECTURE


/*==============================================================================
                                Enums
==============================================================================*/
//The types of status that can are available (tuiSTATUS).
typedef enum eSTATUS
{
    eSTATUS_INVALID = 0,    //Not to be used
    eSTATUS_OK,             //No issues
    eSTATUS_ERR             //Issue occured or exists
     
}teSTATUS;


/*=============================================================================
                              Macros
==============================================================================*/
//The following macro implements the common way to use tuiSTATUS and set an
//errno code.  tuiStatus needs to be declared in order for this macro to work.
#define mWRITE_STATUS_AND_ERRNO( ErrNoCode )    tuiStatus = eSTATUS_ERR;\
                                                errno     = ErrNoCode

//The following macro implements a common way to determine the dimension
//of an array.
#define XT_mDIM(x)             ( sizeof( x ) / sizeof( x[ 0 ] ) )

// The following macro implements x / y such that 
// x / y >= Real( x ) / Real( y )
#define XT_mDIVMAX( x, y )  ( ( x + ( y - 1 ) ) / y )

//The following macro implements the common way to round integer x to be
//integral number of y's. For example, if on input x is 13 and y
//is 5 then the result will be ( ( 13 + 5 - 1 ) / 5 ) * 5 = 15
#define XT_mROUND_UP( x, y )   ( XT_mDIVMAX( x, y ) * y )

/*=============================================================================
                              Structures
==============================================================================*/



#endif  //LIB_STDDEFS_INCL
