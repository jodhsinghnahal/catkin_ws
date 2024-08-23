/******************************************************************************

  (c) 2004 - 2021 Xantrex Canada ULC. All rights reserved.
  All trademarks are owned or licensed by Xantrex Canada ULC,
  its subsidiaries or affiliated companies.

****************************************************************************//**

  @addtogroup canbuf CANBUF
  @{

    @brief CAN Stack Frame Buffer Management

****************************************************************************//**

  @page canbuf_design XanBus CAN Buffer Management

    Handles the message-frame-buffers for the CAN stack.

  @par API Functions
    CANBUF_fnCtor           - Creator for a rx/tx pair of buffers
    CANBUF_fnRxInsert       - Put data into receive buffer
    CANBUF_fnRxExtract      - Remove data from receive buffer
    CANBUF_fnTxInsert       - Put data into transmit buffer
    CANBUF_fnTxExtract      - Remove data from transmit buffer
    CANBUF_fnTxBufFull      - Is Transmit buffer full

  @note
    The  message-frame-buffers are allocated from the XanBus heap.

*******************************************************************************/


/*==============================================================================
                              Includes
==============================================================================*/

#include <string.h>         // Standard C String Library interface

#include "canbuf.h"         // XanBus CAN Buffer Management
#include "LibCfg.h"         // XanBus Library Configuration interface
#include "xbmem.h"          // XanBus Memory Management interface
#include "xassert.h"        // Xantrex assert interface

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
                           External/Public Definitions
==============================================================================*/

extern uint32 XbHeapRunStart;

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
    volatile CANBUF_tzRX_BUF *ptzRxBuf;
    volatile CANBUF_tzTX_BUF *ptzTxBuf;
    volatile CANBUF_tzRX_FIFO *ptzRxFifo;
    volatile CANBUF_tzTX_FIFO *ptzTxFifo;

    // valid the port number
    assert( ucPort < MAX_PORTS );

    // locate memory for Rx buffer on the heap
    ptzRxBuf = XBMEM_fnMalloc( CANBUF_RX_PORT_BUFS *
                               sizeof( CANBUF_tzRX_BUF ) );

    // If the Rx buffer exists
    if ( ptzRxBuf != NULL )
    {
        // Get local pointer to receive FIFO for convenience
        ptzRxFifo = &CANBUF_atzRxFifoList[ ucPort ];

       // Initialize the receive fifo to empty and at the beginning
        ptzRxFifo->ptzBuf = (CANBUF_tzRX_BUF *)ptzRxBuf;
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
        // Get local pointer to receive FIFO for convenience
        ptzTxFifo = &CANBUF_atzTxFifoList[ ucPort ];

        // Initialize the receive fifo to empty and at the beginning
        ptzTxFifo->ptzBuf = (CANBUF_tzTX_BUF *)ptzTxBuf;
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
    volatile CANBUF_tzRX_FIFO *ptzFifo;

    assert( ucPort < MAX_PORTS );

    // Get local pointer to receive FIFO
    ptzFifo = &CANBUF_atzRxFifoList[ ucPort ];
    // Check for validity; message buffer should be in the Xanbus heap
    if( (ptzFifo != NULL ) && ( (uint32)ptzFifo->ptzBuf >= (uint32)&XbHeapRunStart ) )
    {
        // If buffer is not full
        if( ptzFifo->ucCount < ptzFifo->ucMax )
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
    else
    {
        assert( 0 );
    }

    return ( CANBUF_BUFFER_FULL );
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

    // Get local pointer to receive FIFO
    ptzFifo = &CANBUF_atzRxFifoList[ ucPort ];
    // Check for validity; message buffer should be in the Xanbus heap
    if( (ptzFifo != NULL ) && ( (uint32)ptzFifo->ptzBuf >= (uint32)&XbHeapRunStart ) )
    {
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
    else
    {
        assert( 0 );
    }

    return ( CANBUF_BUFFER_EMPTY );
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

    // Get local pointer to transmit FIFO for convenience
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

    // Get local pointer to Tx fifo for convenience
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

    // Get local pointer to transmit FIFO for convenience
    ptzFifo = &CANBUF_atzTxFifoList[ ucPort ];

    // Return result of if count equal or more than than max
    return ( ptzFifo->ucCount >= ptzFifo->ucMax );
}


//*****************************************************************************
// End of file.
//! @}
//*****************************************************************************
