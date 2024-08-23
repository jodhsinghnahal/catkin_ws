/******************************************************************************

  (c) 2020 Xantrex Canada ULC. All rights reserved.
  All trademarks are owned or licensed by Xantrex Canada ULC,
  its subsidiaries or affiliated companies.

****************************************************************************//**

  @addtogroup udpcan UDPCAN
  @{
  @brief  Interface to udpcan.c: virtual CAN over UDP

****************************************************************************//**

  @page udpcan_design CAN over UDP Virtual Interface (UDPCAN)

    This file provides the external interface to udpcan.c which provides
    an interface to a virtual CAN network implemented as UDP broadcasts

*******************************************************************************/

#ifndef UDPCAN_H
#define UDPCAN_H

/*==============================================================================
                              Includes
==============================================================================*/

#include "LIB_stdtype.h"
#include "cdrive.h"

/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/

tucBOOL UDPCAN_fnInit( void );
void UDPCAN_fnClose( void );
TFXCAN_RETURNS UDPCAN_fnStatus( void );
TFXCAN_RETURNS UDPCAN_fnReceiveFrame( CANPORT u8PortNumber,
                                      CANFRAME *pFrame );
TFXCAN_RETURNS UDPCAN_fnSendFrame( CANPORT u8PortNumber,
                                   CANFRAME *pFrame,
                                   INT16 *pi16MessageID );



#endif  // ifndef UDPCAN_H

//*****************************************************************************
// End of file.
//! @}
//*****************************************************************************
