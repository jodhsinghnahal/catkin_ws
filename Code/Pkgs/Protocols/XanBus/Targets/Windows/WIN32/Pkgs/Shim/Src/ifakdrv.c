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
    ifak CAN to USB converter.

FUNCTION(S):
    local:
   
NOTES:

CHANGE HISTORY :
$Log: ifakdrv.c $

    ***********************************************
    Revision: XanBus3_TomL/2
    User: TomL     Date: 02/12/09  Time: 02:02AM
    Error messages go to stderr instead of stdout
==============================================================================*/

/*==============================================================================
                              Includes
==============================================================================*/

/* The isCAN interface needs definitions from <windows.h> but some definitions
   conflict with the Teleflex defines, so only make the definitions that are
   needed just before including the isCAN interface files.
*/
#ifndef BYTE
#define BYTE    unsigned char
#endif

#ifndef BOOLEAN
#define BOOLEAN BYTE
#endif

#ifndef WORD
#define WORD    unsigned short
#endif

#ifndef DWORD
#define DWORD   unsigned long
#endif

#ifndef HANDLE
#define HANDLE  int
#endif

#include <stdio.h>
#include <string.h>
#include "isCANdrv.h"       // ifak can driver interface
#include "isCANext.h"       // ifak can driver interface
#include "ifakdrv.h"

/*==============================================================================
                              Defines
==============================================================================*/

#define DEV_NUMBER              0

#define  CID_PRIORITY_BITOFFSET  26
#define  CID_RESERVED_BITOFFSET  25
#define  CID_DATAPAGE_BITOFFSET  24
#define  CID_FORMAT_BITOFFSET    16
#define  CID_SPECIFIC_BITOFFSET  8
#define  CID_SRCADDR_BITOFFSET   0

/*==============================================================================
                           Local/Private Variables
==============================================================================*/

static unsigned int gBitRate        = 0;

/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/

static void FrameToIfak( CANFRAME *pFrame, CAN_MessageEx_type *pCanMsg );
static void IfakToFrame( CAN_MessageEx_type *pCanMsg, CANFRAME *pFrame);
static char *ErrorString( uchar8 errcode );
static void IndicateError( schar8 *str, uchar8 errcode );

/*==============================================================================
                           Function Definitions
==============================================================================*/

/*******************************************************************************

FUNCTION NAME: 
    ErrorString
  
PURPOSE: 
    Convert isCAN_ error code to string

INPUTS: 
    'errcode' is error value from isCAN_ function

OUTPUTS:
    pointer to string for error code

NOTES:

VERSION HISTORY:

Version: 1.00  Date: OCT-06-2003  By: dalem
    - first version

*******************************************************************************/

static char *ErrorString( uchar8 errcode )
{
    char *estr;

    // Error string depends on errcode
    switch ( errcode )
    {
    case CAN_NO_ERROR:              estr = "no error";                  break;
    case CAN_OPEN_ERROR:            estr = "no access to device";       break;
    case CAN_DEVICE_NOT_FOUND:      estr = "device with ID not found";  break;
    case CAN_DRIVER_ERROR:          estr = "driver operation failed";   break;
    case CAN_INVALID_PARAMETER:     estr = "invalid parameter";         break;
    case CAN_DEVICE_NOT_ONLINE:     estr = "not online";                break;
    case CAN_DEVICE_TIMEOUT:        estr = "device timeout";            break;
    case CAN_TRANSMIT_BUFFER_BUSY:  estr = "tx buffer busy";            break;
    case CAN_RECEIVE_BUFFER_EMPTY:  estr = "rx buffer busy";            break;
    case CAN_THREAD_NOT_STARTED:    estr = "thread not started";        break;
    case CAN_THREAD_ALREADY_STARTED:estr = "thread already started";    break;
    case CAN_BUFFER_OVERRUN:        estr = "buffer overrun";            break;
    case CAN_DEVICE_NOT_INITIALIZED:estr = "not initialized";           break;
    case CAN_DEVICE_REMOVED:        estr = "device removed";            break;
    case CAN_DEVICE_ALREADY_IN_USE: estr = "already in use";            break;
    case CAN_BUS_ERROR:             estr = "bus error";                 break;
    case CAN_BUS_OFF:               estr = "bus off";                   break;
    case CAN_ERROR_PASSIVE:         estr = "bus error passive";         break;
    case CAN_DATA_OVERRUN:          estr = "data overrun";              break;
    case CAN_ERROR_WARNING:         estr = "bus error warning";         break;
    case CAN_SEND_ERROR:            estr = "send error";                break;
    case CAN_SEND_NO_ACK:           estr = "no bus acknowledgement";    break;
    case CAN_ERR_CRITICAL_BUS:      estr = "critical bus error";        break;
    case CAN_THREAD_IS_BLOCKED:     estr = "callbackthread is blocked"; break;
    case CAN_DEVICE_NOT_LICENCED:   estr = "need licence under NT4";    break;
    case CAN_ACCESS_DENIED:         estr = "no access rights";          break;
    default:                        estr = "unknown error";             break;
    }

    return estr;
}

/*******************************************************************************

FUNCTION NAME: 
    IndicateError
  
PURPOSE: 
    Check error from isCAN_ function

INPUTS: 
    'str' is comment string for error
    'errcode' is error value from isCAN_ function

OUTPUTS:
    nothing

NOTES:

VERSION HISTORY:

Version: 1.00  Date: OCT-06-2003  By: dalem
    - first version

*******************************************************************************/

static void IndicateError( schar8 *str, uchar8 errcode )
{
    static uchar8 lastError = CAN_NO_ERROR;
    char *estr;

    // If error has changed
    if ( errcode != lastError )
    {
        // Remember error
        lastError = errcode;

        // Convert error code to a string
        estr = ErrorString( errcode );

        if ( errcode != CAN_NO_ERROR )
        {
            fprintf( stderr, "%s: %s\n", str, estr );
        }
    }
}

/*******************************************************************************

FUNCTION NAME: 
    FrameToIfak
  
PURPOSE: 
    Translate CAN data from Teleflex frame format to CAN controller format

INPUTS: 
    'pFrame' is pointer to data in Teleflex frame format
    'pCanMsg' is pointer to hold return data formatted for CAN controller

OUTPUTS:
    returns nothing

NOTES:

VERSION HISTORY:

Version: 1.00  Date: DEC-18-2003  By: dalem
    - Created from ifak and CANcardX versions

*******************************************************************************/

static void FrameToIfak( CANFRAME *pFrame, CAN_MessageEx_type *pCanMsg )
{
    unsigned long msgId;
    unsigned char dataLen;

    // Get local copy of data length
    dataLen = pFrame->m_CanData.m_u8DataByteCount;

    // Initialize message ID before building
    msgId = 0;

    // Insert Priority
    msgId |= (unsigned long)(pFrame->m_CanData.m_u8Priority & 0x7) << CID_PRIORITY_BITOFFSET;
    // Insert DataPage
    msgId |= (unsigned long)(pFrame->m_CanData.m_u8DataPage & 0x1) << CID_DATAPAGE_BITOFFSET;
    // Insert Pdu Format
    msgId |= (unsigned long)(pFrame->m_CanData.m_u8PF & 0xff) << CID_FORMAT_BITOFFSET;
    // Insert Pdu Specific
    msgId |= (unsigned long)(pFrame->m_CanData.m_u8PS & 0xff) << CID_SPECIFIC_BITOFFSET; 
    // Insert Source Address
    msgId |= (unsigned long)(pFrame->m_CanData.m_u8SA & 0xff) << CID_SRCADDR_BITOFFSET;

    // Fill in isCAN message
    pCanMsg->MessageID = msgId;
    pCanMsg->bExtended = TRUE;
    pCanMsg->RemoteReq = FALSE;
    memcpy( pCanMsg->Data, pFrame->m_CanData.m_u8Data, dataLen );
    pCanMsg->DataLen = dataLen;
}

/*******************************************************************************

FUNCTION NAME: 
    IfakToFrame
  
PURPOSE: 
    Translate CAN data from CAN controller format to Teleflex frame format

INPUTS: 
    'pEvent' is pointer to data in CAN controller format
    'pFrame' is pointer to hold return data formatted in Teleflex frame format

OUTPUTS:
    returns nothing

NOTES:

VERSION HISTORY:

Version: 1.00  Date: DEC-18-2003  By: dalem
    - Created from ifak and CANcardX versions

*******************************************************************************/

static void IfakToFrame( CAN_MessageEx_type *pCanMsg, CANFRAME *pFrame)
{
    unsigned long msgId;
    unsigned char dataLen;

    // Get local copy of data length
    dataLen = pCanMsg->DataLen;

    // Get local copy of message ID clearing Vector extended frame bit
    msgId = pCanMsg->MessageID;

    // 29-bit extended ID
    // Extract DataPage
    pFrame->m_CanData.m_u8DataPage = (UINT8) ( ( msgId >> CID_DATAPAGE_BITOFFSET) & 0x01 );
    // Extract Priority
    pFrame->m_CanData.m_u8Priority = (UINT8) ( msgId >> CID_PRIORITY_BITOFFSET );
    // Extract PduSpecific
    pFrame->m_CanData.m_u8PS = (UINT8) ( msgId >> CID_SPECIFIC_BITOFFSET );
    // Extract PduFormat
    pFrame->m_CanData.m_u8PF = (UINT8) ( msgId >> CID_FORMAT_BITOFFSET );
    // Extract Source Address
    pFrame->m_CanData.m_u8SA = (UINT8) ( msgId >> CID_SRCADDR_BITOFFSET );
    // Extract Reserved ???
    //    tsPdu.Reserved = msgId >> CID_RESERVED_BITOFFSET;
    
    // Extract Data bytes 
    memcpy( pFrame->m_CanData.m_u8Data, pCanMsg->Data, dataLen );
    
    // Set data length
    pFrame->m_CanData.m_u8DataByteCount = dataLen;
}


tucBOOL IFAKDRV_fnInit( uchar8 ucBitRate )
{
    BYTE retval;
    DeviceInfo_type DevInfo;

    // Check for ifak USB
    retval = isCAN_AttachedDevices( FIND_ALL_DEVICES, &DevInfo );
    
    if ( retval == CAN_NO_ERROR )
    {
	if ( DevInfo.DeviceType == DEVTYPE_USB )
	{
	    // Open the driver
	    switch( ucBitRate )
	    { 
	    case CANDRV_eBIT_RATE_20KBPS:
		gBitRate = CAN_BAUD_20;
		retval = isCAN_DeviceInitEx( DEV_NUMBER, gBitRate );
		if ( retval == CAN_NO_ERROR )
		{
		    // Set the active interface
		    return TRUE;
		}

	    case CANDRV_eBIT_RATE_125KBPS:
		gBitRate = CAN_BAUD_125;
		retval = isCAN_DeviceInitEx( DEV_NUMBER, gBitRate );
		if ( retval == CAN_NO_ERROR )
		{
		    // Set the active interface
		    return TRUE;
		}
		
	    case CANDRV_eBIT_RATE_250KBPS:
		gBitRate = CAN_BAUD_250;
		retval = isCAN_DeviceInitEx( DEV_NUMBER, gBitRate );
		if ( retval == CAN_NO_ERROR )
		{
		    // Set the active interface
		    return TRUE;
		}

	    case CANDRV_eBIT_RATE_500KBPS:
		gBitRate = CAN_BAUD_500;
		retval = isCAN_DeviceInitEx( DEV_NUMBER, gBitRate );
		if ( retval == CAN_NO_ERROR )
		{
		    // Set the active interface
		    return TRUE;
		}
		
	    case CANDRV_eBIT_RATE_1MBPS:
		gBitRate = CAN_BAUD_1000;
		retval = isCAN_DeviceInitEx( DEV_NUMBER, gBitRate );
		if ( retval == CAN_NO_ERROR )
		{
		    // Set the active interface
		    return TRUE;
		}

	    default:
		// not supported
		fprintf( stderr, "Bit Rate not supported\r\n" );
	    }
	}
    }

    // No connecton to ifak interface
    return FALSE;
}

void IFAKDRV_fnClose( void )
{
    isCAN_CloseDevice( DEV_NUMBER );
}

TFXCAN_RETURNS IFAKDRV_fnStatus( void )
{
    // Bus is assumed to be active
    return TFXCR_OK;
}

/*******************************************************************************

FUNCTION NAME: 
    IFAKDRV_fnReceiveFrame
  
PURPOSE: 
    Poll for a received frame

INPUTS: 
    'u8PortNumber' is number representing the device hardware port
    'pFrame' is a pointer to copy CAN message into

OUTPUTS:
    returns TFXCR_INVALID_PORT if port is not supported
    returns TFXCR_NEW_DATA if data is read and ready
    returns TFXCR_NO_DATA if no data available
    returns TFXCR_MSG_NOT_HANDLED and prints diagnostic if some other error

NOTES:

VERSION HISTORY:

Version: 1.00  Date: DEC-18-2003  By: dalem
    - Created from ifak and CANcardX versions

*******************************************************************************/

TFXCAN_RETURNS IFAKDRV_fnReceiveFrame( CANPORT u8PortNumber,
                                                  CANFRAME *pFrame )
{
    BYTE retval;
    CAN_MessageEx_type CanMsg;

    // Try to receive a message
    retval = isCAN_ReceiveMessageEx( DEV_NUMBER, &CanMsg );

    // Check result of receive
    if ( retval != CAN_NO_ERROR )
    {
        if ( retval == CAN_RECEIVE_BUFFER_EMPTY )
        {
            return TFXCR_NO_DATA;
        }
        else
        {
            IndicateError( "receiving", retval );
            return TFXCR_MSG_NOT_HANDLED;
        }
    }
    else
    {
        // Change from ifak isCAN format to internal format
        IfakToFrame( &CanMsg, pFrame );

        // Data ready
        return ( TFXCR_NEW_DATA );
    }
}

/*******************************************************************************

FUNCTION NAME: 
    TFXDRV_i16SendFrame
  
PURPOSE: 
    Queue a CAN frame for transmission

INPUTS: 
    'u8PortNumber' is number representing the device hardware port
    'pFrame' is a pointer to the CAN data be be sent
    'pi16MessageID' is a pointer to field to return a buffer handle (NOT USED)

OUTPUTS:
    returns TFXCR_INVALID_PORT if port is invalid
    returns TFXCR_MSG_NOT_HANDLED if isCAN return an error
    returns TFXCR_OK if message queue for sending

NOTES:

VERSION HISTORY:

Version: 1.00  Date: DEC-18-2003  By: dalem
    - Created from ifak and CANcardX versions

*******************************************************************************/

TFXCAN_RETURNS IFAKDRV_fnSendFrame( CANPORT u8PortNumber,
                                               CANFRAME *pFrame,
                                               INT16 *pi16MessageID )
{
    BYTE retval;
    CAN_MessageEx_type CanMsg;

    // Teleflex message buffers not used
    *pi16MessageID = 1000;

    // Change from internal format to isCAN format
    FrameToIfak( pFrame, &CanMsg );

    // Transmit the message
    retval = isCAN_TransmitMessageEx( DEV_NUMBER, &CanMsg );

    // Check for error
    if ( retval != CAN_NO_ERROR )
    {
        // Give error indication and return failure
        IndicateError( "sending", retval );
        return TFXCR_MSG_NOT_HANDLED;
    }
    else
    {
        // Clear error indication
        IndicateError( "sending", retval );
    }

    // Success
    return TFXCR_OK;
}
