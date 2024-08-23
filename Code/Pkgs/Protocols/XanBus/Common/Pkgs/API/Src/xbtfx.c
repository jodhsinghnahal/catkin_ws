/*==============================================================================
Copyright 2003 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by 
copyright. The reproduction, in whole or in part, by anyone without the written 
approval of Xantrex is prohibited.

FILE NAME:  
    xbtfx.c

PURPOSE:
    This file contains functions required by the Teleflex library.  Some
    functions are specified as required by Teleflex others are here to
    replace functions that were lost when some Teleflex files were dropped.
    These functions are not expected to be exposed as part of the XanBus API.

FUNCTION(S):
    TFXGetRandDelay      - get random delay number for Teleflex library use
    XBTFX_fnSRand        - Seed the random number generator
    XBTFX_fnIsFrameForMe - Is Frame For Me function required by TELEFLEX

    local:
   
NOTES:

CHANGE HISTORY:
$History: xbtfx.c $
    
    *****************  Version 28  *****************
    User: Toml         Date: 12/15/03   Time: 3:57p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/UIEx/XB/API/Src
    Replaced TFXEnvMalloc with XBMEM_fnMalloc and TFXEnvFree with
    XBMEM_fnFree in order for XanBus to have its own dedicated heap.
    
    *****************  Version 26  *****************
    User: Peterd       Date: 10/12/03   Time: 3:09p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/BattMonEx/XB/API/Src
    Changed #include <assert.h> to #include "xassert.h"
    
    *****************  Version 24  *****************
    User: Peterd       Date: 9/12/03    Time: 2:48p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/BattMonEx/XB/API/Src
    Updated per Code Review (See
    $/PD/Common/Protocols/XanBus/Admin/CodeReviewSummary.xls for
    specifics):
    - copyright
    - enum prefixing e to te
    - DIM to XB_mDIM
    - lint corrections
    - asserts
    
    *****************  Version 21  *****************
    User: Dalem        Date: 11/05/03   Time: 1:39p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/AGSEx/XB/API/Src
    Added Malloc high water mark to commstats
    
    *****************  Version 17  *****************
    User: Dalem        Date: 10/08/03   Time: 3:26p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/AGSEx/XB/API/Src
    Added counters for malloc and free
    Changed parameter size for TFXEnvMalloc
    
    *****************  Version 10  *****************
    User: Dalem        Date: 8/05/03    Time: 3:34p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/AGSEx/XB/API/Src
    Changed function name to XBTFX_fnIsFrameForMe
    Change uli to ul
    
    *****************  Version 8  *****************
    User: Toml         Date: 6/26/03    Time: 2:38p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/BattMonEx/XB/API/Src
    New API
    
    *****************  Version 9  *****************
    User: Dalem        Date: 6/24/03    Time: 2:52p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DP256B/Test/BattMonEx/XB/API/Src
    Major change to XB API and structure names
    
    *****************  Version 7  *****************
    User: Toml         Date: 6/02/03    Time: 6:52p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/BattMonEx/XB/API/Src
    Voided functions that never fail and who's return values are never
    checked.
    
    *****************  Version 6  *****************
    User: Dalem        Date: 5/01/03    Time: 2:22p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DP256B/Test/BattMonEx/XB/API/Src
    Upper case names of local functions
    Use tucBOOLs
    Changes to PGN support
    
    *****************  Version 5  *****************
    User: Dalem        Date: 2/05/03    Time: 2:55p
    Updated in $/PD/Common/XanBus/Code/Common/XBAPI
    Fixed headers and some spacing
    
    *****************  Version 4  *****************
    User: Dalem        Date: 1/28/03    Time: 10:06a
    Updated in $/PD/Common/XanBus/Code/Common/XBAPI
    Changes to drop Teleflex using their PGN descriptors and variants
    
    *****************  Version 3  *****************
    User: Dalem        Date: 1/23/03    Time: 5:09p
    Updated in $/PD/Common/XanBus/Code/Common/XBAPI
    Drop now empty NETConstruct function
    
    *****************  Version 2  *****************
    User: Dalem        Date: 1/14/03    Time: 2:33p
    Updated in $/PD/Common/XanBus/Code/Common/XBAPI
    Change to upper case prefix for global functions
    
    *****************  Version 1  *****************
    User: Dalem        Date: 1/09/03    Time: 9:26a
    Created in $/PD/Common/XanBus/Code/Common/XBAPI
    Code taken from old xbdata.c
    
==============================================================================*/

/*==============================================================================
                              Includes
==============================================================================*/

// XanBus include files
#include "xbldefs.h"    // Local Definitions

// System include files
#include "xassert.h"     // Debugging

/*==============================================================================
                              Defines
==============================================================================*/

// Definitions and storage for random number generator
#define XB_RAND_DELAY_MAX   153L

/*==============================================================================
                           Local/Private Variables
==============================================================================*/

// Definitions and storage for random number generator
static uint32 ulNextSeed = 1;

/*==============================================================================
                           Function Definitions
==============================================================================*/


/******************************************************************************

FUNCTION NAME:
    TFXGetRandDelay

PURPOSE:
   Return a random number for Teleflex library to use
    
INPUTS:
    None

OUTPUTS:
    Random number between 0 and 153

NOTES:
   This function has a Teleflex function name and is provided specifically
   for the Teleflex library to use.
    
VERSION HISTORY:

Version 1.00  Date: 02/05/03  By: code_fix.py
    - Added version history to function header

******************************************************************************/

uint16 TFXGetRandDelay( void )
{
     uint16 uiRand;

     // Next seed
     ulNextSeed = ulNextSeed * 1103515245L + 12345;

     // Compute pseudo-random value
     uiRand = ( ulNextSeed / 65536 ) % ( XB_RAND_DELAY_MAX + 1 );

     return( uiRand );
}

/******************************************************************************

FUNCTION NAME:
    XBTFX_fnSRand

PURPOSE:
    Provide seed for the random number generator

INPUTS:
    'ulRandSeed' is number to use as seed for random number generator

OUTPUTS:
    Nothing

NOTES:
    
VERSION HISTORY:

Version 1.00  Date: 02/05/03  By: code_fix.py
    - Added version history to function header

******************************************************************************/

void XBTFX_fnSRand( uint32 ulRandSeed )
{
    ulNextSeed = ulRandSeed;
}

/******************************************************************************

FUNCTION NAME:
    XBTFX_fnIsFrameForMe

PURPOSE:
    This function replaces a function from the Teleflex library

INPUTS:
    'i8Port' is which hardware port to access (not used)
    'ptzCANFrame' is pointer to can frame being tested

OUTPUTS:
    TRUE if destination address of CAN frame is global or my address
    FALSE if frame destination is not me

NOTES:
    
VERSION HISTORY:

Version 1.00  Date: 02/05/03  By: code_fix.py
    - Added version history to function header
Version 1.01  Date: 06/24/03  By: Dalem
    - Name change to XB API

******************************************************************************/

BOOL XBTFX_fnIsFrameForMe( CANPORT i8Port, CANFRAME *ptzCANFrame )
{
	uchar8 destAddr;
    uchar8 ucMyAddr;

    // Verify parameter
    (void)assert( ptzCANFrame != NULL );
    
    destAddr = CANFrameGetDestAddress( ptzCANFrame );
    ucMyAddr = XBADDR_fnGetMyAddr();

    // Touch parameter to hide warning about unused parameter
    if( i8Port != 0 )
    {
		i8Port++;
    }    

    // If destination is global or my address
    if( ( destAddr == PGN_ADDR_GLOBAL ) || ( destAddr == ucMyAddr ) )
    {
		return( TRUE );
    }    
    else
    {
    	return( FALSE );
    }    
}

