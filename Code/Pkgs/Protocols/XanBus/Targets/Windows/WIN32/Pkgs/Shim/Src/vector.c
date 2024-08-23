/*==============================================================================
Copyright © 2009 Xantrex Technology, Inc. All rights reserved.

This source file and the information contained in it are confidential and 
proprietary to Xantrex Technology, Inc. The reproduction or disclosure, in 
whole or in part, to anyone outside of Xantrex without the written approval of 
a Xantrex officer under a Non-Disclosure Agreement, or to any employee of 
Xantrex who has not previously obtained written authorization for access from 
the individual responsible for the source code, will have a significant 
detrimental effect on Xantrex and is expressly prohibited.

FILE NAME:  vector.c

PURPOSE:
    This file provides the interface bewteen the XanBus shim and the
    Vector CANCard

FUNCTION(S):
    local:
   
NOTES:

CHANGE HISTORY :
$Log: vector.c $

    ***********************************************
    Revision: XanBus_HollyZ/2
    User: HollyZ     Date: 12/20/10  Time: 07:05PM
    When receiving the messages via CAN card, drop the frames while Over Run
    or Error Frame flag is set.

    ***********************************************
    Revision: XanBus_HollyZ/1
    User: HollyZ     Date: 05/03/10  Time: 10:18PM
    Changed the argument values while calling the vector API to look for a
    valid interface.

    ***********************************************
    Revision: XanBus3_TomL/2
    User: TomL     Date: 02/12/09  Time: 02:02AM
    Error messages go to stderr instead of stdout
==============================================================================*/

/*==============================================================================
                              Includes
==============================================================================*/

#include <stdio.h>
#include <string.h>
#include "vector.h"

#include "VCand.h"          // Vector Can driver interface

/*==============================================================================
                              Defines
==============================================================================*/

#define  CID_PRIORITY_BITOFFSET  26
#define  CID_RESERVED_BITOFFSET  25
#define  CID_DATAPAGE_BITOFFSET  24
#define  CID_FORMAT_BITOFFSET    16
#define  CID_SPECIFIC_BITOFFSET  8
#define  CID_SRCADDR_BITOFFSET   0

/*==============================================================================
                           Local/Private Variables
==============================================================================*/

// Vector variables
unsigned int gHwType         = HWTYPE_CANCARDX;
VportHandle  gPortHandle     = INVALID_PORTHANDLE;
Vaccess      gChannelMask    = 0;
Vaccess      gPermissionMask = 0;
unsigned int gBitRate        = 0;
int          gHwChannel      = 1;

// Last reported bus status
static uchar8 ucBusStatus = 0;

/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/

static void FrameToVector( CANFRAME *pFrame, Vevent *pEvent );
static void VectorToFrame( Vevent *pEvent, CANFRAME *pFrame);


/*==============================================================================
                           Function Definitions
==============================================================================*/

// Initialize the CANCard
tucBOOL VECTOR_fnInit( uchar8 ucBitRate )
{
    Vstatus vErr;
    VsetAcceptance acc;

    // Try opening the Vector driver
    vErr = ncdOpenDriver();
    // If no error
    if ( vErr == VSUCCESS )
    {
	// Select Channel, -1 for any card type, and index, any channel
        gChannelMask = ncdGetChannelMask( -1, -1, -1 );

	// If channel mask not in error
	if ( gChannelMask != 0x80000000 )
	{
	    // Open a port
	    vErr = ncdOpenPort( &gPortHandle, "XanBus",
				gChannelMask, gChannelMask, &gPermissionMask, 128 );
	    // If no error
	    if ( vErr == VSUCCESS )
	    {
		// If permission to initialize the channel
		if ( gPermissionMask )
		{
		    switch( ucBitRate )
		    { 
		    case CANDRV_eBIT_RATE_20KBPS:
			gBitRate = 20000;
			break;
		    case CANDRV_eBIT_RATE_125KBPS:
			gBitRate = 125000;
			break;
		    case CANDRV_eBIT_RATE_250KBPS:
			gBitRate = 250000;
			break;
		    case CANDRV_eBIT_RATE_500KBPS:
			gBitRate = 500000;
			break;                     
		    case CANDRV_eBIT_RATE_1MBPS:
			gBitRate = 1000000;
			break;                     
		    default:
			gBitRate = 0;
			fprintf( stderr, "Bit Rate not supported\r\n" );
			break;
		    }
		    if( gBitRate != 0 )
		    {
                        // Set the bus timing
                        vErr = ncdSetChannelBitrate( gPortHandle,
                                                     gPermissionMask,
                                                     gBitRate );
                        // If no error
                        if ( vErr == VSUCCESS )
                        {
                            // Disable the Tx and TXRQ notifications
                            vErr = ncdSetChannelMode( gPortHandle,
                                                      gChannelMask,
                                                      1,
                                                      0 );
                            // If no error
                            if ( vErr == VSUCCESS )
                            {
                                // Set acceptance filter
                                acc.mask = 0x80000000;
                                acc.code = 0x80000000;
                                vErr = ncdSetChannelAcceptance( gPortHandle,
                                                                gChannelMask,
                                                                &acc );
                                // If no error
                                if ( vErr == VSUCCESS )
                                {
                                    // Go on the bus
                                    vErr = ncdActivateChannel( gPortHandle,
                                                               gChannelMask );
                                    // If no error
                                    if ( vErr == VSUCCESS )
                                    {
                                        // Request a chip state report from
                                        // the controller to set bus state
                                        (void) ncdRequestChipState( gPortHandle,
                                                                    gChannelMask );
                                        
                                        // Set the active interface
                                        return TRUE;
                                    }
                                }
                            }
                        }
                    }
		}
	    }
	}
    }

    // Clean up if possible
    if ( gPortHandle != INVALID_PORTHANDLE )
    {
	ncdClosePort( gPortHandle );
	gPortHandle = INVALID_PORTHANDLE;
    }

    // Connection could not be made
    return FALSE;
}


// Close the connection to the CANCard
void VECTOR_fnClose( void )
{
    ncdDeactivateChannel( gPortHandle, gChannelMask );
    ncdClosePort( gPortHandle );
    gPortHandle = INVALID_PORTHANDLE;
    ncdCloseDriver();
}


// Get the status of the CANCard
TFXCAN_RETURNS VECTOR_fnStatus( void )
{
    // Check the saved buss status
    if ( ucBusStatus & CHIPSTAT_BUSOFF )
    {
	// Bus is off
	return TFXCR_BUS_OFF;
    }
    else if ( ucBusStatus & CHIPSTAT_ERROR_ACTIVE )
    {
	// Bus is active
	return TFXCR_OK;
    }
    else
    {
	// Bus is error warning or error passive
	return TFXCR_BUS_WARNING;
    }
}

/*******************************************************************************

FUNCTION NAME: 
    VECTOR_fnSendFrame
  
PURPOSE: 
    Queue a CAN frame for transmission

INPUTS: 
    'u8PortNumber' is number representing the device hardware port
    'pFrame' is a pointer to the CAN data be be sent
    'pi16MessageID' is a pointer to field to return a buffer handle (NOT USED)

OUTPUTS:
    returns TFXCR_INVALID_PORT if port is invalid
    returns TFXCR_MSG_NOT_HANDLED if CANcardX buffers are full;
    returns TFXCR_OK if message queue for sending
    returns TFXCR_TYPE_ERROR and prints diagnostic for other errors

NOTES:

VERSION HISTORY:

Version: 1.00  Date: DEC-18-2003  By: dalem
    - Created from ifak and CANcardX versions

*******************************************************************************/

TFXCAN_RETURNS VECTOR_fnSendFrame( CANPORT u8PortNumber,
				   CANFRAME *pFrame,
				   INT16 *pi16MessageID )
{
    Vevent event;
    Vstatus vErr;

    // Teleflex message buffers not used
    *pi16MessageID = 1000;

    // Change from internal format to Vector CANcardX format
    FrameToVector( pFrame, &event );

    // Transmit the message
    vErr =  ncdTransmit( gPortHandle, gChannelMask, &event );

    // Check for error
    if ( vErr )
    {
        if ( vErr == VERR_QUEUE_IS_FULL )
        {
            return TFXCR_DRV_TX_BUFFER_ALLOCATION_FAILURE;
        }
        else
        {
            fprintf( stderr, "ERROR: %s!\n", ncdGetErrorString( vErr ) );
            return TFXCR_MSG_NOT_HANDLED;
        }
    }

    // Success
    return TFXCR_OK;
}

/*******************************************************************************

FUNCTION NAME: 
    VECTOR_fnReceiveFrame
  
PURPOSE: 
    Initialize buffers for holding CAN messages.
    NOT USED BY CANcardX

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

TFXCAN_RETURNS VECTOR_fnReceiveFrame( CANPORT u8PortNumber,
				      CANFRAME *pFrame )
{
    Vevent *pEvent;
    Vstatus vErr;

    // Try to receive a message
    vErr = ncdReceive1( gPortHandle, &pEvent );

    // Check result of receive
    if ( vErr )
    {
        if ( vErr == VERR_QUEUE_IS_EMPTY )
        {
            return TFXCR_NO_DATA;
        }
        else
        {
            fprintf( stderr, "ERROR: %s!\n", ncdGetErrorString( vErr ) );
            return TFXCR_MSG_NOT_HANDLED;
        }
    }
    else
    {
        // Is the event a receive message event
        if ( pEvent->tag == V_RECEIVE_MSG )
        {
			if( ( pEvent->tagData.msg.flags & 0x03 ) ==  0 )
			{
                 // Change from Vector CANcardX format to internal format
                 VectorToFrame( pEvent, pFrame );
    
		        // Data ready
                return ( TFXCR_NEW_DATA );
			}
			else
			{
                return TFXCR_NO_DATA;
			}
			
        }
        else if ( pEvent->tag == V_CHIP_STATE )
        {
            // Save last reported bus status
            ucBusStatus = pEvent->tagData.chipState.busStatus;

            // Some other event, not a received message
            return TFXCR_NO_DATA;
        }
        else
        {
            // Some other event, not a received message
            printf( "Some other event %d\n", pEvent->tag );

            // Some other event, not a received message
            return TFXCR_NO_DATA;
        }
    }
}

/*******************************************************************************

FUNCTION NAME: 
    FrameToVector
  
PURPOSE: 
    Translate CAN data from Teleflex frame format to CAN controller format

INPUTS: 
    'pFrame' is pointer to data in Teleflex frame format
    'pEvent' is pointer to hold return data formatted for CAN controller

OUTPUTS:
    returns nothing

NOTES:

VERSION HISTORY:

Version: 1.00  Date: DEC-18-2003  By: dalem
    - Created from ifak and CANcardX versions

*******************************************************************************/

static void FrameToVector( CANFRAME *pFrame, Vevent *pEvent )
{
    unsigned long msgId;
    unsigned char dataLen;

    // Get local copy of data length
    dataLen = pFrame->m_CanData.m_u8DataByteCount;

    // Initialize message ID before building
    msgId = 0;

    // Vector needs high bit set for extended CAN frames
    msgId |= VCAN_EXT_MSG_ID;

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

    // Fill in message event
    pEvent->tag = V_TRANSMIT_MSG;
    pEvent->tagData.msg.id = msgId;
    pEvent->tagData.msg.flags = 0;
    memcpy( pEvent->tagData.msg.data, pFrame->m_CanData.m_u8Data, dataLen );
    pEvent->tagData.msg.dlc = dataLen;
}

/*******************************************************************************

FUNCTION NAME: 
    VectorToFrame
  
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

static void VectorToFrame( Vevent *pEvent, CANFRAME *pFrame)
{
    unsigned long msgId;
    unsigned char dataLen;

    // Get local copy of data length
    dataLen = pEvent->tagData.msg.dlc;

    // Get local copy of message ID clearing Vector extended frame bit
    msgId = pEvent->tagData.msg.id & ~VCAN_EXT_MSG_ID;

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
    memcpy( pFrame->m_CanData.m_u8Data, pEvent->tagData.msg.data, dataLen );
    
    // Set data length
    pFrame->m_CanData.m_u8DataByteCount = dataLen;
}
