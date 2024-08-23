/*
when to issue callbacks - 
    on command
    on condition detect

  interaction using can driver handle
  when to destroy a dob
    if not callbacks installed
  registry error management
  timeout sequence (address claim use)
  variable field count
  on fastpacket - time between next fastpacket

  set source and destination network address
***************************************************************************
** 
** PROJECT: CAN Library
**
** $Workfile:: pgn.h          $       $Author:: Peterd             $
** $Revision:: 38             $       $Date:: 24/03/04 3:53p       $
**
***************************************************************************
** 
** PROJECT: CAN Library
**
** 
** DESCRIPTION: PGN Data structures and object definitions
**
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
$Log: pgn.h $

    ***********************************************
    Revision: XanBus4_TomL/1
    User: TomL     Date: 01/20/10  Time: 10:24PM
    Matched case of include statements to files

    ***********************************************
    Revision: XanBus_HollyZ/5
    User: HollyZ     Date: 08/22/07  Time: 10:48PM
    Added prototype for PDOBTickTransmit()

    ***********************************************
    Revision: XanBus_HollyZ/4
    User: HollyZ     Date: 06/12/06  Time: 06:31PM
    Added new function prototypes for shim updates

    ***********************************************
    Revision: XanBus_HollyZ/3
    User: HollyZ     Date: 05/29/06  Time: 11:53PM
    Removed prototype for unused functions

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
    User: HollyZ     Date: 08/02/05  Time: 05:20PM
    Added prototypes for Link list functions

    ***********************************************
    Revision: XanBus_DaleM/2
    User: DaleM     Date: 02/17/05  Time: 04:50PM
    Dropped unused functions

    ***********************************************
    Revision: GateWayPfrmANMEA_DaleM/1
    User: DaleM     Date: 02/10/05  Time: 07:24PM
    Cleaned up old history list
    
***************************************************************************/

#ifndef PGN_H
#define PGN_H

#include "cdrive.h"
#include "LibCore.h"
#include "candrv.h"

#ifdef __CPLUSPLUS
extern "C"
{
#endif

//***************************************************************************
// PGNDOb
//  
// The PGNDOb contains all the methods and properties needed to create,
// obtain frames and receive frames
//***************************************************************************

// General Methods
DllExport TFXCAN_RETURNS PDOBConstruct( PGNDOB *pDOB, 
                                        PGN pgn, 
                                        sint16 siGrpFunc, 
                                        const PGN_tzPGN_INFO * pzPgnInfo );

DllExport TFXCAN_RETURNS PDOBConstructAlloc( PGNDOB **pDOB, 
                                             PGN pgn, 
                                             sint16 siGrpFunc, 
                                             const PGN_tzPGN_INFO * pzPgnInfo );

DllExport void PDOBDestruct( PGNDOB *pDOB );

DllExport int PDOBCallback(PGNDOB *pDOB, TFXLIB_CB_REASON cbReason);    // causes callback to be issued
                                                              // return 0 says leave dob alone,otherwise
                                                              // free the dob
                                                              
DllExport uint16 TestAndSetFlag( uchar8 *pucFlags, uchar8 ucNFlags, uchar8 ucFlagId );

// Receiving Methods
//DOBTICK_DISPOSITION PDOBInputTick(PGNDOB *pDOB, FRAME *pFrame);
DllExport TFXCAN_RETURNS PDOBData(PGNDOB *pDOB, INT16 i16BteCount, void *pvData);

// Transmitting Methods
DllExport DOBTICK_DISPOSITION PDOBOutputStatus(PGNDOB *pDOB, BOOL bTick);
DllExport DOBTICK_DISPOSITION PDOBTickTransmit(PGNDOB *pDOB, BOOL bTick);

DllExport TFXCAN_RETURNS PDOBWrite(PGNDOB *pDOB);

DllExport TFXCAN_RETURNS PDOBDoXStatus(PGNDOB *pDOB);

//General properties
DllExport void PDOBSetPort(PGNDOB *pDOB, CANPORT i8Port);
DllExport CANPORT PDOBGetPort(PGNDOB *pDOB);
DllExport void PDOBSetTimer(PGNDOB *pDOB, UINT16 u16MilliSeconds);

DllExport void PDOBSetCallback(PGNDOB *pDOB, TFXLIBCB cbFunction, UINT32 u32UserData);

// Transmitting properties
DllExport void PDOBSetDestAddress(PGNDOB *pDOB, uchar8 ucAddr);
DllExport void PDOBSetSourceAddr(PGNDOB *pDOB, uchar8 ucSrcAddr);
DllExport TFXCAN_RETURNS PDOBSetPriority(PGNDOB *pDOB);
DllExport TFXCAN_RETURNS PDOBGetPriority(PGNDOB *pDOB);
DllExport BOOL PDOBHasFrameToTransmit(PGNDOB *pDOB);

#ifdef __CPLUSPLUS
}
#endif
	

//***************************************************************************
// PGNXmiter
//  The Xmiter is responsible for managing outbound pgn data objects and
//  soliciting the objects for frames which can be passed to the
//  can controller
//
//  The Xmiter is also responsible for timing related to fastpackets
//    (one must allow for 10ms between fast packets)
//***************************************************************************

#ifdef __CPLUSPLUS
xtern "C"
{
#endif
// Methods
DllExport void PGNXConstruct(void);
DllExport void PGNXDestruct(void);
DllExport void PGNXSetFramesOutPerTick(CANPORT i8Port, int nFramesPerTick);
DllExport void PGNXTick(CANPORT i8Port, BOOL bTick);

#ifdef __CPLUSPLUS
}
#endif
//Attributes

	//PGNDataObjectList
	//UINT32  u32FastpacketTimer;  //  (wait 10ms between messages)

//***************************************************************************
// PGNRcvr
//  The PGN Rcvr is responsible for managing inbound pgn data object and
//  soliciting can frames from the driver and creating the appropriate
//  pgn data object (or passing the frame to an existing data object)
//
//  Once a pgn indicates it is complete, the Rcvr executes the callback
//  address installed to handle the pgn
//***************************************************************************
// Methods
#ifdef __CPLUSPLUS
extern "C"
{
#endif
DllExport void PGNRConstruct(void);
DllExport void PGNRDestruct(void);
DllExport void PGNRSetFramesInPerTick(CANPORT i8Port, int nFramesPerTick);
DllExport TFXCAN_RETURNS PGNRTick(CANPORT i8Port);

#ifdef __CPLUSPLUS
}
#endif


//*****************************************************************************************************
// DOBHLD - PGN DOB Container Object
//*****************************************************************************************************

#ifdef __CPLUSPLUS
extern "C"
{
#endif

DllExport void DOBHLDConstruct(void);
DllExport void DOBHLDDestruct(void);
DllExport TFXCAN_RETURNS DOBHLDAdd(PGNDOB *pDob);
DllExport PGNDOB *DOBHLDFirst(void);
DllExport PGNDOB *DOBHLDNext(PGNDOB *pLastDob);        
DllExport void DOBHLDDelete(PGNDOB *pDob);   // removes from list
DllExport TFXCAN_RETURNS DOBHLDFindFromFrame( const PGN_tzPGN_INFO * pInfo,
                                              CANFRAME *pCanFrame,
                                              PGNDOB **pDOB );                                             
DllExport TFXCAN_RETURNS addToVipDob(PGNDOB *pDob);
DllExport PGNDOB *searchFirstVipDOB(void);
DllExport PGNDOB *searchNextVipDob(PGNDOB *pLastDob);        
DllExport void deleteFromVipDob(PGNDOB *pDob);   // removes from list
                                      

#ifdef __CPLUSPLUS
}
#endif



//*****************************************************************************************************
// CANFrame Object
//*****************************************************************************************************
#ifdef __CPLUSPLUS
extern "C"
{
#endif

// General
DllExport void CANFrameConstruct(void); // done once in system
DllExport void CANFrameDestruct(void);

// attributes
DllExport PGN CANFrameGetPGN(CANFRAME *pFrame);

DllExport CANFRAME *CANFrameAllocate(void);
DllExport TFXCAN_RETURNS CANFrameFree(CANFRAME *pFrame);

// receive methods
DllExport BOOL CANFrameRecv(CANFRAME *pFrame, CANPORT i8Port);   // read from driver - returns TRUE if have frame
DllExport BOOL CANFrameRecvForeign(CANDRV_tzCAN_FRAME *pFrame, CANPORT i8Port);   // read from driver - returns TRUE if have frame
DllExport TFXCAN_RETURNS CANFrameGetData(CANFRAME *pFrame, INT16 *pi16ByteCount, uchar8 *pucData);

// transmit methods
DllExport TFXCAN_RETURNS CANFrameCreate(PGNDOB *pDOB);
DllExport TFXCAN_RETURNS CANFrameSend(CANFRAME *pFrame, CANPORT i8Port, INT16 *pHandle);   // send to driver
DllExport TFXCAN_RETURNS CANFrameSendForeign(CANDRV_tzCAN_FRAME *pFrame, CANPORT i8Port, INT16 *pHandle);   // send to driver
DllExport TFXCAN_RETURNS CANFrameSetCB(CANFRAME *pFrame, TFXLIBCB cb, UINT32 u32UserData);
DllExport TFXCAN_RETURNS CANFrameXmitStatus(INT16 iHandle, INT16 iFramesInPGN, INT16 *pStatus);            
DllExport TFXCAN_RETURNS CANFrameClearHandle(CANPORT i8Port, INT16 iHandle);

// transmit properties
DllExport TFXCAN_RETURNS CANFrameSetRetry(CANFRAME *pFrame, BOOL bON);
DllExport uchar8         CANFrameGetSourceAddress(CANFRAME *pFrame);
DllExport uchar8         CANFrameGetDestAddress(CANFRAME *pFrame);

#ifdef __CPLUSPLUS
}
#endif

#endif
