/*=============================================================================

Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International and protected by 
copyright. The reproduction, in whole or in part, by anyone without the
written approval of Xantrex is prohibited.

FILE NAME:
    xbplatform.c -  XanBus Application Platform specific code

PURPOSE:
    This file is intended to contain functions that an application must 
    provide for use by XanBus code.

FUNCTION(S):
    XBPLATFORM_fnCheckErrors - Check for system errors
    XBPLATFORM_fnSetNetIndic - Set network indicator
    XBPLATFORM_fnLoad       - Get starting data required for XanBus
    XBPLATFORM_fnSave       - Store XanBus starting data
    XBPLATFORM_fnNodeChange - A node has been added or dropped
    XBPLATFORM_fnRecvXbMsg  - Pass XanBus handled message to application

    local:
        none

NOTES:
    
CHANGE HISTORY:
$Log: xbplatform.c $

=============================================================================*/

/*==============================================================================
                              Includes
==============================================================================*/

#include <string.h>         // for memcpy

#include "xbudefs.h"        // XanBus message interface
#include "xbgdefs.h"        // XanBus application interface
#include "config.h"         // XT_mMK_BYTE macro
#include "xassert.h"        // Assert macro

/*==============================================================================
                              Defines
==============================================================================*/

#define UNIQUE_NUMBER   ( 0x12345 )             // Unique Number (serial number)
#define MANU_CODE       ( XB_MFG_XANTREX )      // Manufacturer Code
#define INST_LOWER      ( 0 )                   // Instance Lower, fill with 1s
#define INST_UPPER      ( 0 )                   // Instance Upper, should be 0
#define DEVICE_FUNC     ( XB_eFUNC_CODE_INVERTER )   // Device Function
#define RESERVED_BIT    ( 0 )                   // Reserved Bit, must be zero
#define DEVICE_CLASS    ( XB_eDEV_CLASS_POWER_MANAGEMENT )    // Device Class
#define SYSTEM_INST     ( XB_SYSTEM_INSTANCE )  // System Instance
#define INDUSTRY_GROUP  ( XB_INDUSTRY_GROUP )   // Industry Group
#define SELF_ADDRESS    ( 1 )                 // Self Config Address, must be 1

/*==============================================================================
                           Local/Private Variables
==============================================================================*/



uchar8 ucPrefAddr = 0x20;
uchar8 aucName[ PGN_NAME_LEN ] =
{
    XT_mMK_BYTE( UNIQUE_NUMBER ),
    XT_mMK_BYTE( UNIQUE_NUMBER >> 8 ),
    XT_mMK_BYTE( ( UNIQUE_NUMBER >> 16 ) & 0x1F ) | ( MANU_CODE << 5 ),
    XT_mMK_BYTE( MANU_CODE >> 3 ),
    XT_mMK_BYTE( INST_LOWER ) | ( INST_UPPER << 3 ),
    XT_mMK_BYTE( DEVICE_FUNC ),
    XT_mMK_BYTE( RESERVED_BIT ) | ( DEVICE_CLASS << 1 ),
    XT_mMK_BYTE( SYSTEM_INST ) | ( INDUSTRY_GROUP >> 4 ) | ( SELF_ADDRESS << 7 )
};

// This is the default list for periodic transmit timers
const XB_tzSEND_TIMERS TEST_ktzDFLT_SEND_TMR =
{
    0,

    {
        // Iso Address Claim, global, 100 ms
        { XB_PGN_ISO_ADDR_CLAIM,    PGN_ADDR_GLOBAL, XB_PERIOD_ISO_ADDR_CLAIM },
        // General status, global, 250 ms
        { XB_PGN_STS,               PGN_ADDR_GLOBAL, XB_PERIOD_STS },
        // Gateway Status, global, 500 ms
        { XB_PGN_GW_STS,            PGN_ADDR_GLOBAL, XB_PERIOD_DEVICE_STS },
        { PGN_UL_MAX, PGN_UC_MAX, PGN_UI_MAX },
        { PGN_UL_MAX, PGN_UC_MAX, PGN_UI_MAX },
        { PGN_UL_MAX, PGN_UC_MAX, PGN_UI_MAX },
        { PGN_UL_MAX, PGN_UC_MAX, PGN_UI_MAX },
        { PGN_UL_MAX, PGN_UC_MAX, PGN_UI_MAX },
        { PGN_UL_MAX, PGN_UC_MAX, PGN_UI_MAX },
        { PGN_UL_MAX, PGN_UC_MAX, PGN_UI_MAX }
    }
};

// This is the default list of messages that can have a receive timeout
const XB_tzRECV_TIMERS TEST_ktzDFLT_RECV_TMR =
{
    0,
    {
        // iso address claim, 5 s
        { XB_PGN_ISO_ADDR_CLAIM,    XB_TO_ISO_ADDR_CLAIM },
        // general status, 1.25 s
        { XB_PGN_STS,               XB_TO_STS },
        // date time status, 1.25 s
        { XB_PGN_DATE_TIME_STS,     XB_TO_DEVICE_STS },
        { PGN_UL_MAX, PGN_UI_MAX },
        { PGN_UL_MAX, PGN_UI_MAX },
        { PGN_UL_MAX, PGN_UI_MAX },
        { PGN_UL_MAX, PGN_UI_MAX },
        { PGN_UL_MAX, PGN_UI_MAX },
        { PGN_UL_MAX, PGN_UI_MAX },
        { PGN_UL_MAX, PGN_UI_MAX }
    }                                                
};

/*==============================================================================
                           Global Variables
==============================================================================*/

/*==============================================================================
                           Local Function
==============================================================================*/
  
/*==============================================================================
                           Function Definitions
==============================================================================*/

/*******************************************************************************

FUNCTION NAME:
    XBPLATFORM_fnCheckErrors

PURPOSE:
    This function is used to check for errors that occur in the XanBus
    interface or below.  Typically these errors happen asynchonous to the
    application function calls.

INPUTS:
    None

OUTPUTS:
    none

NOTES:
    The application must supply this routine for XanBus to use

VERSION HISTORY:

Version: 1.00  Date: 02/18/05  By: Dalem
    - Created for testing

*******************************************************************************/

void XBPLATFORM_fnCheckErrors( void )
{
    // Read to clear latest error
    (void) XBINIT_fnGetErrorNum();
}

/*******************************************************************************

FUNCTION NAME:
    XBPLATFORM_fnSetNetIndic

PURPOSE:
    This function is called by the XanBus code to set the network indicator.

INPUTS:
    'teBusState' is XanBus network state
    'tucOn' is whether indicator should be on or off

OUTPUTS:
    none

NOTES:
    The application must supply this routine for XanBus to use

VERSION HISTORY:

Version: 1.00  Date: 02/18/05  By: Dalem
    - Created for testing

*******************************************************************************/

void XBPLATFORM_fnSetNetIndic( XB_teNETWORK_STATE teBusState, 
                               tucBOOL tucOn )
{
}

/*******************************************************************************

FUNCTION NAME:
    XBPLATFORM_fnLoad

PURPOSE:
    Load XanBus data either reading it from non-volatile storage or 
    from the program data.

INPUTS:
    'teType' is which XanBus specific type of data to be read
    'teCfgType' is User, Oem or Factory
    'pData' is pointer to location to store and return the data
    'uiSize' is number of bytes of data to be read

OUTPUTS:
    eSTATUS_OK if data is ok
    eSTATUS_ERR if data is not valid

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/18/05  By: Dalem
    - Created for testing

*******************************************************************************/

tuiSTATUS XBPLATFORM_fnLoad( XB_tePARAM_TYPE teType,
                             XB_teCFG_TYPE teCfgType,
                             void *pData,
                             uint16 uiSize )
{
    // Which EEProm ID depends on requested data type
    switch ( teType )
    {
        case XB_ePARAM_TYPE_ADDRESS:
        {
            assert( uiSize == sizeof( ucPrefAddr ) );

            memcpy( pData, &ucPrefAddr, uiSize );
        }
        break;

        case XB_ePARAM_TYPE_NAME:
        {
            // Record size must match name length
            assert( uiSize == sizeof( aucName ) );

            memcpy( pData, aucName, uiSize );
        }
        break;

        case XB_ePARAM_TYPE_TX_TIMEOUTS:
        {
            // Default list must be able to fit into data
            assert( uiSize >= ( sizeof( XB_tzSEND_TIMERS ) ) );

            memcpy( pData, &TEST_ktzDFLT_SEND_TMR, uiSize );

        } // case XB_ePARAM_TYPE_TX_TIMEOUTS:
        break;

        case XB_ePARAM_TYPE_RX_TIMEOUTS:
        {
            // Default list must be able to fit into data
            assert( uiSize >= ( sizeof( XB_tzRECV_TIMERS ) ) );

            memcpy( pData, &TEST_ktzDFLT_RECV_TMR, uiSize );

        } // case XB_ePARAM_TYPE_RX_TIMEOUTS:
        break;
            
        case XB_ePARAM_TYPE_NAME_CONFIG:
        {
            assert( FALSE );
        } // case XB_ePARAM_TYPE_NAME_CONFIG:
        break;

        default:
            assert( FALSE );
            // No data
            return eSTATUS_ERR;
    } // switch ( teType )
    return eSTATUS_OK;
}

/*******************************************************************************

FUNCTION NAME:
    XBPLATFORM_fnSave

PURPOSE:
    Save XanBus data to non-volatile storage.

INPUTS:
    'teType' is which XanBus specific type of data to be read
    'teCfgType' is User, Oem or Factory
    'pData' is pointer to location to store and return the data
    'uiSize' is number of bytes of data to be read

OUTPUTS:
    eSTATUS_OK if data was written
    eSTATUS_ERR if error occurred

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 02/18/05  By: Dalem
    - Created for testing

*******************************************************************************/

tuiSTATUS XBPLATFORM_fnSave( XB_tePARAM_TYPE teType,
                             XB_teCFG_TYPE teCfgType,
                             void *pData,
                             uint16 uiSize )

{
    // Which EEProm ID depends on requested data type
    switch ( teType )
    {
        case XB_ePARAM_TYPE_ADDRESS:
            assert( uiSize == sizeof( ucPrefAddr ) );
            memcpy( &ucPrefAddr, pData, uiSize );
            break;

        case XB_ePARAM_TYPE_NAME:
            assert( uiSize == sizeof( aucName ) );
            memcpy( aucName, pData, uiSize );
            break;

        case XB_ePARAM_TYPE_TX_TIMEOUTS:
        {
            // Default list must be able to fit into data
            assert( uiSize >= ( sizeof( XB_tzSEND_TIMERS ) ) );
            assert( FALSE );
        } // case XB_ePARAM_TYPE_TX_TIMEOUTS:
        break;

        case XB_ePARAM_TYPE_RX_TIMEOUTS:
        {
            // Default list must be able to fit into data
            assert( uiSize >= ( sizeof( XB_tzRECV_TIMERS ) ) );
            assert( FALSE );
        } // case XB_ePARAM_TYPE_RX_TIMEOUTS:
        break;

        case XB_ePARAM_TYPE_NAME_CONFIG:
        {
            assert( FALSE );
        } // case XB_ePARAM_TYPE_NAME_CONFIG:
        break;

        default:
            // No data
            assert( FALSE );
            return eSTATUS_ERR;
    }
    return eSTATUS_OK;
}

/*******************************************************************************

FUNCTION NAME:
    XBPLATFORM_fnNodeChange

PURPOSE:
    The XanBus layer calls this application function when it detects a change
    in node addresses. A node is New when the name and address pairing is
    first detected. A Bumped node is one which has lost the use of the address
    that it was previously paired with. A TimedOut node is one that has 
    stopped sending its address claim keep-alive message for the timeout
    period.

INPUTS:
    'teChange' is the type of the change detected
    'uOldcAddr' is the address before bumping
    'ucNewAddr' is the address of the new node
    'ptzName' is the name of the node being added or removed

OUTPUTS:
    none

NOTES:
    The application must supply this routine for XanBus to use

VERSION HISTORY:

Version: 1.00  Date: 02/18/05  By: Dalem
    - Created for testing

*******************************************************************************/
void XBPLATFORM_fnNodeChange( XB_teNODE_CHANGE teChange,          
                              uchar8 ucOldAddr,
                              uchar8 ucNewAddr,                      
                              XB_tzPGN_ISO_ADDR_CLAIM *ptzName )
{
    assert( ptzName != NULL );
}




/*******************************************************************************

FUNCTION NAME:
    XBPLATFORM_fnRecvXbMsg  - Pass XanBus handled message to application

PURPOSE:
    The XanBus layer calls this application function when an internal
    XanBus message has been received that may have interest to the application.
    This would include requested configurations for NAME config, Periodic
    Transmit Config and Timeout Config.

INPUTS:
    'ptzRecv' points to the XanBus message received

OUTPUTS:
    none

NOTES:
    The application must supply this routine for XanBus to use

VERSION HISTORY:

Version: 1.00  Date: 06/30/04  By: Dalem
    - Created - don't care about any messages XanBus handles for me

*******************************************************************************/

void XBPLATFORM_fnRecvXbMsg( PGN_tzRECV_DATA *ptzRecv )
{
    return;
}
