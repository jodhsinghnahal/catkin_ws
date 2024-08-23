/*
***************************************************************************
** 
** PROJECT: CAN Library
**
** 
** DESCRIPTION: Environmental data for comm library
**
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
$Log: libenv.h $

    ***********************************************
    Revision: XanBus_HollyZ/2
    User: HollyZ     Date: 05/25/06  Time: 07:12PM
    Revert Xb stack to V1.2.7

    ***********************************************
    Revision: XanBus_HenryW/1
    User: HenryW     Date: 08/19/05  Time: 08:07PM
    - cleaned up history list.

    ***********************************************
    Revision: XanBus_HollyZ/1
    User: HollyZ     Date: 06/03/05  Time: 10:21PM
    Changed CANPORT type from SINT8 to UINT8

    ***********************************************
    Revision: GateWayPfrmANMEA_DaleM/1
    User: DaleM     Date: 02/10/05  Time: 07:13PM
    Cleaned up old history list
    
***************************************************************************/

#ifndef ENVIRON_H
#define ENVIRON_H

#include <stdlib.h>
#include "LIB_stddefs.h"

#ifdef _PCPLATFORM_
#define DllImport   __declspec( dllimport )
#define DllExport   __declspec( dllexport )
#else
#define DllImport
#define DllExport
#endif

#include "LibCfg.h"

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif


#ifndef NULL
#define NULL    0
#endif

#ifndef BOOL
#define BOOL    int
#endif
#define CHAR8   char
#define tfxINT8 char
#define INT16   short
#define INT32   long
#define UINT8   unsigned char
#define UINT16  unsigned short
#define UINT32  unsigned long

typedef enum
{
    TFXCR_OK,
    TFXCR_IDLE,            // internal operation

    TFXCR_TABLE_FULL,      // referenced table is full
    TFXCR_HEAP_ALLOC_FAIL, // a request to allocate heap failed
    TFXCR_NOT_FOUND,       // requested item was not found

    TFXCR_NEW_DATA,        // New Data from routine
    TFXCR_NO_DATA,         // No Data from routine
    TFXCR_INVALID_PORT,    // Invalid Comm Port
    TFXCR_BUS_OFF,		   // CAN Bus is in Bus-Off condition (no further transmissions possible)
    TFXCR_BUS_WARNING,     // CAN Bus is at Error Warning Level Status
    TFXCR_MSG_NOT_HANDLED, // Message was not handled by routine 

    TFXCR_LIB_BUSY,        // tick was called while library was already running
    
    TFXCR_DRV_BUSY,        // Driver busy
    TFXCR_DRV_RX_BUFFER_ALLOCATION_FAILURE, // Allocation failure for Receive Buffer 
    TFXCR_DRV_TX_BUFFER_ALLOCATION_FAILURE, // Allocation failure for Transmit Buffer
    TFXCR_MSG_HANDLE_NOT_AVAILABLE,     // all message handles are used up
    TFXCR_INVALID_MSG_HANDLE,
    TFXCR_PORT_INIT_ERR, // Got an error while initializing a port.
    TFXCR_COUNT
} TFXCAN_RETURNS;
 


#define PGN        INT32
#ifndef CANPORT
#define CANPORT    tfxINT8
#endif

#ifndef CANBUF
#define CANBUF     tfxINT8
#endif

#define LOCAL      static
#define GLOBAL     





#endif


