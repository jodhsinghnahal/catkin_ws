/******************************************************************************

  (c) 2003 - 2022 Xantrex Canada ULC. All rights reserved.
  All trademarks are owned or licensed by Xantrex Canada ULC,
  its subsidiaries or affiliated companies.

****************************************************************************//**

  @addtogroup xbldefs Xanbus Local Definitions
  @{
  @brief
    This file provides declarations and definitions to be used
    locally and only by the XB library modules.

  @par API Functions
    XBADDR_fnNetNAME2NAME       - Convert net NAME to expanded NAME
    XBADDR_fnResetNAMECfg       - Handle a reset config command for NAME Config
    XBINIT_fnChkAsleep          - Check if Xanbus is asleep
    XBINIT_fnCSec2Ticks         - Convert centiseconds to ticks
    XBINIT_fnSaveError          - Save last error from Teleflex or xbapi code
    XBMSG_fnInit                - Initialize XanBus Message data
    XBMSG_fnCheckCfgSrc         - Validate the source of configuration message
    XBMSG_fnIsFromDiag          - Is message from a Diagnostics node
    XBMSG_fnPeriodicSend        - Handle Periodic Xmit request
    XBMSG_fnSendExPdob          - Send PG message and return queued data object
    XBTIMERS_fnLoadTxTimers     - Load periodic transmit timers from NVRam
    XBTIMERS_fnLoadRxTimers     - Load receive timeouts from NVRam
    XBTIMERS_fnGetClaimTimer    - Get the claim timer
    XBTIMERS_fnTickTimers       - Count down and look for expired timers
    XBTIMERS_fnResetTxToCfg     - Handle a reset config command for Tx Timers
    XBTIMERS_fnResetRxToCfg     - Handle a reset config command for Rx Timers
    XBMEM_fnMalloc              - Allocate memory from the XanBus heap
    XBMEM_fnFree                - Return memory to the XanBus heap

*******************************************************************************/

#ifndef XBLDEFS_INCL
#define XBLDEFS_INCL

/*==============================================================================
                              Includes
==============================================================================*/

// Teleflex include files
#include "libenv.h"
#include "LibCore.h"
#include "pgn.h"

// XanBus include files
#include "rvcudefs.h"   // PGN Definitions
#include "xbgdefs.h"    // XanBus API

/*==============================================================================
                              Defines
==============================================================================*/

#define XB_PORT_A               ( 0 )   // Port number for CAN device A
#define XB_REG_BUF              ( 0 )   // ID for regular message buffer
#define XB_VIP_BUF              ( 1 )   // ID for VIP message buffer
#define XBMEM_VIP_BUF_DISABLE   ( 0 )   // priority buffer is not available
#define XBMEM_VIP_BUF_ENABLE    ( 1 )   // priority buffer is available

#define XB_ADDR_CLAIM_TO        ( 250 ) // Address claim timeout (milliseconds)

// Compare NAMEs result codes
#define XB_CMP_NAME_1_GT_2      ( 1 )   // 1st NAME > 2nd NAME
#define XB_CMP_NAME_1_LT_2      ( -1 )  // 1st NAME < 2nd NAME
#define XB_CMP_NAME_1_EQ_2      ( 0 )   // 1st NAME = 2nd NAME

// Multi-packet Data Transfer Definitions
#define XB_MP_DX_FRAM_NUM_OFFS  0   // Byte offset to frame num in data xfer pkt
#define XB_MP_DX_DATA_OFFS      1   // Byte offset to data in data xfer pkt

/*--- Group Function Codes for ISO Connection Management parameter groups ---*/
#define XB_ISO_CONN_MAN_RTS     ( 0x10 )
#define XB_ISO_CONN_MAN_CTS     ( 0x11 )
#define XB_ISO_CONN_MAN_EOM     ( 0x13 )
#define XB_ISO_CONN_MAN_BAM     ( 0x20 )
#define XB_ISO_CONN_MAN_ABORT   ( 0xFF )

/*--- Multi-Frame Definitions ---*/
#define XB_MULTI_PKT_DATA_BYTES_PER_FRAME   ( 7 )
#define XB_FAST_PKT_DATA_BYTES_1ST_FRAME    ( 6 )
#define XB_FAST_PKT_DATA_BYTES_PER_FRAME    ( 7 )

/*==============================================================================
                            Type Definitions
==============================================================================*/

// Definition for node name as the network uses it
typedef struct
{
    uchar8 aucNAMEArray[ XB_NAME_LEN ];
} XB_tzNET_NAME;

/*==============================================================================
                          External/Public Variables
==============================================================================*/

// From xbmsg.c
extern XB_tzXB_COMM_STATS XBMSG_tzXbCommStats;

// From xbpgntbl.c
extern const uint32 XB_kaulRX_PGN_LIST[];
extern const uint32 XB_kaulTX_PGN_LIST[];
extern const PGN_tzPGN_INFO XB_katzALL_PGN_LIST[];
extern uchar8 XB_aucDynamicRx[];

/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/

extern uint16 XBPGNTBL_fnSizeOfRxPgnList( void );

extern uint16 XBPGNTBL_fnSizeOfTxPgnList( void );

extern uint16 XBPGNTBL_fnSizeOfAllPgnList( void );

extern uint16 XBPGNTBL_fnSizeOfDynRxLists( void );

// From xbaddr.c
extern void XBADDR_fnLoadClaimTimers( void );

extern tucBOOL XBADDR_fnNetNAME2NAME( // Pointer to condensed NAME
                                      XB_tzNET_NAME *ptzNetName,
                                      // Pointer to return expanded NAME
                                      XB_tzPGN_ISO_ADDR_CLAIM *ptzName );

extern tucBOOL XBADDR_fnResetNameCfg( XB_teRST_CMD teCmd ); // Type of reset

extern void XBADDR_fnTickClaimTimers( void );
extern void XBADDR_fnResetClaimTimers( uchar8 ucAddr );
extern tucBOOL XBADDR_fnGetNodeDSA( uchar8 ucAddr, uchar8 *pucDSA );

// From xbinit.c
extern void XBINIT_fnSaveError( // Value indic source of error
                                PGN_teESOURCE uiSource,
                                // Value indic error
                                uint16 uiParam );

extern uint16 XBINIT_fnCSec2Ticks( uint32 ulCentiSecs ); // Time to be converted

extern void XBINIT_fnStartProTimer( uint16 uiCentiSec,      // Time period
                                    XB_pfnPROTOCOL_TO pfunc ); // Function to call

extern void XBINIT_fnStopProTimer( void );

extern tucBOOL XBINIT_fnChkAsleep( void );

// From xbmltpkt.c
extern void XBMLTPKT_fnRecvRts( // Receive message
                                XB_tzPGN_ISO_TP_CONN_MAN_RTS *ptzRts,
                                // Source address
                                uchar8 ucSrcAddr );

extern void XBMLTPKT_fnRecvCts( // Receive message
                                XB_tzPGN_ISO_TP_CONN_MAN_CTS *ptzCts,
                                // Source address
                                uchar8 ucSrcAddr );

extern void XBMLTPKT_fnRecvEom( // Receive message
                                XB_tzPGN_ISO_TP_CONN_MAN_EOM *ptzEom,
                                // Source address
                                uchar8 ucSrcAddr );

extern void XBMLTPKT_fnRecvBam( // Receive message
                                XB_tzPGN_ISO_TP_CONN_MAN_BAM *ptzBam,
                                // Source address
                                uchar8 ucSrcAddr );

extern void XBMLTPKT_fnRecvAbort( // Receive message
                                  XB_tzPGN_ISO_TP_CONN_MAN_ABORT *ptzAbort,
                                  // Source address
                                  uchar8 ucSrcAddr );

// From xbmsg.c
extern tucBOOL XBMSG_fnInit( void );

extern tucBOOL XBMSG_fnCheckCfgSrc( // Configuration type
                                     XB_teCFG_TYPE teCfgType,
                                     // Source node address
                                     uchar8 ucSrcAddr,
                                     // Acknowledgement code
                                     XB_teACK_STS *pteAckType );

extern tucBOOL XBMSG_fnIsFromDiag( uchar8 ucSrcAddr );   // Address being tested

extern void XBMSG_fnRawReceive( PGNDOB *ptzDob ); // Data object holding rxed msg

extern void XBMSG_fnPeriodicSend( uint32 ulPgn,    // Parameter Group Number
                                  uchar8 ucDest ); // Destination address

extern PGNDOB *XBMSG_fnSendExPdob( uint32 ulPgn,
                                   sint16 siGrpFunc,
                                   void *pvSrcStruct,
                                   uint16 uiSrcSize,
                                   uchar8 ucDest,
                                   uchar8 ucBuf );

// From xbpack.c
extern const PGN_tzPGN_INFO * XBPACK_fnGetPgnInfo( // Parameter Group Number
                                                  uint32 ulPgn,
                                                  // Group Function code
                                                  sint16 siGrpFunc );
/*
extern uchar8 *XBPACK_fnPackPgn( // PGN Descriptor pointer
                                 const PGN_tzPGN_DESC * XT_FAR pktzPGNDesc,
                                 // Void pointer to expanded message
                                 void *pvSource,
                                 // Number of bytes in expaned message
                                 uint16 uiSrcSize,
                                 // Pointer to return length of PGN
                                 uint16 *puiDestLen );

extern void *XBPACK_fnUnPackPgn( // PGN Descriptor pointer
                                 const PGN_tzPGN_DESC * XT_FAR pktzPGNDesc,
                                 // Byte pointer to condensed message
                                 uchar8 *pucSrcBuf,
                                 // Length of condensed message
                                 uint16 uiSrcLen,
                                 // Returned Error Code pointer
                                 XB_teGRP_FUNC_VALUE *pteErrCode );
*/
extern tucBOOL XBPACK_fnDestroyPgnStruct( // PGN Descriptor pointer
                                          const PGN_tzPGN_DESC * XT_FAR pktzPGNDesc,
                                          // Pointer to data
                                          void *pvStruct );

extern sint16 XBPACK_fnRawGrpFunc( PGNDOB *ptzDob );    // Data object

extern uchar8 XBPACK_fnUnpackMsgCount(  // Byte pointer to condensed message
                                        uchar8 *pucSrcBuf );

// From xbplatform.c

// From xbtimers.c
extern tucBOOL XBTIMERS_fnLoadTxTimers( void );

extern tucBOOL XBTIMERS_fnLoadRxTimers( void );

extern uint16 XBTIMERS_fnGetClaimTimer( void );

extern void XBTIMERS_fnTickTimers( void );

extern tucBOOL XBTIMERS_fnResetTxToCfg( XB_teRST_CMD teCmd ); // Type of reset

extern tucBOOL XBTIMERS_fnResetRxToCfg( XB_teRST_CMD teCmd ); // Type of reset

#endif // XBLDEFS_INCL


//*****************************************************************************
// End of file.
//! @}
//*****************************************************************************
