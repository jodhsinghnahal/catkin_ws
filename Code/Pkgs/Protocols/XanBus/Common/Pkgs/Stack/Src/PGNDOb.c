/*
***************************************************************************
**
** PROJECT: CAN Library
**
** $Workfile:: PGNDOb.c       $       $Author:: Dalem              $
** $Revision:: 73             $       $Date:: 9/30/04 3:53p        $
**
***************************************************************************
**
** PROJECT: CAN Library
**
**
** DESCRIPTION: NMEA 2000 Comm Library API - data object
**
**
**
**
**            Copyright (c) 2000 Teleflex Marine/Industrial
**                       All Rights Reserved
**   Use, duplication, or disclosure of this material, in any
**   form, is forbidden without permission from Teleflex Marine
**
CHANGE HISTORY:
$Log: PGNDOb.c $

    ***********************************************
    Revision: XanBus_HollyZ/4
    User: HollyZ     Date: 08/22/07  Time: 11:01PM
    Added functions to support ticking the tranmit messages to avoid heap overflow
    due to hardware issues

    ***********************************************
    Revision: XanBus_HollyZ/3
    User: HollyZ     Date: 05/25/06  Time: 10:12PM
    Updated to remove warning for TI compiler

    ***********************************************
    Revision: XanBus_HollyZ/2
    User: HollyZ     Date: 05/25/06  Time: 07:12PM
    Revert Xb stack to V1.2.7

    ***********************************************
    Revision: XanBus_HenryW/1
    User: HenryW     Date: 08/19/05  Time: 03:40PM
    - cleaned up history list.

    ***********************************************
    Revision: XanBus_HollyZ/1
    User: HollyZ     Date: 06/03/05  Time: 10:28PM
    Updated to remove the compiler warning

    ***********************************************
    Revision: XanBus_DaleM/3
    User: DaleM     Date: 03/22/05  Time: 11:27PM
    - Fast packet sequence numbers were chosen and discarded if frame could
    not be queue for transmission.

    ***********************************************
    Revision: XanBus_DaleM/2
    User: DaleM     Date: 02/18/05  Time: 07:03PM
    Removed unused function

    ***********************************************
    Revision: GateWayPfrmANMEA_DaleM/1
    User: DaleM     Date: 02/10/05  Time: 07:46PM
    Allow for creating DOB without PGN Info Cleaned up
    old history list

***************************************************************************/

#include <string.h>
#include <stdio.h>
#include "config.h"
#include "LIB_stddefs.h"
#include "xassert.h"
#include "xbldefs.h"
#include "xbmem.h"


//LOCAL TFXCAN_RETURNS SendNextDataFrame(PGNDOB *pDOB);
LOCAL DOBTICK_DISPOSITION TickDown(PGNDOB *pDOB);
LOCAL DOBTICK_DISPOSITION TickDownState(PGNDOB *pDOB);
LOCAL TFXCAN_RETURNS AddFastPacketFrame(PGNDOB *pDOB, INT16 i16ByteCount, void *pvData);


//*************************************************************************
// General Methods
//*************************************************************************

//*************************************************************************
// PDOBConstruct
//  Initalize the PGNDOB data
//*************************************************************************
TFXCAN_RETURNS PDOBConstructAlloc( PGNDOB **ppDOB,
                                   PGN pgn,
                                   sint16 siGrpFunc,
                                   const PGN_tzPGN_INFO * pzPgnInfo )
{
    TFXCAN_RETURNS ret = TFXCR_HEAP_ALLOC_FAIL;

    *ppDOB = XBMEM_fnMalloc( sizeof( PGNDOB ) );

    if( *ppDOB != NULL )
    {
        ret = PDOBConstruct( *ppDOB, pgn, siGrpFunc, pzPgnInfo );
        if (ret != TFXCR_OK)
        {
            XBMEM_fnFree( *ppDOB );
            *ppDOB = NULL;
        }
        else
        {
            // Remember DOB allocated from heap
            (*ppDOB)->m_ucFlags |= DOB_DOB_FROM_HEAP;
        }
    }
    else
    {
        assert( *ppDOB != NULL );
    }

    return ret;
}

TFXCAN_RETURNS PDOBConstruct( PGNDOB *pDOB,
                              PGN pgn,
                              sint16 siGrpFunc,
                              const PGN_tzPGN_INFO * pzPgnInfo )
{
    (void)memset( pDOB, 0, sizeof(PGNDOB) );
    pDOB->m_PGN = pgn;
    pDOB->m_GroupFunction = siGrpFunc;

    pDOB->m_pPgnInfo = ( const PGN_tzPGN_INFO * )pzPgnInfo;

    // If PGN info is set and protocol is fastpacket
    if( ( pDOB->m_pPgnInfo != NULL )
       && ( pDOB->m_pPgnInfo->ucFastPacketSeqIdx != PGN_NOT_FASTPACKET ) )
    {
        // Set the DOB fastpacket control flag
        pDOB->m_ucFlags |= DOB_IS_FASTPACKET;

        // First fast-packet frame is zero
        pDOB->m_ucCurrentFrame = 0;
    }

    // DOB multipacket control flag is set elsewhere
    // For rx it depends on transport mechanism
    // For tx it depends on message size, it overrides the fastpacket flag

    if( pDOB->m_pPgnInfo && pDOB->m_pPgnInfo->pktzDesc )
    {
        pDOB->m_u8Priority = pDOB->m_pPgnInfo->pktzDesc->ucPriority;  // starting priority
    }

    pDOB->m_eState = XEMPTY;

    // set the default alive timer for the tranmit message
    pDOB->m_i16TickerValue = (INT16)TFXLibMilliToTicks(PGN_STD_TTL);

    // Assume no Received frame flags yet
    pDOB->m_pRxFrameFlags = NULL;

    return TFXCR_OK;
}

//*************************************************************************
// PDOBDestruct
//  release any heap attached to dob
//*************************************************************************
void PDOBDestruct(PGNDOB *pDOB)
{
    if ( ( pDOB->m_ucFlags & DOB_DATA_FROM_HEAP ) && pDOB->m_pData )
    {
        XBMEM_fnFree( pDOB->m_pData );

        pDOB->m_pData = NULL;
    }

    if ( ( pDOB->m_ucFlags & DOB_DATA_FROM_HEAP ) && pDOB->m_pRxFrameFlags )
    {
        XBMEM_fnFree( pDOB->m_pRxFrameFlags );

        pDOB->m_pRxFrameFlags = NULL;
    }

    if ( pDOB->m_ucFlags & DOB_DOB_FROM_HEAP )
    {
        XBMEM_fnFree( pDOB );
    }
}

//*************************************************************************
// PDOBData - a piece of data was received
//*************************************************************************
TFXCAN_RETURNS PDOBData(PGNDOB *pDOB, INT16 i16ByteCount, void *pvData)
{
    // If protocol is Fastpacket
    if ( pDOB->m_ucFlags & DOB_IS_FASTPACKET )
    {
        // allocate for first, add for additional
        return AddFastPacketFrame( pDOB, i16ByteCount, pvData );
    }
    else
    {
        // this is the whole of the pgn
        pDOB->m_pData = pvData;         // Give data to DOB, data not from heap
        pDOB->m_i16ByteCount = i16ByteCount;
        pDOB->m_i16pDataBufPos += i16ByteCount;
        pDOB->m_ucFlags |= DOB_RECV_COMPLETE;

        return TFXCR_OK;
    }
}

//*************************************************************************
// Transmitting Methods
//*************************************************************************
//*************************************************************************
//
//*************************************************************************
DOBTICK_DISPOSITION PDOBOutputStatus(PGNDOB *pDOB, BOOL bTick)
{
    DOBTICK_DISPOSITION ret = DTD_ILLEGAL;

    if (!pDOB)
        return DTD_ILLEGAL;

    switch (pDOB->m_eState)
    {
        case XEMPTY:         // tick down input dobs
            if (pDOB->m_i16TickerValue)
                ret =  bTick ? TickDown(pDOB) : TickDownState(pDOB);
            break;
        case XCTS:           // on receiving, xmit cts
        case XEOM:           // on receiving, xmit eom
        case XRTS:           // xmit rts
        case XDATA:          // xmit data
        case XABORT:         // xmit transmit abort
            ret = DTD_FRAME_READY;
            break;
        case XWAIT:          // transmit, waiting for cts or abort, or eom, or ACK, or ticker
            ret =  bTick ? TickDown(pDOB) : TickDownState(pDOB);
            break;
        case XSTATUS:
            ret = DTD_STATUS;
            break;
        case XCOMPLETE:
            ret = DTD_DELETE;
            break;
        default:
            ret = DTD_ILLEGAL;
    }

    return ret;
}

DOBTICK_DISPOSITION PDOBTickTransmit(PGNDOB *pDOB, BOOL bTick)
{
    DOBTICK_DISPOSITION ret;

    if (!pDOB)
        return DTD_ILLEGAL;

    switch (pDOB->m_eState)
    {

        case XCTS:           // on receiving, xmit cts
        case XEOM:           // on receiving, xmit eom
        case XRTS:           // xmit rts
        case XDATA:          // xmit data
        case XABORT:         // xmit transmit abort
            ret =  bTick ? TickDown(pDOB) : TickDownState(pDOB);
            break;

        default:
            ret = DTD_OK;
    }

    return ret;
}

//*************************************************************************
// PDOBHasFrameToTransmit returns true when a frame can be transmitted
//*************************************************************************
BOOL PDOBHasFrameToTransmit(PGNDOB *pDOB)
{
    BOOL ret = FALSE;
    switch (pDOB->m_eState)
    {
        case XCTS:           // on receiving, xmit cts
        case XEOM:           // on receiving, xmit eom
        case XRTS:           // xmit rts
        case XDATA:          // xmit data
        case XABORT:         // xmit transmit abort
            ret = TRUE;
            break;
        default:
            ret = FALSE;
            break;
    }
    return ret;

}

//*************************************************************************
//
//*************************************************************************
TFXCAN_RETURNS PDOBGetNextXmitFrame(PGNDOB *pDOB, CANFRAME *pFrame)
{
    // Avoid compiler warnings
    UNUSED( pDOB );
    UNUSED( pFrame );

    return TFXCR_OK;
}
//*************************************************************************
// PDOBWrite
//  This method is responsible for creating and passing a frame off to the
//  the driver
//
//  After the frame is sent the state of the PGNDOB is updated to reflect the
//  next state as follows:
//
//    XDATA,          // more data to send
//    XWAIT,          // external event will change state (link layer) or ticker (timeout)
//    XCOMPLETE,      // complete
//    XSTATUS         // have sent all data for now get status
//*************************************************************************
TFXCAN_RETURNS PDOBWrite(PGNDOB *pDOB)
{// allocate frame and write it
    TFXCAN_RETURNS ret = TFXCR_DRV_TX_BUFFER_ALLOCATION_FAILURE;
    uint16 uiDataBufPos;
    uint16 uiByteCount;
    uchar8 ucCurrentFrame;

    // Save data position and byte count before frame create
    uiDataBufPos = pDOB->m_i16pDataBufPos;
    uiByteCount = pDOB->m_i16ByteCount;
    ucCurrentFrame = pDOB->m_ucCurrentFrame;

    // Build up a can frame
    if ( CANFrameCreate(pDOB) == TFXCR_OK )
    {
        // Try to send frame
        ret = CANFrameSend( &pDOB->m_Frame, pDOB->m_i8Port, &pDOB->m_CanHandle );

        // If frame send succesful
        if ( ret == TFXCR_OK )
        {
            // Set state to check status (this is based on single frame - for
            // other protocols would set up with more data or wait for delay
            // If tx message is multipacket
            if ( pDOB->m_ucFlags & DOB_IS_MULTIPACKET )
            {
                // Message is multipacket

                // Frame sent, decrease count of frame to send
                pDOB->m_uiFramesToSend--;

                // If frames to send not zero
                if ( pDOB->m_uiFramesToSend )
                {
                    // Multipacket transfer not complete
                    pDOB->m_eState = XDATA;
                }
                else
                {
                    // Need to wait for EOM or CTS
                    pDOB->m_eState = XWAIT;
                }
                ret = TFXCR_OK;
            }

            // If tx message is fastpacket
            else if ( pDOB->m_ucFlags & DOB_IS_FASTPACKET )
            {
                // Message is fastpacket

                // If message all sent, state is status and return is idle
                pDOB->m_eState = pDOB->m_i16ByteCount ? XDATA : XSTATUS;
                ret = pDOB->m_i16ByteCount ? TFXCR_OK : TFXCR_IDLE;
            }
            else
            {
                // Message is singleframe
                // If message all sent, state is status and return is idle
                pDOB->m_eState = XSTATUS;
                ret = TFXCR_IDLE;
            }
        }
        else if ( ret == TFXCR_DRV_BUSY )
        {
            // If frame not sent because there was no handle, leave
            // DOB unchanged so message can be tried later
            pDOB->m_i16pDataBufPos = uiDataBufPos;
            pDOB->m_i16ByteCount = uiByteCount;
            pDOB->m_ucCurrentFrame = ucCurrentFrame;
        }
        else
        {
             // some kind of error occured - cannot send out frame do callback
            pDOB->m_eState = XCOMPLETE;
            pDOB->m_eTerminatingStatus = DTS_FRAME_TRANS_ERROR;
        }
    }

    if( ret == TFXCR_IDLE )
    {
        // on completion of transmitting single frame or fastpacket frame
        pDOB->m_i16TickerValue = 0;
    }

    return ret;
}


//*************************************************************************
// PDOBDoXStatus
//  check transmit status of frame(s)
//  if status ok
//     if more data
//        based on protocol
//     else if timer, go to wait state
//*************************************************************************
TFXCAN_RETURNS PDOBDoXStatus(PGNDOB *pDOB)
{
    // get state from driver of frame if available
    // ok its good
    pDOB->m_eState = pDOB->m_i16TickerValue ? XWAIT : XCOMPLETE;
    if( pDOB->m_eState == XCOMPLETE)
    {
        if( CANFrameXmitStatus( pDOB->m_CanHandle, pDOB->m_ucCurrentFrame,
                                &pDOB->m_eTerminatingStatus) == TFXCR_IDLE )
            pDOB->m_eState = XWAIT; // not done being transmitted
    }

    return TFXCR_OK;
}

//*************************************************************************
//General properties
//*************************************************************************
//*************************************************************************
//
//*************************************************************************
void PDOBSetCallback(PGNDOB *pDOB, TFXLIBCB cbFunction, UINT32 u32UserData)
{
    pDOB->m_CB1 = cbFunction;
    pDOB->m_CB1UserData = u32UserData;
}

//*************************************************************************
// Receiving properties
//*************************************************************************

//*************************************************************************
//
//*************************************************************************
void PDOBSetSourceAddr(PGNDOB *pDOB, uchar8 ucSrcAddr)
{
    pDOB->m_SourceAddr = ucSrcAddr;
}

//*************************************************************************
// Transmitting properties
//*************************************************************************

void PDOBSetDestAddress(PGNDOB *pDOB, uchar8 ucNetAddr)
{
    pDOB->m_DestAddr = ucNetAddr;
}

//*************************************************************************
// TickDown
//  Logic handles counting down of DOB
//*************************************************************************
LOCAL DOBTICK_DISPOSITION TickDown(PGNDOB *pDOB)
{
    if (--pDOB->m_i16TickerValue <= 0)
    { // ticked down
        //printf("Time Out\n");
        return DTD_TIMED_OUT;
    }
    return DTD_OK;
}
//*************************************************************************
// TickDownState
//*************************************************************************
LOCAL DOBTICK_DISPOSITION TickDownState(PGNDOB *pDOB)
{
    return pDOB->m_i16TickerValue <= 0 ? DTD_TIMED_OUT : DTD_OK;
}
//*************************************************************************
//
//*************************************************************************
void PDOBSetPort(PGNDOB *pDOB, CANPORT i8Port)
{
    pDOB->m_i8Port = i8Port;
}

//*************************************************************************
//
//*************************************************************************
CANPORT PDOBGetPort(PGNDOB *pDOB)
{
    return  pDOB->m_i8Port;
}
//*************************************************************************
//
//*************************************************************************
int PDOBCallback(PGNDOB *pDOB, TFXLIB_CB_REASON cbReason)
{
    int ret = 1;
    if (pDOB->m_CB1)
    {
        ret = (*pDOB->m_CB1)(cbReason, (UINT32) pDOB, (UINT32)0, pDOB->m_CB1UserData);
    }
    return ret;
}
//*************************************************************************
//
//*************************************************************************
void PDOBSetTimer(PGNDOB *pDOB, UINT16 u16MilliSeconds)
{
    pDOB->m_i16TickerValue = (INT16)TFXLibMilliToTicks(u16MilliSeconds);
}

uint16 TestAndSetFlag( uchar8 *pucFlags, uchar8 ucNFlags, uchar8 ucFlagId )
{

    uchar8 ucArrayIndex;
    uchar8 ucBitOffset;
    uchar8 ucBitMask = 1;


    ( void )assert( pucFlags != NULL );

    ( void )assert( ucFlagId < ucNFlags );

    // Avoid compiler warning
    UNUSED( ucNFlags );

    // Each array element has 8 bits
    ucArrayIndex = ( uchar8 )( ucFlagId / XT_BITS_PER_BYTE );

    ucBitOffset = ( uchar8 )( ucFlagId % XT_BITS_PER_BYTE );

    // Shift to select the right bit within the element
    while ( ucBitOffset > 0 )
    {
        ucBitMask <<= 1;

        ucBitOffset--;
    }

    // If the bit is not set, set it
    // Return indication of old setting
    if ( ( pucFlags[ ucArrayIndex ] & ucBitMask ) == 0 )
    {
        pucFlags[ ucArrayIndex ] |= ucBitMask;

        return FALSE;
    }
    else    // Already set
    {
        return TRUE;
    }
}

//*************************************************************************
// AddFastPacketFrame
// adds frames to fast packets
// set pbPGNComplete TRUE if all data is received, FALSE if more to come
// first frame:
//   byte 0 - contains frame number + sequence id
//   byte 1 - contains number of bytes to be received
//   bytes 2-7 - contain data
// subsequent frames
//   byte 0 - contains frame number + sequence id
//   bytes 1-7 - contains data
//*************************************************************************

LOCAL TFXCAN_RETURNS AddFastPacketFrame(PGNDOB *pDOB, INT16 i16ByteCount, void *pvData)
{
    UINT8 *pData = (UINT8 *)pvData;
    int    iDataStart;

    // Fastpacket frames must be full frames
    if ( i16ByteCount != 8 )
    {
        // Cannot handle this frame
        return TFXCR_NO_DATA;
    }

    if ( !pDOB->m_ucCurrentFrame )
    {
        // first frame - allocate data
        if ((pData[0] & 0x001f) != 0)
        {
            // something bad going on here
            pDOB->m_pPgnInfo = NULL;  // create error
            return TFXCR_NOT_FOUND;
        }
        else
        {
            // allocate heap for the data
            pDOB->m_pData = XBMEM_fnMalloc( pData[ 1 ] + 8 );
            if ( pDOB->m_pData == NULL )
            {
                return TFXCR_HEAP_ALLOC_FAIL;
            }

            // The first fast-packet frame only has 6 data bytes, the rest have 7
            // Allocate space for received frame flags, one flag per frame, eight flags per byte
            if ( pData[ 1 ] > 6 )
            {
                pDOB->m_NRxFrameFlags = ( ( pData[ 1 ] - PGN_FAST_PKT_DATA_BYTES_1ST_FRAME +
                            ( PGN_FAST_PKT_DATA_BYTES_PER_FRAME - 1 ) ) / PGN_FAST_PKT_DATA_BYTES_PER_FRAME ) + 1;
            }
            else
            {
                pDOB->m_NRxFrameFlags = 1;
            }

            pDOB->m_pRxFrameFlags = XBMEM_fnMalloc( ( pDOB->m_NRxFrameFlags + ( XT_BITS_PER_BYTE - 1 ) ) /
                                                    XT_BITS_PER_BYTE );

            if ( pDOB->m_pRxFrameFlags == NULL )
            {
                return TFXCR_HEAP_ALLOC_FAIL;
            }

            // Clear 'em out
            ( void )memset( pDOB->m_pRxFrameFlags, 0, ( pDOB->m_NRxFrameFlags + ( XT_BITS_PER_BYTE - 1 ) ) /
                                                    XT_BITS_PER_BYTE );

            pDOB->m_ucNRxFrames = 1;

            pDOB->m_ucFlags |= DOB_DATA_FROM_HEAP;
            pDOB->m_i16pDataBufPos = 0;
            pDOB->m_i16ByteCount = (INT16)pData[1];
            iDataStart = 2;
            pDOB->m_u8FastPacketSequenceNo = (UINT8)(pData[0] & 0x00e0);  // record sequence id
        }
    }
    else
    { // subsequent frame add data
        if ((pData[0] & 0x001f) != pDOB->m_ucCurrentFrame)
            return TFXCR_NOT_FOUND;
        iDataStart = 1;
    }
    i16ByteCount -= iDataStart;  // adjust based on front end control information
    if (i16ByteCount)
    {
        (void) memcpy(&pDOB->m_pData[pDOB->m_i16pDataBufPos], &pData[iDataStart], i16ByteCount);
        pDOB->m_i16pDataBufPos += i16ByteCount;
    }

    // If we have not received this frame already
    if ( TestAndSetFlag( pDOB->m_pRxFrameFlags,
                            pDOB->m_NRxFrameFlags,
                            pData[0] & 0x001f ) == FALSE )
    {
        // Count it
        pDOB->m_ucNRxFrames++;
    }

    // If we received all the frames we expect
    if ( pDOB->m_ucNRxFrames > pDOB->m_NRxFrameFlags )
    {
        // Set data receive complete flag
        pDOB->m_ucFlags |= DOB_RECV_COMPLETE;
    }

    pDOB->m_ucCurrentFrame++;
    PDOBSetTimer(pDOB, 750);  // good for another 750 milliseconds
    return TFXCR_OK;
}

