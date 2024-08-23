/*
***************************************************************************
** 
** PROJECT: TIS NMEA 2000 Communications
**
** 
** DESCRIPTION: Header file for the message handler functions.
**
**
**
**
**
**            Copyright (c) 2001 Teleflex Marine/Industrial
**                       All Rights Reserved
**   Use, duplication, or disclosure of this material, in any
**   form, is forbidden without permission from Teleflex Marine
**
CHANGE HISTORY:
$Log: msghandl.h $

    ***********************************************
    Revision: XanBus_HenryW/1
    User: HenryW     Date: 08/19/05  Time: 08:07PM
    - cleaned up history list.

    ***********************************************
    Revision: GateWayPfrmANMEA_DaleM/1
    User: DaleM     Date: 02/10/05  Time: 07:15PM
    Cleaned up old history list
    
***************************************************************************/

#include "canbuf.h"         // Interface to CAN frame buffering

  // Max number of message handles.  This is set to the number of messages
  // that can go the the circular buffers.
#define NUM_MSG_HANDLES ( CANBUF_TX_BUF_COUNT )


// Structure of message handle information.
typedef struct
  {
    // Number of messages transmitted using this handle.  For single
    // frames, this will always be 1 when transmission is complete.
    // For fast-packet, this will be the number of frames in the fast-packet
    // when the tranmission is complete.
  INT16 iNumFramesTransmitted;
  UINT16 uiStatus; // Status of transmission.  This will only be valid
                   // when the tranmission is complete.
  BOOL bAvailable; // TRUE - message handle is available.
  } MSG_HANDLE;


/////////////////////////////////////////////////////////////////////
// Function prototypes
  // Internal functions
INT16 iGet_msg_handle(INT16 *piHandle);
MSG_HANDLE *pmhGet_handle_ptr(INT16 iHandle);



