/*
***************************************************************************
**
** PROJECT: CAN Library
**
**
** DESCRIPTION: NMEA 2000 Comm Library API - PGN Transmitter
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
$Log: pgnxmtr.c $

    ***********************************************
    Revision: XanBus4_TomL/2
    User: TomL     Date: 01/20/10  Time: 10:25PM
    Matched case of include statements to files

    ***********************************************
    Revision: XanBus4_TomL/1
    User: TomL     Date: 01/16/10  Time: 02:00AM
    Corrected case of some include files

    ***********************************************
    Revision: XanBus_HollyZ/3
    User: HollyZ     Date: 08/22/07  Time: 11:03PM
    Dump the transmit message if it fails to send after timeout

    ***********************************************
    Revision: XanBus_HollyZ/2
    User: HollyZ     Date: 05/25/06  Time: 10:12PM
    Updated to remove warning for TI compiler

    ***********************************************
    Revision: XanBus_HollyZ/1
    User: HollyZ     Date: 05/25/06  Time: 07:12PM
    Revert Xb stack to V1.2.7

    ***********************************************
    Revision: XanBus_DaleM/2
    User: DaleM     Date: 02/17/05  Time: 04:35PM
    Removed unused frame counter


    ***********************************************
    Revision: GateWayPfrmANMEA_DaleM/1
    User: DaleM     Date: 02/10/05  Time: 07:16PM
    Cleaned up old history list

**
***************************************************************************/

#include "libenv.h"
#include "pgn.h"
#include "cdrive.h"
#include "xbldefs.h"
#include "xbmem.h"


extern unsigned char XBMLTPKT_fnStartTx( PGNDOB *ptzDOB );

static int s_nFramesPerTick[MAX_PORTS];

//***************************************************************************
// PGNXmiter
//  The Xmiter is responsible for managing outbound pgn data objects and
//  soliciting the objects for frames which can be passed to the
//  can controller
//
//  The Xmiter is also responsible for timing related to fastpackets
//    (one must allow for 10ms between fast packets)
//***************************************************************************

// Methods
void PGNXConstruct(void)
{
    int i;
    for (i = 0; i < MAX_PORTS; i++)
        s_nFramesPerTick[i] = MAX_FRAMES_OUT_PER_TICK;
}
void PGNXDestruct(void)
{
}

void PGNXSetFramesOutPerTick(CANPORT i8Port, int nFramesPerTick)
{
    if (i8Port < MAX_PORTS)
        s_nFramesPerTick[i8Port] = nFramesPerTick;
}

void PGNXTick( CANPORT i8Port, BOOL bTick )
{
    PGNDOB              *pCurrDOB;
    PGNDOB              *pLastDOB;
    DOBTICK_DISPOSITION  disposition;
    BOOL                 bDOBHasFrame;
    TFXCAN_RETURNS       ret;
    BOOL                 bDumpPDOB;
    BOOL                 bCanTransmit;
    short                nFramesPerTick;
    PGNDOB               *(*searchFirstDob)(void);
    PGNDOB               *(*searchNextDob)(PGNDOB *);
    void                 (*deleteDob)(PGNDOB *);
    uchar8               ucVipDobFlg;
    CANPORT              cPortNumber;

    ucVipDobFlg = 0;
    cPortNumber = i8Port;
    // cycle through each DOB

    // service and transmit priority message first
    if ( XBMEM_fnGetPriorityDobStatus() == XBMEM_VIP_BUF_ENABLE )
    {
        searchFirstDob = searchFirstVipDOB;
        searchNextDob = searchNextVipDob;
        deleteDob = deleteFromVipDob;

        if ( searchFirstDob() != 0 )
        {
            pCurrDOB = searchFirstDob();
            ucVipDobFlg = 1;
            cPortNumber = i8Port + 1;
        }
        else
        {
            searchFirstDob = DOBHLDFirst;
            searchNextDob = DOBHLDNext;
            deleteDob = DOBHLDDelete;
            pCurrDOB = searchFirstDob();
        }
    }
    else
    {
        searchFirstDob = DOBHLDFirst;
        searchNextDob = DOBHLDNext;
        deleteDob = DOBHLDDelete;
        pCurrDOB = searchFirstDob();
    }

    nFramesPerTick = 0;
    bCanTransmit = TRUE;
    while( pCurrDOB && ( nFramesPerTick < s_nFramesPerTick[ i8Port ] ) && bCanTransmit )
    {
        bDumpPDOB = FALSE;
        if( PDOBGetPort( pCurrDOB ) == cPortNumber )
        {
            ret = TFXCR_OK;
            bDumpPDOB = FALSE;

            while( ( ret == TFXCR_OK )
               &&  ( !bDumpPDOB )
               &&  ( nFramesPerTick < s_nFramesPerTick[i8Port] ) )
            {
                bDOBHasFrame = PDOBHasFrameToTransmit( pCurrDOB );
                bCanTransmit = ( TFXDRV_i16SendFrameCheck( cPortNumber ) == TFXCR_OK );

                if( !bDOBHasFrame || ( bDOBHasFrame && bCanTransmit ) )
                {
                    disposition = PDOBOutputStatus( pCurrDOB, bTick );

                    switch( disposition )
                    {
                    case DTD_OK:
                        ret = TFXCR_IDLE;  // next pgn dob
                        break;
                    case DTD_FRAME_READY:
                        // If this is a multi-packet frame and frame count is 0
                        if ( ( pCurrDOB->m_ucFlags & DOB_IS_MULTIPACKET )
                          && ( pCurrDOB->m_ucTotalFrames == 0 ) )
                        {
                            // Try to start multipacket transmit
                            if( !XBMLTPKT_fnStartTx( pCurrDOB ) )
                            {
                                ret = TFXCR_IDLE;  // Move on to next DOB
                            }
                        }
                        else
                        {
                            ret = PDOBWrite( pCurrDOB );

                            if( ret == TFXCR_OK )
                            {
                                nFramesPerTick++;
                            }
                            else if ( ret == TFXCR_MSG_HANDLE_NOT_AVAILABLE )
                            {
                                TFXDRV_vInit_msg_handles();
                            }
                            else
                            {
                                if( ret != TFXCR_IDLE )
                                {
                                    bCanTransmit = FALSE;
                                }
                            }
                        }
                        break;
                    case DTD_DELETE:
                        if ( PDOBCallback( pCurrDOB, TFXCB_PGN_XMIT ) )
                            bDumpPDOB = TRUE;
                        break;
                    case DTD_COMPLETE:   // on receives indicates that the PGN has been completly assembled
                        break;
                    case DTD_TIMED_OUT:  //
                        if( PDOBCallback( pCurrDOB, TFXCB_PGN_TO ) )
                        {
                            bDumpPDOB = TRUE;
                        }
                        break;
                    case DTD_STATUS:
                        ret = PDOBDoXStatus( pCurrDOB );
                        break;
                    case DTD_ILLEGAL: // Fall through to default
                    default:
                        // Error in the DOB
                        bDumpPDOB = TRUE;
                        break;
                    }
                }
                else if( bCanTransmit == FALSE )
                {
                    // Dump the DOB if the tx message has timeout
                    disposition = PDOBTickTransmit( pCurrDOB, bTick );
                    switch (disposition)
                    {
                    case DTD_TIMED_OUT:
                        if( PDOBCallback( pCurrDOB, TFXCB_PGN_TO ) )
                            bDumpPDOB = TRUE;
                        break;
                    default:
                        break;
                    }

                    // move to the next DOB
                    bCanTransmit = TRUE;
                    ret = TFXCR_IDLE;
                }
            }
        }
        pLastDOB = pCurrDOB;
        pCurrDOB = searchNextDob( pCurrDOB );

        if( bDumpPDOB )
        {
            (void) CANFrameClearHandle( pLastDOB->m_i8Port, pLastDOB->m_CanHandle );
            deleteDob( pLastDOB ); // clear from holder
            PDOBDestruct( pLastDOB ); // clear up any storage associated with dob
        }

        if( pCurrDOB == NULL )
        {
            if( ucVipDobFlg == 1 )
            {
                searchFirstDob = DOBHLDFirst;
                searchNextDob = DOBHLDNext;
                deleteDob = DOBHLDDelete;
                pCurrDOB = searchFirstDob();
                ucVipDobFlg = 0;
            }
        }
    }
}
