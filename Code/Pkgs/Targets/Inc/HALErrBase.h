/*==============================================================================
Copyright 2005 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:
    HalErrBase.h

PURPOSE:
    To define a common error grouping alotted to packages which generate errors
    The intention is for these groupings to be used to hold error code
    which are user-defined errno error codes.  A listed group can be used by the
    specified package to define its own set of errors within the given range.

FUNCTION(S):
    None

    local:
        None

NOTES:
    "errno" is part of the ANSI standard and can be used to hold error codes.
    You can find references and descriptions to it in the book
    "The C Programming Language" by Kernighan and Ritchie.

CHANGE HISTORY:
$Log: HALErrBase.h $

    ***********************************************
    Revision: NovaPfrmB_JohnB/2
    User: JohnB     Date: 03/22/05  Time: 05:34PM
    Updated to latest standard.

    Port from VSS to AccuRev.

==============================================================================*/

#ifndef HALERRBASE_H
#define HALERRBASE_H


/*==============================================================================
                              Includes
==============================================================================*/
//None

/*==============================================================================
                              Defines
==============================================================================*/
//Errno ranges for packages.
//Note that these are 16 bit values.
#define ERR_PWM_DRV        ( 1 << 8 )                  //PWM drv errno's
#define ERR_PWM_DRV_MAX    ( ERR_PWM_DRV    | 0x00FF ) //PWM max errno
#define ERR_TMR_DRV	       ( 2 << 8 )                  //TMR drv errno's
#define ERR_TMR_DRV_MAX    ( ERR_TMR_DRV    | 0x00FF ) //TMR max errno
#define ERR_SCI_DRV        ( 3 << 8 )                  //SCI drv errno's
#define ERR_SCI_DRV_MAX    ( ERR_SCI_DRV    | 0x00FF ) //SCI max errno
#define ERR_SPI_DRV        ( 4 << 8 )                  //SPI drv errno's
#define ERR_SPI_DRV_MAX    ( ERR_SPI_DRV    | 0x00FF ) //SPI max errno
#define ERR_SYSLVL         ( 5 << 8 )                  //System level
#define ERR_SYSLVL_MAX     ( ERR_SYSLVL     | 0x00FF ) //System level max errno
#define ERR_CAP_DRV        ( 6 << 8 )                  //CAP level
#define ERR_CAP_DRV_MAX    ( ERR_CAP_DRV    | 0x00FF ) //CAP level max errno
#define ERR_EEPROM_DRV     ( 7 << 8 )                  //EEPROM drv errnos
#define ERR_EEPROM_DRV_MAX ( ERR_EEPROM_DRV | 0x00FF ) //EEPROM max errno
#define ERR_ADC_DRV        ( 8 << 8 )                  //ADC drv errno
#define ERR_ADC_DRV_MAX    ( ERR_ADC_DRV    | 0x00FF ) //ADC drv max errno

#endif

