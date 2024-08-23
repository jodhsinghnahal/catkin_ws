/******************************************************************************

  (c) 2004 - 2021 Xantrex Canada ULC. All rights reserved.
  All trademarks are owned or licensed by Xantrex Canada ULC,
  its subsidiaries or affiliated companies.

****************************************************************************//**

  @addtogroup canbuf CANBUF
  @{
  @brief Interface for the CAN Stack Frame Buffer Management

  @par API Functions
    CANBUF_fnCtor           - Creator for a rx/tx pair of buffers
    CANBUF_fnRxInsert       - Put data into receive buffer
    CANBUF_fnRxExtract      - Remove data from receive buffer
    CANBUF_fnTxInsert       - Put data into transmit buffer
    CANBUF_fnTxExtract      - Remove data from transmit buffer
    CANBUF_fnTxBufFull      - Is Transmit buffer full

*******************************************************************************/

#ifndef CANBUF_INCL
#define CANBUF_INCL

/*==============================================================================
                              Includes
==============================================================================*/

#include "LIB_stddefs.h"    // Xantrex basic type definitions
#include "LibCfg.h"         // For MAX_PORTS
#include "candrv.h"         // Interface to CAN driver

/*==============================================================================
                              Defines
==============================================================================*/

// Set up the number of buffers to use
#ifndef CANBUF_RX_PORT_BUFS
#define CANBUF_RX_PORT_BUFS   ( 40 )
#endif

#ifndef CANBUF_TX_PORT_BUFS
#define CANBUF_TX_PORT_BUFS   ( 40 )
#endif

// Total count of transmit buffer -- used to set number of message handles
#define CANBUF_TX_BUF_COUNT   ( CANBUF_TX_PORT_BUFS * MAX_PORTS )

// Buffer access error indications
#ifndef CANBUF_BUFFER_FULL
#define CANBUF_BUFFER_FULL      ( 255 )     // Buffer was full
#endif

#ifndef CANBUF_BUFFER_EMPTY
#define CANBUF_BUFFER_EMPTY     ( 254 )     // Buffer was empty
#endif

/*==============================================================================
                            Type Definitions
==============================================================================*/


/*==============================================================================
                                Enums
==============================================================================*/


/*=============================================================================
                              Structures
==============================================================================*/

typedef struct canbuf_zTX_REF
{
    // Number of messages transmitted using this handle.  For single
    // frames, this will always be 1 when transmission is complete.
    // For fast-packet, this will be the number of frames in the fast-packet
    // when the tranmission is complete.
    uchar8 ucFramesOut;
    uchar8 ucTxResult;  // Status of transmission.  This will only be valid
                        // when the tranmission is complete.
    tucBOOL tucInUse;   // TRUE - message handle is being used.
} CANBUF_tzTX_REF;

// The structure used for buffering receive CAN frames
typedef struct canbuf_zRX_BUF
{
    CANDRV_tzCAN_FRAME zCanFrame;
} CANBUF_tzRX_BUF;

// The structure used for buffering transmit CAN frames
typedef struct canbuf_zTX_BUF
{
    CANDRV_tzCAN_FRAME zCanFrame;
    uint16 uiMsgHandle;
} CANBUF_tzTX_BUF;

/*==============================================================================
                          External/Public Constants
==============================================================================*/


/*==============================================================================
                          External/Public Variables
==============================================================================*/


/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/
extern tucBOOL CANBUF_fnCtor( uchar8 ucPort );                // CAN port number

#ifdef USE_CANBUF16

extern uint16 CANBUF_fnRxInsert( uchar8 ucPort,               // CAN port number
                                 CANBUF_tzRX_BUF *ptzRxBuf ); // Receive buffer

extern uint16 CANBUF_fnRxExtract( uchar8 ucPort,              // CAN port number
                                  CANBUF_tzRX_BUF *ptzRxBuf );// Receive buffer

extern uint16 CANBUF_fnTxInsert( uchar8 ucPort,               // CAN port number
                                 CANBUF_tzTX_BUF *ptzTxBuf ); // Transmit buffer

extern uint16 CANBUF_fnTxExtract( uchar8 ucPort,              // CAN port number
                                  CANBUF_tzTX_BUF *ptzTxBuf );// Transmit buffer
#else

extern uchar8 CANBUF_fnRxInsert( uchar8 ucPort,               // CAN port number
                                 CANBUF_tzRX_BUF *ptzRxBuf ); // Receive buffer

extern uchar8 CANBUF_fnRxExtract( uchar8 ucPort,              // CAN port number
                                  CANBUF_tzRX_BUF *ptzRxBuf );// Receive buffer

extern uchar8 CANBUF_fnTxInsert( uchar8 ucPort,               // CAN port number
                                 CANBUF_tzTX_BUF *ptzTxBuf ); // Transmit buffer

extern uchar8 CANBUF_fnTxExtract( uchar8 ucPort,              // CAN port number
                                  CANBUF_tzTX_BUF *ptzTxBuf );// Transmit buffer

#endif

extern uchar8 CANBUF_fnTxFull( uchar8 ucPort );               // CAN port number

#endif  // CANBUF_INCL


//*****************************************************************************
// End of file.
//! @}
//*****************************************************************************
