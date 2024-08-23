/*
***************************************************************************
** 
** PROJECT: TIS NMEA 2000 Communications
**
** 
** DESCRIPTION: Manage message handles.
**   This module provides the routines for managing message handles.
**   Message handles operate as follows:
**     - A message handle contains information about the message:
**        > The number of frames transmitted using the message handle
**        > The status of the last frame transmission.
**     - This module maintains an array of message handles.  The handle
**       number is the array index.
**     - When application sends a message, the message gets assigned a
**       message handle.  If there are no available message handles left,
**       it is up to the application to free up a message handle to use.
**     - The application must check the status of the message by calling
**       iGet_msg_status() with the correct message handle.  When the number
**       of transmitted messages is what the application expects, then the
**       application can check the message status to determine if there were
**       any errors.
**   
**   If USE_MSG_HANDLES is defined on the compiler command line, message handles
**   will be used, otherwise the message handle functions will always return
**   success and the message handle will always be 0.
**
**
**
**
**            Copyright (c) 2001 Teleflex Marine/Industrial
**                       All Rights Reserved
**   Use, duplication, or disclosure of this material, in any
**   form, is forbidden without permission from Teleflex Marine
***************************************************************************
** .HISTORY OF REVISIONS
**
$Log: msghandl.c $

    ** ***********************************************
    ** Revision: GateWayPfrmANMEA_DaleM/1
    ** User: DaleM     Date: 02/10/05  Time: 10:26PM
    ** Cleaned up old history list
    ** 
** 
******************************************************************/
#define USE_MSG_HANDLES
#include "libenv.h"
#include "msghandl.h"
#include "cdrive.h"
#ifndef USE_MSG_HANDLES
//#include "CAN.h"
#include "cdrive.h"
#include "config.h"
//#include "CDrvAPI.h"
#endif

  // The handle number is the array index.
#ifdef USE_MSG_HANDLES
static MSG_HANDLE s_MsgHandles[NUM_TX_MSG_HANDLES];
#endif

void TFXDRV_vInit_msg_handles()
  {
#ifdef USE_MSG_HANDLES
  int iIndex;

  for(iIndex = 0; iIndex < NUM_TX_MSG_HANDLES; ++iIndex)
    {
    s_MsgHandles[iIndex].bAvailable = TRUE;
    s_MsgHandles[iIndex].iNumFramesTransmitted = 0;
    s_MsgHandles[iIndex].uiStatus = 0;
    }
#endif
  }

/********************************************************************
iGet_msg_handle - Either get a new message handle or validate an existing
                  message handle.
                    - If the input is INV_MSG_HANDLE, get a message handle 
                      and mark that message handle as not available (in use).
                    - If the input is not INV_MSG_HANDLE, validate the
                      message handle.
                        > It must be within the message handle array bounds.
                        > It must be marked as in use.

  inputs: Pointer to a message handle.
            This is INV_MSG_HANDLE if a new message handle is to be assigned
              to the location pointed to by the pointer.
            This is a message handle thas is being re-used, validate it.

  return: >= 0 indicates a valid message handle was available and has been
            assigned.
          TFXCF_MSG_HANDLE_NOT_AVAILABLE if there are no message handles
            available.
          TFXCR_INVALID_MSG_HANDLE if a message handle was input and it is
            invalid.
********************************************************************/
INT16 iGet_msg_handle(INT16 *piHandle)
  {
  INT16 iRet;
#ifdef USE_MSG_HANDLES
  INT16 iIndex;

  if(piHandle != NULL)
    { // Pointer to the message handle appears valid.
    if(*piHandle == INV_MSG_HANDLE)
      { // Find a message handle to return.
      iRet = TFXCR_MSG_HANDLE_NOT_AVAILABLE;
      for(iIndex = 0; iIndex < NUM_TX_MSG_HANDLES; ++iIndex)
        {
        if(s_MsgHandles[iIndex].bAvailable == TRUE)
          { // Found an available message handle.  Set it to in use and
            // make sure the members of the handle structure are
            // initialized.
          s_MsgHandles[iIndex].bAvailable = FALSE;
          s_MsgHandles[iIndex].iNumFramesTransmitted = 0;
          s_MsgHandles[iIndex].uiStatus = 0;
          *piHandle = iIndex; // Return the handle.
          iRet = TFXCR_OK;
          break;
          }
        }
      }
    else
      { // This handle has already been assigned, check to make sure its
        // within the message handle array bounds and marked not available.
      if(*piHandle >= 0 && *piHandle < NUM_TX_MSG_HANDLES
      && s_MsgHandles[*piHandle].bAvailable == FALSE)
        iRet = TFXCR_OK; // Handle is valid and in use.
      else
        iRet = TFXCR_INVALID_MSG_HANDLE; // Invalid message handle.
      }
    }
#else
  iRet = TFXCR_OK;
  *piHandle = 0;
#endif
  return(iRet);
  }

/********************************************************************
iClear_msg_handle - Mark the specified message handle as available.

  inputs: Message handle

  return: TFXCR_OK if message handle is valid and handle is cleared.
          TFXCR_INVALID_MSG_HANDLE if message handle is invalid.
********************************************************************/
TFXCAN_RETURNS  TFXDRV_iClear_msg_handle(INT16 iHandle)
{
#ifdef USE_MSG_HANDLES
  if(iHandle >= 0 && iHandle < NUM_TX_MSG_HANDLES)
    { // Handle is within the message handle array bounds.
    s_MsgHandles[iHandle].bAvailable = TRUE;
    s_MsgHandles[iHandle].iNumFramesTransmitted = 0;
    s_MsgHandles[iHandle].uiStatus = 0;
    return ( TFXCR_OK );
    }
  else
    return ( TFXCR_INVALID_MSG_HANDLE );
#else
  return ( TFXCR_OK );
#endif
}

/********************************************************************
iGet_msg_status - Get the status and number of frames transmitted of a 
                  message.

  inputs: Message handle
          Pointer to a variable that gets the status of the transmission.
            If pointer is NULL, caller gets no status.
          Pointer to a variable that gets the number of frames transmitted.
            If pointer is NULL, caller doesn'g get number of frames transmitted.

  return: TFXCR_OK if function is successful.
          TFXCR_INVALID_MSG_HANDLE if handle is invalid.
********************************************************************/
TFXCAN_RETURNS  TFXDRV_iGet_msg_status(INT16 iHandle, UINT16 *puiStatus, INT16 *piNumFramesTransmitted)
{
#ifdef USE_MSG_HANDLES
    if ( ( iHandle >= 0 && iHandle < NUM_TX_MSG_HANDLES )
     &&  ( s_MsgHandles[iHandle].bAvailable == FALSE ) )
    { // Message handle is valid.
        if(puiStatus != NULL)
            *puiStatus = s_MsgHandles[iHandle].uiStatus;
        if(piNumFramesTransmitted != NULL)
            *piNumFramesTransmitted = s_MsgHandles[iHandle].iNumFramesTransmitted;
        return ( TFXCR_OK );
  }
  else
      return ( TFXCR_INVALID_MSG_HANDLE );
#else
    *piNumFramesTransmitted = 1;
    *puiStatus = SB_TXOK;
    
    return ( TFXCR_OK );
#endif
  }

/********************************************************************
pmhGet_handle_ptr - Get a pointer to an element in the message handle array.

  inputs: Message handle

  return: Pointer if message handle is valid.
          NULL pointer if message handle is not valid.
********************************************************************/
MSG_HANDLE *pmhGet_handle_ptr(INT16 iHandle)
  {
  MSG_HANDLE *pmhHandle;

#ifdef USE_MSG_HANDLES
  if(iHandle >= 0 && iHandle < NUM_TX_MSG_HANDLES)
    pmhHandle = &s_MsgHandles[iHandle];
  else
    pmhHandle = NULL;
#else
  pmhHandle = NULL;
#endif
  return(pmhHandle);
  }
