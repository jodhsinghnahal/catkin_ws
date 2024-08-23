/*==============================================================================
Copyright © 2004 Xantrex International

This file is the property of Xantrex International and shall not be reproduced,
copied, or used as the basis for the manufacture or sale of equipment without
the express written permission of Xantrex International.

FILE NAME:  
    LIB_stdtype32.h

PURPOSE:
    This is a header file containing all the data type definitions
 	required for a 8 bit processor.

FUNCTION(S):
    None

NOTES:

CHANGE HISTORY:
$Log: LIB_stdtype32.h $

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/2
    User: JeffF     Date: 03/02/07  Time: 07:19PM
    Added 8-bit integer types, and corrected the definition for 64-bit integer
    type. It needed to be long long.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/1
    User: JeffF     Date: 02/13/06  Time: 06:59PM
    Ran the copyright updater script to bring the copyright notice on all project
    files up to date.

    ***********************************************
    Revision: Xantrex1_JohnB/4
    User: JohnB     Date: 12/06/04  Time: 06:33PM
    Updated header information.
    

==============================================================================*/

#ifndef LIB_STDTYPE32_H
#define LIB_STDTYPE32_H


/*==============================================================================
                              Includes
==============================================================================*/


/*==============================================================================
                              Defines
==============================================================================*/


/*==============================================================================
                            Type Definitions
==============================================================================*/
typedef unsigned char tucBOOL;

typedef unsigned char uchar8;
typedef signed   char schar8;

typedef unsigned char uint8;
typedef signed   char sint8;

typedef unsigned short int uint16;
typedef signed   short int sint16;

typedef unsigned long int uint32;
typedef signed   long int sint32;

typedef unsigned long long int uint64;
typedef signed   long long int sint64;

typedef float  float32;
typedef double double64;


/*==============================================================================
                                Enums
==============================================================================*/


/*=============================================================================
                              Structures
==============================================================================*/


#endif  //End LIB_STDTYPE32_INCL
