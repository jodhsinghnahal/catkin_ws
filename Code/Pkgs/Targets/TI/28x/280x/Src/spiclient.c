/*==============================================================================
Copyright © 2005 Xantrex International

This file is the property of Xantrex International and shall not be reproduced,
copied, or used as the basis for the manufacture or sale of equipment without
the express written permission of Xantrex International.

FILE NAME:  spiclient.c

PURPOSE:
    Provide a layer to separate the SPI driver from the RTOS.

FUNCTION(S):
    SPICLIENT_fnCtor                - SPI client construction
    SPICLIENT_fnDtor                - SPI client destruction
    SPICLIENT_fnSignalStartTxRx     - Indicating a start of communication 
                                      process
    SPICLIENT_fnWaitForTxRxComplete - Waiting for communication to finish
    SPICLIENT_fnTxRxComplete        - Complete a SPI communication process
    SPICLIENT_fnAcquireLock         - Acquire exclusive access to the SPI 
                                      hardware
    SPICLIENT_fnReleaseLock         - Release exclusive access to the SPI 
                                      hardware

    local:
        none

NOTES:
    Ported from the TI 281x

CHANGE HISTORY:
$Log: spiclient.c $

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/1
    User: JeffF     Date: 02/13/06  Time: 06:59PM
    Ran the copyright updater script to bring the copyright notice on all project
    files up to date.

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
                              Function Enable Switches
==============================================================================*/

// include code to acquire and release the LOCK for the SPI
#ifndef SPICLIENT_ACQ_REL_LOCK_ENABLE
#define SPICLIENT_ACQ_REL_LOCK_ENABLE      ( FALSE )
#endif


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

#if ( SPICLIENT_ACQ_REL_LOCK_ENABLE == TRUE )
// Use the RTOS semaphore to restrict access to the SPI to one user at a time
OS_EVENT *SpiMutex;
#endif

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
    none

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

#if ( SPICLIENT_ACQ_REL_LOCK_ENABLE == TRUE )
    // create a binary semaphore
    SpiMutex = OSSemCreate( 1 );

    // if there are no more ECBs or there was an error creating the
    //  semaphore, flag an error
    if( SpiMutex == NULL )
    {
        assert( 0 );
        return( eSTATUS_ERR );
    }
#endif
    return( eSTATUS_OK );
} // SPICLIENT_fnCtor


/*******************************************************************************

FUNCTION NAME: SPICLIENT_fnDtor


PURPOSE: destroy the spi tx/rx sync.


INPUTS:
    none.

OUTPUTS:
    tuiSTATUS - eSTATUS_ERR if the pkg could not be destroyed.
              - eSTATUS_ERR if pkg is destroyed.

NOTES:
    none

VERSION HISTORY:

Version: 1.00  Date: Sep 20, 2004    By: Yingran Duan
    - Created.

Version: 1.10  Date: Mar 07, 2005    By: Trevor Monk
    - Added support for deleting the mutex semaphore
     
Version: 1.11  Date: May 27, 2005    By: Wayland Yu
    - if the the define OS_SEM_DEL_EN is not enable, do not compile that 
      section.

*******************************************************************************/
#ifdef GLBL_DTOR_EN
tuiSTATUS SPICLIENT_fnDtor( void )
{
#if( OS_SEM_DEL_EN > 0 )    
    uchar8  OsErrCode1;
    uchar8  OsErrCode2;

    // delete the semaphore even if there are tasks waiting for it
    OSSemDel( SpiSemaphore,
              OS_DEL_ALWAYS,
              &OsErrCode1 );
    if( OsErrCode1 == OS_ERR_TASK_WAITING )
    {
        //there is still a task waiting for the SPI semaphore, so don't dtor yet
        return( eSTATUS_ERR );
    }

#if( SPICLIENT_ACQ_REL_LOCK_ENABLE == TRUE )
    // delete the semaphore even if there are tasks waiting for it
    OSSemDel( SpiMutex,
              OS_DEL_ALWAYS,
              &OsErrCode2 );

    // see if there are any tasks waiting for the SPI semaphore
    if( ( OsErrCode2 == OS_ERR_TASK_WAITING ) 
    {
        //there is still a task waiting for the SPI semaphore, so don't dtor yet
        return( eSTATUS_ERR );
    }
#endif

#endif
    return( eSTATUS_OK );
} // SPICLIENT_fnDtor
#endif


/*******************************************************************************

FUNCTION NAME: SPICLIENT_fnSignalStartTxRx

PURPOSE: Signal the beginning of tx/rx.

INPUTS:
    none

OUTPUTS:
    tuiSTATUS

NOTES:
    none

VERSION HISTORY:

Version: 1.00  Date: Sep 20, 2004  By: Yingran Duan
    - Created. 
Version: 1.01  Date: May 27, 2005  By: Wayland Yu
    - Replaced with use of OSSemAccept

*******************************************************************************/
tuiSTATUS SPICLIENT_fnSignalStartTxRx( void )
{
    // Check if the SPI semaphore is available
    if( OSSemAccept( SpiSemaphore ) > 0 )
    {
        // Resource is available
        return( eSTATUS_OK );
    }
    
    return( eSTATUS_ERR );

} // SPICLIENT_fnSignalStartTxRx


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
    // ticks, it means that the ISR was not called in time.
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
} // SPICLIENT_fnWaitForTxRxComplete


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

} // SPICLIENT_fnTxRxComplete


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
    none

VERSION HISTORY:

Version: 1.00  Date: Mar. 07, 2005  By: Trevor Monk
    - Created.

*******************************************************************************/
tuiSTATUS SPICLIENT_fnAcquireLock( sint16 siWaitTicks )
{
#if ( SPICLIENT_ACQ_REL_LOCK_ENABLE == TRUE )
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
#endif // #if ( SPICLIENT_ACQ_REL_LOCK_ENABLE == TRUE )
    return( eSTATUS_OK );
} // SPICLIENT_fnAcquireLock


/*******************************************************************************

FUNCTION NAME: SPICLIENT_fnReleaseLock

PURPOSE: allow a driver to release exclusive access to the SPI hardware

INPUTS:
    none

OUTPUTS:
    none

NOTES:
    none

VERSION HISTORY:

Version: 1.00  Date: Mar. 07, 2005  By: Trevor Monk
    - Created.

*******************************************************************************/
void SPICLIENT_fnReleaseLock( void )
{
#if ( SPICLIENT_ACQ_REL_LOCK_ENABLE == TRUE )
    // post the semaphore so we know that the SPI hardware is available
    OSSemPost( SpiMutex );
#endif // #if ( SPICLIENT_ACQ_REL_LOCK_ENABLE == TRUE )
} // SPICLIENT_fnReleaseLock
