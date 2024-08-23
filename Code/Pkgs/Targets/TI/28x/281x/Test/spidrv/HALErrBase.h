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
$Log: HALErrBase.h $

    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/2
    User: BaldeeshK     Date: 12/08/04  Time: 05:11PM
    - Initial add to Accurev of spi driver test files
    
    

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
