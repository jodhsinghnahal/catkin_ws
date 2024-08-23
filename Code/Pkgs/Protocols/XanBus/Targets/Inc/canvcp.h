/******************************************************************************

  (c) 2020 Xantrex Canada ULC. All rights reserved.
  All trademarks are owned or licensed by Xantrex Canada ULC,
  its subsidiaries or affiliated companies.

****************************************************************************//**

  @addtogroup canvcp CANVCP
  @{
  @brief  Interface to CANUSB via virtual comm port

****************************************************************************//**

  @page udpcan_design CAN over UDP Virtual Interface (UDPCAN)

  This file provides the external interface to canvcp.c which provides
  an interface to the Lawicel CANUSB interface via a virtual comm port

*******************************************************************************/

#ifndef CANVCP_H
#define CANVCP_H

/*==============================================================================
                              Includes
==============================================================================*/

#include "LIB_stdtype.h"
#include "cdrive.h"

/*==============================================================================
                              Defines
==============================================================================*/

// Bitmasks for status flags
#define CANVCP_STS_RXQ_FULL     0x01
#define CANVCP_STS_TXQ_FULL     0x02
#define CANVCP_STS_ERR_WARNING  0x04
#define CANVCP_STS_OVERRUN      0x08
#define CANVCP_STS_ERR_PASSIVE  0x20
#define CANVCP_STS_ERR_BUSOFF   0x40
#define CANVCP_STS_BUS_ERROR    0x80

/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/

tucBOOL CANVCP_fnInit( uchar8 ucBitRate );
void CANVCP_fnClose( void );
TFXCAN_RETURNS CANVCP_fnStatus( void );
TFXCAN_RETURNS CANVCP_fnReceiveFrame( CANPORT u8PortNumber,
				      CANFRAME *pFrame );
TFXCAN_RETURNS CANVCP_fnSendFrame( CANPORT u8PortNumber,
				   CANFRAME *pFrame,
				   INT16 *pi16MessageID );

#endif  // ifndef CANVCP_H

//*****************************************************************************
// End of file.
//! @}
//*****************************************************************************
