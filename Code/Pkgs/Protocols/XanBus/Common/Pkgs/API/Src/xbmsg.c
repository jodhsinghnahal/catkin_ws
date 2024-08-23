/******************************************************************************

  (c) 2003 - 2022 Xantrex Canada ULC. All rights reserved.
  All trademarks are owned or licensed by Xantrex Canada ULC,
  its subsidiaries or affiliated companies.

****************************************************************************//**

  @addtogroup xbaddr XBMSG
  @{

    @brief Management of CAN Messaging

****************************************************************************//**

  @page xbmsg_design CAN Messaging Management Design

    This file provides functions to support PGN message handling

  @par API Functions
    XBMSG_fnInit                - Initialize XanBus Message data
    XBMSG_fnClearDynamic        - Clear dynamic Rx control list
    XBMSG_fnControlDynamic      - Set dynamic Rx control
    XBMSG_fnRawReceive          - Receive a PG message from Teleflex library
    XBMSG_fnDestroyRecvStruct   - Release memory of receive data structure
    XBMSG_fnOnLine              - XanBus stack ready for application messages
    XBMSG_fnGetBusState         - Get state of the node's connection to CAN bus
    XBMSG_fnSendEx              - Send PG data message on the XanBus
    XBMSG_fnSend                - Send PG data message on the XanBus
    XBMSG_fnPeriodicSend        - Handle Periodic Xmit request
    XBMSG_fnSendISOAck          - Send an acknowledgment message
    XBMSG_fnRecvIsoReq          - Handle Received ISO Request message
    XBMSG_fnIsoReq              - Send an ISO Request
    XBMSG_fnSendPgnList         - Send NMEA Rx and Tx PGN lists
    XBMSG_fnRecvConnMgmt        - Handle Received ISO Connection Management Msg
    XBMSG_fnSendCommStats       - Send communications statistics PGN
    XBMSG_fnClearCommStats      - Clear communications statistics

  @par Local Functions
        xbmsg_fnGetDynamic          - Get the dynamic Rx control bit
        xbmsg_fnCheckPgnRx          - Check receivability of PGN

*******************************************************************************/

/*==============================================================================
                              Includes
==============================================================================*/

// XanBus include files
#include "xbldefs.h"    // Local Definitions
#include "xbpgfns.h"    // XanBus-specific PGN handler functions
#include "xbmem.h"      // memory allocation
#include "pgnudefs.h"   // PGN struct and definition
#include "config.h"

// RV-C Protocol Definitions
#include "rvcpdefs.h"

#include "CRC16.h"      // For calculating CRC on proprietary messages

// System include files
#include <string.h>     // For memset
#include "xassert.h"    // Debugging


/*=============================================================================
                            Definitions
=============================================================================*/

// CRC-CCITT init value
#define RVC_PROP_MSG_CRC_INIT_VAL   ( 0xFFFF )

// Number of bytes of the ISO Network name used in CRC calculation
#define RVC_NET_NAME_CRC_LEN    ( 5 )

// Number range for the start of proprietary message IDs belonging to ZeroRPM.
// Anything less is a Xantrex message with a two-byte CRC at the end.
#define XBMSG_ZRPM_MSG_ID_START  ( 0x60 )

/*==============================================================================
                          External/Public Variables
==============================================================================*/

// Global for collecting XanBus Layer Communications statistics
XB_tzXB_COMM_STATS XBMSG_tzXbCommStats;

/*==============================================================================
                              Forward declarations
==============================================================================*/

static tucBOOL xbmsg_fnCheckReceive( PGNDOB *ptzDob );
static tucBOOL xbmsg_fnGetDynamic( uint16 uiIndex );
static tucBOOL xbmsg_fnGetDevice( uchar8 ucAddr,
                                  XB_teFUNC_CODE *pteDevFunc );
static void xbmsg_fnCalcPropMsgCsum( uchar8 *pcBuf,
                                     uint16 uiBufLen,
                                     uchar8 ucDestAddr );
static tucBOOL xbmsg_fnVerifyPropMsg( PGNDOB *ptzDob );
static tucBOOL xbmsg_fnVerifyPropMsgCsum( PGNDOB *ptzDob );
static uchar8 xbmsg_fnGetDestAddr( PGNDOB *ptzDob );

/*==============================================================================
                           Function Definitions
==============================================================================*/
tucBOOL XBMSG_fnSendPriority ( uint32 ulPgn,
                               sint16 siGrpFunc,
                               void *pvSrcStruct,
                               uint16 uiSrcSize,
                               uchar8 ucDest );

/******************************************************************************

FUNCTION NAME:
    XBMSG_fnInit                - Initialize XanBus Message data

PURPOSE:
    This function is used to initialize XanBus Message data structures

INPUTS:
    None

OUTPUTS:
    TRUE if all is well
    FALSE if error occurred

NOTES:

VERSION HISTORY:

Version 1.02  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.03  Date: 07/03/03  By: Dalem
    - Removed XB_aucDynamicRxControl array
Version 1.04  Date: 08/12/03  By: Dalem
    - Timers interface changed

******************************************************************************/

tucBOOL XBMSG_fnInit( void )
{

   // clear all the data in CommSts struct
   ( void )memset( &XBMSG_tzXbCommStats, 0, sizeof( XB_tzXB_COMM_STATS ) );
    XBMSG_tzXbCommStats.uiHeapFreeMin = PGN_UI_NO_DATA;

    // Set up the transmit timers
    if( XBTIMERS_fnLoadTxTimers() == FALSE )
    {
        // Save error
        XBINIT_fnSaveError( PGN_eESRC_TIMER,
                            ( uint16 )PGN_eERR_NOT_SET );

        // Failure
        return( FALSE );
    }

    // Set up the receive timers
    if( XBTIMERS_fnLoadRxTimers() == FALSE )
    {
        // Save error
        XBINIT_fnSaveError( PGN_eESRC_TIMER,
                            ( uint16 )PGN_eERR_NOT_SET );

        // Failure
        return( FALSE );
    }

    // Success
    return( TRUE );
}

/******************************************************************************

FUNCTION NAME:
    XBMSG_fnClearDynamic        - Clear dynamic Rx control list

PURPOSE:
    This function clears all entries in the dynamic Rx control list.

INPUTS:
    nothing

OUTPUTS:
    nothing

NOTES:

VERSION HISTORY:

Version 1.01  Date: 06/24/03  By: Dalem
    - Major change to XB API

******************************************************************************/

void XBMSG_fnClearDynamic( void )
{
    // Clear dynamic control bytes
    (void) memset( XB_aucDynamicRx,
                   0,
                   XBPGNTBL_fnSizeOfDynRxLists() );
}

/******************************************************************************

FUNCTION NAME:
    XBMSG_fnCheckCfgSrc        - Check source of configuration message

PURPOSE:
    This function validates the source of a Xanbus internal configuration
    message

INPUTS:
    'teCfgType' is the configuration type from the message
    'ucSrcAddr' is the source address of the message
    'pteAckType' is a pointer to the recommended acknowledgement code

OUTPUTS:
    TRUE if the configuration message is permitted from the source node
    FALSE if we cannot accept the configuration

NOTES:
    This function is only used by Xanbus modules.

VERSION HISTORY:

Version 1.00  Date: APR-18-2005 By: Hollyz
    - Replaced XBADDR_fnCheckCfgSrc() for Xanbus internal use.

******************************************************************************/
tucBOOL XBMSG_fnCheckCfgSrc( XB_teCFG_TYPE teCfgType,
                             uchar8        ucSrcAddr,
                             XB_teACK_STS *pteAckType )
{
    XB_teFUNC_CODE teDevFunc;

    // Verify the pointer
    (void)assert( pteAckType != NULL );

    // default acknowledge type
    *pteAckType = XB_eACK_STS_DNY;

    // Find the Xantrex Device function for given address
    if( FALSE != xbmsg_fnGetDevice( ucSrcAddr,
                                    &teDevFunc ) )
    {
        // the node was found;
        switch( teCfgType )
        {
            // User configuration
            case XB_eCFG_TYPE_USER:
                // If source node is Diag or UI
                if( ( teDevFunc ==
                          XB_eFUNC_CODE_USER_INTERFACE ) ||
                    ( teDevFunc ==
                          XB_eFUNC_CODE_DIAGNOSTIC_TERMINAL ) )
                {
                    *pteAckType = XB_eACK_STS_ACK;
                     // Source is allowed to send config
                    return( TRUE );
                }
                break;

            case XB_eCFG_TYPE_OEM:
                // If node is Diag or UI
                if( teDevFunc ==
                        XB_eFUNC_CODE_DIAGNOSTIC_TERMINAL )
                {
                    *pteAckType = XB_eACK_STS_ACK;
                    // Source is allowed to send OEM config
                    return( TRUE );
                }
                break;

            case XB_eCFG_TYPE_FACTORY:
                break;

            case XB_eCFG_TYPE_RESERVED: // fall through
            default:
                // Cannot accept configuration
                *pteAckType = XB_eACK_STS_NAK;
                break;
        }
    }

    // Cannot accept configuration
    return( FALSE );
}

/******************************************************************************

FUNCTION NAME:
    XBMSG_fnControlDynamic      - Set dynamic Rx control

PURPOSE:
    This function sets or clears a dynamic Rx control for the given PGN.

INPUTS:
    'ulPgn' is PGN
    'tucOn' is TRUE to set dynamic RX control to On
    'tucOn' is FALSE to set dynamic RX control to Off

OUTPUTS:
    nothing

NOTES:

VERSION HISTORY:

Version 1.01  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.02  Date: 07/03/03  By: Dalem
    - Change to using XBPACK_fnGetPgnInfo

******************************************************************************/

void XBMSG_fnControlDynamic( uint32 ulPgn,
                             tucBOOL tucOn )
{
    const PGN_tzPGN_INFO *ptzPgnInfo;
    uint16 uiIndex;
    uchar8 ucByte;

    // Look up Info for given PGN, assume no group function code
    ptzPgnInfo = XBPACK_fnGetPgnInfo( ulPgn,
                                      ( sint16 )XB_eGRP_FUNC_VALUE_DO_NOT_CARE );

    // If Pgn Info found
    if( ptzPgnInfo != NULL )
    {
        // Do pointer math to get index to PGN support entry
        // ptzPgnInfo always starts at XB_katzALL_PGN_LIST and goes up
        // so uiIndex shall never be less than 0
        uiIndex = ( uint16 )( ptzPgnInfo - XB_katzALL_PGN_LIST );

        // Get bit position in byte of interest
        ucByte = 0x01 << ( uiIndex % XT_BITS_PER_BYTE );

        // If turning the control on
        if( tucOn != FALSE )
        {
            // Or bit into proper control byte position
            XB_aucDynamicRx[ uiIndex / XT_BITS_PER_BYTE ] |= ucByte;
        }
        else
        {
            // And not bit into proper control byte position
            XB_aucDynamicRx[ uiIndex / XT_BITS_PER_BYTE ] &= ~ucByte;
        }
    }
}

/******************************************************************************

FUNCTION NAME:
    xbmsg_fnGetDynamic          - Get the dynamic Rx control bit

PURPOSE:
    This function returns the dynamic Rx control bit at the given PGN index

INPUTS:
    'uinIndex' is predetermined PGN index for control of interest

OUTPUTS:
    value of dynamic control bit

NOTES:

VERSION HISTORY:

Version 1.01  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.02  Date: 06/25/03  By: Dalem
    - Made function internal

******************************************************************************/

static tucBOOL xbmsg_fnGetDynamic( uint16 uiIndex )
{
    // Get dynamic support data
    return( ( XB_aucDynamicRx[ uiIndex / XT_BITS_PER_BYTE ] >>
            ( uiIndex % XT_BITS_PER_BYTE ) ) & 0x01 );
}

/******************************************************************************

FUNCTION NAME:
    xbmsg_fnGetDevice          - Get device function for node of given address

PURPOSE:
    This function is used to extract the device function type
    from the node name for the node using the address that is given.

INPUTS:
    'ucAddr' is address to look up node name to get device function type
    'pDevFunc' points to where to save the device function

OUTPUTS:
    TRUE if the device address was found
    FALSE if the device address was not found

NOTES:
   This function is used internally by Xanbus modules.

VERSION HISTORY:

Version 1.00  Date: 04/26/05  By: Hollyz
    - Changed XBADDR_fnGetDevice to a local function xbmsg_fnGetDevice for
      Xanbus internal use.

Version 1.01  Date: 03/09/17  By: Doru Botez
    - Re-organized to meet RV-C spec.Use both '2047' and '119' as
      manufacturer's code.

******************************************************************************/
static tucBOOL xbmsg_fnGetDevice( uchar8 ucAddr,
                                  XB_teFUNC_CODE *pteDevFunc )

{
    XB_tzPGN_ISO_ADDR_CLAIM tzNAME;

    assert( pteDevFunc != NULL );

    // Find the node name for given address
    if( FALSE != XBADDR_fnGetNAME( ucAddr,
                                   &tzNAME ) )
    {
        // the node was found;
        // Check the name field
        // look for a Xantrex device
        if( ( tzNAME.uiManufacturerCode == RVC_MANUFACTURER_CODE_XANTREX ) ||
            ( tzNAME.uiManufacturerCode == XB_MFG_XANTREX ) )
        {
            if( (tzNAME.teIndustryGroup == XB_INDUSTRY_GROUP ) &&
                ( tzNAME.teDeviceClass == XB_eDEVICE_CLASS_POWER_MANAGEMENT ) )
            {
                // get the device Function
                *pteDevFunc = tzNAME.teDeviceFunction;

                // Indicate that a device was found
                return( TRUE );
            }
        }
    }

    // Failure
    return( FALSE );
}

/******************************************************************************

FUNCTION NAME:
    xbmsg_fnCheckPgnRx          - Check receivability of PGN

PURPOSE:
    This function is used to check if a PGN can be received by this node
    and will send NAK or DNY with reason code if it cannot be received.

INPUTS:
    'ptzDob' is a pointer to the DOB containing the received message

OUTPUTS:
    TRUE if message passes checks
    FALSE if message should be discarded

NOTES:

VERSION HISTORY:

Version 1.00  Date: 09/04/03  By: Dalem
    - Created
Version 1.01  Date: 09/05/03  By: Dalem
    - Check for descriptor not NULL
Version 1.02  Date: 09/15/03  By: Dalem
    - Do not send NAK if rx ignore is set
Version 1.03  Date: 12/11/03  By: Dalem
    - If ignoring dynamic PGNs, do not send NACK
Version 1.04  Date: 04/01/04  By: Peterd
    - Added some local pointers for performance (I hope)
    - Added check of new message type (XanBusDB.mdb 0.4.28) and ignore
    configuration messages with non-zero message count
    - Added length check of messages that are being ignored dynamically
Version: 1.05  Date: 2009-03-13  By: TomL
    - ptzDob sometimes passed in with null info field.  Check first before
    using its fields.
Version: 1.06  Date: Mon 12/16/2013  By: Scott Volk
    - Removed NAKs cause by the RVC protocol
Version: 1.07  Date: 2019-01-14  By: Tom Lightfoot
    - Added checking for proprietary RV-C/J1939 messages.  If checksum doesn't
      match then received message is ignored

******************************************************************************/
static tucBOOL xbmsg_fnCheckReceive( PGNDOB *ptzDob )
{
    XB_teACK_STS        teAckType;
    XB_teGRP_FUNC_VALUE teReason = XB_eGRP_FUNC_VALUE_NOT_AVAILABLE;
    uint16              uiPgnIndex;
    uchar8              ucMsgCount = 0xFF;
    const PGN_tzPGN_DESC * XT_FAR pktzPgnDesc;
    uint16 uiExtraBlocks;
    uint16 uiSrcLen;
    const PGN_tzPGN_INFO * pktzPgnInfo;

    // Use positive acknowledgement to indicate "send no reply"
    teAckType = XB_eACK_STS_ACK;

    // We'll be examining the PGN info a lot
    pktzPgnInfo = ptzDob->m_pPgnInfo;

    if ( pktzPgnInfo != NULL )
    {
        // Convenience ponter to the PGN descriptor
        pktzPgnDesc = pktzPgnInfo->pktzDesc;

        // If this is a configuration message
        if ( ( pktzPgnInfo->uiPgTypeFlags & PGN_PG_TYPE_FLAG_MASK ) == PGN_PG_TYPE_CFG )
        {
            // It is assumed to have a message count field
            // Extract the message count for use below
            ucMsgCount = XBPACK_fnUnpackMsgCount( ptzDob->m_pData );
        }
    }

    // If PGN unknown to us
    if( pktzPgnInfo == NULL )
    {
        return (FALSE);
    }

    // If PGN is not receivable by us
    else if( ( pktzPgnInfo->uiFlags & PGN_FLAG_RX ) == 0 )
    {
        if ( ( ( pktzPgnInfo->uiPgTypeFlags & PGN_PG_TYPE_FLAG_MASK ) == PGN_PG_TYPE_CFG ) &&
         ( ucMsgCount != 0 ) )
        {
            // Ignore this PGN
            // Drop the PGN but do not send a response
        }
        else
        {
            // If flag is set specifying to 'deny not receivable'
            if( ( pktzPgnInfo->uiFlags & PGN_FLAG_RX_DNY ) != 0 )
            {
                // Send a Deny
                teReason = XB_eGRP_FUNC_VALUE_PROTOCOL;
                teAckType = XB_eACK_STS_DNY;
            }
            // If flag is set specifying to 'nack not receivable'
            else if( ( pktzPgnInfo->uiFlags & PGN_FLAG_RX_NAK ) != 0 )
            {
                // Send a NAK
                teReason = XB_eGRP_FUNC_VALUE_UNSUPPORTED;
                teAckType = XB_eACK_STS_NAK;
            }
            else
            {
                // Ignore this PGN
                // Drop the PGN but do not send a response
            }
        }
    }
    else
    {
        // Do pointer math to get index to PGN support entry
        uiPgnIndex = ( uint16 )( pktzPgnInfo - XB_katzALL_PGN_LIST );

        // If receive support is dynamic and dynamic control not set
        // and we have a receive function
        if( ( ( pktzPgnInfo->uiFlags & PGN_FLAG_DYN ) != 0 )
               && ( xbmsg_fnGetDynamic( uiPgnIndex ) == FALSE ) &&
               ( pktzPgnDesc->pfnRecv != NULL ) )
        {
            // Ignore this PGN
            // Drop the PGN but do not send a response

            // If we made it this far, we could be ignoring a PGN with invalid DLC
            // so check that too

            uiSrcLen = ( uint16 )ptzDob->m_i16ByteCount;

            if ( pktzPgnDesc != NULL )
            {
                // If message is shorter than expected
                if( uiSrcLen < pktzPgnDesc->uiFixedLen )
                {
                    teReason = XB_eGRP_FUNC_VALUE_DLC_INVALID;
                    teAckType = XB_eACK_STS_NAK;
                }
                // If message descriptor has a variable length part
                else if( pktzPgnDesc->uiVarLen != 0 )
                {
                    // Get the number of extra structures to retrieve
                    uiExtraBlocks = ( uiSrcLen - pktzPgnDesc->uiFixedLen )
                        / pktzPgnDesc->uiVarLen;

                    // If the variable length portion does not have complete blocks
                    if( uiExtraBlocks * pktzPgnDesc->uiVarLen +
                        pktzPgnDesc->uiFixedLen != uiSrcLen )
                    {
                        teReason = XB_eGRP_FUNC_VALUE_DLC_INVALID;
                        teAckType = XB_eACK_STS_NAK;
                    }
                }
                // If message not of equal to fixed length
                else if( uiSrcLen != pktzPgnDesc->uiFixedLen )
                {
                    teReason = XB_eGRP_FUNC_VALUE_DLC_INVALID;
                    teAckType = XB_eACK_STS_NAK;
                }
            }
        }

        // If there is no descriptor
        else if( pktzPgnDesc == NULL )
        {
            return( FALSE );
        }

        // Is this somebody else's proprietary message?  If so, then we want to ignore it.
        else if (( ptzDob->m_PGN == PGN_PGN_PROP_MSG )
                 && ( pktzPgnDesc->pfnRecv != NULL ))
        {
            // Make sure that this is a valid proprietary message that is recognized in our database
            // If not then we ignore it
            if ( xbmsg_fnVerifyPropMsg( ptzDob ) == FALSE )
            {
                // Message wrong size or checksum wrong.  Ignore
                return FALSE;
            }
            else
            {
                // This is our message.  We want to receive it
                return TRUE;
            }
        }

        // If no receive function
        else if( pktzPgnDesc->pfnRecv == NULL )
        {
            // If receive ignore flag set
            if( ( pktzPgnInfo->uiFlags & PGN_FLAG_RX_IGN ) != 0 )
            {
                // Ignore this PGN
                // Drop the PGN but do not send a response
            }

            else if ( ( ( pktzPgnInfo->uiPgTypeFlags & PGN_PG_TYPE_FLAG_MASK ) == PGN_PG_TYPE_CFG ) &&
                ( ucMsgCount != 0 ) )
            {
                // Ignore this PGN
                // Drop the PGN but do not send a response
            }
            else
            {
                // Send a NAK
                teReason = XB_eGRP_FUNC_VALUE_NOT_AVAILABLE;
                teAckType = XB_eACK_STS_NAK;
            }
        }
        else
        {
            // Passes all tests
            return( TRUE );
        }
    }

    // If teAckType has been set
    if( teAckType != XB_eACK_STS_ACK )
    {
        // Send reply
        XBMSG_fnSendISOAck( ( uint32 )ptzDob->m_PGN,
                            teReason,
                            teAckType,
                            ptzDob->m_SourceAddr );
    }

    // Failed to pass tests
    return( FALSE );
}

/******************************************************************************

FUNCTION NAME:
    XBMSG_fnRawReceive          - Receive a PG message from Teleflex library

PURPOSE:
    Receive a PG message from the Teleflex data stack. This is the function
    that is installed as the Teleflex receiver callback.

INPUTS:
    'ptzDob' is a pointer to the DOB containing the received message

OUTPUTS:
    Nothing

NOTES:
    The function fnUnPackPGN() allocates memory for the message as a
    data structure.

    If PGN is not in support list, a NACK is sent
    If PGN not marked for receive, message is ignored
    If PGN is marked as dynamic but is not active, message is ignored
    If there is no receive function, message is ignored
    otherwise message is unpacked and receive function called

VERSION HISTORY:

Version 1.06  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.07  Date: 06/25/03  By: Dalem
    - Dynamic activity controlled by flag
Version 1.08  Date: 07/03/03  By: Dalem
    - Change to using XBPACK_fnGetPgnInfo
Version 1.09  Date: 07/04/03  By: Dalem
    - Test group function first
    - Send Nack if PGN unknown
    - Send Nack if PDU1 and cannot be received
    - Send Nack if PGN cannot be unpacked
Version 1.10  Date: 08/04/03  By: Dalem
    - Added CheckReceive function and restructured
Version 1.10  Date: 09/11/03  By: Dalem
    - Perform length check on IsoAddrClaim PGN
Version 1.11  Date: 10/31/03  By Peterd
    - Moved call to XBMSG_fnDestroyRecvStruct inside else part of check
    for pStruct == NULL
Version 1.12  Date: 12/16/03  By Dalem
    - Added ucSrcFlags to XB_tzRECV_DATA
Version 1.13  Date: 2019-03-28  By: TomL
    - PGN desc not needed to get raw group function.  Checking would
      cause a bug when a device did not receive all group functions.

******************************************************************************/

void XBMSG_fnRawReceive( PGNDOB *ptzDob )
{
    PGN_tzRECV_DATA *ptzRecv;
    sint16 siGrpFunc;
    XB_teGRP_FUNC_VALUE teErrCode = XB_eGRP_FUNC_VALUE_DO_NOT_CARE;

    // Verify parameter
    (void)assert( ptzDob != NULL );

    // If PGN known to us
    if ( ptzDob->m_pPgnInfo != NULL )
    {
        // Get the group function from the receive message
        siGrpFunc = XBPACK_fnRawGrpFunc( ptzDob );

        // If there actually is a group function then it is necessary to
        // re-look up the PGN descriptor for the specific code
        if( siGrpFunc != ( sint16 )XB_eGRP_FUNC_VALUE_DO_NOT_CARE )
        {
            // Look up PGN again for the changed group function
            ptzDob->m_pPgnInfo = ( const PGN_tzPGN_INFO * )XBPACK_fnGetPgnInfo( ( uint32 )ptzDob->m_PGN,
                                                                                  siGrpFunc );
        }
    }

    // Check this message for receivability
    if( xbmsg_fnCheckReceive( ptzDob ) != FALSE )
    {
        // Allocate memory for receive message structure
        ptzRecv = XBMEM_fnMalloc( sizeof( *ptzRecv ) );
        if( ptzRecv == NULL )
        {
            // Handle error
            XBPLATFORM_fnCheckErrors();
            DOBHLDDelete( ptzDob );
            PDOBDestruct( ptzDob );

            // Done with this message
            return;
        }

        // No one else is using the new message structure yet
        ptzRecv->ucUseCount = 0;

        // Save PGN Info pointer in receive structure
        ptzRecv->pktzPgnInfo = ptzDob->m_pPgnInfo;

        // If this is an address claim PGN
        if( ptzDob->m_pPgnInfo->ulPgn == XB_PGN_ISO_ADDR_CLAIM )
        {
            // If message is the correct size
            if( ( uint16 )ptzDob->m_i16ByteCount ==
                ptzDob->m_pPgnInfo->pktzDesc->uiFixedLen )
            {
                // Use the raw data from the DOB
                ptzRecv->pvStruct = ptzDob->m_pData;
            }
            else
            {
                // Treat the PGN as if unpacking failed
                ptzRecv->pvStruct = NULL;
            }
        }
        else
        {
            // Unpack the message into given structure
            ptzRecv->pvStruct = XBPACK_fnUnPackPgn( ptzDob->m_pPgnInfo->pktzDesc,
                                                   ptzDob->m_pData,
                                                   ( uint16 )ptzDob->m_i16ByteCount,
                                                   &teErrCode );
        }

        // If unpacking the structure did not succeed
        if( ptzRecv->pvStruct == NULL )
        {
            // Send a NACK
            XBMSG_fnSendISOAck( ( uint32 )ptzDob->m_PGN,
                                teErrCode,
                                XB_eACK_STS_NAK,
                                ptzDob->m_SourceAddr );

            // Get rid of buffer that won't be used now
            XBMEM_fnFree( ptzRecv );
        }
        else
        {
            // Assign other fields to receive work block
            ptzRecv->ulPgn = ptzDob->m_pPgnInfo->ulPgn;
            ptzRecv->siGrpFunc = ptzDob->m_pPgnInfo->siGrpFunc;
            ptzRecv->ucSrcAddr = ptzDob->m_SourceAddr;

            // If PDU1 then Packet Specific part is destination address
            if( ( ptzDob->m_pPgnInfo->uiFlags & PGN_FLAG_PDU1 ) != 0 )
            {
                // PDU 1 packet has a destination address
                ptzRecv->ucDestAddr = ptzDob->m_Frame.m_CanData.m_u8PS;
            }
            else
            {
                // PDU 2 packet does not have a destination address
                ptzRecv->ucDestAddr = PGN_ADDR_GLOBAL;
            }

            // Node is alive -> reset the claim/heartbeat timer
            XBADDR_fnResetClaimTimers( ptzRecv->ucSrcAddr );

            // Call the receive function for this PGN
            ptzDob->m_pPgnInfo->pktzDesc->pfnRecv( ptzRecv );

            // Destroy the receive structure if it is not still in use
            if( ptzRecv->ucUseCount == 0 )
            {
                XBMSG_fnDestroyRecvStruct( ptzRecv );
            }
        }
    }

    // Remove DOB from holder and destroy it
    DOBHLDDelete( ptzDob );
    PDOBDestruct( ptzDob );
}


/******************************************************************************

FUNCTION NAME:
    XBMSG_fnDestroyRecvStruct   - Release memory of receive data structure

PURPOSE:
    To Destroy a receive structure that was created by XBMSG_fnRawReceive.

INPUTS:
    'ptzRecv' points to the receive object to be destroyed

OUTPUTS:
    Nothing

NOTES:
    This function is called by XBMSG_fnRawReceive or any other process that
    decides that it needs to extend the life of the object by incrementing
    the use counter.

VERSION HISTORY:

Version 1.02  Date: 06/24/03  By: Dalem
    - Major change to XB API

******************************************************************************/

void XBMSG_fnDestroyRecvStruct( PGN_tzRECV_DATA *ptzRecv )
{
    // Verify parameter
    (void)assert( ptzRecv != NULL );

    // If this is not an address claim PGN then destroy the child structure
    if( ptzRecv->ulPgn != XB_PGN_ISO_ADDR_CLAIM )
    {
        // Release the received message structure memory
        if( XBPACK_fnDestroyPgnStruct( ptzRecv->pktzPgnInfo->pktzDesc,
                                        ptzRecv->pvStruct ) == FALSE )
        {
            // Handle error
            XBPLATFORM_fnCheckErrors();
        }
    }

    // Always free the parent structure
    XBMEM_fnFree( ptzRecv );
}

/******************************************************************************

FUNCTION NAME:
    XBMSG_fnGetBusState         - Get state of the node's connection to CAN bus

PURPOSE:
    This function returns the state of node's connection to the CAN bus

INPUTS:
    None

OUTPUTS:
    Return one of XB_BUS_ERROR_ACTIVE, XB_BUS_ERROR_PASSIVE,
                 XB_BUS_BUS_OFF, XB_BUS_UNKNOWN.

NOTES:

VERSION HISTORY:

Version 1.01  Date: 06/24/03  By: Dalem
    - Major change to XB API

Version 1.02  Date: 02/04/04  By: Peterd
    - Changed return type to match change in database v. 0.4.20

Version 1.03  Date: 09/30/04  By: Dalem
    - Added driver busy error code

******************************************************************************/
XB_teNETWORK_STATE XBMSG_fnGetBusState( void )
{
    // Get controller state for port A and set bus state code
    switch( TFXDRV_i16ControllerState( XB_PORT_A ) )
    {
        case TFXCR_OK:
        {
            return( XB_eNETWORK_STATE_ERROR_ACTIVE );
        }

        case TFXCR_BUS_WARNING:
        {
            return( XB_eNETWORK_STATE_ERROR_PASSIVE );
        }

        case TFXCR_BUS_OFF:
        {
            return( XB_eNETWORK_STATE_BUS_OFF );
        }

        case TFXCR_IDLE:
        case TFXCR_COUNT:
        case TFXCR_TABLE_FULL:
        case TFXCR_HEAP_ALLOC_FAIL:
        case TFXCR_NOT_FOUND:
        case TFXCR_NEW_DATA:
        case TFXCR_NO_DATA:
        case TFXCR_INVALID_PORT:
        case TFXCR_MSG_NOT_HANDLED:
        case TFXCR_LIB_BUSY:
        case TFXCR_DRV_BUSY:
        case TFXCR_DRV_RX_BUFFER_ALLOCATION_FAILURE:
        case TFXCR_DRV_TX_BUFFER_ALLOCATION_FAILURE:
        case TFXCR_MSG_HANDLE_NOT_AVAILABLE:
        case TFXCR_INVALID_MSG_HANDLE:
        case TFXCR_PORT_INIT_ERR:
        default:
        {
            return( XB_eNETWORK_STATE_UNKNOWN );
        }
    }
}

/******************************************************************************

FUNCTION NAME:
    XBMSG_fnIsFromDiag

PURPOSE:
    This function is used to determine if the source of a message is
    a Xantrex Diagnostics node

INPUTS:
    'ucSrcAddr' is source address of a message

OUTPUTS:
    TRUE if source of message is DIAG
    FALSE otherwise

NOTES:
    This function is only used internally by Xanbus modules.

VERSION HISTORY:

Version 1.00  Date: 04/19/05  By: Hollyz
    - Replaced XBADDR_fnFromDiag to perform security check for Xanbus internal
      configuration message.

******************************************************************************/
tucBOOL XBMSG_fnIsFromDiag( uchar8 ucSrcAddr )
{
    XB_teFUNC_CODE teDevFunc;

    // Get Xantrx device function for source address
    if( FALSE != xbmsg_fnGetDevice( ucSrcAddr,
                                    &teDevFunc ) )
    {
        // If node is Diag
        if( teDevFunc == XB_eFUNC_CODE_DIAGNOSTIC_TERMINAL )
        {
            // Source is a Diagnostics node
            return( TRUE );
        }
    }

    // Source is not a Diagnostics node
    return( FALSE );
}
/******************************************************************************

FUNCTION NAME:
    XBMSG_fnSendExDob              - Send PG data message on the XanBus

PURPOSE:
    This function is used to convert a PG data structure into a PG message
    and add it to the list of messages to send.

INPUTS:
    'ulPgn' is PG Number of PG to send
    'siGrpFunc' is Group function qualifier value if PG uses group functions
    'pvSrcStruct' is a pointer to data structure holding PG data to be sent
    'uiSrcSize' is size of the data structure
    'ucDest' is destination address for messages that require a destination

OUTPUTS:
    Pointer to the message successfully converted and queued for transmission,
    NULL and XB ERROR code should be set on failure.

NOTES:

VERSION HISTORY:

Version 1.02  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.03  Date: 09/03/03  By: Dalem
    - Fixed memory leak, release packed PGN is DOB not available
Version 1.04  Date: 09/19/03  By: Dalem
    - Use function fnAddrState.
Version 1.05  Date: 04/15/04  By: Peterd
    - Added code to simulate reception of globally addressed PGN.
Version 1.06  Date: 04/16/04  By: Peterd
    - Now only simulate reception of commands.
Version 1.07  Date: 04/16/04  By: Peterd
    - Now calling RawRecive with ptzRxDob instead of ptzDob.
Version 1.08  Date: 07/05/04  By: Peterd
    - Removed sizeof in memcpy just before fnRawReceive call to process
    commands sent by this node to all nodes
Version 1.09  Date: 02/11/04  By: Peterd
    - Added call to ChkAsleep to prevent sending when we're asleep
    - PRN 2921
Version 1.10  Date: 02/09/09  By: HollyZ
    - Disable buffering the messages if the CAN transmittter is blocked
Version 2.00  Date: 2019-03-15  By: TomL
    - Returning pDob instead of a BOOL so that a callback can be set
******************************************************************************/

PGNDOB *XBMSG_fnSendExPdob( uint32 ulPgn,
                            sint16 siGrpFunc,
                            void *pvSrcStruct,
                            uint16 uiSrcSize,
                            uchar8 ucDest,
                            uchar8 ucBuf )
{
    PGN_tzPGN_INFO const * ptzPgnInfo;
    uchar8 *pucBuffer;
    uint16 uiBufSize;
    PGNDOB *ptzDob;
    PGNDOB *ptzRxDob;
    TFXCAN_RETURNS TFX_teResult;

    // Verify parameter
    (void)assert( pvSrcStruct != NULL );

    // If we are sleeping
    if( XBINIT_fnChkAsleep() == TRUE )
    {
        // Ignore
        return NULL;
    }

    // If node has an address
    // if the CAN transmitter is sending frames
    if( ( XBADDR_fnOnLine() == TRUE )  &&
        ( TFXDRV_fnIsBlocked(  XB_PORT_A  ) == FALSE  ) )
    {
        // Look up the descriptor for this message
        ptzPgnInfo = XBPACK_fnGetPgnInfo( ulPgn,
                                          siGrpFunc );
        if( ptzPgnInfo == NULL )
        {
            XBINIT_fnSaveError( PGN_eESRC_TFX_QUEUE_DOB,
                                ( uint16 )PGN_eERR_NO_PGDESC );
            return NULL;
        }

        // Place field data into message
        pucBuffer = XBPACK_fnPackPgn( ptzPgnInfo->pktzDesc,
                                      pvSrcStruct, uiSrcSize,
                                      &uiBufSize );

        // If message was built successfully
        if( uiBufSize != PGN_UI_NO_DATA )
        {
            // Allocate memory for a Data OBject
            TFX_teResult = PDOBConstructAlloc( &ptzDob,
                                               ( sint32 )ulPgn,
                                               siGrpFunc,
                                               ptzPgnInfo );

            // If call succeeded
            if( TFX_teResult == TFXCR_OK )
            {
                // Set size of buffer
                ptzDob->m_i16ByteCount = ( sint16 )uiBufSize;

                if( uiBufSize != 0 )
                {
                    // Give data buffer to DOB
                    ptzDob->m_pData = pucBuffer;

                    // Remember buffer is from heap
                    ptzDob->m_ucFlags |= DOB_DATA_FROM_HEAP;
                }

                // If this is a Xantrex proprietary PGN then the last two bytes of
                // the buffer is a CRC that identifies this as a Xantrex
                // message rather than anyone else's
                // This does not apply if we are sending a ZeroRPM proprietary message
                if( ( ulPgn == PGN_PGN_PROP_MSG )
                    && ( pucBuffer[ 0 ] < XBMSG_ZRPM_MSG_ID_START )
                    && ( uiBufSize > 2 )
                    && ( ucDest != PGN_ADDR_GLOBAL ) )
                {
                    xbmsg_fnCalcPropMsgCsum( pucBuffer,
                                             ptzPgnInfo->pktzDesc->uiFixedLen,
                                             ucDest );
                }

                // Set port, source and destination in DOB
                if ( XBMEM_fnGetPriorityDobStatus() == XBMEM_VIP_BUF_ENABLE )
                {
                    if (ucBuf == XB_VIP_BUF)
                    {
                        PDOBSetPort( ptzDob, XB_VIP_BUF );
                    }
                    else if (ucBuf == XB_REG_BUF)
                    {
                        PDOBSetPort( ptzDob, XB_REG_BUF );
                    }
                }
                else
                {
                    PDOBSetPort( ptzDob, XB_PORT_A );
                }
                ptzDob->m_DestAddr = ucDest;
                ptzDob->m_SourceAddr = XBADDR_fnGetMyAddr();

                // Look at PGN protocol and message length to determine
                // the actual transfer protocol to use
                // If protocol is Fastpacket and message too long for fastpacket
                if( ( ptzDob->m_ucFlags & DOB_IS_FASTPACKET ) != 0 )
                {
                    if( ptzDob->m_i16ByteCount > PGN_MAX_LEN_FASTPACKET )
                    {
                        // Set multipacket flag to send as multipacket
                        ptzDob->m_ucFlags |= DOB_IS_MULTIPACKET;

                        // RVC Multipacket destination is always FF.
                        ptzDob->m_DestAddr = 0xFF;
                    }
                }
                // Not Fast packet -- If message too long for a single frame
                else if( ptzDob->m_i16ByteCount > PGN_MAX_LEN_SINGLEFRAME )
                {
                    // Set multipacket flag to send as multipacket
                    ptzDob->m_ucFlags |= DOB_IS_MULTIPACKET;

                    // RVC Multipacket destination is always FF.
                    ptzDob->m_DestAddr = 0xFF;

                }
                // else single frame, no flag set

                // Queue DOB for sending
                if ( XBMEM_fnGetPriorityDobStatus() == XBMEM_VIP_BUF_ENABLE )
                {
                    if (ucBuf == XB_VIP_BUF)
                    {
                        TFX_teResult = addToVipDob( ptzDob );
                    }
                    else if (ucBuf == XB_REG_BUF)
                    {
                        TFX_teResult = DOBHLDAdd( ptzDob );
                    }
                }
                else
                {
                    TFX_teResult = DOBHLDAdd( ptzDob );
                }

                // Check result
                if( TFX_teResult == TFXCR_OK )
                {
                    // Set state as ready to go out
                    ptzDob->m_eState = XDATA;

                    // If this is for all nodes and its a command message
                    if( ( ucDest == PGN_ADDR_GLOBAL ) &&
                        ( ptzPgnInfo->uiPgTypeFlags == PGN_PG_TYPE_CMD ) )
                    {

                        // Allocate memory for a simulated Rx Data OBject
                        TFX_teResult = PDOBConstructAlloc( &ptzRxDob,
                                                           ( sint32 )ulPgn,
                                                           siGrpFunc,
                                                           ptzPgnInfo );

                        // Ok so far?
                        if( TFX_teResult == TFXCR_OK )
                        {
                            // Copy TX to Rx DOB
                            ( void )memcpy( ptzRxDob,
                                            ptzDob,
                                            sizeof( *ptzRxDob ) );

                            // Allocate data buffer for Rx DOB
                            ptzRxDob->m_pData =  XBMEM_fnMalloc( uiBufSize );

                            if( ptzRxDob->m_pData != NULL )
                            {
                                // Copy data
                                ( void )memcpy( ptzRxDob->m_pData,
                                                ptzDob->m_pData,
                                                uiBufSize );

                                // Set state as received
                                ptzRxDob->m_eState = XCOMPLETE;

                                ptzRxDob->m_ucFlags |= DOB_RECV_COMPLETE;

                                // Give it back to the sending node
                                XBMSG_fnRawReceive( ptzRxDob );
                            }
                            else
                            {
                                PDOBDestruct( ptzRxDob );
                            }
                        }
                    }
                    // Success
                    return ptzDob;
                }
                else
                {
                    // Cannot queue DOB, release DOB - this will release the buffer
                    PDOBDestruct( ptzDob );

                    // Save error
                    XBINIT_fnSaveError( PGN_eESRC_TFX_QUEUE_DOB,
                                        ( uint16 )TFX_teResult );
                }
            }
            else
            {
                // Data OBject allocation failed

                // Save error
                XBINIT_fnSaveError( PGN_eESRC_TFX_QUEUE_DOB,
                                    ( uint16 )TFX_teResult );

                // Is a buffer allocated
                if( pucBuffer != NULL )
                {
                    // Free the allocated buffer
                    XBMEM_fnFree( pucBuffer );
                }
            }
        }
        // else -- pack will save its own error
    }

    // Failure
    return NULL;
}

/******************************************************************************

FUNCTION NAME:
    XBMSG_fnSend                - Send PG data message on the XanBus

PURPOSE:
    This function is used to convert a PG data structure into a PG message
    and add it to the list of messages to send.

INPUTS:
    'ulPgn' is PG Number of PG to send
    'pvSrcStruct' is a pointer to data structure holding PG data to be sent
    'uiSrcSize' is size of the data structure
    'ucDest' is destination address for messages that require a destination

OUTPUTS:
    nothing

NOTES:

VERSION HISTORY:

Version 1.04  Date: 06/24/03  By: Dalem
    - Major change to XB API

******************************************************************************/
tucBOOL XBMSG_fnSend( uint32 ulPgn,
                      void *pvSrcStruct,
                      uint16 uiSrcSize,
                      uchar8 ucDest )
{
    // Verify parameter
    (void)assert( pvSrcStruct != NULL );

    // Call extended send function
    return XBMSG_fnSendEx( ulPgn,
                           ( sint16 )XB_eGRP_FUNC_VALUE_DO_NOT_CARE,
                           pvSrcStruct,
                           uiSrcSize,
                           ucDest );
}

/******************************************************************************

FUNCTION NAME:
    XBMSG_fnPeriodicSend        - Handle Periodic Xmit request

PURPOSE:
    This function is used to handle a periodic transmit request from a timer

INPUTS:
    'ulPgn' is PGN from periodic timer
    'ucDest' is destination for message send

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.03  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.04  Date: 07/03/03  By: Dalem
    - Change to using XBPACK_fnGetPgnInfo
Version 1.05  Date: 2019-01-10  By: TomL
    - Allowed for support of proprietary messages with group function packed
      into normally zero PS byte

******************************************************************************/

void XBMSG_fnPeriodicSend( uint32 ulPgn,
                           uchar8 ucDest )
{
    const PGN_tzPGN_INFO *pktzInfo;
    PGN_tzWANT_DATA tzWant;
    uint32 ulBasePgn;
    uint32 ulDaByte;
    sint16 siGrpFunc = ( sint16 )XB_eGRP_FUNC_VALUE_DO_NOT_CARE;

    // Proprietary messages (00EF00) can use the PS byte to specify the
    // Group Function
    ulBasePgn = ulPgn & 0x01ff00;
    ulDaByte = ulPgn & 0x0000ff;
    if (( ulBasePgn == PGN_PGN_PROP_MSG )
        && ( ulDaByte != 0 ))
    {
        ulPgn = ulBasePgn;
        siGrpFunc = ( sint16 )ulDaByte;
    }

    // Search list of support for PGNs
    pktzInfo = XBPACK_fnGetPgnInfo( ulPgn, siGrpFunc );

    // If PGN found and send function exists
    if( ( pktzInfo != NULL ) && ( pktzInfo->pktzDesc != NULL ) &&
        ( pktzInfo->pktzDesc->pfnWant != NULL ) )
    {
        // Call the send function for this PGN
         tzWant.tucSolicited = FALSE;
        tzWant.ucDestAddr = ucDest;

        pktzInfo->pktzDesc->pfnWant( &tzWant );
    }
}

/******************************************************************************

FUNCTION NAME:
    XBMSG_fnSendISOAck          - Send an acknowledgement message

PURPOSE:
    This function is used to send an acknowledgement message for a given PGN.
    The PGN determines if the interpretation of the group function code is
    a reason or a group function code.

INPUTS:
    'ulPgn' is PGN to ACK
    'teReason' is reason code for NAK or Deny
    'eControl' is the type of ACK

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.03  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.04  Date: 08/28/03  By: Dalem
    - Added teReason parameter
Version 1.05  Date: 05/18/06  By: Hollyz
    - Sending ISO ACK now needs specific destination address

******************************************************************************/

void XBMSG_fnSendISOAck( uint32 ulPgn,
                         XB_teGRP_FUNC_VALUE teReason,
                         XB_teACK_STS teControl,
                         uchar8 ucDest )
{
    XB_tzPGN_ISO_ACK tzAckPG;

    // Set control byte, 0 for ACK
    tzAckPG.teCtrlByte = teControl;

    // Set Group Function qualifier
    tzAckPG.teGroupFunctionValue = teReason;

    // Set PGN number
    tzAckPG.ulParmGrpNum = ulPgn;

    // Send ISO Acknowledge message
    XBMSG_fnSend( XB_PGN_ISO_ACK,
                  &tzAckPG,
                  sizeof(tzAckPG),
                  ucDest );
}

/******************************************************************************

FUNCTION NAME:
    XBMSG_fnRecvISOReq          - Handle Received ISO Request message

PURPOSE:
    This function is used to handle a ISO Request message

INPUTS:
    'ptzRecv' is a pointer to the received message work block

OUTPUTS:
    Nothing

NOTES:
 If PGN is transmittable, requestable and has a function, function is called
 If PGN is transmittable but not requestable, an ACK Deny is sent
 Otherwise, if ISO Request was not a broadcast, an NACK is sent
 If ISO Request was a broadcast, request is ignored

VERSION HISTORY:

Version 1.04  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.05  Date: 07/03/03  By: Dalem
    - Change to using XBPACK_fnGetPgnInfo
Version 1.06  Date: 08/18/03  By: Dalem
    - Sending Nack does not depend on request being directed
Version 1.07  Date: 09/04/03  By: Dalem
    - Restructured because there are more flags
Version 1.08  Date: 09/08/03  By: Dalem
    - Send NAK for unsupported PGN only if request is not broadcast
Version 1.08  Date: 09/15/03  By: Dalem
    - Do not NAK unknown PGN if request is broadcast
Version 1.09  Date: 09/30/03  By: Dalem
    - Added check for nonsupport (neither received nor send function)
Version 1.10  Date: 12/11/03  By: Dalem
    - Removed check for nonsupport (neither received nor send function)
Version 1.11  Date: 12/12/03  By: Peterd
    - Moved check for NULL descriptor to check for send function
Version 1.12  Date: 02/10/04  By: Peterd
    - Added checks for newly added flags
Version 1.13  Date: 04/14/04  By: Peterd
    - Added check for sleep

******************************************************************************/
void XBMSG_fnRecvISOReq( PGN_tzRECV_DATA *ptzRecv )
{
    XB_tzPGN_ISO_REQ *ptzISOReq;
    const PGN_tzPGN_INFO *pktzInfo;
    PGN_tzWANT_DATA tzWant;
    uint32 ulPgn;

    // Verify parameter
    (void)assert( ptzRecv != NULL );

    // If we are sleeping
    if( XBINIT_fnChkAsleep() == TRUE )
    {
        // Ignore request
        return;
    }

    // Convert void pointer to ISO request pointer
    ptzISOReq = ( XB_tzPGN_ISO_REQ * )ptzRecv->pvStruct;

    // Extract requested PGN from request
    ulPgn = ptzISOReq->ulParmGrpNum;

    // If address claim PGN
    if( ulPgn == XB_PGN_ISO_ADDR_CLAIM )
    {
        // Send claim to global address
        tzWant.tucSolicited = TRUE;

    // For RV-C, the request is usually from a device that doesn't have
    // an address.  In such cases, give a destination address of zero,
    // in compliance with section 3.3.2 of the spec.
    if ( ptzRecv->ucSrcAddr == PGN_ADDR_NULL )
    {
        // This is the usual case, with a new node wanting to claim an address
        tzWant.ucDestAddr = 0;
    }
    else
    {
        // Reply directly to requester's address
        tzWant.ucDestAddr = ptzRecv->ucSrcAddr;
    }

        XBADDR_fnWantClaim( &tzWant );
    }
    else
    {
        // Search PGN support list for PGN
        pktzInfo = XBPACK_fnGetPgnInfo( ulPgn,
                                       ( sint16 )XB_eGRP_FUNC_VALUE_DO_NOT_CARE );

        // If PGN not found
        // or PGN is found but has no descriptor
        if( pktzInfo == NULL )
        {
            // Send NAK but only if request is not broadcast
            if( ptzRecv->ucDestAddr != PGN_ADDR_GLOBAL )
            {
                // Send NACK
                XBMSG_fnSendISOAck( ulPgn,
                                    XB_eGRP_FUNC_VALUE_UNSUPPORTED,
                                    XB_eACK_STS_NAK,
                                    ptzRecv->ucSrcAddr );
            }
        }
        else
        {
            // If PGN is requestable (i.e., we're a responder)
            if( ( pktzInfo->uiFlags & PGN_FLAG_REQ ) != 0 )
            {
                // If we should respond with the PGN
                if( ( pktzInfo->uiFlags & PGN_FLAG_RESP_TX_PGN ) != 0 )
                {
                    // If there is descriptor and a send function
                    if( ( pktzInfo->pktzDesc != NULL )
                        && ( pktzInfo->pktzDesc->pfnWant != NULL ) )
                    {
                        // Set up a data structure for request to send
                        // Request is solicited
                        tzWant.tucSolicited = TRUE;

                        // Destination is the ISO REQ sender
                        tzWant.ucDestAddr = ptzRecv->ucSrcAddr;

                        // Call the function for this PGN
                        pktzInfo->pktzDesc->pfnWant( &tzWant );
                    }
                    else
                    {
                        // Send NAK but only if request is not broadcast
                        if( ptzRecv->ucDestAddr != PGN_ADDR_GLOBAL )
                        {
                            // Send a NACK
                            XBMSG_fnSendISOAck( ulPgn,
                                                XB_eGRP_FUNC_VALUE_NOT_AVAILABLE,
                                                XB_eACK_STS_NAK,
                                                ptzRecv->ucSrcAddr );
                        }
                    }
                }
                else if( ( ( pktzInfo->uiFlags & PGN_FLAG_RESP_TX_DNY ) != 0 ) &&
                    ( pktzInfo->pktzDesc->pfnRecv != NULL ) )
                {
                    // Send a Deny
                    XBMSG_fnSendISOAck( ulPgn,
                                        XB_eGRP_FUNC_VALUE_PROTOCOL,
                                        XB_eACK_STS_DNY,
                                        ptzRecv->ucSrcAddr );
                }
                else
                {
                    // Send NAK but only if request is not broadcast
                    if( ptzRecv->ucDestAddr != PGN_ADDR_GLOBAL )
                    {
                        // Send a NACK
                        XBMSG_fnSendISOAck( ulPgn,
                                            XB_eGRP_FUNC_VALUE_NOT_AVAILABLE,
                                            XB_eACK_STS_NAK,
                                            ptzRecv->ucSrcAddr );
                    }
                }
            }
            else
            {
                // If deny flag set for TX
                if( ( pktzInfo->uiFlags & PGN_FLAG_NRSP_TX_DNY ) != 0 )
                {
                    // Send Deny
                    XBMSG_fnSendISOAck( ulPgn,
                                        XB_eGRP_FUNC_VALUE_PROTOCOL,
                                        XB_eACK_STS_DNY,
                                        ptzRecv->ucSrcAddr );
                }
                else if( ( pktzInfo->uiFlags & PGN_FLAG_NRSP_TX_NAK ) != 0 )
                {
                    // Send NAK but only if request is not broadcast
                    if( ptzRecv->ucDestAddr != PGN_ADDR_GLOBAL )
                    {
                        // Send NACK
                        XBMSG_fnSendISOAck( ulPgn,
                                            XB_eGRP_FUNC_VALUE_UNSUPPORTED,
                                            XB_eACK_STS_NAK,
                                            ptzRecv->ucSrcAddr );
                    }
                }
                else
                {
                    // Send NAK but only if request is not broadcast
                    if( ptzRecv->ucDestAddr != PGN_ADDR_GLOBAL )
                    {
                        // Send a NACK
                        XBMSG_fnSendISOAck( ulPgn,
                                            XB_eGRP_FUNC_VALUE_NOT_AVAILABLE,
                                            XB_eACK_STS_NAK,
                                            ptzRecv->ucSrcAddr );
                    }
                }
            }
        }
    }
}

/******************************************************************************

FUNCTION NAME:
    XBMSG_fnIsoReq              - Send an ISO Request

PURPOSE:
    This function sends an ISO Request for the given PGN from all other
    nodes on the network.

INPUTS:
    'ulPgn' is PGN being requested from other nodes
    'ucDest' is destination address for request

OUTPUTS:
    Nothing

NOTES:
    This function is not intended to be linked with the Tx Support list.
    Its parameters are different and it does not make sense for the
    Tx Support list to have an entry for ISO Request PGN because it needs
    to be passed a PGN parameter.

VERSION HISTORY:

Version 1.03  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.04  Date: 07/03/03  By: Dalem
    - Change to using XBPACK_fnGetPgnInfo

******************************************************************************/
void XBMSG_fnISOReq( uint32 ulPgn, uchar8 ucDest )
{
    XB_tzPGN_ISO_REQ tzISOReq;
    const PGN_tzPGN_INFO *pktzInfo;
    PGN_tzWANT_DATA tzWant;

    // Place PGN in ISO request structure
    tzISOReq.ulParmGrpNum = ulPgn;

    // Send ISO request
    XBMSG_fnSend( XB_PGN_ISO_REQ,
                  &tzISOReq,
                  sizeof(tzISOReq),
                  ucDest );

    // If destination address is Global, respond to it ourself
    if( ucDest == PGN_ADDR_GLOBAL )
    {
        // Search list of support for PGNs
        pktzInfo = XBPACK_fnGetPgnInfo( ulPgn,
                                       ( sint16 )XB_eGRP_FUNC_VALUE_DO_NOT_CARE );

        // If PGN found and send function exists
        if( ( pktzInfo != NULL )
         && ( pktzInfo->pktzDesc != NULL )
         && ( pktzInfo->pktzDesc->pfnWant != NULL ) )
        {
            // Request that this PGN to be sent solicited
            tzWant.tucSolicited = TRUE;
            tzWant.ucDestAddr = ucDest;
            pktzInfo->pktzDesc->pfnWant( &tzWant );
        }
    }
}

/******************************************************************************

FUNCTION NAME:
    XBMSG_fnWantPgnList         - Send NMEA Rx and Tx PGN lists

PURPOSE:
    This function is used to handle a request for sending the NMEA Rx and Tx
    PGN supported lists

INPUTS:
    'ucDest' is destination for message send
    'tucSolicited' is TRUE if request is from network

OUTPUTS:
    none

NOTES:
    tucSolicited is included so function complies with expectations
    in PGN table data structure

VERSION HISTORY:

Version 1.03  Date: 06/24/03  By: Dalem
    - Major change to XB API

******************************************************************************/
void XBMSG_fnWantPgnList( PGN_tzWANT_DATA *ptzWant )
{
    XB_tzPGN_PGN_LIST_RX tzRxPGNMsg;
    XB_tzPGN_PGN_LIST_TX tzTxPGNMsg;
    sint16 siGrpFunc;

    //--- Send RX list ---

    // Set Rx group function
    siGrpFunc = ( sint16 )XB_eGRP_FUNC_TX_AND_RX_PGN_LIST_RX_PGN_LIST_MESSAGE;

    // Set extra field pointer and extra field count
    tzRxPGNMsg.ptzExtra = (XB_tzPGN_EXTRA_PGN_LIST_RX *) XB_kaulRX_PGN_LIST;
    tzRxPGNMsg.uiExtraCount = XBPGNTBL_fnSizeOfRxPgnList();

    // Set group function to Receive list message
    tzRxPGNMsg.teRXedPgnGroupFunctionCode = ( XB_teGRP_FUNC_TX_AND_RX_PGN_LIST )siGrpFunc;

    // Send RX list
    (void) XBMSG_fnSendEx( XB_PGN_PGN_LIST,
                           siGrpFunc,
                           &tzRxPGNMsg,
                           sizeof(tzRxPGNMsg),
                           ptzWant->ucDestAddr );

    //--- Send TX list ---

    // Set number of entries in Rx list and Rx group function
    siGrpFunc = ( sint16 )XB_eGRP_FUNC_TX_AND_RX_PGN_LIST_TX_PGN_LIST_MESSAGE;

    // Set extra field pointer and extra field count
    tzTxPGNMsg.ptzExtra = (XB_tzPGN_EXTRA_PGN_LIST_TX *) XB_kaulTX_PGN_LIST;
    tzTxPGNMsg.uiExtraCount = XBPGNTBL_fnSizeOfTxPgnList();

    // Set group function to Transmit list message
    tzTxPGNMsg.teTXedPgnGroupFunctionCode = ( XB_teGRP_FUNC_TX_AND_RX_PGN_LIST )siGrpFunc;

    // Send TX list
    (void) XBMSG_fnSendEx( XB_PGN_PGN_LIST,
                           siGrpFunc,
                           &tzTxPGNMsg,
                           sizeof(tzTxPGNMsg),
                           ptzWant->ucDestAddr );
}


/******************************************************************************

FUNCTION NAME:
    XBMSG_fnRecvConnMgmt - Handle Received ISO Connection Management message

PURPOSE:
    This function is used to handle a ISO Connection Management message

INPUTS:
    'ptzRecv' is a pointer to the received message work block

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.01  Date: 06/24/03  By: Dalem
    - Major change to XB API

******************************************************************************/
void XBMSG_fnRecvConnMgmt( PGN_tzRECV_DATA *ptzRecv )
{
    // Verify parameter
    (void)assert( ptzRecv != NULL );

    // Action depends on the message group function
    switch( ptzRecv->siGrpFunc )
    {
        case XB_ISO_CONN_MAN_RTS:
        {
            // Call RTS receive handler with structure and source address
            XBMLTPKT_fnRecvRts( ptzRecv->pvStruct,
                                ptzRecv->ucSrcAddr );
        }
        break;

        case XB_ISO_CONN_MAN_CTS:
        {
            // Call CTS receive handler with structure and source address
            XBMLTPKT_fnRecvCts( ptzRecv->pvStruct,
                                ptzRecv->ucSrcAddr );
        }
        break;

        case XB_ISO_CONN_MAN_EOM:
        {
            // Call EOM receive handler with structure and source address
            XBMLTPKT_fnRecvEom( ptzRecv->pvStruct,
                                ptzRecv->ucSrcAddr );
        }
        break;

        case XB_ISO_CONN_MAN_BAM:
        {
            // Call BAM receive handler with structure and source address
            XBMLTPKT_fnRecvBam( ptzRecv->pvStruct,
                                ptzRecv->ucSrcAddr );
        }
        break;

        case XB_ISO_CONN_MAN_ABORT:
        {
            // Call Abort receive handler with structure and source address
            XBMLTPKT_fnRecvAbort( ptzRecv->pvStruct,
                                  ptzRecv->ucSrcAddr );
        }
        break;

        default:
        {
            // Save error
            XBINIT_fnSaveError( PGN_eESRC_MESSAGE,
                                ( uint16 )PGN_eERR_BAD_VALUE );
        }
        break;
    }
}

/******************************************************************************

FUNCTION NAME:
    XBMSG_fnSendCommStats       - Send communications statistics PGN

PURPOSE:
    This function is used to send a communications statistics PGN

INPUTS:
    'ucDest' is the destination address (not used by message)

OUTPUTS:
    none

NOTES:

VERSION HISTORY:

Version 1.03  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.04  Date: 07/08/03  By: Tom Lightfoot
    - Error frame counts are no longer part of Comm Stats
Version 1.05  Date: 09/17/03  By: Dalem
    - Added more statistics

******************************************************************************/
void XBMSG_fnWantCommStats( PGN_tzWANT_DATA *ptzWant )
{
    CAN_tzCOMM_STATS tzStats;
    XB_tzPGN_COMM_STATS tzCommStatsPG;

    // Set message count to 1 if responding to ISO request
    tzCommStatsPG.ucMsgCount = ( ptzWant->tucSolicited ? 1 : 0 );

    // Get count of frames that have been sent
    if( TFXDRV_fnGetCommStats( XB_PORT_A,
                                &tzStats ) == TFXCR_OK )
    {
        tzCommStatsPG.ulTxExtFrameCnt   = tzStats.ulTxExtFrames;
        tzCommStatsPG.ulRxExtFrameCnt   = tzStats.ulRxExtFrames;
        tzCommStatsPG.uiRxErrCnt        = tzStats.uiRxErrCount;
        tzCommStatsPG.uiTxErrCnt        = tzStats.uiTxErrCount;
        tzCommStatsPG.uiBusOffCnt       = tzStats.uiBusoffCount;
        tzCommStatsPG.uiRxFrameDropCnt  = tzStats.uiRxDropped;
        tzCommStatsPG.uiTxFrameDropCnt  = tzStats.uiTxDropped;
        tzCommStatsPG.uiMemAllocFailCnt = XBMSG_tzXbCommStats.uiMallocFail;
        tzCommStatsPG.ucMpToCnt         = XBMSG_tzXbCommStats.ucMPRxTimeout
                                          + XBMSG_tzXbCommStats.ucMPTxTimeout;
        tzCommStatsPG.ucMpLateFrameCnt  = XBMSG_tzXbCommStats.ucMPLateCts
                                          + XBMSG_tzXbCommStats.ucMPLateEom
                                          + XBMSG_tzXbCommStats.ucMPLateAbort
                                          + XBMSG_tzXbCommStats.ucMPLateXfer;
        tzCommStatsPG.uiHeapFreeMin     = XBMSG_tzXbCommStats.uiHeapFreeMin;
    }
    else
    {
        tzCommStatsPG.ulTxExtFrameCnt   = PGN_UL_NO_DATA;
        tzCommStatsPG.ulRxExtFrameCnt   = PGN_UL_NO_DATA;
        tzCommStatsPG.uiRxErrCnt        = PGN_UI_NO_DATA;
        tzCommStatsPG.uiTxErrCnt        = PGN_UI_NO_DATA;
        tzCommStatsPG.uiBusOffCnt       = PGN_UI_NO_DATA;
        tzCommStatsPG.uiRxFrameDropCnt  = PGN_UI_NO_DATA;
        tzCommStatsPG.uiTxFrameDropCnt  = PGN_UI_NO_DATA;
        tzCommStatsPG.uiMemAllocFailCnt = PGN_UI_NO_DATA;
        tzCommStatsPG.ucMpToCnt         = PGN_UC_NO_DATA;
        tzCommStatsPG.ucMpLateFrameCnt  = PGN_UC_NO_DATA;
        tzCommStatsPG.uiHeapFreeMin     = PGN_UI_NO_DATA;
    }

    // Send a communications statistics message
    XBMSG_fnSend( XB_PGN_COMM_STATS,
                  &tzCommStatsPG,
                  sizeof(tzCommStatsPG),
                  ptzWant->ucDestAddr );
}

/******************************************************************************

FUNCTION NAME:
    XBMSG_fnClearCommStats      - Clear communications statistics

PURPOSE:
    This function is used to clear a communications statistics

INPUTS:
    none

OUTPUTS:
    none

NOTES:

VERSION HISTORY:

Version 1.00  Date: OCT-02-2003  By: Dalem
    - Created

******************************************************************************/
void XBMSG_fnClearCommStats( void )
{
    // Clear XanBus layer comm stats
    (void) memset( &XBMSG_tzXbCommStats, 0, sizeof( XBMSG_tzXbCommStats ) );

    XBMSG_tzXbCommStats.uiHeapFreeMin = PGN_UI_NO_DATA;

    // Clear driver layer comm stats
    (void) TFXDRV_fnClearCommStats( XB_PORT_A );
}

/******************************************************************************

FUNCTION NAME:
    XBMSG_fnGetCommStats      - Get communications statistics

PURPOSE:
    This function is used to get the communications statistics data

INPUTS:
    none

OUTPUTS:
    none

NOTES:

VERSION HISTORY:

Version 1.00  Date: Feb-11-2009  By: HollyZ
    - Created

******************************************************************************/
XB_tzXB_COMM_STATS * XBMSG_fnGetCommStats( void )
{
    return( &XBMSG_tzXbCommStats );
}

/******************************************************************************

FUNCTION NAME:
    XBMSG_fnSendEx              - Send PG data message on the XanBus

PURPOSE:
    This function is used to convert a PG data structure into a PG message
    and add it to the list of messages to send.

INPUTS:
    'ulPgn' is PG Number of PG to send
    'siGrpFunc' is Group function qualifier value if PG uses group functions
    'pvSrcStruct' is a pointer to data structure holding PG data to be sent
    'uiSrcSize' is size of the data structure
    'ucDest' is destination address for messages that require a destination

OUTPUTS:
    TRUE if message is successfully converted and queued for transmission,
    FALSE and XB ERROR code should be set on failure.

NOTES:

VERSION HISTORY:

Version 1.02  Date: 06/24/03  By: Dalem
******************************************************************************/
tucBOOL XBMSG_fnSendEx( uint32 ulPgn,
                        sint16 siGrpFunc,
                        void *pvSrcStruct,
                        uint16 uiSrcSize,
                        uchar8 ucDest )
{
    PGNDOB *pDob;

    pDob =  XBMSG_fnSendExPdob( ulPgn,
                                siGrpFunc,
                                pvSrcStruct,
                                uiSrcSize,
                                ucDest,
                                XB_REG_BUF );
    return ( pDob == NULL ) ? FALSE : TRUE;
}


/******************************************************************************

FUNCTION NAME:
    XBMSG_fnSendPriority          - Send PG data message on the XanBus

PURPOSE:
    This function is used to convert a PG data structure into a PG message
    and add it to the list of messages to send.

INPUTS:
    'ulPgn' is PG Number of PG to send
    'siGrpFunc' is Group function qualifier value if PG uses group functions
    'pvSrcStruct' is a pointer to data structure holding PG data to be sent
    'uiSrcSize' is size of the data structure
    'ucDest' is destination address for messages that require a destination

OUTPUTS:
    TRUE if message is successfully converted and queued for transmission,
    FALSE and XB ERROR code should be set on failure.

NOTES:

VERSION HISTORY:

Version 1.02  Date: 06/24/03  By: Dalem
******************************************************************************/
tucBOOL XBMSG_fnSendPriority ( uint32 ulPgn,
                               sint16 siGrpFunc,
                               void *pvSrcStruct,
                               uint16 uiSrcSize,
                               uchar8 ucDest )
{
    PGNDOB *pDob;

    pDob = XBMSG_fnSendExPdob( ulPgn,
                               siGrpFunc,
                               pvSrcStruct,
                               uiSrcSize,
                               ucDest,
                               XB_VIP_BUF );
    return ( pDob == NULL ) ? FALSE : TRUE;
}

/******************************************************************************

FUNCTION NAME:
    XBMSG_fnOnLine              - XanBus stack ready for application messages

PURPOSE:
    This function is used to test if address claiming has completed and
    if the TeleFlex library is on line.

INPUTS:
    None

OUTPUTS:
    TRUE if Address Claiming is complete and application can send/receive,
    FALSE if not ready for application messages.

NOTES:

VERSION HISTORY:

Version 1.01  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.02  Date: 09/19/03  By: Dalem
    - Use function fnAddrState.
Version 1.03  Date: 12/01/08  By: HollyZ
    - We're not able to send/receive if the bus is off.
Version:1.04  Date: Fri 11/29/2013  By: Scott Volk
    - Re-added function since it will be used to restart the xanbus task at the
      application level

******************************************************************************/
tucBOOL XBMSG_fnOnLine( void )
{
    // If we are on line
    if( ( XBADDR_fnAddrState() == XB_eADDR_ONLINE ) &&
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
    xbmsg_fnCalcPropMsgCsum

PURPOSE:
    This function calculates and embeds a two-byte checksum at the end
    of a proprietary message.  This helps to identify it as proprietary
    to Xantrex as opposed to any other manufacturer's device.

INPUTS:
    pucBuf - The outgoing packed message buffer
    uiBufLen - The length of the buffer in bytes
    uiDestAddr - The node address of the device to receive the message

OUTPUTS:
    None

NOTES:
    The CRC calculation includes not just the message buffer, but the
    net name of the sender and receiver as well.

VERSION HISTORY:

Version 1.00  Date: 2019-01-11  By: TomL
    - Initial version, based on rvcprop_fnCalculateMessageChecksum by Doru Botez

******************************************************************************/
static void xbmsg_fnCalcPropMsgCsum( uchar8 *pucBuf,
                                     uint16 uiBufLen,
                                     uchar8 ucDestAddr )
{
    uint16 uiCrc = RVC_PROP_MSG_CRC_INIT_VAL;
    uchar8 *pucName = NULL;

    // Calculate first the CRC for the network name of the sender - this device
    if ( XBADDR_fnGetNetNAME( XBADDR_fnGetMyAddr(), &pucName ) == TRUE )
    {
        if( pucName != NULL )
        {
            uiCrc = CRC16_fnCalculateBlock( uiCrc, pucName, RVC_NET_NAME_CRC_LEN );
        }
    }

    // Continue with the message body, except for the last two bytes
    uiCrc = CRC16_fnCalculateBlock( uiCrc, pucBuf, uiBufLen - 2 );

    // Add the CRC for the network name of the destination
    pucName = NULL;
    if ( XBADDR_fnGetNetNAME( ucDestAddr, &pucName ) == TRUE )
    {
        if( pucName != NULL )
        {
            uiCrc = CRC16_fnCalculateBlock( uiCrc, pucName, RVC_NET_NAME_CRC_LEN );
        }
    }

    // Insert the CRC into the message - MSB
    pucBuf[ uiBufLen - 2 ] = ( uchar8 )( uiCrc >> 8 );
    // LSB
    pucBuf[ uiBufLen - 1 ] = ( uchar8 )( uiCrc & 0x00FF );
}

/******************************************************************************

FUNCTION NAME:
    xbmsg_fnVerifyPropMsg

PURPOSE:
    This function makes sure that an incoming proprietary message is valid.
    In the usual case, Xantrex proprietary messages (those with message IDs
    in the low end of the number range) have a two-byte CRC at the end of
    the message.  However, we also recognize some ZeroRPM and Lithionics proprietary
    messages.  These have message IDs 0xAA and 0xAB and do not have a CRC
    at the end.  We just assume that those are properly formed.

INPUTS:
    ptzDob - Points to the data object received. Holds all the info we need

OUTPUTS:
    TRUE if the message is recognized

******************************************************************************/
static tucBOOL xbmsg_fnVerifyPropMsg( PGNDOB *ptzDob )
{
    tucBOOL bIsValid = FALSE;

    // Is this proprietary message a regular Xantrex message or a ZeroRPM one?
    // ZeroRPM messages start at 0x60, whereas Xantrex messages have
    // IDs starting at 1
    if ( ptzDob->m_pData[ 0 ] < XBMSG_ZRPM_MSG_ID_START )
    {
        // All Xantrex proprietary messages have a two-byte crc at the end
        // of the fixed-length portion.  Check if the CRC is correct.
        bIsValid = xbmsg_fnVerifyPropMsgCsum( ptzDob );
    }
    else
    {
        // This must be a message which does not have a checksum.
        // Since we recognize the message ID, assume that the message is valid
        bIsValid = TRUE;
    }

    return bIsValid;
}

/******************************************************************************

FUNCTION NAME:
    xbmsg_fnVerifyPropMsgCsum

PURPOSE:
    This function calculates a two-byte checksum for a proprietary message
    and makes sure that the CRC at the end of the message matches it.
    This lets us identify the message as proprietary to Xantrex as opposed to
    any other manufacturer's device.

INPUTS:
    ptzDob - Points to the data object received. Holds all the info we need

OUTPUTS:
    TRUE if the message is the right size and the checksum matches

NOTES:
    The CRC calculation includes not just the message buffer, but the
    net name of the sender and receiver as well.

VERSION HISTORY:

Version 1.00  Date: 2019-01-11  By: TomL
    - Initial version, based on rvcprop_fnValidateMessageChecksum by Doru Botez

******************************************************************************/
static tucBOOL xbmsg_fnVerifyPropMsgCsum( PGNDOB *ptzDob )
{
    uchar8 *pucBuf = ptzDob->m_pData;
    uint16 uiBufLen = ptzDob->m_i16ByteCount;
    uint16 uiMsgSize = 0;
    uint16 uiRcvCrc;
    uint16 uiCrc = RVC_PROP_MSG_CRC_INIT_VAL;
    uchar8 *pucName = NULL;
    tucBOOL ucNoError = FALSE;
    uint16 uiPktLen;

    // Carefully get the expected size of the fixed-length portion of the received buffer
    if (( ptzDob->m_pPgnInfo != NULL )
        && (ptzDob->m_pPgnInfo->pktzDesc != NULL ))
    {
        uiMsgSize = ptzDob->m_pPgnInfo->pktzDesc->uiFixedLen;
    }

    // Is the buffer big enough to hold the expected message, including CRC?
    if (( uiBufLen > 2 ) && ( uiBufLen >= uiMsgSize ))
    {
        // Calculate first the CRC for the network name of the sender
        if ( XBADDR_fnGetNetNAME( ptzDob->m_SourceAddr, &pucName ) == TRUE )
        {
            if( pucName != NULL )
            {
                uiCrc = CRC16_fnCalculateBlock( uiCrc, pucName, RVC_NET_NAME_CRC_LEN );
            }
        }

        // Continue with the fixed-length part of the message, not including CRC
        uiPktLen = uiMsgSize - 2;
        uiCrc = CRC16_fnCalculateBlock( uiCrc, pucBuf, uiPktLen );

        // Add the CRC for the network name of the destination (if not sent to global address)
        if (( xbmsg_fnGetDestAddr( ptzDob ) == XBADDR_fnGetMyAddr() )
            && ( XBADDR_fnGetNetNAME( XBADDR_fnGetMyAddr(), &pucName ) == TRUE ))
        {
            if ( pucName != NULL )
            {
                uiCrc = CRC16_fnCalculateBlock( uiCrc, pucName, RVC_NET_NAME_CRC_LEN );
            }
        }

        // Extract the received CRC
        uiRcvCrc = ( (uint16)( pucBuf[ uiPktLen ] ) ) << 8;
        uiRcvCrc += (uint16)( pucBuf[ uiPktLen + 1 ]);

        // Compare the CRCs
        if ( uiRcvCrc == uiCrc )
        {
            ucNoError = TRUE;
        }
    }

    return ucNoError;
}

/******************************************************************************

FUNCTION NAME:
    xbmsg_fnGetDestAddr

PURPOSE:
    Get the destination address of a received DOB.

INPUTS:
    ptzDob - Points to the data object received. Holds all the info we need

OUTPUTS:
    The intended destination address of the message

NOTES:
    At the stage we need the destination address of a received DOB
    for CRC calculation on proprietary messages, it hasn't been calculated yet.
    We calculate it here from the ID field of the received CAN message.

VERSION HISTORY:

Version 1.00  Date: 2019-01-31  By: TomL
    - Initial version

******************************************************************************/
static uchar8 xbmsg_fnGetDestAddr( PGNDOB *ptzDob )
{
    uchar8 ucDestAddr = PGN_ADDR_GLOBAL;  // Always global unless it's a PDU1 message

    if (( ptzDob->m_pPgnInfo->uiFlags & PGN_FLAG_PDU1 ) != 0 )
    {
        // The PS byte has the destination address
        ucDestAddr = ptzDob->m_Frame.m_CanData.m_u8PS;
    }

    return ucDestAddr;
}
