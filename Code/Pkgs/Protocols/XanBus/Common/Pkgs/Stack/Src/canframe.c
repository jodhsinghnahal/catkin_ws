/*
***************************************************************************
**
** PROJECT: CAN Library
**
**
** DESCRIPTION: NMEA 2000 Comm Library API - CAN Frame object
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

   ***********************************************
    Revision: XanBus_HollyZ/1
    User: HollyZ     Date: 05/25/06  Time: 10:27PM
    Revert XB stack to V1.2.7

    ***********************************************
    Revision: XanBus_HenryW/1
    User: HenryW     Date: 08/19/05  Time: 03:40PM
    - cleaned up history list.

    ***********************************************
    Revision: XanBus_HollyZ/1
    User: HollyZ     Date: 06/03/05  Time: 10:27PM
    Updated to remove complier warning

    ***********************************************
    Revision: XanBus_DaleM/5
    User: DaleM     Date: 03/22/05  Time: 11:27PM
    - Fast packet sequence numbers were chosen and discarded if frame could
    not be queue for transmission.

    ***********************************************
    Revision: XanBus_DaleM/4
    User: DaleM     Date: 03/02/05  Time: 12:19AM
    Added config.h.

    ***********************************************
    Revision: XanBus_DaleM/3
    User: DaleM     Date: 02/18/05  Time: 07:05PM
    No Comment Entered

    ***********************************************
    Revision: XanBus_DaleM/2
    User: DaleM     Date: 02/17/05  Time: 04:50PM
    Dropped unused functions

    ***********************************************
    Revision: GateWayPfrmANMEA_DaleM/1
    User: DaleM     Date: 02/10/05  Time: 07:17PM
    Cleaned up old history list

***************************************************************************/

#include "config.h"         // for XT_mMK_BYTE()
#include "libenv.h"
#include "LIB_stddefs.h"
#include "pgn.h"
#include <string.h>
#include "xbudefs.h"
#include "xbgdefs.h"
#include "xbldefs.h"

LOCAL TFXCAN_RETURNS CreateSingleFrame(PGNDOB *pDOB);
LOCAL TFXCAN_RETURNS CreateFastPacketFrame(PGNDOB *pDOB);
LOCAL TFXCAN_RETURNS CreateMultiPacketFrame(PGNDOB *pDOB);
LOCAL void           DoDataXferFrameSetup( PGNDOB *pDOB );
LOCAL void           DoCommonFrameSetup(PGNDOB *pDOB);
LOCAL BOOL           IsPSDestinationAddress(UINT8 u8PF);
LOCAL UINT8          GetPS(PGNDOB *pDOB);
LOCAL UINT8          GetFastPacketSequenceNo(CANPORT i8Port, PGNDOB *pDOB);

// only used by XB port
LOCAL UINT8  s_SequenceCntr[ XB_PGN_FASTPACKET_COUNT ];

//*************************************************************************
// General Methods
//*************************************************************************
//*************************************************************************
//
//*************************************************************************
void CANFrameConstruct(void)
{
    (void)memset(s_SequenceCntr,0, sizeof(s_SequenceCntr));
}
//*************************************************************************
//
//*************************************************************************
void CANFrameDestruct(void)
{
}
//*************************************************************************
// CANFrameGetPGN
// create pgn from frame data
//*************************************************************************
PGN CANFrameGetPGN(CANFRAME *pFrame)
{
    PGN    i32PGN = 0L;
    UINT8  u8PF;

    if ( pFrame == NULL )
    {
    return i32PGN;
    }

    u8PF = pFrame->m_CanData.m_u8PF;

    i32PGN |= ((PGN)u8PF << 8);

    // determine if PS is part of pgn number of destination address
    if (IsPSDestinationAddress(u8PF) == FALSE)
        i32PGN |= (PGN)(pFrame->m_CanData.m_u8PS);

    if (pFrame->m_CanData.m_u8DataPage)
        i32PGN |= 0x00010000L;

    return i32PGN;
}
//*************************************************************************
//
//*************************************************************************
CANFRAME *CANFrameAllocate(void)
{
    return NULL;
}
//*************************************************************************
//
//*************************************************************************
TFXCAN_RETURNS CANFrameFree(CANFRAME *pFrame)
{
    // Avoid compiler warning
    UNUSED( pFrame );

    return TFXCR_OK;
}


//*************************************************************************
// Receive methods
//*************************************************************************


//*************************************************************************
// CANFrameRecv
//  read frame from driver
//*************************************************************************
BOOL CANFrameRecv(CANFRAME *pFrame, CANPORT i8Port)   // read from driver
{
    return  TFXDRV_i16ReceiveFrame( i8Port, pFrame) == TFXCR_NEW_DATA ? TRUE : FALSE;
}
//*************************************************************************
// CANFrameRecvForeign
//  read raw frame from driver
//*************************************************************************
BOOL CANFrameRecvForeign(CANDRV_tzCAN_FRAME *pFrame, CANPORT i8Port)   // read from driver
{
    return  TFXDRV_i16ReceiveForeignFrame( i8Port, pFrame) == TFXCR_NEW_DATA ? TRUE : FALSE;
}

//*************************************************************************
// CANFrameGetData
//  extract data from frame buffer
//*************************************************************************
TFXCAN_RETURNS CANFrameGetData(CANFRAME *pFrame, INT16 *pi16ByteCount, uchar8 *pucData)
{
    if (( pFrame != NULL )
    && ( pi16ByteCount != NULL )
    && ( pucData != NULL ))
    {
    *pi16ByteCount = (INT16)pFrame->m_CanData.m_u8DataByteCount;
    (void) memcpy( pucData, pFrame->m_CanData.m_u8Data, *pi16ByteCount);
    }
    return TFXCR_OK;
}

// transmit methods
//*************************************************************************
// CANFrameCreate - create a frame for transmission using PGNDOB data and
// control information
//*************************************************************************
TFXCAN_RETURNS CANFrameCreate(PGNDOB *pDOB)
{
    if ( pDOB == NULL )
    {
    return TFXCR_NO_DATA;
    }

    // If tx message is multipacket
    if ( pDOB->m_ucFlags & DOB_IS_MULTIPACKET )
    {
        return CreateMultiPacketFrame( pDOB );
    }

    // If tx message is fastpacket
    else if ( pDOB->m_ucFlags & DOB_IS_FASTPACKET )
    {
        return CreateFastPacketFrame( pDOB );
    }
    else
    {
        // Message is single frame
        return CreateSingleFrame( pDOB );
    }
}

//*************************************************************************
//
//*************************************************************************
TFXCAN_RETURNS CANFrameSend(CANFRAME *pFrame, CANPORT i8Port, INT16 *pHandle)   // send to driver
{
    return TFXDRV_i16SendFrame(i8Port, pFrame, pHandle );
}

//*************************************************************************
//
//*************************************************************************
TFXCAN_RETURNS CANFrameSendForeign(CANDRV_tzCAN_FRAME *pFrame, CANPORT i8Port, INT16 *pHandle)   // send to driver
{
    return TFXDRV_i16SendForeignFrame(i8Port, pFrame, pHandle );
}

//*************************************************************************
//
//*************************************************************************
TFXCAN_RETURNS CANFrameClearHandle(CANPORT i8Port, INT16 iHandle)
{
    // Avoid compiler warning
    UNUSED( i8Port );

    (void) TFXDRV_iClear_msg_handle(iHandle);

    return TFXCR_OK;
}

//*************************************************************************
// Determine state of transmitted data
// TFXCR_IDLE - waiting for all data to go out
// TFXCR_OK - all frames transmitted
//*************************************************************************
TFXCAN_RETURNS CANFrameXmitStatus(INT16 iHandle, INT16 iFramesInPGN, INT16 *pStatus)
{
    UINT16 uiStatus;
    INT16  iFramesSent;
    TFXCAN_RETURNS ret = TFXCR_OK;

    *pStatus = DTS_OK;
    if (TFXDRV_iGet_msg_status(iHandle, &uiStatus, &iFramesSent) == TFXCR_OK)
    {
        if (iFramesSent >= iFramesInPGN)
        {
            ret = TFXCR_OK;
            if (uiStatus != SB_TXOK)
                *pStatus = DTS_FRAME_TRANS_ERROR;
        }
        else
            ret = TFXCR_IDLE; // wait for rest of frames
    }


    return ret;
}

// transmit properties
//*************************************************************************
//
//*************************************************************************
TFXCAN_RETURNS CANFrameSetRetry(CANFRAME *pFrame, BOOL bON)
{
    // Avoid compiler warnings
    UNUSED( pFrame );
    UNUSED( bON );

    return TFXCR_OK;
}

//*************************************************************************
//
//*************************************************************************
uchar8 CANFrameGetSourceAddress(CANFRAME *pFrame)
{
    return pFrame->m_CanData.m_u8SA;
}
//*************************************************************************
// CANFrameGetDestAddress
//
//*************************************************************************
uchar8 CANFrameGetDestAddress(CANFRAME *pFrame)
{
    if ( IsPSDestinationAddress(pFrame->m_CanData.m_u8PF))
        return pFrame->m_CanData.m_u8PS;
    return PGN_ADDR_GLOBAL;
}
//*************************************************************************
// support locals
// The CreateXXXXFrame routines behave as a virtual function would in C++
// One routine packages data up for transmission based on the type of
// protocol being used for the PGNDOB
//*************************************************************************
//*************************************************************************
// CreateSingleFrame
//  Sets up the frame for output in a single frame
//*************************************************************************
LOCAL TFXCAN_RETURNS CreateSingleFrame(PGNDOB *pDOB)
{
    int iDataBytesToSend;

    if ( pDOB == NULL )
    {
    return TFXCR_NO_DATA;
    }

    iDataBytesToSend = pDOB->m_i16ByteCount > 8 ? 8 : pDOB->m_i16ByteCount;

    // If data pointer is not Null
    if ( pDOB->m_pData )
    {
        (void)memcpy(pDOB->m_Frame.m_CanData.m_u8Data,
                   &pDOB->m_pData[pDOB->m_i16pDataBufPos],iDataBytesToSend);
    }

    pDOB->m_i16pDataBufPos += iDataBytesToSend;
    pDOB->m_Frame.m_CanData.m_u8DataByteCount = (UINT8) iDataBytesToSend;
    pDOB->m_CanHandle = INV_MSG_HANDLE;
    pDOB->m_ucCurrentFrame = 1;
    DoCommonFrameSetup(pDOB);

    return TFXCR_OK;
}
//*************************************************************************
// CreateFastPacketFrame
//  Sets up the frame to be the next Fast Packet frame
//*************************************************************************
LOCAL TFXCAN_RETURNS CreateFastPacketFrame( PGNDOB *pDOB )
{
    uint16 uiDataLen;
    uchar8 *pucCANBuf;

    if ( pDOB == NULL )
    {
    return TFXCR_NO_DATA;
    }

    // Get local pointer to CAN buffer
    pucCANBuf = pDOB->m_Frame.m_CanData.m_u8Data;

    // Fill CAN frame with 0xFF and set length to all of it
    (void)memset( pucCANBuf, 0xFF, 8 );
    pDOB->m_Frame.m_CanData.m_u8DataByteCount = 8;
    uiDataLen = 8;

    // If current frame is zero
    if ( !pDOB->m_ucCurrentFrame )
    {
        // First frame, set handle to get new handle and get new sequence number
        pDOB->m_CanHandle = INV_MSG_HANDLE;
        pDOB->m_u8FastPacketSequenceNo =
             GetFastPacketSequenceNo(pDOB->m_i8Port, pDOB);

        // Put sequence number and overall byte count into CAN frame
        *pucCANBuf++ = pDOB->m_u8FastPacketSequenceNo;
        *pucCANBuf++ = (uchar8) pDOB->m_i16ByteCount;

        // Adjust for already used bytes of the frame
        uiDataLen -=2;
    }
    else
    {
        // Subsequent frame, set frame number and sequence  number
        *pucCANBuf++ = pDOB->m_ucCurrentFrame | pDOB->m_u8FastPacketSequenceNo;

        // Adjust for already used bytes of the frame
        uiDataLen -= 1;
    }

    // Limit length of data
    uiDataLen = ( (uint16) pDOB->m_i16ByteCount > uiDataLen )
                         ? uiDataLen : pDOB->m_i16ByteCount;

    // If data pointer is not Null
    if ( pDOB->m_pData )
    {
        // Copy data from buffer to CAN frame
        (void)memcpy( pucCANBuf, &pDOB->m_pData[pDOB->m_i16pDataBufPos], uiDataLen);
    }

    // Adjust DOB buffer extract position and bytes remaining
    pDOB->m_i16pDataBufPos += uiDataLen;
    pDOB->m_i16ByteCount -= uiDataLen;

    // Advance frame
    pDOB->m_ucCurrentFrame++;

    // Do rest of frame setup
    DoCommonFrameSetup(pDOB);

    // Success
    return TFXCR_OK;
}

//*************************************************************************
// CreateMultiPacketFrame
//  Packages the the output into a multiframe data transfer packet
//*************************************************************************
LOCAL TFXCAN_RETURNS CreateMultiPacketFrame(PGNDOB *pDOB)
{
    uint16 uiDataOffset;
    uint16 uiDataLen;
    uchar8 *pucCANBuf;

    if ( pDOB == NULL )
    {
    return TFXCR_NO_DATA;
    }

    // If there is a frame to send
    if ( ( pDOB->m_uiFramesToSend ) && ( pDOB->m_ucCurrentFrame > 0 ) )
    {
        // Get local pointer to CAN buffer
        pucCANBuf = pDOB->m_Frame.m_CanData.m_u8Data;

        // First byte in CAN frame is the frame number
        *pucCANBuf++ = pDOB->m_ucCurrentFrame;

        // Data sent depends on frame number
        uiDataOffset = ( pDOB->m_ucCurrentFrame - 1 ) * 7;

        // Get length of data and limit it to room left in CAN frame
        uiDataLen = pDOB->m_i16ByteCount - uiDataOffset;
        uiDataLen = ( uiDataLen > 7 ) ? 7 : uiDataLen;

        // If data pointer is not Null
        if ( pDOB->m_pData )
        {
            // Copy data from buffer to CAN frame
            (void)memcpy( pucCANBuf, &pDOB->m_pData[ uiDataOffset ], uiDataLen );
        }

        // If frame is not full
        if ( uiDataLen < 7 )
        {
            // Fill rest of frame with 0xFF
            pucCANBuf += uiDataLen;
            (void)memset( pucCANBuf, 0xFF, 7 - uiDataLen );
        }

        // Set length of data, always a full frame
        pDOB->m_Frame.m_CanData.m_u8DataByteCount = 8;

        // Setup a frame as a Data Transfer PGN frame
        DoDataXferFrameSetup( pDOB );

        // Increment the frame count
        pDOB->m_ucCurrentFrame++;

        // Success
        return TFXCR_OK;
    }

    // Set DOB state to WAIT
    pDOB->m_eState = XWAIT;

    // Failure
    return TFXCR_NO_DATA;
}
//*************************************************************************
// DoDatAXferFrameSetup
//*************************************************************************
LOCAL void DoDataXferFrameSetup( PGNDOB *pDOB )
{
    uint32 uliPgn;
    const PGN_tzPGN_INFO *ptzInfo;

    // PGN is for Data Transfer
    uliPgn = XB_PGN_ISO_TP_XFER;

    // Look up a descriptor for this PGN
    ptzInfo = XBPACK_fnGetPgnInfo( XB_PGN_ISO_TP_XFER,
                                   XB_eGRP_FUNC_VALUE_DO_NOT_CARE );

    // Message priority is Transport transfer priority
    pDOB->m_Frame.m_CanData.m_u8Priority = ptzInfo->pktzDesc->ucPriority;

    pDOB->m_Frame.m_CanData.m_u8DataPage = ( uliPgn > 0x0FF0AUL ) ? 1 : 0;
    pDOB->m_Frame.m_CanData.m_u8PF = (UINT8) ( ( uliPgn & 0xFF00UL ) >> 8 );

    pDOB->m_Frame.m_CanData.m_u8PS = pDOB->m_DestAddr;
    pDOB->m_Frame.m_CanData.m_u8SA = pDOB->m_SourceAddr;

    pDOB->m_Frame.pFrameCB = (TFXCANDRVCB)pDOB->m_CB1;
    pDOB->m_Frame.u32UserData = pDOB->m_CB1UserData;
}

//*************************************************************************
// DoCommonFrameSetup
//*************************************************************************
LOCAL void DoCommonFrameSetup(PGNDOB *pDOB)
{
    pDOB->m_Frame.m_CanData.m_u8Priority = pDOB->m_u8Priority;
    pDOB->m_Frame.m_CanData.m_u8DataPage = ( (uint32)pDOB->m_PGN > 0x0FF0AUL ) ? 1 : 0;
    pDOB->m_Frame.m_CanData.m_u8PF = (UINT8) ( ( pDOB->m_PGN & 0xff00) >> 8 );

    pDOB->m_Frame.m_CanData.m_u8PS = GetPS(pDOB);
    pDOB->m_Frame.m_CanData.m_u8SA = pDOB->m_SourceAddr;

    pDOB->m_Frame.pFrameCB = (TFXCANDRVCB)pDOB->m_CB1;
    pDOB->m_Frame.u32UserData = pDOB->m_CB1UserData;
}


//*************************************************************************
//
//*************************************************************************
LOCAL UINT8 GetPS(PGNDOB *pDOB)
{
    if (IsPSDestinationAddress( (UINT8)( ( pDOB->m_PGN & 0x0FF00UL) >> 8 ) ) )
        return pDOB->m_DestAddr;
    else
        return (UINT8) pDOB->m_PGN & 0x00ff;
}
//*************************************************************************
//
//*************************************************************************
LOCAL BOOL IsPSDestinationAddress(UINT8 u8PF)
{
    if (u8PF <= 0xEF)
        return TRUE;
    return FALSE;
}
//*************************************************************************
//
//*************************************************************************
LOCAL UINT8 GetFastPacketSequenceNo(CANPORT i8Port, PGNDOB *pDOB)
{

    uchar8 ucSeqIndex;
    uchar8 *pucSeqCtr;

    if( i8Port == 0 )
    {
        // Get Fast Packet sequence number index from PGN Info from the DOB
        ucSeqIndex = pDOB->m_pPgnInfo->ucFastPacketSeqIdx;

        // Get pointer to sequence counter
        pucSeqCtr = &s_SequenceCntr[ ucSeqIndex ];

        // Increment the sequence counter (upper 3 bits only)
        *pucSeqCtr = XT_mMK_BYTE( *pucSeqCtr + 0x20 );

        // Return the sequence number
        return( *pucSeqCtr );
    }
    else
    {
        return( 0 );
    }
}

