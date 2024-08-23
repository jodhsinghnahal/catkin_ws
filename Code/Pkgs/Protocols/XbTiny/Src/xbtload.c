/*=============================================================================
  (c) 2003 - 2019 Xantrex Canada ULC. All rights reserved.
  All trademarks are owned or licensed by Xantrex Canada ULC,
  its subsidiaries or affiliated companies.

FILE NAME:  xbtload.c

PURPOSE:
    This file provides the interface between the XanBus Tiny drivers and
    the loader application for the download messages.

FUNCTION(S):
    XBTLOAD_fnRecvLoadInfo  - Handle File Download Info PGN
    XBTLOAD_fnRecvLoadBlock - Handle File Download Block PGN
    XBTLOAD_fnRecvResetCmd  - Handle Reset Command PGN
    XBTLOAD_fnReqLoadStatus - Handle request to send a Download Status PGN
    XBTLOAD_fnNextEvent     - Check for Download event
    XBTLOAD_fnSendLoadStatus- Send Download Status PGN

    local:
        xbtload_fnQueueEvent    - Add event and data to queue

=============================================================================*/

/*==============================================================================
                              Includes
==============================================================================*/

//#include <string.h>

#include "xbudefs.h"    // For message structures
#include "xbtgdefs.h"   // XB Tiny Global interface
#include "xbtldefs.h"   // XB Tiny Local interface
#include "xassert.h"    // for assert statements

/*==============================================================================
                              Defines
==============================================================================*/
/*
    Max number of elements in the event queue.
    One element for each message type: ISO ACK, ISO REQ, ISO Addr Claim,
    ISO Transport, DownLoad, and Reset.
*/
#define XBT_EVENT_Q_DEPTH   ( 6 )


// DL Info PGN Defines
//
// Value of the Reserved bits in the File Download Info PGN
#define XBTLOAD_DL_STS_RSV_BITS 0xF0


/*==============================================================================
                            Type Definitions
==============================================================================*/


/*==============================================================================
                           Local/Private Constants
==============================================================================*/


/*==============================================================================
                           Local/Private Variables
==============================================================================*/

// Data structure for queueing events
typedef struct zEVENT
{
    uchar8 ucEvent;         // Event identifier
    uchar8 ucDestAddr;      // Destination Address
    XBT_tzMSG *ptzMsg;      // Pointer to message
} tzEVENT;

static tzEVENT atzEventList[ XBT_EVENT_Q_DEPTH ];   // Event Queue
static uchar8 ucEventCount = 0;         // Number of events in queue
static uchar8 ucNextEvent = 0;          // Index to next active event

/*==============================================================================
                           Function Definitions
==============================================================================*/

/******************************************************************************

FUNCTION NAME:
    xbtload_fnQueueEvent

PURPOSE:
    Adds event and its data to the event queue

INPUTS:
    'ucEvent' - event type
    'ptzMsg'  - pointer to message data or NULL
    'ucDest'  - destination address or XBT_ADDR_NULL

OUTPUTS:
    None

NOTES:
    If the event queue is full the latest event is discarded

VERSION HISTORY:

Version 1.00  Date: 05/30/03  By: dalem
    - Created

******************************************************************************/
static void xbtload_fnQueueEvent( uchar8 ucEvent,
                                  XBT_tzMSG *ptzMsg,
                                  uchar8 ucDest )
{
    uchar8 ucInsert;
    tzEVENT *ptzEvt;

    assert( ucEventCount < DIM( atzEventList ) );

    // If there is room in event list
    if( ucEventCount < DIM( atzEventList ) )
    {
        // Get insert index
        ucInsert = ( ucNextEvent + ucEventCount ) % DIM( atzEventList );

        // Get pointer to event at insert index
        ptzEvt = &atzEventList[ ucInsert ];

        // Copy event data
        ptzEvt->ucEvent = ucEvent;
        ptzEvt->ptzMsg = ptzMsg;
        ptzEvt->ucDestAddr = ucDest;

        // Increment the events' count
        ucEventCount++;
    }
}

/******************************************************************************

FUNCTION NAME:
    XBTLOAD_fnRecvLoadInfo

PURPOSE:
    Handle reception of a File Download Info PGN

INPUTS:
    'ptzMsg' - pointer to received message data

OUTPUTS:
    None

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version
Version 1.01  Date: 03/28/03  By: dalem
    - Checking for Diag is done globally
Version 1.02  Date: 05/30/03  By: dalem
    - Just queue the message
    - Changed name of function

******************************************************************************/

void XBTLOAD_fnRecvLoadInfo( XBT_tzMSG *ptzMsg )
{
    // Save the event in the event queue
    xbtload_fnQueueEvent( XBT_LOAD_INFO_EVENT,
                          ptzMsg,
                          XBT_ADDR_NULL );
}

/******************************************************************************

FUNCTION NAME:
    XBTLOAD_fnRecvLoadBlock

PURPOSE:
    Handle reception of a File Download Block PGN

INPUTS:
    'ptzMsg' - pointer to received message data

OUTPUTS:
    None

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version
Version 1.01  Date: 03/28/03  By: dalem
    - Checking for Diag is done globally
Version 1.02  Date: 05/30/03  By: dalem
    - Just queue the message

******************************************************************************/

void XBTLOAD_fnRecvLoadBlock( XBT_tzMSG *ptzMsg )
{
    // Save the event in the event queue
    xbtload_fnQueueEvent( XBT_LOAD_DATA_EVENT,
                          ptzMsg,
                          XBT_ADDR_NULL );
}

/******************************************************************************

FUNCTION NAME:
    XBTLOAD_fnRecvResetCmd

PURPOSE:
    Handle reception of a Reset Command

INPUTS:
    'ptzMsg' - pointer to received message data

OUTPUTS:
    None

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version
Version 1.01  Date: 03/28/03  By: dalem
    - Checking for Diag is done globally
Version 1.02  Date: 05/30/03  By: dalem
    - Just queue the message

******************************************************************************/

void XBTLOAD_fnRecvResetCmd( XBT_tzMSG *ptzMsg )
{
    // Save the event in the event queue
    xbtload_fnQueueEvent( XBT_LOAD_RESET_EVENT,
                          ptzMsg,
                          XBT_ADDR_NULL );
}

/******************************************************************************

FUNCTION NAME:
    XBTLOAD_fnReqLoadStatus

PURPOSE:
    Handle a request from the network to send a File Download Status PGN

INPUTS:
    'ucDest' - destination address of where to send the PGN

OUTPUTS:
    None

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version
Version 1.02  Date: 05/30/03  By: dalem
    - Just queue the event and address

******************************************************************************/

void XBTLOAD_fnReqLoadStatus( uchar8 ucDest )
{
    // Save the event in the event queue (specifying the destination address)
    xbtload_fnQueueEvent( XBT_LOAD_STATUS_EVENT, NULL, ucDest );
}

/******************************************************************************

FUNCTION NAME:
    XBTLOAD_fnNextEvent

PURPOSE:
    Extract the next event from the event queue if available.

INPUTS:
    'pptzMsg'     - pointer to a pointer to the message associated with the event
    'pucDestAddr' - pointer to location where to extract the message destination
                    address

OUTPUTS:
    Returns event code

NOTES:

VERSION HISTORY:

Version 1.00  Date: 05/30/03  By: dalem
    - Created

******************************************************************************/

uchar8 XBTLOAD_fnNextEvent( XBT_tzMSG **pptzMsg,
                            uchar8 *pucDestAddr )
{
    tzEVENT *ptzEvt;

    // If the event count is non-zero
    if( ucEventCount != 0 )
    {
        // Get pointer to next event entry;
        ptzEvt = &atzEventList[ ucNextEvent ];

        // Remove one event from the count
        ucEventCount--;

        // Move the index of the next event and check if it is beyond the end
        if( ++ucNextEvent >= DIM( atzEventList ) )
        {
            // Wrap back to beginning index
            ucNextEvent = 0;
        }

        // Assign message pointer and destination address from event record
        *pptzMsg = ptzEvt->ptzMsg;
        *pucDestAddr = ptzEvt->ucDestAddr;

        // Return the event
        return( ptzEvt->ucEvent );
    }
    else
    {
        // No events available
        return( XBT_LOAD_NO_EVENT );
    }
}

/******************************************************************************

FUNCTION NAME:
    XBTLOAD_fnSendLoadStatus

PURPOSE:
    Assemble and send a File Download Status PGN

INPUTS:
    'eOpState'   - download operational state value
    'eResult'    - download result code value
    'ucDestAddr' - destination address
    'ucInstance' - processor instance
    'uiModelNum' - hardware model number
    'uiHwModRev' - hardware module revision number

OUTPUTS:
    None

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - First version
Version 1.01  Date: 04/24/03  By: dalem
    - PGN changed
Version 1.02  Date: 04/24/03  By: dalem
    - Remove restriction of ( ucCurrentEvent == XBT_LOAD_STATUS_EVENT )
Version 1.03  Date: 05/30/03  By: dalem
    - Added destination address parameter
Version 1.04  Date: 06/02/03  By: dalem
    - Added block number to message
Version 1.05  Date: 11/07/03  By: Alan Travelbea
    - Added set of reserved bits in message.

******************************************************************************/

void XBTLOAD_fnSendLoadStatus( XB_teOP_STATE_LDR eOpState,
                               XB_teDL_RESULT_CODE eResult,
                               uint16 uiBlockNum,
                               uchar8 ucDestAddr,
                               uchar8 ucInstance,
                               uint16 uiModelNum,
                               uint16 uiHwModRev )
{
    XBT_tzMSG *ptzMsg;

    // Get empty message
    ptzMsg = XBTMSG_fnNewMsg( XB_PGN_FILE_DNLD_STS );

    // If message buffer is valid
    if( ptzMsg )
    {
        // Set data
        ptzMsg->pucData[ 0 ] = (uchar8) ( ( eOpState & 0x0F ) | ( eResult << 4 ) );
        ptzMsg->pucData[ 1 ] = ucInstance | XBTLOAD_DL_STS_RSV_BITS;
        ptzMsg->pucData[ 2 ] = (uchar8) uiModelNum;
        ptzMsg->pucData[ 3 ] = (uchar8) ( uiModelNum >> 8 );
        ptzMsg->pucData[ 4 ] = (uchar8) uiHwModRev;
        ptzMsg->pucData[ 5 ] = (uchar8) ( uiHwModRev >> 8 );
        ptzMsg->pucData[ 6 ] = (uchar8) uiBlockNum;
        ptzMsg->pucData[ 7 ] = (uchar8) ( uiBlockNum >> 8 );

        // Set data length
        ptzMsg->uiDataLen = 8;

        // Send this message
        XBTMSG_fnSend( ptzMsg, ucDestAddr );
    }
}
