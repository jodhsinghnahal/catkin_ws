/*
***************************************************************************
**
** PROJECT: CAN Library
**
**
** DESCRIPTION: NMEA 2000 Comm Library API - PGN Recviever
**
**            Copyright (c) 2000 Teleflex Marine/Industrial
**                       All Rights Reserved
**   Use, duplication, or disclosure of this material, in any
**   form, is forbidden without permission from Teleflex Marine
**
CHANGE HISTORY:
$Log: pgnrcvr.c $

    ***********************************************
    Revision: XanBus4_TomL/1
    User: TomL     Date: 01/16/10  Time: 02:00AM
    Corrected case of some include files

    ***********************************************
    Revision: XanBus_HollyZ/7
    User: HollyZ     Date: 01/09/07  Time: 05:12PM
    ignore the received messages if they are unreceivale

    ***********************************************
    Revision: XanBus_HenryW/5
    User: HenryW     Date: 06/12/06  Time: 05:38PM
    - fixed the problem of ucSrcAddr not initialized

    ***********************************************
    Revision: XanBus_HenryW/4
    User: HenryW     Date: 06/02/06  Time: 02:27PM
    - fixed a problem where memory was not being released from the HEAP for
    failed Fast-Packet messages.

    ***********************************************
    Revision: XanBus_HollyZ/5
    User: HollyZ     Date: 05/25/06  Time: 10:12PM
    Updated to remove warning for TI compiler

    ***********************************************
    Revision: XanBus_HollyZ/4
    User: HollyZ     Date: 05/25/06  Time: 07:12PM
    Revert Xb stack to V1.2.7

    ***********************************************
    Revision: XanBus_HenryW/3
    User: HenryW     Date: 08/19/05  Time: 03:40PM
    - cleaned up history list.

    ***********************************************
    Revision: XanBus_HollyZ/3
    User: HollyZ     Date: 08/11/05  Time: 08:14PM
    Fixed parameter when calling PGNR_fnNakFrame()

    ***********************************************
    Revision: XanBus_HollyZ/2
    User: HollyZ     Date: 08/02/05  Time: 05:25PM
    Updated to reduce the ROM size

    ***********************************************
    Revision: XanBus_HollyZ/1
    User: HollyZ     Date: 06/03/05  Time: 10:29PM
    In CheckComplete(), call PGDRV_fnRawReceive() to handle the Teleflex received
    message. Removed unused parameter for CheckComplete().

    ***********************************************
    Revision: XanBus_DaleM/2
    User: DaleM     Date: 02/17/05  Time: 04:36PM
    Removed unused frame counter

    ***********************************************
    Revision: GateWayPfrmANMEA_DaleM/1
    User: DaleM     Date: 02/10/05  Time: 07:49PM
    RecvRawDataXfer passes DOB for complete messages Added
    control for NAK of unknown frames/messages Cleaned
    up old history list

*****************************************************************************/
#include <string.h>
#include <stdio.h>
#include "libenv.h"
#include "pgn.h"
#include "xbudefs.h"
#include "xbldefs.h"
#include "xbgdefs.h"

extern const PGN_tzPGN_INFO * XBPACK_fnGetPgnInfo( uint32 uliPgn, sint16 siGrpFunc );
extern void XBMSG_fnRawReceive( PGNDOB *ptzDOB );
extern void XBMLTPKT_fnRecvRawDataXfer( uchar8 *pucRawData, uchar8 ucDataLen, uchar8 ucSrcAddr );

LOCAL void CheckComplete(PGNDOB *pDOB);
LOCAL BOOL IsFrameForMe( CANPORT i8Port, CANFRAME *pCANFrame);
LOCAL void PGNR_fnNakFrame( uchar8            ucPort,
                            uchar8            ucDest,
                            uint32            ulPgn );

static int s_nFramesPerTick[MAX_PORTS];

//***************************************************************************
// PGNRcvr
//  The PGN Rcvr is responsible for managing inbound pgn data object and
//  soliciting can frames from the driver and creating the appropriate
//  pgn data object (or passing the frame to an existing data object)
//
//  Once a pgn indicates it is complete, the Rcvr executes the callback
//  address installed to handle the pgn
//***************************************************************************

// Methods
//*************************************************************************
//
//*************************************************************************
void PGNRConstruct(void)
{
    int i;
    for (i = 0; i < MAX_PORTS; i++)
    {
        s_nFramesPerTick[i] = MAX_FRAMES_IN_PER_TICK;
    }
}

//*************************************************************************
//
//*************************************************************************
void PGNRDestruct(void)
{
}

//*************************************************************************
//
//*************************************************************************
void PGNRSetFramesInPerTick(CANPORT i8Port, int nFramesPerTick)
{
    if (i8Port < MAX_PORTS)
        s_nFramesPerTick[i8Port] = nFramesPerTick;
}

//*************************************************************************
//
//*************************************************************************
TFXCAN_RETURNS PGNRTick(CANPORT i8Port)
{
    CANFRAME                canFrame;
    PGN                     i32PGN;
    PGNDOB                  *pDOB;
    INT16                   i16ByteCount;
    uchar8                  ucData[8];
    const PGN_tzPGN_INFO    *ptzInfo;
    TFXCAN_RETURNS          ret;
    int                     nFramesPerTick;
    uchar8                  ucSrcAddr;

    // Clear frame counters for this tick
    nFramesPerTick = 0;

    // Initialize return code assuming success
    ret = TFXCR_OK;

    // While number of frames processed is less than limit and there is a frame
    while ( ( nFramesPerTick < s_nFramesPerTick[ i8Port ] )
        &&  ( CANFrameRecv( &canFrame, i8Port ) ) )
    {
        // Get PGN frame frame
        i32PGN = CANFrameGetPGN( &canFrame );

        // Check if this frame is for this node
        // always allow addrclaim, RVC uses 0xEE00 instead of 0xEEFF for addrclaim.
        if( IsFrameForMe( i8Port, &canFrame ) || i32PGN == XB_PGN_ISO_ADDR_CLAIM )
        {
            // Get source address
            ucSrcAddr = CANFrameGetSourceAddress( &canFrame );

            // Count number for frames processed within this tick
            nFramesPerTick++;

            // Get CAN data block
            (void) CANFrameGetData( &canFrame, &i16ByteCount, ucData );

            // If this is a multipacket data transfer
            if( i32PGN == XB_PGN_ISO_TP_XFER )
            {
                // Handle the data block in raw mode for speed
                XBMLTPKT_fnRecvRawDataXfer( ucData, (uchar8) i16ByteCount, ucSrcAddr );

                // Finished with this frame, go back to start of while loop
                continue;
            }

            // Look up a descriptor for this PGN
            ptzInfo = XBPACK_fnGetPgnInfo( i32PGN, XB_eGRP_FUNC_VALUE_DO_NOT_CARE );
            if( ptzInfo == NULL )
            {
                // Cannot handle this frame
                continue;
            }

            // if we are not a receiver and ignore flag is set
            if( ( ( ptzInfo->uiFlags & PGN_FLAG_RX ) == 0 ) &&
                ( ( ptzInfo->uiFlags & PGN_FLAG_RX_IGN ) != 0 ) )
            {
               // ignore this message
               continue;
            }

            // If protocol is Fastpacket but it is not a full frame
            if( ( ptzInfo->ucFastPacketSeqIdx != PGN_NOT_FASTPACKET )
             && ( i16ByteCount != PGN_MAX_LEN_SINGLEFRAME ) )
            {
                // send NAK to unknown PGN
                PGNR_fnNakFrame( i8Port,
                                 ucSrcAddr,
                                 i32PGN );

                // Cannot handle this frame
                continue;
            }

            // locate existing dob or allocate a new one
            ret = DOBHLDFindFromFrame( ptzInfo, &canFrame, &pDOB );

            if( ret == TFXCR_OK )
            {
                // have dob already
                pDOB->m_Frame = canFrame;
                (void) PDOBData( pDOB, i16ByteCount, (void *)ucData );
                CheckComplete( pDOB );
            }
            else if( ret == TFXCR_NOT_FOUND )
            {
                continue;
            }
            else
            {
                // Create a DOB for this PGN
                ret = PDOBConstructAlloc( &pDOB, i32PGN, XB_eGRP_FUNC_VALUE_DO_NOT_CARE, ptzInfo );
                if( ret == TFXCR_OK )
                {
                    pDOB->m_ucFlags |= DOB_IS_RECV;
                    PDOBSetPort( pDOB, i8Port );
                    PDOBSetSourceAddr( pDOB, CANFrameGetSourceAddress( &canFrame ) );
                    pDOB->m_Frame = canFrame;
                    if( PDOBData( pDOB, i16ByteCount, (void *)ucData ) == TFXCR_OK )
                    {
                        // put dob into container
                        ret = DOBHLDAdd( pDOB );

                        if( ret )
                        {
                            PDOBDestruct( pDOB );
                        }
                        else
                        {
                            // Check if dob is complete, pass it off handler
                            CheckComplete( pDOB );
                        }
                    }
                    else
                    {
                        PDOBDestruct( pDOB );
                    }
                }
            }
        }
    }
    return ret;
}


//*************************************************************************
//
//*************************************************************************
LOCAL void CheckComplete( PGNDOB *pDOB )
{
    // If DOB exists and message is complete
    if( ( pDOB != NULL ) && ( ( pDOB->m_ucFlags & DOB_RECV_COMPLETE ) != 0 ) )
    {
        // Call receive handler
        XBMSG_fnRawReceive( pDOB );
    }
}

LOCAL BOOL IsFrameForMe( CANPORT i8Port, CANFRAME *ptzCANFrame )
{
    uchar8 destAddr;
    uchar8 ucMyAddr;

    // Verify parameter
 //   (void)assert( ptzCANFrame != NULL );

    destAddr = CANFrameGetDestAddress( ptzCANFrame );
    ucMyAddr = XBADDR_fnGetMyAddr();

    // Touch parameter to hide warning about unused parameter
    if( i8Port != 0 )
    {
        i8Port++;
    }

    // If destination is global or my address
    if( ( destAddr == PGN_ADDR_GLOBAL ) || ( destAddr == ucMyAddr ) )
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
    PGNR_fnNakFrame         - Negatively Acknowledge receive frame

PURPOSE:
    This function is use to send a ISO negative Acknowledgement for a
    misunderstood received frame

INPUTS:
    'ucPort' is the CAN port to use
    'ulPgn' is the Parameter Group Number of the received frame
    'ucDest' is the destination address of the send frame

OUTPUTS:
     None

NOTES:

VERSION HISTORY:

Version 1.00  Date: 05/23/06  By: Hollyz
    - Created

******************************************************************************/

LOCAL void PGNR_fnNakFrame( uchar8            ucPort,
                            uchar8            ucDest,
                            uint32            ulPgn )
{
    // send NAK only for messages received from XB port
    if( ucPort == XB_PORT_A )
    {
        XBMSG_fnSendISOAck( ulPgn,
                            XB_eGRP_FUNC_VALUE_DO_NOT_CARE,
                            XB_eACK_STS_NAK,
                            ucDest );
    }
}
