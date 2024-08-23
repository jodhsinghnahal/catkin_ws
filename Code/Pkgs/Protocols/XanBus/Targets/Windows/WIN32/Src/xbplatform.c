/*=============================================================================
Copyright � 2003 Xantrex Technology, Inc. All rights reserved.

This source file and the information contained in it are confidential and 
proprietary to Xantrex Technology, Inc. The reproduction or disclosure, in 
whole or in part, to anyone outside of Xantrex without the written approval of 
a Xantrex officer under a Non-Disclosure Agreement, or to any employee of 
Xantrex who has not previously obtained written authorization for access from 
the individual responsible for the source code, will have a significant 
detrimental effect on Xantrex and is expressly prohibited.

FILE NAME:  xbplatform.c

PURPOSE:
    This file holds platform specific subroutines required for the XanBus
    interface. This file is specific to the windows XB DLL implementation.

FUNCTION(S):
    XBPLATFORM_fnCheckErrors  - Check for system errors
    XBPLATFORM_fnSetNetIndic  - Set network indicator
    XBPLATFORM_fnLoad         - Get starting data required for XanBus
    XBPLATFORM_fnSave         - Store XanBus starting data
    XBPLATFORM_fnNodeChange   - A node has been added or dropped
    XBPLATFORM_fnInitCritical - Initiate the critical section
    XBPLATFORM_fnEnterCritical- Request ownership of the critical section
    XBPLATFORM_fnLeaveCritical- release ownership of the critical section
    XBPLATFORM_fnDeleteCritical-Release the resource used by the critical section
    
	local:
        none


    
/*==============================================================================
                              Includes
==============================================================================*/

// Windows header file
#include "windows.h"
#include <stdio.h>

// XanBus include files
#include "xbudefs.h"
#include "xbgdefs.h"
#include "rvcudefs.h"
#include "xbwincb.h"

// For direct communication to CAN driver
#include "isCANdrv.h"
#include "isCANext.h"

// critical section object
CRITICAL_SECTION CriticalSection;

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
    'pucTag' is pointer to an extra string to print

OUTPUTS:
    none

NOTES:
    The application must supply this routine for XanBus to use

VERSION HISTORY:

Version: 1.00  Date: 03/31/03  By: Dalem
    - Created from example code
Version: 1.01  Date: 04/08/03  By: Dalem
    - Added callback for windows
Version 1.02  Date: 06/26/03  By: Dalem
    - Reflects major change to XB API

*******************************************************************************/

void ( *fnCheckErrorsCB ) ( schar8 *pscTag );

void XBWIN_fnSetCheckErrorsCB( void (*fn) ( schar8 *pscTag ) )
{
    fnCheckErrorsCB = fn;
}

void XBPLATFORM_fnCheckErrors()
{
    // If function pointer is set
    if ( fnCheckErrorsCB != NULL )
    {
        // Call function
         ( *fnCheckErrorsCB ) ( NULL );
    }
}

/*******************************************************************************

FUNCTION NAME:
    XBPLATFORM_fnSetNetIndic

PURPOSE:
    This function is called by the XanBus code to set the network indicator.

INPUTS:
    'teCondition' is XanBus error condition code
    'tucOn' is whether indicator should be on or off

OUTPUTS:
    none

NOTES:
    The application must supply this routine for XanBus to use

VERSION HISTORY:

Version: 1.00  Date: 03/31/03  By: Dalem
    - Created from example code
Version: 1.01  Date: 04/08/03  By: Dalem
    - Added callback for windows
Version 1.02  Date: 06/26/03  By: Dalem
    - Reflects major change to XB API

*******************************************************************************/

void ( *fnSetNetIndicCB ) ( XB_teNETWORK_STATE teBusState,
                            tucBOOL tucOn );

void XBWIN_fnSetSetNetIndicCB( void (*fn) ( XB_teNETWORK_STATE teBusState,
                                            tucBOOL tucOn ) )
{
    fnSetNetIndicCB = fn;
}

void XBPLATFORM_fnSetNetIndic( XB_teNETWORK_STATE ucBusState,
                               tucBOOL tucOn )
{
    // If function pointer is set
    if ( fnSetNetIndicCB != NULL )
    {
        // Call function
         ( *fnSetNetIndicCB ) ( ucBusState, tucOn );
    }
}

/*******************************************************************************

FUNCTION NAME:
    XBPLATFORM_fnLoad

PURPOSE:
    Load XanBus data either reading it from non-volatile storage or 
    from the program data.

INPUTS:
    'teType' is which XanBus specific type of data to be read
    'teCfgType' is the type of the configuration data
    'pData' is pointer to location to store and return the data
    'uiSize' is number of bytes of data to be read

OUTPUTS:
    eSTATUS_OK if data is ok
    eSTATUS_ERR if data is not valid

NOTES:

VERSION HISTORY:

Version 1.00  Date: 06/26/03  By: Dalem
    - Reflects major change to XB API
Version 1.01  Date: 08/19/03  By: Peterd
    - Added configuration type parameter

*******************************************************************************/

tuiSTATUS ( *fnLoadCB )( XB_tePARAM_TYPE teType,
                         XB_teCFG_TYPE teCfgType,
                         void *pData,
                         uint16 uiSize );

void XBWIN_fnSetLoadCB( tuiSTATUS (*fn)( XB_tePARAM_TYPE teType,
                                         XB_teCFG_TYPE teCfgType,
                                         void *pData,
                                         uint16 uiSize ) )
{
    fnLoadCB = fn;
}

tuiSTATUS XBPLATFORM_fnLoad( XB_tePARAM_TYPE teType,  // Type of data to load
                             XB_teCFG_TYPE teCfgType, // Configuration type
                             void *pData,             // Pointer to put data
                             uint16 uiSize )
{
    // If function pointer is set
    if ( fnLoadCB != NULL )
    {
        // Call function
        // Call function
         return ( ( *fnLoadCB ) ( teType, 
                                  teCfgType,
                                  pData,
                                  uiSize ) );
    }
    else
    {
        return ( eSTATUS_ERR );
    }
}

/*******************************************************************************

FUNCTION NAME:
    XBPLATFORM_fnSave

PURPOSE:
    Save XanBus data to non-volatile storage.

INPUTS:
    'teType' is which XanBus specific type of data to be read
    'teCfgType' is the type of the configuration data
    'pData' is pointer to location to store and return the data
    'uiSize' is number of bytes of data to be read

OUTPUTS:
    eSTATUS_OK if data was written
    eSTATUS_ERR if error occurred

NOTES:

VERSION HISTORY:

Version 1.00  Date: 06/26/03  By: Dalem
    - Reflects major change to XB API
Version 1.01  Date: 08/19/03  By: Peterd
    - Added configuration type parameter

*******************************************************************************/

tuiSTATUS ( *fnSaveCB )( XB_tePARAM_TYPE teType,
                         XB_teCFG_TYPE teCfgType,
                         void *pData,
                         uint16 uiSize );

void XBWIN_fnSetSaveCB( tuiSTATUS (*fn)( XB_tePARAM_TYPE teType,
                                         XB_teCFG_TYPE teCfgType,
                                         void *pData,
                                         uint16 uiSize ) )
{
    fnSaveCB = fn;
}

tuiSTATUS XBPLATFORM_fnSave( XB_tePARAM_TYPE teType,  // Type of data to load
                             XB_teCFG_TYPE teCfgType, // Configuration type
                             void *pData,             // Pointer to data
                             uint16 uiSize )
{
    // If function pointer is set
    if ( fnSaveCB != NULL )
    {
        // Call function
         return ( ( *fnSaveCB ) ( teType, 
                                  teCfgType,
                                  pData,
                                  uiSize ) );
    }
    else
    {
        return ( eSTATUS_ERR );
    }
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
    'ucAddr' is the address of the new node
    'ptzNAME' is the NAME of the node being added or removed

OUTPUTS:
    none

NOTES:
    The application must supply this routine for XanBus to use

VERSION HISTORY:

Version: 1.00  Date: 04/11/03  By: Dalem
    - Created, supplants XBP_fnNewNode
Version 1.01  Date: 06/26/03  By: Dalem
    - Reflects major change to XB API

*******************************************************************************/

void (*fnNodeChangeCB)( XB_teNODE_CHANGE teChange,
                        uchar8 ucOldAddr,
                        uchar8 ucNewAddr,
                        XB_tzPGN_ISO_ADDR_CLAIM *ptzNAME );

void XBWIN_fnSetNodeChangeCB( void (*fn)( XB_teNODE_CHANGE teChange,
                                          uchar8 ucOldAddr,
                                          uchar8 ucNewAddr,
                                          XB_tzPGN_ISO_ADDR_CLAIM *ptzNAME ) )
{
    fnNodeChangeCB = fn;
}

void XBPLATFORM_fnNodeChange( XB_teNODE_CHANGE teChange,
                              uchar8 ucOldAddr,
                              uchar8 ucNewAddr,
                              XB_tzPGN_ISO_ADDR_CLAIM *ptzNAME )
{
    // If function pointer is set
    if ( fnNodeChangeCB != NULL )
    {
        // Call function
        ( *fnNodeChangeCB ) ( teChange, ucOldAddr, ucNewAddr, ptzNAME );
    }
}

/*******************************************************************************

FUNCTION NAME:
    XBPLATFORM_fnRecvXbMsg

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

VERSION HISTORY:

Version: 1.00  Date: 09/04/03  By: Peterd
    - Created

*******************************************************************************/

void (*fnRecvXbMsgCB)( PGN_tzRECV_DATA *ptzRecv );

void XBWIN_fnSetRecvXbMsgCB( void (*fn)( PGN_tzRECV_DATA *ptzRecv ) )
{
    fnRecvXbMsgCB = fn;
}

void XBPLATFORM_fnRecvXbMsg( PGN_tzRECV_DATA *ptzRecv )
{
    // If function pointer is set
    if ( fnRecvXbMsgCB != NULL )
    {
        // Call function
         ( *fnRecvXbMsgCB )( ptzRecv );
    }
}

// General purpose receive handler for PC-based applications
void (*fnRecvGeneralMsgCB)( PGN_tzRECV_DATA *ptzRecv );

void XBWIN_fnSetRecvGeneralMsgCB( void (*fn)( PGN_tzRECV_DATA *ptzRecv ) )
{
    fnRecvGeneralMsgCB = fn;
}

void XBWIN_fnRecvGeneralMsg( PGN_tzRECV_DATA *ptzRecv )
{
    if ( fnRecvGeneralMsgCB != NULL )
    {
        ( *fnRecvGeneralMsgCB )( ptzRecv );
    }
}

// Default diagnostic message
void XBWIN_fnWantDiagMsg1( PGN_tzWANT_DATA *ptzWant )
{
    RVC_tzPGN_DIAG_MSG1  DiagMsg;
    RVC_tzPGN_EXTRA_DIAG_MSG1 tzNoFltWrnEntry;

    // Product on and active
    DiagMsg.teOpStsProdOn = RVC_eGENERIC_STSPAIR_ON;
    DiagMsg.teOpStsProdActive = RVC_eGENERIC_STSPAIR_ON;


    // No faults or warnings to report
    // Send the 'no fault and warnings entry'
    // Yellow Lamp status
    DiagMsg.teOpStsYel          = RVC_eGENERIC_STSPAIR_OFF;

    // Red Lamp status
    DiagMsg.teOpStsRed          = RVC_eGENERIC_STSPAIR_OFF;

    // Identify as a service tool
    DiagMsg.teProdId = RVC_eDFLT_SRC_ADDR_SERVICE_TOOL;

    // Indicate no error conditions
    DiagMsg.uiExtraCount = 1;

    tzNoFltWrnEntry.ucSpnMsb = PGNHAN_UC_DATA_NOT_AVAILABLE;
    tzNoFltWrnEntry.ucSpnIsb = PGNHAN_UC_DATA_NOT_AVAILABLE;
    tzNoFltWrnEntry.ucSpnLsb = PGNHAN_UC_DATA_NOT_AVAILABLE;
    tzNoFltWrnEntry.teFmi = RVC_eSAEJ1939FMI_NO_FAILURE_INFORMATION_AVAILABLE;
    tzNoFltWrnEntry.ucOccurrenceCnt = PGNHAN_UC_DATA_NOT_AVAILABLE;
    tzNoFltWrnEntry.ucProdIdExt = PGNHAN_UC_DATA_NOT_AVAILABLE;

    DiagMsg.ptzExtra = (RVC_tzPGN_EXTRA_DIAG_MSG1 *)&tzNoFltWrnEntry;

    // Send a single frame PGN for this entry
    XBMSG_fnSend( RVC_PGN_DIAG_MSG1,
		  &DiagMsg,
		  sizeof(RVC_tzPGN_DIAG_MSG1),
		  ptzWant->ucDestAddr );
}

/*******************************************************************************

FUNCTION NAME:
    XBPLATFORM_fnInitCritical

PURPOSE:
    Allocate the memory used by a critical section

INPUTS:
    none

OUTPUTS:
    none

NOTES:
    
VERSION HISTORY:

Version: 1.00  Date: 06/21/05  By: Hollyz
    - Created

*******************************************************************************/
void XBPLATFORM_fnInitCritical( void )
{
    InitializeCriticalSection( &CriticalSection );
}
/*******************************************************************************

FUNCTION NAME:
    XBPLATFORM_fnDeleteCritical

PURPOSE:
    Release the resource used by a critical section

INPUTS:
    none

OUTPUTS:
    none

NOTES:
    
VERSION HISTORY:

Version: 1.00  Date: 06/21/05  By: Hollyz
    - Created

*******************************************************************************/
void XBPLATFORM_fnDeleteCritical( void )
{
    DeleteCriticalSection( &CriticalSection );
}
/*******************************************************************************

FUNCTION NAME:
    XBPLATFORM_fnEnterCritical

PURPOSE:
    The XanBus layer calls this application function when it requests ownership
	of the critical section for memory malloc/free operation on the Xanbus Heap.

INPUTS:
    none

OUTPUTS:
    none

NOTES:
    Only Windows' applications supply this routine for XanBus to use

VERSION HISTORY:

Version: 1.00  Date: 06/21/05  By: Hollyz
    - Created

*******************************************************************************/

void XBPLATFORM_fnEnterCritical( void )
{
    EnterCriticalSection( &CriticalSection );
}

/*******************************************************************************

FUNCTION NAME:
    XBPLATFORM_fnLeaveCritical

PURPOSE:
    The XanBus layer calls this application function when it releases ownership
	of the critical section for memory malloc/free operation on the Xanbus Heap.

INPUTS:
    none

OUTPUTS:
    none

NOTES:
    Only Windows' applications supply this routine for XanBus to use

VERSION HISTORY:

Version: 1.00  Date: 06/21/05  By: Hollyz
    - Created

*******************************************************************************/

void XBPLATFORM_fnLeaveCritical( void )
{
    LeaveCriticalSection( &CriticalSection );
}

// Send an arbitrary frame to the CAN interface
tucBOOL XBWIN_fnSendArbitraryFrame( uint32 ulCanId,
                                    tucBOOL ucExtended,
                                    uchar8* pucData,
                                    uchar8 ucDataLen )
{
    CAN_MessageEx_type tzMessage;
    uchar8 i;
    BYTE result;

    /* printf("XBWIN_fnSendArbitraryFrame %08x\r\n", ulCanId); */
    tzMessage.MessageID = (DWORD)ulCanId;
    tzMessage.bExtended = (BOOLEAN)ucExtended;
    tzMessage.RemoteReq = FALSE;
    tzMessage.DataLen = ucDataLen;
    for ( i = 0; i < ucDataLen; i++ )
    {
        tzMessage.Data[ i ] = pucData[ i ];
    }
    
    result = isCAN_TransmitMessageEx( 0, &tzMessage );

    return (result == CAN_NO_ERROR) ? TRUE : FALSE;
}
