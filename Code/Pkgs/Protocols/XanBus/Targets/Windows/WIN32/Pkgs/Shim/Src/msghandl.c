/*==============================================================================
Copyright © 2002 Xantrex Technology, Inc. All rights reserved.

This source file and the information contained in it are confidential and 
proprietary to Xantrex Technology, Inc. The reproduction or disclosure, in 
whole or in part, to anyone outside of Xantrex without the written approval of 
a Xantrex officer under a Non-Disclosure Agreement, or to any employee of 
Xantrex who has not previously obtained written authorization for access from 
the individual responsible for the source code, will have a significant 
detrimental effect on Xantrex and is expressly prohibited.

FILE NAME:  msghandl.c

PURPOSE:
    This file provides stubs for functions called by Teleflex but not used
    in CANcardX interface. This file is part of the interface bewteen the
    XanBus stack and the CAN Drive Library supplied by Vector Informatik
    for the CANcardX. This interface is set up to sustitute for the interface
    to the CAN hardware as supplied by Teleflex.

FUNCTION(S):
    TFXDRV_vInit_msg_handles - Initialize message handle data structures
    iGet_msg_handle          - Get a message handle
    TFXDRV_iClear_msg_handle - Free a message handle
    TFXDRV_iGet_msg_status   - Get status for a message handle
   
    local:
   
NOTES:
    #$-VIEW:C Coding Standard-$#
    **If you do not see a button above this, either use CodeWright or ignore 
    **the text.
    
    <note description>

CHANGE HISTORY :
$Log: msghandl.c $

    ***********************************************
    Revision: XanBus4_TomL/1
    User: TomL     Date: 01/16/10  Time: 02:00AM
    Corrected case of some include files
    
    *****************  Version 1  *****************
    User: Dalem        Date: 2/27/03    Time: 1:26p
    Created in $/PD/Common/XanBus/Code/Common/CAN Driver/CANcardX


==============================================================================*/


/*==============================================================================
                              Includes
==============================================================================*/

#include "libenv.h"
#include "msghandl.h"
#include "cdrive.h"

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

/*==============================================================================
                           Local/Private Variables
==============================================================================*/

/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/

/*==============================================================================
                           Function Definitions
==============================================================================*/

void TFXDRV_vInit_msg_handles( void )
{
}

INT16 iGet_msg_handle( INT16 *piHandle )
{
     return TFXCR_INVALID_MSG_HANDLE; // Invalid message handle.
}

TFXCAN_RETURNS  TFXDRV_iClear_msg_handle( INT16 iHandle )
{
    return TFXCR_INVALID_MSG_HANDLE;
}

TFXCAN_RETURNS  TFXDRV_iGet_msg_status(INT16 iHandle, UINT16 *puiStatus, INT16 *piNumFramesTransmitted)
{
    return TFXCR_INVALID_MSG_HANDLE;
}

