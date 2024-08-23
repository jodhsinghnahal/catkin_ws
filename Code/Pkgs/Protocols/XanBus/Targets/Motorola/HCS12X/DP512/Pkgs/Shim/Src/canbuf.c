/*==============================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by 
copyright. The reproduction, in whole or in part, by anyone without the written 
approval of Xantrex is prohibited.

FILE NAME:  
    canbuf.c            - CAN stack frame buffers

PURPOSE:
    This file provides message frame buffers for the CAN stack.

FUNCTION(S):
    CANBUF_fnCtor           - Creator for a rx/tx pair of buffers
    CANBUF_fnRxInsert       - Put data into receive buffer
    CANBUF_fnRxExtract      - Remove data from receive buffer
    CANBUF_fnTxInsert       - Put data into transmit buffer
    CANBUF_fnTxExtract      - Remove data from transmit buffer
    CANBUF_fnTxBufFull      - Is Transmit buffer full
   
    local:
        none
   
NOTES:

CHANGE HISTORY:
$Log: canbuf.c $

    ***********************************************
    Revision: XanBus_HollyZ/1
    User: HollyZ     Date: 08/02/05  Time: 05:47PM
    Updated to locate the CAN Rx/Tx buffers on the heap

   ***********************************************
    Revision: XanBus_DaleM/1
    User: DaleM     Date: 03/24/05  Time: 10:37PM
    - Use xassert.h not assert.h
    
    ***********************************************
    Revision: GateWayPfrmANMEA_DaleM/1
    User: DaleM     Date: 02/10/05  Time: 10:25PM
    Cleaned up old history list
    
    
    
==============================================================================*/


/*==============================================================================
                              Includes
==============================================================================*/

#include <string.h>         // for memcpy

#include "canbuf.h"         // External interface to this file
#include "LibCfg.h"         // For MAX_PORTS
#include "xbmem.h"          // for XBMEM_fnMalloc
#include "xassert.h"        // Debugging

/*==============================================================================
                              Defines
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

// The structure for holding the receive buffers
typedef struct canbuf_zRX_FIFO
{
    // The order of entries should not be changed: they get auto-initialized
    CANBUF_tzRX_BUF *ptzBuf;
    uchar8 ucMax;
    uchar8 ucGet;
    uchar8 ucPut;
    uchar8 ucCount;
} CANBUF_tzRX_FIFO;

// The structure for holding the transmit buffers
typedef struct canbuf_zTX_FIFO
{
    // The order of entries should not be changed: they get auto-initialized
    CANBUF_tzTX_BUF *ptzBuf;
    uchar8 ucMax;
    uchar8 ucGet;
    uchar8 ucPut;
    uchar8 ucCount;
} CANBUF_tzTX_FIFO;

/*==============================================================================
                           Local/Private Constants
==============================================================================*/


/*==============================================================================
                           Local/Private Variables
==============================================================================*/

// List of receive FIFOs
static CANBUF_tzRX_FIFO CANBUF_atzRxFifoList[ MAX_PORTS ];

// List of transmit FIFOs
static CANBUF_tzTX_FIFO CANBUF_atzTxFifoList[ MAX_PORTS ];

/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/


/*==============================================================================
                           Function Definitions
==============================================================================*/

/*******************************************************************************

FUNCTION NAME: 
    CANBUF_fnCtor           - Creator for a rx/tx pair of buffers
  
PURPOSE: 
    This function initializes a Rx/Tx pair of buffer for the given channnel

INPUTS: 
    'ucPort' - is the CAN channel of interest

OUTPUTS:
    TRUE if all went well
    FALSE if memory cannot be initialized

NOTES:
    The Rx/Tx buffers are located on the Heap. Hence:
    1. XBMEM_fnCtor() must be called before this function is called;
    2. Heap size shall be adjusted according to the number of CAN channels to
       be used by the application.

VERSION HISTORY:

Version: 1.00  Date: 06-25-04  By: Dalem
    - Created
Version: 1.01  Date: 07-29-05  By: Hollyz
    - Initialize the buffers for the given channel on the heap
    
*******************************************************************************/

tucBOOL CANBUF_fnCtor( uchar8 ucPort )
{

    CANBUF_tzRX_BUF *ptzRxBuf;
    CANBUF_tzTX_BUF *ptzTxBuf;
    CANBUF_tzRX_FIFO *ptzRxFifo;
    CANBUF_tzTX_FIFO *ptzTxFifo;
    
    // valid the port number
    assert( ucPort < MAX_PORTS );
    
    // locate memory for Rx buffer on the heap
  	ptzRxBuf = XBMEM_fnMalloc( CANBUF_RX_PORT_BUFS * 
   	                           sizeof( CANBUF_tzRX_BUF ) );    	                           

    // If the Rx buffer exists
    if ( ptzRxBuf != NULL )
    { 
        // Get local pointer to receive fifo for convenience
        ptzRxFifo = &CANBUF_atzRxFifoList[ ucPort ];

        // Initialize the receive fifo to empty and at the begining
        ptzRxFifo->ptzBuf = ptzRxBuf;
        ptzRxFifo->ucMax = CANBUF_RX_PORT_BUFS;
        ptzRxFifo->ucCount = 0;
        ptzRxFifo->ucPut = 0;
        ptzRxFifo->ucGet = 0;
            
    }
    else
    {   
        //Failure
      	return ( FALSE );
    }	
        
    // locate memory for Tx buffer on the heap
    ptzTxBuf = XBMEM_fnMalloc( CANBUF_TX_PORT_BUFS * 
  	                           sizeof( CANBUF_tzTX_BUF ) );
    	                           
    // If the fifo exists and has non-zero length
    if ( ptzTxBuf != NULL )
    {  
        // Get local pointer to receive fifo for convenience
        ptzTxFifo = &CANBUF_atzTxFifoList[ ucPort ];

        // Initialize the receive fifo to empty and at the begining
        ptzTxFifo->ptzBuf = ptzTxBuf;
        ptzTxFifo->ucMax = CANBUF_TX_PORT_BUFS;
        ptzTxFifo->ucCount = 0;
        ptzTxFifo->ucPut = 0;
        ptzTxFifo->ucGet = 0; 
    }  
    else
    {
        //Failure
        return( FALSE );
    }

    // Success
    return( TRUE );    
}

/*******************************************************************************

FUNCTION NAME: 
    CANBUF_fnRxInsert       - Put data into receive buffer
  
PURPOSE: 
    This function saves a CAN frame into a receive buffer

INPUTS: 
    'ucPort' - is the CAN channel of interest
    'pvData' - is a pointer to data being inserted into buffer

OUTPUTS:
    Number of empty buffers remaining after the call
    CANBUF_BUFFER_FULL if the data could not be saved

NOTES:
    This code DOES NOT PROTECT against simultaneous access. The user needs
    to disable interrupts as appropriate.

VERSION HISTORY:

Version: 1.00  Date: 06-25-04  By: Dalem
    - Created

*******************************************************************************/

uchar8 CANBUF_fnRxInsert( uchar8 ucPort,
                          CANBUF_tzRX_BUF *ptzRxBuf )
{
    CANBUF_tzRX_FIFO *ptzFifo;
    
    assert( ucPort < MAX_PORTS );

    // Get local pointer to receive fifo for convenience
    ptzFifo = &CANBUF_atzRxFifoList[ ucPort ];
    
    // If buffer is not full
    if ( ptzFifo->ucCount < ptzFifo->ucMax )
    {
        // Copy data to frame in fifo
        (void) memcpy( &ptzFifo->ptzBuf[ ptzFifo->ucPut ],
                       ptzRxBuf,
                       sizeof( *ptzRxBuf ) );

        // Move insert point and test for wrapping
        if ( ++ptzFifo->ucPut >= ptzFifo->ucMax )
        {
            // Wrap insert point
            ptzFifo->ucPut = 0;
        }

        // Increase count of entries
        ptzFifo->ucCount++;

        // Return number of empty buffers remaining
        return( ptzFifo->ucMax - ptzFifo->ucCount );
    }
    else
    {
        // Buffer was full
        return( CANBUF_BUFFER_FULL );
    }
}


/*******************************************************************************

FUNCTION NAME: 
    CANBUF_fnRxExtract      - Remove data from receive buffer
  
PURPOSE: 
    This function extracts a CAN frame from a receive buffer

INPUTS: 
    'ucPort' - is the CAN channel of interest
    'pvData' - ia a pointer to memory for returning the data from a buffer

OUTPUTS:
    Number of buffers that still have data after the call
    CANBUF_BUFFER_EMPTY if there is no data being returned

NOTES:
    This code DOES NOT PROTECT against simultaneous access. The user needs
    to disable interrupts as appropriate.

VERSION HISTORY:

Version: 1.00  Date: 06-25-04  By: Dalem
    - Created

*******************************************************************************/

uchar8 CANBUF_fnRxExtract( uchar8 ucPort,
                          CANBUF_tzRX_BUF *ptzRxBuf )
{
    CANBUF_tzRX_FIFO *ptzFifo;
    
    assert( ucPort < MAX_PORTS );

    // Get local pointer to receive fifo for convenience
    ptzFifo = &CANBUF_atzRxFifoList[ ucPort ];
    
    // If buffer is not empty
    if ( ptzFifo->ucCount > 0 )
    {
        // Copy data from fifo
        (void) memcpy( ptzRxBuf,
                       &ptzFifo->ptzBuf[ ptzFifo->ucGet ],
                       sizeof( *ptzRxBuf ) );

        // Move extraction point and test for wrapping
        if ( ++ptzFifo->ucGet >= ptzFifo->ucMax )
        {
            // Wrap extract point
            ptzFifo->ucGet = 0;
        }

        // Decrease count of entries
        ptzFifo->ucCount--;

        // Return count of remaining buffers with data
        return( ptzFifo->ucCount );
    }
    else
    {
        // Buffer was empty
        return( CANBUF_BUFFER_EMPTY );
    }
}

/*******************************************************************************

FUNCTION NAME: 
    CANBUF_fnTxInsert       - Put data into transmit buffer
  
PURPOSE: 
    This function saves a CAN frame into a transmit buffer

INPUTS: 
    'ucPort' - is the CAN channel of interest
    'pvData' - is a pointer to data being inserted into buffer

OUTPUTS:
    Number of empty buffers remaining after the call
    CANBUF_BUFFER_FULL if the data could not be saved

NOTES:
    This code DOES NOT PROTECT against simultaneous access. The user needs
    to disable interrupts as appropriate.

VERSION HISTORY:

Version: 1.00  Date: 06-25-04  By: Dalem
    - Created

*******************************************************************************/

uchar8 CANBUF_fnTxInsert( uchar8 ucPort,
                          CANBUF_tzTX_BUF *ptzTxBuf )
{
    CANBUF_tzTX_FIFO *ptzFifo;
    
    assert( ucPort < MAX_PORTS );

    // Get local pointer to receive fifo for convenience
    ptzFifo = &CANBUF_atzTxFifoList[ ucPort ];
    
    // If buffer is not full
    if ( ptzFifo->ucCount < ptzFifo->ucMax )
    {
        // Copy data to frame in fifo
        (void) memcpy( &ptzFifo->ptzBuf[ ptzFifo->ucPut ],
                       ptzTxBuf,
                       sizeof( *ptzTxBuf ) );

        // Move insert point and test for wrapping
        if ( ++ptzFifo->ucPut >= ptzFifo->ucMax )
        {
            // Wrap insert point
            ptzFifo->ucPut = 0;
        }

        // Increase count of entries
        ptzFifo->ucCount++;

        // Return number of empty buffers remaining
        return( ptzFifo->ucMax - ptzFifo->ucCount );
    }
    else
    {
        // Buffer was full
        return( CANBUF_BUFFER_FULL );
    }
}

/*******************************************************************************

FUNCTION NAME: 
    CANBUF_fnTxExtract      - Remove data from transmit buffer
  
PURPOSE: 
    This function extracts a CAN frame from a transmit buffer

INPUTS: 
    'ucPort' - is the CAN channel of interest
    'pvData' - ia a pointer to memory for returning the data from a buffer

OUTPUTS:
    Number of buffers that still have data after the call
    CANBUF_BUFFER_EMPTY if there is no data being returned

NOTES:
    This code DOES NOT PROTECT against simultaneous access. The user needs
    to disable interrupts as appropriate.

VERSION HISTORY:

Version: 1.00  Date: 06-25-04  By: Dalem
    - Created

*******************************************************************************/

uchar8 CANBUF_fnTxExtract( uchar8 ucPort,
                          CANBUF_tzTX_BUF *ptzTxBuf )
{
    CANBUF_tzTX_FIFO *ptzFifo;
    
    assert( ucPort < MAX_PORTS );

    // Get local pointer to receive fifo for convenience
    ptzFifo = &CANBUF_atzTxFifoList[ ucPort ];
    
    // If buffer is not empty
    if ( ptzFifo->ucCount > 0 )
    {
        // Copy data from fifo
        (void) memcpy( ptzTxBuf,
                       &ptzFifo->ptzBuf[ ptzFifo->ucGet ],
                       sizeof( *ptzTxBuf ) );

        // Move extraction point and test for wrapping
        if ( ++ptzFifo->ucGet >= ptzFifo->ucMax )
        {
            // Wrap extract point
            ptzFifo->ucGet = 0;
        }

        // Decrease count of entries
        ptzFifo->ucCount--;

        // Return count of remaining buffers with data
        return( ptzFifo->ucCount );
    }
    else
    {
        // Buffer was empty
        return( CANBUF_BUFFER_EMPTY );
    }
}

/*******************************************************************************

FUNCTION NAME: 
    CANBUF_fnTxBufFull      - Is Transmit buffer full
  
PURPOSE: 
    This function test if there is room left in the transmit buffer.

INPUTS: 
    'ucPort' - is the CAN channel of interest

OUTPUTS:
    TRUE if buffer is full
    FALSE if buffer has unused space

NOTES:
    This code DOES NOT PROTECT against simultaneous access. The user needs
    to disable interrupts as appropriate.

VERSION HISTORY:

Version: 1.00  Date: 06-25-04  By: Dalem
    - Created

*******************************************************************************/

uchar8 CANBUF_fnTxFull( uchar8 ucPort )
{
    CANBUF_tzTX_FIFO *ptzFifo;
    
    assert( ucPort < MAX_PORTS );

    // Get local pointer to receive fifo for convenience
    ptzFifo = &CANBUF_atzTxFifoList[ ucPort ];
    
    // Return result of if count equal or more than than max
    return ( ptzFifo->ucCount >= ptzFifo->ucMax );
}

