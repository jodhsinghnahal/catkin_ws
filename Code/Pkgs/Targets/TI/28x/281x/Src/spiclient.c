/*==============================================================================
Copyright 2005 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:  spiclient.c

PURPOSE:
    Provide a layer to separate the SPI driver from the RTOS.

FUNCTION(S):
    SPICLIENT_fnCtor -  SPI client construction
    SPICLIENT_fnSignalStartTxRx - indicating a start of communication process
    SPICLIENT_fnWaitForTxRxComplete - waiting for communication to finish
    SPICLIENT_fnTxRxComplete - Complete a spi communication process
    SPICLIENT_fnDtor - SPI client destruction

    local:


NOTES:


CHANGE HISTORY:
$Log: spiclient.c $

    ***********************************************
    Revision: NovaPfrmB_JohnB/1
    User: JohnB     Date: 03/22/05  Time: 06:13PM
    Updated project pathing based on updates to debug lib files.

   ***********************************************
   Revision: NovaPfrmB_TrevorM/1
   User: TrevorM     Date: 03/11/05  Time: 11:49PM
   modified to use SPI driver handle

    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/2
    User: BaldeeshK     Date: 02/04/05  Time: 10:55PM
    - Code review changes 04/02/2005.  Commented header
    and removed previous history


    ***********************************************
    Revision: NovaPfrmB_HongY/1
    User: HongY     Date: 01/11/05  Time: 06:06PM
    removed notes because spiclient.h existed


==============================================================================*/


/*==============================================================================
                              Includes
==============================================================================*/
#include "spiclient.h"  // package header
#include "includes.h"   // RTOS
#include "xassert.h"    // allow the use of assert( n )

/*==============================================================================
                              Defines
==============================================================================*/
// The length of the timeout for waiting on the SPI semaphore
#define SPI_TIMEOUT_TICKS         ( OS_TICKS_PER_SEC / 5 )

/*==============================================================================
                              Macros
==============================================================================*/


/*==============================================================================
                            Type Definitions
==============================================================================*/


/*==============================================================================
                                Enums
==============================================================================*/


/*=============================================================================
                              Structures
==============================================================================*/


/*==============================================================================
                           Local/Private Constants
==============================================================================*/


/*==============================================================================
                           Local/Private Variables
==============================================================================*/
// Use the RTOS semaphore to synchronize tx/rx
OS_EVENT *SpiSemaphore;

// Use the RTOS semaphore to restrict access to the SPI to one user at a time
OS_EVENT *SpiMutex;

/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/


/*==============================================================================
                           Function Definitions
==============================================================================*/

/*******************************************************************************

FUNCTION NAME: SPICLIENT_fnCtor


PURPOSE: Setup and initialize the tx/rx signal synchronization.


INPUTS:
    none

OUTPUTS:
    tuiSTATUS

NOTES:


VERSION HISTORY:

Version: 1.00  Date: May 31 2003  By: Ron Mok
    - Created.

Version: 1.10  Date: Mar 07 2005  By: Trevor Monk
    - Added a mutex to restrict access to the SPI driver

*******************************************************************************/
tuiSTATUS SPICLIENT_fnCtor( void )
{
    // create a binary semaphore
    SpiSemaphore = OSSemCreate( 1 );

    // if there are no more ECBs or there was an error creating the
    //  semaphore, flag an error
    if( SpiSemaphore == NULL )
    {
        assert( 0 );
        return( eSTATUS_ERR );
    }

    // create a binary semaphore
    SpiMutex = OSSemCreate( 1 );

    // if there are no more ECBs or there was an error creating the
    //  semaphore, flag an error
    if( SpiMutex == NULL )
    {
        assert( 0 );
        return( eSTATUS_ERR );
    }

    return( eSTATUS_OK );
}


/*******************************************************************************

FUNCTION NAME: SPICLIENT_fnDtor


PURPOSE: destroy the spi tx/rx sync.


INPUTS:
    none.

OUTPUTS:
    tuiSTATUS - eSTATUS_ERR if the pkg could not be destroyed.
              - eSTATUS_ERR if pkg is destroyed.

NOTES:
    None.

VERSION HISTORY:

Version: 1.00  Date: Sept. 20, 2004  By: Yingran Duan
    - Created.

Version: 1.10  Date: Mar 07, 2005    By: Trevor Monk
    - Added support for deleting the mutex semaphore

*******************************************************************************/
#ifdef GLBL_DTOR_EN
tuiSTATUS SPICLIENT_fnDtor( void )
{
    uchar8  OsErrCode1;
    uchar8  OsErrCode2;

    // delete the semaphore even if there are tasks waiting for it
    OSSemDel( SpiSemaphore,
              OS_DEL_ALWAYS,
              &OsErrCode1 );

    // delete the semaphore even if there are tasks waiting for it
    OSSemDel( SpiMutex,
              OS_DEL_ALWAYS,
              &OsErrCode2 );

    // see if there are any tasks waiting for the SPI semaphore
    if( ( OsErrCode1 == OS_ERR_TASK_WAITING ) ||
        ( OsErrCode2 == OS_ERR_TASK_WAITING ) )
    {
        //there is still a task waiting for the SPI semaphore, so don't dtor yet
        return( eSTATUS_ERR );
    }

    return( eSTATUS_OK );
}
#endif
/*******************************************************************************

FUNCTION NAME: SPICLIENT_fnSignalStartTxRx


PURPOSE: Signal the beginning of tx/rx.


INPUTS:
    none

OUTPUTS:
    tuiSTATUS

NOTES:
    None.

VERSION HISTORY:

Version: 1.00  Date: Setp. 20, 2004  By: Yingran Duan
    - Created.

*******************************************************************************/
tuiSTATUS SPICLIENT_fnSignalStartTxRx( void )
{
    uchar8          SpiSemErrCode;
    OS_SEM_DATA     SpiSemData;

    // check the status of the SPI semaphore
    OSSemQuery( SpiSemaphore,
                &SpiSemData );

    // check to see if the semaphore is free ( 1 = available, 0 = not avail. )
    if( SpiSemData.OSCnt == 1 )
    {
        //get the semaphore
        OSSemPend( SpiSemaphore,
                   SPI_TIMEOUT_TICKS,
                   &SpiSemErrCode );
        if( SpiSemErrCode != OS_NO_ERR )
        {
            assert( 0 );
            return( eSTATUS_ERR );
        }
        return( eSTATUS_OK );
    }

    return( eSTATUS_ERR );

}


/*******************************************************************************

FUNCTION NAME: SPICLIENT_fnWaitForTxRxComplete


PURPOSE: Provide a means for the spi driver to wait for the tx/rx to complete.


INPUTS:
    none

OUTPUTS:
     tuiSTATUS - eSTATUS_ERR if timeout or other error occurred
               - eSTATUS_OK  if tx/rx completed in time


NOTES:
    None.

VERSION HISTORY:

Version: 1.00  Date: Sept. 20, 2004  By: Yingran Duan
    - Created.

*******************************************************************************/
tuiSTATUS SPICLIENT_fnWaitForTxRxComplete( void )
{
    uchar8 SpiSemErrCode;

    // If this func doesn't get the semaphore within SPI_TIMEOUT_TICKS number of
    //  ticks, it means that the ISR was not called in time.
    OSSemPend( SpiSemaphore,
               SPI_TIMEOUT_TICKS,
               &SpiSemErrCode );

    if( SpiSemErrCode != OS_NO_ERR )
    {
        assert( 0 );
        return( eSTATUS_ERR );
    }

    // post the semaphore so it is avaialbe for next transmission
    OSSemPost( SpiSemaphore );

    return( eSTATUS_OK );
}


/*******************************************************************************

FUNCTION NAME: SPICLIENT_fnTxRxComplete


PURPOSE: let the spi drv know that the tx/rx has been completed successfully.


INPUTS:
    none

OUTPUTS:
    none

NOTES:
    None.

VERSION HISTORY:

Version: 1.00  Date: Sept. 20, 2004  By: Yingran Duan
    - Created.

*******************************************************************************/
void SPICLIENT_fnTxRxComplete( void )
{
    // post the semaphore so we know that the tx/rx was ok
    OSSemPost( SpiSemaphore );
}

/*******************************************************************************

FUNCTION NAME: SPICLIENT_fnAcquireLock


PURPOSE: allow a driver to obtain exclusive access to the SPI hardware


INPUTS:
    siWaitTicks : maximum number of ticks to wait for the SPI hardware
                  to become available.  If the number of ticks is zero
                  this function will wait forever for the lock.
                  If the number of ticks is negative, this function will
                  try to acquire the lock without waiting.

OUTPUTS:
    tuiSTATUS - eSTATUS_ERR if the lock could not be acquired
              - eSTATUS_OK if access to the SPI hardware has been granted

NOTES:
    None.

VERSION HISTORY:

Version: 1.00  Date: Mar. 07, 2005  By: Trevor Monk
    - Created.

*******************************************************************************/
tuiSTATUS SPICLIENT_fnAcquireLock( sint16 siWaitTicks )
{
    uchar8 SpiSemErrCode;

    if( siWaitTicks < 0 )
    {
        // try to get the semaphore without blocking
        if( OSSemAccept( SpiMutex ) == 0 )
            return( eSTATUS_ERR );
        else
            return( eSTATUS_OK );
    }

    // If this func doesn't get the semaphore within uiWaitTicks it means that
    // the SPI hardware is still busy with another transaction
    OSSemPend( SpiMutex,
               siWaitTicks,
               &SpiSemErrCode );

    if( SpiSemErrCode != OS_NO_ERR )
    {
        assert( 0 );
        return( eSTATUS_ERR );
    }

    return( eSTATUS_OK );
}

/*******************************************************************************

FUNCTION NAME: SPICLIENT_fnReleaseLock


PURPOSE: allow a driver to release exclusive access to the SPI hardware


INPUTS:
    None

OUTPUTS:
    None

NOTES:
    None.

VERSION HISTORY:

Version: 1.00  Date: Mar. 07, 2005  By: Trevor Monk
    - Created.

*******************************************************************************/
void SPICLIENT_fnReleaseLock( void )
{
    // post the semaphore so we know that the SPI hardware is available
    OSSemPost( SpiMutex );
}


