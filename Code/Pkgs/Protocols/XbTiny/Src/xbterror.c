/*=============================================================================
  (c) 2003 - 2019 Xantrex Canada ULC. All rights reserved.
  All trademarks are owned or licensed by Xantrex Canada ULC,
  its subsidiaries or affiliated companies.


FILE NAME:  xbterror.c

PURPOSE:
    This module is used to handle XBT errors.

FUNCTION(S):
    XBTERROR_fnSaveError    - Store a reported error code

=============================================================================*/

/*==============================================================================
                              Includes
==============================================================================*/

#include <stdio.h>

#include "xbudefs.h"
#include "xbtgdefs.h"
#include "xbtldefs.h"

/*==============================================================================
                           Local/Private Constants
==============================================================================*/

/*==============================================================================
                           Local/Private Variables
==============================================================================*/

static uint16 uiErrCode;

/*==============================================================================
                           Function Definitions
==============================================================================*/

/******************************************************************************

FUNCTION NAME:
    XBTERROR_fnSaveError

PURPOSE:
    Store reported error codes

INPUTS:
    'uiCode' is error code to be saved

NOTES:
    
VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version

******************************************************************************/

void XBTERROR_fnSaveError( uint16 uiCode )
{
    uiErrCode = uiCode;

    (void)uiErrCode;    // Avoid compiler warning #552-D
}

