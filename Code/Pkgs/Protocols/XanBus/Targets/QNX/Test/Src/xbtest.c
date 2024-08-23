/*=============================================================================
Copyright © 2009 Xantrex International

This file is the property of Xantrex International and shall not be reproduced,
copied, or used as the basis for the manufacture or sale of equipment without
the express written permission of Xantrex International.

FILE NAME:  xbtest.c

PURPOSE:
    This is a console application meant to demonstrate how to develop
    a XanBus application on the PC.

FUNCTION(S):
   
    local:
   
NOTES:
    This program uses the XanBus library for communication, which in turn uses
    libraries for the various CAN interface drivers that it supports.
    The code in the XanBus library is essentially the same code that is used in
    all of Xantrex's XanBus supported devices.  As such it is fairly 
    low-level and operating system agnostic.  It is therefore up to the
    application program to provide the following items:
    - A thread to periodically call the 'tick' function which processes
      incoming and outgoing messages.
    - A number of functions that are called when a message is received
      or an event occurs.
    - A queue for sending received messages from the 'tick' thread to the
      main thread.

    This program has been compiled and tested with Microsoft Visual C++
    .NET 2003.

CHANGE HISTORY :
$Log$
        
==============================================================================*/

/*==============================================================================
                              Includes
==============================================================================*/

#include <stdio.h>     // Support for printf 
#include <pthread.h>   // Posix threads
#include "xbudefs.h"   // Message definitions
#include "xbgdefs.h"   // The XanBus API
#include "xbwincb.h"   // Some standard callback functions used by the tick thread

/*=============================================================================
                              Definitions
==============================================================================*/

// Run the XBINIT_fnTick function every 10ms
#define TICKS_PER_SEC   100
#define MS_PER_TICK     ( 1000 / TICKS_PER_SEC )
#define US_PER_TICK     ( 1000000 / TICKS_PER_SEC )

// If no reply comes within 2 seconds then don't expect one.
#define RX_TIMEOUT_MS   2000

// Maximum number of other devices on the XanBus network
#define XB_MAX_NODES    20

// Bit Rate defines shall be consistent with CANDRV.h defines
// the standard rate is 250 kbaud
#define CAN_BAUD_20K    ( 0 )
#define CAN_BAUD_125K   ( 1 )
#define CAN_BAUD_250K   ( 2 )
#define CAN_BAUD_500K   ( 3 )
#define CAN_BAUD_1M     ( 4 )

// Size of input queue
#define QUEUE_SIZE      8

/*==============================================================================
                            File Variables
==============================================================================*/

// Bus Ready flag
uint16 uiBusReady = FALSE;

// XanBus message heap
static uchar8 aucHeap[ 32768 ];

// Mutex for sharing memory between the main thread and the tick thread
pthread_mutex_t XbShare;           // Mutex
pthread_mutexattr_t MutexAttr;     // Mutex attribute variable

// Handle for tick thread
pthread_t hTickThread;

// Received message queue shared between main thread and tick thread
PGN_tzRECV_DATA *atzReceiveQueue[ QUEUE_SIZE ];
uint16 uiQueueStart = 0;
uint16 uiQueueEnd = 0;

/*==============================================================================
                           Global Variables
==============================================================================*/

tucBOOL XanBusClosed;

/*==============================================================================
                           Function Prototypes
==============================================================================*/

// -- Internal functions --
void *fnTickThread( void *pParam );

static void fnInstallRxHandler( uint32 ulPgn,
				void ( *pfnRecv )( PGN_tzRECV_DATA *ptzRecv ) );

tucBOOL fnQueueWrite( PGN_tzRECV_DATA *ptzRecv );

PGN_tzRECV_DATA *fnQueueRead( void );


// -- Useful application-level functions --

// Open connection to XanBus
tucBOOL fnXbInit( uchar8 ucDeviceInstanceLower,
		  uchar8 ucDeviceInstanceUpper,
		  XB_teFUNC_CODE teDeviceFunction );

// Close XanBus connection
void fnXbClose( void );

// Print list of connected devices
uchar8 fnScanDevices( void );

// Set receive handling for a PGN
void fnListen( uint32 ulPgn,
	       tucBOOL bEnable, 
	       void ( *pfnRecv )( PGN_tzRECV_DATA *ptzRecv ) );

// Handle EnDisCfg PGN
void fnRecvEnDisCfg( PGN_tzRECV_DATA *ptzRecv );

// Handle ISO Ack PGN
void fnRecvIsoAck( PGN_tzRECV_DATA *ptzRecv );

// Receive general Sts PGN
void fnRecvSts( PGN_tzRECV_DATA *ptzRecv );

// Receive a reply from the received messages queue
PGN_tzRECV_DATA *fnReceiveFromQueue( uchar8 ucAddr, uint32 ulExpectedPgn );

// Wait for an Ack as a reply from a control command or commanded configuration
tucBOOL fnWaitForAck( uchar8 ucAddr, uint32 ulPgn );

// Turn a device's Identify feature on or off
void fnIdentify( uchar8 ucAddr, tucBOOL bEnable );

// Set a device's high level operating mode
void fnSetMode( uchar8 ucAddr, XB_teCTRL_MODE teNewMode );


/*==============================================================================
                           Function Definitions
==============================================================================*/

/*******************************************************************************

FUNCTION NAME:
    fnInitCritical

PURPOSE:
    Allocate the memory used by a critical section

INPUTS:
    none

OUTPUTS:
    none

NOTES:
    
*******************************************************************************/
void fnInitCritical( void )
{
    // Set the mutex as a recursive mutex
    pthread_mutexattr_settype( &MutexAttr, PTHREAD_MUTEX_RECURSIVE_NP );

    // create the mutex with the attributes set
    pthread_mutex_init( &XbShare, &MutexAttr );

    //After initializing the mutex, the thread attribute can be destroyed
    pthread_mutexattr_destroy( &MutexAttr );
}

/*******************************************************************************

FUNCTION NAME:
    fnDeleteCritical

PURPOSE:
    Release the resource used by a critical section

INPUTS:
    none

OUTPUTS:
    none

NOTES:
    
*******************************************************************************/
void fnDeleteCritical( void )
{
    pthread_mutex_destroy( &XbShare );
}

/*******************************************************************************

FUNCTION NAME:
    fnEnterCritical

PURPOSE:
    The XanBus layer calls this application function when it requests ownership
	of the critical section for memory malloc/free operation on the Xanbus Heap.

INPUTS:
    none

OUTPUTS:
    none

NOTES:
*******************************************************************************/

void fnEnterCritical( void )
{
    // Acquire the mutex to access the shared resource
    pthread_mutex_lock( &XbShare );
}

/*******************************************************************************

FUNCTION NAME:
    fnLeaveCritical

PURPOSE:
    The XanBus layer calls this application function when it releases ownership
	of the critical section for memory malloc/free operation on the Xanbus Heap.

INPUTS:
    none

OUTPUTS:
    none

NOTES:
*******************************************************************************/

void fnLeaveCritical( void )
{
    pthread_mutex_unlock( &XbShare );
}


/*******************************************************************************

FUNCTION NAME: fnTickThread

PURPOSE:
    The function that executes the XanBus tick in a separate thread.

INPUTS:
    pParam - Not used, just a general parameter for any thread function
     
OUTPUTS:
    Always NULL

NOTES:
    This function is deceptively simple.  XBINIT_fnTick will call a number
    of different callback functions as complete messages are received or
    other network events occur.  All of these callback functions are part of
    this thread.  Therefore special handling is required if received messages
    are to be passed on and handled by the main thread.

*******************************************************************************/

void *fnTickThread( void *pParam )
{
    tucBOOL Online = FALSE;

    printf( "Tick thread starting\n" );

    // Run as an infinite loop until a close command is received
    while ( !XanBusClosed )
    {
	XBINIT_fnTick();
	if ( !Online )
	{
	    if ( XBADDR_fnOnLine() )
	    {
		Online = TRUE;
	    }
	}

	usleep( US_PER_TICK );
    }

    printf( "Tick thread closed\n" );

    return 0;
}

/*******************************************************************************

FUNCTION NAME: fnXbInit

PURPOSE:
    Sequence for initializing the XanBus interface and starting the tick thread

INPUTS:
    DeviceInstanceLower - Device Instance Lower (ISO ECU Instance)
    DeviceInstanceUpper - Device Instance Upper (ISO Function Instance)
    DeviceFunction      - Device Function (ISO Function)
     
OUTPUTS:
    TRUE if initialization completed successfully
    FALSE if there was a problem

NOTES:

*******************************************************************************/

tucBOOL fnXbInit( uchar8 ucDeviceInstanceLower,
		  uchar8 ucDeviceInstanceUpper,
		  XB_teFUNC_CODE teDeviceFunction )
{
    int Success = TRUE;
    int ThreadResult;         // Result of initializing the tick thread

    // Set the device name this computer will use on the XanBus network.
    XBWIN_fnSetName( ucDeviceInstanceLower, ucDeviceInstanceUpper, teDeviceFunction );
    
    // Initialize the platform-specific callback functions
    XBWIN_fnInitCB();

    // Initialize XanBus memory
    XBMEM_fnCtor( aucHeap, sizeof( aucHeap ) );

    // Initialize XanBus address handling
    srand( (unsigned)time( NULL ) );
    XBADDR_fnCtor( rand(), XB_MAX_NODES );

    // Initialize the XanBus interface
    if ( !XBINIT_fnInit( 0,
                         CAN_BAUD_250K,
                         TICKS_PER_SEC ) )
    {
        // Display indication of fatal error and quit
        printf( "XB init failed\n" );
        XBPLATFORM_fnCheckErrors();
        XBMEM_fnDtor();
        Success = FALSE;
    }

    // Make sure network is not suspended
    if ( Success )
    {
	printf( "XanBus initialized\n" );
        XBINIT_fnNetCtrl( XB_eNET_CTRL_RESUME );
    }

    // Start address claiming
    if ( Success )
    {
        if ( !XBADDR_fnBeginClaiming() )
        {
            printf( "Failed to start address claiming\n" );
            XBPLATFORM_fnCheckErrors();
            XBMEM_fnDtor();
            Success = FALSE;
        }
    }

    if ( Success )
    {
	printf( "Address claiming started\n" );

	// Set up mutual exclusion between for the tick thread
	fnInitCritical();

	// Start the tick thread
	XanBusClosed = FALSE;
	ThreadResult = pthread_create( &hTickThread,
				       NULL,
				       fnTickThread,
				       NULL );
	if ( ThreadResult != 0 )
	{
	    printf( "Thread not created\n" );
	    XanBusClosed = TRUE;
	    Success = FALSE;
	}
    }

    return Success;
}

/*******************************************************************************

FUNCTION NAME: fnXbClose

PURPOSE:
    Close the Xanbus connection

INPUTS:
    None
     
OUTPUTS:
    None

NOTES:

*******************************************************************************/

void fnXbClose( void )
{
    // Shut down the tick task
    XanBusClosed = TRUE;

    // Wait for it to finish
    pthread_join( hTickThread, NULL );

    // Close the handle and critical section
    fnDeleteCritical();

    // Free stack and driver resources
    XBMEM_fnDtor();
    TFXDRV_CloseDriver();
}

/*******************************************************************************

FUNCTION NAME: fnScanDevices

PURPOSE:
    Print out the devices found on the network and return the node address of
    the first one found.

INPUTS:
    None
     
OUTPUTS:
    The node address of the first XanBus device found
    0xff if no devices were found

NOTES:
    This function is meant for the demo application only

*******************************************************************************/

uchar8 fnScanDevices( void )
{
    uchar8 ucFirstAddr = 0xff;
    uchar8 i;
    uchar8 ucAddr;
    XB_tzPGN_ISO_ADDR_CLAIM tzNAME;
    const char *pFuncName;

    for ( i = 0; i < XB_MAX_NODES; i++ )
    {
	if ( XBADDR_fnFindDevice( i, &ucAddr, &tzNAME ) )
	{
	    // Print address and name
	    switch ( tzNAME.teDeviceFunction )
	    {
	    case XB_eFUNC_CODE_USER_INTERFACE:
		pFuncName = "User Interface";
		break;

	    case XB_eFUNC_CODE_INVERTER_CHARGER:
		pFuncName = "Inverter Charger";
		break;

	    case XB_eFUNC_CODE_INVERTER:
		pFuncName = "Inverter";
		break;

	    case XB_eFUNC_CODE_CHARGER:
		pFuncName = "Charger";
		break;

	    case XB_eFUNC_CODE_AGS:
		pFuncName = "Generator Starter";
		break;

	    case XB_eFUNC_CODE_DIAGNOSTIC_TERMINAL:
		pFuncName = "Diagnostic Terminal";
		break;

	    case XB_eFUNC_CODE_GATEWAY:
		pFuncName = "Gateway";
		break;

	    default:
		pFuncName = "Unknown";
		break;
	    }

	    printf( "%s %d at node %d\n",
		    pFuncName,
		    tzNAME.ucDeviceInstanceUpper,
		    ucAddr );

	    // Capture the first address found
	    if ( ucFirstAddr == 0xff )
	    {
		ucFirstAddr = ucAddr;
	    }
	}
    }

    return ucFirstAddr;
}

/*******************************************************************************

FUNCTION NAME: fnInstallRxHandler

PURPOSE:
    Indicate which function will handle a specific XanBus PGN

INPUTS:
    ulPgn    - The PGN to be handled
    pfnRecv  - The pointer to the function that will handle it
     
OUTPUTS:
    None

NOTES:
    Use fnListen instead of using this function directly, as it also
    enables receipt of the PGN in question.

*******************************************************************************/

static void fnInstallRxHandler( uint32 ulPgn,
				void ( *pfnRecv )( PGN_tzRECV_DATA *ptzRecv ) )
{
    PGN_tzPGN_INFO const * ptzPgnInfo;
    PGN_tzPGN_DESC *ptzDesc;

    // Get the Info record for this PGN
    ptzPgnInfo = XBPACK_fnGetPgnInfo( ulPgn, ( sint16 )XB_eGRP_FUNC_VALUE_DO_NOT_CARE );

    // If Info record found
    if ( ptzPgnInfo != NULL )
    {
        // Copy the rx handler to the descriptor for PGN
        ptzDesc = (PGN_tzPGN_DESC *) ptzPgnInfo->pktzDesc;
        if ( ptzDesc != NULL )
        {
            ptzDesc->pfnRecv = pfnRecv;
        }
    }
}

/*******************************************************************************

FUNCTION NAME: fnListen

PURPOSE:
    Set up to listen (or not) to a particular PGN

INPUTS:
    ulPgn   - PGN to be either handled or ignored
    bEnable - Flag to enable (or disable) receipt of the PGN
    pfnRecv - Function to call when the PGN is handled
     
OUTPUTS:
    None

NOTES:
    Note that the receive function will be called within the tick thread

*******************************************************************************/

void fnListen( uint32 ulPgn,
	       tucBOOL bEnable, 
	       void ( *pfnRecv )( PGN_tzRECV_DATA *ptzRecv ) )
{
    // Make sure that the callback function is enabled if turning on
    if ( bEnable )
    {
        fnInstallRxHandler( ulPgn, pfnRecv );
    }

    XBMSG_fnControlDynamic( ulPgn, bEnable );
}

/*******************************************************************************

FUNCTION NAME: fnQueueWrite

PURPOSE:
    Write to the queue for received messages.

INPUTS:
    ptzRecv - Pointer to the received message block to be put on the queue
     
OUTPUTS:
    TRUE if there was room for the message on the queue

NOTES:
    This function is used by receive message callbacks in the tick thread
    to get messages out to the main thread.

*******************************************************************************/

tucBOOL fnQueueWrite( PGN_tzRECV_DATA *ptzRecv )
{
    uint16 uiNewQueueEnd;
    tucBOOL Success;

    fnEnterCritical();

    // Only do this if queue isn't full
    uiNewQueueEnd = ( uiQueueEnd + 1 ) % QUEUE_SIZE;
    if ( uiQueueStart != uiNewQueueEnd )
    {
	// Prevent the tick thread from destroying the message
	ptzRecv->ucUseCount++;

	atzReceiveQueue[ uiQueueEnd ] = ptzRecv;
	uiQueueEnd = uiNewQueueEnd;
	Success = TRUE;
    }
    else
    {
	// Queue full. Destroy the message now
	printf( "Input queue full\n" );
	Success = FALSE;
    }

    fnLeaveCritical();

    return Success;
}

/*******************************************************************************

FUNCTION NAME: fnQueueRead

PURPOSE:
    Read from the input message queue

INPUTS:
    None
     
OUTPUTS:
    A pointer to the next message on the queue
    NULL if the queue is empty

NOTES:
    This function returns immediately whether there is a message in the queue
    or not.  Use fnReceiveFromQueue to wait for an expected reply form a device.

*******************************************************************************/

PGN_tzRECV_DATA *fnQueueRead( void )
{
    PGN_tzRECV_DATA *ptzMessage;

    fnEnterCritical();

    // Check for empty queue
    if ( uiQueueStart == uiQueueEnd )
    {
	ptzMessage = NULL;
    }
    else
    {
	ptzMessage = atzReceiveQueue[ uiQueueStart ];
	uiQueueStart++;
	if ( uiQueueStart >= QUEUE_SIZE )
	{
	    uiQueueStart = 0;
	}
    }

    fnLeaveCritical();

    return ptzMessage;
}

/*******************************************************************************

FUNCTION NAME: fnRecvEnDisCfg

PURPOSE:
    Received message handler for EnDisCfg PGN.  Only user level configurations
    are sent through the queue to the main thread.  All others are filtered out.

INPUTS:
    ptzRecv - a pointer to the received message block
     
OUTPUTS:
    None

NOTES:
    This 

*******************************************************************************/

void fnRecvEnDisCfg( PGN_tzRECV_DATA *ptzRecv )
{
    XB_tzPGN_EN_DIS_CFG *ptzEnDisCfg;

    if ( ptzRecv->ulPgn == XB_PGN_EN_DIS_CFG )
    {
	// Filter out Factory and OEM configurations
	ptzEnDisCfg = ( XB_tzPGN_EN_DIS_CFG * )ptzRecv->pvStruct;
	if ( ptzEnDisCfg->teCfgType == XB_eCFG_TYPE_USER )
	{
	    // Post this to the main thread
	    fnQueueWrite( ptzRecv );
	}
    }
}

/*******************************************************************************

FUNCTION NAME: fnRecvIsoAck

PURPOSE:
    Received message handler for ISO Ack PGN

INPUTS:
    ptzRecv - a pointer to the received message block
     
OUTPUTS:
    None

NOTES:

*******************************************************************************/

void fnRecvIsoAck( PGN_tzRECV_DATA *ptzRecv )
{
    // Just dump into the queue
    ptzRecv->ucUseCount++;
    fnQueueWrite( ptzRecv );
}

/*******************************************************************************

FUNCTION NAME: fnRecvSts

PURPOSE:
    Received message handler for Sts PGN

INPUTS:
    ptzRecv - a pointer to the received message block
     
OUTPUTS:
    None

NOTES:
    This function just prints the current mode field of the message to console
    rather than sending the message to the main thread.

*******************************************************************************/

void fnRecvSts( PGN_tzRECV_DATA *ptzRecv )
{
    XB_tzPGN_STS *ptzSts;
    const char *pucMode;

    if ( ptzRecv->ulPgn == XB_PGN_STS )
    {
	// Just print out the sending node and the system status
	// within the tick thread
	ptzSts = ( XB_tzPGN_STS * )ptzRecv->pvStruct;
	switch ( ptzSts->teCurrentMode )
	{
	case XB_eCTRL_MODE_HIBERNATE:
	    pucMode = "Hibernate";
	    break;

	case XB_eCTRL_MODE_POWER_SAVE:
	    pucMode = "Power Save";
	    break;

	case XB_eCTRL_MODE_SAFE:
	    pucMode = "Safe";
	    break;

	case XB_eCTRL_MODE_OPERATING:
	    pucMode = "Operating";
	    break;

	case XB_eCTRL_MODE_DIAGNOSTIC:
	    pucMode = "Diagnostic";
	    break;

	default:
	    pucMode = "Unknown";
	}
	
	printf( "Node %d in %s mode\n",
		ptzRecv->ucSrcAddr,
		pucMode );
    }
}

/*******************************************************************************

FUNCTION NAME: fnReceiveFromQueue

PURPOSE:
    Receive an expected message (or NAK) from queue

INPUTS:
    ucAddr   - Node address of the device that is expected to send a message
    ulExpectedPgn - The PGN that is expected to be received
     
OUTPUTS:
    A pointer to the received message
    NULL if no message was received in the anticipated time

NOTES:

*******************************************************************************/

PGN_tzRECV_DATA *fnReceiveFromQueue( uchar8 ucAddr, uint32 ulExpectedPgn )
{
    PGN_tzRECV_DATA *ptzRecv = NULL;
    uint32 ulElapsedMs = 0;

    // Wait for a message to come in or the timeout period to expire
    while (( ulElapsedMs < RX_TIMEOUT_MS ) && ( ptzRecv == NULL ))
    {
	usleep( US_PER_TICK );
	ulElapsedMs += MS_PER_TICK;

	do
	{
	    ptzRecv = fnQueueRead();
	    if ( ptzRecv != NULL )
	    {
		// Does this message come from the expected node with the
		// expected PGN?
		if (( ptzRecv->ucSrcAddr == ucAddr )
		    && (( ptzRecv->ulPgn == ulExpectedPgn )
			|| ( ptzRecv->ulPgn == XB_PGN_ISO_ACK )))
		{
		    // This is the message we were waiting for
		    break;
		}
		else
		{
		    // Not what we're expecting so destroy it
		    XBMSG_fnDestroyRecvStruct( ptzRecv );
		}
	    }
	} while ( ptzRecv != NULL );
    }

    // Timeout or success
    return ptzRecv;
}

/*******************************************************************************

FUNCTION NAME: fnWaitForAck

PURPOSE:
    Wait for an ISO Ack after sending a command or configuration

INPUTS:
    ucAddr  - Node address of the device that is expected to send a message
    ulPgn   - The PGN is to be acknowledged
   
OUTPUTS:
    TRUE if a positive acknowledgement was received
    FALSE if there was a timeout NAK or DNY

NOTES:
    The main thread should call this function after sending a command or
    a new configuration to a device.

*******************************************************************************/

tucBOOL fnWaitForAck( uchar8 ucAddr, uint32 ulPgn )
{
    PGN_tzRECV_DATA *ptzRecv;
    XB_tzPGN_ISO_ACK *ptzIsoAck;
    tucBOOL ucSuccess = FALSE;

    ptzRecv = fnReceiveFromQueue( ucAddr, XB_PGN_ISO_ACK );
    if ( ptzRecv == NULL )
    {
	printf( "Timeout waiting for ACK\n" );
    }
    else
    {
	if ( ptzRecv->ulPgn == XB_PGN_ISO_ACK )
	{
	    ptzIsoAck = ( XB_tzPGN_ISO_ACK * )ptzRecv->pvStruct;
	    switch ( ptzIsoAck->teCtrlByte )
	    {
	    case XB_eACK_STS_ACK:
		// This is what we wanted
		ucSuccess = TRUE;
		break;
		
	    case XB_eACK_STS_NAK:
		printf( "NAK after sending EnDisCfg\n" );
		break;
		
	    case XB_eACK_STS_DNY:
		printf( "Deny after sending EnDisCfg\n" );
		break;
		
	    default:
		printf( "Unknown control byte in ACK message\n" );
	    }

	    // Check that this ack is for the message we're expecting
	    if ( ptzIsoAck->ulParmGrpNum != ulPgn )
	    {
		printf( "Received ISOAck for 0x%06X, not 0x%06X\n",
			ptzIsoAck->ulParmGrpNum,
			ulPgn );
		ucSuccess = FALSE;
	    }
	}
	else
	{
	    printf( "PGN 0x%06X received while waiting for ACK\n", ptzRecv->ulPgn );
	}

	XBMSG_fnDestroyRecvStruct( ptzRecv );
    }

    return ucSuccess;
}

/*******************************************************************************

FUNCTION NAME: fnIdentify

PURPOSE:
    Use EnDisCfg to get a device to turn its identify feature on or off

INPUTS:
    ucAddr  - Node address of the device being commanded
    bEnable - Flag to enable or disable the identify feature
     
OUTPUTS:
    None

NOTES:

*******************************************************************************/

void fnIdentify( uchar8 ucAddr, tucBOOL bEnable )
{
    PGN_tzRECV_DATA *ptzRecv;
    XB_tzPGN_EN_DIS_CFG *ptzEnDisCfg;

    // Enable receipt of EnDisCfg
    fnListen( XB_PGN_ISO_ACK, TRUE, fnRecvIsoAck );
    fnListen( XB_PGN_EN_DIS_CFG, TRUE, fnRecvEnDisCfg );

    // Request it from the device
    XBMSG_fnISOReq( XB_PGN_EN_DIS_CFG, ucAddr );

    // Wait for the message to come back
    ptzRecv = fnReceiveFromQueue( ucAddr, XB_PGN_EN_DIS_CFG );
    if ( ptzRecv == NULL )
    {
	// Timeout
	printf( "Timeout\n" );
    }
    else if ( ptzRecv->ulPgn == XB_PGN_EN_DIS_CFG )
    {
	// Adjust the identify field
	ptzEnDisCfg = ( XB_tzPGN_EN_DIS_CFG * )ptzRecv->pvStruct;

	// Turn the message count field to zero to indicate a command
	ptzEnDisCfg->ucMsgCount = 0;

	// Turn the identify feature on or off as required
	if ( bEnable )
	{
	    ptzEnDisCfg->teIdentEnDis = XB_eCMD_PAIR_ON;
	}
	else
	{
	    ptzEnDisCfg->teIdentEnDis = XB_eCMD_PAIR_OFF;
	}

	// Send back to the device
	XBMSG_fnSend(XB_PGN_EN_DIS_CFG,
		     ptzEnDisCfg,
		     sizeof( *ptzEnDisCfg ),
		     ucAddr );

	// Wait for acknowledgement
	fnWaitForAck( ucAddr, XB_PGN_EN_DIS_CFG );

	XBMSG_fnDestroyRecvStruct( ptzRecv );
    }
    else if ( ptzRecv->ulPgn == XB_PGN_ISO_ACK )
    {
	// Probably a NAK
	printf( "ISO Ack received after requesting EnDisCfg\n" );
	XBMSG_fnDestroyRecvStruct( ptzRecv );
    }
}

/*******************************************************************************

FUNCTION NAME: fnSetMode

PURPOSE:
    Send a control command message telling the device to go into a new mode

INPUTS:
    ucAddr    - Node address of the device under control
    teNewMode - Mode to command the device into
     
OUTPUTS:
    None

NOTES:

*******************************************************************************/

void fnSetMode( uchar8 ucAddr, XB_teCTRL_MODE teNewMode )
{
    XB_tzPGN_CTRL_CMD tzCtrlCmd;

    // Make up a CtrlCmd message
    tzCtrlCmd.teDesiredMode = teNewMode;

    // Send away
    XBMSG_fnSend( XB_PGN_CTRL_CMD, &tzCtrlCmd, sizeof( tzCtrlCmd ), ucAddr );
    fnWaitForAck( ucAddr, XB_PGN_CTRL_CMD );
}

/*******************************************************************************

FUNCTION NAME: tmain

PURPOSE:
    The main program.  This program initializes the XanBus connection,
    waits two seconds to see which devices are connected and then
    plays with the first device that it discovers.  The discovered device
    will have its Identify feature turned on for a few seconds while
    its status is reported as its mode is switched to safe and then to operating.

INPUTS:
    None
     
OUTPUTS:
    None

NOTES:

*******************************************************************************/

int main( int argc, char* argv[] )
{
    uchar8 ucFirstAddr;

    // Initialize the XanBus interface
    printf( "Initializing XanBus\n" );
//    if ( !fnXbInit( 0, 0, XB_eFUNC_CODE_DIAGNOSTIC_TERMINAL) )
        if ( !fnXbInit( 0, 0, XB_eFUNC_CODE_USER_INTERFACE ) )
    {
	printf( "Initialization failed\n" );
	return 0;
    }
    
    // List the devices on the network
    sleep( 10 );
    printf( "Scanning for devices...\n" );
    ucFirstAddr = fnScanDevices();

    if ( ucFirstAddr != 0xff )
    {
	// Enable the Identify feature
	fnIdentify( ucFirstAddr, TRUE );

	// Start listening to broadcast of Sts PGN
	fnListen( XB_PGN_STS, TRUE, fnRecvSts );

	// Wait five seconds
	sleep( 2 );
	fnSetMode( ucFirstAddr, XB_eCTRL_MODE_SAFE );
	sleep( 2 );
	fnSetMode( ucFirstAddr, XB_eCTRL_MODE_OPERATING );
	sleep( 2 );

	// Stop listening for Sts PGN
	fnListen( XB_PGN_STS, FALSE, NULL );
	
	// Turn off the identify feature
	fnIdentify( ucFirstAddr, FALSE );
    }
    
    // Close the connection and quit
    printf( "Quitting...\n" );
    fnXbClose();
    return 0;
}

