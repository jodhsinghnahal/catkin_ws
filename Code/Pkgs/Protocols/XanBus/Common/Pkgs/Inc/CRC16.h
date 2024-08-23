/*==============================================================================
Copyright (c) 2004, 2013 Schneider Electric Solar Inverters USA, Inc.

No part of this document may be reproduced in any form or disclosed to third
parties without the express written consent of:

             Schneider Electric Solar Inverters USA, Inc.
                         250 South Vasco Road
                        Livermore, California
                              USA 94551

This source file is proprietary to Xantrex International and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.


FILE NAME:
    CRC16.h

PURPOSE:
    To calculate the CRC-16 of a collection of data.

FUNCTION(S):
    CRC16_Calculate - Calculates the CRC-16 of an array of data.

NOTES:


CHANGE HISTORY:
$Log: CRC16.h $

    ***********************************************
    Revision: NovaPfrmB_JohnB/3
    User: JohnB     Date: 12/17/04  Time: 09:24PM
    Updates to coding standard.
    Updated so Calc.c, Calc2ndOrder.c, CRC16.c, CRC8.c
    to be able to build them into a library.
    Updated the square root unit test.


==============================================================================*/

#ifndef CALCCRC16_INCL
#define CALCCRC16_INCL


/*==============================================================================
                              Includes
==============================================================================*/
#include "LIB_stdtype.h"    // Requires and architecture defn. in the compiler
#include "LIB_stddefs.h"    // Standard definitions for all projects


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

// CRC16_fnCalculate
//
// Inputs:
//  uint16 *SourceArrayPtr - The data from which the CRC is calculated.
//  uint16 ArrayLength     - The number of elements in the source array.
//  uint16 *CRCPtr         - The value with which the CRC calc is initialized.
//
// Outputs:
//  uint16 *CRCPtr         - The calculated value of the entire array.
void CRC16_fnCalculate( uint16 *SourceArrayPtr,
                             uint16 ArrayLength,
                             uint16 *CRCPtr );

// CRC16_fnAddByte
//
// Inputs:
//  uint16 uiCrc  - current value of the CRC
//  uchar8 ucData - byte to add in CRC calculation
//
// Outputs:
//  uint16 - calculated value of the CRC
uint16 CRC16_fnAddByte( uint16 uiCrc, uchar8 ucData );

// CRC16_fnCalculateBlock
//
// Inputs:
//  uint16 uiCrc    - starting value of the CRC
//  uchar8 *pucData - pointer to data block to use in CRC calculation
//  uint16 uiLen    - data block length
//
// Outputs:
//  uint16 - the calculated CRC value for the block
uint16 CRC16_fnCalculateBlock( volatile uint16 uiCrc, uchar8 *pucData, uint16 uiLen );

#endif  // CALCCRC16_INCL

