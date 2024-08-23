/******************************************************************************

  (c) 2003 - 2023 Xantrex Canada ULC. All rights reserved.
  All trademarks are owned or licensed by Xantrex Canada ULC,
  its subsidiaries or affiliated companies.

****************************************************************************//**

  @addtogroup xbgdefs Xanbus Global Definitions
  @{
  @brief
    This file provides the interface definitions and functions for
    use by modules outside of the XanBus library modules.

  @par API Functions
    XBADDR_fnCtor               - Initialize random seed for Address Claiming
    XBADDR_fnGetNAME            - Get Name for node using given address
    XBADDR_fnGetNetNAME         - Get Network Name for node using given address
    XBADDR_fnFindDevice         - Get devices on network
    XBADDR_fnTestPriority       - Get my Name Priority versus node of given address
    XBADDR_fnGetMyAddr          - Get current address
    XBADDR_fnBeginClaiming      - Initialize and start address claim sequence
    XBADDR_fnNAME2ShortNAME     - Convert NAME to short form
    XBADDR_fnOnLine

    XBINIT_fnInit               - Initialize XB module
    XBINIT_fnGetErrorNum        - Get last unreported error as a number
    XBINIT_fnNetCtrl            - Control the activity of the XanBus APIs
    XBINIT_fnResetCfg           - Handle reset configurations request
    XBINIT_fnTick               - Periodically check if Teleflex stack can send or receive
    XBINIT_fnCSec2Ticks         - Convert centiseconds to ticks
    XBINIT_fnGetVersion         - Get XanBus library version

    XBMEM_fnCtor                - Setup Xanbus memory heap
    XBMEM_fnDtor                - Release the resource used by Xanbus memory heap
    XBMEM_fnMalloc              - Allocate a block of memory from the heap
    XBMEM_fnFree                - Free a block of memory back to the heap

    XBMSG_fnClearDynamic        - Clear dynamic list of PGNs to accept
    XBMSG_fnControlDynamic      - Dynamically set a PGN to accept
    XBMSG_fnDestroyRecvStruct   - Release memory of receive data structure
    XBMSG_fnOnLine              - XanBus stack ready for application messages
    XBMSG_fnGetBusState         - Get state of the node's connection to the CAN bus
    XBMSG_fnSendEx              - Send PG data message on the XanBus
    XBMSG_fnSend                - Send PG data message on the XanBus
    XBMSG_fnSendISOAck          - Send an acknowledgement message
    XBMSG_fnISOReq              - Send an ISO Request
    XBMSG_fnClearCommStats      - Clear communications statistics

    XBTIMERS_fnEnablePeriodReports - Enable or disable periodic reporting
    XBTIMERS_fnSetTxTimer       - Start or remove periodic transmit timer

    XBPLATFORM_fnCheckErrors    - Tell application to check for errors
    XBPLATFORM_fnSetNetIndic    - Tell application to indicate appropriate network status
    XBPLATFORM_fnLoad           - Ask application to load parameter from non-volatile storage
    XBPLATFORM_fnSave           - Ask application to save parameter to non-volatile storage
    XBPLATFORM_fnNodeChange     - Tell application that the node list has changed
    XBPLATFORM_fnRecvXbMsg      - Handle received internal XanBus message by application

*******************************************************************************/

#ifndef XBGDEFS_INCL
#define XBGDEFS_INCL

#include <limits.h>
#include "pgnudefs.h"
#include "xbudefs.h"

#ifndef NULL
#define NULL	    ( 0L )
#endif

//--- Define processor instance number for XanBus processor ---
#define XB_PROC_INST_XANBUS     ( 0 )

//--- Define length of node name ---
#define XB_NAME_LEN             ( XB_DLC_ISO_ADDR_CLAIM )

//--- Values for fields in a node NAME ---
#define XB_INDUSTRY_GROUP       ( XB_eINDUSTRY_GRP_ALL )    // Global Industry Group
#define XB_DEVICE_INST_LOWER    ( 0 )    // Default Device Instance Lower
#define XB_SYSTEM_INSTANCE      ( 0 )    // Default System Instance
#define XB_MFG_XANTREX          ( 2047 ) // 0x07FF

//--- Limits for node NAME field ---
#define XB_NAME_DEV_INST_LO_MAX ( 7 )
#define XB_NAME_DEV_INST_HI_MAX ( 31 )
#define XB_NAME_SYS_INST_MAX    ( 15 )
#define XB_NAME_DEV_CLASS_MAX   ( 127 )

//--- Maximum number of entries in timer lists ---
#define XB_TIMER_MAX_SEND       ( 30 )      // Max of Periodic Transmit timers
#define XB_TIMER_MAX_RECV       ( 10 )      // Max of Receive Timeout timers

//  This value indicates 'do not changed current value' when used for
//  the period or start time in function XBTIMERS_fnSetTxTimer
#define XB_TIMER_NO_CHANGE      ( PGN_UI_NO_DATA )   // Do not change time

// Some time value definitions
#define XB_TIMER_IMMEDIATE      ( 1 )       // Shortest period of time (1 tick)
#define XB_TIMER_OFF            ( 0 )       // Disable timer
#define XB_TIMER_DELETE         ( 0 )       // Remove timer

//  Some common time definitions for default transmit period and
//  receive timeout of PGNs. Time value is centiseconds.
#define XB_PERIOD_ISO_ADDR_CLAIM    ( 100 )     // Once per second
#define XB_PERIOD_STS               ( 25 )      // Four times a second
#define XB_PERIOD_DEVICE_STS        ( 50 )      // Twice a second
#define XB_PERIOD_SOURCE_STS        ( 50 )      // Twice a second
#define XB_PERIOD_ASSOC_STS         ( 600 )     // Once every six seconds
#define XB_PERIOD_MIN               ( 25 )      // Four times a second

#define XB_TO_ISO_ADDR_CLAIM        ( 500 )
#define XB_TO_STS                   ( 125 )
#define XB_TO_DEVICE_STS            ( 125 )

#define XB_TEST_PRI_MYNAME_HI       ( 1 )   // My NAME is higher priority
#define XB_TEST_PRI_MYNAME_LO       ( -1 )  // My NAME is lower priority
#define XB_TEST_PRI_MYNAME_EQ       ( 0 )   // My NAME is equal priority
#define XB_TEST_PRI_FAIL            ( -2 )  // Call to TestPriority failed

#define XB_CENTI_TO_MILLI           ( 10 )      // Converter centi s to milli s

// Defines for PGN message Source information
#define XB_NODE_DEVICE_UI           ( 0x01 )  // Source is a User Interface
#define XB_NODE_DEVICE_DIAG         ( 0x02 )  // Source is a Diagnostic Term.
#define XB_NODE_DEVICE_UI_OR_DIAG   ( XB_NODE_DEVICE_UI | XB_NODE_DEVICE_DIAG )
#define XB_NODE_DEVICE_INVERTS      ( 0x04 )  // Source is an Inverter
#define XB_NODE_DEVICE_LIKE_ME      ( 0x08 )  // Source is same type as me
#define XB_NODE_NAME_HI_PRI         ( 0x10 )  // Source NAME has higher priority
#define XB_NODE_NAME_LO_PRI         ( 0x20 )  // Source NAME has lower priority
#define XB_NODE_APP_FLAGS           ( 0xC0 )  // Application can use these bits



//--- Values for Node Change parameter in function XBPLATFORM_fnNodeChange---
typedef enum eNODE_CHANGE
{
    XB_eNODE_CHANGE_NEW_NODE,   // Node has joined the bus
    XB_eNODE_CHANGE_ADDRESS,    // Existing node has a new address
    XB_eNODE_CHANGE_BUMPED_OFF, // Node must give up its address
    XB_eNODE_CHANGE_TIMED_OUT   // Node has not reported within timeout period
} XB_teNODE_CHANGE;


//--- Parameters types used by XB_PLATFORM.C ---
typedef enum ePARAM_TYPE
{
    XB_ePARAM_TYPE_ADDRESS,         // Node Preferred Address
    XB_ePARAM_TYPE_NAME,            // Node Name
    XB_ePARAM_TYPE_NAME_CONFIG,     // Node Name Configuration
    XB_ePARAM_TYPE_TX_TIMEOUTS,     // Periodic Xmit timeouts
    XB_ePARAM_TYPE_RX_TIMEOUTS,     // Receive timeouts
    XB_ePARAM_TYPE_DSA              // Default Source Address
} XB_tePARAM_TYPE;

//--- Address State ---
typedef enum eADDR_STATE
{
    XB_eADDR_IDLE,
    XB_eADDR_CLAIMING,
    XB_eADDR_BLOCKED,
    XB_eADDR_ONLINE
} XB_teADDR_STATE;

//--- Values for XanBus Network Control ---
typedef enum eNET_CTRL
{
    XB_eNET_CTRL_POWERUP,
    XB_eNET_CTRL_SUSPEND,
    XB_eNET_CTRL_RESUME
} XB_teNET_CTRL;

// Wake from sleep callback function prototype type
typedef void ( *XB_tfnSLEEP_CALLBACK )( void );

// Protocol timer timeout function prototype type
typedef void ( *XB_pfnPROTOCOL_TO )( void );

// Function to test if the application is in the right state to handle a PGN
typedef tucBOOL ( *XB_pfnCONTEXT_TEST )( uint32 ulPgn );

/*=============================================================================
                         Structure Definitions
=============================================================================*/

// Structure for saving Address Claiming Config in non-volatile memory
typedef struct zADDR_CLAIM_CONFIG
{
    uchar8 ucPrefAddr;
    uchar8 ucLowAddr;
    uchar8 ucHighAddr;
    tucBOOL tucIncrement;

} XB_tzADDR_CLAIM_CONFIG;

// Structure for saving Name Config in non-volatile memory
typedef struct zNAME_CONFIG
{
    uchar8 ucConcurAccCtr;
    XB_tzPGN_ISO_ADDR_CLAIM    tzNAME;
} XB_tzNAME_CONFIG;

// Structure for saving Periodic Xmit Timers in non-volatile memory
typedef struct zSEND_TIMERS
{
    uchar8 ucConcurAccCtr;
    XB_tzPGN_EXTRA_PER_XMIT_CFG atzTimers[ XB_TIMER_MAX_SEND ];
} XB_tzSEND_TIMERS;

// Structure for saving Receive Timeout Timers in non-volatile memory
typedef struct zRECV_TIMERS
{
    uchar8 ucConcurAccCtr;
    XB_tzPGN_EXTRA_RX_TO_CFG atzTimers[ XB_TIMER_MAX_RECV ];
} XB_tzRECV_TIMERS;

// Definition for collecting XanBus layer communications statistics
typedef struct zXB_COMM_STATS
{
    uchar8 ucMPRxTimeout;
    uchar8 ucMPTxTimeout;
    uchar8 ucMPLateCts;
    uchar8 ucMPLateEom;
    uchar8 ucMPLateAbort;
    uchar8 ucMPLateXfer;
    uchar8 ucMaxActiveDobs;
    uint16 uiMallocFail;
    uint16 uiHeapFreeMin;
} XB_tzXB_COMM_STATS;

// Application level filter for received configurations
extern  XB_pfnCONTEXT_TEST XBINIT_pfnAppCfgFilter;

/*=============================================================================
                   Function Prototypes for exported functions
=============================================================================*/

#ifndef XBDLL_API
#ifdef WIN32
#ifdef XBDLL_EXPORTS
#define XBDLL_API __declspec(dllexport)
#else
#define XBDLL_API __declspec(dllimport)
#endif
#else // !WIN32
#define XBDLL_API extern
#endif
#endif  // ndef XBDLL_API

#ifdef __cplusplus
extern "C" {
#endif

// From xbaddr.c
XBDLL_API void XBADDR_fnCtor( // Random number seed
                              uint32 ulRandSeed,
                              // maximum number of nodes allowed on the node list
                              uchar8 ucMaxNodes );

XBDLL_API tucBOOL XBADDR_fnBeginClaiming( void );
XBDLL_API tucBOOL XBADDR_fnReloadNAME( XB_tzPGN_ISO_ADDR_CLAIM *ptzName );


XBDLL_API uchar8 XBADDR_fnGetMyAddr( void );

XBDLL_API tucBOOL XBADDR_fnGetNAME( // Address of device to get
                                    uchar8 ucAddr,
                                    // Pointer to name being returned
                                    XB_tzPGN_ISO_ADDR_CLAIM *ptzName );

XBDLL_API tucBOOL XBADDR_fnGetNetNAME( // Address of device to get
                                       uchar8 ucAddr,
                                       // Pointer pointer to the condensed name being returned
                                       uchar8 **pucShortNAME );

XBDLL_API void XBADDR_fnNAME2ShortNAME( // Pointer to expanded NAME
                                        XB_tzPGN_ISO_ADDR_CLAIM *ptzLongNAME,
                                       // Pointer to condensed NAME
                                        uchar8 *pucShortNAME );

XBDLL_API tucBOOL XBADDR_fnOnLine( void );

XBDLL_API tucBOOL XBADDR_fnFindDevice( uchar8 ucIndex,
                                       uchar8 *pucAddr,
                                       XB_tzPGN_ISO_ADDR_CLAIM *ptzNAME );

XBDLL_API sint16 XBADDR_fnTestPriority( uchar8 ucAddr ); // Address of other node

// From xbinit.c

// Application level filter for received configurations

XBDLL_API tucBOOL XBINIT_fnInit( uchar8 ucPort,
                       uchar8 ucBitRate,
                       uchar8 ucTicksPerSec );

XBDLL_API void XBINIT_fnNetCtrl( XB_teNET_CTRL teControl ); // Network control

XBDLL_API void XBINIT_fnSleepCtrl( tucBOOL ucSleep,         // Sleep or wake
                                   // Callback when sleep starts
                                   XB_tfnSLEEP_CALLBACK pCallback );

XBDLL_API uint16 XBINIT_fnGetErrorNum( void );
XBDLL_API uint16 XBINIT_fnGetErrorSrc( void );

XBDLL_API void XBINIT_fnTick( void );

XBDLL_API tucBOOL XBINIT_fnResetCfg( XB_teRST_CMD eCmd );   // Type of reset

XBDLL_API void XBINIT_fnGetVersion( uint32 *pulVer, uint16 *puiBuild );

XBDLL_API void XBINIT_fnSetAppCfgFilter( XB_pfnCONTEXT_TEST pfnFilter );

// From xbmltpkt.c

// From xbmem.c
XBDLL_API tucBOOL XBMEM_fnCtor( // pointer to the start of the XanBus heap
                                void *pvHeapStart,
                                // number of bytes in the Xanbus heap
                                uint16 uiHeapSize );
XBDLL_API void XBMEM_fnDtor( void );
XBDLL_API void *XBMEM_fnMalloc( // number of bytes to locate in the Xanbus heap
                                uint16 NumBytes );

XBDLL_API void XBMEM_fnFree( // pointer to the memory block to return to the Xanbus heap
                             void *pvMemBlock );

XBDLL_API uint16 XBMEM_fnGetFreeBytes( void );

// From xbmsg.c
XBDLL_API void XBMSG_fnClearDynamic( void );

XBDLL_API void XBMSG_fnControlDynamic( uint32 ulPgn,    // Parameter Group Number
                                       tucBOOL tucOn ); // Dynamic state

XBDLL_API void XBMSG_fnDestroyRecvStruct( // Receive workblock pointer
                                          PGN_tzRECV_DATA *ptzRecv );

XBDLL_API XB_tzXB_COMM_STATS * XBMSG_fnGetCommStats( void );

XBDLL_API tucBOOL XBMSG_fnOnLine( void );

XBDLL_API XB_teNETWORK_STATE XBMSG_fnGetBusState( void );

XBDLL_API tucBOOL XBMSG_fnSend( // Parameter Group Number
                                uint32 ulPgn,
                                // Pointer to structure with message
                                void *pvStruct,
                                // Size of message structure in bytes
                                uint16 uiSize,
                                // Destination address for message
                                uchar8 ucDest );

XBDLL_API tucBOOL XBMSG_fnSendEx( // Parameter Group Function
                                  uint32 ulPgn,
                                  // Group Function code
                                  sint16 siGrpFunc,
                                  // Pointer to structure with message
                                  void *pvStruct,
                                  // Size of message structure in bytes
                                  uint16 uiSize,
                                  // Destination address for message
                                  uchar8 ucDest );

XBDLL_API tucBOOL XBMSG_fnSendPriority( // Parameter Group Function
                                        uint32 ulPgn,
                                        // Group Function code
                                        sint16 siGrpFunc,
                                        // Pointer to structure with message
                                        void *pvStruct,
                                        // Size of message structure in bytes
                                        uint16 uiSize,
                                        // Destination address for message
                                        uchar8 ucDest );

XBDLL_API void XBMSG_fnSendISOAck( // Parameter Group Number being Acked
                                   uint32 ulPgn,
                                   // Reason for NAK or Deny
                                   XB_teGRP_FUNC_VALUE teReason,
                                   // Control byte for Ack
                                   XB_teACK_STS teControl,
                                   uchar8 ucSrcAddr );

XBDLL_API void XBMSG_fnISOReq( // Parameter Group Number being requested
                               uint32 ulPgn,
                               // Destination address for request
                               uchar8 ucDest );

XBDLL_API void XBMSG_fnClearCommStats( void );

// From xbpack.c
// Note: This function is used only in the shared library incarnation, not embedded firmware
XBDLL_API const PGN_tzPGN_INFO * XBPACK_fnGetPgnInfo( // Parameter Group Number
                                                     uint32 ulPgn,
                                                     // Group Function Code
                                                     sint16 siGrpFunc );

XBDLL_API uchar8 *XBPACK_fnPackPgn( const PGN_tzPGN_DESC * XT_FAR pktzPgnDesc,  ///< Pointer to the PGN descriptor
                                    void *pvSource,                             ///< Points to the structure that needs to be packed
                                    uint16 uiSrcSize,                           ///< The size of the structure
                                    uint16 *puiDestLen );                       ///< The length of the outgoing message buffer

XBDLL_API void *XBPACK_fnUnPackPgn( const PGN_tzPGN_DESC * XT_FAR pktzPgnDesc,  ///< Pointer to the PGN descriptor
                                    uchar8 *pucSrcBuf,                          ///< Pointer to the received message
                                    uint16 uiSrcLen,                            ///< The length of the received message
                                    XB_teGRP_FUNC_VALUE *pteErrCode );          ///< Pointe5r to the returned error code

// From xbplatform.c
XBDLL_API void XBPLATFORM_fnCheckErrors( void );

XBDLL_API void XBPLATFORM_fnSetNetIndic( // State of CAN bus
                                         XB_teNETWORK_STATE teBusState,
                                         tucBOOL tucOn );   // Indicator on or off

XBDLL_API tuiSTATUS XBPLATFORM_fnLoad( // Type of data to load
                                       XB_tePARAM_TYPE teType,
                                       // Configuration type
                                       XB_teCFG_TYPE teCfgType,
                                       // Pointer to put data
                                       void *pvData,
                                       // Size of data allowed
                                       uint16 uiSize );

XBDLL_API tuiSTATUS XBPLATFORM_fnSave( // Type of data to load
                                       XB_tePARAM_TYPE teType,
                                       // Configuration type
                                       XB_teCFG_TYPE teCfgType,
                                       // Pointer to data
                                       void *pvData,
                                       // Size of data
                                       uint16 uiSize );

XBDLL_API void XBPLATFORM_fnNodeChange( // Type of change
                                        XB_teNODE_CHANGE teChange,
                                        // Addresses affected
                                        uchar8 ucOldAddr,
                                        uchar8 ucNewAddr,
                                        // Name of Node
                                        XB_tzPGN_ISO_ADDR_CLAIM *ptzName );

XBDLL_API void XBPLATFORM_fnRecvXbMsg( // Receive workblock pointer
                                       PGN_tzRECV_DATA *ptzRecv );

// Optional application level filter for received XanBus configuration messages
XBDLL_API tucBOOL XBPLATFORM_fnAppCfgFilter( uint32 ulPgn );

#ifdef WIN32
XBDLL_API void XBPLATFORM_fnInitCritical( void );

XBDLL_API void XBPLATFORM_fnDeleteCritical( void );

XBDLL_API void XBPLATFORM_fnEnterCritical( void );

XBDLL_API void XBPLATFORM_fnLeaveCritical( void );
#endif

// From xbtfx.c

// From xbtimers.c
XBDLL_API tucBOOL XBTIMERS_fnSetTxTimer( // PGN
                                         uint32 ulPgn,
                                         // Destination address
                                         uchar8 ucDest,
                                         // Period in centiseconds
                                         uint16 uiPeriod,
                                         // Time of first period
                                         uint16 uiStartAt );

XBDLL_API void XBTIMERS_fnEnablePeriodReports( tucBOOL tucActive );// Reports On

XBDLL_API void TFXDRV_CloseDriver( void );

XBDLL_API XB_teADDR_STATE XBADDR_fnAddrState( void );
#ifdef __cplusplus
}
#endif   // _cplusplus

#endif // XBGDEFS_INCL

//*****************************************************************************
// End of file.
//! @}
//*****************************************************************************
