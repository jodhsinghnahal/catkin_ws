/*=============================================================================
Copyright © 2009 Xantrex International

This file is the property of Xantrex International and shall not be reproduced,
copied, or used as the basis for the manufacture or sale of equipment without
the express written permission of Xantrex International.

FILE NAME:  xbwincb.c

PURPOSE:
    This file holds platform specific subroutines required for the XanBus
    interface. The functions herein are the counterparts to the functions
    in xbplatform.c of the XB API except for the setup function fnInitCB.
    This file is specific to the windows XB DLL implementation.

FUNCTION(S):
    XBWIN_fnCheckErrors - Check for saved errors
    XBWIN_fnSetNetIndic - Set network indicator
    XBWIN_fnLoad        - Get starting data required for XanBus
    XBWIN_fnSave        - Store XanBus starting data
    XBWIN_fnNodeChange  - A node has been added or dropped
    XBWIN_fnInitCB      - Initialize callbacks

    local:
        none

NOTES:
    This driver does no buffering
    of frames aside from what is implemented in hardware.
    There is also no use or assumption of any operating system services.

HISTORY:
$Log:$

==============================================================================*/
    
/*==============================================================================
                              Includes
==============================================================================*/

// System include files

#include <stdio.h>
#include <string.h>
#include "xbudefs.h"
#include "xbgdefs.h"
#include "xbwincb.h"

// Debug trace switch
tucBOOL XANBUS_ucTrace = TRUE;

/*==============================================================================
                           Local/Private Constants
==============================================================================*/

// Net address we try to use first
#define XB_PREF_NET_ADDR    1

/*
    Defines for making up default node NAME
*/
#define UNIQUE_NUMBER   ( 0x1FFFFF )            // Unique Number (serial number)
#define MANU_CODE       ( XB_MFG_XANTREX )      // Manufacturer Code
#define INST_LOWER      ( 7 )                   // Instance Lower, fill with 1s
#define INST_UPPER      ( 0 )                   // Instance Upper, should be 0
#define DEVICE_FUNC     ( XB_eFUNC_CODE_DIAGNOSTIC_TERMINAL )// Device Function
#define RESERVED_BIT    ( 0 )                   // Reserved Bit, must be zero
#define DEVICE_CLASS    ( XB_eDEVICE_CLASS_POWER_MANAGEMENT ) // Device Class
#define SYSTEM_INST     ( XB_SYSTEM_INSTANCE )  // System Instance
#define INDUSTRY_GROUP  ( XB_INDUSTRY_GROUP )   // Industry Group
#define SELF_ADDRESS    ( 1 )                   // Self Config Address, must be 1

/*==============================================================================
                           Local/Private Variables
==============================================================================*/

// Long form of my address claim
static XB_tzPGN_ISO_ADDR_CLAIM tzDefaultName =
{
    UNIQUE_NUMBER,
    XB_MFG_XANTREX,
    INST_LOWER,
    INST_UPPER,
    DEVICE_FUNC,
    RESERVED_BIT,
    DEVICE_CLASS,
    SYSTEM_INST,
    INDUSTRY_GROUP,
    SELF_ADDRESS
};

XB_tzSEND_TIMERS atzDefaultTxTimers =
{
    0,
    {
        { XB_PGN_ISO_ADDR_CLAIM,    PGN_ADDR_GLOBAL,    100 },
        { XB_PGN_STS,               PGN_ADDR_GLOBAL,    25 },
        { 0xFFFFFFFF,               0xFF,               0xFFFF },
        { 0xFFFFFFFF,               0xFF,               0xFFFF },
        { 0xFFFFFFFF,               0xFF,               0xFFFF },
        { 0xFFFFFFFF,               0xFF,               0xFFFF },
        { 0xFFFFFFFF,               0xFF,               0xFFFF },
        { 0xFFFFFFFF,               0xFF,               0xFFFF },
        { 0xFFFFFFFF,               0xFF,               0xFFFF },
        { 0xFFFFFFFF,               0xFF,               0xFFFF }
    }
};

// This is the default list of messages that can have a receive timeout
XB_tzRECV_TIMERS atzDefaultRxTimers =
{
    0,
    {
        { XB_PGN_ISO_ADDR_CLAIM,    500 },
        { XB_PGN_STS,               100 },
        { 0xFFFFFFFF,               0xFFFF },
        { 0xFFFFFFFF,               0xFFFF },
        { 0xFFFFFFFF,               0xFFFF },
        { 0xFFFFFFFF,               0xFFFF },
        { 0xFFFFFFFF,               0xFFFF },
        { 0xFFFFFFFF,               0xFFFF },
        { 0xFFFFFFFF,               0xFFFF },
        { 0xFFFFFFFF,               0xFFFF }
    }
};

/*******************************************************************************

FUNCTION NAME:
    XBWIN_fnCheckErrors

PURPOSE:
    This function is used to check for errors that occur in the XanBus
    interface or below.  Typically these errors happen asynchonous to the
    application function calls.

INPUTS:
    'pucTag' is pointer to an extra string to print

OUTPUTS:
    none

NOTES:
    The application must supply this routine for XanBus to use

*******************************************************************************/

void XBWIN_fnCheckErrors( schar8 *pscTag )
{
    if ( XANBUS_ucTrace )
    {
        printf( "CheckErrors: %s\n", pscTag );
    }
}

/*******************************************************************************

FUNCTION NAME:
    XBWIN_fnSetNetIndic

PURPOSE:
    This function is called by the XanBus code to set the network indicator.

INPUTS:
    'teCondition' is XanBus error condition code
    'tucOn' is whether indicator should be on or off

OUTPUTS:
    none

NOTES:
    The application must supply this routine for XanBus to use

*******************************************************************************/

void XBWIN_fnSetNetIndic( XB_teNETWORK_STATE teBusState, tucBOOL tucOn )
{
    const char *pcState;

    static XB_teNETWORK_STATE teOldState = ( XB_teNETWORK_STATE )0xFF;

    // If bus state has changed
    if ( teBusState != teOldState )
    {
	if ( XANBUS_ucTrace )
	{
	    switch ( teBusState )
	    {
	    case XB_eNETWORK_STATE_ERROR_ACTIVE:
		pcState = "Error Active";
		break;

	    case XB_eNETWORK_STATE_ERROR_PASSIVE:
		pcState = "Error Passive";
		break;

	    case XB_eNETWORK_STATE_BUS_OFF:
		pcState = "Bus Off";
		break;

	    default:
		pcState = "Unknown";
	    }

	    printf( "SetNetIndic: %s\n", pcState );
	}

        // Save current state
        teOldState = teBusState;
    }
}

/*******************************************************************************

FUNCTION NAME:
    XBWIN_fnLoad

PURPOSE:
    Load XanBus data either reading it from non-volatile storage or 
    from the program data.

INPUTS:
    'teType' is which XanBus specific type of data to be read
    'teCfgType' is the configuration type of the data
    'pData' is pointer to location to store and return the data
    'uiSize' is number of bytes of data to be read

OUTPUTS:
    eSTATUS_OK if data is ok
    eSTATUS_ERR if data is not valid

NOTES:

*******************************************************************************/

tuiSTATUS XBWIN_fnLoad( XB_tePARAM_TYPE teType,
                        XB_teCFG_TYPE teCfgType,
                        void *pData,
                        uint16 uiSize )
{
    uchar8 *pucData;
    uchar8 ucNetAddr;
    uchar8 aucShortNAME[ PGN_NAME_LEN ];

    // Set pointer to NULL as default
    pucData = NULL;

    // Which NV ID depends on requested data type
    switch ( teType )
    {
        case XB_ePARAM_TYPE_ADDRESS:
            // If size is right for preferred address
            if ( uiSize == 1 )
            {
                // Set preferred address and pointer to it
                ucNetAddr = XB_PREF_NET_ADDR;
                pucData = &ucNetAddr;
            }
            break;

        case XB_ePARAM_TYPE_NAME:
            // Convert name to packed format
            XBADDR_fnNAME2ShortNAME( &tzDefaultName, aucShortNAME );

            // If size is right for node NAME
            if ( uiSize == PGN_NAME_LEN )
            {
                // Set pointer to node NAME
                pucData = aucShortNAME;
            }
            break;

        case XB_ePARAM_TYPE_TX_TIMEOUTS:
            // If size matches our default Periodic Transmit data
            if ( uiSize == sizeof( atzDefaultTxTimers ) )
            {
                // Use default periodic transmit timers
                pucData = (uchar8 *) &atzDefaultTxTimers;
            }
            break;

        case XB_ePARAM_TYPE_RX_TIMEOUTS:
            // If size matches our default Receive Timeout data
            if ( uiSize == sizeof( atzDefaultRxTimers ) )
            {
                // Use default Receive timeouts
                pucData = (uchar8 *) &atzDefaultRxTimers;
            }
            break;

        default:
            // No data
            break;
    }

    // If entry found
    if ( pucData )
    {
        // Copy the data to the given pointer
        (void) memcpy( pData, pucData, uiSize );

        // Data found
        return ( eSTATUS_OK );
    }
    else
    {
        // Data not found
        return ( eSTATUS_ERR );
    }
}

/*******************************************************************************

FUNCTION NAME:
    XBWIN_fnSave

PURPOSE:
    Save XanBus data to non-volatile storage.

INPUTS:
    'teType' is which XanBus specific type of data to be read
    'teCfgType' is the configuration type of the data
    'pData' is pointer to location to store and return the data
    'uiSize' is number of bytes of data to be read

OUTPUTS:
    eSTATUS_OK if data was written
    eSTATUS_ERR if error occurred

NOTES:

*******************************************************************************/

tuiSTATUS XBWIN_fnSave( XB_tePARAM_TYPE teType,
                        XB_teCFG_TYPE teCfgType,
                        void *pData,
                        uint16 uiSize )
{
    // Write complete
    return ( eSTATUS_OK );
}

/*******************************************************************************

FUNCTION NAME:
    XBP_fnNodeChange

PURPOSE:
    The XanBus layer calls this application function when it detects a change
    in node addresses. A node is New when the NAME and address pairing is
    first detected. A Bumped node is one which has lost the use of the address
    that it was previously paired with. A TimedOut node is one that has 
    stopped sending its address claim keep-alive message for the timeout
    period.

INPUTS:
    'teChange' is the type of the change detected
    'ucOldAddr' is the node's old address
    'ucNewAddr' is the node's new address
    'ptzNAME' is the NAME of the node being added or removed

OUTPUTS:
    none

NOTES:
    The application must supply this routine for XanBus to use

*******************************************************************************/

void XBWIN_fnNodeChange( XB_teNODE_CHANGE teChange,
                         uchar8 ucOldAddr,
                         uchar8 ucNewAddr,
                         XB_tzPGN_ISO_ADDR_CLAIM *ptzNAME )
{
    if ( XANBUS_ucTrace )
    {
	switch ( teChange )
	{
	case XB_eNODE_CHANGE_NEW_NODE:
	    printf( "New node %d\n", ucNewAddr );
	    break;

	case XB_eNODE_CHANGE_ADDRESS:
	    printf( "Node %d changed address to %d\n", ucOldAddr, ucNewAddr );
	    break;

	case XB_eNODE_CHANGE_BUMPED_OFF:
	    printf( "Node %d bumped off\n", ucOldAddr );
	    break;

	case XB_eNODE_CHANGE_TIMED_OUT:
	    printf( "Node %d timed out\n", ucOldAddr );
	    break;

	default:
	    printf( "Unknown node change %d\n", teChange );
	}
    }
}

/*******************************************************************************

FUNCTION NAME:
    XBWIN_fnRecvXbMsg

PURPOSE:
    The XanBus layer calls this application function when an ISO Requested
    NAME Configuration, Periodic Transmit Configuration or Receive Timeout 
    Configuration is received. This function is only useful for Diagnostic 
    and User Interface Nodes.

INPUTS:
    'ptzRecv' is the received data and header

OUTPUTS:
    none

NOTES:
    The application must supply this routine for XanBus to use

*******************************************************************************/

void XBWIN_fnRecvXbMsg( PGN_tzRECV_DATA *ptzRecv )
{
    if ( XANBUS_ucTrace )
    {
	printf( "_xanbus: RecvXbMsg\n" );
    }
}

/*******************************************************************************

FUNCTION NAME:
    XBWIN_fnSetName

PURPOSE:
    Set the device function and instance for the address claim

INPUTS:
    DevInstLower - Lower field of the device instance
    DevInstUpper - Upper field of the device instance
    DevFunc      - Device function code

OUTPUTS:
    none

NOTES:

*******************************************************************************/

void XBWIN_fnSetName( uchar8 DevInstLower, uchar8 DevInstUpper, XB_teFUNC_CODE DevFunc )
{
    tzDefaultName.ucDeviceInstanceLower = DevInstLower;
    tzDefaultName.ucDeviceInstanceUpper = DevInstUpper;
    tzDefaultName.teDeviceFunction = DevFunc;
}

/*******************************************************************************

FUNCTION NAME:
    XBWIN_fnInitCB

PURPOSE:
    Initailize all the XanBus Platform callbacks.
    
INPUTS:
    none
    
OUTPUTS:
    none

NOTES:
    
*******************************************************************************/

void XBWIN_fnInitCB( void )
{
    XBWIN_fnSetCheckErrorsCB( XBWIN_fnCheckErrors );
    XBWIN_fnSetSetNetIndicCB( XBWIN_fnSetNetIndic );
    XBWIN_fnSetLoadCB( XBWIN_fnLoad );
    XBWIN_fnSetSaveCB( XBWIN_fnSave );
    XBWIN_fnSetNodeChangeCB( XBWIN_fnNodeChange );
    XBWIN_fnSetRecvXbMsgCB( XBWIN_fnRecvXbMsg );
}
