/*==============================================================================
Copyright (c) 2020 Xantrex Canada, ULC. All rights reserved.

This source file and the information contained in it are confidential and 
proprietary to Xantrex Canada, ULC. The reproduction or disclosure, in 
whole or in part, to anyone outside of Xantrex without the written approval of 
a Xantrex officer under a Non-Disclosure Agreement, or to any employee of 
Xantrex who has not previously obtained written authorization for access from 
the individual responsible for the source code, will have a significant 
detrimental effect on Xantrex and is expressly prohibited.

FILE NAME:  CDrive.c

PURPOSE:
    This file provides the interface bewteen the XanBus stack and the
    'isCAN' USB Drive Library supplied by ifak systems.
    This interface is set up to sustitute for the interface to the CAN
    hardware supplied by Teleflex.

FUNCTION(S):
    TFXDRV_i16InitBuffers       - Init buffers (not used)
    TFXDRV_i16InitController    - Initialize CAN controller
    TFXDRV_i16ControllerState   - Get CAN controller state
    TFXDRV_i16ReceiveFrameCheck - Check somthing in received frame (not used)
    TFXDRV_i16ReceiveFrame      - Receive CAN message from CAN controller
    TFXDRV_i16SendFrameCheck    - Check if frame can be sent (not used)
    TFXDRV_i16SendFrame         - Send CAN message to CAN controller
    TFXDRV_WriteFrame           - Write data to CAN controller (not used)
   
    local:
        FrameToBuffer - format data from Teleflex frame to CAN controller
        BufferToFrame - format data from CAN controller to Teleflex frame
        bIsPortValid  - check if port number is valid
   
    
==============================================================================*/


/*==============================================================================
                              Includes
==============================================================================*/

#include <stdio.h>
#include <string.h>

#include "xbudefs.h"
#include "xbgdefs.h"

#include "libenv.h"         // Teleflex interface
#include "cdrive.h"         // Interface to this file
#include "canbuf.h"

#ifndef NO_CANUSB
#include "canusb.h"         // CANUSB interface
#endif
#ifndef NO_CANVCP
#include "canvcp.h"         // CANUSB via virtual comm port interface
#endif
#ifndef NO_SOCKETCAN
#include "socketcan.h"      // Linux socketcan interface
#endif
#ifndef NO_XANBR
#include "xanbr.h"          // TCP XanBridge interface
#endif
#ifndef NO_UDPCAN
#include "udpcan.h"         // Simulated CAN over UDP
#endif
#ifndef NO_IFAK
#include "ifakdrv.h"
#endif
#ifndef NO_APOX
#include "apoxdrv.h"
#endif
#ifndef NO_CANCARDX
#include "vector.h"
#endif
#include "candrv.h"         // Low level interface, used for bit rates



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


/*==============================================================================
                           Local/Private Constants
==============================================================================*/



#define  USING_NONE             0
#define  USING_IFAK_USB         1
#define  USING_VECTOR_CANCARDX  2
#define  USING_APOX_USBCAN      3
#define  USING_TCP              4
#define  USING_CANUSB           5
#define  USING_CANVCP           6
#define  USING_SOCKETCAN        7
#define  USING_UDP              8


/*==============================================================================
                           Local/Private Variables
==============================================================================*/

// Active interface ifak_usb or CanCardX
static uchar8 ucUsingInterface = USING_NONE;

// Array of communications statistics records
static CAN_tzCOMM_STATS atzCanCommStats[ MAX_PORTS ];

// Last reported bus status
static uchar8 ucBusStatus = USING_NONE;


/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/


static BOOL bIsPortValid( CANPORT u8PortNumber );




/*==============================================================================
                           Function Definitions
==============================================================================*/

/*******************************************************************************

FUNCTION NAME: 
    TFXDRV_i16InitBuffers
  
PURPOSE: 
    Initialize buffers for holding CAN messages.
    NOT USED BY isCAN

INPUTS: 
    'u8PortNumber' is number representing the device hardware port

OUTPUTS:
    returns TFXCR_OK

NOTES:

VERSION HISTORY:

Version: 1.00  Date: OCT-03-2003  By: dalem
    - Created

*******************************************************************************/

TFXCAN_RETURNS TFXDRV_i16InitBuffers(CANPORT u8PortNumber )
{
    // Teleflex message buffers not used
    return TFXCR_OK;
}

/*******************************************************************************

FUNCTION NAME: 
    TFXDRV_i16InitController
  
PURPOSE: 
    Initialize CAN controller hardware.

INPUTS: 
    'u8PortNumber' is number representing the device hardware port

OUTPUTS:
    returns TFXCR_OK on success
    returns TFXCR_INVALID_PORT if port is invalid
    returns TFXCR_PORT_INIT_ERR and prints diagnostic for other errors

NOTES:

VERSION HISTORY:

Version: 1.00  Date: DEC-18-2003  By: dalem
    - Created from ifak and CANcardX versions

Version: 1.10  Date: APR-20-2007  By: Trevor Monk
	- Added support for a virtual CAN interface over TCP

*******************************************************************************/

TFXCAN_RETURNS TFXDRV_i16InitController(CANPORT u8PortNumber,
                                        uchar8 ucBitRate)
{

    // check port and initialize appropriate controller
    if (!bIsPortValid(u8PortNumber))
        return TFXCR_INVALID_PORT;


#ifdef WIN32
#ifndef NO_APOX
    // If no interface yet
    if ( ucUsingInterface == USING_NONE )
    {
        if ( APOXDRV_fnInit( ucBitRate ) )
        {
            ucUsingInterface = USING_APOX_USBCAN;
        }
    }
#endif // NO_APOX
    // If no interface yet
    if ( ucUsingInterface == USING_NONE )
    {
        if ( IFAKDRV_fnInit(  ucBitRate ) )
        {
            ucUsingInterface = USING_IFAK_USB;
        }
    }

#ifndef NO_CANCARDX
    // If no interface yet
    if ( ucUsingInterface == USING_NONE )
    {
        if ( VECTOR_fnInit( ucBitRate ) )
        {
            ucUsingInterface = USING_VECTOR_CANCARDX;
        }
    }
#endif // NO_CANCARDX

#endif // ifdef WIN32

#ifndef NO_SOCKETCAN
    // Try the socketcan interface
    if ( ucUsingInterface == USING_NONE )
    {
        if ( SOCKETCAN_fnInit( ucBitRate ) )
        {
            printf( "Using socketcan interface\r\n" );
            ucUsingInterface = USING_SOCKETCAN;
        }
    }
#endif // NO_SOCKETCAN

#ifndef NO_CANUSB
    // Try the CANUSB interface
    if ( ucUsingInterface == USING_NONE )
    {
        if ( CANUSB_fnInit( ucBitRate ) )
        {
            printf( "Using CANUSB interface\r\n" );
            ucUsingInterface = USING_CANUSB;
        }
    }
#endif // NO_CANUSB

#ifndef NO_CANVCP
    // Try the CANVCP interface
    if ( ucUsingInterface == USING_NONE )
    {
        if ( CANVCP_fnInit( ucBitRate ) )
        {
            printf( "Using CANVCP interface\r\n" );
            ucUsingInterface = USING_CANVCP;
        }
    }
#endif // NO_CANVCP

#ifndef NO_XANBR
    // If no interface yet
    if ( ucUsingInterface == USING_NONE )
    {
        if( XANBR_fnInit() )
        {
            //printf( "Using Virtual CAN TCP interface\r\n" );
            ucUsingInterface = USING_TCP;
        }
    }
#endif // NO_XANBR

#ifndef NO_UDPCAN
    if ( ucUsingInterface == USING_NONE )
    {
        if ( UDPCAN_fnInit() )
        {
            printf( "Using virtual CAN UDP interface\r\n" );
            ucUsingInterface = USING_UDP;
        }
    }
#endif
    
    // If no interface yet
    if ( ucUsingInterface == USING_NONE )
    {
        // Could not open any interface
        fprintf( stderr, "Could not open any CAN interface device\r\n" );

        // Failure
        return TFXCR_PORT_INIT_ERR;
    }

    // Success
    return TFXCR_OK;
}

/*******************************************************************************

FUNCTION NAME: 
    TFXDRV_CloseDriver
  
PURPOSE: 
    Close the isCAN driver.

INPUTS: 
    None

OUTPUTS:
    Handle is made invalid

NOTES:

VERSION HISTORY:

Version: 1.00  Date: DEC-18-2003  By: dalem
    - Created from ifak and CANcardX versions

Version: 1.10  Date: APR-20-2007  By: Trevor Monk
	- Added support for a virtual CAN interface over TCP

*******************************************************************************/

void TFXDRV_CloseDriver( void )
{
    switch ( ucUsingInterface )
    {

#ifdef WIN32
#ifndef NO_IFAK
        case USING_IFAK_USB:
        {
            IFAKDRV_fnClose();
        }
        break;
#endif
#ifndef NO_CANCARDX
        case USING_VECTOR_CANCARDX:
        {
            VECTOR_fnClose();
        }
        break;
#endif // NO_CANCARDX
#ifndef NO_APOX
        case USING_APOX_USBCAN:
        {
            APOXDRV_fnClose();
        }
        break;
#endif // NO_APOX
#endif // ifdef WIN32

#ifndef NO_CANUSB
    case USING_CANUSB:
    {
        CANUSB_fnClose();
    }
    break;
#endif // NO_CANUSB

#ifndef NO_CANVCP
    case USING_CANVCP:
    {
        CANVCP_fnClose();
    }
    break;
#endif // NO_CANVCP

#ifndef NO_SOCKETCAN
    case USING_SOCKETCAN:
    {
        SOCKETCAN_fnClose();
    }
    break;
#endif // NO_SOCKETCAN

#ifndef NO_UDPCAN
    case USING_UDP:
    {
        UDPCAN_fnClose();
    }
    break;
#endif // NO_UDPCAN
    
#ifndef NO_XANBR
    case USING_TCP:
    {
        XANBR_fnClose();
    }		
    break;
#endif // NO_XANBR
    default:
	// Nothing to close
	break;
    }

    // Flag that the driver is no longer in use
    ucUsingInterface = USING_NONE;
}

/*******************************************************************************

FUNCTION NAME: 
    TFXDRV_i16ControllerState
  
PURPOSE: 
    Return controller state information

INPUTS: 
    'u8PortNumber' is number representing the device hardware port

OUTPUTS:
    returns TFXCR_OK

NOTES:

VERSION HISTORY:

Version: 1.00  Date: DEC-18-2003  By: dalem
    - Created from ifak and CANcardX versions

Version: 1.10  Date: APR-20-2007  By: Trevor Monk
	- Added support for a virtual CAN interface over TCP

*******************************************************************************/

TFXCAN_RETURNS TFXDRV_i16ControllerState(CANPORT u8PortNumber)
{
    // check port and initialize appropriate controller
    if (!bIsPortValid(u8PortNumber))
        return TFXCR_INVALID_PORT;

    // Action depends on interface
    switch ( ucUsingInterface )
    {
#ifdef WIN32
        case USING_IFAK_USB:
        {
            return IFAKDRV_fnStatus();
        }

#ifndef NO_CANCARDX
        case USING_VECTOR_CANCARDX:
        {
            return VECTOR_fnStatus();
        }
#endif
#ifndef NO_APOX
        case USING_APOX_USBCAN:
        {
            return APOXDRV_fnStatus();
        }
#endif // NO_APOX
#endif  // ifdef WIN32

#ifndef NO_CANUSB
    case USING_CANUSB:
        return CANUSB_fnStatus();
#endif
#ifndef NO_CANVCP
    case USING_CANVCP:
        return CANVCP_fnStatus();
#endif
#ifndef NO_SOCKETCAN
    case USING_SOCKETCAN:
        return SOCKETCAN_fnStatus();
#endif
#ifndef NO_UDPCAN
    case USING_UDP:
        return UDPCAN_fnStatus();
#endif
#ifndef NO_XANBR
    case USING_TCP:
        return XANBR_fnStatus();
#endif
    default:
        // Nothing to do
        return TFXCR_BUS_OFF;
    }
}

/*******************************************************************************

FUNCTION NAME: 
    TFXDRV_i16ConnectController
  
PURPOSE: 
    Reinitialize CAN controller 

INPUTS: 
    'u8PortNumber' is number representing the device hardware port

OUTPUTS:
    returns result of TFXDRV_i16InitController

NOTES:

VERSION HISTORY:

Version: 1.00  Date: Feb 27, 2003  By: dalem
    - first version

*******************************************************************************/

TFXCAN_RETURNS TFXDRV_i16ConnectController(CANPORT u8PortNumber,
                                           uchar8  ucBitRate)
{
    // Initialize Controller
    return TFXDRV_i16InitController(u8PortNumber,
                                    ucBitRate);
}

/*******************************************************************************

FUNCTION NAME:
    TFXDRV_fnTickStatus     - Provide tick for CAN status processing

PURPOSE:
    This function is use to provide a time tick to CAN status processing.
    The CAN controller will quickly switch into and out of Bus Off when it
    should just stay in the Bus Off condition. This function is used to
    provide time filtering for Bus Off status.

INPUTS:
    Channel is the number of the CAN port being serviced

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 01/10/05  By: dalem
    - Created

*******************************************************************************/

void TFXDRV_fnStatusTick( CANPORT Channel )
{
    return;
}

/*******************************************************************************

FUNCTION NAME:
    TFXDRV_fnIsBlocked      - Check is CAN channel is blocked

PURPOSE:
    This function is use to return the blocked status of a channel.

INPUTS:
    Channel is the number of the CAN port being checked

OUTPUTS:
    TRUE if CAN port is blocked (cannot accept any more frames)
    FALSE if frames can be sent

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 01/10/05  By: dalem
    - Created

*******************************************************************************/

tucBOOL TFXDRV_fnIsBlocked( CANPORT Channel )
{
    return ( FALSE );
}

/*******************************************************************************

FUNCTION NAME: 
    TFXDRV_fnGetComStats
  
PURPOSE: 
    Return current value of the CAN communications statistics counters

INPUTS: 
    'Channel' is the channel number, usually zero
    'ptzStats' is a pointer to a location to copy the statistics into

OUTPUTS:
    TFXCR_OK if all goes well

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 03/03/03  By: dalem
    - Creation

*******************************************************************************/

TFXCAN_RETURNS TFXDRV_fnGetCommStats( uchar8 Channel, CAN_tzCOMM_STATS *ptzStats )
{
    // Validate channel number
    if ( Channel >= MAX_PORTS )
    {
        //  Index out of range
        return TFXCR_INVALID_PORT;
    }

    // Get copy of statistics
    *ptzStats = atzCanCommStats[ Channel ];

    // Success
    return TFXCR_OK;
}

/*******************************************************************************

FUNCTION NAME: 
    TFXDRV_fnClearComStats
  
PURPOSE: 
    Clear communications statistics counters

INPUTS: 
    Channel is the channel number, usually zero

OUTPUTS:
    Clears communications statistics counters
    and returns TFXCR_OK if all goes well

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 03/03/03  By: dalem
    - Creation

*******************************************************************************/

TFXCAN_RETURNS TFXDRV_fnClearCommStats( uchar8 Channel )
{
    // Validate channel number
    if ( Channel >= MAX_PORTS )
    {
        //  Index out of range
        return TFXCR_INVALID_PORT;
    }

    // Clear communications statics for the channel
    memset( &atzCanCommStats[ Channel ], 0, sizeof( CAN_tzCOMM_STATS ) );

    // Success
    return TFXCR_OK;
}

/*******************************************************************************

FUNCTION NAME: 
    TFXDRV_i16ReceiveFrameCheck
  
PURPOSE: 
    Check something in received frame
    NOT USED BY isCAN, NOT USED BY TELEFLEX

INPUTS: 
    'u8PortNumber' is number representing the device hardware port

OUTPUTS:
    returns TFXCR_OK

NOTES:

VERSION HISTORY:

Version: 1.00  Date: Feb 27, 2003  By: dalem
    - first version

*******************************************************************************/

TFXCAN_RETURNS TFXDRV_i16ReceiveFrameCheck( CANPORT u8PortNumber )
{
    // check port
    if (!bIsPortValid(u8PortNumber))
        return TFXCR_INVALID_PORT;
    return TFXCR_OK;
}



/*******************************************************************************

FUNCTION NAME: 
    TFXDRV_i16ReceiveFrame
  
PURPOSE: 
    Poll for a received frame

INPUTS: 
    'u8PortNumber' is number representing the device hardware port
    'pFrame' is a pointer to copy CAN message into

OUTPUTS:
    returns TFXCR_INVALID_PORT if port is not supported
    returns TFXCR_NEW_DATA if data is read and ready
    returns TFXCR_NO_DATA if no data available
    returns TFXCR_MSG_NOT_HANDLED and prints diagnostic if some other error

NOTES:

VERSION HISTORY:

Version: 1.00  Date: DEC-18-2003  By: dalem
    - Created from ifak and CANcardX versions

Version: 1.10  Date: APR-20-2007  By: Trevor Monk
	- Added support for a virtual CAN interface over TCP

*******************************************************************************/

TFXCAN_RETURNS TFXDRV_i16ReceiveFrame( CANPORT u8PortNumber, CANFRAME *pFrame )
{
    if (!bIsPortValid(u8PortNumber))
        return TFXCR_INVALID_PORT;

    // Action depends on interface
    switch ( ucUsingInterface )
    {
#ifdef WIN32
#ifndef NO_IFAK
        case USING_IFAK_USB:
        {
            // Receive ifak frame
            return IFAKDRV_fnReceiveFrame( u8PortNumber, pFrame );
        }
#endif
#ifndef NO_CANCARDX
        case USING_VECTOR_CANCARDX:
        {
            // Receive Vector frame
            return VECTOR_fnReceiveFrame( u8PortNumber, pFrame );
        }
#endif // ndef NO_CANCARDX
#ifndef NO_APOX
        case USING_APOX_USBCAN:
        {
            // Receive Apox frame
            return APOXDRV_fnReceiveFrame( u8PortNumber, pFrame );
        }
#endif // ndef NO_APOX
#endif // ifdef WIN32

#ifndef NO_CANUSB
    case USING_CANUSB:
    {
        // Recieve CANUSB frame
        return CANUSB_fnReceiveFrame( u8PortNumber, pFrame );
    }
#endif
#ifndef NO_CANVCP
    case USING_CANVCP:
    {
        // Recieve CANVCP frame
        return CANVCP_fnReceiveFrame( u8PortNumber, pFrame );
    }
#endif
#ifndef NO_SOCKETCAN
    case USING_SOCKETCAN:
    {
        // Recieve socketcan frame
        return SOCKETCAN_fnReceiveFrame( u8PortNumber, pFrame );
    }
#endif
#ifndef NO_UDPCAN
    case USING_UDP:
    {
        return UDPCAN_fnReceiveFrame( u8PortNumber, pFrame );
    }
#endif
#ifndef NO_XANBR
    case USING_TCP:
    {
        // Receive using virtual CAN port over TCP
        return XANBR_fnReceiveFrame( u8PortNumber, pFrame );
    }
#endif
    default:
        // Nothing to do
        return TFXCR_NO_DATA;
    }
}

//**********************************************************************
// TFXDRV_i16ReceiveForeignFrame
//**********************************************************************
TFXCAN_RETURNS TFXDRV_i16ReceiveForeignFrame( CANPORT u8PortNumber,
                                              CANDRV_tzCAN_FRAME *pi16Frame )
{
    if (!bIsPortValid(u8PortNumber))
        return TFXCR_INVALID_PORT;
    // check for port and buffer for data
    
    // not support yet
    return TFXCR_NO_DATA;
    
}
/*******************************************************************************

FUNCTION NAME: 
    TFXDRV_i16SendFrameCheck
  
PURPOSE: 
    Check on sending of CAN frames
    NOT USED BY isCAN

INPUTS: 
    'u8PortNumber' is number representing the device hardware port

OUTPUTS:
    returns TFXCR_OK

NOTES:

VERSION HISTORY:

Version: 1.00  Date: Feb 27, 2003  By: dalem
    - first version

*******************************************************************************/

TFXCAN_RETURNS TFXDRV_i16SendFrameCheck(CANPORT u8PortNumber)
{
    if (!bIsPortValid(u8PortNumber))
        return TFXCR_INVALID_PORT;

    // All OK return
    return TFXCR_OK;
}


//**********************************************************************
// 
//**********************************************************************
TFXCAN_RETURNS TFXDRV_i16SendForeignFrame( CANPORT u8PortNumber, 
                                           CANDRV_tzCAN_FRAME *pFrame, 
                                           INT16 *pi16MessageID )
{
    // not support yet
    return TFXCR_MSG_NOT_HANDLED;
}



/*******************************************************************************

FUNCTION NAME: 
    TFXDRV_i16SendFrame
  
PURPOSE: 
    Queue a CAN frame for transmission

INPUTS: 
    'u8PortNumber' is number representing the device hardware port
    'pFrame' is a pointer to the CAN data be be sent
    'pi16MessageID' is a pointer to field to return a buffer handle (NOT USED)

OUTPUTS:
    returns TFXCR_INVALID_PORT if port is invalid
    returns TFXCR_MSG_NOT_HANDLED if isCAN return an error
    returns TFXCR_OK if message queue for sending

NOTES:

VERSION HISTORY:

Version: 1.00  Date: DEC-18-2003  By: dalem
    - Created from ifak and CANcardX versions

Version: 1.10  Date: APR-20-2007  By: Trevor Monk
	- Added support for a virtual CAN interface over TCP

*******************************************************************************/

TFXCAN_RETURNS TFXDRV_i16SendFrame( CANPORT u8PortNumber,
                                    CANFRAME *pFrame,
                                    INT16 *pi16MessageID )
{
    // Action depends on interface
    switch ( ucUsingInterface )
    {
#ifdef WIN32
#ifndef NO_IFAK
        case USING_IFAK_USB:
        {
            // Send ifak frame
            return IFAKDRV_fnSendFrame( u8PortNumber, pFrame, pi16MessageID );
        }
#endif
#ifndef NO_CANCARDX
        case USING_VECTOR_CANCARDX:
        {
            // Send Vector frame
            return VECTOR_fnSendFrame( u8PortNumber, pFrame, pi16MessageID );
        }
#endif

#ifndef NO_APOX
        case USING_APOX_USBCAN:
        {
            // Send Apox frame
            return APOXDRV_fnSendFrame( u8PortNumber, pFrame, pi16MessageID );
        }
#endif // ndef NO_APOX
#endif // ifdef WIN32

#ifndef NO_CANUSB
    case USING_CANUSB:
    {
        // Send a CANUSB frame
        return CANUSB_fnSendFrame( u8PortNumber, pFrame, pi16MessageID );
    }
#endif
#ifndef NO_CANVCP
    case USING_CANVCP:
    {
        // Send a CANVCP frame
        return CANVCP_fnSendFrame( u8PortNumber, pFrame, pi16MessageID );
    }
#endif
#ifndef NO_SOCKETCAN
    case USING_SOCKETCAN:
    {
        // Send a socketcan frame
        return SOCKETCAN_fnSendFrame( u8PortNumber, pFrame, pi16MessageID );
    }
#endif
#ifndef NO_UDPCAN
    case USING_UDP:
    {
        return UDPCAN_fnSendFrame( u8PortNumber, pFrame, pi16MessageID );
    }
#endif
#ifndef NO_XANBR
    case USING_TCP:
    {
	// Send TCP frame
        return XANBR_fnSendFrame( u8PortNumber, pFrame, pi16MessageID );
    }
#endif
    default:
        // Nothing to do
        return TFXCR_MSG_NOT_HANDLED;
    }
}


/*******************************************************************************

FUNCTION NAME: 
    TFXDRV_WriteFrame
  
PURPOSE: 
    Remove frame from buffer and send to CAN controller
    NOT USED BY isCAN

INPUTS: 
    'u8PortNumber' is number representing the device hardware port

OUTPUTS:
    returns TFXCR_NO_DATA

NOTES:

VERSION HISTORY:

Version: 1.00  Date: Feb 27, 2003  By: dalem
    - first version

*******************************************************************************/

TFXCAN_RETURNS TFXDRV_WriteFrame( CANPORT u8PortNumber )
{
    if ( !bIsPortValid( u8PortNumber ) )
        return TFXCR_INVALID_PORT;

    // Teleflex message buffers not used
    return TFXCR_NO_DATA;
}



/*******************************************************************************

FUNCTION NAME: 
    bIsPortValid
  
PURPOSE: 
    Check if port is supported

INPUTS: 
    'u8PortNumber' is number representing the device hardware port

OUTPUTS:
    returns TRUE if port is valid
            FALSE if port is not valid

NOTES:

VERSION HISTORY:

Version: 1.00  Date: Feb 27, 2003  By: dalem
    - first version

*******************************************************************************/

static BOOL bIsPortValid(CANPORT u8PortNumber)
{
    return u8PortNumber < MAX_PORTS ? TRUE : FALSE;
}



/*******************************************************************************

FUNCTION NAME:
    TFXDRV_fnSleep

PURPOSE:
    Put CAN channel to sleep so that the CPU can use less power.

INPUTS:
    Channel is the number of the CAN module to put to sleep, usually zero.
    WakeEnable is TRUE if the module is to automatically wake up if there
        is traffic on the CAN bus.

OUTPUTS:
    TFXCR_OK if all goes well

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 10/17/03  By: Peter Drexel
    - Creation

*******************************************************************************/

TFXCAN_RETURNS TFXDRV_fnSleep( uchar8 Channel,
                                uchar8 WakeEnable )
{
    return( TFXCR_OK );
}

/*******************************************************************************

FUNCTION NAME:
    TFXDRV_fnInstallWakeupCb

PURPOSE:
    Install callback for handling wakeup interrupts

INPUTS:
    Channel is the number of the CAN module to use, usually zero.
    pFunc is the pointer to the callback function.

OUTPUTS:
    Returns TFXCR_OK on completion, or a failure code.
    

NOTES:
    The callback functions are called while the interrupt is being
    handled so they should not do very much processing, mostly on the
    order of queueing buffers and setting flags for task-level
    processing later.

VERSION HISTORY:

Version: 1.00  Date: 10/17/03  By: Peter Drexel
    - Creation from equivalent CANDRV function

*******************************************************************************/

TFXCAN_RETURNS TFXDRV_fnInstallWakeupCb( uchar8 Channel,
                                    TFXDRV_tpCALLBACK pFunc )
{
    return( TFXCR_OK );
}

/*******************************************************************************

FUNCTION NAME:
    TFXDRV_fnWakeUp

PURPOSE:
    Wake up CAN channel.

INPUTS:
    Channel is the number of the CAN module to put to wakeup, usually zero.

OUTPUTS:
    TFXCR_OK if all goes well

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 04/15/04  By: Peter Drexel
    - Creation

*******************************************************************************/

TFXCAN_RETURNS TFXDRV_fnWakeUp( uchar8 Channel )
{
    return( TFXCR_OK );
}

