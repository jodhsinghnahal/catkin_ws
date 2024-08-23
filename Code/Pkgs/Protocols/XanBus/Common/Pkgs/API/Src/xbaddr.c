/******************************************************************************

  (c) 2005 - 2023 Xantrex Canada ULC. All rights reserved.
  All trademarks are owned or licensed by Xantrex Canada ULC,
  its subsidiaries or affiliated companies.

****************************************************************************//**

  @addtogroup xbaddr XBADDR
  @{

    @brief Xanbus Address Management

****************************************************************************//**

  @page xbaddr_design XanBus Address Management Design

    Provides an API to manage the node names and addresses.  It has two basic
    purposes, first, claim and defend this node's network address, second, to
    keep a list of active nodes name/address pairs.

  @par API Functions
    XBADDR_fnCtor               - Initialize data for ISO Address Claiming
    XBADDR_fnBeginClaiming      - Initialize and start address claim sequence
    XBADDR_fnGetMyAddr          - Get current address
    XBADDR_fnGetNAME            - Get Name for node using given address
    XBADDR_fnGetNetNAME         - Get Network Name for node using given address
    XBADDR_fnNAME2ShortNAME     - Convert node expanded name to netwrok form
    XBADDR_fnNetNAME2NAME       - Convert net name to expanded name
    XBADDR_fnFindDevice         - Get devices on network
    XBADDR_fnTestPriority       - Get my Name Priority vs node of given address
    XBADDR_fnWantClaim          - Send an address claim message
    XBADDR_fnOnLine             - XanBus stack ready for application messages
    XBADDR_fnRecvClaim          - Handler for the ISO Address Claim message
    XBADDR_fnRecvCmdAddr        - Handle Commanded Address message
    XBADDR_fnRecvNAMECfg        - Handle Name Configuration message
    XBADDR_fnWantNAMECfg        - Send Name Config message
    XBADDR_fnResetNameCfg       - Handle a reset config command for Name Config
    XBADDR_fnReloadNAME         - Handle change in name

  @par Local Functions
    xbaddr_fnCompareNetNames    - Compare names in network form
    xbaddr_fnFindName           - Search for node by network name
    xbaddr_fnFindAddr           - Search for node by address
    xbaddr_fnClearNode          - Clear Node entry
    xbaddr_fnState2             - Perform NMEA state 2 processing
    xbaddr_fnState4             - Perform NMEA state 4 processing
    xbaddr_fnState6             - Perform NMEA state 6 processing
    xbaddr_fnState7             - Perform NMEA state 7 processing
    xbaddr_fnState9             - Perform NMEA state 9 processing
    xbaddr_fnRandTimerTimeout   - Handler for Random Start Timer event
    xbaddr_fnClaimTimerTimeout  - Handler for the Claim Timer event
    xbaddr_fnChallenge          - Determine outcome of Addr Name Challenge
    xbaddr_fnSendOneNameCfg     - Send Name Config for one config type
    xbaddr_fnPrcClaim           - Process ISO Address Claim/Node List

*******************************************************************************/


/*==============================================================================
                              Includes
==============================================================================*/

// C include library files
#include <string.h>         // Standard library string and memory interface

#include "LIB_stddefs.h"    // Xantrex Standard Library definitions

// XanBus include files
#include "xbldefs.h"        // Local Definitions
#include "xbmem.h"          // Xantrex Heap Memory interface
#include "xassert.h"        // Xantrex assert interface

// RV-C Protocol Definitions
#include "rvcpdefs.h"
#include "rvcudefs.h"

/*==============================================================================
                              Debugging
==============================================================================*/


/*==============================================================================
                              Defines
==============================================================================*/

// Defines for converting long names to short names
#define XBADDR_RESERVED_BIT_BYTE_LOCATION ( 6 )     // location of reserved byte
#define XBADDR_DEVICE_TYPE_BYTE_LOCATION  ( 5 )     // location of device type
#define XBADDR_MANUFAC_CODE_LOC           ( 2 )     // mfg. code location
#define XBADDR_SHIFT_HIGH_NYBBLE          ( 4 )     // high nybble location
#define XBADDR_SHIFT_BYTE                 ( 8 )     // shift one byte
#define XBADDR_SHIFT_3RD_BYTE             ( 16 )    // shift to third byte
#define XBADDR_SHIFT_2ND_BYTE             ( 8 )     // shift to second byte
#define XBADDR_NAME_SERIAL_NUM_MASK       ( 0x1F )  // mask for 3rd byte of
                                                    // serial number
#define XBADDR_BYTE_MASK                  ( 0xFF )  // all one's set

/*==============================================================================
                            Type Definitions
==============================================================================*/
// Definitions and storage for random number generator
#define XB_RAND_DELAY_MAX     ( 153L )

/*
 * These are the NMEA Claiming States as defined in NMEA 2000. Some modification
 * to a couple of the states to aligned with RVC address claiming process
 */
typedef enum eNMEA_STATE
{
    XBADDR_eNMEA_STATE0 = 0,
    XBADDR_eNMEA_STATE1,
    XBADDR_eNMEA_STATE2,
    XBADDR_eNMEA_STATE3,
    XBADDR_eNMEA_STATE4,
    XBADDR_eNMEA_STATE5,
    XBADDR_eNMEA_STATE6,
    XBADDR_eNMEA_STATE7,
    XBADDR_eNMEA_STATE8,
    XBADDR_eNMEA_STATE9,
    XBADDR_eNMEA_STATE9A,
    XBADDR_eNMEA_STATE10,
} XBADDR_teNMEA_STATE;

/*
 * Structure for maintaining node name/address pairs
 */
typedef struct zNODE
{
    struct zNODE *pNext;            ///< Pointer to the next node

    XB_tzNET_NAME tzNetName;        ///< NAME in condensed format
    uint16        uiTicks;          ///< Claim ticks
    uchar8        ucAddress;        ///< Node address
    uchar8        ucDSA;            ///< Default Source Address - used only for RV-C protocol
} XBADDR_tzNODE;

/*==============================================================================
                           Local/Private Variables
==============================================================================*/

// pointer to the first node
static XBADDR_tzNODE *ptzNodeHead;

// current number of nodes we have on the node list
static uchar8 ucTotalNodes;

// the maximum number of nodes allowed
static uchar8 ucMaxNodeNum;

// Definitions and storage for random number generator
static uint32 ulNextSeed = 1L;

// My node name in network form and expanded form
static XB_tzNET_NAME tzMyNetNAME;               // network form
static RVC_tzPGN_ISO_ADDR_CLAIM tzMyNAME;       // expanded form

static uchar8 ucMyAddr = PGN_ADDR_NULL;         // Address being used (claimed)
static uchar8 ucSaveAddr;                       // Previous claimed address
static XBADDR_teNMEA_STATE ucClaimingState;     // Address claiming state

static const uchar8 isoReqAddr[PGN_MAX_LEN_SINGLEFRAME]={0, 0xEE,0, 0xFF, 0xFF,0xFF,0xFF,0xFF};  // IsoReq addrclaim data value

// NAME of device undefined or not received yet
static const XB_tzNET_NAME tzNullNetName = {{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }};


/*==============================================================================
                              Forward declarations
==============================================================================*/
static void xbaddr_fnClearNode( XBADDR_tzNODE *ptzNode );
static sint16 xbaddr_fnCompareNetNames( XB_tzNET_NAME *ptzN1,
                                        XB_tzNET_NAME *ptzN2 );
static void xbaddr_fnDropNode( XBADDR_tzNODE *ptzNode );
static XBADDR_tzNODE *xbaddr_fnFindName( XB_tzNET_NAME *ptzNetName );
static XBADDR_tzNODE *xbaddr_fnFindAddr( uchar8 ucAddr );
static void xbaddr_fnState2( void );
static void xbaddr_fnState4( void );
static void xbaddr_fnState6( void );
static void xbaddr_fnState7( void );
static void xbaddr_fnState9( void );
static void xbaddr_fnState9A( void );
static void xbaddr_fnRandTimerTimeout( void );
static void xbaddr_fnClaimTimerTimeout( void );
static void xbaddr_fnChallenge( XB_tzNET_NAME *ptzOther );
static void xbaddr_fnSendOneNameCfg( XB_teCFG_TYPE teCfgType,
                                     uchar8 ucMsgCount,
                                     uchar8 ucDest );
static void xbaddr_fnPrcClaim( uchar8 ucSrcAddr,
                        XBADDR_tzNODE *ptzClaimNameNode,
                        XBADDR_tzNODE *ptzClaimAddrNode,
                        tucBOOL tucAlreadyInList );
static uint16 xbaddr_fnGetRandonDelay( void );
static tucBOOL xbaddr_fnRespondToCmdAddr( void );
static uchar8 xbaddr_fnListAdd( XBADDR_tzNODE **ppHead,
                                XBADDR_tzNODE *pNode );
static void xbaddr_fnListDelete( XBADDR_tzNODE **ppHead,
                                 XBADDR_tzNODE *pNode );
static tucBOOL xbaddr_fnCheckIfProperAddr( uchar8 ucAddr );
static uchar8 xbaddr_fnCheckAndSetProperAddr( uchar8 ucAddr );
static tucBOOL xbaddr_fnIsDynamicAddr( uchar8 ucAddr );
static uchar8 xbaddr_fnDynamicAddr( uchar8 ucAddr );
static uchar8 xbaddr_fnGetEndOfDynamicAddressRange( void );
static uchar8 xbaddr_fnGetStartOfDynamicAddressRange( void );
static void xbaddr_fnIsoReqAddrClaim( uchar8 ucAddr );
static void xbaddr_fnGetMyPreferredAddress( void );

/*==============================================================================
                           Function Definitions
==============================================================================*/

/*******************************************************************************

FUNCTION NAME:
    XBADDR_fnCtor          - Constructor for Address package

PURPOSE:
    This function is used to initialize ISO Address Claiming data structure and
    and the number of nodes we want to listen to.

INPUTS:
    'ulRandSeed' is value to use for random time value generator
    'ucMaxNodes' is the maximum number of nodes the device wants to listen to.

OUTPUTS:
    None

NOTES:
    Application can setup the maximum number of nodes it wants to listen to by
    calling this function.

VERSION HISTORY:

Version 1.00  Date: 07/20/04  By: Dalem
    - Created
Version 1.01  Date: 12/01/05  By: Hollyz
    - Added parameter ucMaxNodes

*******************************************************************************/

void XBADDR_fnCtor( uint32 ulRandSeed,
                    uchar8 ucMaxNodes )
{
    // Initialize the head of the node list
    ptzNodeHead = NULL;

    //  random seed
    ulNextSeed = ulRandSeed;

    // the current number of nodes we have on the node list
    ucTotalNodes = 0;

    ucMaxNodeNum = ucMaxNodes;
}

/******************************************************************************

FUNCTION NAME:
    xbaddr_fnCompareNetNames    - Compare names in network form

PURPOSE:
    This function is used to compare node names in network form.
    This form is more dense so it is quicker and smaller.
    The network form is 8 bytes in little endian order.

INPUTS:
    'ptzN1' is a pointer to first node
    'ptzN2' is a pointer to second node

OUTPUTS:
    XB_CMP_NAME_1_GT_2 if node 1 is numerically larger than node 2
    XB_CMP_NAME_1_LT_2 if node 1 is smaller than node 2
    XB_CMP_NAME_1_EQ_2 if node names are the same

NOTES:

VERSION HISTORY:

Version 1.01  Date: 06/24/03  By: Dalem
    - Major change to XB API

******************************************************************************/

static sint16 xbaddr_fnCompareNetNames( XB_tzNET_NAME *ptzN1,
                                        XB_tzNET_NAME *ptzN2 )
{
    uchar8 Idx;
    uchar8 *puc1;
    uchar8 *puc2;

    // Verify parameters
    (void)assert( ptzN1 != NULL );
    (void)assert( ptzN2 != NULL );

    // Get pointer to highest order byte
    puc1 = &ptzN1->aucNAMEArray[ XB_NAME_LEN - 1 ];
    puc2 = &ptzN2->aucNAMEArray[ XB_NAME_LEN - 1 ];

    // Look at 8 bytes of name
    for( Idx = 0; Idx < XB_NAME_LEN; Idx++, --puc1, --puc2 )
    {
        if( *puc1 == *puc2 )
        {
            continue;
        }
        else if( *puc1 > *puc2 )
        {
            return( XB_CMP_NAME_1_GT_2 );
        }
        else
        {
            return( XB_CMP_NAME_1_LT_2 );
        }
    }

    // Must be equal
    return( XB_CMP_NAME_1_EQ_2 );
}

/******************************************************************************

FUNCTION NAME:
    xbaddr_fnFindName           - Search for node by network name

PURPOSE:
    This function is used to search the list of maintained nodes by node name

INPUTS:
    'ptzNetName' is pointer to network name of node being hunted

OUTPUTS:
    pointer to node entry for given name
    NULL if node is not found

NOTES:

VERSION HISTORY:

Version 1.01  Date: 06/24/03  By: Dalem
    - Major change to XB API

Version 1.02  Date: 07/24/05  By: Hollyz
    - Find the pointer to the node from the linked list by gived node name

******************************************************************************/

static XBADDR_tzNODE *xbaddr_fnFindName( XB_tzNET_NAME *ptzNetName )
{
    XBADDR_tzNODE *ptzNode = ptzNodeHead;

    // Verify parameter
    (void)assert( ptzNetName != NULL );

    // Search through the node list
    while( ptzNode != NULL )
    {
        // Look for node with name matching given name
        if( xbaddr_fnCompareNetNames( &ptzNode->tzNetName,
                                      ptzNetName ) == XB_CMP_NAME_1_EQ_2 )
        {
            // Return pointer to matching node
            return( ptzNode );
        }
        else
        {
            ptzNode = ptzNode->pNext;
        }

    }

    // No matching node
    return( NULL );
}

/******************************************************************************

FUNCTION NAME:
    xbaddr_fnFindAddr           - Search for node by address

PURPOSE:
    This function is used to search the list of nodes by node address

 PURPOSE:

INPUTS:
    'ucAddr' is node address being searched for

OUTPUTS:
    pointer to node entry for given name
    NULL if node is not found

NOTES:

VERSION HISTORY:

Version 1.01  Date: 06/24/03  By: Dalem
    - Major change to XB API

Version 1.02  Date: 07/24/05  By: Hollyz
    - Find the pointer to the node from the linked list by gived node address

******************************************************************************/

static XBADDR_tzNODE *xbaddr_fnFindAddr( uchar8 ucAddr )
{
    XBADDR_tzNODE *ptzNode = ptzNodeHead;

    // Search through the node list
    while( ptzNode != NULL )
    {
        // Look for node with address matching given address
        if( ptzNode->ucAddress == ucAddr )
        {
            // Return pointer to matching node
            return( ptzNode );
        }
        else
        {
            ptzNode = ptzNode->pNext;
        }

    }

    // No matching node
    return( NULL );
}

/******************************************************************************

FUNCTION NAME:
    xbaddr_fnClearNode          - Clear Node entry

PURPOSE:
    This function is used to clear the node data at given node pointer

INPUTS:
    'ptzNode' is pointer to node entry to be cleared

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.01  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.02  Date: 07/24/05  By: Hollyz
    - Removed the node from the linked list and release the momory used by the
      node from the heap
Version 1.03  Date: 12/2/05  By: Hollyz
    - Decrease the total number of nodes when a node is removed

******************************************************************************/

static void xbaddr_fnClearNode( XBADDR_tzNODE *ptzNode )
{
     assert( ptzNode );

     // removed the node from the linked list
     xbaddr_fnListDelete( &ptzNodeHead,
                          ptzNode );

     // free the memory used by the node
     XBMEM_fnFree( ptzNode );

     // decrease the number of nodes on the list
     ucTotalNodes--;
}

/******************************************************************************

FUNCTION NAME:
    XBADDR_fnNetNAME2NAME       - Convert net name to expanded name

PURPOSE:
    This function is used to convert a node name in network form (eight bytes)
    to expaned form as used by application

INPUTS:
    'ptzNetName' is pointer to node name in network form
    'ptzName' is pointer to store name in expanded form

OUTPUTS:
    TRUE if success
    FALSE if failure (e.g. no memory)

NOTES:

VERSION HISTORY:

Version 1.02  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.03  Date: 12/19/03  By: Dalem
    - Changed a test for not NULL to test for not FALSE

******************************************************************************/

tucBOOL XBADDR_fnNetNAME2NAME( XB_tzNET_NAME *ptzNetName,
                               XB_tzPGN_ISO_ADDR_CLAIM *ptzName )
{
    const PGN_tzPGN_INFO * ptzPgnInfo;
    XB_tzPGN_ISO_ADDR_CLAIM *ptzUnpackName;
    XB_teGRP_FUNC_VALUE teErrCode = XB_eGRP_FUNC_VALUE_DO_NOT_CARE;

    // Verify parameter
    (void)assert( ptzNetName != NULL );
    (void)assert( ptzName != NULL );
    // Convert name to name as application sees it
    ptzPgnInfo = XBPACK_fnGetPgnInfo( ( uint32 )XB_PGN_ISO_ADDR_CLAIM,
                                     ( sint16 )XB_eGRP_FUNC_VALUE_DO_NOT_CARE );
    if( ptzPgnInfo == NULL )
    {
        // Could not find the descriptor so could not unpack
        return( FALSE );
    }

    // Unpack the structure to some memory on the heap
    ptzUnpackName = XBPACK_fnUnPackPgn( ptzPgnInfo->pktzDesc,
                                        ptzNetName->aucNAMEArray,
                                        sizeof(ptzNetName->aucNAMEArray),
                                        &teErrCode );

    // If name was unpacked successfully
    if( ptzUnpackName != NULL )
    {
        // Copy name to return structure
        //*ptzName = *ptzUnpackName;
        memcpy( ptzName, ptzUnpackName, sizeof( XB_tzPGN_ISO_ADDR_CLAIM ) );

        // Free the memory used for unpacking
        if( XBPACK_fnDestroyPgnStruct( ptzPgnInfo->pktzDesc,
                                       ptzUnpackName ) != FALSE )
        {
            // Success
            return( TRUE );
        }
    }

    // Failure
    return( FALSE );
}

/*******************************************************************************

FUNCTION NAME:
    XBADDR_fnNAME2ShortNAME - Convert NAME to short form

PURPOSE:
    This function converts the name in structure form to a NAME in commpressed
    short form.

INPUTS:
    ptzLongNAME - source NAME in structure form
    pucShortNAME - place to store the NAME in short form

OUTPUTS:
    none

NOTES:
    NAME is built as follows:

    MSB                                                              LSB
    Byte 7:
    1 bit Self Configurable Address : 3 Bits Industry grp : 4 bits Sys Instance

    Byte 6:
    7 Bits Device Class : 1 Bit Reserved

    Byte 5:
    8 Bits Device Function:

    Byte 4:
    5 Bits Device Instance Upper : 3 Bits Device Instance Lower

    Byte 3:
    8 Bits Mfg Code:

    Byte 2:
    3 Bits Mfg Code : 5 Bits Unique Number

    Byte 1-0 :
    16 Bits Unique Number


VERSION HISTORY:

Version 1.00  Date: 06/17/04  By: Dalem
    - Created
Version 1.01  Date: 06/09/05  By: Hollyz
    - Fixed bug related to PRN3668
Version 1.02  Date: 04/13/06  By: Hollyz
    - Mask off the MSB for byte # 2.

*******************************************************************************/

void XBADDR_fnNAME2ShortNAME( XB_tzPGN_ISO_ADDR_CLAIM *ptzLongNAME,
                              uchar8 *pucShortNAME )
{
    // unique number byte 0 in name byte 0
    pucShortNAME[ 0 ]
        = (uchar8)( ptzLongNAME->ulUniqueNumber & XBADDR_BYTE_MASK );

    // unique number byte 1 into name byte 1
    pucShortNAME[ 1 ]
         = (uchar8)( ( ptzLongNAME->ulUniqueNumber >> XBADDR_SHIFT_2ND_BYTE )
                     & XBADDR_BYTE_MASK );

    // unique number byte 2 and mfg code in byte 2
    pucShortNAME[ 2 ]
        = (uchar8)( ( ( ( ptzLongNAME->ulUniqueNumber >> XBADDR_SHIFT_3RD_BYTE )
                        & XBADDR_NAME_SERIAL_NUM_MASK )
                      | ( ( ( uchar8 )( ptzLongNAME->uiManufacturerCode << XT_BITPOS5 ) )
                    & ~( XBADDR_NAME_SERIAL_NUM_MASK ) ) ) & XBADDR_BYTE_MASK ) ;

    // rest of mfg code in byte 3
    pucShortNAME[ 3 ]
        = ( ( uchar8 )( ptzLongNAME->uiManufacturerCode >> XT_BITPOS3 ) );

    // device instance upper and device instance lower in byte 4
    pucShortNAME[ 4 ] = ( ptzLongNAME->ucDeviceInstanceUpper << XT_BITPOS3 )
                      | ( ptzLongNAME->ucDeviceInstanceLower );

    // device function into byte 5
    pucShortNAME[ 5 ] = ( uchar8 )( ptzLongNAME->teDeviceFunction );

    // reserved bit and device class into byte 6
    pucShortNAME[ 6 ] = ( uchar8 )( ptzLongNAME->ucRsvBit ) |
                      ( ( uchar8 )( ptzLongNAME->teDeviceClass ) )
                             << XT_BITPOS1;

    // system instance, industry group( power ) and self cfg address in byte 7
    pucShortNAME[ 7 ] = ( uchar8 )( ptzLongNAME->ucSystemInstance )
            | ( ( uchar8 )( ptzLongNAME->teIndustryGroup ) ) << XT_BITPOS4
            | ( ( uchar8 )( ptzLongNAME->ucSelfCfgAddr ) ) << XT_BITPOS7;
}

/******************************************************************************

FUNCTION NAME:
    XBADDR_fnGetNAME            - Get Name for node using given address

PURPOSE:
    This function is used to extract the whole name structure for a given
    device address.

INPUTS:
    'ucAddr' is address to look up node name to get device name
    'ptzName' points to where to store the name structure

OUTPUTS:
    TRUE if the device address was found in the list
    FALSE if the device address is not online

NOTES:

VERSION HISTORY:

Version 1.01  Date: 06/24/03  By: Dalem
    - Major change to XB API

******************************************************************************/
tucBOOL XBADDR_fnGetNAME( uchar8 ucAddr,
                          XB_tzPGN_ISO_ADDR_CLAIM *ptzName )
{
    XBADDR_tzNODE *ptzNode;

    // Verify parameter
    (void)assert( ptzName != NULL );

    // Find node for given address
    ptzNode = xbaddr_fnFindAddr( ucAddr );

    // If node found
    if( ptzNode != NULL )
    {
        // Convert net name to name
        if( XBADDR_fnNetNAME2NAME( &ptzNode->tzNetName,
                                   ptzName ) != FALSE )
        {
            // Indicate success
            return( TRUE );
        }
    }

    // Failure
    return( FALSE );
}

/******************************************************************************

FUNCTION NAME:
    XBADDR_fnGetNetNAME

PURPOSE:
    This function is used to extract the Network Name for node using given
    device address.

INPUTS:
    'ucAddr' is address to look up node name to get device name
    'pucShortNAME' points to a pointer to where to store the name's address

OUTPUTS:
    TRUE if the device address was found in the list
    FALSE if the device address is not online

NOTES:

VERSION HISTORY:

Version 1.01  Date: 03/17/17  By: Doru Botez
    - Created.

******************************************************************************/
tucBOOL XBADDR_fnGetNetNAME( uchar8 ucAddr,
                             uchar8 **pucShortNAME )
{
    XBADDR_tzNODE *ptzNode;
    tucBOOL ucNameFound = FALSE;

    // Verify parameter
    (void)assert( pucShortNAME != NULL );

    if( ucAddr == ucMyAddr )
    {
        // The requested address is for this device
        (*pucShortNAME) = &tzMyNetNAME.aucNAMEArray[ 0 ];
        ucNameFound = TRUE;
    }
    else
    {
        // Find node for given address
        ptzNode = xbaddr_fnFindAddr( ucAddr );

        // If node found
        if( ptzNode != NULL )
        {
            (*pucShortNAME) = &ptzNode->tzNetName.aucNAMEArray[ 0 ];
            ucNameFound = TRUE;
        }
    }

    // Return
    return ucNameFound;
}


/***************************************************************************//**
  Extract the Default Source Address (DSA) for node using given device address.

  @param ucAddr - node address to get the DSA for
  @param pucDSA - pointer to the location to store the retrieved DSA

  @return \b TRUE if the specified node exists
          \b FALSE otherwise.
*******************************************************************************/
tucBOOL XBADDR_fnGetNodeDSA( uchar8 ucAddr, uchar8 *pucDSA )
{
    tucBOOL ucNodeFound = FALSE;
    XBADDR_tzNODE *ptzNode;

    // Verify parameter
    (void)assert( pucDSA != NULL );

    if( ucAddr == ucMyAddr )
    {
        // The requested address is for this device - load it from the platform module
        if( XBPLATFORM_fnLoad( XB_ePARAM_TYPE_DSA, XB_eCFG_TYPE_USER, pucDSA, 1 ) == eSTATUS_OK )
        {
            ucNodeFound = TRUE;
        }
    }
    else
    {
        // Find node for given address
        ptzNode = xbaddr_fnFindAddr( ucAddr );

        // If node found
        if( ptzNode != NULL )
        {
            (*pucDSA) = ptzNode->ucDSA;
            ucNodeFound = TRUE;
        }
    }

    // Return
    return ucNodeFound;
}


/******************************************************************************

FUNCTION NAME:
    XBADDR_fnFindDevice    - Get the node name by given index to node list

PURPOSE:
    This function is used to get the node name by given index to the
    linked Node List

INPUTS:
    'ucIndex' is the index to the node list
    'pucAddr' points to node address
    'ptzName' points to where to save the node name

OUTPUTS:
    TRUE if a valid node was found
    FALSE otherwise

NOTES:

VERSION HISTORY:

Version 1.00  Date: 12/12/05  By: Hollyz
    - Created

Version 1.01  Date: 012/12/06  By: Hollyz
    - Added getting Node address

******************************************************************************/
tucBOOL XBADDR_fnFindDevice( uchar8 ucIndex,
                             uchar8 *pucAddr,
                             XB_tzPGN_ISO_ADDR_CLAIM *ptzNAME )
{
    uchar8 ucNodeIdx;
    XBADDR_tzNODE *ptzNode;

    assert( ptzNAME != NULL );

    // from the beginning of the node list
    ptzNode = ptzNodeHead;

    for( ucNodeIdx = 0; ucNodeIdx < ucIndex; ucNodeIdx++ )
    {
        // handle is invalid
        if( ptzNode == NULL )
        {
            break;
        }

        // move to the next one
        ptzNode = ptzNode->pNext;

    }

    // If node was found
    if( ptzNode != NULL )
    {
        // Convert net name to name
        if( XBADDR_fnNetNAME2NAME( &ptzNode->tzNetName,
                                   ptzNAME ) != FALSE )
        {
             // node address
             *pucAddr = ptzNode->ucAddress;

             // Indicate that a device was found
             return( TRUE );
        }
    }

    // Failure
    return( FALSE );
}

/******************************************************************************

FUNCTION NAME:
    XBADDR_fnLoadClaimTimers

PURPOSE:
    Reload the claim timer for all nodes

INPUTS:
    None

OUTPUTS:
    None

NOTES:

VERSION HISTORY:

Version 1.00  Date: 07/24/05  By: Hollyz
    - Created

******************************************************************************/

void XBADDR_fnLoadClaimTimers( void )
{
    uint16 uiClaimTimer;
    XBADDR_tzNODE *ptzNode;

    // Get the claim tick count in timer
    uiClaimTimer = XBTIMERS_fnGetClaimTimer();

    // pointer to the first node on the list
    ptzNode = ptzNodeHead;

    // For all active claim timers
    while( ptzNode != NULL )
    {
        // Restart the tick down counter
        ptzNode->uiTicks = uiClaimTimer;

        // move to the next node
        ptzNode = ptzNode->pNext;
    }
}

/******************************************************************************

FUNCTION NAME:
    XBADDR_fnTestPriority - Get my Name Priority versus node of given address

PURPOSE:
    This function is used to get the name priority of this node versus
    the node that is using the given address.  The name that is numerically
    lower when taken as a little endian 8 byte number is the higher priority.

INPUTS:
    'ucAddr' is address of foreign node

OUTPUTS:
    XB_TEST_PRI_MYNAME_HI if my name has higher priority
    XB_TEST_PRI_MYNAME_LO if my name has lower priority
    XB_TEST_PRI_MYNAME_EQ if the node names are the same
    XB_TEST_PRI_FAIL if foreign node not found

NOTES:
   The compare net names function returns which is the larger nodes
   so the names in the function call is reversed to get priority right.

VERSION HISTORY:

Version 1.01  Date: 06/24/03  By: Dalem
    - Major change to XB API

******************************************************************************/

sint16 XBADDR_fnTestPriority( uchar8 ucAddr )
{
    XBADDR_tzNODE *ptzNode;

    // Find node for given address
    ptzNode = xbaddr_fnFindAddr( ucAddr );

    // If node found
    if( ptzNode != NULL )
    {
        // Return result of compare, other name versus my name
        switch ( xbaddr_fnCompareNetNames( &ptzNode->tzNetName,
                                           &tzMyNetNAME ) )
        {
            case XB_CMP_NAME_1_GT_2:
            {
                return( XB_TEST_PRI_MYNAME_HI );
            }

            case XB_CMP_NAME_1_LT_2:
            {
                return( XB_TEST_PRI_MYNAME_LO );
            }

            case XB_CMP_NAME_1_EQ_2:
            {
                return( XB_TEST_PRI_MYNAME_EQ );
            }

            default:
            {
                return( XB_TEST_PRI_FAIL );
            }
        }
    }

    // Name lookup failed
    return( XB_TEST_PRI_FAIL );
}

/******************************************************************************

FUNCTION NAME:
    XBADDR_fnTickClaimTimers    - Count down and look for expired claim timers

PURPOSE:
    When called, this function counts down claim timers and checks for expired
    ones.  For expired Claim timers, the node will be removed from active node
    list.

INPUTS:
    None

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.00  Date: 07/24/05  By: Hollyz
    - Created

******************************************************************************/

void XBADDR_fnTickClaimTimers( void )
{
    XBADDR_tzNODE *ptzNode;
    XBADDR_tzNODE *ptzNext;

    // get the first node from the node list
    ptzNode = ptzNodeHead;

    // Go through list of active nodes and count down tick timers
    while( ptzNode != NULL )
    {
        ptzNext = ptzNode->pNext;

        // Count down timer and check for expired
        if( ( ptzNode->uiTicks == 0 ) || ( --ptzNode->uiTicks == 0 ) )
        {
            // Remove node record
            xbaddr_fnDropNode( ptzNode );
        }

        // move to the next one
        ptzNode = ptzNext;
    }
}

/******************************************************************************

FUNCTION NAME:
    XBADDR_fnDropNode           - Remove node from node data list

PURPOSE:
    This function is used to drop a node from the node data list

INPUTS:
    'ucAddr' is address of node to be dropped from list

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.02  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.03  Date: 05/14/04  By: Peterd
    - Updated call to XBPLATFORM_fnNodeChange based on new behaviour

******************************************************************************/

static void xbaddr_fnDropNode( XBADDR_tzNODE *ptzNode )
{
    XB_tzPGN_ISO_ADDR_CLAIM tzLongName;       // expanded form of name

    // If node exists
    if( ptzNode != NULL )
    {
        // Convert the name previously using the address
        if( XBADDR_fnNetNAME2NAME( &ptzNode->tzNetName,
                                   &tzLongName ) != FALSE )
        {
            // Tell the application that there is a node change on the bus
            XBPLATFORM_fnNodeChange( XB_eNODE_CHANGE_TIMED_OUT,
                                     ptzNode->ucAddress,
                                     PGN_ADDR_NULL,
                                     &tzLongName );
        }

        if( ucClaimingState == XBADDR_eNMEA_STATE10 )
        {
            // Get My Preferred Address
            xbaddr_fnGetMyPreferredAddress();

            // Is this the static node that was using my address?
            if( ucMyAddr == ptzNode->ucAddress )
            {
                // Now that the other node is gone can try to claim my preferred address
                // Start random timer
                XBINIT_fnStartProTimer( ( xbaddr_fnGetRandonDelay() / XB_CENTI_TO_MILLI ),
                                        xbaddr_fnRandTimerTimeout );

                // Set address claiming state to state 1
                ucClaimingState = XBADDR_eNMEA_STATE1;
            }
        }

        // Clear the record
        xbaddr_fnClearNode( ptzNode );
    }
}

/******************************************************************************

FUNCTION NAME:
    XBADDR_fnWantClaim          - Send an address claim message

PURPOSE:
    This function is used to send an address claim message.  This can be
    used while claiming addresses or as a keep alive message.

INPUTS:
    'ucDest' is destination address for the message
    'tucSolicited' is TRUE if request is from network

OUTPUTS:
    nothing

NOTES:
    tucSolicited is included so function complies with expectations
    in PGN table data structure

VERSION HISTORY:

Version 1.04  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.05  Date: 09/12/03  By: Dalem
    - Added switch on claiming state transitions

******************************************************************************/

void XBADDR_fnWantClaim( PGN_tzWANT_DATA *ptzWant )
{
    const PGN_tzPGN_INFO * ptzPgnInfo;
    PGNDOB *ptzDOB;
    TFXCAN_RETURNS TFX_teResult;

    // Avoid compiler warning
    UNUSED( ptzWant->tucSolicited );

    switch( ucClaimingState )
    {
        case XBADDR_eNMEA_STATE2:
#if( DEVICE_CLAIM_PGN == RVC_PGN_DIAG_MSG1 )
            // Cannot send when in this state
            return;
#else
            // Sending is allowed, break out and send
            break;
#endif
        case XBADDR_eNMEA_STATE7:
        case XBADDR_eNMEA_STATE9:
        case XBADDR_eNMEA_STATE9A:
            // Sending is allowed, break out and send
            break;

        case XBADDR_eNMEA_STATE5:
            // Go to state 9
            xbaddr_fnState9();

            // Do not send yet
            return;

        case XBADDR_eNMEA_STATE8:
            // Go back to state 6
            xbaddr_fnState6();

            // Do not send yet
            return;

        case XBADDR_eNMEA_STATE0:
        case XBADDR_eNMEA_STATE1:
        case XBADDR_eNMEA_STATE3:
        case XBADDR_eNMEA_STATE4:
        case XBADDR_eNMEA_STATE6:
        case XBADDR_eNMEA_STATE10:
            // Cannot send when in this state
            return;

        default:
            // Error, don't send
            return;
    }

    // Look up the PGN for address claim
    ptzPgnInfo = XBPACK_fnGetPgnInfo( XB_PGN_ISO_ADDR_CLAIM,
                                     ( sint16 )XB_eGRP_FUNC_VALUE_DO_NOT_CARE );

    // Allocate memory for a Data OBject
    TFX_teResult = PDOBConstructAlloc( &ptzDOB,
                                       XB_PGN_ISO_ADDR_CLAIM,
                                       ( sint16 )XB_eGRP_FUNC_VALUE_DO_NOT_CARE,
                                       ptzPgnInfo );

    // If call succeeded
    if( TFX_teResult == TFXCR_OK )
    {
        // Set size of buffer
        ptzDOB->m_i16ByteCount = sizeof( tzMyNetNAME.aucNAMEArray );

        // Give data buffer to DOB
        ptzDOB->m_pData = tzMyNetNAME.aucNAMEArray;

        // Remember buffer is NOT from heap
        ptzDOB->m_ucFlags &= ~DOB_DATA_FROM_HEAP;

        // Set port and destination in DOB
        PDOBSetPort( ptzDOB, XB_PORT_A );
        ptzDOB->m_DestAddr = ptzWant->ucDestAddr;

        // If address is claimed or requesting a claim
        if( ( ucClaimingState == XBADDR_eNMEA_STATE9 )
         || ( ucClaimingState == XBADDR_eNMEA_STATE9A )
         || ( ucClaimingState == XBADDR_eNMEA_STATE2 ) )
        {
            // Use my address
            ptzDOB->m_SourceAddr = ucMyAddr;
        }
        else
        {
            // Use null address
            ptzDOB->m_SourceAddr = PGN_ADDR_NULL;
        }

        // Queue DOB for sending
        TFX_teResult = DOBHLDAdd( ptzDOB );

        // Check result
        if( TFX_teResult == TFXCR_OK )
        {
            // Set state as ready to go out, success
            ptzDOB->m_eState = XDATA;
        }
        else
        {
            // Cannot queue dob, release DOB -- this will release the buffer
            PDOBDestruct( ptzDOB );

            // Save error
            XBINIT_fnSaveError( PGN_eESRC_TFX_QUEUE_DOB,
                                ( uint16 )TFX_teResult );
        }
    }
}

/******************************************************************************

FUNCTION NAME:
    XBADDR_fnGetMyAddr          - Get current address

PURPOSE:
    This function is used to get the current address of this node.  This
    address is the address that has been claimed or that is being claimed.

INPUTS:
    None

OUTPUTS:
    Current address of this node

NOTES:

VERSION HISTORY:

Version 1.01  Date: 06/24/03  By: Dalem
    - Major change to XB API

******************************************************************************/

uchar8 XBADDR_fnGetMyAddr( void )
{
    // Return currently used address
    return( ucMyAddr );
}

/******************************************************************************

FUNCTION NAME:
    xbaddr_fnCheckIfProperAddr

PURPOSE:
    This function is used to check if the specified address is proper for
    this node.

INPUTS:
    'ucAddr' - address to check

OUTPUTS:
    TRUE: - address is proper
    FALSE - address is not proper

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/08/17  By: Doru Botez
    - First revision of the function.

******************************************************************************/
static tucBOOL xbaddr_fnCheckIfProperAddr( uchar8 ucAddr )
{
    tucBOOL ucProper = TRUE;

    // Check if preferred address not proper
    switch( ucAddr )
    {
    case ( uchar8 )RVC_eDFLT_SRC_ADDR_GENSTART_CONTROLLER:
    case ( uchar8 )RVC_eDFLT_SRC_ADDR_INVERTER:
    case ( uchar8 )RVC_eDFLT_SRC_ADDR_INVERTER2:
    case ( uchar8 )RVC_eDFLT_SRC_ADDR_CONTROL_PANEL:
    case ( uchar8 )RVC_eDFLT_SRC_ADDR_BATTERY_MONITOR:
    case ( uchar8 )RVC_eDFLT_SRC_ADDR_BATTERY_MONITOR2:
    case ( uchar8 )RVC_eDFLT_SRC_ADDR_CHASSIS_BATTERY_MONITOR:
    case ( uchar8 )RVC_eDFLT_SRC_ADDR_CONVERTER:
    case ( uchar8 )RVC_eDFLT_SRC_ADDR_CONVERTER2:
    case ( uchar8 )RVC_eDFLT_SRC_ADDR_CHARGE_CONTROLLER:
    case ( uchar8 )RVC_eDFLT_SRC_ADDR_AC_LOAD_MONITOR:
    case ( uchar8 )RVC_eDFLT_SRC_ADDR_AC_FAULT_PROTECTION:
    case ( uchar8 )RVC_eDFLT_SRC_ADDR_TRANSFER_SWITCH:
    case ( uchar8 )RVC_eDFLT_SRC_ADDR_DC_LOAD:
    case ( uchar8 )RVC_eDFLT_SRC_ADDR_AC_LOAD:
    case ( uchar8 )RVC_eDFLT_SRC_ADDR_GENERIC_AC_SOURCE:
    case ( uchar8 )RVC_eDFLT_SRC_ADDR_SOLAR_CHARGE_CONTROLLER:
    case ( uchar8 )RVC_eDFLT_SRC_ADDR_SERVICE_TOOL:
    case ( uchar8 )RVC_eDFLT_SRC_ADDR_SYSTEM_CLOCK:
    case ( uchar8 )RVC_eDFLT_SRC_ADDR_DATA_LOGGER:
    case ( uchar8 )RVC_eDFLT_SRC_ADDR_CHASSIS_BRIDGE:
    case ( uchar8 )RVC_eDFLT_SRC_ADDR_NETWORK_BRIDGE:
        break;

    default:
        if( xbaddr_fnIsDynamicAddr( ucAddr ) == FALSE )
        {
            ucProper = FALSE;
        }
        break;
    }

    return ucProper;
}

/******************************************************************************

FUNCTION NAME:
    xbaddr_fnCheckAndSetProperAddr

PURPOSE:
    This function is used to check if the specified address is proper for
    this node and if not it returns a proper address.

INPUTS:
    'ucAddr' - address to check

OUTPUTS:
    Proper address for this node

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/06/17  By: Doru Botez
    - First revision of the function.

******************************************************************************/
static uchar8 xbaddr_fnCheckAndSetProperAddr( uchar8 ucAddr )
{
    // Check if address is not proper
    if( xbaddr_fnCheckIfProperAddr( ucAddr ) == FALSE )
    {
        // Invalid address - set a valid one starting at address end range
        ucAddr = RVC_PWR_COMP_DYN_ADDR_END;
    }

    return ucAddr;
}

/***************************************************************************//**

  Determines if the CAN address is within the allowed dynamic address range.

 *******************************************************************************/
static tucBOOL xbaddr_fnIsDynamicAddr(uchar8 ucAddr)
{
    tucBOOL tucRetVal = FALSE;

    // Using a set of if / else-if statemets to avoid compiler optimisation errors
    if( ( ucAddr >= RVC_PWR_COMP_DYN_ADDR_START ) && ( ucAddr <= RVC_PWR_COMP_DYN_ADDR_END ) )
    {
        tucRetVal = TRUE;
    }
    else if( ( ucAddr >= RVC_CTRL_DISP_DYNAMIC_ADDR_START ) && ( ucAddr <= RVC_CTRL_DISP_DYNAMIC_ADDR_END ) )
    {
        tucRetVal = TRUE;
    }
    else if( ( ucAddr >= RVC_SENSORS_DYNAMIC_ADDR_START ) && ( ucAddr <= RVC_SENSORS_DYNAMIC_ADDR_END ) )
    {
        tucRetVal = TRUE;
    }
    else if(
        #if( RVC_COMP_DYN_ADDR_EXT_START != 0 )
             ( ucAddr >= RVC_COMP_DYN_ADDR_EXT_START ) &&
        #endif // RVC_COMP_DYN_ADDR_EXT_START
             ( ucAddr <= RVC_COMP_DYN_ADDR_EXT_END ) )
    {
        tucRetVal = TRUE;
    }
    return tucRetVal;
}

/***************************************************************************//**

  Get the highest address from the address range specific to the device function

*******************************************************************************/
static uchar8 xbaddr_fnGetEndOfDynamicAddressRange( void )
{
    uchar8 ucAddr;
    // If getting my node name from the application fails
    // or the reserved bit in the name is not zero
    if( ( XBPLATFORM_fnLoad( XB_ePARAM_TYPE_NAME,
                             XB_eCFG_TYPE_USER,
                             tzMyNetNAME.aucNAMEArray,
                             XB_NAME_LEN ) != ( tuiSTATUS )eSTATUS_OK )
     || ( ( tzMyNetNAME.aucNAMEArray[ 6 ] & 0x01 ) != 0 ) )
    {
        // Failure - do not know device function
        ucAddr = RVC_COMP_DYN_ADDR_EXT_END;
    }
    // Convert application name to network name
    else if( XBADDR_fnNetNAME2NAME( &tzMyNetNAME,
                                    (XB_tzPGN_ISO_ADDR_CLAIM *)&tzMyNAME ) == FALSE )
    {
        // Failure - do not know device function
        ucAddr = RVC_COMP_DYN_ADDR_EXT_END;
    }
    else
    {
        // If the network name is available return the end of the address range
        // specific for device function
        switch( tzMyNAME.teDeviceFunction )
        {
        case RVC_eFUNC_CODE_BATTERY_MONITOR:
            ucAddr = RVC_SENSORS_DYNAMIC_ADDR_END;
            break;
        case RVC_eFUNC_CODE_ENERGY_MANAGEMENT_SYSTEM:
        case RVC_eFUNC_CODE_BATTERY_MANAGEMENT_SYSTEM:
        case RVC_eFUNC_CODE_INVERTER_CHARGER:
        case RVC_eFUNC_CODE_INVERTER:
        case RVC_eFUNC_CODE_CHARGER:
            ucAddr = RVC_PWR_COMP_DYN_ADDR_END;
            break;
        case RVC_eFUNC_CODE_USER_INTERFACE:
        case RVC_eFUNC_CODE_SERVICE_TOOL:
        case RVC_eFUNC_CODE_DIAGNOSTIC_TERMINAL:
        case RVC_eFUNC_CODE_GATEWAY:
        case RVC_eFUNC_CODE_SYSTEM_CLOCK:
            ucAddr = RVC_CTRL_DISP_DYNAMIC_ADDR_END;
            break;
        default:
            ucAddr = RVC_COMP_DYN_ADDR_EXT_END;
            break;
        }
    }

    return ucAddr;
}

/***************************************************************************//**

  Get the lowest address from the dynamic range specific to the device function

*******************************************************************************/
static uchar8 xbaddr_fnGetStartOfDynamicAddressRange( void )
{
    uchar8 ucAddr;
    // If getting my node name from the application fails
    // or the reserved bit in the name is not zero
    if( ( XBPLATFORM_fnLoad( XB_ePARAM_TYPE_NAME,
                             XB_eCFG_TYPE_USER,
                             tzMyNetNAME.aucNAMEArray,
                             XB_NAME_LEN ) != ( tuiSTATUS )eSTATUS_OK )
     || ( ( tzMyNetNAME.aucNAMEArray[ 6 ] & 0x01 ) != 0 ) )
    {
        // Failure - do not know device function
        ucAddr = RVC_COMP_DYN_ADDR_EXT_START;
    }
    // Convert application name to network name
    else if( XBADDR_fnNetNAME2NAME( &tzMyNetNAME,
                                    (XB_tzPGN_ISO_ADDR_CLAIM *)&tzMyNAME ) == FALSE )
    {
        // Failure - do not know device function
        ucAddr = RVC_COMP_DYN_ADDR_EXT_START;
    }
    else
    {
        // If the network name is available return the end of the address range
        // specific for device function
        switch( tzMyNAME.teDeviceFunction )
        {
        case RVC_eFUNC_CODE_BATTERY_MONITOR:
            ucAddr = RVC_SENSORS_DYNAMIC_ADDR_START;
            break;
        case RVC_eFUNC_CODE_ENERGY_MANAGEMENT_SYSTEM:
        case RVC_eFUNC_CODE_BATTERY_MANAGEMENT_SYSTEM:
        case RVC_eFUNC_CODE_INVERTER_CHARGER:
        case RVC_eFUNC_CODE_INVERTER:
        case RVC_eFUNC_CODE_CHARGER:
            ucAddr = RVC_PWR_COMP_DYN_ADDR_START;
            break;
        case RVC_eFUNC_CODE_USER_INTERFACE:
        case RVC_eFUNC_CODE_SERVICE_TOOL:
        case RVC_eFUNC_CODE_DIAGNOSTIC_TERMINAL:
        case RVC_eFUNC_CODE_GATEWAY:
        case RVC_eFUNC_CODE_SYSTEM_CLOCK:
            ucAddr = RVC_CTRL_DISP_DYNAMIC_ADDR_START;
            break;
        default:
            ucAddr = RVC_COMP_DYN_ADDR_EXT_START;
            break;
        }
    }

    return ucAddr;
}

/***************************************************************************//**

    Dynamic CAN address searching

*******************************************************************************/
static uchar8 xbaddr_fnDynamicAddr( uchar8 ucAddr )
{
    // Have we reached the lowest dynamic address?
    if( ucAddr == RVC_COMP_DYN_ADDR_EXT_START )
    {
        if( xbaddr_fnIsDynamicAddr( ucSaveAddr ) == FALSE )
        {
            // reach the lowest value, end searching
            ucAddr = ucSaveAddr;
        }
        else
        {
            // Address is the lowest value use highest address to start
            ucAddr = xbaddr_fnGetEndOfDynamicAddressRange();
        }
    }
    else if( ucAddr == xbaddr_fnGetStartOfDynamicAddressRange() )
    {
        // Go to the extended range
        ucAddr = RVC_COMP_DYN_ADDR_EXT_END;
    }
    else if( xbaddr_fnIsDynamicAddr( ucAddr ) )
    {
        ucAddr--;
    }
    else
    {
        // Not in dynamic address range, use highest address to start searching
        ucAddr = xbaddr_fnGetEndOfDynamicAddressRange();
    }

    return ucAddr;
}

/******************************************************************************

FUNCTION NAME:
    XBADDR_fnBeginClaiming      - Initialize and start address claim sequence

PURPOSE:
    This function is used to begin the address claiming operation

INPUTS:
    None

OUTPUTS:
    FALSE if a detectable error occurred.
    TRUE if no error detected

NOTES:

VERSION HISTORY:

Version 1.01  Date: 06/24/03  By: Dalem
    - Major change to XB API

Version 1.02  Date: 03/09/17  By: Doru Botez
    - Re-organized to meet RV-C spec.Use the xbaddr_fnCheckAndSetProperAdd()
      function to get a proper address.

Version 1.03 Date: 2023/03/07  By: Peter Newman
    - Start dynamic address claiming using either the Preferred Address or
      the highest end of the address range allocated for the particular device
      function/type.

******************************************************************************/
tucBOOL XBADDR_fnBeginClaiming( void )
{
    // If getting my node name from the application fails
    // or the reserved bit in the name is not zero
    if( ( XBPLATFORM_fnLoad( XB_ePARAM_TYPE_NAME,
                             XB_eCFG_TYPE_USER,
                             tzMyNetNAME.aucNAMEArray,
                             XB_NAME_LEN ) != ( tuiSTATUS )eSTATUS_OK )
     || ( ( tzMyNetNAME.aucNAMEArray[ 6 ] & 0x01 ) != 0 ) )
    {
        // Failure
        return( FALSE );
    }

    // Convert application name to network name
    if( XBADDR_fnNetNAME2NAME( &tzMyNetNAME,
                               (XB_tzPGN_ISO_ADDR_CLAIM *)&tzMyNAME ) == FALSE )
    {
        // Failure
        return( FALSE );
    }

    // Start claiming with the Preferred Address
    xbaddr_fnGetMyPreferredAddress();

    // Start random timer
    XBINIT_fnStartProTimer( ( xbaddr_fnGetRandonDelay() / XB_CENTI_TO_MILLI ),
                            xbaddr_fnRandTimerTimeout );

    // Set address claiming state to state 1
    ucClaimingState = XBADDR_eNMEA_STATE1;

    // Success
    return( TRUE );
}

/******************************************************************************

FUNCTION NAME:
    xbaddr_fnState2             - Perform NMEA state 2 processing

PURPOSE:
    This function carries out the NMEA claiming state 2 processing

INPUTS:
    None

OUTPUTS:
    Nothing

NOTES:
    Transmit an Address Claim Message and start a 250 ms delay timer

VERSION HISTORY:

Version 1.01  Date: 06/24/03  By: Dalem
    - Major change to XB API

******************************************************************************/

static void xbaddr_fnState2( void )
{
#if( DEVICE_CLAIM_PGN != RVC_PGN_DIAG_MSG1 )
    PGN_tzWANT_DATA tzWant;
#endif

    // Set state to state 2
    ucClaimingState = XBADDR_eNMEA_STATE2;

    // Clear random delay or claim timer
    XBINIT_fnStopProTimer();

    // Action depends on device claim message
    // if not use address claim for device detection then
    // send req for address claim to see if the address is used
    // if use address claim, then try an address claim
    //
#if( DEVICE_CLAIM_PGN == RVC_PGN_DIAG_MSG1 )
    // Request address claim to be sent just to my address
    xbaddr_fnIsoReqAddrClaim( ucMyAddr );
#else
    // Send claim to global address
    tzWant.tucSolicited = FALSE;
    tzWant.ucDestAddr = PGN_ADDR_GLOBAL;

    // Send address claim
    XBADDR_fnWantClaim( &tzWant );
#endif

    // Start 250 millisecond claim timer
    XBINIT_fnStartProTimer( XB_ADDR_CLAIM_TO / XB_CENTI_TO_MILLI,
                            xbaddr_fnClaimTimerTimeout );

    // Set state to state 3
    ucClaimingState = XBADDR_eNMEA_STATE3;
}

/******************************************************************************

FUNCTION NAME:
    xbaddr_fnState4             - Perform NMEA state 4 processing

PURPOSE:
    This function carries out the NMEA claiming state 4 processing

INPUTS:
    None

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.02  Date: 06/24/03  By: Dalem
    - Major change to XB API

Version 1.03  Date: 09/12/03  By: Dalem
    - Call fnState6

Version 1.04  Date: 03/09/17  By: Doru Botez
    - Re-organized to meet RV-C spec.Use the xbaddr_fnCheckAndSetProperAdd()
      function to get a proper address.

******************************************************************************/

static void xbaddr_fnState4( void )
{
    // Set state to state 4
    ucClaimingState = XBADDR_eNMEA_STATE4;

    // Clear address claim timer
    XBINIT_fnStopProTimer();

    // Select new address - decrement for RV-C protocol
    ucMyAddr = xbaddr_fnDynamicAddr( ucMyAddr );

    // If address selection has wrapped back to starting address
    if( ucMyAddr == ucSaveAddr )
    {
        // Start MNEA loop of death
        xbaddr_fnState6();
    }
    else
    {
        // Try this address
        xbaddr_fnState2();
    }
}

/******************************************************************************

FUNCTION NAME:
    xbaddr_fnState6             - Perform NMEA state 6 processing

PURPOSE:
    This function carries out the NMEA claiming state 6 processing

INPUTS:
    None

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.00  Date: 09/12/03  By: Dalem
    - Created

******************************************************************************/

static void xbaddr_fnState6( void )
{
    // Set my address to the null address
    ucMyAddr = PGN_ADDR_NULL;

    // Start random timer
    XBINIT_fnStartProTimer( xbaddr_fnGetRandonDelay() / XB_CENTI_TO_MILLI,
                            xbaddr_fnRandTimerTimeout );

    // Set state to state 6
    ucClaimingState = XBADDR_eNMEA_STATE6;
}


/******************************************************************************

FUNCTION NAME:
    xbaddr_fnState7             - Perform NMEA state 7 processing

PURPOSE:
    This function carries out the NMEA claiming state 7 processing

INPUTS:
    None

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.02  Date: 06/24/03  By: Dalem
    - Major change to XB API

******************************************************************************/

static void xbaddr_fnState7( void )
{
    PGN_tzWANT_DATA tzWant;

    // Set state to state 7
    ucClaimingState = XBADDR_eNMEA_STATE7;

    // Send claim to global address
    tzWant.tucSolicited = FALSE;
    tzWant.ucDestAddr = PGN_ADDR_GLOBAL;

    // Send address claim
    XBADDR_fnWantClaim( &tzWant );

    // Set state to state 8
    ucClaimingState = XBADDR_eNMEA_STATE8;
}

/******************************************************************************

FUNCTION NAME:
    xbaddr_fnState9             - Perform NMEA state 9 processing

PURPOSE:
    This function carries out the NMEA claiming state 9 processing

INPUTS:
    None

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.00  Date: 09/12/03  By: Dalem
    - Created

******************************************************************************/

static void xbaddr_fnState9( void )
{
    PGN_tzWANT_DATA tzWant;

    // Set state to state 9
    ucClaimingState = XBADDR_eNMEA_STATE9;

    // Send claim to global address
    tzWant.tucSolicited = FALSE;
    tzWant.ucDestAddr = PGN_ADDR_GLOBAL;


    // Send address claim
    XBADDR_fnWantClaim( &tzWant );

    // Set state to state 5
    ucClaimingState = XBADDR_eNMEA_STATE5;
}

/***************************************************************************//**

    This function carries out the RVC initial addr claiming processing

*******************************************************************************/
static void xbaddr_fnState9A( void )
{
    PGN_tzWANT_DATA tzWant;

    // Set state to state 9
    ucClaimingState = XBADDR_eNMEA_STATE9A;

    // Send claim to global address
    tzWant.tucSolicited = FALSE;
    tzWant.ucDestAddr = 0x00;   //RVC addr claim is EE00;

    // Send address claim
    XBADDR_fnWantClaim( &tzWant );

    // Start 250 millisecond claim timer
    XBINIT_fnStartProTimer( XB_ADDR_CLAIM_TO / XB_CENTI_TO_MILLI,
                            xbaddr_fnClaimTimerTimeout );

}

/******************************************************************************

FUNCTION NAME:
    xbaddr_fnRandTimerTimeout        - Handler for Random Start Timer event

PURPOSE:
    This function handles the timeout of the Random Timer

INPUTS:
    None

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.01  Date: 06/24/03  By: Dalem
    - Major change to XB API

******************************************************************************/

static void xbaddr_fnRandTimerTimeout( void )
{
    // Action depends on NMEA claiming state
    switch( ucClaimingState )
    {
        case XBADDR_eNMEA_STATE1:
        {
            // Start address claiming
            // jump directly to state 9 then 5 if static addressing type
            if( tzMyNAME.ucSelfCfgAddr == 0 )
            {
                xbaddr_fnState9();
            }
            else
            {
                xbaddr_fnState2();
            }
        }
        break;

        case XBADDR_eNMEA_STATE6:
        {
            // Send address claim
            xbaddr_fnState7();
        }
        break;

        case XBADDR_eNMEA_STATE0:
        case XBADDR_eNMEA_STATE2:
        case XBADDR_eNMEA_STATE3:
        case XBADDR_eNMEA_STATE4:
        case XBADDR_eNMEA_STATE5:
        case XBADDR_eNMEA_STATE7:
        case XBADDR_eNMEA_STATE8:
        case XBADDR_eNMEA_STATE9:
        case XBADDR_eNMEA_STATE9A:
        case XBADDR_eNMEA_STATE10:
        default:
        {
            // Should never get to this condition
            XBINIT_fnSaveError( PGN_eESRC_CLAIMING,
                                ( uint16 )PGN_eERR_BAD_STATE );
        }
        break;
    }
}

/******************************************************************************

FUNCTION NAME:
    xbaddr_fnClaimTimerTimeout         - Handler for the Claim Timer event

PURPOSE:
    This function handles the timeout of the Claim Timer

INPUTS:
    None

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.01  Date: 06/24/03  By: Dalem
    - Major change to XB API

******************************************************************************/

static void xbaddr_fnClaimTimerTimeout( void )
{
    if( ucClaimingState == XBADDR_eNMEA_STATE3 )
    {
        // Save address as preferred address
        // Ignore return code
        (void) XBPLATFORM_fnSave( XB_ePARAM_TYPE_ADDRESS,
                                  XB_eCFG_TYPE_USER,
                                  &ucMyAddr,
                                  sizeof( ucMyAddr ) );

        // Save address for re-claiming process
        ucSaveAddr = ucMyAddr;

#if( DEVICE_CLAIM_PGN == RVC_PGN_DIAG_MSG1 )
        // Set claiming state to 9A -- initial addr claim
        xbaddr_fnState9A();
#else
        // Set claiming state 9 then 5 -- normal operation
        xbaddr_fnState9();
#endif
    }
    else if( ucClaimingState == XBADDR_eNMEA_STATE9A )
    {
        // Set state to state 5
        ucClaimingState = XBADDR_eNMEA_STATE5;
    }
    else
    {
        // Should never get to this condition
        XBINIT_fnSaveError( PGN_eESRC_CLAIMING,
                            ( uint16 )PGN_eERR_BAD_STATE );
    }
}

/******************************************************************************

FUNCTION NAME:
    xbaddr_fnChallenge          - Determine outcome of Address Name Challenge

PURPOSE:
    This function is used to when a node sends an address claim message
    with the same address as this node is using.

INPUTS:
    'ptzOther' is net name of node claiming our address

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.01  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.02  Date: 01/21/04  By: Peterd
    - Corrected 'siCmp' compares to be '==' instead of '<' and '>'

******************************************************************************/
static void xbaddr_fnChallenge( XB_tzNET_NAME *ptzOther )
{
    sint16 siCmp;
    PGN_tzWANT_DATA tzWant;

    // Verify parameter
    (void)assert( ptzOther != NULL );

    // Compare my name against challenger
    siCmp = xbaddr_fnCompareNetNames( &tzMyNetNAME,
                                      ptzOther );

    // If the name is the same
    if( siCmp == XB_CMP_NAME_1_EQ_2 )
    {
        // It's me, ignore it
        return;
    }

    //Send out addrclaim if static addressing type
    if( tzMyNAME.ucSelfCfgAddr == 0 )
    {
        // If my name is better (lower) -- I Win
        if( siCmp == XB_CMP_NAME_1_LT_2 )
        {
            xbaddr_fnState9();
            return;
        }
        else
        {
            // Another device uses the same static address, cannot transmit
            // anything; need to wait for the other device to go offline
            ucClaimingState = XBADDR_eNMEA_STATE10;
            return;
        }
    }

    // Action depends on NMEA claiming state
    switch( ucClaimingState )
    {
        case XBADDR_eNMEA_STATE3:
        {
            // If my name is better (lower) -- I Win
            if( siCmp == XB_CMP_NAME_1_LT_2 )
            {
#if( DEVICE_CLAIM_PGN == RVC_PGN_DIAG_MSG1 )
                // keep waiting for timeout
                ucClaimingState = XBADDR_eNMEA_STATE3;
#else
                // Restart address claiming
                xbaddr_fnState2();
#endif
            }
            else
            {
                // I lose, do state 4 processing
                xbaddr_fnState4();
            }
        }
        break;

        case XBADDR_eNMEA_STATE5:
        case XBADDR_eNMEA_STATE9A:
        {
            // If my name is better (lower) -- I Win
            if( siCmp == XB_CMP_NAME_1_LT_2 )
            {
                // Send claim to global address
                tzWant.tucSolicited = FALSE;
                tzWant.ucDestAddr = PGN_ADDR_GLOBAL;

                // Send address claim
                XBADDR_fnWantClaim( &tzWant );
            }
            else
            {
                // I lose, do state 4 processing
                xbaddr_fnState4();
            }
        }
        break;

        case XBADDR_eNMEA_STATE0:
        case XBADDR_eNMEA_STATE1:
        case XBADDR_eNMEA_STATE2:
        case XBADDR_eNMEA_STATE4:
        case XBADDR_eNMEA_STATE6:
        case XBADDR_eNMEA_STATE7:
        case XBADDR_eNMEA_STATE8:
        case XBADDR_eNMEA_STATE9:
        case XBADDR_eNMEA_STATE10:
        default:
        {
            // Should never get to this condition
            XBINIT_fnSaveError( PGN_eESRC_CLAIMING,
                                ( uint16 )PGN_eERR_BAD_STATE );
        }
    }
}

/******************************************************************************

FUNCTION NAME:
    xbaddr_fnPrcClaim          - Process ISO Address Claim/Node List

PURPOSE:
    This function handles the ISO address claim message for claims that do
    not impact this node's address.

INPUTS:
    'ucSrcAddr' is the source address of the claiming node
    'ptzClaimNameNode' is a pointer to the results of the NAME lookup
    'ptzClaimAddrNode' is a pointer to the results of the address lookup
    'tucAlreadyInList' is TRUE if the NAME is already in this node's list

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.00  Date: 06/29/04  By: Peterd
    - Taken from old XBADDR_fnRecvClaim
Version 1.01  Date: 07/05/04  By: Peterd
    - Added call to remove newly added node if it loses the battle

******************************************************************************/

static void xbaddr_fnPrcClaim( uchar8 ucSrcAddr,
                               XBADDR_tzNODE *ptzClaimNameNode,
                               XBADDR_tzNODE *ptzClaimAddrNode,
                               tucBOOL tucAlreadyInList )
{
    XB_tzPGN_ISO_ADDR_CLAIM tzLongName;       // expanded form of name
    uchar8 ucTempAddr;
    XB_teNODE_CHANGE teNodeChange = ( tucAlreadyInList != FALSE ?
                                      XB_eNODE_CHANGE_ADDRESS :
                                      XB_eNODE_CHANGE_NEW_NODE );

    // 2.1.2.1/2.2.1 The claiming node's address is not in use by another node
    if ( ptzClaimAddrNode == NULL )
    {
        // Convert the name to long format
        if( XBADDR_fnNetNAME2NAME( &ptzClaimNameNode->tzNetName,
                                &tzLongName ) != FALSE )
        {
            // If the claiming node was already in our list
            if ( tucAlreadyInList != FALSE )
            {
                // Save the node's last known address
                ucTempAddr = ptzClaimNameNode->ucAddress;
            }
            else
            {
                // It's new
                ucTempAddr = PGN_ADDR_NULL;
            }

            // Set the new address
            ptzClaimNameNode->ucAddress = ucSrcAddr;

            // Report the change in address to the application
            XBPLATFORM_fnNodeChange( teNodeChange,
                                    ucTempAddr,
                                    ptzClaimNameNode->ucAddress,
                                    &tzLongName );
        }

        // Restart the claim timer
        ptzClaimNameNode->uiTicks = XBTIMERS_fnGetClaimTimer();
    }
    // 2.1.2.2/2.2.2 The claiming node's address is in use by another node
    else
    {
        // Compare the names
        switch( xbaddr_fnCompareNetNames( &ptzClaimNameNode->tzNetName,
                                          &ptzClaimAddrNode->tzNetName ) )
        {
            // 2.1.2.2.1/2.2.2.1 The claiming node's NAME is lower priority
            case XB_CMP_NAME_1_GT_2:
            {
                // The claiming node is going to lose
                // So leave his address as it was and ignore him

                // if the claiming node was not already in this node's list
                if ( tucAlreadyInList == FALSE )
                {
                    // We added earlier so get rid of it now
                    xbaddr_fnClearNode( ptzClaimNameNode );
                }
            }
            break;

            // 2.1.2.2.2/2.2.2.2 The claiming node's NAME is higher priority
            case XB_CMP_NAME_1_LT_2:
            {
                // The claiming node is going to win
                // So, Change his address and bump the other guy

                // Convert the name of the previous user of the address
                if( XBADDR_fnNetNAME2NAME( &ptzClaimAddrNode->tzNetName,
                                           &tzLongName ) != FALSE )
                {
                    uchar8 ucTempNodeAddr = ptzClaimAddrNode->ucAddress;

                    ptzClaimAddrNode->ucAddress = PGN_ADDR_GLOBAL;

                    // Report the (temporary) loss of old node
                    XBPLATFORM_fnNodeChange( XB_eNODE_CHANGE_BUMPED_OFF,
                                             ucTempNodeAddr,
                                             ptzClaimAddrNode->ucAddress,
                                             &tzLongName );

                    xbaddr_fnClearNode( ptzClaimAddrNode );
                }

                // Convert the name to long format
                if( XBADDR_fnNetNAME2NAME( &ptzClaimNameNode->tzNetName,
                                           &tzLongName ) != FALSE )
                {
                    // If the claiming node was already in our list
                    if ( tucAlreadyInList != FALSE )
                    {
                        // Save the node's last known address
                        ucTempAddr = ptzClaimNameNode->ucAddress;
                    }
                    else
                    {
                        // It's new
                        ucTempAddr = PGN_ADDR_NULL;
                    }

                    // Set the new address
                    ptzClaimNameNode->ucAddress = ucSrcAddr;

                    // Report the change in address to the application
                    XBPLATFORM_fnNodeChange( teNodeChange,
                                            ucTempAddr,
                                            ptzClaimNameNode->ucAddress,
                                            &tzLongName );
                }

                // Restart the claim timer
                ptzClaimNameNode->uiTicks = XBTIMERS_fnGetClaimTimer();
            }
            break;
        }
        // Should never get here
    }
}

/*******************************************************************************

FUNCTION NAME:
    xbaddr_fnRespondToCmdAddr

PURPOSE:
    This function is used to check the state before we respond to a Commanded
    Address PGN.
    It is intended for use when a Commanded Address PGN is handled.

INPUTS:
    None

OUTPUTS:
    Ture if we may respond to a Commanded Address PGN;
    False otherwise

NOTES:

VERSION HISTORY:

Version 1.00  Date: 06/09/05  By: Hollyz
    - Created
Version 1.01  Date: 07/25/05  By: Hollyz
    - Updated to find the claim data from the linked list

*******************************************************************************/
static tucBOOL xbaddr_fnRespondToCmdAddr( void )
{
    tucBOOL tucRetVal;

    // Refer to NMEA Address Claim State Diagram
    // Action depends on claiming state
    //
    switch( ucClaimingState )
    {
        case XBADDR_eNMEA_STATE3:
#if( DEVICE_CLAIM_PGN == RVC_PGN_DIAG_MSG1 )
            tucRetVal = FALSE;
#else
            tucRetVal = TRUE;
#endif
            break;
        case XBADDR_eNMEA_STATE5:
        case XBADDR_eNMEA_STATE8:
        case XBADDR_eNMEA_STATE9A:
            // Response is allowed
            tucRetVal = TRUE;
            break;

        case XBADDR_eNMEA_STATE0:
        case XBADDR_eNMEA_STATE1:
        case XBADDR_eNMEA_STATE2:
        case XBADDR_eNMEA_STATE4:
        case XBADDR_eNMEA_STATE6:
        case XBADDR_eNMEA_STATE7:
        case XBADDR_eNMEA_STATE9:
        case XBADDR_eNMEA_STATE10:
            // Cannot accept Commanded Address message when in these states
            tucRetVal = FALSE;
            break;

        default:
            // Error, don't resond to it
            tucRetVal = FALSE;
            break;
    }

    return ( tucRetVal );
}
/*******************************************************************************

FUNCTION NAME:
    xbaddr_fnListAdd

PURPOSE:
    Add a new node at the end of a linked list

INPUTS:
    ppHead - location of the pointer to the first node on the list
    pNode  - pointer to the new node

OUTPUTS:
    Index to the last node on the linked list

NOTES:
    Memory for the new node  shall be successfully located before this function is
    called;


VERSION HISTORY:

Version: 1.00  Date: 07-25-05  By: Hollyz
    - Created

*******************************************************************************/
static uchar8 xbaddr_fnListAdd( XBADDR_tzNODE **ppHead,
                                XBADDR_tzNODE *pNode )
{
    XBADDR_tzNODE *pList;
    uchar8 ucIndex = 0;

    assert( ppHead );
    assert( pNode );

    // pointer to the first node on the list
    pList = *ppHead;

    // if the list was empty
    if( pList == NULL )
    {
        // add the node at the head
        *ppHead = pNode;
    }
    else
    {
        // Search for end of list
        while ( pList->pNext != NULL )
        {
            // Move to next list
            pList = pList->pNext;
            ucIndex++;
        }

        // Insert this node at the end
        pList->pNext = pNode;
        ucIndex++;
    }

    // this is the last node on the list
    pNode->pNext = NULL;

    // return the index to the last node
    return ( ucIndex );
}

/*******************************************************************************

FUNCTION NAME:
    xbaddr_fnListDelete

PURPOSE:
    Delete a node from a linked list

INPUTS:
   ppHead - location of the pointer to the first node on the list
   pNode  - pointer to the node to remove

OUTPUTS:
    None

NOTES:
    Memory for the removed node is not released in this function;

VERSION HISTORY:

Version: 1.00  Date: 07-25-05  By: Hollyz
    - Created

*******************************************************************************/
static void xbaddr_fnListDelete( XBADDR_tzNODE **ppHead,
                                 XBADDR_tzNODE *pNode )
{
    XBADDR_tzNODE *pList;

    assert( ppHead );
    assert( pNode );

    // pointer to the first node on the list
    pList = *ppHead;

    // If the node to delete is the first entry
    if ( pList == pNode )
    {
        // Remove the node at list head by overwriting pointer with listed
        // next node
        *ppHead = pNode->pNext;
    }
    else
    {
        // While not at end of list
        while ( pList != NULL )
        {
            // If the next node is the node to remove
            if ( pList->pNext == pNode )
            {
                // Remove the node by overwriting pointer to listed next node
                pList->pNext = pNode->pNext;

                // Finished
                break;
            }

            // Move to next node
            pList = pList->pNext;
        }
    }
}
/******************************************************************************

FUNCTION NAME:
    XBADDR_fnRecvClaim          - Handler for the ISO Address Claim message

PURPOSE:
    This function handles the ISO address claim message.  It is used when
    address claiming and can also be used during normal operation.  This
    function helps to maintain the active node list.

    When an address claim comes in there are two major possibilities:

    1. The address is already being used by this node
        1.1 This node has a higher priority NAME and must defend its claim
        1.2 This node has a lower priority NAME and must relinquish its address
    2. The address is not being used by this node
        2.1 The claiming node's NAME is already in this node's node list
            2.1.1 The claiming node's address matches:
                    restart the claim timer, DONE
            2.1.2 The claiming node's address does not match
                2.1.2.1 The claiming node's address is not in use by
                        another node: update address, restart the claim timer,
                        DONE
                2.1.2.2 The claiming node's address is in use by another node
                    2.1.2.2.1 The claiming node's NAME is lower priority:
                                no change to this node's list, do NOT
                                restart the claim timer
                    2.1.2.2.2 The claiming node's NAME is higher priority:
                                update address of claiming node, remove
                                lower priority node, restart the claim timer,
                                DONE
        2.2 The claiming node's NAME is not in this node's node list
            2.2.1 The claiming node's address is not in use by another node:
                    Add the new node to the list, restart the claim timer,
                    DONE
            2.2.2 The claiming node's address is in use by another node
                2.2.2.1 The claiming node's NAME is lower priority:
                        no change to this node's list, do NOT restart
                        claim timer
                2.2.2.2 The claiming node's NAME is higher priority:
                        Add the new node to the list, remove lower priority
                        node, restart the claim timer, DONE

INPUTS:
    'ptzRecv' is a pointer to the received message work block

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.03  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.04  Date: 08/25/03  By: Dalem
    - Report address change if node found by name but not by address
Version 1.05  Date: 12/16/03  By: Dalem
    - Added setting of source node flags
Version 1.06  Date: 03/08/04  By: Peterd
    - Added clearing of source node flags prior to setting
Version 1.07  Date: 05/14/04  By: Peterd
    - Changed handling to reduce number of callbacks to XBPLATFORM_fnNodeChange
    and make them more intuitive
Version 1.08  Date: 06/28/04  By: Peterd
    - When new NAME detected, added check to see if claimed address already
    in use. If it is and its lower priority then take it over other wise
    wait for old NAME to timeout.

******************************************************************************/

void XBADDR_fnRecvClaim( PGN_tzRECV_DATA *ptzRecv )
{
    XBADDR_tzNODE *ptzClaimAddrNode;
    XBADDR_tzNODE *ptzClaimNameNode;
    uchar8 ucSrcAddr;

    // Verify parameter
    (void)assert( ptzRecv != NULL );

    // Get local copy of source address for ease of use
    ucSrcAddr = ptzRecv->ucSrcAddr;

    // If received address is my address
    if( ucSrcAddr == ucMyAddr )
    {
        // Someone wants to use my address
        xbaddr_fnChallenge( (XB_tzNET_NAME *) ptzRecv->pvStruct );

        // We're done here
        return;
    }

    // If node address is not NULL or GLOBAL
    if( ( ucSrcAddr == PGN_ADDR_NULL ) || ( ucSrcAddr == PGN_ADDR_GLOBAL ) )
    {
        // We're done here
        return;
    }

    // Register the node

    // Search node list for received NAME
    ptzClaimNameNode = xbaddr_fnFindName( (XB_tzNET_NAME *)ptzRecv->pvStruct );

    // Search node list for received address
    ptzClaimAddrNode = xbaddr_fnFindAddr( ucSrcAddr );

    // 2.1 The claiming node's NAME is already in our node list
    if ( ptzClaimNameNode != NULL )
    {
        // 2.1.1 The claiming node's address matches the NAME
        if ( ptzClaimNameNode == ptzClaimAddrNode )
        {
            // Restart the claim timer
            ptzClaimNameNode->uiTicks = XBTIMERS_fnGetClaimTimer();
            // We're done here
            return;
        }
        // 2.1.2 The claiming node's address does not match the NAME
        else
        {
            // Process the rules as if this was an existing node
            xbaddr_fnPrcClaim( ucSrcAddr, ptzClaimNameNode, ptzClaimAddrNode, TRUE );

            // We're done here
            return;
        }
    }
    // 2.2 The claiming node's NAME is not in this node's node list
    else
    {
        // Is this an address we already have in our list from receiving DiagMsg1 but not AddrClaim?
        if (( ptzClaimAddrNode != NULL )
            && ( xbaddr_fnCompareNetNames( &ptzClaimAddrNode->tzNetName,
                           (XB_tzNET_NAME *)&tzNullNetName ) == XB_CMP_NAME_1_EQ_2 ))
        {
            // Save the NAME here and restart the timer
            ptzClaimAddrNode->tzNetName = *(XB_tzNET_NAME *)ptzRecv->pvStruct;
            ptzClaimAddrNode->uiTicks = XBTIMERS_fnGetClaimTimer();
            // We're done here
            return;
        }

        if( ucTotalNodes >= ucMaxNodeNum  )
        {
            // We reach the max number of nodes we can record. Log the error
            XBINIT_fnSaveError( PGN_eESRC_CLAIMING, ( uint16 )PGN_eERR_NO_ROOM );
            return;
        }

        // Create a new node
        ptzClaimNameNode = XBMEM_fnMalloc( sizeof(XBADDR_tzNODE) );

        if ( ptzClaimNameNode != NULL )
        {
            // Register the node name and address, reset the claim timer
            ptzClaimNameNode->ucAddress = ucSrcAddr;
            ptzClaimNameNode->uiTicks   = XBTIMERS_fnGetClaimTimer();
            memcpy( (void *)ptzClaimNameNode->tzNetName.aucNAMEArray, (void *)ptzRecv->pvStruct, 8 );

            // add the node to the node linked list
            ( void )xbaddr_fnListAdd( &ptzNodeHead, ptzClaimNameNode );

            // increase the number of total nodes on the list
            ucTotalNodes++;

            // Process the rules as if this was a new node
            xbaddr_fnPrcClaim( ucSrcAddr, ptzClaimNameNode, ptzClaimAddrNode, FALSE );

            // We're done here
            return;
        }
        else
        {
            // This is not good, no node list space available, ignore node
            // remember the error
            XBINIT_fnSaveError( PGN_eESRC_CLAIMING,
                              ( uint16 )PGN_eERR_NO_MEM );
            return;
        }

        // This is not good, no node list space available, ignore node
    }
}

/******************************************************************************

FUNCTION NAME:
    XBADDR_fnRecvCmdAddr        - Handle Commanded Address message

PURPOSE:
    This function is used to handle the commanded address message.  If the
    message is accepted then the node starts to claim the new address.

INPUTS:
    'ptzRecv' is a pointer to the received message work block

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.01  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.02  Date: 08/29/03  By: Dalem
    - Added data checking and IsoAcks
Version 1.03  Date: 09/12/03  By: Dalem
    - If address does not change there is no need to reclaim address
    - Change saved address to fix quirk in claiming where a node commanded
    to an address one less than its claimed address would give up if it
    could not claim that address because it would try to claim the next address
    which was its previous address.
Version 1.04  Date: 02/01/05  By: Dalem
    - Sending ISO ACK now needs specific destination address
Version 1.05  Date: 04/19/05  By: Hollyz
    - Changed function to perform security check
Version 1.06  Date: 06/09/05  By: Hollyz
    - Do NAME compare before performing security check;
      Check the Address Claim State before changing the address
Version 1.07  Date: 08/15/06  By: Hollyz
    - Compare the received Name with my Name after converting the expanded
      Name to short format Name. Fix PRN4198.
Version 1.08  Date: 03/09/17  By: Doru Botez
    - Re-organized to meet RV-C spec.Use the xbaddr_fnCheckIfProperAddr()
      function to check if it is a proper address.

******************************************************************************/
void XBADDR_fnRecvCmdAddr( PGN_tzRECV_DATA *ptzRecv )
{
    XB_tzPGN_ISO_CMD_ADDR   *ptzCmdAddr;
    XB_tzPGN_ISO_ADDR_CLAIM *ptzName;
    uchar8                   aucShortNAME[ XB_NAME_LEN ];


    // Verify parameter
    (void)assert( ptzRecv != NULL );

    // Convert void pointer to enable/disable config pointer
    ptzCmdAddr = ptzRecv->pvStruct;

    // Cast commanded address to address claim, the structures are defined
    // to be matching
    ptzName = (XB_tzPGN_ISO_ADDR_CLAIM *) ptzCmdAddr;

    // Convert commanded name to short form
    XBADDR_fnNAME2ShortNAME( ptzName,
                             aucShortNAME );

    // If name matches my name
    if( memcmp( aucShortNAME,
                &tzMyNetNAME,
                XB_NAME_LEN ) == 0 )
    {
        // Check the state
        if( TRUE != xbaddr_fnRespondToCmdAddr( ) )
        {
            // No response shall be send out if we are not ready to receive
            // a Commanded Address message
            return;
        }

        // Reject and ingore message if it is not from Diag
        if( XBMSG_fnIsFromDiag( ptzRecv->ucSrcAddr ) == FALSE )
        {
            // Reject with an Deny
            XBMSG_fnSendISOAck( XB_PGN_ISO_CMD_ADDR,
                                XB_eGRP_FUNC_VALUE_SECURITY,
                                XB_eACK_STS_DNY,
                                ptzRecv->ucSrcAddr );

            return;
        }

        // If address acceptable
        if( xbaddr_fnCheckIfProperAddr( ptzCmdAddr->ucPrefSrcAddr ) == TRUE )
        {
            // Send Ack before address change starts
            XBMSG_fnSendISOAck( XB_PGN_ISO_CMD_ADDR,
                                XB_eGRP_FUNC_VALUE_DO_NOT_CARE,
                                XB_eACK_STS_ACK,
                                ptzRecv->ucSrcAddr );

            // If the address is changing
            if( ptzCmdAddr->ucPrefSrcAddr != ucMyAddr )
            {
                // Set my network address from message
                ucMyAddr = ptzCmdAddr->ucPrefSrcAddr;

                // Also set this as the saved address so that if this node
                // cannot claim the address it will try all others
                ucSaveAddr = ucMyAddr;

                // Start address claiming for new address
                xbaddr_fnState2();
            }
        }
        else
        {
            // Reject with an NAK
            XBMSG_fnSendISOAck( XB_PGN_ISO_CMD_ADDR,
                                XB_eGRP_FUNC_VALUE_DATA_INVALID,
                                XB_eACK_STS_NAK,
                                ptzRecv->ucSrcAddr );
        }
    }
}

/******************************************************************************

FUNCTION NAME:
    XBADDR_fnReloadNAME         - Handle change in name

PURPOSE:
    This function is used to reload the node name and reclaim an address for
    the new name.

INPUTS:
    None

OUTPUTS:
    FALSE if a detectable error occurred.
    TRUE if no error detected

NOTES:

VERSION HISTORY:

Version 1.00  Date: 08/15/03  By: Dalem
    - Created
Version 1.01  Date: 09/12/03  By: Dalem
    - Reset the preferred address if it is invalid
Version 1.02  Date: 03/09/17  By: Doru Botez
    - Re-organized to meet RV-C spec.Use the xbaddr_fnCheckAndSetProperAddr()
      function to get a proper address.

******************************************************************************/

tucBOOL XBADDR_fnReloadNAME( XB_tzPGN_ISO_ADDR_CLAIM *ptzName )
{

    // Verify parameter
    (void)assert( ptzName != NULL );

    // Copy config name to my local extended name
    (void) memcpy( &tzMyNAME,
                   ptzName,
                   sizeof( tzMyNAME ) );

    // Convert my extended name to network name for internal use
    // and save my node name
    XBADDR_fnNAME2ShortNAME( (XB_tzPGN_ISO_ADDR_CLAIM *)&tzMyNAME,
                             tzMyNetNAME.aucNAMEArray );


    if( XBPLATFORM_fnSave( XB_ePARAM_TYPE_NAME,
                           XB_eCFG_TYPE_USER,
                           &tzMyNetNAME,
                           XB_NAME_LEN ) == ( tuiSTATUS )eSTATUS_OK )
    {
        // We are invalidating our current address

        // Check if current address is not proper
        ucMyAddr  = xbaddr_fnCheckAndSetProperAddr( ucMyAddr );

        // Also reset the saved address so that if this node
        // cannot claim the address it will try all others
        ucSaveAddr = ucMyAddr;

        // Start address claiming for new name
        xbaddr_fnState2();

        //Success
        return( TRUE );
    }
    else
    {
        // Failure
        return( FALSE );
    }
}

/******************************************************************************

FUNCTION NAME:
    XBADDR_fnRecvNAMECfg        - Handle Name Configuration message

PURPOSE:
    This function is used to handle the name configuration message.  If the
    message is accepted then the node starts to claim the new address.

INPUTS:
    'ptzRecv' is a pointer to the received message work block

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.02  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.03  Date: 08/14/03  By: Dalem
    - Handle different configuration types
    - Send ISO ACKs
Version 1.04  Date: 08/22/03  By: Dalem
    - System Instance is not a read only field
Version 1.05  Date: 09/02/03  By: Dalem
    - Code restructured to set reasons for SendIsoAck
Version 1.06  Date: 09/02/03  By: Dalem
    - Must send ack before address claiming stars
Version 1.07  Date: 12/12/03  By: Dalem
    - Reject if broadcast
    - Reject if changing read only field
Version 1.08  Date: 03/08/04  By: Peterd
    - Removed check of read-only fields
Version 1.09  Date: 06/25/04  By: Peterd
    - Changed call to XBADDR_fnCheckCfgSrc to include Ack Type parameter
    - PRN 2514

******************************************************************************/

void XBADDR_fnRecvNAMECfg( PGN_tzRECV_DATA *ptzRecv )
{
    XB_tzPGN_NAME_CFG   *ptzCfg;
    XB_tzNAME_CONFIG    tzNameCfg;
    XB_teACK_STS        teAckType;
    XB_teGRP_FUNC_VALUE teReason;

    // Verify parameter
    (void)assert( ptzRecv != NULL );

    // Convert void pointer to enable/disable config pointer
    ptzCfg = ptzRecv->pvStruct;

    // Set a default response
    teAckType = XB_eACK_STS_NAK;
    teReason = XB_eGRP_FUNC_VALUE_DATA_NOT_AVAILABLE;

    // If the message count is zero then it is a command to change the
    // NAME config, otherwise, it is a report of the current configuration
    // to be handled by the application layer.
    if( ptzCfg->ucMsgCount != 0 )
    {
        // Send the message to the application
        XBPLATFORM_fnRecvXbMsg( ptzRecv );
        return;
    }

    // If message is a broadcast
    if ( ptzRecv->ucDestAddr == PGN_ADDR_GLOBAL )
    {
        // Reject for protocol (cannot broadcast name cfg)
        teAckType = XB_eACK_STS_NAK;
        teReason = XB_eGRP_FUNC_VALUE_PROTOCOL;
    }

    // Check the source of the configuration command
    else if( XBMSG_fnCheckCfgSrc( ptzCfg->teCfgType,
                                ptzRecv->ucSrcAddr,
                                &teAckType ) == FALSE )
    {
        // Reject with appropriate code
        teReason = XB_eGRP_FUNC_VALUE_SECURITY;
    }
    // If we are in the wrong mode to handle this configuration
    else if (( XBINIT_pfnAppCfgFilter != NULL )
         && !XBINIT_pfnAppCfgFilter( ptzRecv->ulPgn ) )
    {
        teAckType = XB_eACK_STS_DNY;
        teReason = XB_eGRP_FUNC_VALUE_WRONG_MODE;
    }
    // If reading of configuration fails
    else if( XBPLATFORM_fnLoad( XB_ePARAM_TYPE_NAME_CONFIG,
                                ptzCfg->teCfgType,
                                &tzNameCfg,
                                sizeof( tzNameCfg ) ) != ( tuiSTATUS )eSTATUS_OK )
    {
        // Reject for read error
        teAckType = XB_eACK_STS_NAK;
        teReason = XB_eGRP_FUNC_VALUE_DATA_ACCESS_ERROR;
    }
    // If the concurrent access counter (from loaded data) does not match
    else if( ptzCfg->ucConcurAccCtr != tzNameCfg.ucConcurAccCtr )
    {
        // Reject for CAC
        teAckType = XB_eACK_STS_NAK;
        teReason = XB_eGRP_FUNC_VALUE_CAC;
    }
    else
    {
        // Copy fields that are not read only to the local copy of the node name
        tzNameCfg.tzNAME.ulUniqueNumber        = ptzCfg->ulUniqueNumber;
        tzNameCfg.tzNAME.ucDeviceInstanceLower = ptzCfg->ucDeviceInstanceLower;
        tzNameCfg.tzNAME.ucDeviceInstanceUpper = ptzCfg->ucDeviceInstanceUpper;
        tzNameCfg.tzNAME.ucSystemInstance      = ptzCfg->ucSystemInstance;
        tzNameCfg.tzNAME.teIndustryGroup       = ptzCfg->teIndustryGroup;

        // Increment concurrent access counter
        tzNameCfg.ucConcurAccCtr++;

        // If saving configuration fails
        if( XBPLATFORM_fnSave( XB_ePARAM_TYPE_NAME_CONFIG,
                               ptzCfg->teCfgType,
                               &tzNameCfg,
                               sizeof( tzNameCfg ) ) != ( tuiSTATUS )eSTATUS_OK )
        {
            // Reject for write error
            teAckType = XB_eACK_STS_NAK;
            teReason = XB_eGRP_FUNC_VALUE_DATA_ACCESS_ERROR;
        }
        else
        {
            // Configuration accepted
            teAckType = XB_eACK_STS_ACK;
            teReason = XB_eGRP_FUNC_VALUE_DO_NOT_CARE;

            // If the configuration is USER
            if( ptzCfg->teCfgType == XB_eCFG_TYPE_USER )
            {
                // Reply with ack before name is changed
                XBMSG_fnSendISOAck( XB_PGN_NAME_CFG,
                                    teReason,
                                    teAckType,
                                    ptzRecv->ucSrcAddr );

                // Reload the name (and reclaim address)
                ( void )XBADDR_fnReloadNAME( &tzNameCfg.tzNAME );

                // Done
                return;
            }
        }
    }

    // Reply
    XBMSG_fnSendISOAck( XB_PGN_NAME_CFG,
                        teReason,
                        teAckType,
                        ptzRecv->ucSrcAddr );
}


/******************************************************************************

FUNCTION NAME:
    xbaddr_fnSendOneNameCfg     - Send Name Config for one configuration type

PURPOSE:
    This function is used to build and send a name configuration message

INPUTS:
    'teCfgType' is configuration type to send
    'ucMsgCount' is message count for config message
    'ucDest' is destination for the message

OUTPUTS:
    none

NOTES:

VERSION HISTORY:

Version 1.00  Date: 08/15/03  By: Dalem
    - Created

******************************************************************************/

static void xbaddr_fnSendOneNameCfg( XB_teCFG_TYPE teCfgType,
                                     uchar8 ucMsgCount,
                                     uchar8 ucDest )
{
    XB_tzPGN_NAME_CFG tzCfg;
    XB_tzNAME_CONFIG  tzGetName;

    // If read of the configuration succeeds
    if( XBPLATFORM_fnLoad( XB_ePARAM_TYPE_NAME_CONFIG,
                           teCfgType,
                           &tzGetName,
                           sizeof( tzGetName ) ) == ( tuiSTATUS )eSTATUS_OK )
    {
        // Set the configuration type and message count
        tzCfg.teCfgType = teCfgType;
        tzCfg.ucMsgCount = ucMsgCount;

        // Copy the concurrent access counter and name
        if ( teCfgType == XB_eCFG_TYPE_FACTORY )
        {
            tzCfg.ucConcurAccCtr = PGN_UC_NO_DATA;
        }
        else
        {
            tzCfg.ucConcurAccCtr        = tzGetName.ucConcurAccCtr;
        }
        tzCfg.ulUniqueNumber        = tzGetName.tzNAME.ulUniqueNumber;
        tzCfg.uiManufacturerCode    = tzGetName.tzNAME.uiManufacturerCode;
        tzCfg.ucDeviceInstanceLower = tzGetName.tzNAME.ucDeviceInstanceLower;
        tzCfg.ucDeviceInstanceUpper = tzGetName.tzNAME.ucDeviceInstanceUpper;
        tzCfg.teDeviceFunction      = tzGetName.tzNAME.teDeviceFunction;
        tzCfg.ucRsvBit              = tzGetName.tzNAME.ucRsvBit;
        tzCfg.teDeviceClass         = tzGetName.tzNAME.teDeviceClass;
        tzCfg.ucSystemInstance      = tzGetName.tzNAME.ucSystemInstance;
        tzCfg.teIndustryGroup       = tzGetName.tzNAME.teIndustryGroup;
        tzCfg.ucSelfCfgAddr         = tzGetName.tzNAME.ucSelfCfgAddr;

        // Send a Name Config PGN
        XBMSG_fnSend( XB_PGN_NAME_CFG,
                      &tzCfg,
                      sizeof(tzCfg),
                      ucDest );
    }
}

/******************************************************************************

FUNCTION NAME:
    XBADDR_fnWantNAMECfg        - Send Name Config message

PURPOSE:
    This function is used to build and send name configuration messages

INPUTS:
    'ucDest' is destination for message send
    'tucSolicited' is TRUE if request is from network

OUTPUTS:
    none

NOTES:

VERSION HISTORY:

Version 1.00  Date: 08/15/03  By: Dalem
    - Created

******************************************************************************/

void XBADDR_fnWantNAMECfg( PGN_tzWANT_DATA *ptzWant )
{
    uchar8 ucMsgCount;

    // Message count depends on message solicitation
    ucMsgCount = ( ptzWant->tucSolicited ? 1 : 0 );


    // Send the current active timers as USER configuration
    xbaddr_fnSendOneNameCfg( XB_eCFG_TYPE_USER,
                             ucMsgCount,
                             ptzWant->ucDestAddr );

    // If the requester/destination is a diag
    if( XBMSG_fnIsFromDiag( ptzWant->ucDestAddr ) != FALSE )
    {
        // Must send OEM and Factory config as well

        // Send OEM configuration
        xbaddr_fnSendOneNameCfg( XB_eCFG_TYPE_OEM,
                                 ucMsgCount,
                                 ptzWant->ucDestAddr );

        // Send Factory configuration
        xbaddr_fnSendOneNameCfg( XB_eCFG_TYPE_FACTORY,
                                 ucMsgCount,
                                 ptzWant->ucDestAddr );
    }
}

/******************************************************************************

FUNCTION NAME:
    XBADDR_fnResetNameCfg       - Handle a reset config command for Name Config

PURPOSE:
    This function is used to manage a command to reset the name configuration.

INPUTS:
    'teCmd' is the type of the configuration reset command

OUTPUTS:
    TRUE if successful
    FALSE if error occurred

NOTES:

VERSION HISTORY:

Version 1.00  Date: 08/14/03  By: Dalem
    - Created

******************************************************************************/

tucBOOL XBADDR_fnResetNameCfg( XB_teRST_CMD teCmd )
{
    XB_tzNAME_CONFIG tzNameCfg;
    uchar8 ucUserCac = 0;   // User Cfg Concurrent Access Counter
    uchar8 ucOemCac = 0;    // OEM Cfg Concurrent Access Counter

    // Read the concurrent access counters from present configurations
    switch( teCmd )
    {
        case XB_eRST_CMD_FACTORY:
        {
            // If reading of OEM configuration fails
            if( XBPLATFORM_fnLoad( XB_ePARAM_TYPE_NAME_CONFIG,
                                   XB_eCFG_TYPE_OEM,
                                   &tzNameCfg,
                                   sizeof( tzNameCfg ) ) != ( tuiSTATUS )eSTATUS_OK )
            {
                // Configuration does not exist
                ucOemCac = 0;
            }
            else
            {
                // Save Access counter for OEM
                ucOemCac = tzNameCfg.ucConcurAccCtr;
            }
        }
        //lint -fallthrough
        // Fall through to OEM command
        case XB_eRST_CMD_OEM:
        {
            // If reading of User configuration fails
            if( XBPLATFORM_fnLoad( XB_ePARAM_TYPE_NAME_CONFIG,
                                   XB_eCFG_TYPE_USER,
                                   &tzNameCfg,
                                   sizeof( tzNameCfg ) ) != ( tuiSTATUS )eSTATUS_OK )
            {
                // Configuration does not exist
                ucUserCac = 0;
            }
            else
            {
                // Save Access counter for user
                ucUserCac = tzNameCfg.ucConcurAccCtr;
            }
        }
        break;

        case XB_eRST_CMD_REBOOT:
        case XB_eRST_CMD_REBOOT_DL_MODE:
        case XB_eRST_CMD_CLEAR_NV_AND_REBOOT:
        case XB_eRST_CMD_NO_CHANGE:
        default:
            // Failure
            return( FALSE );
    }

    // Determine which configuration we will use
    switch( teCmd )
    {
        case XB_eRST_CMD_FACTORY:
        {
            // If read of Factory configuration fails
            if( XBPLATFORM_fnLoad( XB_ePARAM_TYPE_NAME_CONFIG,
                                   XB_eCFG_TYPE_FACTORY,
                                   &tzNameCfg,
                                   sizeof( tzNameCfg ) ) != ( tuiSTATUS )eSTATUS_OK )
            {
                // Failure
                return( FALSE );
            }
        }
        break;

        case XB_eRST_CMD_OEM:
        {
            // If read of OEM configuration fails
            if( XBPLATFORM_fnLoad( XB_ePARAM_TYPE_NAME_CONFIG,
                                   XB_eCFG_TYPE_OEM,
                                   &tzNameCfg,
                                   sizeof( tzNameCfg ) ) != ( tuiSTATUS )eSTATUS_OK )
            {
                // Failure
                return( FALSE );
            }
        }
        break;

        case XB_eRST_CMD_REBOOT:
        case XB_eRST_CMD_REBOOT_DL_MODE:
        case XB_eRST_CMD_CLEAR_NV_AND_REBOOT:
        case XB_eRST_CMD_NO_CHANGE:
        default:
            // Failure
            return( FALSE );
    }

    // Increment access counters and save configurations
    switch( teCmd )
    {
        case XB_eRST_CMD_FACTORY:
        {
            // Increment access counter for OEM
            tzNameCfg.ucConcurAccCtr = ucOemCac + 1;

            // If writing of OEM configuration fails
            if( XBPLATFORM_fnSave( XB_ePARAM_TYPE_NAME_CONFIG,
                                   XB_eCFG_TYPE_OEM,
                                   &tzNameCfg,
                                   sizeof( tzNameCfg ) ) != ( tuiSTATUS )eSTATUS_OK )
            {
                // Failure
                return( FALSE );
            }
        }
        //lint -fallthrough
        // Fall through to OEM
        case XB_eRST_CMD_OEM:
        {
            // Increment access counter for User
            tzNameCfg.ucConcurAccCtr = ucUserCac + 1;

            // If writing of OEM configuration fails
            if( XBPLATFORM_fnSave( XB_ePARAM_TYPE_NAME_CONFIG,
                                   XB_eCFG_TYPE_USER,
                                   &tzNameCfg,
                                   sizeof( tzNameCfg ) ) != ( tuiSTATUS )eSTATUS_OK )
            {
                // Failure
                return( FALSE );
            }
        }
        break;

        case XB_eRST_CMD_REBOOT:
        case XB_eRST_CMD_REBOOT_DL_MODE:
        case XB_eRST_CMD_CLEAR_NV_AND_REBOOT:
        case XB_eRST_CMD_NO_CHANGE:
        default:
            // Failure
            return( FALSE );
    }

    // If Reloading of the name fails
    if( XBADDR_fnReloadNAME( &tzNameCfg.tzNAME ) == FALSE )
    {
        // Failure
        return( FALSE );
    }
    else
    {
        // Success
        return( TRUE );
    }
}

/******************************************************************************

FUNCTION NAME:
    XBADDR_fnOnLine         - XanBus stack ready for application messages

PURPOSE:
    This function is used to test if address claiming has completed.

INPUTS:
    None

OUTPUTS:
    TRUE if Address Claiming is complete and application can send/receive,
    FALSE if not ready for application messages.

NOTES:

VERSION HISTORY:

Version 1.00  Date: 09/19/03  By: Dalem
    - Created
Version 1.01  Date: 02/09/09  By: Hollyz
    - Disable the message buffering while in BusOff state

******************************************************************************/

tucBOOL XBADDR_fnOnLine( void )
{
    // If we are on line
    if( ( ucClaimingState == XBADDR_eNMEA_STATE5 ) &&
        ( XBMSG_fnGetBusState() != XB_eNETWORK_STATE_BUS_OFF ) )
    {
        return( TRUE );
    }
    else
    {
        return( FALSE );
    }
}

/******************************************************************************

FUNCTION NAME:
    xbaddr_fnGetRandonDelay

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

static uint16 xbaddr_fnGetRandonDelay( void )
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
    XBADDR_fnAddrState          - Get state of address claiming

PURPOSE:
    This function is used to get the state address claiming

INPUTS:
    None

OUTPUTS:
    Simplified state of address claiming

NOTES:

VERSION HISTORY:

Version 1.00  Date: 09/19/03  By: Dalem
    - Created

Version: 1.01     Date: Fri 11/29/2013  By: SESA149932
    - Re-added function since it will be used to restart the xanbus task at the
      application level

******************************************************************************/
XB_teADDR_STATE XBADDR_fnAddrState( void )
{
    switch( ucClaimingState )
    {
        case XBADDR_eNMEA_STATE0:
        default:
            // Node has not tried to get and address
            return( XB_eADDR_IDLE );

        case XBADDR_eNMEA_STATE1:
        case XBADDR_eNMEA_STATE2:
        case XBADDR_eNMEA_STATE3:
        case XBADDR_eNMEA_STATE4:
        case XBADDR_eNMEA_STATE9:
        case XBADDR_eNMEA_STATE9A:
            // Node is establishing address and is not allowed to send messages
            return( XB_eADDR_CLAIMING );

        case XBADDR_eNMEA_STATE5:
            // Node has address and is ready for business
            return( XB_eADDR_ONLINE );

        case XBADDR_eNMEA_STATE6:
        case XBADDR_eNMEA_STATE7:
        case XBADDR_eNMEA_STATE8:
        case XBADDR_eNMEA_STATE10:
            // Node has no address and is not allowed to send messages
            return( XB_eADDR_BLOCKED );
    }
}

/***************************************************************************//**

  Reset device claim timer count

  @param[in] ucAddr - pointer to the received data

*******************************************************************************/
void XBADDR_fnResetClaimTimers( uchar8 ucAddr )
{
    XBADDR_tzNODE *ptzNode;

    // Find node for given address
    ptzNode = xbaddr_fnFindAddr( ucAddr );

    // If node found
    if( ptzNode != NULL )
    {
        ptzNode->uiTicks = XBTIMERS_fnGetClaimTimer();
    }
}

/***************************************************************************//**

  Generate and send an isoreq for trying address value

  @param[in] ucAddr - network address to use.

*******************************************************************************/
static void xbaddr_fnIsoReqAddrClaim( uchar8 ucAddr )
{
    const PGN_tzPGN_INFO * ptzPgnInfo;
    PGNDOB *ptzDOB;
    TFXCAN_RETURNS TFX_teResult;


    // Look up the PGN for address claim
    ptzPgnInfo = XBPACK_fnGetPgnInfo( XB_PGN_ISO_REQ,
                                      ( sint16 )XB_eGRP_FUNC_VALUE_DO_NOT_CARE );

    // Allocate memory for a Data OBject
    TFX_teResult = PDOBConstructAlloc( &ptzDOB,
                                       XB_PGN_ISO_REQ,
                                       ( sint16 )XB_eGRP_FUNC_VALUE_DO_NOT_CARE,
                                       ptzPgnInfo );

    // If call succeeded
    if( TFX_teResult == TFXCR_OK )
    {

        // Set size of buffer
        ptzDOB->m_i16ByteCount = PGN_MAX_LEN_SINGLEFRAME ;

        // Give data buffer to DOB
        ptzDOB->m_pData = (uchar8 *)&isoReqAddr[0];

        // Remember buffer is NOT from heap
        ptzDOB->m_ucFlags &= ~DOB_DATA_FROM_HEAP;

        // Set port and destination in DOB
        PDOBSetPort( ptzDOB, XB_PORT_A );
        ptzDOB->m_DestAddr = ucAddr;

        // Use null address
        ptzDOB->m_SourceAddr = PGN_ADDR_NULL;

        // Queue DOB for sending
        TFX_teResult = DOBHLDAdd( ptzDOB );

        // Check result
        if( TFX_teResult == TFXCR_OK )
        {
            // Set state as ready to go out, success
            ptzDOB->m_eState = XDATA;
        }
        else
        {
            // Cannot queue dob, release DOB -- this will release the buffer
            PDOBDestruct( ptzDOB );

            // Save error
            XBINIT_fnSaveError( PGN_eESRC_TFX_QUEUE_DOB,
                                ( uint16 )TFX_teResult );
        }
    }
}

/***************************************************************************//**

  Receive DiagMsg1

  This message receives the DiagMsg1 message and uses it as a keep-alive
  message for maintaining its source address in the list of active nodes.
  If a message is received from an address not in the active node list,
  a new entry is made in that list and the application is informed of the
  new node's presence.

  @param[in] ptzRecv - The received message object, which includes the source address

*******************************************************************************/
XBDLL_API void XBADDR_fnRecvDiagMsg1( PGN_tzRECV_DATA *ptzRecv )
{
    XBADDR_tzNODE *ptzAddrNode;
    XB_tzPGN_ISO_ADDR_CLAIM tzLongName;
    uchar8 ucSrcAddr;

    // At this level we just want the address so that we can keep
    // it active in our node list
    ( void )assert( ptzRecv != NULL );
    ucSrcAddr = ptzRecv->ucSrcAddr;
    if (( ucSrcAddr == ucMyAddr )
        && ( ucClaimingState == XBADDR_eNMEA_STATE5 ))
    {
        // Hey! This is my address!  Demand the other node's address
        // claim and let the NAME priority battle begin!
        xbaddr_fnState2();
    }
    else if( ( ucSrcAddr != PGN_ADDR_NULL )
          && ( ucSrcAddr != PGN_ADDR_GLOBAL ) )
    {
        // Search node list for received address
        ptzAddrNode = xbaddr_fnFindAddr( ucSrcAddr );
        if( ptzAddrNode == NULL )
        {
            // This address is new to us - add, but with no NAME information
            if( ucTotalNodes >= ucMaxNodeNum  )
            {
                // We reach the max number of nodes we can record. Log the error
                XBINIT_fnSaveError( PGN_eESRC_CLAIMING, ( uint16 )PGN_eERR_NO_ROOM );
            }
            else
            {
                // Create a new node
                ptzAddrNode = XBMEM_fnMalloc( sizeof(XBADDR_tzNODE) );
                if ( ptzAddrNode != NULL )
                {
                    // Register the node address, start the node timer
                    ptzAddrNode->ucAddress = ucSrcAddr;
                    ptzAddrNode->uiTicks   = XBTIMERS_fnGetClaimTimer();
                    ptzAddrNode->tzNetName = *(XB_tzNET_NAME *)&tzNullNetName;
                    ptzAddrNode->ucDSA = (uchar8)( ( ( RVC_tzPGN_DIAG_MSG1 *) ptzRecv->pvStruct ) -> teProdId );

                    // add the node to the node linked list
                    ( void )xbaddr_fnListAdd( &ptzNodeHead, ptzAddrNode );

                    // increase the number of total nodes on the list
                    ucTotalNodes++;

                    // Report the new node to the application
                    if ( XBADDR_fnNetNAME2NAME( &ptzAddrNode->tzNetName,
                                                &tzLongName ) )
                    {
                        XBPLATFORM_fnNodeChange( XB_eNODE_CHANGE_NEW_NODE,
                                                 PGN_ADDR_NULL,
                                                 ucSrcAddr,
                                                 &tzLongName );
                    }

                    // Now that we have a record on this new node, we should get to
                    // know each other better for the sake of potential proprietary
                    // messaging.  Politely ask for this 'new' node's address claim
                    // (ie NAME) so that we can get its manufacturer code and device
                    // function.
                    XBMSG_fnISOReq( RVC_PGN_ISO_ADDR_CLAIM, ucSrcAddr );
                }
                else
                {
                    // This is not good, no node list space available, ignore node
                    // remember the error
                    XBINIT_fnSaveError( PGN_eESRC_CLAIMING,
                                        ( uint16 )PGN_eERR_NO_MEM );
                }
            }
        }
        else
        {
            // This a known address - restart the timer
            ptzAddrNode->uiTicks = XBTIMERS_fnGetClaimTimer();

            // If we still don't know its NAME then we will still want to get
            // its address claim
            if( xbaddr_fnCompareNetNames( &ptzAddrNode->tzNetName,
                                          (XB_tzNET_NAME *)&tzNullNetName )
                                          == XB_CMP_NAME_1_EQ_2 )
            {
                XBMSG_fnISOReq( RVC_PGN_ISO_ADDR_CLAIM, ucSrcAddr );
            }
        }
    }

    // Send the message to the application
    XBPLATFORM_fnRecvXbMsg( ptzRecv );
}


/***************************************************************************//**

  Get the preferred address to use for address claiming

  Try first to load the preferred address and if that fails use either the DSA
  associated to the device function or a dynamic address from the range
  specific for the device function.
  On exit sets the ucMyAddr static variable.

*******************************************************************************/
static void xbaddr_fnGetMyPreferredAddress( void )
{
    // Start claiming with the Preferred Address
    if( XBPLATFORM_fnLoad( XB_ePARAM_TYPE_ADDRESS,
                           XB_eCFG_TYPE_USER,
                           &ucMyAddr,
                           sizeof( ucMyAddr ) ) != ( tuiSTATUS )eSTATUS_OK )
    {
        // If reading the Preferred Address fails, use end of address range
        // allocated for the device function
        if( tzMyNAME.ucSelfCfgAddr == 1 )
        {
            // Based on device type ucMyAddr is set to the upper end of the addr range
            switch( tzMyNAME.teDeviceFunction )
            {
            case RVC_eFUNC_CODE_BATTERY_MONITOR:
                ucMyAddr = RVC_SENSORS_DYNAMIC_ADDR_END;
                break;
            case RVC_eFUNC_CODE_ENERGY_MANAGEMENT_SYSTEM:
            case RVC_eFUNC_CODE_BATTERY_MANAGEMENT_SYSTEM:
            case RVC_eFUNC_CODE_INVERTER_CHARGER:
            case RVC_eFUNC_CODE_INVERTER:
            case RVC_eFUNC_CODE_CHARGER:
                ucMyAddr = RVC_PWR_COMP_DYN_ADDR_END;
                break;
            case RVC_eFUNC_CODE_USER_INTERFACE:
            case RVC_eFUNC_CODE_SERVICE_TOOL:
            case RVC_eFUNC_CODE_DIAGNOSTIC_TERMINAL:
            case RVC_eFUNC_CODE_GATEWAY:
            case RVC_eFUNC_CODE_SYSTEM_CLOCK:
                ucMyAddr = RVC_CTRL_DISP_DYNAMIC_ADDR_END;
                break;
            default:
                ucMyAddr = RVC_COMP_DYN_ADDR_EXT_END;
            }
        }
        else
        {
            // For static adresses use the DSA
            switch( tzMyNAME.teDeviceFunction )
            {
            case RVC_eFUNC_CODE_INVERTER_CHARGER:
            case RVC_eFUNC_CODE_INVERTER:
                ucMyAddr = RVC_eDFLT_SRC_ADDR_INVERTER;
                break;
            case RVC_eFUNC_CODE_CHARGER:
                ucMyAddr = RVC_eDFLT_SRC_ADDR_CHARGE_CONTROLLER;
                break;
            case RVC_eFUNC_CODE_USER_INTERFACE:
            case RVC_eFUNC_CODE_DIAGNOSTIC_TERMINAL:
                ucMyAddr = RVC_eDFLT_SRC_ADDR_CONTROL_PANEL;
                break;
            case RVC_eFUNC_CODE_SERVICE_TOOL:
                ucMyAddr = RVC_eDFLT_SRC_ADDR_SERVICE_TOOL;
                break;
            case RVC_eFUNC_CODE_GATEWAY:
                ucMyAddr = RVC_eDFLT_SRC_ADDR_NETWORK_BRIDGE;
                break;
            case RVC_eFUNC_CODE_SYSTEM_CLOCK:
                ucMyAddr = RVC_eDFLT_SRC_ADDR_SYSTEM_CLOCK;
                break;
            case RVC_eFUNC_CODE_BATTERY_MANAGEMENT_SYSTEM:
            case RVC_eFUNC_CODE_ENERGY_MANAGEMENT_SYSTEM:
                ucMyAddr = RVC_eDFLT_SRC_ADDR_BATTERY_MONITOR2;
                break;
            case RVC_eFUNC_CODE_BATTERY_MONITOR:
                ucMyAddr = RVC_eDFLT_SRC_ADDR_BATTERY_MONITOR;
                break;
            default:
                // As a last resort use the start of the extended dynamic range.
                ucMyAddr = RVC_COMP_DYN_ADDR_EXT_START;
                break;
            }
        }
    }

    // Check if preferred address is not proper
    ucMyAddr  = xbaddr_fnCheckAndSetProperAddr( ucMyAddr );

    // Save this address as the previous address
    ucSaveAddr = ucMyAddr;
}


//*****************************************************************************
// End of file.
//! @}
//*****************************************************************************
