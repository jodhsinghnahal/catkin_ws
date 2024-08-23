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
    HalErrBase.h

PURPOSE:
    To define all of the common error groupings alotted to packages.
    There are user-defined errno error codes.

FUNCTION(S):
    None
       
    local:
        None
   
NOTES:
    #$-VIEW:C Coding Standard-$#
    **If you do not see a button above this, either use Code Wright or ignore 
    **the text.

    "errno" is part of the ANSI standard.  You can find references and 
    descriptions to it in the book "The C Programming Language" by Kernighan 
    and Ritchie.
    
CHANGE HISTORY:
$History: HALErrBase.h $
    
    *****************  Version 10  *****************
    User: Johnb        Date: 5/20/03    Time: 2:44p
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/Common/Inc
    Removed include file LIB_stdtype.h.
    Removed NVMEM error code range and renumbered the remaining.
    
    *****************  Version 9  *****************
    User: Johnb        Date: 5/15/03    Time: 10:37a
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/Common/Inc
    Updated comments.
    Added History for VSS comment placement.

Date:   Oct 29, 2002   By: John Bellini
    -   Created.
    
Date:   Nov 8, 2002    By: Ron Mok
    -   Added errnos for SPI.
    
Date:   Nov 12, 2002   By: John Bellini
    -   Added errnos for System Level package

Date:   Dec. 10, 2002  By: Yingran Duan
    -   Added errnos for CAP.

Date:   Jan 7, 2003    By: Ron Mok
    -   Added errnos for EEPROM.
    
Date:   Jan 16, 2003   By: John Bellini
    -   Added errnos for ADC.

==============================================================================*/

#ifndef HALERRBASE_INCL
#define HALERRBASE_INCL


/*==============================================================================
                              Includes
==============================================================================*/


/*==============================================================================
                              Defines
==============================================================================*/
//Errno ranges for packages.  
//Note that these are 16 bit values.
#define ERR_PWM_DRV        ( 1 << 8 )                 //PWM drv errno's
#define ERR_PWM_DRV_MAX    ( ERR_PWM_DRV   | 0x00FF ) //PWM max errno
#define ERR_TMR_DRV	       ( 2 << 8 )                 //TMR drv errno's
#define ERR_TMR_DRV_MAX    ( ERR_TMR_DRV   | 0x00FF ) //TMR max errno
#define ERR_SCI_DRV        ( 3 << 8 )                 //SCI drv errno's
#define ERR_SCI_DRV_MAX    ( ERR_SCI_DRV   | 0x00FF ) //SCI max errno
#define ERR_SPI_DRV        ( 4 << 8 )                 //SPI drv errno's
#define ERR_SPI_DRV_MAX    ( ERR_SPI_DRV   | 0x00FF ) //SPI max errno
#define ERR_SYSLVL         ( 5 << 8 )                 //System level 
#define ERR_SYSLVL_MAX     ( ERR_SYSLVL    | 0x00FF ) //System level max errno
#define ERR_CAP_DRV        ( 6 << 8 )                 //CAP level 
#define ERR_CAP_DRV_MAX    ( ERR_CAP_DRV   | 0x00FF ) //CAP level max errno
#define ERR_EEPROM_DRV     ( 7 << 8 )                 //EEPROM drv errnos
#define ERR_EEPROM_DRV_MAX ( ERR_EEPROM_DRV | 0x00FF )//EEPROM max errno
#define ERR_ADC_DRV        ( 8 << 8 )                 //ADC drv errno
#define ERR_ADC_DRV_MAX    ( ERR_ADC_DRV   | 0x00FF ) //ADC drv max errno

#endif 
