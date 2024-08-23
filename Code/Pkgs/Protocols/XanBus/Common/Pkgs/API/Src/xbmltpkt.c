/*==============================================================================
Copyright 2003 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:
    xbmltpkt.c

PURPOSE:
    This file provides functions for handling the sending and receiving of
    multipacket messages.  Multipacket messages use the ISO Connection
    Management and ISO Data Transfer messages to transfer messages that are
    longer than a single frame (8 bytes).  The Fast Packet protocol provides
    another method of transferring messages longer than a frame.

FUNCTION(S):
    XBMLTPKT_fnStartTx      - Try to start a multipacket transfer
    XBMLTPKT_fnRecvRts      - Handle receiving of an RTS message
    XBMLTPKT_fnRecvCts      - Handle receiving of a CTS message
    XBMLTPKT_fnRecvEom      - Handle receiving of an EOM message
    XBMLTPKT_fnRecvAbort    - Handle receiving of an Abort message
    XBMLTPKT_fnRecvBam      - Handle receiving of a BAM message
    XBMLTPKT_fnRecvRawDataXfer  - Handle receiving of a Data Transfer message

    local:
        xbmltpkt_fnSendCts      - Send Clear To Send
        xbmltpkt_fnSendRts      - Send a Request To Send message
        xbmltpkt_fnSendBam      - Send Broadcast Announce Message
        xbmltpkt_fnSendAbort    - Send Abort Message message
        xbmltpkt_fnSendEom      - Send End Of Message message
        xbmltpkt_fnBamTxCallBack - Callback routine for BAM transmit complete
        xbmltpkt_fnBamDataToCallback - Callback routine for BAM data timer
        xbmltpkt_fnRtsTxCallBack - Callback routine for RTS transfer timer
        xbmltpkt_fnDataRxCallBack - Multipacket Data receive timeout
        xbmltpkt_fnCreateRxDob  - Create DOB for receiving data

NOTES:
   Sending a frame:
     If frame length is greater than 8 and not a fast packet
          if address is global send a BAM and start protocol timer
          otherwise send an RTS
     When BAM protocol timer expires send a frame
          if more to send restart BAM protocol timer
     When CTS received send as many frames as CTS allows
     If Abort received discard transmitting DOB
     If EOM received and all frames sent discard transmitting DOB

   Receiving a frame:
     If receive BAM create receiving DOB
     If receive RTS create receiving DOB, send CTS or Abort
     When receive Data fill receiving DOB if set
          if DOB full and BAM then handle message
          if DOB full and not BAM then send EOM and handle message

CHANGE HISTORY:
$Log: xbmltpkt.c $

    ***********************************************
    Revision: XanBus_HollyZ/3
    User: HollyZ     Date: 08/22/07  Time: 10:50PM
    In xbmltpkt_fnCreateRxDob(), Set DOB_DATA_FROM_HEAP flag before the DOB
    is put in the holder so that the flag field would be freed later even if
    the DOB is failed to be allocated

    ***********************************************
    Revision: XanBus_HollyZ/2
    User: HollyZ     Date: 12/08/06  Time: 09:58PM
    Fixed the bug on multi-packet message transfering.

    ***********************************************
    Revision: XanBus_HenryW/2
    User: HenryW     Date: 12/20/04  Time: 07:23PM
    trying to get XanBus to build again... after removing RVC and NMEA2000


    *****************  Version 57  *****************
    User: Peterd       Date: 24/03/04   Time: 3:54p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/AGSEx/XB/API/Src
    Added support for received frame flags

    *****************  Version 48  *****************
    User: Peterd       Date: 4/02/04    Time: 10:25a
    Updated in $/PD/Common/Protocols/XanBus/Code/Windows/CANCardX/Test/DiagWinEx/XB/API/Src
    Added check for own address when processing multi-packet data transfers
    to prevent freeing multi-packet DOBs that this node is sending.

    *****************  Version 47  *****************
    User: Peterd       Date: 4/02/04    Time: 10:04a
    Updated in $/PD/Common/Protocols/XanBus/Code/Windows/ifak_usb/Test/DiagWinEx/XB/API/Src
    Corrected negative test for BAM

    *****************  Version 42  *****************
    User: Toml         Date: 12/15/03   Time: 3:57p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/UIEx/XB/API/Src
    Replaced TFXEnvMalloc with XBMEM_fnMalloc and TFXEnvFree with
    XBMEM_fnFree in order for XanBus to have its own dedicated heap.

    *****************  Version 40  *****************
    User: Peterd       Date: 10/12/03   Time: 3:09p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/BattMonEx/XB/API/Src
    Changed #include <assert.h> to #include "xassert.h"

    *****************  Version 38  *****************
    User: Peterd       Date: 9/12/03    Time: 2:48p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/BattMonEx/XB/API/Src
    Updated per Code Review (See
    $/PD/Common/Protocols/XanBus/Admin/CodeReviewSummary.xls for
    specifics):
    - copyright
    - enum prefixing e to te
    - DIM to XB_mDIM
    - lint corrections
    - asserts

    *****************  Version 32  *****************
    User: Dalem        Date: 10/08/03   Time: 3:34p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/AGSEx/XB/API/Src
    Change Some error reporting to stats counting
    CreateRxDob was corrupting memory
    Improved validatation of CTS messages
    Improved validatation of DataXfer receive
    Changed search for DOB in ReceiveAbort

    *****************  Version 25  *****************
    User: Dalem        Date: 8/18/03    Time: 1:44p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/AGSEx/XB/API/Src
    Group Don't Care definition changed

    *****************  Version 24  *****************
    User: Dalem        Date: 8/06/03    Time: 4:06p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/AGSEx/XB/API/Src
    Set control flag in DOB for multipackets

    *****************  Version 23  *****************
    User: Dalem        Date: 8/05/03    Time: 3:44p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/AGSEx/XB/API/Src
    Changed enums to PGN_eESRC_xxx
    Updated function descriptions
    Rearrange code for review

    *****************  Version 21  *****************
    User: Peterd       Date: 7/24/03    Time: 11:21a
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DP256B/Test/BattMonEx/XB/API/Src
    Corrected enumerations to follow updated coding standard:

    typedef enum eXXXX
    {
        <pkg>_eXXXX_YYYY = 0;
        <pkg>_eXXXX_ZZZZ
    } <pkg>_teXXXX;

    Changed prefixes uli and sli to ul and sl respectively.

    Added shortening of enumerated type names.

    *****************  Version 20  *****************
    User: Dalem        Date: 7/18/03    Time: 10:07a
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/AGSEx/XB/API/Src
    Changed capitalization for EOM, BAM and DOB
    Put multiple parameters on separate lines

    *****************  Version 19  *****************
    User: Peterd       Date: 7/17/03    Time: 11:56a
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DP256B/Test/BattMonEx/XB/API/Src
    Changed enum/field names based on xbudefs.h generated using
    xanbus_headergen.py and xb.py labelled 0.5.1 and XanBusDB.mdb labelled
    0.3.1.

    Changed PGN_ISOTP_CONN_MAN_x structure names to PGN_ISO_TP_CONN_MAN_x.

    Fixe case of some function and variable names to comply with coding
    standard.

    *****************  Version 18  *****************
    User: Peterd       Date: 7/11/03    Time: 4:14p
    Updated in $/PD/Common/Protocols/XanBus/Code/Common/XB/API/Src
    Updated per changes in xanbus_headergen.py and xb.py scripts labeled
    0.5.0.
    Generated enum and struct type definitions now comply with coding
    standard.

    *****************  Version 17  *****************
    User: Dalem        Date: 7/08/03    Time: 2:05p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DP256B/Test/BattMonEx/XB/API/Src
    Change error reporting

    *****************  Version 16  *****************
    User: Dalem        Date: 7/07/03    Time: 8:54a
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DP256B/Test/BattMonEx/XB/API/Src
    Do not quit if PGN not found
    Do not allocate buffer if PGN not found
    Change case to ulPgn

    *****************  Version 15  *****************
    User: Dalem        Date: 7/03/03    Time: 10:00a
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DP256B/Test/BattMonEx/XB/API/Src
    Reuse info pointer instead of refinding it

    *****************  Version 14  *****************
    User: Toml         Date: 6/26/03    Time: 2:39p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/BattMonEx/XB/API/Src
    New API

    *****************  Version 15  *****************
    User: Dalem        Date: 6/24/03    Time: 2:48p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DP256B/Test/BattMonEx/XB/API/Src
    Major change to XB API and structure names

==============================================================================*/

/*==============================================================================
                              Includes
==============================================================================*/

#include "LIB_stddefs.h"    // Xantrex Standard Library definitions

#include <string.h>         // Standard library string and memory interface

// Xanbus include files
#include "xbldefs.h"    // Local Definitions
#include "xbmem.h"          // Xantrex Heap Memory interface
#include "config.h"

// System include files
#include "xassert.h"        // Xantrex assert interface

/*==============================================================================
                              Defines
==============================================================================*/

#define XB_MP_DATA_SIZE         ( PGN_MAX_LEN_SINGLEFRAME - 1 )

#define XB_BAM_TX_WAIT_MS       ( 50 )   // Wait between sending BAM packets
#define XB_RTS_TX_WAIT_MS       ( 1250 ) // Wait for CTS after RTS
#define XB_DATA_RX_WAIT_MS      ( 250 )  // Wait after data receive has started
#define XB_RTS_RX_WAIT_MS       ( 1250 ) // Wait for first non-BAM receive packet
#define XB_RTS_RX_0_CTS_WAIT_MS ( 550 )  // Wait for CTS after zero packet CTS

/*==============================================================================
                              Forward declarations
==============================================================================*/

static void xbmltpkt_fnSendCts( uchar8 ucDestAddr,
                                uchar8 ucFramesToSend,
                                uchar8 ucFrameNumber,
                                uint32 ulPgn );
static void xbmltpkt_fnSendRts( uchar8 ucDestAddr,
                                uchar8 ucFramesToSend,
                                uint16 uiMessageSize,
                                uint32 ulPgn );
static PGNDOB *xbmltpkt_fnSendBam( uchar8 ucDestAddr,
                                   uchar8 ucFramesToSend,
                                   uint16 uiMessageSize,
                                   uint32 ulPgn );
static void xbmltpkt_fnSendAbort( uchar8 ucDestAddr,
                                  uint32 ulPgn );
static void xbmltpkt_fnSendEom( uchar8 ucDestAddr,
                                uchar8 ucFramesIn,
                                uint16 uiMessageSize,
                                uint32 ulPgn );
static int xbmltpkt_fnBamTxCallBack( TFXLIB_CB_REASON tzReason,
                                     uint32 ulData1,
                                     uint32 ulData2,
                                     uint32 ulUserData );
static int xbmltpkt_fnBamDataToCallBack( TFXLIB_CB_REASON tzReason,
                                         uint32 ulData1,
                                         uint32 ulData2,
                                         uint32 ulUserData );
static int xbmltpkt_fnRtsTxCallBack( TFXLIB_CB_REASON tzReason,
                                     uint32 ulData1,
                                     uint32 ulData2,
                                     uint32 ulUserData );
static int xbmltpkt_fnDataRxCallBack( TFXLIB_CB_REASON tzReason,
                                      uint32 ulData1,
                                      uint32 ulData2,
                                      uint32 ulUserData );
static  PGNDOB *xbmltpkt_fnCreateRxDob( uchar8 ucSrcAddr,
                                        uint32 ulPgn,
                                        uchar8 ucFrameCount,
                                        uint16 uiMsgSize,
                                        tucBOOL tucBam );

/*==============================================================================
                           Function Definitions
==============================================================================*/

/******************************************************************************

FUNCTION NAME:
    xbmltpkt_fnSendCts

PURPOSE:
    This functions send a ISO Transport Protocol Connection Management
    Clear To Send message. This message is used for multipacket data
    transfers

INPUTS:
    'ucDestAddr' is destination address of this CTS message
    'ucFramesToSend' is number of frames other end allowed to send
    'ucFrameNumber' is starting frame number of the overall message
    'ulPgn' is PGN that is being transferred

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.02  Date: 06/24/03  By: Dalem
    - Major change to XB API

******************************************************************************/

static void xbmltpkt_fnSendCts( uchar8 ucDestAddr,
                                uchar8 ucFramesToSend,
                                uchar8 ucFrameNumber,
                                uint32 ulPgn )
{
    XB_tzPGN_ISO_TP_CONN_MAN_CTS    tzCts;

    // Fill in fields of Clear To Send
    tzCts.ucCTSGroupFunctionCode    = XB_ISO_CONN_MAN_CTS;
    tzCts.ucNumFramesThatCanBeSent  = ucFramesToSend;
    tzCts.ucNumNextFrameToBeSent    = ucFrameNumber;
    tzCts.ulParmGrpNum             = ulPgn;

    // Send of ISO Connection Management message
    (void) XBMSG_fnSendEx( XB_PGN_ISO_TP_CONN_MAN,
                           XB_ISO_CONN_MAN_CTS,
                           &tzCts,
                           sizeof( tzCts ),
                           ucDestAddr );
}

/******************************************************************************

FUNCTION NAME:
    xbmltpkt_fnSendRts

PURPOSE:
    This functions send a ISO Transport Protocol Connection Management
    Request To Send message. This message is used for multipacket data
    transfers


INPUTS:
    'ucDestAddr' is the destination address of this RTS message
    'ucFramesToSend' is the number of frames in the message
    'uiMessageSize' is the number of bytes in the message
    'ulPgn' is the PGN of the message being transferred

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.02  Date: 06/24/03  By: Dalem
    - Major change to XB API

******************************************************************************/

static void xbmltpkt_fnSendRts( uchar8 ucDestAddr,
                                uchar8 ucFramesToSend,
                                uint16 uiMessageSize,
                                uint32 ulPgn )
{
    XB_tzPGN_ISO_TP_CONN_MAN_RTS    tzRts;

    // Fill in fields of Request To Send
    tzRts.ucRTSGroupFunctionCode    = XB_ISO_CONN_MAN_RTS;
    tzRts.uiTotalMsgSz              = uiMessageSize;
    tzRts.ucTotalNumFramesXmit      = ucFramesToSend;
    tzRts.ulParmGrpNum             = ulPgn;

    // Send of ISO Connection Management message
    (void) XBMSG_fnSendEx( XB_PGN_ISO_TP_CONN_MAN,
                           XB_ISO_CONN_MAN_RTS,
                           &tzRts,
                           sizeof( tzRts ),
                           ucDestAddr );
}

/******************************************************************************

FUNCTION NAME:
    xbmltpkt_fnSendBam

PURPOSE:
    This functions send a ISO Transport Protocol Connection Management
    Broadcast Announce Message. This message is used for multipacket data
    transfers

INPUTS:
    'ucDestAddr' is the destination address of this BAM message
    'ucFramesToSend' is the number of frames in the message
    'uiMessageSize' is the number of bytes in the message
    'ulPgn' is the PGN of the message being transferred

OUTPUTS:
    A pointer to the queued BAM data object

NOTES:

VERSION HISTORY:

Version 1.02  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.03  Date: 2019-03-15  By: TomL
    - Now returning the pointer to the queued DOB so that a callback can be set

******************************************************************************/

static PGNDOB *xbmltpkt_fnSendBam( uchar8 ucDestAddr,
                                   uchar8 ucFramesToSend,
                                   uint16 uiMessageSize,
                                   uint32 ulPgn )
{
    XB_tzPGN_ISO_TP_CONN_MAN_BAM    tzBam;

    // Fill in fields of Broadcast Announce Message
    tzBam.ucBAMGroupFunctionCode    = XB_ISO_CONN_MAN_BAM;
    tzBam.uiTotalMsgSz              = uiMessageSize;
    tzBam.ucTotalNumFramesToBeSent  = ucFramesToSend;
    tzBam.ulParmGrpNum             = ulPgn;

    // Send of ISO Connection Management message
    return XBMSG_fnSendExPdob( XB_PGN_ISO_TP_CONN_MAN,
                               XB_ISO_CONN_MAN_BAM,
                               &tzBam,
                               sizeof( tzBam ),
                               ucDestAddr,
                               XB_REG_BUF );
}

/******************************************************************************

FUNCTION NAME:
    xbmltpkt_fnSendAbort

PURPOSE:
    This functions send a ISO Transport Protocol Connection Management
    Abort Message message. This message is used for multipacket data
    transfers

INPUTS:
    'ucDestAddr' is the destination address of this Abort message
    'ulPgn' is the PGN of the message being transferred

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.02  Date: 06/24/03  By: Dalem
    - Major change to XB API

******************************************************************************/

static void xbmltpkt_fnSendAbort( uchar8 ucDestAddr,
                                  uint32 ulPgn )
{
    XB_tzPGN_ISO_TP_CONN_MAN_ABORT tzAbort;

    // Fill in fields of End Of Message
    tzAbort.ucAbortGroupFunctionCode = XB_ISO_CONN_MAN_ABORT;
    tzAbort.ulParmGrpNum            = ulPgn;

    // Send of ISO Connection Management message
    (void) XBMSG_fnSendEx( XB_PGN_ISO_TP_CONN_MAN,
                           XB_ISO_CONN_MAN_ABORT,
                           &tzAbort,
                           sizeof( tzAbort ),
                           ucDestAddr );
}

/******************************************************************************

FUNCTION NAME:
    xbmltpkt_fnSendEom

PURPOSE:
    This functions send a ISO Transport Protocol Connection Management
    End Of Message message. This message is used for multipacket data
    transfers

INPUTS:
    'ucDestAddr' is the destination address of this EOM message
    'ucFramesIn' is total number of frames received
    'uiMessageSize' is the total size in bytes of the received message
    'ulPgn' is the PGN of the message being transferred

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.02  Date: 06/24/03  By: Dalem
    - Major change to XB API

******************************************************************************/

static void xbmltpkt_fnSendEom( uchar8 ucDestAddr,
                                uchar8 ucFramesIn,
                                uint16 uiMessageSize,
                                uint32 ulPgn )
{
    XB_tzPGN_ISO_TP_CONN_MAN_EOM    tzEom;

    // Fill in fields of End Of Message
    tzEom.ucEOMGroupFunctionCode    = XB_ISO_CONN_MAN_EOM;
    tzEom.uiTotalMsgSz              = uiMessageSize;
    tzEom.ucTotalNumFramesRXed      = ucFramesIn;
    tzEom.ulParmGrpNum             = ulPgn;

    // Send of ISO Connection Management message
    (void) XBMSG_fnSendEx( XB_PGN_ISO_TP_CONN_MAN,
                           XB_ISO_CONN_MAN_EOM,
                           &tzEom,
                           sizeof( tzEom ),
                           ucDestAddr );
}

/******************************************************************************

FUNCTION NAME:
    xbmltpkt_fnBamTxCallBack

PURPOSE:
    This function is used to handle the DOB timeout when transmitting
    a multipacket message to the global address.  The individual data
    transfer messages are preceeded by a Broadcast Announce Message and
    the data transfer messages are then broadcast at 50 millisecond
    intervals.

INPUTS:
    'tzReason' is reason this callback routine is called
    'ulData1' is a generic parameter used here for DOB pointer
    'ulData2' is a generic parameter not used
    'ulUserData' is the pDOB of the data to broadcast

OUTPUTS:
    TRUE if BAM completed successfully
    FALSE otherwise

NOTES:
    Function prototype designed to comply with Teleflex expectations

VERSION HISTORY:

Version 1.01  Date: 2019-03-15  By: TomL
    - Added to ensure that BAM comes before data frames even when very busy

******************************************************************************/

static int xbmltpkt_fnBamTxCallBack( TFXLIB_CB_REASON tzReason,
                                     uint32 ulData1,
                                     uint32 ulData2,
                                     uint32 ulUserData )
{
    PGNDOB *ptzDob;
    PGNDOB *ptzDataDob;

    // Avoid compiler warnings
    UNUSED( ulData2 );

    // Convert user data 1 to PGN
    ptzDob = (PGNDOB *)ulData1;
    // Avoid compiler warning
    (void)ptzDob;
    ptzDataDob = (PGNDOB *)ulUserData;

    // If reason is transmit complete
    if( tzReason == TFXCB_PGN_XMIT )
    {
        // Adjust the timer on the Data DOB to transmit after 50ms
        // this timer had originally been set to over a second.
        PDOBSetTimer( ptzDataDob,
                      XB_BAM_TX_WAIT_MS );

    }

    // Discard this DOB at the end
    return TRUE;
}

/******************************************************************************

FUNCTION NAME:
    xbmltpkt_fnBamDataToCallBack

PURPOSE:
    This function is used to handle the DOB timeout when transmitting
    a multipacket message to the global address.  The individual data
    transfer messages are preceeded by a Broadcast Announce Message and
    the data transfer messages are then broadcast at 50 millisecond
    intervals.

INPUTS:
    'tzReason' is reason this callback routine is called
    'ulData1' is a generic parameter used here for DOB pointer
    'ulData2' is a generic parameter not used
    'ulUserData' is the user data parameter not used

OUTPUTS:
    TRUE if BAM completed successfully
    FALSE otherwise

NOTES:
    Function prototype designed to comply with Teleflex expectations

VERSION HISTORY:

Version 1.01  Date: 06/24/03  By: Dalem
    - Major change to XB API

******************************************************************************/

static int xbmltpkt_fnBamDataToCallBack( TFXLIB_CB_REASON tzReason,
                                         uint32 ulData1,
                                         uint32 ulData2,
                                         uint32 ulUserData )
{
    PGNDOB *ptzDob;

    // Avoid compiler warnings
    UNUSED( ulUserData );
    UNUSED( ulData2 );

    // Convert user data 1 to PGN
    ptzDob = (PGNDOB *)ulData1;

    // If reason is time out
    if( tzReason == TFXCB_PGN_TO )
    {
        // If there is more data to send
        if( ptzDob->m_ucCurrentFrame <= ptzDob->m_ucTotalFrames )
        {
            // Set to send one more multipacket frame
            ptzDob->m_uiFramesToSend = 1;

            // DOB should be in the data state
            ptzDob->m_eState = XDATA;

            // Restart protocol timeout
            PDOBSetTimer( ptzDob,
                          XB_BAM_TX_WAIT_MS );

            // Do not discard DOB
            return( FALSE );
        }
        else
        {
            // Done, discard DOB
            return( TRUE );
        }
    }
    else
    {
        // Save error
        XBINIT_fnSaveError( PGN_eESRC_MULTIPACKET,
                            ( uint16 )PGN_eERR_NOT_SET );
    }

    // Failure or done, discard DOB
    return( TRUE );
}

/******************************************************************************

FUNCTION NAME:
    xbmltpkt_fnRtsTxCallBack

PURPOSE:
    This function is used to handle the DOB timeout when transmitting
    a multipacket message to a specific address.  The individual data
    transfer messages are preceeded by sending a Request To Send message
    and receiving a Clear To Send response.  If the handshake or data
    transfers fails to complete in the allotted time this timeout occurs.

INPUTS:
    'tzReason' is reason this callback routine is called
    'ulData1' is a generic parameter used here for DOB pointer
    'ulData2' is a generic parameter not used
    'ulUserData' is the user data parameter not used

OUTPUTS:
    TRUE

NOTES:
    Function prototype designed to comply with Teleflex expectations

VERSION HISTORY:

Version 1.01  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.02  Date: 10/08/03  By: Dalem
    - Change error report to comm stats counting

******************************************************************************/

static int xbmltpkt_fnRtsTxCallBack( TFXLIB_CB_REASON tzReason,
                                     uint32 ulData1,
                                     uint32 ulData2,
                                     uint32 ulUserData )
{
    // Avoid compiler warnings
    UNUSED( ulUserData );
    UNUSED( ulData1 );
    UNUSED( ulData2 );

    // If reason is time out
    if( tzReason == TFXCB_PGN_TO )
    {
        // Count this timeout event
        XBMSG_tzXbCommStats.ucMPTxTimeout++;
    }
    else
    {
        // Save error
        XBINIT_fnSaveError( PGN_eESRC_MULTIPACKET,
                            ( uint16 )PGN_eERR_NOT_SET );
    }

    // DOB has timed out, allow it to be removed
    return( TRUE );
}

/******************************************************************************

FUNCTION NAME:
    xbmltpkt_fnDataRxCallBack

PURPOSE:
    This function is used to handle timing out when receiving a multipacket
    data transfer. If the transfer of a multipacket message does not
    complete in the allotted time then this function is called.

INPUTS:
    'tzReason' is reason this callback routine is called
    'ulData1' is a generic parameter used here for DOB pointer
    'ulData2' is a generic parameter not used
    'ulUserData' is the user data parameter not used

OUTPUTS:
    TRUE

NOTES:
    Function prototype designed to comply with Teleflex expectations

VERSION HISTORY:

Version 1.01  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.02  Date: 10/08/03  By: Dalem
    - Change error report to comm stats counting

******************************************************************************/

static int xbmltpkt_fnDataRxCallBack( TFXLIB_CB_REASON tzReason,
                                      uint32 ulData1,
                                      uint32 ulData2,
                                      uint32 ulUserData )
{
    PGNDOB *ptzDob;

    // Avoid compiler warnings
    UNUSED( ulUserData );
    UNUSED( ulData2 );

    // If reason is time out
    if( tzReason == TFXCB_PGN_TO )
    {
        // Convert user data 1 to PGN
        ptzDob = (PGNDOB *)ulData1;

        // If multipacket DOB
        if( ( ptzDob->m_ucFlags & DOB_IS_MULTIPACKET ) != 0 )
        {
            // Count this timeout event
            XBMSG_tzXbCommStats.ucMPRxTimeout++;
        }
        else
        {
            // Save error
            XBINIT_fnSaveError( PGN_eESRC_MULTIPACKET,
                                ( uint16 )PGN_eERR_NOT_SET );
        }
    }
    else
    {
        // Save error
        XBINIT_fnSaveError( PGN_eESRC_MULTIPACKET,
                            ( uint16 )PGN_eERR_NOT_SET );
    }

    // DOB has timed out, allow it to be removed
    return( TRUE );
}

/******************************************************************************

FUNCTION NAME:
    XBMLTPKT_fnStartTx

PURPOSE:
    This function is used to check to see if a multipacket transfer can be
    started and will set up the DOB and send either a BAM or RTS
    depending on the destination of the multipacket message.

INPUTS:
    'ptzDob' is Data Object of the multipacket message being transferred

OUTPUTS:
    FALSE if multipacket in progress and another cannot be started
    TRUE if multipacket started

NOTES:

VERSION HISTORY:

Version 1.04  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.05  Date: 2019-03-15  By: TomL
    - Ensured that broadcast data frames don't get sent before BAM

******************************************************************************/

tucBOOL XBMLTPKT_fnStartTx( PGNDOB *ptzDob )
{
    uchar8 ucFrameCount;
    PGNDOB *ptzSearch;
    PGNDOB *ptzBamDob;

    // Verify parameter
    (void)assert( ptzDob != NULL );

    // Search through DOBs in holder
    for( ptzSearch = DOBHLDFirst();
         ptzSearch != NULL;
         ptzSearch = DOBHLDNext( ptzSearch ) )
    {
        // If DOB is a multipacket, source address is me
        // and it is waiting
        if( ( ( ptzSearch->m_ucFlags & DOB_IS_MULTIPACKET ) != 0 )
         && ( ptzSearch->m_SourceAddr == XBADDR_fnGetMyAddr() )
         && ( ptzSearch->m_eState == XWAIT ) )
        {
            // Multipacket send in progress, do not start another
            return( FALSE );
        }
    }

    // Calculate number of frames, rounding up
    ucFrameCount = (uchar8) ( ( ptzDob->m_i16ByteCount +
                    XB_MP_DATA_SIZE - 1 ) / XB_MP_DATA_SIZE );

    // Set current frame and number of frames
    ptzDob->m_ucCurrentFrame = 1;   // First frame is numbered 1
    ptzDob->m_ucTotalFrames = ucFrameCount;

    // No handle assigned yet
    ptzDob->m_CanHandle = INV_MSG_HANDLE;

    // Set DOB state to waiting
    ptzDob->m_eState = XWAIT;

    // If destination is global address
    if( ptzDob->m_DestAddr == PGN_ADDR_GLOBAL )
    {
        // Send a broadcast announce
        ptzBamDob = xbmltpkt_fnSendBam( ptzDob->m_DestAddr,
                                        ucFrameCount,
                                        ( uint16 )ptzDob->m_i16ByteCount,
                                        ( uint32 )ptzDob->m_PGN );

        // Set callback on transmit of the BAM frame.
        // This will set the data frames to start sending 50 ms afterwards
        PDOBSetCallback( ptzBamDob,
                         xbmltpkt_fnBamTxCallBack,
                         (uint32)ptzDob );

        // Set BAM callback and start timer
        PDOBSetCallback( ptzDob,
                         xbmltpkt_fnBamDataToCallBack,
                         0L ); // User callback parameter is zero
        PDOBSetTimer( ptzDob,
                      XB_RTS_TX_WAIT_MS );
    }
    else
    {
        // Send a request to send
        xbmltpkt_fnSendRts( ptzDob->m_DestAddr,
                            ucFrameCount,
                            ( uint16 )ptzDob->m_i16ByteCount,
                            ( uint32 )ptzDob->m_PGN );

        // Set RTS callback and start timer
        PDOBSetCallback( ptzDob,
                         xbmltpkt_fnRtsTxCallBack,
                         0L ); // User callback parameter is zero
        PDOBSetTimer( ptzDob,
                      XB_RTS_TX_WAIT_MS );
    }

    // Success
    return( TRUE );
}

/******************************************************************************

FUNCTION NAME:
    xbmltpkt_fnCreateRxDob

PURPOSE:
    This function is used to obtain a Data Object to use for receiving
    a multipacket data transfer.

INPUTS:
    'ucSrcAddr' is source address of the multipacket message
    'ulPgn' is PGN of the multipacket message
    'ucFrameCount' is number of frames expected for the multipacket message
    'uiMsgSize' is number of bytes expected for the multipacket message
    'tucBam' is TRUE if msg is being transferred as a Broadcast Announce Message

OUTPUTS:
    pointer to the allocated DOB

NOTES:
    A DOB is allocated for the PGN even if the PGN is not a known PGN.
    There is no buffer allocated for the PGN. The PGN will be NACKed
    after the data transfer is complete.

VERSION HISTORY:

Version 1.02  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.03  Date: 07/03/03  By: Dalem
    - Do not quit if PGN not found
    - Do not allocate buffer if PGN not found
Version 1.04  Date: 10/08/03  By: Dalem
    - Malloc was using one size and memset was using another
Version 1.05  Date: 08/20/08  By: Hollyz
    - Set DOB_DATA_FROM_HEAP flag before the DOB is put in the holder

******************************************************************************/

static  PGNDOB *xbmltpkt_fnCreateRxDob( uchar8 ucSrcAddr,
                                        uint32 ulPgn,
                                        uchar8 ucFrameCount,
                                        uint16 uiMsgSize,
                                        tucBOOL tucBam )
{
    PGNDOB *ptzDob;
    const PGN_tzPGN_INFO * ptzPgnInfo;
    uint16 uiResult;

	// Look up the PGN for the message being received
	ptzPgnInfo = XBPACK_fnGetPgnInfo( ulPgn,
                                     ( sint16 )XB_eGRP_FUNC_VALUE_DO_NOT_CARE );

    // If this is a broadcast message that we don't understand then there
    // is no point in receiving the rest of the message into a DOB.
    // If this is RTS/CTS directed to us then we want to hold a DOB
    // so that we can wait until the end and reply with NAK.
    // But broadcasts? we don't reply to those.
    if (( tucBam == TRUE )
        && ( ptzPgnInfo == NULL ))
    {
        // Do not listen to the data frames that will follow
        return NULL;
    }
	
    // Allocate a DOB for message to be received
    uiResult = ( uint16 )PDOBConstructAlloc( &ptzDob,
                                   ( long )ulPgn,
                                   ( sint16 )XB_eGRP_FUNC_VALUE_DO_NOT_CARE,
                                   ptzPgnInfo );

    // If allocation success
    if( uiResult == ( uint16 )TFXCR_OK )
    {
        ptzDob->m_NRxFrameFlags = ucFrameCount;

        // Allocate space for received frame flags, one flag per frame, eight flags per byte
        ptzDob->m_pRxFrameFlags = ( uchar8 * )XBMEM_fnMalloc( ( ucFrameCount + ( XT_BITS_PER_BYTE - 1 ) ) / XT_BITS_PER_BYTE );

        if ( ptzDob->m_pRxFrameFlags == NULL )
        {
            // Destroy allocated DOB
            PDOBDestruct( ptzDob );

            // Save error
            XBINIT_fnSaveError( PGN_eESRC_MULTIPACKET,
                                ( uint16 )PGN_eERR_NO_MEM );

            return NULL;
        }

        // Clear 'em out
        ( void )memset( ptzDob->m_pRxFrameFlags, 0,
                        ( ucFrameCount + ( XT_BITS_PER_BYTE - 1 ) ) / XT_BITS_PER_BYTE );

        // Set multipacket flag
        ptzDob->m_ucFlags |= DOB_IS_MULTIPACKET;

        // Remember buffer is from heap
        ptzDob->m_ucFlags |= DOB_DATA_FROM_HEAP;

        // Put DOB in holder
        uiResult = ( uint16 )DOBHLDAdd( ptzDob );

        // If the DOB can be put in holder
        if( uiResult == ( uint16 )TFXCR_OK )
        {
            // If a Broadcast Announce Message
            if( tucBam != FALSE )
            {
                // Set BAM flag
                ptzDob->m_ucFlags |= DOB_IS_BAM;
            }
            else
            {
                // Clear BAM flag
                ptzDob->m_ucFlags &= ~DOB_IS_BAM;
            }

            // Set port
            PDOBSetPort( ptzDob,
                         XB_PORT_A );

            // Remember source address
            PDOBSetSourceAddr( ptzDob,
                               ucSrcAddr );

            // Set frame count, message size and current frame
            ptzDob->m_ucTotalFrames = ucFrameCount;
            ptzDob->m_i16ByteCount = ( short )uiMsgSize;
            ptzDob->m_ucCurrentFrame = 1;

            // Set multipacket flag
            //ptzDob->m_ucFlags |= DOB_IS_MULTIPACKET;

            // Remember buffer is from heap
            //ptzDob->m_ucFlags |= DOB_DATA_FROM_HEAP;

            // If PGN is known
            if( ptzDob->m_pPgnInfo != NULL )
            {
                // Allocate buffer for the message
                ptzDob->m_pData = XBMEM_fnMalloc( ucFrameCount * XB_MP_DATA_SIZE );

                // If buffer allocation is OK
                if( ptzDob->m_pData != NULL )
                {
                    // Initialize the buffer to zero
                    (void) memset( ptzDob->m_pData,
                                   0,
                                   ucFrameCount * XB_MP_DATA_SIZE );
                }
            }

            // Success return DOB pointer
            return( ptzDob );
        }
        else
        {
            // Destroy allocated DOB
            PDOBDestruct( ptzDob );

            // Save error
            XBINIT_fnSaveError( PGN_eESRC_MULTIPACKET,
                                ( uint16 )PGN_eERR_NO_MEM );
        }
    }
    else
    {
        // Save error
        XBINIT_fnSaveError( PGN_eESRC_MULTIPACKET,
                            ( uint16 )PGN_eERR_NO_MEM );
    }

    // Failure, return NULL
    return( NULL );
}

/******************************************************************************

FUNCTION NAME:
    XBMLTPKT_fnRecvRts

PURPOSE:
    This function is used to handle receiving of a Request to Send message.
    An RTS is part of the protocol for tranferring a multipacket message.
    A DOB is created for the message and a Clear to Send message is sent.

INPUTS:
    'ptzRts' is pointer to the RTS message
    'ucSrcAddr' is source address of the RTS message

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.03  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.04  Date: 10/08/03  By: Dalem
    - Check that frame count and message size match up

******************************************************************************/

void XBMLTPKT_fnRecvRts( XB_tzPGN_ISO_TP_CONN_MAN_RTS *ptzRts,
                         uchar8 ucSrcAddr )
{
    uint16 uiMsgSize;
    uchar8 ucFrameCount;
    uint32 ulPgn;
    PGNDOB *ptzDob;

    // Verify parameter
    (void)assert( ptzRts != NULL );

    // Get local copies of data for clarity
    uiMsgSize    = ptzRts->uiTotalMsgSz;
    ucFrameCount = ptzRts->ucTotalNumFramesXmit;
    ulPgn       = ptzRts->ulParmGrpNum;

    // If message size does not fit with frame count
    if( uiMsgSize > ( ucFrameCount * XB_MP_DATA_SIZE ) )
    {
        // Message size mismatch, send an Abort message
        xbmltpkt_fnSendAbort( ucSrcAddr,
                              ulPgn );

        // Quit
        return;
    }

    // Create a DOB for receiving a not BAM data transfer
    ptzDob = xbmltpkt_fnCreateRxDob( ucSrcAddr,
                                     ulPgn,
                                     ucFrameCount,
                                     uiMsgSize,
                                     FALSE );

    // If DOB created
    if( ptzDob != NULL )
    {
        // DOB created, send a Clear To Send message
        xbmltpkt_fnSendCts( ucSrcAddr,
                            ucFrameCount,
                            1,
                            ulPgn );

        // Set RTS receive callback and start timer
        PDOBSetCallback( ptzDob,
                         xbmltpkt_fnDataRxCallBack,
                         0L );
        PDOBSetTimer( ptzDob,
                      XB_RTS_RX_WAIT_MS );
    }
    else
    {
        // No receiving DOB, send an Abort message
        xbmltpkt_fnSendAbort( ucSrcAddr,
                              ulPgn );
    }
}

/******************************************************************************

FUNCTION NAME:
    XBMLTPKT_fnRecvCts

PURPOSE:
    This function is used to handle receiving of a Clear to Send message.
    A CTS is part of the protocol for tranferring a multipacket message.
    A CTS usually starts a data transfer.

INPUTS:
    'ptzCts' is pointer to the CTS message
    'ucSrcAddr' is source address of the CTS message

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.04  Date: 06/24/03  By: Dalem
    - Major change to XB API
Version 1.05  Date: 10/08/03  By: Dalem
    - Check for request to send frame 0
    - Change error report to late CTS counting

******************************************************************************/

void XBMLTPKT_fnRecvCts( XB_tzPGN_ISO_TP_CONN_MAN_CTS *ptzCts,
                         uchar8 ucSrcAddr )
{
    PGNDOB *ptzDob;

    // Verify parameter
    (void)assert( ptzCts != NULL );

    // Search through DOBs in holder
    for( ptzDob = DOBHLDFirst(); ptzDob != NULL; ptzDob = DOBHLDNext( ptzDob ) )
    {
        // If DOB is a multipacket, source address is me
        // and source address of CTS matches DOB destination
        if( ( ( ptzDob->m_ucFlags & DOB_IS_MULTIPACKET ) != 0 )
         && ( ptzDob->m_SourceAddr == XBADDR_fnGetMyAddr() )
         && ( ptzDob->m_DestAddr == ucSrcAddr ) )
        {
            // End search
            break;
        }
    }

    // If DOB is found
    if( ptzDob != NULL )
    {
        // Set first frame and frames to send for multipacket DOB
        ptzDob->m_ucCurrentFrame = ptzCts->ucNumNextFrameToBeSent;
        ptzDob->m_uiFramesToSend = ptzCts->ucNumFramesThatCanBeSent;

        // If frame count would go beyond number of frames to send
        // or frame to send is zero (frames start at 1)
        if( ( ( ptzDob->m_ucCurrentFrame + ptzDob->m_uiFramesToSend - 1 ) >
            ptzDob->m_ucTotalFrames )
         || ( ptzDob->m_ucCurrentFrame == 0 ) )
        {
            // Send abort back to source
            xbmltpkt_fnSendAbort( ucSrcAddr,
                                  ( uint32 )ptzDob->m_PGN );

           // Remove and destroy DOB
           (void) CANFrameClearHandle(ptzDob->m_i8Port, ptzDob->m_CanHandle);
            DOBHLDDelete( ptzDob );
            PDOBDestruct( ptzDob );
        }
        else
        {
            // If the number of frames requested was zero
            if( ptzCts->ucNumFramesThatCanBeSent == 0 )
            {
                // Restart protocol timeout for a shortened period of time
                PDOBSetTimer( ptzDob,
                              XB_RTS_RX_0_CTS_WAIT_MS );
            }
            else
            {
                // Restart protocol timeout for the full RTS receive wait time
                PDOBSetTimer( ptzDob,
                              XB_RTS_RX_WAIT_MS );

                // Set DOB state to Data
                ptzDob->m_eState = XDATA;
            }
        }
    }
    else
    {
        // Count this late CTS
        XBMSG_tzXbCommStats.ucMPLateCts++;
    }
}

/******************************************************************************

FUNCTION NAME:
    XBMLTPKT_fnRecvEom

PURPOSE:
    This function is used to handle receiving of a End Of Message message.
    An EOM is part of the protocol for tranferring a multipacket message.
    The DOB for transmitting the multipacket message can be deallocated.

INPUTS:
    'ptzEom' is pointer to the EOM message
    'ucSrcAddr' is source address of the EOM message

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.00  Date: 02/05/03  By: code_fix.py
    - Added version history to function header
Version 1.01  Date: 02/18/03  By: Dalem
    - Save error if DOB not found
Version 1.02  Date: 10/08/03  By: Dalem
    - Change error report to late EOM counting

******************************************************************************/

void XBMLTPKT_fnRecvEom( XB_tzPGN_ISO_TP_CONN_MAN_EOM *ptzEom,
                         uchar8 ucSrcAddr )
{
    PGNDOB  *ptzDob;

    // Verify parameter
    (void)assert( ptzEom != NULL );

    // Avoid compiler warning
    UNUSED( ptzEom );

    // Search through DOBs in holder
    for( ptzDob = DOBHLDFirst(); ptzDob != NULL; ptzDob = DOBHLDNext( ptzDob ) )
    {
        // If DOB is a multipacket
        // and EOM source address matches DOB destination address
        if( ( ( ptzDob->m_ucFlags & DOB_IS_MULTIPACKET ) != 0 )
         && ( ptzDob->m_DestAddr == ucSrcAddr ) )
        {
            // End search
            break;
        }
    }

    // If we have the DOB corresponding to the EOM we just received
    if( ptzDob != NULL )
    {
        // Remove and destroy DOB
        (void) CANFrameClearHandle(ptzDob->m_i8Port, ptzDob->m_CanHandle);
        DOBHLDDelete( ptzDob );
        PDOBDestruct( ptzDob );
    }
    else
    {
        // Count this late EOM
        XBMSG_tzXbCommStats.ucMPLateEom++;
    }
}

/******************************************************************************

FUNCTION NAME:
    XBMLTPKT_fnRecvAbort

PURPOSE:
    This function is used to handle receiving of an Abort message. An
    abort is part of the protocol for tranferring a multipacket message.
    The DOB for transmitting the multipacket message can be deallocated.

INPUTS:
    'ptzAbort' is pointer to the Abort message
    'ucSrcAddr' is source address of the Abort message

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.00  Date: 02/05/03  By: code_fix.py
    - Added version history to function header
Version 1.01  Date: 02/18/03  By: Dalem
    - Save error if DOB not found
Version 1.01  Date: 10/08/03  By: Dalem
    - Change search for DOB, Abort can be for message Rx or Tx
    - Change error report to late Abort counting

******************************************************************************/

void XBMLTPKT_fnRecvAbort( XB_tzPGN_ISO_TP_CONN_MAN_ABORT *ptzAbort,
                           uchar8 ucSrcAddr )
{
    PGNDOB  *ptzDob;
    uchar8  ucMyAddr;

    // Verify parameter
    (void)assert( ptzAbort != NULL );

    // Avoid compiler warning
    UNUSED( ptzAbort );

    // Get my address
    ucMyAddr = XBADDR_fnGetMyAddr();

    // Search through DOBs in holder
    for( ptzDob = DOBHLDFirst(); ptzDob != NULL; ptzDob = DOBHLDNext( ptzDob ) )
    {
        // If DOB is a multipacket
        if( ( ptzDob->m_ucFlags & DOB_IS_MULTIPACKET ) != 0 )
        {
            // If I am the source of the DOB
            if( ptzDob->m_SourceAddr == ucMyAddr )
            {
                // Check is source of Abort is destination of DOB
                if( ptzDob->m_DestAddr == ucSrcAddr )
                {
                    // End search
                    break;
                }
            }
            else
            {
                // I am not the source of DOB, is source of Abort source of DOB
                if( ptzDob->m_SourceAddr == ucSrcAddr )
                {
                    // End search
                    break;
                }
            }
        }
    }

    // If we have the DOB corresponding to the Abort we just received
    if( ptzDob != NULL )
    {
        // Remove and destroy DOB
        (void) CANFrameClearHandle(ptzDob->m_i8Port, ptzDob->m_CanHandle);
        DOBHLDDelete( ptzDob );
        PDOBDestruct( ptzDob );
    }
    else
    {
        // Count this late Abort
        XBMSG_tzXbCommStats.ucMPLateAbort++;
    }
}

/******************************************************************************

FUNCTION NAME:
    XBMLTPKT_fnRecvBam

PURPOSE:
    This function is used to handle receiving of a Broadcast Announce message.
    A BAM is part of the protocol for tranferring a multipacket message.
    A DOB is created for the message and the BAM timer is started.

INPUTS:
    'ptzBam' is pointer to the BAM message
    'ucSrcAddr' is source address of the BAM message

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.00  Date: 02/05/03  By: code_fix.py
    - Added version history to function header
Version 1.01  Date: 02/28/03  By: Dalem
    - Removed unreferenced local variable

******************************************************************************/

void XBMLTPKT_fnRecvBam( XB_tzPGN_ISO_TP_CONN_MAN_BAM *ptzBam,
                         uchar8 ucSrcAddr )
{
    PGNDOB *ptzDob;

    // Verify parameter
    (void)assert( ptzBam != NULL );

    // Create a DOB for receiving a BAM data transfer
    ptzDob = xbmltpkt_fnCreateRxDob( ucSrcAddr,
                                     ptzBam->ulParmGrpNum,
                                     ptzBam->ucTotalNumFramesToBeSent,
                                     ptzBam->uiTotalMsgSz,
                                     TRUE );

    // If we are capable of receiving the BAM
    if( ptzDob != NULL )
    {
        // Set Data receive callback and start timer
        PDOBSetCallback( ptzDob,
                         xbmltpkt_fnDataRxCallBack,
                         0L );
        PDOBSetTimer( ptzDob,
                      XB_DATA_RX_WAIT_MS );
    }
}

/******************************************************************************

FUNCTION NAME:
    XBMLTPKT_fnRecvRawDataXfer

PURPOSE:
    This function is used to handle receiving of a Data Transfer message.
    A Data Transfer is part of the protocol for tranferring a multipacket
    message. The data is placed in the receiving DOB.  When the
    multipacket message is complete it is passed on for processing.
    This function was modified to be called directly by Tick processing to
    speed up processing.

INPUTS:
    'pucRawData' is pointer to raw the Data Transfer message
    'ucSrcAddr' is source address of the Data Transfer message

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.00  Date: 02/18/03  By: Dalem
    - Created from xbmltpkt_fnRecvDataXfer
Version 1.01  Date: 02/18/03  By: Dalem
    - Save error if DOB not found
Version 1.02  Date: 02/28/03  By: Dalem
    - Removed unreferenced local variable
Version 1.03  Date: 07/03/03  By: Dalem
    - Change to always check for end of message even if data does not fit
    - Copy data if there is a buffer and data will fit
Version 1.04  Date: 10/08/03  By: Dalem
    - Added length parameter and more error checking
    - Change error report to late EOM counting

******************************************************************************/

void XBMLTPKT_fnRecvRawDataXfer( uchar8 *pucRawData,
                                 uchar8 ucDataLen,
                                 uchar8 ucSrcAddr )
{
    PGNDOB *ptzDob;
    uchar8 ucFrameNo;
    uchar8 *pucPktData;

    // Verify parameter
    (void)assert( pucRawData != NULL );

    // Search through DOBs in holder
    for( ptzDob = DOBHLDFirst(); ptzDob != NULL; ptzDob = DOBHLDNext( ptzDob ) )
    {
        // If DOB is a multipacket and the source address matches
        if( ( ( ptzDob->m_ucFlags & DOB_IS_MULTIPACKET ) != 0 )
         && ( ptzDob->m_SourceAddr == ucSrcAddr ) )
            break;
    }

    // If there is a receiving DOB
    if( ptzDob != NULL )
    {
        // Get local variables for clarity of code
        ucFrameNo = pucRawData[ XB_MP_DX_FRAM_NUM_OFFS ];
        pucPktData = &pucRawData[ XB_MP_DX_DATA_OFFS ];

        // Check that this is not single frame (which is not multi-packet)
        // Check that we have not already received this frame
        if( ( ucDataLen != PGN_MAX_LEN_SINGLEFRAME )
         || ( ucFrameNo != ptzDob->m_ucCurrentFrame ) )
        {
            // If multipacket Rx message is not a Broadcast Announce Message
            if( ( ptzDob->m_ucFlags & DOB_IS_BAM ) == 0 )
            {
                // If the message is not from me
                if ( ucSrcAddr != XBADDR_fnGetMyAddr() )
                {
                    // Send an abort
                    xbmltpkt_fnSendAbort( ucSrcAddr,
                                        ( uint32 )ptzDob->m_PGN );
                }
            }
            else
            {
                // Do not send abort for BAM, so save an error
                XBINIT_fnSaveError( PGN_eESRC_MULTIPACKET,
                                    ( uint16 )PGN_eERR_BAD_VALUE );
            }

            // If the message is not from me
            if ( ucSrcAddr != XBADDR_fnGetMyAddr() )
            {
                // Remove and destroy DOB
                DOBHLDDelete( ptzDob );
                PDOBDestruct( ptzDob );
            }
        }
        else
        {
            // If there is a buffer and this frame fits in this multipacket
            if( ( ptzDob->m_pData != NULL )
             && ( ucFrameNo > 0 )
             && ( ucFrameNo <= ptzDob->m_ucTotalFrames ) )
            {
                // Copy data to multipacket DOB from transfer message
                (void) memcpy( &ptzDob->m_pData[ ( ucFrameNo - 1 ) * XB_MP_DATA_SIZE ],
                               pucPktData,
                               XB_MP_DATA_SIZE );

                // If we have not received this frame already
                if ( TestAndSetFlag( ptzDob->m_pRxFrameFlags,
                                    ptzDob->m_ucTotalFrames,
                                    ucFrameNo - 1 ) == FALSE )
                {
                    // Count frame
                    ptzDob->m_ucCurrentFrame++;
                }
            }

            // If we received all the frames
            if( ptzDob->m_ucCurrentFrame > ptzDob->m_ucTotalFrames )
            {
                // If multipacket Rx message is not a Broadcast Announce Message
                if( ( ptzDob->m_ucFlags & DOB_IS_BAM ) == 0 )
                {
                    // Send an End Of Message, data source becomes destination
                    xbmltpkt_fnSendEom( ucSrcAddr,
                                        (uchar8) ptzDob->m_ucTotalFrames,
                                        ( uint16 )ptzDob->m_i16ByteCount,
                                        ( uint32 )ptzDob->m_PGN );
                }

                // Set data receive complete flag
                ptzDob->m_ucFlags |= DOB_RECV_COMPLETE;

                // Handle the multipacket DOB as a received message
                XBMSG_fnRawReceive( ptzDob );
            }
            else
            {
                // Reset the receive timer
                PDOBSetTimer( ptzDob,
                              XB_DATA_RX_WAIT_MS );
            }
        }
    }
    else
    {
        // Count this late Xfer frame
        XBMSG_tzXbCommStats.ucMPLateXfer++;
    }
}

