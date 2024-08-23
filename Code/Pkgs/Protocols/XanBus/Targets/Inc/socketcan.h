/******************************************************************************

  (c) 2020 Xantrex Canada ULC. All rights reserved.
  All trademarks are owned or licensed by Xantrex Canada ULC,
  its subsidiaries or affiliated companies.

****************************************************************************//**

  @addtogroup udpcan UDPCAN
  @{
  @brief Interface to Linux's socketcan

****************************************************************************//**

  @page socketcan_design Interface to Linux's socketcan

    This file provides an interface to socketcan, Linux's network-like
    interface to its native CAN drivers.  It will also work for 
    vcan, a local simulated CAN interface.

*******************************************************************************/

#ifndef SOCKETCAN_H
#define SOCKETCAN_H

/*==============================================================================
                              Includes
==============================================================================*/

#include "LIB_stdtype.h"
#include "cdrive.h"

/*==============================================================================
                              Defines
==============================================================================*/

// Bitmasks for status flags
#define SOCKETCAN_STS_RXQ_FULL     0x01
#define SOCKETCAN_STS_TXQ_FULL     0x02
#define SOCKETCAN_STS_ERR_WARNING  0x04
#define SOCKETCAN_STS_OVERRUN      0x08
#define SOCKETCAN_STS_ERR_PASSIVE  0x20
#define SOCKETCAN_STS_ERR_BUSOFF   0x40
#define SOCKETCAN_STS_BUS_ERROR    0x80

/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/

tucBOOL SOCKETCAN_fnInit( uchar8 ucBitRate );
void SOCKETCAN_fnClose( void );
TFXCAN_RETURNS SOCKETCAN_fnStatus( void );
TFXCAN_RETURNS SOCKETCAN_fnReceiveFrame( CANPORT u8PortNumber,
					 CANFRAME *pFrame );
TFXCAN_RETURNS SOCKETCAN_fnSendFrame( CANPORT u8PortNumber,
				      CANFRAME *pFrame,
				      INT16 *pi16MessageID );

#endif  // ifndef SOCKETCAN_H

//*****************************************************************************
// End of file.
//! @}
//*****************************************************************************
