/*==============================================================================
Copyright © 2009 Xantrex Technology, Inc. All rights reserved.

This source file and the information contained in it are confidential and 
proprietary to Xantrex Technology, Inc. The reproduction or disclosure, in 
whole or in part, to anyone outside of Xantrex without the written approval of 
a Xantrex officer under a Non-Disclosure Agreement, or to any employee of 
Xantrex who has not previously obtained written authorization for access from 
the individual responsible for the source code, will have a significant 
detrimental effect on Xantrex and is expressly prohibited.

FILE NAME:  canusb.c

PURPOSE:
    This file provides the interface bewteen the XanBus shim and the
    APOX CAN to USB interface

FUNCTION(S):
    local:
   
NOTES:

CHANGE HISTORY :
$Log: apoxdrv.c $

    ***********************************************
    Revision: XanBus4_TomL/1
    User: TomL     Date: 02/22/10  Time: 09:42PM
    Updated to Visual C++ version 9 (2008).

    Added checks for null pointers
==============================================================================*/

/*==============================================================================
                              Includes
==============================================================================*/

#include <stdio.h>
#include "apox.h"
#include "apoxdrv.h"

/*==============================================================================
                           Function Definitions
==============================================================================*/

// Initialize the APOX dongle
tucBOOL APOXDRV_fnInit( uchar8 ucBitRate )
{
    if (APOX_fnListUSBCANBoards() > 0 )
    {
	if ( APOX_fnInstantiate( ucBitRate ) )
	{
	    return TRUE;
	}
    }

    return FALSE;
}

// Close the driver
void APOXDRV_fnClose( void )
{
    APOX_fnClose();
}


// Get the status of the APOX interface
TFXCAN_RETURNS APOXDRV_fnStatus( void )
{
    switch ( APOX_fnStatus() )
    {
    case APOX_STS_ERR_ACTIVE:
        return TFXCR_OK;
    
    case APOX_STS_ERR_PASSIVE:
        return TFXCR_BUS_WARNING;
    
    case APOX_STS_BUS_OFF:
    case APOX_STS_UNKNOWN:
    default:
        return TFXCR_BUS_OFF;
    }
}

// Receive a frame from the APOX
TFXCAN_RETURNS APOXDRV_fnReceiveFrame( CANPORT u8PortNumber,
                                       CANFRAME *pFrame )
{
    if ( pFrame == NULL )
    {
	return TFXCR_NO_DATA;
    }

    if ( APOX_fnReadCANMsg( &pFrame->m_CanData.m_u8DataPage,
                            &pFrame->m_CanData.m_u8Priority,
                            &pFrame->m_CanData.m_u8PS,
                            &pFrame->m_CanData.m_u8PF,
                            &pFrame->m_CanData.m_u8SA,
                            pFrame->m_CanData.m_u8Data,
                            &pFrame->m_CanData.m_u8DataByteCount ) != FALSE )
    {
        return ( TFXCR_NEW_DATA );
    }
    
    return TFXCR_NO_DATA;
}

// Send a frame to the APOX interface
TFXCAN_RETURNS APOXDRV_fnSendFrame( CANPORT u8PortNumber,
				    CANFRAME *pFrame,
				    INT16 *pi16MessageID )
{
    if (( pFrame == NULL ) || ( pi16MessageID == NULL ))
    {
	return TFXCR_MSG_NOT_HANDLED;
    }
    
    // Teleflex message buffers not used
    *pi16MessageID = 1000;

    if ( APOX_fnSendRawCANMessage(  pFrame->m_CanData.m_u8DataPage,
                                    pFrame->m_CanData.m_u8Priority,
                                    pFrame->m_CanData.m_u8PS,
                                    pFrame->m_CanData.m_u8PF,
                                    pFrame->m_CanData.m_u8SA,
                                    pFrame->m_CanData.m_u8Data,
                                    pFrame->m_CanData.m_u8DataByteCount ) != FALSE )
    {
        // Success
        return TFXCR_OK;
    }
    
    return TFXCR_MSG_NOT_HANDLED;
}

