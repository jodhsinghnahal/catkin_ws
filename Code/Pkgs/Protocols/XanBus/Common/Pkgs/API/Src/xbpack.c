/*==============================================================================
Copyright 2003 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:
    xbpack.c

PURPOSE:
   This file contains functions for unpacking the contents of received
   messages and for packing the contents of messages to be sent.

FUNCTION(S):
    XBPACK_fnGetPgnInfo       - Look up field info for given PGN
    XBPACK_fnPackPgn          - convert a structure into a XanBus message
    XBPACK_fnUnPackPgn        - Convert a XanBus message into a structure
    XBPACK_fnDestroyPgnStruct - Release memory of an unpacked structure
    XBPACK_fnRawGrpFunc       - get group function of a PGN if there is one

    local:
        xbpack_fnPgnCmp          - compare pairs of PGN/GrpFunc
        xbpack_fnGetExtraCount   - get count of extras field
        xbpack_fnSetExtraCount   - set count of extras field
        xbpack_fnGetExtraPoint   - get pointer to extras fields
        xbpack_fnSetExtraPoint   - set pointer to extras fields
        xbpack_fnPackInt         - pack an int value
        xbpack_fnPackLong        - pack a long value
        xbpack_fnPackFloat       - pack a float value
        xbpack_fnPackDouble      - pack a double value
        xbpack_fnPackBits        - pack bit field
        xbpack_fnPackField       - pack a field of a structure
        xbpack_fnPackStruct      - pack all fields of a structure
        xbpack_fnUnPackInt       - unpack an integer value
        xbpack_fnUnPackLong      - unpack a long value
        xbpack_fnUnPackFloat     - unpack a float value
        xbpack_fnUnPackDouble    - unpack a double value
        xbpack_fnUnPackBits2Byte - unpack an 8 bit bit field
        xbpack_fnUnPackBits2Int  - unpack a 16 bit bit field
        xbpack_fnUnPackBits2Long - unpack a 32 bit bit field
        xbpack_fnUnPackField     - unpack one field of a structure
        xbpack_fnUnPackStruct    - unpack all fields of a structure

NOTES:

CHANGE HISTORY:
$Log: xbpack.c $

    ***********************************************
    Revision: XanBus_HollyZ/3
    User: HollyZ     Date: 06/01/06  Time: 03:59PM
    Removed #ifdef  for packing/unpacking integer or long interger

    ***********************************************
    Revision: XanBus_HollyZ/2
    User: HollyZ     Date: 05/25/06  Time: 10:12PM
    Updated to remove warning for TI compiler

    ***********************************************
    Revision: XanBus_HollyZ/1
    User: HollyZ     Date: 05/25/06  Time: 07:12PM
    Revert Xb stack to V1.2.7

    ***********************************************
    Revision: XanBus_DaleM/2
    User: DaleM     Date: 01/12/05  Time: 10:31PM
    Multiport XanBus


    *****************  Version 71  *****************
    User: Peterd       Date: 30/03/04   Time: 4:46p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/AGSEx/XB/API/Src
    Added function XBPACK_fnUnpackMsgCount and supporting variables and
    types.

    *****************  Version 62  *****************
    User: Peterd       Date: 4/02/04    Time: 9:29a
    Updated in $/PD/Common/Protocols/XanBus/Code/Windows/ifak_usb/Test/DiagWinEx/XB/API/Src
    Corrected comments and comparison to FALSE that is actually NULL

    *****************  Version 60  *****************
    User: Peterd       Date: 23/01/04   Time: 4:36p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/AGSEx/XB/API/Src
    Added error code to function "XBPACK_fnUnPackPgn"

    *****************  Version 56  *****************
    User: Toml         Date: 12/15/03   Time: 3:57p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/UIEx/XB/API/Src
    Replaced TFXEnvMalloc with XBMEM_fnMalloc and TFXEnvFree with
    XBMEM_fnFree in order for XanBus to have its own dedicated heap.

    *****************  Version 54  *****************
    User: Peterd       Date: 10/12/03   Time: 3:09p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/BattMonEx/XB/API/Src
    Changed #include <assert.h> to #include "xassert.h"

    *****************  Version 52  *****************
    User: Peterd       Date: 10/12/03   Time: 2:09p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/BattMonEx/XB/API/Src
    Removed invalid assert from fnSetExtraPoint

    *****************  Version 50  *****************
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

    *****************  Version 40  *****************
    User: Dalem        Date: 9/05/03    Time: 11:20a
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/AGSEx/XB/API/Src
    GetPgnInfo cannot check for null descriptor so users of it must.

    *****************  Version 38  *****************
    User: Dalem        Date: 8/22/03    Time: 5:40p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/InvChgEx/XB/API/Src
    Do all message length checking first
    Change to RawGroupFunction

    *****************  Version 37  *****************
    User: Peterd       Date: 8/22/03    Time: 1:13p
    Updated in $/PD/Common/Protocols/XanBus/Code/Windows/CANCardX/Test/DiagWinEx/XB/API/Src
    Changed uchar group function to enumeration per latest xbudefs.h

    *****************  Version 35  *****************
    User: Dalem        Date: 8/18/03    Time: 1:43p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/AGSEx/XB/API/Src
    Group Don't Care definition changed

    *****************  Version 34  *****************
    User: Peterd       Date: 8/12/03    Time: 9:08a
    Updated in $/PD/Common/Protocols/XanBus/Code/Windows/CANCardX/Test/DiagWinEx/XB/API/Src
    Added check for NULL PGN descriptor in XBPACK_fnGetPgnInfo.
    Added check for unpack from 16-bit int to 32-bit int in
    xbpack_fnUnPackField.

    *****************  Version 33  *****************
    User: Dalem        Date: 8/05/03    Time: 3:44p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/AGSEx/XB/API/Src
    Changed enums to PGN_eESRC_xxx
    Updated function descriptions
    Rearrange code for review

    *****************  Version 31  *****************
    User: Peterd       Date: 7/24/03    Time: 11:22a
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DP256B/Test/BattMonEx/XB/API/Src
    Corrected enumerations to follow updated coding standard:

    typedef enum eXXXX
    {
        <pkg>_eXXXX_YYYY = 0;
        <pkg>_eXXXX_ZZZZ
    } <pkg>_teXXXX;

    Changed prefixes uli and sli to ul and sl respectively.

    Added shortening of enumerated type names.

    *****************  Version 30  *****************
    User: Peterd       Date: 7/11/03    Time: 4:14p
    Updated in $/PD/Common/Protocols/XanBus/Code/Common/XB/API/Src
    Updated per changes in xanbus_headergen.py and xb.py scripts labeled
    0.5.0.
    Generated enum and struct type definitions now comply with coding
    standard.

    *****************  Version 29  *****************
    User: Dalem        Date: 7/08/03    Time: 2:05p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DP256B/Test/BattMonEx/XB/API/Src
    Change error reporting

    *****************  Version 28  *****************
    User: Dalem        Date: 7/07/03    Time: 8:52a
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DP256B/Test/BattMonEx/XB/API/Src
    Do not unpack messages that do not have complet variable length units
    Changed case to uliPgn and changed DLC to Len

    *****************  Version 27  *****************
    User: Dalem        Date: 6/27/03    Time: 8:33a
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DP256B/Test/BattMonEx/XB/API/Src
    Use XT_FAR in place of far

    *****************  Version 26  *****************
    User: Toml         Date: 6/26/03    Time: 2:39p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/BattMonEx/XB/API/Src
    New API

    *****************  Version 28  *****************
    User: Toml         Date: 6/26/03    Time: 1:14p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DP256B/Test/BattMonEx/XB/API/Src
    Added sense of signed/unsigned to field data types

    *****************  Version 27  *****************
    User: Dalem        Date: 6/24/03    Time: 2:53p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DP256B/Test/BattMonEx/XB/API/Src
    Major change to XB API and structure names

    *****************  Version 25  *****************
    User: Dalem        Date: 6/03/03    Time: 11:27a
    Updated in $/PD/Common/Protocols/XanBus/Code/Windows/CANCardX/Test/DiagEx/XB/API/Src
    Handle 32-bit enums

    *****************  Version 24  *****************
    User: Toml         Date: 5/22/03    Time: 11:09a
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DP256B/Test/UIEx/XB/API/Src
    Moved PGN table and code that references it into one page in banked
    memory.

    *****************  Version 23  *****************
    User: Dalem        Date: 5/20/03    Time: 8:57a
    Updated in $/PD/Common/Protocols/XanBus/Code/Windows/CANCardX/Test/DiagEx/XB/API/Src
    Fix in comparing group function

    *****************  Version 22  *****************
    User: Dalem        Date: 5/12/03    Time: 6:05p
    Updated in $/PD/Common/Protocols/XanBus/Code/Windows/CANCardX/Test/DiagEx/XB/API/Src
    Fixing some size warnings for windows use

    *****************  Version 21  *****************
    User: Toml         Date: 5/12/03    Time: 3:23p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DP256B/Test/UIEx/XB/API/Src
    Separated descriptor search from pack/unpack process in order to reduce
    search time.

    *****************  Version 20  *****************
    User: Dalem        Date: 5/01/03    Time: 2:22p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DP256B/Test/BattMonEx/XB/API/Src
    Upper case names of local functions
    Use tucBOOLs
    Changes to PGN support

    *****************  Version 19  *****************
    User: Dalem        Date: 4/22/03    Time: 1:12p
    Updated in $/PD/Common/Protocols/XanBus/Code/Windows/CANCardX/Test/DiagEx/XB/API/Src
    Save an error in destroy PGN

    *****************  Version 18  *****************
    User: Toml         Date: 4/02/03    Time: 4:15p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DP256B/Test/UIEx/XB/API/Src
    Fixed the pack side of the enum to bit field problem

    *****************  Version 17  *****************
    User: Toml         Date: 4/02/03    Time: 2:01p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DP256B/Test/UIEx/XB/API/Src
    Fixed bug where small bit fields get unpacked into enums

    *****************  Version 16  *****************
    User: Dalem        Date: 2/24/03    Time: 3:18p
    Updated in $/PD/Common/XanBus/Code/Common/XBAPI
    Cleaned up some warnings about const and type conversions

    *****************  Version 15  *****************
    User: Dalem        Date: 2/14/03    Time: 1:33p
    Updated in $/PD/Common/XanBus/Code/Common/XBAPI
    Packing and unpacking of bytes depends on structure element size

    *****************  Version 14  *****************
    User: Dalem        Date: 2/07/03    Time: 3:43p
    Updated in $/PD/Common/XanBus/Code/Common/XBAPI
    Set length from pack to 0xFFFF on error, NULL is not an error.

    *****************  Version 13  *****************
    User: Dalem        Date: 2/05/03    Time: 2:55p
    Updated in $/PD/Common/XanBus/Code/Common/XBAPI
    Fixed headers and some spacing

    *****************  Version 12  *****************
    User: Dalem        Date: 1/28/03    Time: 11:14a
    Updated in $/PD/Common/XanBus/Code/Common/XBAPI
    Change to use XT_BIG_ENDIAN switch

    *****************  Version 11  *****************
    User: Dalem        Date: 1/24/03    Time: 3:46p
    Updated in $/PD/Common/XanBus/Code/Common/XBAPI
    Changes to drop Teleflex using their PGN descriptors and variants

    *****************  Version 10  *****************
    User: Dalem        Date: 1/23/03    Time: 5:09p
    Updated in $/PD/Common/XanBus/Code/Common/XBAPI
    Add function to get group function code from raw receive buffer

    *****************  Version 9  *****************
    User: Dalem        Date: 1/13/03    Time: 1:24p
    Updated in $/PD/Common/XanBus/Code/Common/XBAPI
    Renaming pack functions and receive function

    *****************  Version 8  *****************
    User: Dalem        Date: 1/09/03    Time: 9:31a
    Updated in $/PD/Common/XanBus/Code/Common/XBAPI
    Renamed some functions to xbi_, xbm_ and xbtfx_

    *****************  Version 7  *****************
    User: Dalem        Date: 12/16/02   Time: 11:10a
    Updated in $/PD/Common/XanBus/Code/Common/XBAPI
    changed DestroyPGNStruct so PGN is 32 bits

    *****************  Version 6  *****************
    User: Toml         Date: 12/16/02   Time: 10:16a
    Updated in $/PD/Common/XanBus/Code/Common/XBAPI
    Fixed variable prefix for big endian case

    *****************  Version 5  *****************
    User: Toml         Date: 12/09/02   Time: 2:17p
    Updated in $/PD/Common/XanBus/Code/Common/XBAPI
    Corrected byte ordering of functions that unpack byte fields

    *****************  Version 4  *****************
    User: Dalem        Date: 12/02/02   Time: 8:17a
    Updated in $/PD/Common/XanBus/Code/Common/XBAPI
    More changes to error reporting

    *****************  Version 3  *****************
    User: Dalem        Date: 11/28/02   Time: 3:35p
    Updated in $/PD/Common/XanBus/Code/Common/XBAPI
    Changed error handling

    *****************  Version 2  *****************
    User: Dalem        Date: 11/28/02   Time: 9:13a
    Updated in $/PD/Common/XanBus/Code/Common/XBAPI
    Save error reasons and free memory on errors

    *****************  Version 1  *****************
    User: Toml         Date: 11/21/02   Time: 10:16a
    Updated in $/PD/Common/XanBus/Code/Common/XBAPI
    Initial version

==============================================================================*/

/*=============================================================================
                                  Includes
=============================================================================*/

// XanBus include files
#include "xbldefs.h"    // Local Definitions
#include "xbmem.h"      // memory allocation
#include "config.h"     // For XT_BIT_PER_BYTE

// RV-C Protocol Definitions
#include "rvcpdefs.h"

// System include files
#include <string.h>     // for mem functions
#include "xassert.h"    // Debugging

/*==============================================================================
                              Defines
==============================================================================*/

#define XBPACK_BITS_PER_WORD    ( 16 )
#define XBPACK_BITS_PER_DWORD   ( 32 )

/*==============================================================================
                            Type Definitions
==============================================================================*/

typedef struct zPGN_TO_GET_MSG_COUNT
{
    XB_teCFG_TYPE teCfgType;        // Configuration Type
    uchar8 ucMsgCount;              // Message Count
} XB_tzPGN_TO_GET_MSG_COUNT;

/*==============================================================================
                           Local/Private Variables
==============================================================================*/

const PGN_tzFIELD_DESC XT_FAR azPgnToGetMsgCountFields[] =
{
    { PGN_eTYPE_BIT,   0,   2, (uint16)((uint32)&( ((XB_tzPGN_TO_GET_MSG_COUNT *)NULL )->teCfgType )), sizeof( ((XB_tzPGN_TO_GET_MSG_COUNT *)NULL )->teCfgType ) },
    { PGN_eTYPE_BIT,   2,   6, (uint16)((uint32)&( ((XB_tzPGN_TO_GET_MSG_COUNT *)NULL )->ucMsgCount )), sizeof( ((XB_tzPGN_TO_GET_MSG_COUNT *)NULL )->ucMsgCount ) }
};

const PGN_tzPGN_DESC XT_FAR tzPgnToGetMsgCountDesc =
{
    -1,
    6,
    NULL,
    NULL,
    1,
    sizeof( XB_tzPGN_TO_GET_MSG_COUNT ),
    2,
    (const PGN_tzFIELD_DESC * XT_FAR) azPgnToGetMsgCountFields,
    0,
    0,
    0,
    NULL
};

/*==============================================================================
                              Forward declarations
==============================================================================*/

static schar8 xbpack_fnPgnCmp( uint32 ulPgnA,
                               sint16 siGrpFuncA,
                               uint32 ulPgnB,
                               sint16 siGrpFuncB );
static uint16 xbpack_fnGetExtraCount( const PGN_tzPGN_DESC * XT_FAR pktzPgnDesc,
                                      void *pvStruct );
static void xbpack_fnSetExtraCount( const PGN_tzPGN_DESC * XT_FAR pktzPgnDesc,
                                    void *pvStruct,
                                    uint16 uiCount );
static void *xbpack_fnGetExtraPoint( const PGN_tzPGN_DESC * XT_FAR pktzPgnDesc,
                                     void *pvStruct );
static void xbpack_fnSetExtraPoint( const PGN_tzPGN_DESC * XT_FAR pktzPgnDesc,
                                    void *pvStruct,
                                    void *pvExtras );
static void xbpack_fnPackInt( uint16 uiSource,
                              uchar8 *pucDest );
static void xbpack_fnPackLong( uint32 ulSource,
                               uchar8 *pucDest );
static void xbpack_fnPackFloat( float32 fSource,
                                uchar8 *pucDest );
static void xbpack_fnPackDouble( double64 dSource,
                                 uchar8 *pucDest );
static void xbpack_fnPackBits( uint32 ulSource,
                               uchar8 *pucDest,
                               uint16 uiBitOffset,
                               uint16 uiBitCount );
static tucBOOL xbpack_fnPackField( const PGN_tzFIELD_DESC * XT_FAR pktzFieldDesc,
                                  void *pvSource,
                                  uchar8 *pucDestBuf );
static tucBOOL xbpack_fnPackStruct( const PGN_tzFIELD_DESC * XT_FAR pktzFieldDesc,
                                   uint16 uiFields,
                                   void *pvSource,
                                   uchar8 *pucDestBuf );
static uint16 xbpack_fnUnPackInt( uchar8 *pucSrcBuf );
static uint32 xbpack_fnUnPackLong( uchar8 *pucSrcBuf );
static float32 xbpack_fnUnPackFloat( uchar8 *pucSrcBuf );
static double64 xbpack_fnUnPackDouble( uchar8 *pucSrcBuf );
static uchar8 xbpack_fnUnPackBits2Byte( uchar8 *pucSrcBuf,
                                        uint16 uiBitOffset,
                                        uint16 uiBitCount );
static uint16 xbpack_fnUnPackBits2Int( uchar8 *pucSrcBuf,
                                       uint16 uiBitOffset,
                                       uint16 uiBitCount );
static uint32 xbpack_fnUnPackBits2Long( uchar8 *pucSrcBuf,
                                        uint16 uiBitOffset,
                                        uint16 uiBitCount );
static tucBOOL xbpack_fnUnPackField( const PGN_tzFIELD_DESC * XT_FAR pktzFieldDesc,
                                    uchar8 *pucSrcBuf,
                                    void *pvDest );
static tucBOOL xbpack_fnUnPackStruct( const PGN_tzFIELD_DESC * XT_FAR pktzFieldDesc,
                                     uint16 uiFields,
                                     uchar8 *pucSrcBuf,
                                     void *pvDest );

/*==============================================================================
                              Function Definitions
==============================================================================*/

/******************************************************************************

FUNCTION NAME:
    xbpack_fnPgnCmp         - compare pairs of PGN/GrpFunc

PURPOSE:
    Function to compare pairs of PGN/GrpFunc

INPUTS:
    'ulPgnA' is the PG Number A being compared
    'siGrpFuncA' is the Group Function A being compared, usually DONT_CARE
    'ulPgnB' is the PG Number B being compared
    'siGrpFuncB' is the Group Function B being compared, usually DONT_CARE

OUTPUTS:
    1 if A > B
    0 if A == B
    -1 if A < B

NOTES:
    This function is used by the binary search in XBPACK_fnGetPGNDesc()

VERSION HISTORY:

Version: 1.00   Date: May 12, 2003   By: Tom Lightfoot
  - Creation
Version: 1.01   Date: May 16, 2003   By: Dalem
  - Fix in comparing group function

******************************************************************************/

static schar8 xbpack_fnPgnCmp( uint32 ulPgnA,
                               sint16 siGrpFuncA,
                               uint32 ulPgnB,
                               sint16 siGrpFuncB )
{
    schar8 scResult;

    // Compare PGNs first
    if( ulPgnA > ulPgnB )
    {
        // A > B
        scResult = 1;
    }
    else if( ulPgnA < ulPgnB )
    {
        // A < B
        scResult = -1;
    }
    else
    {
        // Do we care about group functions?
        if( ( siGrpFuncA == ( sint16 )XB_eGRP_FUNC_VALUE_DO_NOT_CARE )
         || ( siGrpFuncB == ( sint16 )XB_eGRP_FUNC_VALUE_DO_NOT_CARE )
         || ( siGrpFuncA == siGrpFuncB ) )
        {
            // Match found ( A == B)
            scResult = 0;
        }
        else
        {
            if( siGrpFuncA > siGrpFuncB )
            {
                // A > B
                scResult = 1;
            }
            else
            {
                // A < B
                scResult = -1;
            }
        }
    }

    return( scResult );
}

/******************************************************************************

FUNCTION NAME:
    XBPACK_fnGetPgnInfo     - Look up field info for given PGN

PURPOSE:
    Look up the field info for a given pgn and group function code

INPUTS:
    'ulPgn' is the PGN of the message being converted
    'siGrpFunc' is the group function code of the message,
                    usually XB_eGRP_FUNC_VALUE_DO_NOT_CARE

OUTPUTS:
    A pointer in PGN which is relevant to the PGN and Group Code.
    NULL is returned if no match is found.

NOTES:
    The PGN list is over 100 PGNs long so a binary search is used.

VERSION HISTORY:

Version: 1.01   Date: August 12, 2003   By: Peter Drexel
  - Added check for NULL PGN descriptor upon finding PGN info
Version 1.02  Date: 09/05/03  By: Dalem
  - Removed check for NULL PGN descriptor upon finding PGN info

******************************************************************************/

const PGN_tzPGN_INFO * XBPACK_fnGetPgnInfo( uint32 ulPgn,
                                           sint16 siGrpFunc )
{
    const PGN_tzPGN_INFO *pzPgnInfo;
    uint16 uiLowIndex;
    uint16 uiHiIndex;
    uint16 uiMidIndex;
    schar8 scComparison;

    // The PGN table is sorted by PGN and group function so do a binary search
    uiLowIndex = 0;
    uiHiIndex = XBPGNTBL_fnSizeOfAllPgnList();

    // Get Searching!
    for( ; ; )
    {
        // Go for the middle
        uiMidIndex = ( uiLowIndex + uiHiIndex ) / 2;
        pzPgnInfo = &XB_katzALL_PGN_LIST[ uiMidIndex ];

        // Compare pgn/grp function pairs
        scComparison = xbpack_fnPgnCmp( ulPgn,
                                        siGrpFunc,
                                        pzPgnInfo->ulPgn,
                                        pzPgnInfo->siGrpFunc );
        if( scComparison == 0 )
        {
            // Found!
            return( pzPgnInfo );
        }
        else if( uiLowIndex == uiHiIndex )
        {
            // No such entry in the table
            return( (const PGN_tzPGN_INFO *) NULL );
        }
        else if( scComparison > 0 )
        {
            // What we're looking for is greater than the middle
            if( uiMidIndex == uiLowIndex )
            {
                // Boundary condition
                uiLowIndex++;
            }
            else
            {
                uiLowIndex = uiMidIndex;
            }
        }
        else
        {
            // What we're looking for is less than the middle
            uiHiIndex = uiMidIndex;
        }
    }
}

/******************************************************************************

FUNCTION NAME:
    xbpack_fnGetExtraCount  - get count of extras field

PURPOSE:
    Get the count of extras field in a PGN's structure

INPUTS:
    'pktzPgnDesc' points to the PGN's descriptor structure
    'pvStruct' points to the unpacked structure with the fixed length portion

OUTPUTS:
    The number of elements in the extras array
    zero if this is a fixed length PGN

NOTES:

VERSION HISTORY:

Version: 1.00   Date: May 12, 2003   By: Tom Lightfoot
  - First function header

******************************************************************************/

static uint16 xbpack_fnGetExtraCount( const PGN_tzPGN_DESC * XT_FAR pktzPgnDesc,
                                      void *pvStruct )
{
    // Verify parameter
    (void)assert( pktzPgnDesc != NULL );
    (void)assert( pvStruct != NULL );

    // Is this a variable length PGN?
    if( pktzPgnDesc->pktzVarList != NULL )
    {
        // The count of extra fields is always one field past
        // the regular data fields
        return( *(uint16 *)((uchar8 *)pvStruct +
            pktzPgnDesc->pktzFixedList[ pktzPgnDesc->uiFixedFields ].uiStructOffset ) );
    }
    else
    {
        // Not a variable length PGN so no extras
        return( 0 );
    }
}


/******************************************************************************

FUNCTION NAME:
    xbpack_fnSetExtraCount  - set count of extras field

PURPOSE:
    Set the count of extras field in a PGN's structure

INPUTS:
    'pktzPgnDesc' points to the PGN's descriptor structure
    'pvStruct' points to the unpacked structure with the fixed length portion
    'uiCount' is the size of the extras array to set

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version: 1.00   Date: May 12, 2003   By: Tom Lightfoot
  - First function header

******************************************************************************/

static void xbpack_fnSetExtraCount( const PGN_tzPGN_DESC * XT_FAR pktzPgnDesc,
                                    void *pvStruct,
                                    uint16 uiCount )
{
    // Verify parameter
    (void)assert( pktzPgnDesc != NULL );
    (void)assert( pvStruct != NULL );

    // Is this a variable length PGN?
    if( pktzPgnDesc->pktzVarList != NULL )
    {
        // The count of extra fields is always one field past
        // the regular data fields
        *(uint16 *)((uchar8 *)pvStruct +
            pktzPgnDesc->pktzFixedList[ pktzPgnDesc->uiFixedFields ].uiStructOffset ) = uiCount;
    }
}


/******************************************************************************

FUNCTION NAME:
    xbpack_fnGetExtraPoint  - get pointer to extras fields

PURPOSE:
    Get a pointer to the extras array in a PGN's structure

INPUTS:
    'pktzPgnDesc' points to the PGN's descriptor structure
    'pvStruct' points to the unpacked structure with the fixed length portion

OUTPUTS:
    A pointer to the array of extra structures if applicable
    NULL if this is a fixed-length PGN

NOTES:

VERSION HISTORY:

Version: 1.00   Date: May 12, 2003   By: Tom Lightfoot
  - First function header

******************************************************************************/

static void *xbpack_fnGetExtraPoint( const PGN_tzPGN_DESC * XT_FAR pktzPgnDesc,
                                     void *pvStruct )
{
    // Verify parameter
    (void)assert( pktzPgnDesc != NULL );
    (void)assert( pvStruct != NULL );

    // Is this a variable length PGN?
    if( pktzPgnDesc->pktzVarList != NULL )
    {
        // The count of extra fields is always one field past
        // the regular data fields
        return( *(void **)((uchar8 *)pvStruct +
            pktzPgnDesc->pktzFixedList[ pktzPgnDesc->uiFixedFields + 1 ].uiStructOffset ) );
    }
    else
    {
        // Not a variable length PGN so no extras
        return( NULL );
    }
}


/******************************************************************************

FUNCTION NAME:
    xbpack_fnSetExtraPoint  - set pointer to extras fields

PURPOSE:
    Set the pointer to the extras array in a PGN's structure

INPUTS:
    'pktzPgnDesc' points to the PGN's descriptor structure
    'pvStruct' points to the unpacked structure with the fixed length portion
    'pvExtras' points to the unpacked variable length portion

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version: 1.00   Date: May 12, 2003   By: Tom Lightfoot
  - First function header

******************************************************************************/

static void xbpack_fnSetExtraPoint( const PGN_tzPGN_DESC * XT_FAR pktzPgnDesc,
                                    void *pvStruct,
                                    void *pvExtras )
{
    // Verify parameter
    (void)assert( pktzPgnDesc != NULL );
    (void)assert( pvStruct != NULL );

    // Is this a variable length PGN?
    if( pktzPgnDesc->pktzVarList != NULL )
    {
        // The count of extra fields is always one field past
        // the regular data fields
        *(void **)((uchar8 *)pvStruct +
            pktzPgnDesc->pktzFixedList[ pktzPgnDesc->uiFixedFields + 1 ].uiStructOffset ) = pvExtras;
    }
}

/******************************************************************************
*
*  Message packing functions start here
*
******************************************************************************/

/******************************************************************************

FUNCTION NAME:
    xbpack_fnPackInt        - pack an int value

PURPOSE:
    To put an integer value into a byte-oriented message buffer

INPUTS:
    'uiSource' is the integer to put into the buffer
    'pucDest' points to the location in the buffer to write to

OUTPUTS:
    Nothing

NOTES:
    Bytes are inserted lowest order first, regardless of the processor's
    native byte ordering.

VERSION HISTORY:

Version: 1.00   Date: Nov 21, 2002   By: Tom Lightfoot
  - Initial version

******************************************************************************/
static void xbpack_fnPackInt( uint16 uiSource,
                              uchar8 *pucDest )
{
    // Verify parameter
    (void)assert( pucDest != NULL );


    *pucDest++ = (uchar8)( uiSource & 0x00ff );
    *pucDest = (uchar8)(( uiSource >> 8 ) & 0x00ff );

}

/******************************************************************************

FUNCTION NAME:
    xbpack_fnPackLong       - pack a long value

PURPOSE:
    To put an integer value into a byte-oriented message buffer

INPUTS:
    'ulSource' is the integer to put into the buffer
    'pucDest' points to the location in the buffer to write to

OUTPUTS:
    Nothing

NOTES:
    Bytes are inserted lowest order first, regardless of the processor's
    native byte ordering.

VERSION HISTORY:

Version: 1.00   Date: Nov 21, 2002   By: Tom Lightfoot
  - Initial version

******************************************************************************/
static void xbpack_fnPackLong( uint32 ulSource,
                               uchar8 *pucDest )
{
    // Verify parameter
    (void)assert( pucDest != NULL );


    *pucDest++ = (uchar8)( ulSource & 0x00ff );
    *pucDest++ = (uchar8)(( ulSource >> 8 ) & 0x000000ff );
    *pucDest++ = (uchar8)(( ulSource >> 16 ) & 0x000000ff );
    *pucDest   = (uchar8)(( ulSource >> 24 ) & 0x000000ff );

}

/******************************************************************************

FUNCTION NAME:
    xbpack_fnPackFloat      - pack a float value

PURPOSE:
    To put a floating point value into a byte-oriented message buffer

INPUTS:
    'fSource' is the value to put into the buffer
    'pucDest' points to the location in the buffer to write to

OUTPUTS:
    Nothing

NOTES:
    This function may not be portable, depending on the floating point
    encoding of the processors involved.

    ***This code is completely untested!

VERSION HISTORY:

Version: 1.00   Date: Nov 21, 2002   By: Tom Lightfoot
  - Initial version

******************************************************************************/
static void xbpack_fnPackFloat( float32 fSource,
                                uchar8 *pucDest )
{
    // Verify parameter
    (void)assert( pucDest != NULL );

    // Just copy straight to the buffer
    (void)memcpy( pucDest,
                  &fSource,
                  sizeof( fSource ) );
}

/******************************************************************************

FUNCTION NAME:
    xbpack_fnPackDouble     - pack a double value

PURPOSE:
    To put a floating point value into a byte-oriented message buffer

INPUTS:
    'fSource' is the value to put into the buffer
    'pucDest' points to the location in the buffer to write to

OUTPUTS:
    Nothing

NOTES:
    This function may not be portable, depending on the floating point
    encoding of the processors involved.

    ***This code is completely untested!

VERSION HISTORY:

Version: 1.00   Date: Nov 21, 2002   By: Tom Lightfoot
  - Initial version

******************************************************************************/
static void xbpack_fnPackDouble( double64 dSource,
                                 uchar8 *pucDest )
{
    // Verify parameter
    (void)assert( pucDest != NULL );

    // Just copy straight to the buffer
    (void) memcpy( pucDest,
                   &dSource,
                   sizeof( dSource ) );
}

/******************************************************************************

FUNCTION NAME:
    xbpack_fnPackBits       - pack bit field

PURPOSE:
    To put a bit field into a byte-oriented message buffer

INPUTS:
    'ulSource' is the numeric value of the bit field
    'pucDest' points to the location in the buffer to write to
    'uiBitOffset' is the offset within first byte to write to, counts from LSB
    'uiBitCount' is the number of bits to write

OUTPUTS:
    Nothing

NOTES:
    This function writes the least significant byte first.  In the case of
    a bit offset, the least significant bits are left unchanged.

VERSION HISTORY:

Version: 1.00   Date: Nov 21, 2002   By: Tom Lightfoot
  - Initial version

******************************************************************************/
static void xbpack_fnPackBits( uint32 ulSource,
                               uchar8 *pucDest,
                               uint16 uiBitOffset,
                               uint16 uiBitCount )
{
    uint16 uiBlockSize;   // Number of bits to copy to current byte
    uint16 uiUnused;      // Bits unused at the upper end of the byte.
    uchar8 ucMask;        // Non-interference mask

    // Verify parameter
    (void)assert( pucDest != NULL );

    // Keep the bits in order but load the lowest order part of the byte available
    while( uiBitCount )
    {
        // How many bits are we copying in this iteration?
        if( uiBitCount <= 8 - uiBitOffset )
        {
            uiBlockSize = uiBitCount;
        }
        else
        {
            uiBlockSize = 8 - uiBitOffset;
        }

        uiBitCount -= uiBlockSize;

        // Make up the mask
        ucMask = 0xff << uiBitOffset;
        uiUnused = ( 8 - uiBlockSize ) - uiBitOffset;
        if( uiUnused > 0 )
        {
            ucMask &= 0xff >> (uchar8) uiUnused;
        }

        // Zero out the field we're about to write
        *pucDest &= ~ucMask;

        // Now write the field without wiping out the bits outside our influence
        *pucDest |= ucMask & ( ulSource << uiBitOffset );

        // Go to the next chunk
        uiBitOffset = 0;
        ulSource = ulSource >> (uchar8) uiBlockSize;
        pucDest++;
    }
}

/******************************************************************************

FUNCTION NAME:
    xbpack_fnPackField      - pack a field of a structure

PURPOSE:
    To pack a given field from a structure to a message buffer, according
    to a field descriptor which indicates the locations of the ingoing
    and outgoing fields and the data type.

INPUTS:
    'pktzFieldDesc' points to the field descriptor
    'pvSource'     points to the beginning of the structure to be packed
    'pucDestBuf'  points to the beginning of the outgoing message buffer

OUTPUTS:
    TRUE if the function is successful,
    FALSE if the data type is invalid.

NOTES:
    This function is called by XBPACK_fnPackPgn which makes sure that the
    structure and buffer are appropriately sized.

VERSION HISTORY:

Version: 1.00  Date: Nov 21, 2002  By: Tom Lightfoot
    - Initial version
Version: 1.01  Date: Apr 2, 2003  By: Tom Lightfoot
    - Allowed int-sized enums to pack into bit fields <= 8 bits

******************************************************************************/
static tucBOOL xbpack_fnPackField( const PGN_tzFIELD_DESC * XT_FAR pktzFieldDesc,
                                  void *pvSource,
                                  uchar8 *pucDestBuf )
{
    uint16 uiMsgByteOffset;   // Message offset in bytes
    uint16 uiMsgBitOffset;    // Remainder bits in message offset
    uint16 uiCount;           // Counter for arrays of fields
    uint32 ulLongVal;         // Intermediate holder for longs
    uint16 uiIntVal;          // Intermediate holder for integers
    uchar8 ucByteVal;         // Intermediate holder for bytes

    // Verify parameter
    (void)assert( pktzFieldDesc != NULL );
    (void)assert( pvSource != NULL );
    (void)assert( pucDestBuf != NULL );

    // Update the destination pointer to point to the data field
    uiMsgByteOffset = pktzFieldDesc->uiMsgOffsetBits / 8;
    uiMsgBitOffset = pktzFieldDesc->uiMsgOffsetBits % 8;
    pucDestBuf += uiMsgByteOffset;

    // Update the source pointer to point to the structure element
    pvSource = (uchar8 *)pvSource + pktzFieldDesc->uiStructOffset;

    // Action depends on the type of the field
    switch( pktzFieldDesc->teType )
    {
        case PGN_eTYPE_BIT:
        {
            // Target data size depends on number of bits
            if( pktzFieldDesc->uiFieldCount > 32 )
            {
                // Won't fit
                return( FALSE );
            }
            else if( pktzFieldDesc->uiFieldCount > 16 )
            {
                // Use a long
                ulLongVal = *(uint32 *)pvSource;
                xbpack_fnPackBits( ulLongVal,
                                   pucDestBuf,
                                   uiMsgBitOffset,
                                   pktzFieldDesc->uiFieldCount );
            }
            else if( ( pktzFieldDesc->uiFieldCount > 8 )
                  || ( pktzFieldDesc->uiStructSize == 2 ) )
            {
                // Use an int
                uiIntVal = *(uint16 *)pvSource;
                xbpack_fnPackBits( (uint32)uiIntVal,
                                   pucDestBuf,
                                   uiMsgBitOffset,
                                   pktzFieldDesc->uiFieldCount );
            }
            else
            {
                // Use a byte
                ucByteVal = XT_mMK_BYTE( *(uchar8 *)pvSource );
                xbpack_fnPackBits( (uint32)ucByteVal,
                                   pucDestBuf,
                                   uiMsgBitOffset,
                                   pktzFieldDesc->uiFieldCount );
            }
        }
        break;

        case PGN_eTYPE_UCHAR:
        case PGN_eTYPE_SCHAR:
        {
            for( uiCount = 0; uiCount < pktzFieldDesc->uiFieldCount; uiCount++ )
            {
                // Casting depends on source element size which depends on the
                // structure record size and the number of records
                switch ( pktzFieldDesc->uiStructSize / pktzFieldDesc->uiFieldCount )
                {
                    case 1:
                    {
                        // Element is a byte or char
                        *( pucDestBuf + uiCount ) =
                           XT_mMK_BYTE( *( (uchar8 *)pvSource + uiCount ) ) ;
                    }
                    break;

                    case 2:
                    {
                        // Element is a word or short int
                        *( pucDestBuf + uiCount ) = (uchar8)
                           XT_mMK_BYTE( *( (uint16 *)pvSource + uiCount ) );
                    }
                    break;

                    case 4:
                    {
                        // Element is a double word or long int
                        *( pucDestBuf + uiCount ) = (uchar8)
                           XT_mMK_BYTE( *( (uint32 *)pvSource + uiCount ) );
                    }
                    break;

                    default:
                    {
                        // Save error
                        XBINIT_fnSaveError( PGN_eESRC_PACK,
                                            ( uint16 )PGN_eERR_FIELD_SIZE );
                    }
                    break;
                }
            }
        }
        break;

        case PGN_eTYPE_UINT:
        case PGN_eTYPE_SINT:
        {
            for( uiCount = 0; uiCount < pktzFieldDesc->uiFieldCount; uiCount++ )
            {
                uiIntVal = *( (uint16 *)pvSource + uiCount );
                xbpack_fnPackInt( uiIntVal,
                                  pucDestBuf + ( uiCount * sizeof( uint16 ) ) );
            }
        }
        break;

        case PGN_eTYPE_ULONG:
        case PGN_eTYPE_SLONG:
        {
            for( uiCount = 0; uiCount < pktzFieldDesc->uiFieldCount; uiCount++ )
            {
                ulLongVal = *( (uint32 *)pvSource + uiCount );
                xbpack_fnPackLong( ulLongVal,
                                   pucDestBuf + ( uiCount * sizeof( uint32 ) ) );
            }
        }
        break;

        case PGN_eTYPE_FLOAT:
        {
            xbpack_fnPackFloat( *(float32 *)pvSource,
                                pucDestBuf );
        }
        break;

        case PGN_eTYPE_DOUBLE:
        {
            xbpack_fnPackDouble( *(double64 *)pvSource,
                                 pucDestBuf );
        }
        break;

        case PGN_eTYPE_STRING:
        case PGN_eTYPE_POINTER:
        case PGN_eTYPE_UNDEFINED:
        default:
        {
            // Unsupported type
            return( FALSE );
        }
    }

    // The packing was a big success
    return( TRUE );
}


/******************************************************************************

FUNCTION NAME:
    xbpack_fnPackStruct     - pack all fields of a structure

PURPOSE:
    To pack all of the fields within a structure

INPUTS:
    'pktzFieldDesc' points to the array of field descriptions for the structure
    'uiFields' is the number of fields defiend by the pktzFieldDesc arrray
    'pvSource' points to the unpacked structure
    'pucDestBuf' points to the beginning of the packed structure in the buffer

OUTPUTS:
    TRUE if the function is successful
    FALSE otherwise

NOTES:

VERSION HISTORY:

Version: 1.00   Date: Nov 25, 2002   By: Tom Lightfoot
  - Initial version

******************************************************************************/

static tucBOOL xbpack_fnPackStruct( const PGN_tzFIELD_DESC * XT_FAR pktzFieldDesc,
                                   uint16 uiFields,
                                   void *pvSource,
                                   uchar8 *pucDestBuf )
{
    uint16 uiField;                  // Counts through the PGN fields

    // Verify parameter
    (void)assert( pktzFieldDesc != NULL );
    (void)assert( pvSource != NULL );
    (void)assert( pucDestBuf != NULL );

    // Go through all of the fields in the list
    uiField = 0;
    while( uiField < uiFields )
    {
        // Use the general field packing function
        if( xbpack_fnPackField( pktzFieldDesc + uiField,
                                 pvSource,
                                 pucDestBuf ) == FALSE )
        {
            // Function was unsuccessful
            return( FALSE );
        }

        // Go to the next field
        uiField++;
    }

    // The structure was packed successfully
    return( TRUE );
}

/******************************************************************************

FUNCTION NAME:
    XBPACK_fnPackPgn        - convert a structure into a XanBus message

PURPOSE:
    To pack information from a message-specific structure into a buffer
    to be sent to XanBus.

INPUTS:
    'pktzPgnDesc' points to the descriptor for the PGN
    'pvSource' points to the structure
    'uiSrcSize' is the size of the structure
    'puiDestLen' is the length of the outgoing message buffer

OUTPUTS:
    A pointer to the newly created destination buffer
    *puiDestLen set to 0xFFFF upon error

NOTES:
    This function also changes the value of *puiDestLen to the length
    of the newly created message buffer.  Dispose of the buffer with
    XBMEM_fnFree when done.

VERSION HISTORY:

Version: 1.00   Date: Nov 21, 2002   By: Tom Lightfoot
  - Initial version
Version: 1.01   Date: Nov 25, 2002   By: Tom Lightfoot
  - Now creates the destination buffer and copies with variable length PGNs
Version: 1.02   Date: Nov 28, 2002   By: Dale Mernett
  - Save error codes on failures
  - Free memory on failures
Version: 1.03   Date: Feb 07, 2003   By: Dale Mernett
  - Set DestLen to 0xFFFF on an error -- NULL pointer is allowed
Version: 1.04   Date: May 12, 2003   By: Tom Lightfoot
  - Separated search for descriptor from packing
Version 1.05  Date: 09/05/03  By: Dalem
    - Check descriptor for NULL
Version 1.06  Date: November 21, 2017  By: DoruB
    - Ensure all transmitted packets are 8 bytes to conform with section 3.1
      of the RV-C Revised Application Layer document dated December 17, 2015.

******************************************************************************/
uchar8 *XBPACK_fnPackPgn( const PGN_tzPGN_DESC * XT_FAR pktzPgnDesc,
                          void *pvSource,
                          uint16 uiSrcSize,
                          uint16 *puiDestLen )
{
    uchar8 *pucDestBuf;                // The target message buffer
    uchar8 *pucDestBlock;              // Points to an extra block in DestBuf
    void *pSrcBlock;                   // Points to an extra block in Source
    uint16 uiExtraCount;               // The number of extra blocks
    uint16 uiBlock;                    // Counts through the extra blocks

    // Verify parameter
    (void)assert( pktzPgnDesc != NULL );
    (void)assert( pvSource != NULL );
    (void)assert( puiDestLen != NULL );

    // If there is no descriptor
    if( pktzPgnDesc == NULL )
    {
        // Save error
        XBINIT_fnSaveError( PGN_eESRC_PACK,
                            ( uint16 )PGN_eERR_NO_PGDESC );

        // No descriptor
        *puiDestLen = PGN_UI_NO_DATA;
        return( NULL );
    }

    // Make sure that the output structure is as big as expected
    if( uiSrcSize < pktzPgnDesc->uiFixedSize )
    {
        // Save error
        XBINIT_fnSaveError( PGN_eESRC_PACK,
                            ( uint16 )PGN_eERR_BUF_SIZE );

        // Too small
        *puiDestLen = PGN_UI_NO_DATA;
        return( NULL );
    }

    // Calculate the size of the destination buffer
    uiExtraCount = xbpack_fnGetExtraCount( pktzPgnDesc,
                                           pvSource );
    *puiDestLen = pktzPgnDesc->uiFixedLen + ( uiExtraCount * pktzPgnDesc->uiVarLen );

    // If message length is zero
    if( *puiDestLen == 0 )
    {
        // Done packing -- this is not an error
        return( NULL );
    }

    // Ensure all transmitted packets are 8 bytes to conform with section 3.1
    // of the RV-C Revised Application Layer document dated December 17, 2015.
    if( *puiDestLen < RVC_CAN_FRAME_DLC )
    {
        *puiDestLen = RVC_CAN_FRAME_DLC;
    }

    // Allocate the memory for the buffer
    pucDestBuf = XBMEM_fnMalloc( *puiDestLen );
    if( pucDestBuf == NULL )
    {
        // Save error
        XBINIT_fnSaveError( PGN_eESRC_PACK,
                            ( uint16 )PGN_eERR_NO_MEM );

        // Could not allocate any memory
        *puiDestLen = PGN_UI_NO_DATA;
        return( NULL );
    }

    // Any unspecified reserved fields need to be set to 0xFF, so
    // Initialize the whole buffer with 0xFF
    (void) memset( pucDestBuf,
                   PGN_UC_NO_DATA,
                   *puiDestLen );

    // Pack all of the fields in the fixed portion of the message
    if( xbpack_fnPackStruct( pktzPgnDesc->pktzFixedList,
                              pktzPgnDesc->uiFixedFields,
                              pvSource, pucDestBuf ) == FALSE )
    {
        // Save error
        XBINIT_fnSaveError( PGN_eESRC_PACK,
                            ( uint16 )PGN_eERR_PACK );

        // Release the allocated memory
        XBMEM_fnFree( pucDestBuf );

        // Function was unsuccessful
        *puiDestLen = PGN_UI_NO_DATA;
        return( NULL );
    }

    // Pack all of the extra structures, if any
    uiBlock = 0;
    pSrcBlock = xbpack_fnGetExtraPoint( pktzPgnDesc,
                                        pvSource );
    pucDestBlock = pucDestBuf + pktzPgnDesc->uiFixedLen;
    while( uiBlock < uiExtraCount )
    {
        // Pack each extra block
        if( xbpack_fnPackStruct( pktzPgnDesc->pktzVarList,
                                  pktzPgnDesc->uiVarFields,
                                  pSrcBlock,
                                  pucDestBlock ) == FALSE )
        {
            // Save error
            XBINIT_fnSaveError( PGN_eESRC_PACK,
                                ( uint16 )PGN_eERR_PACK_MORE );

            // Release the allocated memory
            XBMEM_fnFree( pucDestBuf );

            // Function was unsuccessful
            *puiDestLen = PGN_UI_MAX;
            return( NULL );
        }

        // Go to the next extra block
        uiBlock++;
        pSrcBlock = (uchar8 *)pSrcBlock + pktzPgnDesc->uiVarSize;
        pucDestBlock = pucDestBlock + pktzPgnDesc->uiVarLen;
    }

    // The pack was a big success
    return( pucDestBuf );
}

/******************************************************************************
*
*  Message unpacking functions start here
*
******************************************************************************/

/******************************************************************************

FUNCTION NAME:
    xbpack_fnUnPackInt      - unpack an integer value

PURPOSE:
    To retrieve an integer from a message buffer

INPUTS:
    'pucSrcBuf' points to the first byte of the integer field within the buffer

OUTPUTS:
    The retrieved integer value

NOTES:
    All integers within the buffer are stored least significant byte first
    regardless of the native byte ordering for the processor.

VERSION HISTORY:

Version: 1.00   Date: Nov 21, 2002   By: Tom Lightfoot
  - Initial version

******************************************************************************/
static uint16 xbpack_fnUnPackInt( uchar8 *pucSrcBuf )
{
    uint16 uiDest;    // The value returned by this function

    // Verify parameter
    (void)assert( pucSrcBuf != NULL );


    uiDest = (uint16)( *pucSrcBuf );
    pucSrcBuf++;
    uiDest += (uint16)( *pucSrcBuf ) << 8;


    return( uiDest );
}

/******************************************************************************

FUNCTION NAME:
    xbpack_fnUnPackLong     - unpack a long value

PURPOSE:
    To retrieve an integer from a message buffer

INPUTS:
    'pucSrcBuf' points to the first byte of the integer field within the buffer

OUTPUTS:
    The retrieved long integer value

NOTES:
    All integers within the buffer are stored least significant byte first
    regardless of the native byte ordering for the processor.

VERSION HISTORY:

Version: 1.00   Date: Nov 21, 2002   By: Tom Lightfoot
  - Initial version

******************************************************************************/
static uint32 xbpack_fnUnPackLong( uchar8 *pucSrcBuf )
{
    uint32 ulDest;    // The value returned by this function

    // Verify parameter
    (void)assert( pucSrcBuf != NULL );


    ulDest = (uint32)( *pucSrcBuf );
    pucSrcBuf++;
    ulDest += (uint32)( *pucSrcBuf ) << 8;
    pucSrcBuf++;
    ulDest += (uint32)( *pucSrcBuf ) << 16;
    pucSrcBuf++;
    ulDest += (uint32)( *pucSrcBuf ) << 24;

    return( ulDest );
}

/******************************************************************************

FUNCTION NAME:
    xbpack_fnUnPackFloat    - unpack a float value

PURPOSE:
    To retrieve a floating point number from a message buffer

INPUTS:
    'pucSrcBuf' points to the number within the message buffer

OUTPUTS:
    The retrieved floating point number

NOTES:
    This function may not be portable, depending on how the processor
    stores floating point numbers

    ***This code is completely untested!

VERSION HISTORY:

Version: 1.00   Date: Nov 21, 2002   By: Tom Lightfoot
  - Initial version

******************************************************************************/
static float32 xbpack_fnUnPackFloat( uchar8 *pucSrcBuf )
{
    float32 fDest;    // The value returned by this function

    // Verify parameter
    (void)assert( pucSrcBuf != NULL );

    // Just copy straight from the buffer
    (void) memcpy( &fDest,
                   pucSrcBuf,
                   sizeof( fDest ) );

    return( fDest );
}

/******************************************************************************

FUNCTION NAME:
    xbpack_fnUnPackDouble   - unpack a double value

PURPOSE:
    To retrieve a double precision floating point number from a message buffer

INPUTS:
    'pucSrcBuf' points to the number within the message buffer

OUTPUTS:
    The retrieved floating point number

NOTES:
    This function may not be portable, depending on how the processor
    stores floating point numbers

    ***This code is completely untested!

VERSION HISTORY:

Version: 1.00   Date: Nov 21, 2002   By: Tom Lightfoot
  - Initial version

******************************************************************************/
static double64 xbpack_fnUnPackDouble( uchar8 *pucSrcBuf )
{
    double64 dDest;    // The value returned by this function

    // Verify parameter
    (void)assert( pucSrcBuf != NULL );

    // Just copy straight from the buffer
    (void) memcpy( &dDest,
                   pucSrcBuf,
                   sizeof( dDest ) );

    return( dDest );
}

/******************************************************************************

FUNCTION NAME:
    xbpack_fnUnPackBits2Byte - unpack an 8 bit bit field

PURPOSE:
    To unpack a bit field of 8 bits or less into a byte value

INPUTS:
    'pucSrcBuf' points to the first byte of the bit field within the buffer
    'uiBitOffset' is how many bits of the first byte are used by another field
    'uiBitCount' is how many bits to copy

OUTPUTS:
    The numeric value of the bit field

NOTES:
    Bit fields are encoded least significant byte first.  Bit offsets index
    from the least significant bit upwards.

VERSION HISTORY:

Version: 1.00   Date: Nov 21, 2002   By: Tom Lightfoot
  - Initial version

******************************************************************************/
static uchar8 xbpack_fnUnPackBits2Byte( uchar8 *pucSrcBuf,
                                        uint16 uiBitOffset,
                                        uint16 uiBitCount )
{
    uchar8 ucDest;        // The value returned by this function
    uint16 uiByteMax;     // Count of num of bytes that bit mask spans, less one
    sint16 siByte;        // The byte counter
    uint16 uiBlockSize;   // Number of bits to copy in current iteration
    uint16 uiUnused;      // Bits unused at the upper end of the byte
    uchar8 ucBlock;       // Byte retrieved from pucSrcBuf
    uchar8 ucMask;        // Non-interference mask

    // Verify parameter
    (void)assert( pucSrcBuf != NULL );

    // Load bit chunks from highest order first so where is the other end
    // of the source buffer?
    uiByteMax = (( uiBitOffset + uiBitCount + ( XT_BITS_PER_BYTE - 1 ) ) /
                    XT_BITS_PER_BYTE ) - 1;

    // Initialize the destination variable
    ucDest = 0;

    // Keep bits in order but load bits offset from the lowest order end
    // Byte ordering in buffer is least significant byte first.
    // We'll load the bytes from the far (most significant) end first.
    for( siByte = ( sint16 )uiByteMax; siByte >= 0; siByte-- )
    {
        // Grab the byte from the buffer
        ucBlock = *( pucSrcBuf + siByte );

        // No unused bits in this byte until proven otherwise
        uiUnused = 0;

        // Is this the first, Least Significant Byte?
        if( siByte == 0 )
        {
            // Slide down by the bit offset
            ucBlock = ucBlock >> uiBitOffset;
            uiUnused = uiBitOffset;
        }

        // Is this the last, Most Significant Byte?
        if( siByte == ( sint16 )uiByteMax )
        {
            // There may be bits at the top end of this byte that are unused
            uiUnused += ( XT_BITS_PER_BYTE - (( uiBitOffset +
                        uiBitCount ) % XT_BITS_PER_BYTE )) % XT_BITS_PER_BYTE;
        }

        // Calculate the mask
        ucMask = (uchar8) ( 0xFF >> (uchar8) uiUnused );

        // Calculate the number of bits in the current block
        uiBlockSize = XT_BITS_PER_BYTE - uiUnused;

        // Slide any previously copied bits from the buffer
        ucDest = ucDest << uiBlockSize;

        // Grab the used bits from the buffer
        ucDest |= ( ucBlock & ucMask );
    }

    return( ucDest );
}

/******************************************************************************

FUNCTION NAME:
    xbpack_fnUnPackBits2Int - unpack a 16 bit bit field

PURPOSE:
    To unpack a bit field of 16 bits or less into an int value

INPUTS:
    'pucSrcBuf' points to the first byte of the bit field within the buffer
    'uiBitOffset' is how many bits of the first byte are used by another field
    'uiBitCount' is how many bits to copy

OUTPUTS:
    The numeric value of the bit field

NOTES:
    Bit fields are encoded least significant byte first.  Bit offsets index
    from the least significant bit upwards.

VERSION HISTORY:

Version: 1.00   Date: Nov 21, 2002   By: Tom Lightfoot
  - Initial version

******************************************************************************/
static uint16 xbpack_fnUnPackBits2Int( uchar8 *pucSrcBuf,
                                       uint16 uiBitOffset,
                                       uint16 uiBitCount )
{
    uint16 uiDest;        // The value returned by this function
    uint16 uiByteMax;     // Count of num of bytes that bit mask spans, less one
    sint16 siByte;        // The byte counter
    uint16 uiBlockSize;   // Number of bits to copy in current iteration
    uint16 uiUnused;      // Bits unused at the upper end of the byte
    uchar8 ucBlock;       // Byte retrieved from pucSrcBuf
    uchar8 ucMask;        // Non-interference mask

    // Verify parameter
    (void)assert( pucSrcBuf != NULL );

    // Load bit chunks from highest order first so where is the other end
    // of the source buffer?
    uiByteMax = (( uiBitOffset + uiBitCount + ( XT_BITS_PER_BYTE - 1 ) ) /
                    XT_BITS_PER_BYTE ) - 1;

    // Initialize the destination variable
    uiDest = 0;

    // Keep bits in order but load bits offset from the lowest order end
    // Byte ordering in buffer is least significant byte first.
    // We'll load the bytes from the far (most significant) end first.
    for( siByte = ( sint16 )uiByteMax; siByte >= 0; siByte-- )
    {
        // Grab the byte from the buffer
        ucBlock = *( pucSrcBuf + siByte );

        // No unused bits in this byte until proven otherwise
        uiUnused = 0;

        // Is this the first, Least Significant Byte?
        if( siByte == 0 )
        {
            // Slide down by the bit offset
            ucBlock = ucBlock >> uiBitOffset;
            uiUnused = uiBitOffset;
        }

        // Is this the last, Most Significant Byte?
        if( siByte == ( sint16 )uiByteMax )
        {
            // There may be bits at the top end of this byte that are unused
            uiUnused += ( XT_BITS_PER_BYTE - (( uiBitOffset + uiBitCount ) %
                        XT_BITS_PER_BYTE )) % XT_BITS_PER_BYTE;
        }

        // Calculate the mask
        ucMask = (uchar8) ( 0xFF >> (uchar8) uiUnused );

        // Calculate the number of bits in the current block
        uiBlockSize = XT_BITS_PER_BYTE - uiUnused;

        // Slide any previously copied bits from the buffer
        uiDest = uiDest << (uchar8) uiBlockSize;

        // Grab the used bits from the buffer
        uiDest |= ( ucBlock & ucMask );
    }

    return( uiDest );
}

/******************************************************************************

FUNCTION NAME:
    xbpack_fnUnPackBits2Long - unpack a 32 bit bit field

PURPOSE:
    To unpack a bit field of 64 bits or less into a long int value

INPUTS:
    'pucSrcBuf' points to the first byte of the bit field within the buffer
    'uiBitOffset' is how many bits of the first byte are used by another field
    'uiBitCount' is how many bits to copy

OUTPUTS:
    The numeric value of the bit field

NOTES:
    Bit fields are encoded least significant byte first.  Bit offsets index
    from the least significant bit upwards.

VERSION HISTORY:

Version: 1.00   Date: Nov 21, 2002   By: Tom Lightfoot
  - Initial version

******************************************************************************/
static uint32 xbpack_fnUnPackBits2Long( uchar8 *pucSrcBuf,
                                        uint16 uiBitOffset,
                                        uint16 uiBitCount )
{
    uint32 ulDest;       // The value returned by this function
    uint16 uiByteMax;     // Count of num of bytes that bit mask spans, less one
    sint16 siByte;        // The byte counter
    uint16 uiBlockSize;   // Number of bits to copy in current iteration
    uint16 uiUnused;      // Bits unused at the upper end of the byte
    uchar8 ucBlock;       // Byte retrieved from pucSrcBuf
    uchar8 ucMask;        // Non-interference mask

    // Verify parameter
    (void)assert( pucSrcBuf != NULL );

    // Load bit chunks from highest order first so where is the other end
    // of the source buffer?
    uiByteMax = (( uiBitOffset + uiBitCount + ( XT_BITS_PER_BYTE - 1 ) ) /
                XT_BITS_PER_BYTE ) - 1;

    // Initialize the destination variable
    ulDest = 0;

    // Keep bits in order but load bits offset from the lowest order end
    // Byte ordering in buffer is least significant byte first.
    // We'll load the bytes from the far (most significant) end first.
    for( siByte = ( sint16 )uiByteMax; siByte >= 0; siByte-- )
    {
        // Grab the byte from the buffer
        ucBlock = *( pucSrcBuf + siByte );

        // No unused bits in this byte until proven otherwise
        uiUnused = 0;

        // Is this the first, Least Significant Byte?
        if( siByte == 0 )
        {
            // Slide down by the bit offset
            ucBlock = ucBlock >> uiBitOffset;
            uiUnused = uiBitOffset;
        }

        // Is this the last, Most Significant Byte?
        if( siByte == ( sint16 )uiByteMax )
        {
            // There may be bits at the top end of this byte that are unused
            uiUnused += ( XT_BITS_PER_BYTE - (( uiBitOffset + uiBitCount ) %
                        XT_BITS_PER_BYTE )) % XT_BITS_PER_BYTE;
        }

        // Calculate the mask
        ucMask = (uchar8) ( 0xFF >> (uchar8) uiUnused );

        // Calculate the number of bits in the current block
        uiBlockSize = 8 - uiUnused;

        // Slide any previously copied bits from the buffer
        ulDest = ulDest << (uchar8) uiBlockSize;

        // Grab the used bits from the buffer
        ulDest |= ( ucBlock & ucMask );
    }

    return( ulDest );
}


/******************************************************************************

FUNCTION NAME:
    xbpack_fnUnPackField    - unpack one field of a structure

PURPOSE:
    A function to copy the contents of one field of a received message
    into a structure

INPUTS:
    'pktzFieldDesc' points to the field descriptor
    'pucSrcBuf' points to the beginning of the message buffer
    'pvDest' points to the beginning of the destination structure

OUTPUTS:
    TRUE if the function is successful
    FALSE otherwise

NOTES:

VERSION HISTORY:

Version: 1.00   Date: Nov 21, 2002   By: Tom Lightfoot
    - Initial version
Version: 1.01   Date: Apr 2, 2003    By: Tom Lightfoot
    - Small bitfields can unpack to int if the dest field is big enough
Version: 1.02   Date: Jun 2, 2003    By: Dalem
    - Check for unpacking to 32bit enums
Version 1.03    Date: Aug 12, 2003   By: Peter Drexel
    - Added check for 32-bit int destination from 16-bit int source

******************************************************************************/
static tucBOOL xbpack_fnUnPackField( const PGN_tzFIELD_DESC * XT_FAR pktzFieldDesc,
                                    uchar8 *pucSrcBuf,
                                    void *pvDest )
{
    uint16 uiMsgByteOffset;   // Message offset in bytes
    uint16 uiMsgBitOffset;    // Remainder bits in message offset
    uint16 uiCount;           // Counter for arrays of fields

    // Verify parameter
    (void)assert( pktzFieldDesc != NULL );
    (void)assert( pucSrcBuf != NULL );
    (void)assert( pvDest != NULL );

    // Update the source pointer to point to the data field
    uiMsgByteOffset = pktzFieldDesc->uiMsgOffsetBits / XT_BITS_PER_BYTE;
    uiMsgBitOffset = pktzFieldDesc->uiMsgOffsetBits % XT_BITS_PER_BYTE;
    pucSrcBuf += uiMsgByteOffset;

    // Update the destination pointer to point to the structure element
    pvDest = (uchar8 *)pvDest + pktzFieldDesc->uiStructOffset;

    // Action depends on the type of the field
    switch( pktzFieldDesc->teType )
    {
        case PGN_eTYPE_BIT:
        {
            // Target data size depends on number of bits
            if( pktzFieldDesc->uiFieldCount > XBPACK_BITS_PER_DWORD )
            {
                // Won't fit
                return( FALSE );
            }
            else if( ( pktzFieldDesc->uiFieldCount > XBPACK_BITS_PER_WORD ) ||
                ( pktzFieldDesc->uiStructSize == sizeof( uint32 ) ) )
            {
                // Use a long
                *(uint32 *)pvDest = xbpack_fnUnPackBits2Long( pucSrcBuf,
                                                             uiMsgBitOffset,
                                                             pktzFieldDesc->uiFieldCount );
            }
            else if( ( pktzFieldDesc->uiFieldCount > XT_BITS_PER_BYTE )
                  || ( pktzFieldDesc->uiStructSize == sizeof( uint16 ) ) )
            {
                // Use an int
                *(uint16 *)pvDest = xbpack_fnUnPackBits2Int( pucSrcBuf,
                                                            uiMsgBitOffset,
                                                            pktzFieldDesc->uiFieldCount );
            }
            else
            {
                // Use a byte
                *(uchar8 *)pvDest = xbpack_fnUnPackBits2Byte( pucSrcBuf,
                                                             uiMsgBitOffset,
                                                             pktzFieldDesc->uiFieldCount );
            }
        }
        break;

        case PGN_eTYPE_UCHAR:
        case PGN_eTYPE_SCHAR:
        {
            for( uiCount = 0; uiCount < pktzFieldDesc->uiFieldCount; uiCount++ )
            {
                // Casting depends on source element size which depends on the
                // structure record size and the number of records
                switch ( pktzFieldDesc->uiStructSize / pktzFieldDesc->uiFieldCount )
                {
                    case 1:
                    {
                        // Element is a byte or char
                       *((uchar8 *)pvDest + uiCount ) = *( pucSrcBuf + uiCount );
                    }
                    break;

                    case 2:
                    {
                        // Element is a word or short int
                        *((uint16 *)pvDest + uiCount ) = *( pucSrcBuf + uiCount );
                    }
                    break;

                    case 4:
                    {
                        // Element is a double word or long int
                        *((uint32 *)pvDest + uiCount ) = *( pucSrcBuf + uiCount );
                    }
                    break;

                    default:
                    {
                        // Save error
                        XBINIT_fnSaveError( PGN_eESRC_UNPACK,
                                            ( uint16 )PGN_eERR_FIELD_SIZE );
                    }
                    break;
                }
            }
        }
        break;

        case PGN_eTYPE_UINT:
        case PGN_eTYPE_SINT:
        {
            for( uiCount = 0; uiCount < pktzFieldDesc->uiFieldCount; uiCount++ )
            {
                if( pktzFieldDesc->uiStructSize == sizeof( uint32 ) )
                {
                    uint16 uiTemp;

                    uiTemp = xbpack_fnUnPackInt( pucSrcBuf +
                        ( uiCount * sizeof( uint16 ) ) );
                    *((uint32 *)pvDest + uiCount ) = ( uint32 )uiTemp;
                }
                else
                {
                    *((uint16 *)pvDest + uiCount ) = xbpack_fnUnPackInt( pucSrcBuf +
                        ( uiCount * sizeof( uint16 ) ) );
                }
            }
        }
        break;

        case PGN_eTYPE_ULONG:
        case PGN_eTYPE_SLONG:
        {
            for( uiCount = 0; uiCount < pktzFieldDesc->uiFieldCount; uiCount++ )
            {
                *((uint32 *)pvDest + uiCount ) = xbpack_fnUnPackLong( pucSrcBuf +
                    ( uiCount * sizeof( uint32 ) ) );
            }
        }
        break;

        case PGN_eTYPE_FLOAT:
        {
            *(float32 *)pvDest = xbpack_fnUnPackFloat( pucSrcBuf );
        }
        break;

        case PGN_eTYPE_DOUBLE:
        {
            *(double64 *)pvDest = xbpack_fnUnPackDouble( pucSrcBuf );
        }
        break;

        case PGN_eTYPE_STRING:
        case PGN_eTYPE_POINTER:
        case PGN_eTYPE_UNDEFINED:
        default:
        {
            // Unsupported type
            return( FALSE );
        }
    }

    // The unpacking was a big success
    return( TRUE );
}


/******************************************************************************

FUNCTION NAME:
    xbpack_fnUnPackStruct   - unpack all fields of a structure

PURPOSE:
    To unpack all of the fields of a structure

INPUTS:
    'pktzFieldDesc' points to the field descriptor array
    'uiFields' is the number of descriptors in the array
    'pucSrcBuf' points to the beginning of the message buffer
    'pvDest' points to the beginning of the destination structure

OUTPUTS:
    TRUE if the function is successful
    FALSE otherwise

NOTES:

VERSION HISTORY:

Version: 1.00   Date: Nov 25, 2002   By: Tom Lightfoot
  - Initial version

******************************************************************************/
static tucBOOL xbpack_fnUnPackStruct( const PGN_tzFIELD_DESC * XT_FAR pktzFieldDesc,
                                     uint16 uiFields,
                                     uchar8 *pucSrcBuf,
                                     void *pvDest )
{
    uint16 uiField;    // Counts through the fields

    // Verify parameter
    (void)assert( pktzFieldDesc != NULL );
    (void)assert( pucSrcBuf != NULL );
    (void)assert( pvDest != NULL );

    // Go throuth each field (if any)
    uiField = 0;
    while( uiField < uiFields )
    {
        // Use the general field unpacking function
        if( xbpack_fnUnPackField( pktzFieldDesc + uiField, pucSrcBuf,
                                   pvDest ) == FALSE )
        {
            // Unpack function was not successful
            return( FALSE );
        }

        // Go to the next field
        uiField++;
    }

    // Indicate success
    return( TRUE );
}

/******************************************************************************

FUNCTION NAME:
    XBPACK_fnUnPackPgn      - Convert a XanBus message into a structure

PURPOSE:
    Function for copying the fields of a received message into a
    structure to be used by the application.

INPUTS:
    'pktzPgnDesc'  points to the PGN descriptor
    'pucSrcBuf' points to the received message
    'uiSrcLen' is the length of the received message
    'pteErrCode' points to the returned error code

OUTPUTS:
    A pointer to the newly created unpacked structure.

NOTES:
    When done, the unpacked structure should be disposed of by using
    XBPACK_fnDestroyPgnStruct.

VERSION HISTORY:

Version: 1.00   Date: Nov 21, 2002   By: Tom Lightfoot
  - Initial version
Version: 1.01   Date: Nov 25, 2002   By: Tom Lightfoot
  - Now creates the destination structure and handles variable length PGNs
Version: 1.02   Date: Nov 28, 2002   By: Dale Mernett
  - Save error codes on failures
  - Free memory on failures
Version: 1.03   Date: May 12, 2003   By: Tom Lightfoot
  - Separated search for descriptor from packing
Version: 1.04   Date: Aug 22, 2003   By: Dalem
  - Do all message length checking first to avoid do extra work if it fails
Version 1.05  Date: 09/05/03  By: Dalem
    - Check descriptor for NULL
Version 1.06  Date: 01/23/04  By: Peterd
    - Check descriptor for NULL
Version 1.07  Date: November 21, 2017  By: DoruB
    - Allow RV-C packets of 8 bytes that exceed the DGN's length to conform
      with section 3.1 of the RV-C Revised Application Layer document dated
      December 17, 2015 which specifies that all transmitted packets are 8
      bytes even if the DGN is less than 8 bytes.
Version 1.08  Date: 2019-01-21  By: Tom Lightfoot
    - Added clause to not reject variable length PGNs padded out to 8 bytes
      long.

******************************************************************************/
void *XBPACK_fnUnPackPgn( const PGN_tzPGN_DESC * XT_FAR pktzPgnDesc,
                          uchar8 *pucSrcBuf,
                          uint16 uiSrcLen,
                          XB_teGRP_FUNC_VALUE *pteErrCode )
{
    void *pvDest;          // A pointer to the main destination structure
    void *pvExtras;        // A pointer to any extra fields in variable len msg
    void *pArray;         // A pointer to a specific member of the Extras array
    uint16 uiExtraBlocks; // Extra blocks to unpack from variable length buffer
    uint16 uiBlock;       // Counts through those extra blocks

    // Verify parameter
    (void)assert( pktzPgnDesc != NULL );
    (void)assert( pucSrcBuf != NULL );

    // Assume no errors yet
    *pteErrCode = XB_eGRP_FUNC_VALUE_DO_NOT_CARE;

    // If there is no descriptor
    if( pktzPgnDesc == NULL )
    {
        // Save error
        XBINIT_fnSaveError( PGN_eESRC_UNPACK,
                            ( uint16 )PGN_eERR_NO_PGDESC );

        *pteErrCode = XB_eGRP_FUNC_VALUE_DATA_ACCESS_ERROR;

        // No descriptor
        return( NULL );
    }

    // If message is shorter than expected
    if( uiSrcLen < pktzPgnDesc->uiFixedLen )
    {
        *pteErrCode = XB_eGRP_FUNC_VALUE_DLC_INVALID;

        // Too short
        return( NULL );
    }
    // If message descriptor has a variable length part
    else if( pktzPgnDesc->uiVarLen != 0 )
    {
        // Get the number of extra structures to retrieve
        uiExtraBlocks = ( uiSrcLen - pktzPgnDesc->uiFixedLen )
            / pktzPgnDesc->uiVarLen;

        // If the variable length portion does not have complete blocks
        // But be aware that stack may pad out message to fill one whole frame
        if (( uiExtraBlocks * pktzPgnDesc->uiVarLen +
              pktzPgnDesc->uiFixedLen != uiSrcLen )
            && ( uiSrcLen > RVC_CAN_FRAME_DLC ))
        {
            *pteErrCode = XB_eGRP_FUNC_VALUE_DLC_INVALID;

            // Not complete variable length blocks
            return( NULL );
        }
    }
    // If message not of equal to fixed length
    else if( uiSrcLen != pktzPgnDesc->uiFixedLen )
    {
        // According to section 3.1 of the RV-C Revised Application Layer
        // document dated December 17, 2015 all transmitted packets are 8
        // bytes even if the DGN is less than 8 bytes, therefore need to
        // accept 8 bytes packets discarding the extra bytes.
        if( ( uiSrcLen > pktzPgnDesc->uiFixedLen ) &&
            ( uiSrcLen == RVC_CAN_FRAME_DLC ) )
        {
            // Consider it as fixed length packet and discard the extra bytes.
            uiExtraBlocks = 0;
        }
        else
        {
            *pteErrCode = XB_eGRP_FUNC_VALUE_DLC_INVALID;

            // Too long
            return( NULL );
        }
    }
    else
    {
        // Fixed length
        uiExtraBlocks = 0;
    }

    // Allocate a block of memory for the main structure
    pvDest = XBMEM_fnMalloc( pktzPgnDesc->uiFixedSize );

    if( pvDest == NULL )
    {
        // Save error
        XBINIT_fnSaveError( PGN_eESRC_UNPACK,
                            ( uint16 )PGN_eERR_NO_MEM );

        *pteErrCode = XB_eGRP_FUNC_VALUE_MEMORY_ERROR;

        // Memory allocation failed
        return( NULL );
    }

    // Unpack all of the fields in the fixed size portion
    if( xbpack_fnUnPackStruct( pktzPgnDesc->pktzFixedList,
                                pktzPgnDesc->uiFixedFields,
                                pucSrcBuf,
                                pvDest ) == FALSE )
    {
        // Save error
        XBINIT_fnSaveError( PGN_eESRC_UNPACK,
                            ( uint16 )PGN_eERR_UNPACK );

        // Release main structure memory
        XBMEM_fnFree( pvDest );

        *pteErrCode = XB_eGRP_FUNC_VALUE_DATA_ACCESS_ERROR;

        // Function was unsuccessful
        return( NULL );
    }

    // Unpack the variable length portion if necessary
    if( uiExtraBlocks > 0 )
    {
        // Allocate an appropriate sized block for the array
        pvExtras = XBMEM_fnMalloc( uiExtraBlocks * pktzPgnDesc->uiVarSize );
        if( pvExtras == NULL )
        {
            // Save error
            XBINIT_fnSaveError( PGN_eESRC_UNPACK,
                                ( uint16 )PGN_eERR_NO_MEM );

            // Release main structure memory
            XBMEM_fnFree( pvDest );

            *pteErrCode = XB_eGRP_FUNC_VALUE_MEMORY_ERROR;

            // Function was unsuccessful
            return( NULL );
        }

        // Point to the variable length portion of the message buffer
        pucSrcBuf += pktzPgnDesc->uiFixedLen;

        // Point to the first element of the variable array
        pArray = pvExtras;

        // Copy each block
        for( uiBlock = 0; uiBlock < uiExtraBlocks; uiBlock++ )
        {
            // Call the general structure unpacking function
            if( xbpack_fnUnPackStruct( pktzPgnDesc->pktzVarList,
                                        pktzPgnDesc->uiVarFields,
                                        pucSrcBuf,
                                        pArray ) == FALSE )
            {
                // Save error
                XBINIT_fnSaveError( PGN_eESRC_UNPACK,
                                    ( uint16 )PGN_eERR_UNPACK_MORE );

                // Release main structure memory
                XBMEM_fnFree( pvDest );

                // Release variable structure memory
                XBMEM_fnFree( pvExtras );

                *pteErrCode = XB_eGRP_FUNC_VALUE_DATA_ACCESS_ERROR;

                // Function was unsuccessful
                return( NULL );
            }

            // Advance the input pointer
            pucSrcBuf += pktzPgnDesc->uiVarLen;

            // Advance the array pointer
            pArray = (uchar8 *)pArray + pktzPgnDesc->uiVarSize;
        }
    }
    else
    {
        // Null the array pointer
        pvExtras = NULL;
    }

    // Store the array count
    xbpack_fnSetExtraCount( pktzPgnDesc,
                            pvDest,
                            uiExtraBlocks );

    // Store the array pointer
    xbpack_fnSetExtraPoint( pktzPgnDesc,
                            pvDest,
                            pvExtras );

    // The unpack was a big success
    return( pvDest );
}



/******************************************************************************

FUNCTION NAME:
    XBPACK_fnDestroyPgnStruct - Release memory of an unpacked structure

PURPOSE:
    To destroy a structure created by XBPACK_fnUnPackPgn

INPUTS:
    'pktzPgnDesc' points to the PGN descriptor
    'pvStruct' points to the message structure to be destroyed

OUTPUTS:
    TRUE if the function is successful

NOTES:

VERSION HISTORY:

Version: 1.00   Date: Nov 25, 2002   By: Tom Lightfoot
  - Initial version
Version: 1.01   Date: May 12, 2003   By: Tom Lightfoot
  - Separated search for descriptor from destruction

******************************************************************************/

tucBOOL XBPACK_fnDestroyPgnStruct( const PGN_tzPGN_DESC * XT_FAR pktzPgnDesc,
                                   void *pvStruct )
{
    void *pvExtras;                     // A pointer to any extra fields in
                                       // variable length messages

    // Verify parameter
    (void)assert( pktzPgnDesc != NULL );
    (void)assert( pvStruct != NULL );

    // Does this message type have a variable length component?
    if( xbpack_fnGetExtraCount( pktzPgnDesc,
                                pvStruct ) > 0 )
    {
        // We need to free the memory block holding the variable
        // length array.  Find it.
        pvExtras = xbpack_fnGetExtraPoint( pktzPgnDesc,
                                          pvStruct );

        // Free the extra memory block
        XBMEM_fnFree( pvExtras );
    }

    // Free the main memory block
    XBMEM_fnFree( pvStruct );

    // Indicate success
    return( TRUE );
}

/******************************************************************************

FUNCTION NAME:
    XBPACK_fnRawGrpFunc     - get group function of a PGN if there is one

PURPOSE:
    This function is used to determine if there is a group function for
    a message before it is unpacked.

INPUTS:
    'ptzDob' points to the received Data OBject that would hold the group function

OUTPUTS:
    Group function number if PGN provides one
    XB_eGRP_FUNC_VALUE_DO_NOT_CARE if there is no Group Function for this message

NOTES:

VERSION HISTORY:

Version: 1.00   Date: May 12, 2003   By: Tom Lightfoot
  - New function header with version history
Version: 1.01   Date: Aug 22, 2003   By: Peter Drexel
  - Changed group function to enumeration from uchar
Version: 1.02   Date: Aug 22, 2003   By: Dalem
  - Do not use UnPackField
Version: 1.0.3  Date: 2019-03-28  By: Tom Lightfoot
  - Not bothering with looking up field info based on siGrpFuncFieldIndex.
    This is hard-coded to zero by xanbus_codegen.py anyways and is always
    a single byte.

******************************************************************************/

sint16 XBPACK_fnRawGrpFunc( PGNDOB *ptzDob )
{
    sint16 siGrpFunc = ( sint16 )XB_eGRP_FUNC_VALUE_DO_NOT_CARE;

    // Verify parameter
    (void)assert( ptzDob != NULL );

    // Does this PGN have a group function?
    // If so then this may not be the info for our specific message but it
    // will at least indicate whether or not a group function code is
    // applicable.  Once we have the group function from the first byte
    // of the message then we can look up the right info with the PGN and
    // group function code in hand
    if ( ptzDob->m_pPgnInfo->siGrpFunc != ( sint16 )XB_eGRP_FUNC_VALUE_DO_NOT_CARE )
    {
        // We care about the group function.  This is always the first byte
        // of the message
        siGrpFunc = ( sint16 )ptzDob->m_pData[ 0 ];
    }
    
    // Group function not found, return Group Don't Care
    return( siGrpFunc );
}

/******************************************************************************

FUNCTION NAME:
    XBPACK_fnUnPackMsgCount      - Get the message count from a config message

PURPOSE:
    Function for unpacking the message count from a configuration message.

INPUTS:
    'pucSrcBuf' points to the received message

OUTPUTS:
    The value of the message count field or
    0xFF if the message count could not be extracted

NOTES:
    None

VERSION HISTORY:

Version: 1.00   Date: Mar 30, 2004   By: Peter Drexel
  - Initial version

******************************************************************************/
uchar8 XBPACK_fnUnpackMsgCount( uchar8 *pucSrcBuf )
{
    XB_tzPGN_TO_GET_MSG_COUNT tzPgnToGetMsgCount;

    (void)assert( pucSrcBuf != NULL );

    // Unpack all of the fields in the fixed size portion
    if( xbpack_fnUnPackStruct( tzPgnToGetMsgCountDesc.pktzFixedList,
                                tzPgnToGetMsgCountDesc.uiFixedFields,
                                pucSrcBuf,
                                &tzPgnToGetMsgCount ) == FALSE )
    {
        // Save error
        XBINIT_fnSaveError( PGN_eESRC_UNPACK,
                            ( uint16 )PGN_eERR_UNPACK );

        // Function was unsuccessful
        return( 0xFF );
    }

    // Success
    return( tzPgnToGetMsgCount.ucMsgCount );
}
