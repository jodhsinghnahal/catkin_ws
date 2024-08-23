/*==============================================================================
Copyright © 2003 Xantrex Technology, Inc. All rights reserved.

This source file and the information contained in it are confidential and 
proprietary to Xantrex Technology, Inc. The reproduction or disclosure, in 
whole or in part, to anyone outside of Xantrex without the written approval of 
a Xantrex officer under a Non-Disclosure Agreement, or to any employee of 
Xantrex who has not previously obtained written authorization for access from 
the individual responsible for the source code, will have a significant 
detrimental effect on Xantrex and is expressly prohibited.

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
    #$-VIEW:C Coding Standard-$#
    **If you do not see a button above this, either use CodeWright or ignore 
    **the text.
    
HISTORY:
$Log: config.h $

    ***********************************************
    Revision: XanBus_HenryW/1
    User: HenryW     Date: 08/19/05  Time: 08:07PM
    - cleaned up history list.

    ***********************************************
    Revision: XanBus_DaleM/3
    User: DaleM     Date: 03/08/05  Time: 10:21PM
    Added XT_mMK_BYTE macro for TI compatability

    ***********************************************
    Revision: XanBus_DaleM/2
    User: DaleM     Date: 01/14/05  Time: 04:39PM
    Multiport XanBus Library

    ***********************************************
    Revision: XanBus_DaleM/1
    User: DaleM     Date: 01/12/05  Time: 07:19PM
    Multiport XanBus Library
    
==============================================================================*/

#ifndef CONFIG_INCL
#define CONFIG_INCL
/*==============================================================================
                              Includes
==============================================================================*/

/*==============================================================================
                              Defines
==============================================================================*/

// Define far but make it empty
#define XT_FAR                 
 
#define XT_BITS_PER_BYTE        ( 8 )

/* // Macro for converting value to an 8 bit value */
/* #define XT_mMK_BYTE(x)          ( x ) */

#endif  // CONFIG_INCL
