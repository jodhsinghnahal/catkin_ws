/*
***************************************************************************
**
** PROJECT: CAN Library
**
**
** DESCRIPTION: NMEA 2000 Comm Library API - data object
**
**            Copyright (c) 2000 Teleflex Marine/Industrial
**                       All Rights Reserved
**   Use, duplication, or disclosure of this material, in any
**   form, is forbidden without permission from Teleflex Marine
**
***************************************************************************/

#include "xbldefs.h"
#include "xbmem.h"
#include "cdrive.h"
#include <stdio.h>

LOCAL INT32    m_si32TicksPerSecond = 0;
LOCAL BOOL     m_bLibBusy[MAX_PORTS];

//*************************************************************************
//
//*************************************************************************
TFXCAN_RETURNS TFXLibInit(CANPORT i8Port, uchar8 ucBitRate )
{
    TFXCAN_RETURNS ret;

    if( i8Port == 0 )
    {
        // Simply set the initial values for the message handles. No
        // failure possible and it doesn't depend on other modules.
        TFXDRV_vInit_msg_handles();
    }

    // Execute the constructors of the following modules to get them ready
    // to process CAN messages.  They simply set initial values, there is
    // no memory allocation involved and so no failure is possible and can
    // be executed at any time.
    DOBHLDConstruct();
    PGNXConstruct();
    PGNRConstruct();
    CANFrameConstruct();

    // Initialize the TeleFlex Rx and Tx buffers; it involves allocating
    // memory from the Xanbus heap so it could fail.
    ret = TFXDRV_i16InitBuffers(i8Port);
    // Continue only if ok.
    if (ret == TFXCR_OK)
    {
        // Now that the buffers are properly setup initialize the CAN controller
        ret = TFXDRV_i16InitController( i8Port, ucBitRate );

        if( ( ret == TFXCR_OK ) && ( i8Port == 0 ) )
        {
            m_bLibBusy[i8Port] = FALSE;
        }
    }

    return ret;
}

//*************************************************************************
//
//*************************************************************************
TFXCAN_RETURNS TFXLibTimeBase(INT32 i32TicksPerSecond)
{
    m_si32TicksPerSecond = i32TicksPerSecond;
    return TFXCR_OK;
}

//*************************************************************************
//
//*************************************************************************
INT32 TFXLibMilliToTicks(INT32 i32Milliseconds)
{
    INT32 ticks = (i32Milliseconds *  m_si32TicksPerSecond) / 1000L ;
    if ((i32Milliseconds *  m_si32TicksPerSecond) % 1000L)
        ticks++;
    if (ticks < 1)
        ticks = 1;
    return ticks;
}

//*************************************************************************
//
//*************************************************************************
TFXCAN_RETURNS TFXLibDelay(CANPORT i8Port, UINT16 u16Milliseconds, PGNDOB *pDOB, TFXLIBCB cbFunction, UINT32 u32UserData)
{
    TFXCAN_RETURNS ret;
    (void) PDOBConstruct( pDOB, 0, XB_eGRP_FUNC_VALUE_DO_NOT_CARE, NULL );
    PDOBSetPort(pDOB, i8Port);                                  // set port
    // install callbacks
    if (cbFunction)
        PDOBSetCallback(pDOB, cbFunction, u32UserData);

    ret = DOBHLDAdd(pDOB);

    if ( ret == TFXCR_OK )
        PDOBSetTimer(pDOB, u16Milliseconds);

    return ret;
}

//*************************************************************************
//
//*************************************************************************
TFXCAN_RETURNS TFXLibCancelDelay(PGNDOB *pDOB)
{
   DOBHLDDelete( pDOB );  // make sure my dob is removed if timing down
   XBMEM_fnFree( pDOB );
   return TFXCR_OK;
}

//*************************************************************************
//
//*************************************************************************
TFXCAN_RETURNS TFXLibCycleInput(CANPORT i8Port)
{
    return TFXLibTickInput(i8Port);
}

//*************************************************************************
//
//*************************************************************************
void TFXLibMaxXmitFramesPerCycle(CANPORT i8Port, int maxFramesPerCycle)
{
    PGNXSetFramesOutPerTick(i8Port, maxFramesPerCycle);

}

//*************************************************************************
//
//*************************************************************************
void TFXLibMaxRcvFramesPerCycle(CANPORT i8Port, int maxFramesPerCycle)
{
    PGNRSetFramesInPerTick(i8Port, maxFramesPerCycle);

}

//*************************************************************************
//
//*************************************************************************
TFXCAN_RETURNS TFXLibTickInput(CANPORT i8Port)
{
    if (m_bLibBusy[i8Port])
        return TFXCR_LIB_BUSY;
    m_bLibBusy[i8Port] = TRUE;
    (void) PGNRTick(i8Port);
    m_bLibBusy[i8Port] = FALSE;
    return TFXCR_OK;
}

//*************************************************************************
//
//*************************************************************************
static TFXCAN_RETURNS RunOutputCycle(CANPORT i8Port)
{
    if (TFXDRV_i16ControllerState(i8Port) == TFXCR_BUS_OFF)
        return TFXCR_BUS_OFF;

    if (m_bLibBusy[i8Port])
        return TFXCR_LIB_BUSY;

    m_bLibBusy[i8Port] = TRUE;

    PGNXTick(i8Port, TRUE);

    m_bLibBusy[i8Port] = FALSE;

    return TFXCR_OK;
}

//*************************************************************************
//
//*************************************************************************
TFXCAN_RETURNS TFXLibTickOutput(CANPORT i8Port)
{
    return RunOutputCycle(i8Port);
}

//*************************************************************************
//
//*************************************************************************
TFXCAN_RETURNS TFXLibCycleOutput(CANPORT i8Port)
{
    return RunOutputCycle(i8Port);
}


