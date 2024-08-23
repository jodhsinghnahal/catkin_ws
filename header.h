// ISO Acknowledgment
typedef struct RVC_zPGN_ISO_ACK
{
    RVC_teACK_STS teCtrlByte;       // Control Byte
    RVC_teGRP_FUNC_VALUE teGroupFunctionValue; // Group Function Value
    uint32 ulParmGrpNum;            // PGN of Requested Information
} RVC_tzPGN_ISO_ACK;

// Parameter Device Mode Config
typedef struct RVC_zPGN_PM_PARAM_DEV_MD_CFG
{
    RVC_teCTRL_MODE teDesiredMode;  // Desired Device Mode uchar8
    uchar8 ucReserved;              // Reserved
    uint16 uiReserved;              // Reserved
} RVC_tzPGN_PM_PARAM_DEV_MD_CFG;

// Parameter Association Config
typedef struct RVC_zPGN_PM_PARAM_ASSOC_CFG
{
    RVC_teASSN_TYPE teAssocType;    // Association Type uchar8
    uchar8 ucAssocInst;             // Association Instance
    uchar8 ucAssocId;               // Association Identifier
    uchar8 ucReserved;              // Reserved
} RVC_tzPGN_PM_PARAM_ASSOC_CFG;

// Parameter Auxiliary Configuration
typedef struct RVC_zPGN_PM_PARAM_AUX_CFG
{
    uchar8 ucOutputId;              // Output Identifier
    RVC_teAUX_DESIRED_STATE teDesiredState; // Desired State 4bit
    RVC_teAUX_ACTIVE_LVL teActiveLevel; // Active Level 4bit
    RVC_teAUX_TRIGGER_ID teSetTrigger; // Set Trigger Id uchar8
    RVC_teAUX_TRIGGER_ID teClrTrigger; // Clear Trigger Id uchar8
} RVC_tzPGN_PM_PARAM_AUX_CFG;

// Parameter Auxiliary Configuration Trigger Level
typedef struct RVC_zPGN_PM_PARAM_AUX_CFG_TRIG_LVL
{
    RVC_teAUX_TRIGGER_ID teTriggerId; // Trigger Id uchar8
    sint16 siTriggerLvl;            // Trigger Level
    uchar8 ucReserved;              // Reserved
} RVC_tzPGN_PM_PARAM_AUX_CFG_TRIG_LVL;

// Parameter Auxiliary Configuration Trigger Delay
typedef struct RVC_zPGN_PM_PARAM_AUX_CFG_TRIG_DLY
{
    RVC_teAUX_TRIGGER_ID teTriggerId; // Trigger Id uchar8
    uint16 uiTriggerDly;            // Trigger Delay
    uchar8 ucReserved;              // Reserved
} RVC_tzPGN_PM_PARAM_AUX_CFG_TRIG_DLY;

// Parameter Auxiliary Status
typedef struct RVC_zPGN_PM_PARAM_AUX_STS
{
    uchar8 ucOutputId;              // Output Identifier
    RVC_teAUX_OP_STATE teOpState;   // Operating State uchar8
    RVC_teAUX_ON_OFF_REASON teOnReason; // On Reason uchar8
    RVC_teAUX_ON_OFF_REASON teOffReason; // Off Reason uchar8
} RVC_tzPGN_PM_PARAM_AUX_STS;

// ISO Request
typedef struct RVC_zPGN_ISO_REQ
{
    uint32 ulParmGrpNum;            // PGN being requested
} RVC_tzPGN_ISO_REQ;

// ISO Transport Protocol, Data Transfer
typedef struct RVC_zPGN_ISO_TP_XFER
{
    uchar8 ucSeqNumMultiPacketFrame; // Sequence number of multi-packet frame
    uchar8 aucMultiPacketPacketizedData; //[ 7 ]; // Multi-packet packetized data
} RVC_tzPGN_ISO_TP_XFER;

// ISO Transport Protocol, Connection Management - RTS
typedef struct RVC_zPGN_ISO_TP_CONN_MAN_RTS
{
    uchar8 ucRtsGroupFunctionCode;  // RTS Group Function Code
    uint16 uiTotalMsgSz;            // Total message size, bytes
    uchar8 ucTotalNumFramesXmit;    // Total number of frames to be transmitted
    uint32 ulParmGrpNum;            // PGN of multi-packet message
} RVC_tzPGN_ISO_TP_CONN_MAN_RTS;

// ISO Transport Protocol, Connection Management - CTS
typedef struct RVC_zPGN_ISO_TP_CONN_MAN_CTS
{
    uchar8 ucCtsGroupFunctionCode;  // CTS Group Function Code
    uchar8 ucNumFramesThatCanBeSent; // Number of frames that can be sent
    uchar8 ucNumNextFrameToBeSent;  // Number of next frame to be sent
    uint32 ulParmGrpNum;            // PGN of multi-packet message
} RVC_tzPGN_ISO_TP_CONN_MAN_CTS;

// ISO Transport Protocol, Connection Management - EOM
typedef struct RVC_zPGN_ISO_TP_CONN_MAN_EOM
{
    uchar8 ucEomGroupFunctionCode;  // EOM Group Function Code
    uint16 uiTotalMsgSz;            // Total message size, bytes
    uchar8 ucTotalNumFramesRxed;    // Total number of frames received
    uint32 ulParmGrpNum;            // PGN of multi-packet message
} RVC_tzPGN_ISO_TP_CONN_MAN_EOM;

// ISO Transport Protocol, Connection Management - BAM
typedef struct RVC_zPGN_ISO_TP_CONN_MAN_BAM
{
    uchar8 ucBamGroupFunctionCode;  // BAM Group Function Code
    uint16 uiTotalMsgSz;            // Total message size, bytes
    uchar8 ucTotalNumFramesToBeSent; // Total number of frames to be sent
    uint32 ulParmGrpNum;            // PGN of multi-packet message
} RVC_tzPGN_ISO_TP_CONN_MAN_BAM;

// ISO Transport Protocol, Connection Management - Abort
typedef struct RVC_zPGN_ISO_TP_CONN_MAN_ABORT
{
    uchar8 ucAbortGroupFunctionCode; // Abort Group Function Code
    uint32 ulParmGrpNum;            // PGN of multi-packet message
} RVC_tzPGN_ISO_TP_CONN_MAN_ABORT;

// ISO Address Claim
typedef struct RVC_zPGN_ISO_ADDR_CLAIM
{
    uint32 ulUniqueNumber;          // Unique Number (ISO Identity Number)
    uint16 uiManufacturerCode;      // Manufacturer Code
    uchar8 ucDeviceInstanceLower;   // Device Instance Lower (ISO ECU Instance)
    uchar8 ucDeviceInstanceUpper;   // Device Instance Upper (ISO Function Instance)
    RVC_teFUNC_CODE teDeviceFunction; // Device Function (ISO Function)
    uchar8 ucRsvBit;                // Reserved Bit
    RVC_teDEVICE_CLASS teDeviceClass; // Device Class
    uchar8 ucSystemInstance;        // System Instance (ISO Device Class Instance)
    RVC_teINDUSTRY_GRP teIndustryGroup; // Industry Group
    uchar8 ucSelfCfgAddr;           // ISO Self Configurable Address
} RVC_tzPGN_ISO_ADDR_CLAIM;

// Proprietary Message Association Config
typedef struct RVC_zPGN_PM_ASSOC_CFG
{
    RVC_tePROP_MSG_ID teMsgId;      // Message Identifier
    RVC_teASSN_TYPE teAssocType;    // Association Type
    uchar8 ucAssocInst;             // Association Instance
    uchar8 ucAssocId;               // Association Identifier
    uint16 uiCrc;                   // CRC
} RVC_tzPGN_PM_ASSOC_CFG;

// Proprietary Message Request
typedef struct RVC_zPGN_PM_REQ
{
    RVC_tePROP_MSG_ID teMsgId;      // Message Identifier
    RVC_tePROP_MSG_ID teReqId;      // Requested Identifier
    uchar8 ucParam1;                // Param 1
    uchar8 ucParam2;                // Param 2
    uint16 uiCrc;                   // CRC
} RVC_tzPGN_PM_REQ;

// Proprietary Message Association Status
typedef struct RVC_zPGN_PM_ASSOC_STS
{
    RVC_tePROP_MSG_ID teMsgId;      // Message Identifier
    RVC_teASSN_TYPE teAssocType;    // Association Type
    uchar8 ucAssocInst;             // Association Instance
    uchar8 ucAssocId;               // Association Identifier
    uint16 uiCrc;                   // CRC
} RVC_tzPGN_PM_ASSOC_STS;

// Proprietary Message Device Mode Config
typedef struct RVC_zPGN_PM_DEV_MD_CFG
{
    RVC_tePROP_MSG_ID teMsgId;      // Message Identifier
    RVC_teCTRL_MODE teDesiredMode;  // Desired Device Mode
    uint16 uiCrc;                   // CRC
} RVC_tzPGN_PM_DEV_MD_CFG;

// Proprietary Message FXCC Configuration Command
typedef struct RVC_zPGN_PM_FXCC_CFG_CMD
{
    RVC_tePROP_MSG_ID teMsgId;      // Message Identifier
    uchar8 ucPwrSvHr;               // Power Save To Hibernate Timeout
    RVC_teIGNITION_CTRL_STS teInvIgnCtrl; // Inverter Ignition Control
    RVC_teGENERIC_STSPAIR teTransMode; // Transfer Mode
    RVC_teGENERIC_STSPAIR teAlarmEnDis; // Audible Alarm Enable/Disable
    RVC_teGENERIC_STSPAIR teOverFltRecEnDis; // Overload Fault Recovery
    uint16 uiCrc;                   // CRC
} RVC_tzPGN_PM_FXCC_CFG_CMD;

// Proprietary Message Device Mode Status
typedef struct RVC_zPGN_PM_DEV_MD_STS
{
    RVC_tePROP_MSG_ID teMsgId;      // Message Identifier
    RVC_teCTRL_MODE teCurrentMode;  // Current Device Mode
    uint16 uiCrc;                   // CRC
} RVC_tzPGN_PM_DEV_MD_STS;

// Proprietary Message FXCC Configuration Status
typedef struct RVC_zPGN_PM_FXCC_CFG_STS
{
    RVC_tePROP_MSG_ID teMsgId;      // Message Identifier
    uchar8 ucPwrSvHr;               // Power Save To Hibernate Time Out
    RVC_teIGNITION_CTRL_STS teInvIgnCtrl; // Inverter Ignition Control
    RVC_teGENERIC_STSPAIR teTransMode; // Transfer Mode
    RVC_teGENERIC_STSPAIR teAlarmEnDis; // Audible Alarm Enable
    RVC_teGENERIC_STSPAIR teOvrFltRecEnDis; // Overload Fault Recovery
    uint16 uiCrc;                   // CRC
} RVC_tzPGN_PM_FXCC_CFG_STS;

// Proprietary Message Software Version Status
typedef struct RVC_zPGN_PM_SW_VER_STS
{
    RVC_tePROP_MSG_ID teMsgId;      // Message ID
    uchar8 ucProcInst;              // Processor Instance
    RVC_teSW_MOD teSwModIdent;      // SW Module ID
    uchar8 ucSwVerMsb;              // SW Version MSB
    uchar8 ucSwVerIsb;              // SW Version ISB
    uchar8 ucSwVerLsb;              // SW Version LSB
    uchar8 ucSwBldNumMsb;           // SW Build Number MSB
    uchar8 ucSwBldNumLsb;           // SW Build Number LSB
    uint16 uiCrc;                   // CRC
} RVC_tzPGN_PM_SW_VER_STS;

// Proprietary Message Remote Procedure Command
typedef struct RVC_zPGN_PM_RPC_CMD
{
    RVC_tePROP_MSG_ID teMsgId;      // Message ID
    uchar8 ucProcedureId;           // Procedure ID
    uint32 ulProcedureArg;          // Procedure Argument
    uint16 uiCrc;                   // CRC
} RVC_tzPGN_PM_RPC_CMD;

// Proprietary Message Inverter Load Share Status
typedef struct RVC_zPGN_PM_INV_LD_SH_STS
{
    RVC_tePROP_MSG_ID teMsgId;      // Message Identifier
    RVC_teAC_SRC_ID teAcSrcId;      // AC Source Identifier
    RVC_teAC_LINE_PM teLine;        // Line
    uchar8 ucInvOutPwr_1;           // Inverter Output Power LSB 4 bits
    uchar8 ucInvOutPwr_2;           // Inverter Output Power ISB 8 bits
    uchar8 ucInvOutPwr_3;           // Inverter Output Power MSB 3 bits
    uchar8 ucCurrent_1;             // Inverter Output Current LSB 5 bits
    uchar8 ucCurrent_2;             // Inverter Output Current MSB 8 bits
    uint16 uiCrc;                   // CRC
} RVC_tzPGN_PM_INV_LD_SH_STS;

// Proprietary Message Inverter Status
typedef struct RVC_zPGN_PM_INV_STS
{
    RVC_tePROP_MSG_ID teMsgId;      // Message Identifier
    RVC_teAC_SRC_ID teAcSrcId;      // AC Source Identifier
    RVC_teINV_MODE teInvMode;       // Inverter Mode
    RVC_teOP_STATE_INV teOpState;   // Operating State
    RVC_teCMD_PAIR teInvEnDis;      // Inverter Enable/Disable
    uint16 uiCrc;                   // CRC
} RVC_tzPGN_PM_INV_STS;

// Proprietary Message Charger Status
typedef struct RVC_zPGN_PM_CHG_STS
{
    RVC_tePROP_MSG_ID teMsgId;      // Message Identifier
    RVC_teDC_SRC_ID teDcInOutAssocId; // DC In/Out Association ID
    RVC_teGENERIC_STSPAIR teBattTempSensorPresent; // Battery Temperature Sensor Present
    RVC_teOP_STATE_CHG teOpState;   // Operating State
    RVC_teCHG_MODE teChgMode;       // Charger Mode
    RVC_teXANTREX_CHG_ALG teChgAlgorithm; // Charger Algorithm
    sint16 siCtrlVolts;             // Charger Control Voltage
    uint16 uiCrc;                   // CRC
} RVC_tzPGN_PM_CHG_STS;

// Proprietary Message Inverter Control Command
typedef struct RVC_zPGN_PM_INV_CTRL_CMD
{
    RVC_tePROP_MSG_ID teMsgId;      // Message ID
    RVC_teAC_SRC_ID teAcSrcId;      // AC Source ID
    RVC_teCMD_PAIR teInvEnDis;      // Inverter Enable/Disable
    RVC_teCMD_PAIR teSellToGridEnDis; // Sell Enable/Disable
    RVC_teCMD_PAIR teForceSell;     // Force Sell On/Off
    uint16 uiCrc;                   // CRC
} RVC_tzPGN_PM_INV_CTRL_CMD;

// Proprietary Message AC Status RMS
typedef struct RVC_zPGN_PM_AC_STS_RMS
{
    RVC_tePROP_MSG_ID teMsgId;      // Message Identifier
    RVC_teAC_SRC_ID teAcSrcId;      // AC Source Identifier
    RVC_teAC_LINE_PM teLine;        // Line
    RVC_teAC_CONNECTION_PM teAcConn; // AC Connection
    RVC_teSRC_QUAL teQuality;       // Quality
    uint16 uiFreq;                  // Frequency
    uint16 uiCrc;                   // CRC
} RVC_tzPGN_PM_AC_STS_RMS;

// Proprietary Message AC Transfer Switch Status
typedef struct RVC_zPGN_PM_AC_XFER_SW_STS
{
    RVC_tePROP_MSG_ID teMsgId;      // Message Identifier
    RVC_teOP_STATE_AC_XFR_SWITCH teOpState; // Operating State
    RVC_teAC_XFER_SW_MODE teXferSwMode; // Transfer Switch Mode
    uint16 uiCrc;                   // CRC
} RVC_tzPGN_PM_AC_XFER_SW_STS;

// Proprietary Message Get Configuration Parameter
typedef struct RVC_zPGN_PM_GET_CONFIG_PARAM
{
    RVC_tePROP_MSG_ID teMsgId;      // Message ID
    RVC_teCFG_PARAM_ID teCfgParamId; // Configuration Parameter Identifier
    uint32 ulParamValue;            // Parameter Value
    uint16 uiCrc;                   // CRC
} RVC_tzPGN_PM_GET_CONFIG_PARAM;

// Proprietary Message Set Configuration Parameter
typedef struct RVC_zPGN_PM_SET_CONFIG_PARAM
{
    RVC_tePROP_MSG_ID teMsgId;      // Message Identifier
    RVC_teCFG_PARAM_ID teCfgParamId; // Configuration Parameter Identifier
    uint32 ulParamValue;            // Parameter Value
    uint16 uiCrc;                   // CRC
} RVC_tzPGN_PM_SET_CONFIG_PARAM;

// Proprietary Message Get Status Parameter
typedef struct RVC_zPGN_PM_GET_STATUS_PARAM
{
    RVC_tePROP_MSG_ID teMsgId;      // Message Identifier
    RVC_teSTS_PARAM_ID teStsParamId; // Status Parameter Identifier
    uint32 ulParamValue;            // Parameter Value
    uint16 uiCrc;                   // CRC
} RVC_tzPGN_PM_GET_STATUS_PARAM;

// PPN Session Command
typedef struct RVC_zPGN_PM_PPN_SESSION_CMD
{
    RVC_tePROP_MSG_ID teMsgId;      // Message ID
    RVC_teSESSION_STS teSessEnDis;  // Session Enable/Disable
} RVC_tzPGN_PM_PPN_SESSION_CMD;

// PPM Session Response
typedef struct RVC_zPGN_PM_PPN_SESSION_RSP
{
    RVC_tePROP_MSG_ID teMsgId;      // Message ID
    RVC_teSESSION_STS teSessEnDis;  // Session Enable/Disable
} RVC_tzPGN_PM_PPN_SESSION_RSP;

// PPN Read Command
typedef struct RVC_zPGN_PM_PPN_READ_CMD
{
    RVC_tePROP_MSG_ID teMsgId;      // Message ID
    RVC_tePROPRIETARY_PARAM_NUM teParamId; // Proprietary Parameter Number
} RVC_tzPGN_PM_PPN_READ_CMD;

// PPN Read Response
typedef struct RVC_zPGN_PM_PPN_READ_RSP
{
    RVC_tePROP_MSG_ID teMsgId;      // Message ID
    RVC_tePROPRIETARY_PARAM_NUM teParamId; // Proprietary Parameter Number
    uchar8 ucValue0;                // Value byte 0
    uchar8 ucValue1;                // Value byte 1
    uchar8 ucValue2;                // Value byte 2
    uchar8 ucValue3;                // Value byte 3
    uchar8 ucValue4;                // Value byte 4
    uchar8 ucValue5;                // Value byte 5
} RVC_tzPGN_PM_PPN_READ_RSP;

// PPN Write Command
typedef struct RVC_zPGN_PM_PPN_WRITE_CMD
{
    RVC_tePROP_MSG_ID teMsgId;      // Message ID
    RVC_tePROPRIETARY_PARAM_NUM teParamId; // Proprietary Parameter Number
    uchar8 ucValue0;                // Value byte 0
    uchar8 ucValue1;                // Value byte 1
    uchar8 ucValue2;                // Value byte 2
    uchar8 ucValue3;                // Value byte 3
    uchar8 ucValue4;                // Value byte 4
    uchar8 ucValue5;                // Value byte 5
} RVC_tzPGN_PM_PPN_WRITE_CMD;

// PPN Write Response
typedef struct RVC_zPGN_PM_PPN_WRITE_RSP
{
    RVC_tePROP_MSG_ID teMsgId;      // Message ID
    RVC_tePROPRIETARY_PARAM_NUM teParamId; // Proprietary Parameter Number
    uchar8 ucValue0;                // Value byte 0
    uchar8 ucValue1;                // Value byte 1
    uchar8 ucValue2;                // Value byte 2
    uchar8 ucValue3;                // Value byte 3
    uchar8 ucValue4;                // Value byte 4
    uchar8 ucValue5;                // Value byte 5
} RVC_tzPGN_PM_PPN_WRITE_RSP;

// PPN Nak Response
typedef struct RVC_zPGN_PM_PPN_NAK_RSP
{
    RVC_tePROP_MSG_ID teMsgId;      // Message ID
    RVC_tePROPRIETARY_PARAM_NUM teParamId; // Proprietary Parameter Number
} RVC_tzPGN_PM_PPN_NAK_RSP;

// Lithionics Proprietary Command
typedef struct RVC_zPGN_PM_LITHIONICS_CMD
{
    RVC_tePROP_MSG_ID teMsgId;      // Message ID
    RVC_teDC_SRC_ID teInst;         // Instance
} RVC_tzPGN_PM_LITHIONICS_CMD;

// Lithionics Proprietary Status
typedef struct RVC_zPGN_PM_LITHIONICS_STS
{
    RVC_tePROP_MSG_ID teMsgId;      // Message ID
    RVC_teDC_SRC_ID teInst;         // Instance
    uchar8 ucMaxRecTemp;            // Maximum Recorded Temperature
    uchar8 ucMinRecTemp;            // Minimum Recorded Temperature
    RVC_teSTS_BIT teHighVoltageState; // High Voltage State
    RVC_teSTS_BIT teChgSrcDet;      // Charge Source Detected
    RVC_teSTS_BIT teNeverDieResState; // NeverDie Reserve State
    RVC_teSTS_BIT teOptoLoopOpen;   // OptoLoop is Open
    RVC_teSTS_BIT teResVoltageRange; // Reserve Voltage Range
    RVC_teSTS_BIT teLowVoltageState; // Low Voltage State
    RVC_teSTS_BIT teBattProtState;  // Battery Protection State
    RVC_teSTS_BIT tePwrOffState;    // Power Off State
    RVC_teSTS_BIT teAuxCntctState;  // AUX Contacts State
    RVC_teSTS_BIT teAuxCntctErr;    // AUX Contacts Error
    RVC_teSTS_BIT tePreChgErr;      // Pre-charge Error
    RVC_teSTS_BIT teCntctFlutter;   // Contactor Flutter
    RVC_teSTS_BIT teAcPwrPres;      // AC Power Present
    RVC_teSTS_BIT teTsmChgPres;     // TSM Charger Present
    RVC_teSTS_BIT teTsmChgErr;      // TSM Charger Error
    RVC_teSTS_BIT teTempInterSensErr; // Temperature Intervention Sensor Error
    RVC_teSTS_BIT teAgsrState;      // AGSR State
    RVC_teSTS_BIT teHotTempState;   // Hot Temperature State
    RVC_teSTS_BIT teColdTempState;  // Cold Temperature State
    RVC_teSTS_BIT teAuxIn1State;    // AUXIN1 State
    RVC_teSTS_BIT teChgDisState;    // Charge Disable State
    RVC_teSTS_BIT teOverCurrState;  // Over-current State
} RVC_tzPGN_PM_LITHIONICS_STS;

// ISO Commanded Address
typedef struct RVC_zPGN_ISO_CMD_ADDR
{
    uint32 ulUniqueNumber;          // Unique Number (ISO Identity Number)
    uint16 uiManufacturerCode;      // Manufacturer Code
    uchar8 ucDeviceInstanceLower;   // Device Instance Lower (ISO ECU Instance)
    uchar8 ucDeviceInstanceUpper;   // Device Instance Upper (ISO Function Instance)
    RVC_teFUNC_CODE teDeviceFunction; // Device Function (ISO Function)
    uchar8 ucRsvBit;                // Reserved Bit
    RVC_teDEVICE_CLASS teDeviceClass; // Device Class
    uchar8 ucSystemInstance;        // System Instance (ISO Device Class Instance)
    RVC_teINDUSTRY_GRP teIndustryGroup; // Industry Group
    uchar8 ucSelfCfgAddr;           // ISO Self Configurable Address
    RVC_teDFLT_SRC_ADDR tePrefSrcAddr; // Preferred Source Address
} RVC_tzPGN_ISO_CMD_ADDR;

// Product Identification
typedef struct RVC_zPGN_EXTRA_PROD_IDENT
{
    uchar8 ucData;                  // Data
} RVC_tzPGN_EXTRA_PROD_IDENT;

typedef struct RVC_zPGN_PROD_IDENT
{
    uint16 uiExtraCount;            // Count of extra structure
    RVC_tzPGN_EXTRA_PROD_IDENT *ptzExtra;   // Extra structures
} RVC_tzPGN_PROD_IDENT;

// XB Reset Command
typedef struct RVC_zPGN_XB_RESET_CMD
{
    uchar8 ucProcessorInst;         // None
    RVC_teXB_RST_CMD teCmd;         // Command
} RVC_tzPGN_XB_RESET_CMD;

// File Download Block
typedef struct RVC_zPGN_EXTRA_FILE_DNLD_BLK
{
    uchar8 ucBlkData;               // Block Data
} RVC_tzPGN_EXTRA_FILE_DNLD_BLK;

typedef struct RVC_zPGN_FILE_DNLD_BLK
{
    uint16 uiBlkNum;                // Block Number
    uint16 uiBlkSz;                 // Block Size
    uint16 uiExtraCount;            // Count of extra structure
    RVC_tzPGN_EXTRA_FILE_DNLD_BLK *ptzExtra;   // Extra structures
} RVC_tzPGN_FILE_DNLD_BLK;

// File Download Information
typedef struct RVC_zPGN_FILE_DNLD_INF
{
    RVC_teFUNC_CODE teDeviceFunction; // Device Function
    uchar8 ucProcessorInst;         // Controller Instance
    RVC_teSW_MOD_ID teSwModId;      // Software Module Identifier
    uint32 ulSwVer;                 // Software Version
    uint32 ulFileSz;                // File Size
    uint16 uiCrc;                   // CRC
    uint16 uiExpectNumBlks;         // Expected Number of Blocks
} RVC_tzPGN_FILE_DNLD_INF;

// Simple Calibration Configuration
typedef struct RVC_zPGN_SIMPLE_CALIB_CFG
{
    uchar8 ucRsvBits;               // Reserved Bits
    uchar8 ucMsgCount;              // Message Count
    uint32 ulStsPgn;                // Status Parameter Group Number
    RVC_teCAL_DATA_POINT teDataPtId; // Data Point Identifier
    uint16 uiScale;                 // Scaling Factor (k)
    uint16 uiOffset;                // Offset Value
    uint32 ulValue;                 // Value
} RVC_tzPGN_SIMPLE_CALIB_CFG;

// File Download Status
typedef struct RVC_zPGN_FILE_DNLD_STS
{
    RVC_teOP_STATE_LDR teOpState;   // Operating State
    RVC_teDL_RESULT_CODE teResultCode; // Result Code
    uchar8 ucProcessorInst;         // Controller Instance
    uint16 uiModelNum;              // Model Number
    uint16 uiHwModRev;              // Hardware Module Revision
    uint16 uiBlkNum;                // Block Number
} RVC_tzPGN_FILE_DNLD_STS;

// Instance Status
typedef struct RVC_zPGN_INST_STS
{
    RVC_teDFLT_SRC_ADDR teDevType;  // Device Type
    uchar8 ucBaseInst;              // Base Instance
    uchar8 ucMaxInst;               // Max Instance
    uint16 uiBaseIntAddr;           // Base Internal Address
    uint16 uiMaxIntAddr;            // Max Internal Address
} RVC_tzPGN_INST_STS;

// Instance Assignment
typedef struct RVC_zPGN_INST_ASSIGN
{
    RVC_teDFLT_SRC_ADDR teDevType;  // Device Type
    uchar8 ucBaseInst;              // Base Instance
    uchar8 ucMaxInst;               // Max Instance
    uint16 uiBaseIntAddr;           // Base Internal Address
    uint16 uiMaxIntAddr;            // Max Internal Address
} RVC_tzPGN_INST_ASSIGN;

// Virtual Terminal
typedef struct RVC_zPGN_VIRTUAL_TERMINAL
{
    uchar8 ucAsciiChar1;            // ASCII Character 1
    uchar8 ucAsciiChar2;            // ASCII Character 2
    uchar8 ucAsciiChar3;            // ASCII Character 3
    uchar8 ucAsciiChar4;            // ASCII Character 4
    uchar8 ucAsciiChar5;            // ASCII Character 5
    uchar8 ucAsciiChar6;            // ASCII Character 6
    uchar8 ucAsciiChar7;            // ASCII Character 7
    uchar8 ucAsciiChar8;            // ASCII Character 8
} RVC_tzPGN_VIRTUAL_TERMINAL;

// General Reset
typedef struct RVC_zPGN_GEN_RST
{
    RVC_teCMD_PAIR teReboot;        // Reboot
    RVC_teCMD_PAIR teClrFlts;       // Clear Faults
    RVC_teCMD_PAIR teRstDflt;       // Reset to Default Settings
    RVC_teCMD_PAIR teRstStats;      // Reset Statistics
    RVC_teCMD_PAIR teTestMd;        // Test Mode
    RVC_teCMD_PAIR teRstOEM;        // Reset to OEM Settings
    RVC_teCMD_PAIR teRstLdr;        // Reset to Loader Mode
} RVC_tzPGN_GEN_RST;

// Product Information Status
typedef struct RVC_zPGN_PROD_INFO_STS
{
    uchar8 ucMsgCount;              // Message Count
    uchar8 aucModelName; //[ 16 ];      // Model Name
    uchar8 aucFga; //[ 12 ];            // Finished Goods Assembly
    uint32 ulPwrLvl;                // Power Level
    sint32 slNomDcLvl;              // Nominal DC Level
    uint32 ulNomAcLvl;              // Nominal AC Level
    uint16 uiNomAcOutFreq;          // Nominal AC Output Frequency
} RVC_tzPGN_PROD_INFO_STS;

// Software Version Status
typedef struct RVC_zPGN_EXTRA_SW_VER_STS
{
    uchar8 ucProcessorInst;         // Controller or Processor Instance
    RVC_teSW_MOD_ID teSwModId;      // Software Module Identifier
    uint32 ulSwVer;                 // Software Version
    uint16 uiSwBldNum;              // Software Build Number
} RVC_tzPGN_EXTRA_SW_VER_STS;

typedef struct RVC_zPGN_SW_VER_STS
{
    uchar8 ucMsgCount;              // Message Count
    uint16 uiExtraCount;            // Count of extra structure
    RVC_tzPGN_EXTRA_SW_VER_STS *ptzExtra;   // Extra structures
} RVC_tzPGN_SW_VER_STS;

// Charger Status 3
typedef struct RVC_zPGN_CHG_STS3
{
    uchar8 ucChgInst;               // Charger Instance
    RVC_teDERATING_STS teDerateSts; // Derating Status
    RVC_teDERATING_REASON teDerateReason; // Derating Reason
} RVC_tzPGN_CHG_STS3;

// Inverter Temperature Status 2
typedef struct RVC_zPGN_INV_TEMP_STS2
{
    uchar8 ucInst;                  // Instance
    uint16 uiCtrlBrdTemp;           // Control Board Temperature
    uint16 uiCapTemp;               // Capacitor Temperature
    uint16 uiAmbTemp;               // Ambient Temperature
} RVC_tzPGN_INV_TEMP_STS2;

// Solar Controller Configuration Command 5
typedef struct RVC_zPGN_SCC_CFG_CMD5
{
    uchar8 ucInst;                  // Instance
    uchar8 ucChgPrio;               // Charger priority
    uchar8 ucExtTempOtSdThresh;     // External temperature sensor high temperature limit
    uchar8 ucExtTempUtSdThresh;     // External temperature sensor low temperature limit
} RVC_tzPGN_SCC_CFG_CMD5;

// Solar Controller Configuration Status 5
typedef struct RVC_zPGN_SCC_CFG_STS5
{
    uchar8 ucInst;                  // Instance
    uchar8 ucChgPrio;               // Charger priority
    uchar8 ucExtTempOtSdThresh;     // External temperature sensor high temperature limit
    uchar8 ucExtTempUtSdThresh;     // External temperature sensor low temperature limi
} RVC_tzPGN_SCC_CFG_STS5;

// DC Source Connection Status
typedef struct RVC_zPGN_DC_SRC_CONN_STS
{
    uchar8 ucDevInst;               // Device Instance
    uchar8 ucDevDSA;                // Device DSA
    RVC_teDC_SRC_FUNC teFunction;   // Function
    uchar8 ucPrimaryDcInst;         // Primary DC instance
    uchar8 ucSecondDcInst;          // Secondary DC instance
} RVC_tzPGN_DC_SRC_CONN_STS;

// Device State Synchronization
typedef struct RVC_zPGN_DEVICE_STATE_SYNC
{
    uint16 uiManufacturerCode;      // Manufacturer Code
    uchar8 ucPayloadID;             // Payload Identifier
    uchar8 ucPayload1;              // Payload 1
    uchar8 ucPayload2;              // Payload 2
    uchar8 ucPayload3;              // Payload 3
    uchar8 ucPayload4;              // Payload 4
    uchar8 ucPayload5;              // Payload 5
} RVC_tzPGN_DEVICE_STATE_SYNC;

// DC Source Configuration Command 3
typedef struct RVC_zPGN_DC_SRC_CFG_CMD3
{
    uchar8 ucDevInst;               // Device Instance
    uchar8 ucDevDSA;                // Device DSA
    RVC_teDC_SRC_FUNC teFunction;   // Function
    uchar8 ucPrimaryDcInst;         // Primary DC instance
    uchar8 ucSecondDcInst;          // Secondary DC instance
} RVC_tzPGN_DC_SRC_CFG_CMD3;

// Battery Summary
typedef struct RVC_zPGN_BATT_SUMM
{
    RVC_teDC_SRC_ID teBattInst;     // Battery Instance
    RVC_teDC_SRC_ID teDcInst;       // DC Instance
    uchar8 ucSeriesString;          // Series String
    RVC_teMOD_COUNT teModuleCnt;    // Module Count
    uchar8 ucCellsPerModule;        // Cells per Module
    RVC_teV_STS teVoltSts;          // Voltage Status
    RVC_teTEMP_STS teTempSts;       // Temperature Status
} RVC_tzPGN_BATT_SUMM;

// Solar Controller Configuration Command 4
typedef struct RVC_zPGN_SCC_CFG_CMD4
{
    uchar8 ucInst;                  // Instance
    uint16 uiBattOvRetV;            // Battery high voltage limit return voltage
    uint16 uiBattUvRetV;            // Battery low voltage llimit return voltage
    uchar8 ucBattUvDly;             // Battery low voltage time delay
    uchar8 ucAbsorpTime;            // Absorption duration
    uchar8 ucTempCompConst;         // Temperature compensation factor
} RVC_tzPGN_SCC_CFG_CMD4;

// Solar Controller Configuration Status 4
typedef struct RVC_zPGN_SCC_CFG_STS4
{
    uchar8 ucInst;                  // Instance
    uint16 uiBattOvRetV;            // Battery high voltage limit return voltage
    uint16 uiBattUvRetV;            // Battery low voltage limit return voltage
    uchar8 ucBattUvDly;             // Battery low voltage time delay
    uchar8 ucAbsorpTime;            // Absorption duration
    uchar8 ucTempCompConst;         // Temperature compensation factor
} RVC_tzPGN_SCC_CFG_STS4;

// Solar Controller Configuration Command 3
typedef struct RVC_zPGN_SCC_CFG_CMD3
{
    uchar8 ucInst;                  // Inststance
    uint16 uiBattUvWarnThresh;      // Under-voltage warning voltage
    uint16 uiBattOvSdThresh;        // Battery high voltage limit voltage
    uint16 uiBattUvSdThresh;        // Battery low voltage limit voltage
} RVC_tzPGN_SCC_CFG_CMD3;

// Solar Controller Configuration Status 3
typedef struct RVC_zPGN_SCC_CFG_STS3
{
    uchar8 ucInst;                  // Instance
    uint16 uiBattUvWarnThresh;      // Under-voltage warning voltage
    uint16 uiBattOvSdThresh;        // Battery high voltage limit voltage
    uint16 uiBattUvSdThresh;        // Battery low voltage limit voltage
} RVC_tzPGN_SCC_CFG_STS3;

// Solar Controller Configuration Command 2
typedef struct RVC_zPGN_SCC_CFG_CMD2
{
    uchar8 ucInst;                  // Instance
    uint16 uiBulkV;                 // Bulk-absorption voltage
    uint16 uiFloatV;                // Float voltage
    uint16 uiChgRetV;               // Charge return voltage
} RVC_tzPGN_SCC_CFG_CMD2;

// Solar Controller Configuration Status 2
typedef struct RVC_zPGN_SCC_CFG_STS2
{
    uchar8 ucInst;                  // Instance
    uint16 uiBulkV;                 // Bulk absorption voltage
    uint16 uiFloatV;                // Float voltage
    uint16 uiChgRetV;               // Charge return voltage
} RVC_tzPGN_SCC_CFG_STS2;

// Solar Controller Solar Array Status
typedef struct RVC_zPGN_SCC_ARRAY_STS
{
    uchar8 ucInst;                  // Instance
    uint16 uiArrayV;                // Solar array measured voltage
    uint16 uiArrayI;                // Solar array measured input current
} RVC_tzPGN_SCC_ARRAY_STS;

// Solar Controller Battery Status
typedef struct RVC_zPGN_SCC_BATT_STS
{
    uchar8 ucInst;                  // Instance
    uchar8 ucDcSrcInst;             // DC Source Instance
    uchar8 ucChgPriority;           // Charger Priority
    uint16 uiBattVolts;             // Measured voltage
    uint16 uiBattCurrent;           // Measured current
    uchar8 ucBattTemp;              // Measured temperature
} RVC_tzPGN_SCC_BATT_STS;

// Solar Controller Status 6
typedef struct RVC_zPGN_SCC_STS6
{
    uchar8 ucInst;                  // Instance
    uint16 uiOperatingDays;         // Total number of operating days
    uint16 uiIntTemp;               // Solar charge controller measured temperature
} RVC_tzPGN_SCC_STS6;

// Solar Controller Status 5
typedef struct RVC_zPGN_SCC_STS5
{
    uchar8 ucInst;                  // Instance
    uint16 uiWeekAh;                // Last 7 days amp-hours to battery
    uint32 ulCumPwrGen;             // Cumulative power generation
} RVC_tzPGN_SCC_STS5;

// Solar Controller Status 4
typedef struct RVC_zPGN_SCC_STS4
{
    uchar8 ucInst;                  // Instance
    uint16 uiTodayAh;               // Today amp-hours to battery
    uint16 uiYesterdayAh;           // Yesterday amp-hours to battery
    uint16 uiBeforeYesterdayAh;     // Day before yesterday's amp-hours to battery
} RVC_tzPGN_SCC_STS4;

// Solar Controller Status 3
typedef struct RVC_zPGN_SCC_STS3
{
    uchar8 ucInst;                  // Instance
    uint16 uiRatedPvV;              // Rated solar input voltage
    uint16 uiRatedPvI;              // Rated solar input current
    uint16 uiRatedPvOverPwr;        // Rated solar over-power
} RVC_tzPGN_SCC_STS3;

// Solar Controller Status 2
typedef struct RVC_zPGN_SCC_STS2
{
    uchar8 ucInst;                  // Instance
    uint16 uiRatedBattV;            // Rated battery voltage
    uint16 uiRatedBattI;            // Rated battery current
} RVC_tzPGN_SCC_STS2;

// Battery Command
typedef struct RVC_zPGN_BATT_CMD
{
    RVC_teDC_SRC_ID teBattInst;     // Battery Instance
    RVC_teGENERIC_STSPAIR teDesiredLoadOnOffSts; // Desired Load On/Off status
    RVC_teGENERIC_STSPAIR teDesiredChgOnOffSts; // Desired Charge On/Off status
    RVC_teGENERIC_STSPAIR teClrHistory; // Clear History
    RVC_teGENERIC_STSPAIR teReserved; // Reserved
    uchar8 ucRtnModuleCellDetail;   // Return Modules Cell Details
    RVC_teDC_SRC_ID teDcInst;       // DC Instance
} RVC_tzPGN_BATT_CMD;

// Battery Status 11
typedef struct RVC_zPGN_BATT_STS11
{
    RVC_teDC_SRC_ID teBattInst;     // Battery Instance
    RVC_teDC_SRC_ID teDcInst;       // DC Instance
    RVC_teGENERIC_STSPAIR teDischgOnOffSts; // Discharge On/Off status
    RVC_teGENERIC_STSPAIR teChgOnOffSts; // Charge On/Off status
    RVC_teGENERIC_STSPAIR teChgDetect; // Charge Detected
    RVC_teGENERIC_STSPAIR teReservSts; // Reserve Status
    uint16 uiNomFullCap;            // Nominal Full Capacity
    uint16 uiDcPwr;                 // DC Power
} RVC_tzPGN_BATT_STS11;

// Battery Status 6
typedef struct RVC_zPGN_BATT_STS6
{
    RVC_teDC_SRC_ID teBattInst;     // Battery Instance
    RVC_teDC_SRC_ID teDcInst;       // DC Instance
    RVC_teGENERIC_STSPAIR teHiVoltLimitSts; // High Voltage Limit Status
    RVC_teDISCONNECT_STS teHiVoltDisconSts; // High Voltage Disconnect Status
    RVC_teGENERIC_STSPAIR teLowVoltLimitSts; // Low Voltage Limit Status
    RVC_teDISCONNECT_STS teLowVoltDisconSts; // Low Voltage Disconnect Status
    RVC_teGENERIC_STSPAIR teLowSocLimitSts; // Low State of Charge Limit Status
    RVC_teDISCONNECT_STS teLowSocDisconSts; // Low State of Charge Disconnect Status
    RVC_teGENERIC_STSPAIR teLowDcSrcTempLimitSts; // Low DC Source Temperature Limit Status
    RVC_teDISCONNECT_STS teLowDcSrcTempDisconSts; // Low DC Source Temperature Disconnect Status
    RVC_teGENERIC_STSPAIR teHiDcSrcTempLimitSts; // High DC Source Temperature Limit Status
    RVC_teDISCONNECT_STS teHiDcSrcTempDisconSts; // High DC Source Temperature Disconnect Status
    RVC_teGENERIC_STSPAIR teHiAmpDcSrcLimitSts; // High Current DC Source Limit Status
    RVC_teDISCONNECT_STS teHiAmpDcSrcDisconSts; // High Current DC Source Disconnect Status
} RVC_tzPGN_BATT_STS6;

// Battery Status 4
typedef struct RVC_zPGN_BATT_STS4
{
    RVC_teDC_SRC_ID teBattInst;     // Battery Instance
    RVC_teDC_SRC_ID teDcInst;       // DC Instance
    RVC_teRVC_DESIRED_CHG_STATE teDesChgState; // Desired Charge State
    uint16 uiDesDcV;                // Desired DC Voltage
    uint16 uiDesDcI;                // Desired DC Current
    RVC_teRVC_BATT_TYPE teBattType; // Battery Type
} RVC_tzPGN_BATT_STS4;

// Battery Status 3
typedef struct RVC_zPGN_BATT_STS3
{
    RVC_teDC_SRC_ID teBattInst;     // Battery Instance
    RVC_teDC_SRC_ID teDcInst;       // DC Instance
    uchar8 ucSoh;                   // State of Health
    uint16 uiCapRemain;             // Capacity Remaining
    uchar8 ucRelCap;                // Relative Capacity
    uint16 uiAcRmsRipple;           // AC RMS Ripple
} RVC_tzPGN_BATT_STS3;

// Battery Status 2
typedef struct RVC_zPGN_BATT_STS2
{
    RVC_teDC_SRC_ID teBattInst;     // Battery Instance
    RVC_teDC_SRC_ID teDcInst;       // DC Instance
    uint16 uiSrcTemp;               // Source Temperature
    uchar8 ucSoc;                   // State of Charge
    uint16 uiTimeRemain;            // Time Remaining
    RVC_teTIME_REMAIN_INTERP teTimeRemainInterp; // Time Remain Interpretion
} RVC_tzPGN_BATT_STS2;

// Battery Status 1
typedef struct RVC_zPGN_BATT_STS1
{
    RVC_teDC_SRC_ID teBattInst;     // Battery Instance
    RVC_teDC_SRC_ID teDcInst;       // DC Instance
    uint16 uiDcV;                   // DC Voltage
    uint32 ulDcI;                   // DC Current
} RVC_tzPGN_BATT_STS1;

// Inverter Configuration Command 4
typedef struct RVC_zPGN_INV_CFG_CMD4
{
    uchar8 ucInst;                  // Instance
    uint16 uiOutACVolt;             // Output AC Voltage
    uchar8 ucOutFreq;               // Output Frequency
    uint16 uiACOutPwrLim;           // AC Output Power Limit
    uint16 uiACOutPwrTimeLim;       // AC Output Power Time Limit
} RVC_tzPGN_INV_CFG_CMD4;

// Inverter Configuration Status 4
typedef struct RVC_zPGN_INV_CFG_STS4
{
    uchar8 ucInst;                  // Instance
    uint16 uiOutACVolt;             // Output AC Voltage
    uchar8 ucOutFreq;               // Output Frequency
    uint16 uiACOutPwrLim;           // AC Output Power Limit
    uint16 uiACOutPwrTimeLim;       // AC Output Power Time Limit
} RVC_tzPGN_INV_CFG_STS4;

// Alarm Command
typedef struct RVC_zPGN_ALARM_CMD
{
    uchar8 ucInst;                  // Instance
    RVC_teDFLT_SRC_ADDR teDevType;  // DSA
    RVC_teCMD_PAIR teReadyAlarm;    // Ready alarm
    RVC_teCMD_PAIR teAckAlarm;      // Acknowledge alarm
    RVC_teCMD_PAIR teAutoRstEn;     // Automatic reset enabled
    uchar8 ucDevInst;               // Device instance
} RVC_tzPGN_ALARM_CMD;

// Alarm Status
typedef struct RVC_zPGN_ALARM_STS
{
    uchar8 ucInst;                  // Instance
    RVC_teDFLT_SRC_ADDR teDevType;  // DSA
    RVC_teGENERIC_STSPAIR teAlarmTrig; // Alarm triggered
    RVC_teGENERIC_STSPAIR teAlarmRdy; // Alarm ready
    RVC_teGENERIC_STSPAIR teAlarmAckd; // Alarm is acknowledged
    RVC_teGENERIC_STSPAIR teAlarmRstEn; // Alarm reset enabled
    uint16 uiElapsedTime;           // Elapsed time
    uchar8 ucDevInst;               // Device instance
    RVC_teGENERIC_STSPAIR teFtMonReq; // Full-time monitoring required
    RVC_teGENERIC_STSPAIR teAlarmForLog; // Alarm for logging
    RVC_teGENERIC_STSPAIR teAlarmForUser; // Alarm for user notification
} RVC_tzPGN_ALARM_STS;

// Charger Status 2
typedef struct RVC_zPGN_CHG_STS2
{
    uchar8 ucChgInst;               // Charger Instance
    uchar8 ucDcSrcInst;             // DC Source Instance
    RVC_teRVC_DEVICE_PRIORITY teChgPrio; // Charger Priority
    uint16 uiChgV;                  // Charging Voltage
    uint16 uiChgI;                  // Charging Current
    uchar8 ucChgTemp;               // Charger temperature
} RVC_tzPGN_CHG_STS2;

// DC SOURCE COMMAND
typedef struct RVC_zPGN_DC_SRC_CMD
{
    RVC_teDC_SRC_ID teInst;         // Instance
    RVC_teGENERIC_STSPAIR teDesPwrOnOffSts; // Desired Power On/Off Status
    RVC_teGENERIC_STSPAIR teDesChgOnOffSts; // Desired Charge On/Off Status
} RVC_tzPGN_DC_SRC_CMD;

// DC SOURCE STATUS 11
typedef struct RVC_zPGN_DC_SRC_STS11
{
    RVC_teDC_SRC_ID teInst;         // Instance
    RVC_teRVC_DEVICE_PRIORITY teDevPri; // Device Priority
    RVC_teGENERIC_STSPAIR teDischgOnOffSts; // Discharge On/Off status
    RVC_teGENERIC_STSPAIR teChgOnOffSts; // Charge On/Off status
    RVC_teGENERIC_STSPAIR teChgDetect; // Charge Detected
    RVC_teGENERIC_STSPAIR teReservSts; // Reserve Status
    uint16 uiNomFullCap;            // Nominal Full Capacity
    uint16 uiDcPwr;                 // DC Power
} RVC_tzPGN_DC_SRC_STS11;

// Solar Equalization Configuration Command
typedef struct RVC_zPGN_SCC_EQLZ_CFG_CMD
{
    uchar8 ucInst;                  // Instance
    uint16 uiEqlzV;                 // Equalization voltage
    uint16 uiEqlzTime;              // Equalization time
    uchar8 ucEqlzIntvl;             // Equalization interval
} RVC_tzPGN_SCC_EQLZ_CFG_CMD;

// Solar Equalization Configuration Status
typedef struct RVC_zPGN_SCC_EQLZ_CFG_STS
{
    uchar8 ucInst;                  // Instance
    uint16 uiEqlzV;                 // Equalization voltage
    uint16 uiEqlzTime;              // Equalization time
    uchar8 ucEqlzIntvl;             // Equalization interval
} RVC_tzPGN_SCC_EQLZ_CFG_STS;

// Solar Equalization Status
typedef struct RVC_zPGN_SCC_EQLZ_STS
{
    uchar8 ucInst;                  // Instance
    uint16 uiTimeRemain;            // Time remaining
    RVC_teGENERIC_STSPAIR tePreChgSts; // Pre-charging status
    uchar8 ucTimeSinceEqlz;         // Time since last equalization
} RVC_tzPGN_SCC_EQLZ_STS;

// Solar Controller Configuration Command
typedef struct RVC_zPGN_SCC_CFG_CMD
{
    uchar8 ucInst;                  // Instance
    RVC_teRVC_CHG_ALG teChgAlg;     // Charging algorithm
    RVC_teRVC_CHG_MODE teChgMode;   // Charger mode
    RVC_teCMD_PAIR teBattSensPres;  // Battery sensor present
    RVC_teCMD_PAIR teLinkageMode;   // Linkage mode
    RVC_teRVC_BATT_TYPE teBattType; // Battery type
    uint16 uiBattBankSz;            // Battery bank size
    uchar8 ucMaxChgI;               // Maximum charging current
} RVC_tzPGN_SCC_CFG_CMD;

// Solar Controller Command
typedef struct RVC_zPGN_SCC_CMD
{
    uchar8 ucInst;                  // Instance
    RVC_teRVC_CHG_CMD teSts;        // Solar charge controller status
    RVC_teCMD_PAIR teEnableOnPwrUp; // Default state on power-up
    RVC_teCMD_PAIR teClrHist;       // Clear history
    RVC_teRVC_FORCE_CHG teForceChg; // Force charge
} RVC_tzPGN_SCC_CMD;

// Solar Controller Configuration Status
typedef struct RVC_zPGN_SCC_CFG_STS
{
    uchar8 ucInst;                  // Instance
    RVC_teRVC_CHG_ALG teChgAlg;     // Charging algorithm
    RVC_teRVC_CHG_MODE teChgMode;   // Controller mode
    RVC_teGENERIC_STSPAIR teBattSensPres; // Battery sensor present
    RVC_teGENERIC_STSPAIR teLinkMode; // Linkage mode
    RVC_teRVC_BATT_TYPE teBattType; // Battery type
    uint16 uiBattBankSz;            // Battery bank size
    uchar8 ucMaxChgI;               // Maximum charging current
} RVC_tzPGN_SCC_CFG_STS;

// Solar Controller Status
typedef struct RVC_zPGN_SCC_STS
{
    uchar8 ucInst;                  // Instance
    uint16 uiChgV;                  // Charge voltage
    uint16 uiChgI;                  // Charge current
    uchar8 ucChgIPer;               // Charge current percent of maximum
    RVC_teRVC_DESIRED_CHG_STATE teOpState; // Operating state
    RVC_teGENERIC_STSPAIR tePwrUpEnDis; // Power-up state
    RVC_teGENERIC_STSPAIR teClrHist; // Clear history
    RVC_teRVC_FORCE_CHG teForceChg; // Force charge
} RVC_tzPGN_SCC_STS;

// Inverter Temperature Status
typedef struct RVC_zPGN_INV_TEMP_STS
{
    uchar8 ucInst;                  // Instance
    uint16 uiFet1Temp;              // FET1 Temperature
    uint16 uiXfmrTemp;              // Transformer Temperature
    uint16 uiFet2Temp;              // FET2 Temperature
} RVC_tzPGN_INV_TEMP_STS;

// Charger Configuration Command 4
typedef struct RVC_zPGN_CHG_CFG_CMD4
{
    uchar8 ucInst;                  // Instance
    uint16 uiBulkTime;              // Bulk Time
    uint16 uiAbsorpTime;            // Absorption Time
    uint16 uiFloatTime;             // Float Time
} RVC_tzPGN_CHG_CFG_CMD4;

// Charger Configuration Status 4
typedef struct RVC_zPGN_CHG_CFG_STS4
{
    uchar8 ucInst;                  // Instance
    uint16 uiBulkTime;              // Bulk Time
    uint16 uiAbsorpTime;            // Absorption Time
    uint16 uiFloatTime;             // Float Time
} RVC_tzPGN_CHG_CFG_STS4;

// DC Source Status 6
typedef struct RVC_zPGN_DC_SRC_STS6
{
    RVC_teDC_SRC_ID teDcInst;       // DC Instance
    RVC_teRVC_DEVICE_PRIORITY teDevPri; // Device Priority
    RVC_teGENERIC_STSPAIR teHiVoltLimitSts; // High Voltage Limit Status
    RVC_teDISCONNECT_STS teHiVoltDisconSts; // High Voltage Disconnect Status
    RVC_teGENERIC_STSPAIR teLowVoltLimitSts; // Low Voltage Limit Status
    RVC_teDISCONNECT_STS teLowVoltDisconSts; // Low Voltage Disconnect Status
    RVC_teGENERIC_STSPAIR teLowSocLimitSts; // Low State of Charge Limit Status
    RVC_teDISCONNECT_STS teLowSocDisconSts; // Low State of Charge Disconnect Status
    RVC_teGENERIC_STSPAIR teLowDcSrcTempLimitSts; // Low DC Source Temperature Limit Status
    RVC_teDISCONNECT_STS teLowDcSrcTempDisconSts; // Low DC Source Temperature Disconnect Status
    RVC_teGENERIC_STSPAIR teHiDcSrcTempLimitSts; // High DC Source Temperature Limit Status
    RVC_teDISCONNECT_STS teHiDcSrcTempDisconSts; // High DC Source Temperature Disconnect Status
    RVC_teGENERIC_STSPAIR teHiAmpDcSrcLimitSts; // High Current DC Source Limit Status
    RVC_teDISCONNECT_STS teHiAmpDcSrcDisconSts; // High Current DC Source Disconnect Status
} RVC_tzPGN_DC_SRC_STS6;

// DC Source Status 4
typedef struct RVC_zPGN_DC_SRC_STS4
{
    RVC_teDC_SRC_ID teInst;         // Instance
    RVC_teRVC_DEVICE_PRIORITY teDevPri; // Device Priority
    RVC_teRVC_DESIRED_CHG_STATE teDesChgState; // Desired Charge State
    uint16 uiDesDcV;                // Desired DC Voltage
    uint16 uiDesDcI;                // Desired DC Current
    RVC_teRVC_BATT_TYPE teBattType; // Battery Type
} RVC_tzPGN_DC_SRC_STS4;

// Diagnostic Message 1
typedef struct RVC_zPGN_EXTRA_DIAG_MSG1
{
    uchar8 ucSpnMsb;                // Suspect Parameter Number MSB
    uchar8 ucSpnIsb;                // Suspect Parameter Number ISB
    RVC_teSAEJ1939FMI teFmi;        // Failure Mode Identifier
    uchar8 ucSpnLsb;                // Suspect Parameter Number LSB
    uchar8 ucOccurrenceCnt;         // Occurrence Count
    uchar8 ucProdIdExt;             // Default Source Address Extension
} RVC_tzPGN_EXTRA_DIAG_MSG1;

typedef struct RVC_zPGN_DIAG_MSG1
{
    RVC_teGENERIC_STSPAIR teOpStsProdOn; // Operating Status - Product On
    RVC_teGENERIC_STSPAIR teOpStsProdActive; // Operating Status - Product Active
    RVC_teGENERIC_STSPAIR teOpStsYel; // Yellow Lamp status
    RVC_teGENERIC_STSPAIR teOpStsRed; // Red Lamp status
    RVC_teDFLT_SRC_ADDR teProdId;   // Product Identifier - Default Source Address
    uint16 uiExtraCount;            // Count of extra structure
    RVC_tzPGN_EXTRA_DIAG_MSG1 *ptzExtra;   // Extra structures
} RVC_tzPGN_DIAG_MSG1;

// Charger Configuration Command 3
typedef struct RVC_zPGN_CHG_CFG_CMD3
{
    uchar8 ucInst;                  // Instance
    uint16 uiBulkV;                 // Bulk Voltage
    uint16 uiAbsorpV;               // Absorption Voltage
    uint16 uiFloatV;                // Float Voltage
    uchar8 ucTempCompConst;         // Temperature Compensation Constant
} RVC_tzPGN_CHG_CFG_CMD3;

// Charger Configuration Status 3
typedef struct RVC_zPGN_CHG_CFG_STS3
{
    uchar8 ucInst;                  // Instance
    uint16 uiBulkV;                 // Bulk Voltage
    uint16 uiAbsorpV;               // Absorption Voltage
    uint16 uiFloatV;                // Float Voltage
    uchar8 ucTempCompConst;         // Temperature Compensation Constant
} RVC_tzPGN_CHG_CFG_STS3;

// Inverter Configuration Command 3
typedef struct RVC_zPGN_INV_CFG_CMD3
{
    uchar8 ucInst;                  // Instance
    uint16 uiDcSrcShtDwnDly;        // DC Source Shutdown Delay
    RVC_teRVC_STACK_MODE teStackMd; // Stack Mode
    uint16 uiDcUvSdRecLvl;          // DC Under Voltage Shutdown Recovery Level
    uint16 uiGenSupportEngI;        // Generator Support Engage Current
} RVC_tzPGN_INV_CFG_CMD3;

// Inverter Configuration Status 3
typedef struct RVC_zPGN_INV_CFG_STS3
{
    uchar8 ucInst;                  // Instance
    uint16 uiDcSrcShtDwnDly;        // DC Source shutdown delay
    RVC_teRVC_STACK_MODE teStackMd; // Stack Mode
    uint16 uiDcUvSdRecLvl;          // DC Under Voltage Shutdown Recovery Level
    uint16 uiGenSupportEngI;        // Generator Support Engage Current
} RVC_tzPGN_INV_CFG_STS3;

// Inverter_DC_Status
typedef struct RVC_zPGN_INV_DC_STS
{
    uchar8 ucInst;                  // Instance
    uint16 uiDcVolt;                // DC_Voltage
    uint16 uiDcAmps;                // DC_Amperage
} RVC_tzPGN_INV_DC_STS;

// Charger AC Fault Configuration Command 2
typedef struct RVC_zPGN_CHG_AC_FLT_CFG_CMD2
{
    uchar8 ucInst;                  // Instance
    RVC_teAC_LINE_NUM teLine;       // Line
    RVC_teGENERIC_STSPAIR teInOut;  // Input/Output
    uchar8 ucHiFreqLimit;           // High Frequency Limit
    uchar8 ucLoFreqLimit;           // Low Frequency Limit
} RVC_tzPGN_CHG_AC_FLT_CFG_CMD2;

// Charger AC Fault Configuration Command 1
typedef struct RVC_zPGN_CHG_AC_FLT_CFG_CMD1
{
    uchar8 ucInst;                  // Instance
    RVC_teAC_LINE_NUM teLine;       // Line
    RVC_teGENERIC_STSPAIR teInOut;  // Input/Output
    uchar8 ucExtremeLoV;            // Extreme Low Voltage Level
    uchar8 ucLoV;                   // Low Voltage Level
    uchar8 ucHiV;                   // High Voltage Level
    uchar8 ucExtremeHiV;            // Extreme High Voltage Level
    uchar8 ucQualTime;              // Qualification Time
    RVC_teGENERIC_STSPAIR teBypassMode; // Bypass Mode
} RVC_tzPGN_CHG_AC_FLT_CFG_CMD1;

// Charger AC Fault Configuration Status 2
typedef struct RVC_zPGN_CHG_AC_FLT_CFG_STS2
{
    uchar8 ucInst;                  // Instance
    RVC_teAC_LINE_NUM teLine;       // Line
    RVC_teGENERIC_STSPAIR teInOut;  // Input/Output
    uchar8 ucHiFreqLimit;           // High Frequency Limit
    uchar8 ucLoFreqLimit;           // Low Frequency Limit
} RVC_tzPGN_CHG_AC_FLT_CFG_STS2;

// Charger AC Fault Configuration Status 1
typedef struct RVC_zPGN_CHG_AC_FLT_CFG_STS1
{
    uchar8 ucInst;                  // Instance
    RVC_teAC_LINE_NUM teLine;       // Line
    RVC_teGENERIC_STSPAIR teInOut;  // Input/Output
    uchar8 ucExtremeLoV;            // Extreme Low Voltage Level
    uchar8 ucLoV;                   // Low Voltage Level
    uchar8 ucHiV;                   // High Voltage Level
    uchar8 ucExtremeHiV;            // Extreme High Voltage Level
    uchar8 ucQualTime;              // Qualification Time
    RVC_teGENERIC_STSPAIR teBypassMode; // Bypass Mode
} RVC_tzPGN_CHG_AC_FLT_CFG_STS1;

// Charger AC Status 4
typedef struct RVC_zPGN_CHG_AC_STS4
{
    uchar8 ucInst;                  // Instance
    RVC_teAC_LINE_NUM teLine;       // Line
    RVC_teGENERIC_STSPAIR teInOut;  // Input/Output
    RVC_teRVCV_FLT teVFlt;          // Voltage Fault
    RVC_teGENERIC_STSPAIR teFltSurgeProt; // Fault - Surge Protection
    RVC_teGENERIC_STSPAIR teFltHiFreq; // Fault - High Frequency
    RVC_teGENERIC_STSPAIR teFltLoFreq; // Fault - Low Frequency
    RVC_teGENERIC_STSPAIR teBypassModeAct; // Bypass Mode Active
    RVC_teRVC_QUALIFICATION_STS teQualifStat; // Qualification Status
} RVC_tzPGN_CHG_AC_STS4;

// Inverter AC Fault Configuration Command 1
typedef struct RVC_zPGN_INV_AC_FLT_CFG_CMD1
{
    uchar8 ucInst;                  // Instance
    uchar8 ucExtremeLoV;            // Extreme Low Voltage Level
    uchar8 ucLoV;                   // Low Voltage Level
    uchar8 ucHiV;                   // High Voltage Level
    uchar8 ucExtremeHiV;            // Extreme High Voltage Level
    uchar8 ucQualTime;              // Qualification Time
    RVC_teGENERIC_STSPAIR teBypassMode; // Bypass Mode
    uchar8 ucRsvBits;               // Reserved Bits
} RVC_tzPGN_INV_AC_FLT_CFG_CMD1;

// Inverter AC Fault Configuration Status 1
typedef struct RVC_zPGN_INV_AC_FLT_CFG_STS1
{
    uchar8 ucInst;                  // Instance
    uchar8 ucExtremeLoV;            // Extreme Low Voltage Level
    uchar8 ucLoV;                   // Low Voltage Level
    uchar8 ucHiV;                   // High Voltage Level
    uchar8 ucExtremeHiV;            // Extreme High Voltage Level
    uchar8 ucQualTime;              // Qualification Time
    RVC_teGENERIC_STSPAIR teBypassMode; // Bypass Mode
    uchar8 ucRsvBits;               // Reserved Bits
} RVC_tzPGN_INV_AC_FLT_CFG_STS1;

// Inverter AC Status 4
typedef struct RVC_zPGN_INV_AC_STS4
{
    uchar8 ucInst;                  // Instance
    RVC_teAC_LINE_NUM teLine;       // Line
    RVC_teGENERIC_STSPAIR teInOut;  // Input/Output
    RVC_teRVCV_FLT teVFlt;          // Voltage Fault
    RVC_teGENERIC_STSPAIR teFltSurgeProt; // Fault - Surge Protection
    RVC_teGENERIC_STSPAIR teFltHiFreq; // Fault - High Frequency
    RVC_teGENERIC_STSPAIR teFltLoFreq; // Fault - Low Frequency
    RVC_teGENERIC_STSPAIR teBypassModeAct; // Bypass Mode Active
    RVC_teRVC_QUALIFICATION_STS teQualifStat; // Qualification Status
} RVC_tzPGN_INV_AC_STS4;

// Charger Configuration Command 2
typedef struct RVC_zPGN_CHG_CFG_CMD2
{
    uchar8 ucInst;                  // Instance
    uchar8 ucMaxChgIPer;            // Maximum Charging Current Percent
    uchar8 ucChgRateLimitPerBankSz; // Charge Rate Limit as a Percent of Bank Size
    uchar8 ucShoreBrkSz;            // Shore Breaker Size
    uchar8 ucDefBattTemp;           // Default Battery Temperature
    uint16 uiReChgVolt;             // Recharge Voltage
} RVC_tzPGN_CHG_CFG_CMD2;

// Charger Configuration Status 2
typedef struct RVC_zPGN_CHG_CFG_STS2
{
    uchar8 ucInst;                  // Instance
    uchar8 ucMaxChgIPer;            // Maximum Charging Current Percent
    uchar8 ucChgRateLimitPerBankSz; // Charge Rate Limit as a Percent of Bank Size
    uchar8 ucShoreBrkSz;            // Shore Breaker Size
    uchar8 ucDefBattTemp;           // Default Battery Temperature
    uint16 uiReChgVolt;             // Recharge Voltage
} RVC_tzPGN_CHG_CFG_STS2;

// Charger Equalization Configuration Command
typedef struct RVC_zPGN_CHG_EQLZ_CFG_CMD
{
    uchar8 ucInst;                  // Instance
    uint16 uiEqlzV;                 // Equalization Voltage
    uint16 uiEqlzTime;              // Equalization Time
} RVC_tzPGN_CHG_EQLZ_CFG_CMD;

// Charger Equalization Configuration Status
typedef struct RVC_zPGN_CHG_EQLZ_CFG_STS
{
    uchar8 ucInst;                  // Instance
    uint16 uiEqlzV;                 // Equalization Voltage
    uint16 uiEqlzTime;              // Equalization Time
} RVC_tzPGN_CHG_EQLZ_CFG_STS;

// Charger Equalization Status
typedef struct RVC_zPGN_CHG_EQLZ_STS
{
    uchar8 ucInst;                  // Instance
    uint16 uiTimeRem;               // Time Remaning
    RVC_teRVC_PRE_CHG_STS tePreChgSts; // Pre-charging status
} RVC_tzPGN_CHG_EQLZ_STS;

// AC Fault Control Configuration Command 2
typedef struct RVC_zPGN_AC_FLT_CTRL_CFG_CMD2
{
    uchar8 ucInst;                  // Instance
    RVC_teGENERIC_STSPAIR teLine;   // Line
    RVC_teGENERIC_STSPAIR teInOut;  // Input/Output
    uchar8 ucHiFreqLimit;           // High Frequency Limit
    uchar8 ucLoFreqLimit;           // Low Frequency Limit
} RVC_tzPGN_AC_FLT_CTRL_CFG_CMD2;

// AC Fault Control Configuration Command 1
typedef struct RVC_zPGN_AC_FLT_CTRL_CFG_CMD1
{
    uchar8 ucInst;                  // Instance
    RVC_teGENERIC_STSPAIR teLine;   // Line
    RVC_teGENERIC_STSPAIR teInOut;  // Input/Output
    uchar8 ucExtremeLoV;            // Extreme Low Voltage Level
    uchar8 ucLoV;                   // Low Voltage Level
    uchar8 ucHiV;                   // High Voltage Level
    uchar8 ucExtremeHiV;            // Extreme High Voltage Level
    uchar8 ucQualTime;              // Qualification Time
    RVC_teGENERIC_STSPAIR teBypassMode; // Bypass Mode
} RVC_tzPGN_AC_FLT_CTRL_CFG_CMD1;

// AC Fault Control Configuration Status 2
typedef struct RVC_zPGN_AC_FLT_CTRL_CFG_STS2
{
    uchar8 ucInst;                  // Instance
    RVC_teGENERIC_STSPAIR teLine;   // Line
    RVC_teGENERIC_STSPAIR teInOut;  // Input/Output
    uchar8 ucHiFreqLimit;           // High Frequency Limit
    uchar8 ucLoFreqLimit;           // Low Frequency Limit
} RVC_tzPGN_AC_FLT_CTRL_CFG_STS2;

// AC Fault Control Configuration Status 1
typedef struct RVC_zPGN_AC_FLT_CTRL_CFG_STS1
{
    uchar8 ucInst;                  // Instance
    RVC_teGENERIC_STSPAIR teLine;   // Line
    RVC_teGENERIC_STSPAIR teInOut;  // Input/Output
    uchar8 ucExtremeLoV;            // Extreme Low Voltage Level
    uchar8 ucLoV;                   // Low Voltage Level
    uchar8 ucHiV;                   // High Voltage Level
    uchar8 ucExtremeHiV;            // Extreme High Voltage Level
    uchar8 ucQualTime;              // Qualification Time
    RVC_teGENERIC_STSPAIR teBypassMode; // Bypass Mode
} RVC_tzPGN_AC_FLT_CTRL_CFG_STS1;

// AGS Criterion Command - Xfer Switch AC Voltage
typedef struct RVC_zPGN_AGS_CRIT_CMD_XFER_AC_V
{
    uchar8 ucInst;                  // Instance
    RVC_teCMD_PAIR teCmd;           // Command
    RVC_teCMD_PAIR teActSts;        // Active Status
    RVC_teRVCAGS_CRITERION_TYPE teCritType; // Criterion Type
    uchar8 ucAcInst;                // AC Instance
    uint16 uiAcVThresh;             // AC Voltage Threshold
    uchar8 ucTimeUnderThresh;       // Time Under Threshold
} RVC_tzPGN_AGS_CRIT_CMD_XFER_AC_V;

// AGS Criterion Command - DC State of Charge
typedef struct RVC_zPGN_AGS_CRIT_CMD_DC_SOC
{
    uchar8 ucInst;                  // Instance
    RVC_teCMD_PAIR teCmd;           // Command
    RVC_teCMD_PAIR teActSts;        // Active Status
    RVC_teRVCAGS_CRITERION_TYPE teCritType; // Criterion Type
    uchar8 ucDcInst;                // DC Instance
    uchar8 ucDcSocThresh;           // DC SOC Threshold
    uchar8 ucTimeUnderThresh;       // Time Under Threshold
} RVC_tzPGN_AGS_CRIT_CMD_DC_SOC;

// AGS Criterion Command - DC Voltage
typedef struct RVC_zPGN_AGS_CRIT_CMD_DC_V
{
    uchar8 ucInst;                  // Instance
    RVC_teCMD_PAIR teCmd;           // Command
    RVC_teCMD_PAIR teActSts;        // Active Status
    RVC_teRVCAGS_CRITERION_TYPE teCritType; // Criterion Type
    uchar8 ucDcInst;                // DC Instance
    uint16 uiDcVThresh;             // DC Voltage Threshold
    uchar8 ucTimeUnderThresh;       // Time Under Threshold
} RVC_tzPGN_AGS_CRIT_CMD_DC_V;

// AGS Criterion Command - Ambient Temperature
typedef struct RVC_zPGN_AGS_CRIT_CMD_AMB_TEMP
{
    uchar8 ucInst;                  // Instance
    RVC_teCMD_PAIR teCmd;           // Command
    RVC_teCMD_PAIR teActSts;        // Active Status
    RVC_teRVCAGS_CRITERION_TYPE teCritType; // Criterion Type
    uchar8 ucThermAmbInst;          // Thermostat Ambient Instance
    uint16 uiAmbTempThresh;         // Ambient Temperature Threshold
    uchar8 ucTimeUnderThresh;       // Time Under Threshold
} RVC_tzPGN_AGS_CRIT_CMD_AMB_TEMP;

// AGS Criterion Command - DC Current
typedef struct RVC_zPGN_AGS_CRIT_CMD_DC_I
{
    uchar8 ucInst;                  // Instance
    RVC_teCMD_PAIR teCmd;           // Command
    RVC_teCMD_PAIR teActSts;        // Active Status
    RVC_teRVCAGS_CRITERION_TYPE teCritType; // Criterion Type
    uchar8 ucDcInst;                // DC Instance
    uint16 uiDcIThresh;             // DC Current Threshold
    uchar8 ucTimeOverThresh;        // Time Over Threshold
} RVC_tzPGN_AGS_CRIT_CMD_DC_I;

// AGS Criterion Status - Ambient Temperature
typedef struct RVC_zPGN_AGS_CRIT_STS_AMB_TEMP
{
    uchar8 ucInst;                  // Instance
    RVC_teGENERIC_STSPAIR teCurrDemandSts; // Current Demand Status
    RVC_teGENERIC_STSPAIR teActSts; // Active Status
    RVC_teRVCAGS_CRITERION_TYPE teCritType; // Criterion Type
    uchar8 ucThermAmbInst;          // Thermostat Ambient Instance
    uint16 uiAmbTempThresh;         // Ambient Temperature Threshold
    uchar8 ucTimeUnderThresh;       // Time Under Threshold
} RVC_tzPGN_AGS_CRIT_STS_AMB_TEMP;

// AGS Criterion Status - DC Voltage
typedef struct RVC_zPGN_AGS_CRIT_STS_DC_V
{
    uchar8 ucInst;                  // Instance
    RVC_teGENERIC_STSPAIR teCurrDemandSts; // Current Demand Status
    RVC_teGENERIC_STSPAIR teActSts; // Active Status
    RVC_teRVCAGS_CRITERION_TYPE teCritType; // Criterion Type
    uchar8 ucDcInst;                // DC Instance
    uint16 uiDcVThresh;             // DC Voltage Threshold
    uchar8 ucTimeUnderThresh;       // Time Under Threshold
} RVC_tzPGN_AGS_CRIT_STS_DC_V;

// AGS Criterion Status - DC State of Charge
typedef struct RVC_zPGN_AGS_CRIT_STS_DC_SOC
{
    uchar8 ucInst;                  // Instance
    RVC_teGENERIC_STSPAIR teCurrDemandSts; // Current Demand Status
    RVC_teGENERIC_STSPAIR teActSts; // Active Status
    RVC_teRVCAGS_CRITERION_TYPE teCritType; // Criterion Type
    uchar8 ucDcInst;                // DC Instance
    uchar8 ucDcSocThresh;           // DC SOC Threshold
    uchar8 ucTimeUnderThresh;       // Time Under Threshold
} RVC_tzPGN_AGS_CRIT_STS_DC_SOC;

// AGS Criterion Status - DC Current
typedef struct RVC_zPGN_AGS_CRIT_STS_DC_I
{
    uchar8 ucInst;                  // Instance
    RVC_teGENERIC_STSPAIR teCurrDemandSts; // Current Demand Status
    RVC_teGENERIC_STSPAIR teActSts; // Active Status
    RVC_teRVCAGS_CRITERION_TYPE teCritType; // Criterion Type
    uchar8 ucDcInst;                // DC Instance
    uint16 uiDcIThresh;             // DC Current Threshold
    uchar8 ucTimeOverThresh;        // Time Over Threshold
} RVC_tzPGN_AGS_CRIT_STS_DC_I;

// AGS Criterion Status - Xfer Switch AC Voltage
typedef struct RVC_zPGN_AGS_CRIT_STS_XFER_AC_V
{
    uchar8 ucInst;                  // Instance
    RVC_teGENERIC_STSPAIR teCurrDemandSts; // Current Demand Status
    RVC_teGENERIC_STSPAIR teActSts; // Active Status
    RVC_teRVCAGS_CRITERION_TYPE teCritType; // Criterion Type
    uchar8 ucAcInst;                // AC Instance
    uint16 uiAcVThresh;             // AC Voltage Threshold
    uchar8 ucTimeUnderThresh;       // Time Under Threshold
} RVC_tzPGN_AGS_CRIT_STS_XFER_AC_V;

// Generator Demand Command
typedef struct RVC_zPGN_GEN_DEMAND_CMD
{
    RVC_teGENERIC_STSPAIR teGenDemand; // Generator Demand
    RVC_teGENERIC_STSPAIR teQuietTimeOverride; // Quiet Time Override
    RVC_teGENERIC_STSPAIR teExtActRst; // External Activity Reset
    RVC_teGENERIC_STSPAIR teManOverride; // Manual Override
    RVC_teGENERIC_STSPAIR teGenLock; // Generator Lock
    uchar8 ucSetQuietTimeBeginHr;   // Set Quiet Time Begin Hour
    uchar8 ucSetQuietTimeBeginMin;  // Set Quiet Time Begin Minute
    uchar8 ucSetQuietTimeEndHr;     // Set Quiet Time End Hour
    uchar8 ucSetQuietTimeEndMin;    // Set Quiet Time End Minute
    uchar8 ucSetMinCycTime;         // Set Minmum Cycle Time
} RVC_tzPGN_GEN_DEMAND_CMD;

// Generator Demand Status
typedef struct RVC_zPGN_GEN_DEMAND_STS
{
    RVC_teGENERIC_STSPAIR teGenDemand; // Generator Demand
    RVC_teGENERIC_STSPAIR teIntGenDemand; // Internal Generator Demand
    RVC_teGENERIC_STSPAIR teNetGenDemand; // Network Generator Demand
    RVC_teGENERIC_STSPAIR teExtActDetect; // External Activity Detected
    RVC_teGENERIC_STSPAIR teManOverrideDetect; // Manual Override Detected
    RVC_teGENERIC_STSPAIR teQuietTime; // Quiet Time
    RVC_teGENERIC_STSPAIR teQuietTimeOverride; // Quiet Time Override
    RVC_teGENERIC_STSPAIR teGenLock; // Generator Lock
    uchar8 ucQuietTimeBeginHr;      // Quiet Time Begin Hour
    uchar8 ucQuietTimeBeginMin;     // Quiet Time Begin Minute
    uchar8 ucQuietTimeEndHr;        // Quiet Time End Hour
    uchar8 ucQuietTimeEndMin;       // Quiet Time End Minute
    uchar8 ucMinCycTime;            // Minmum Cycle Time
} RVC_tzPGN_GEN_DEMAND_STS;

// Charger Statistics Status
typedef struct RVC_zPGN_CHG_STATS_STS
{
    uchar8 ucInst;                  // Instance
} RVC_tzPGN_CHG_STATS_STS;

// Charger Configuration Command 1
typedef struct RVC_zPGN_CHG_CFG_CMD1
{
    uchar8 ucInst;                  // Instance
    RVC_teRVC_CHG_ALG teChgAlg;     // Charging Algorithm
    RVC_teRVC_CHG_MODE teChgMode;   // Charger Mode
    RVC_teGENERIC_STSPAIR teBattSensPres; // Battery Sensor Present
    RVC_teAC_LINE_NUM teChgInstLine; // Charger Installation Line
    uint16 uiBattBankSz;            // Battery Bank Size
    RVC_teRVC_BATT_TYPE teBattType; // Battery Type
    uchar8 ucMaxChgI;               // Maximum Charging Current
} RVC_tzPGN_CHG_CFG_CMD1;

// Charger Command
typedef struct RVC_zPGN_CHG_CMD
{
    uchar8 ucInst;                  // Instance
    RVC_teRVC_CHG_CMD teSts;        // Status
    RVC_teCMD_PAIR teDfltStatePwrUp; // Default State on Power Up
    RVC_teCMD_PAIR teAutoReChgEn;   // Auto Recharge Enabled
    RVC_teRVC_FORCE_CHG teForceChg; // Force Charge
    uint16 uiCtrlVCCCV;             // Control Voltage for CC/CV Mode
    uint16 uiCtrlICCCV;             // Control Current for CC/CV Mode
} RVC_tzPGN_CHG_CMD;

// Charger Configuration Status 1
typedef struct RVC_zPGN_CHG_CFG_STS1
{
    uchar8 ucInst;                  // Instance
    RVC_teRVC_CHG_ALG teChgAlg;     // Charging Algorithm
    RVC_teRVC_CHG_MODE teChgMode;   // Charger Mode
    RVC_teGENERIC_STSPAIR teBattSensPres; // Battery Sensor Present
    RVC_teAC_LINE_NUM teChgInstLine; // Charger Installation Line
    RVC_teRVC_BATT_TYPE teBattType; // Battery Type
    uint16 uiBattBankSz;            // Battery Bank Size
    uint16 uiMaxChgI;               // Maximum Charging Current
} RVC_tzPGN_CHG_CFG_STS1;

// Charger Status
typedef struct RVC_zPGN_CHG_STS
{
    uchar8 ucInst;                  // Instance
    uint16 uiChgV;                  // Charge Voltage
    uint16 uiChgI;                  // Charge Current
    uchar8 ucChgIPerMax;            // Charge Current Percent of Maximum
    RVC_teRVC_CHG_OP_STATE teOpState; // Operating State
    RVC_teCMD_PAIR teDefltStatePwrUp; // Default State on Power Up
    RVC_teCMD_PAIR teAutoReChgEn;   // Auto Recharge Enable
    RVC_teRVC_FORCE_CHG teForceChg; // Force Charge
} RVC_tzPGN_CHG_STS;

// Charger AC Status 3
typedef struct RVC_zPGN_CHG_AC_STS3
{
    uchar8 ucInst;                  // Instance
    RVC_teAC_LINE_NUM teLine;       // Line
    RVC_teGENERIC_STSPAIR teInOut;  // Input/Output
    RVC_teRVCAC_WAVEFORM teWaveform; // Waveform
    RVC_teRVC_PHASE_STS tePhsSts;   // Phase Status
    uint16 uiRealPwr;               // Real Power
    uint16 uiReactivePwr;           // Reactive Power
    uchar8 ucHarmonicDist;          // Hamonic Distortion
    uchar8 ucCompLeg;               // Complementary Leg
} RVC_tzPGN_CHG_AC_STS3;

// Charger AC Status 2
typedef struct RVC_zPGN_CHG_AC_STS2
{
    uchar8 ucInst;                  // Instance
    RVC_teAC_LINE_NUM teLine;       // Line
    RVC_teGENERIC_STSPAIR teInOut;  // Input/Output
    uint16 uiPkV;                   // Peak Voltage
    uint16 uiPkI;                   // Peak Current
    uint16 uiGndI;                  // Ground Current
    uchar8 ucCap;                   // Capacity
} RVC_tzPGN_CHG_AC_STS2;

// Charger AC Status 1
typedef struct RVC_zPGN_CHG_AC_STS1
{
    uchar8 ucInst;                  // Instance
    RVC_teAC_LINE_NUM teLine;       // Line
    RVC_teGENERIC_STSPAIR teInOut;  // Input/Output
    uint16 uiRmsV;                  // RMS Voltage
    uint16 uiRmsI;                  // RMS Current
    uint16 uiFreq;                  // Frequency
    RVC_teGENERIC_STSPAIR teFltOpnGnd; // Fault - Open Ground
    RVC_teGENERIC_STSPAIR teFltOpnNeutral; // Fault - Open Neutral
    RVC_teGENERIC_STSPAIR teFltRevPolarity; // Fault - Reverse Polarity
    RVC_teGENERIC_STSPAIR teFltGndFlt; // Fault - Ground Fault
} RVC_tzPGN_CHG_AC_STS1;

// Inverter OPS Status
typedef struct RVC_zPGN_INV_OPS_STS
{
    uchar8 ucInst;                  // Instance
    uchar8 ucTotalInst;             // Total Instance Count
    uint16 uiVoltage;               // Voltage
    uint16 uiCurrent;               // Current
    uchar8 ucTemp;                  // Temperature
} RVC_tzPGN_INV_OPS_STS;

// Inverter HVDC Bus Status
typedef struct RVC_zPGN_INV_HV_DC_BUS_STS
{
    uchar8 ucInst;                  // Instance
    uchar8 ucTotalInst;             // Total Instance Count
    uint16 uiVoltage;               // Voltage
    uint16 uiCurrent;               // Current
    uchar8 ucTemp;                  // Temperature
} RVC_tzPGN_INV_HV_DC_BUS_STS;

// Inverter APS Status
typedef struct RVC_zPGN_INV_APS_STS
{
    uchar8 ucInst;                  // Instance
    uchar8 ucTotalInst;             // Total Instance Count
    uint16 uiVoltage;               // Voltage
    uint16 uiCurrent;               // Current
    uchar8 ucTemp;                  // Temperature
} RVC_tzPGN_INV_APS_STS;

// Inverter Statistics Status
typedef struct RVC_zPGN_INV_STATS_STS
{
    uchar8 ucInst;                  // Instance
    uint16 uiNumDcUvDetects;        // Number of DC Under Voltage Detections
    uint16 uiNumInvAcOutOvlds;      // Number of Inverter AC Output Overloads
    uint16 uiNumLdSensEngage;       // Number of Times Load Sense has Been Engaged
    uint16 uiLowestDcV;             // Lowest DC Voltage
    uint16 uiHighestDcV;            // Highest DC Voltage
    uint16 uiLowestAcVIn;           // Lowest AC Input Voltage
    uint16 uiHighestAcVIn;          // Highest AC Input Voltage
    uint16 uiLowestAcVOut;          // Lowest AC Output Voltage
    uint16 uiHighestAcVOut;         // Highest AC Output Voltage
} RVC_tzPGN_INV_STATS_STS;

// Inverter Configuration Command 2
typedef struct RVC_zPGN_INV_CFG_CMD2
{
    uchar8 ucInst;                  // Instance
    uint16 uiDcSrcSdVMax;           // DC Source Shutdown Voltage - Maximum
    uint16 uiDcSrcWrnVMin;          // DC Source Warning Voltage - Minimum
    uint16 uiDcSrcWrnVMax;          // DC Source Warning Voltage - Maximum
} RVC_tzPGN_INV_CFG_CMD2;

// Inverter Configuration Command 1
typedef struct RVC_zPGN_INV_CFG_CMD1
{
    uchar8 ucInst;                  // Instance
    uint16 uiLoadSensePwrThresh;    // Load Sense Power Threshold
    uint16 uiLoadSenseIntvl;        // Load Sense Interval
    uint16 uiDcSrcSdVMin;           // DC Source Shutdown Voltage - Minimum
} RVC_tzPGN_INV_CFG_CMD1;

// Inverter Configuration Status 2
typedef struct RVC_zPGN_INV_CFG_STS2
{
    uchar8 ucInst;                  // Instance
    uint16 uiDcSrcSdVMax;           // DC Source Shutdown Voltage - Maximum
    uint16 uiDcSrcWrnVMin;          // DC Source Warning Voltage - Minimum
    uint16 uiDcSrcWrnVMax;          // DC Source Warning Voltage - Maximum
} RVC_tzPGN_INV_CFG_STS2;

// Inverter Configuration Status 1
typedef struct RVC_zPGN_INV_CFG_STS1
{
    uchar8 ucInst;                  // Instance
    uint16 uiLoadSensePwrThresh;    // Load Sense Power Threshold
    uint16 uiLoadSenseIntvl;        // Load Sense Interval
    uint16 uiDcSrcSdVMin;           // DC Source Shutdown Voltage - Minimum
    RVC_teCMD_PAIR teInvEnOnStartUp; // Inverter enable on startup
    RVC_teCMD_PAIR teLdSenseEnOnStartUp; // Load sense enable on startup
    RVC_teCMD_PAIR teAcPassThruEnOnStartUp; // AC pass-through enable on startup
    RVC_teCMD_PAIR teGenSupportEnOnStartUp; // Generator Support enable on startup
} RVC_tzPGN_INV_CFG_STS1;

// Inverter Command
typedef struct RVC_zPGN_INV_CMD
{
    uchar8 ucInst;                  // Instance
    RVC_teCMD_PAIR teInvEn;         // Inverter Enable
    RVC_teCMD_PAIR teLoadSenseEn;   // Load Sense Enable
    RVC_teCMD_PAIR tePassThruEn;    // Pass Thru Enable
    RVC_teCMD_PAIR teGenSupportEn;  // Generator Support Enable
    RVC_teCMD_PAIR teInvEnOnStartUp; // Inverter enable on startup
    RVC_teCMD_PAIR teLdSenseEnOnStartUp; // Load sense enable on startup
    RVC_teCMD_PAIR teAcPassThruEnOnStartUp; // AC pass-through enable on startup
    RVC_teCMD_PAIR teGenSupportOnStartUp; // Generator Support enable on startup
} RVC_tzPGN_INV_CMD;

// Inverter Status
typedef struct RVC_zPGN_INV_STS
{
    uchar8 ucInst;                  // Instance
    RVC_teRVC_INV_STS teSts;        // Status
    RVC_teGENERIC_STSPAIR teBattTempSensPres; // Battery Temperature Sensor Present
    RVC_teGENERIC_STSPAIR teLoadSenseEn; // Load Sense Enabled
    RVC_teGENERIC_STSPAIR teInverterEn; // Inverter Enabled
    RVC_teGENERIC_STSPAIR tePassthruEn; // Pass-through Enabled
    RVC_teGENERIC_STSPAIR teGenSupportEn; // Generator Support Enabled
} RVC_tzPGN_INV_STS;

// Inverter AC Status 3
typedef struct RVC_zPGN_INV_AC_STS3
{
    uchar8 ucInst;                  // Instance
    RVC_teAC_LINE_NUM teLine;       // Line
    RVC_teGENERIC_STSPAIR teInOut;  // Input/Output
    RVC_teRVCAC_WAVEFORM teWaveform; // Waveform
    RVC_teRVC_PHASE_STS tePhsSts;   // Phase Status
    uint16 uiRealPwr;               // Real Power
    uint16 uiReactivePwr;           // Reactive Power
    uchar8 ucHarmonicDist;          // Hamonic Distortion
    uchar8 ucCompLeg;               // Complementary Leg
} RVC_tzPGN_INV_AC_STS3;

// Inverter AC Status 2
typedef struct RVC_zPGN_INV_AC_STS2
{
    uchar8 ucInst;                  // Instance
    RVC_teAC_LINE_NUM teLine;       // Line
    RVC_teGENERIC_STSPAIR teInOut;  // Input/Output
    uint16 uiPkV;                   // Peak Voltage
    uint16 uiPkI;                   // Peak Current
    uint16 uiGndI;                  // Ground Current
    uchar8 ucCap;                   // Capacity
} RVC_tzPGN_INV_AC_STS2;

// Inverter AC Status 1
typedef struct RVC_zPGN_INV_AC_STS1
{
    uchar8 ucInst;                  // Instance
    RVC_teAC_LINE_NUM teLine;       // Line
    RVC_teGENERIC_STSPAIR teInOut;  // Input/Output
    uint16 uiRmsV;                  // RMS Voltage
    uint16 uiRmsI;                  // RMS Current
    uint16 uiFreq;                  // Frequency
    RVC_teGENERIC_STSPAIR teFltOpnGnd; // Fault - Open Ground
    RVC_teGENERIC_STSPAIR teFltOpnNeutral; // Fault - Open Neutral
    RVC_teGENERIC_STSPAIR teFltRevPolarity; // Fault - Reverse Polarity
    RVC_teGENERIC_STSPAIR teFltGndFlt; // Fault - Ground Fault
} RVC_tzPGN_INV_AC_STS1;

// Generator Starter Configuration Command
typedef struct RVC_zPGN_GEN_START_CFG_CMD
{
    RVC_teRVC_GEN_TYPE teGenType;   // Generator Type
    uchar8 ucGenPrecrankTime;       // Generator Pre-crank Time
    uchar8 ucGenMaxCrankTime;       // Generator Maximum Crank Time
    uchar8 ucGenStopTime;           // Generator Stop Time
} RVC_tzPGN_GEN_START_CFG_CMD;

// Generator Starter Configuration Status
typedef struct RVC_zPGN_GEN_START_CFG_STS
{
    RVC_teRVC_GEN_TYPE teGenType;   // Generator Type
    uchar8 ucGenPrecrankTime;       // Generator Pre-crank Time
    uchar8 ucGenMaxCrankTime;       // Generator Maximum Crank Time
    uchar8 ucGenStopTime;           // Generator Stop Time
} RVC_tzPGN_GEN_START_CFG_STS;

// Generator Command
typedef struct RVC_zPGN_GEN_CMD
{
    RVC_teRVC_GEN_CMD teCmd;        // Command
} RVC_tzPGN_GEN_CMD;

// Generator Status 2
typedef struct RVC_zPGN_GEN_STS2
{
    RVC_teGENERIC_STSPAIR teTempSdSw; // Temperature Shutdown Switch
    RVC_teGENERIC_STSPAIR teOilPresSdSw; // Oil Pressure Shutdown Switch
    RVC_teGENERIC_STSPAIR teOilLvlSw; // Oil Level Switch
    RVC_teGENERIC_STSPAIR teCautionLight; // Caution Light
    uchar8 ucEngCoolantTemp;        // Engine Coolant Temperature
    uchar8 ucEngOilPress;           // Engine Oil Pressure
    uint16 uiEngRpm;                // Engine RPM
    uint16 uiFuelRate;              // Fuel Rate
} RVC_tzPGN_GEN_STS2;

// Generator Status 1
typedef struct RVC_zPGN_GEN_STS1
{
    RVC_teRVC_INV_STS teSts;        // Status
    uint32 ulEngRunTime;            // Engine Run Time
    uchar8 ucEngLoad;               // Engine load
    uint16 uiStartBattV;            // Start Battery Voltage
} RVC_tzPGN_GEN_STS1;

// Communication Status 3
typedef struct RVC_zPGN_COMM_STS3
{
    uint16 uiBusOffErrCnt;          // Bus-Off Error Count
    uint16 uiRxFrameDropCnt;        // Receive Frames Dropped Count
    uint16 uiTxFrameDropCnt;        // Transmit Frames Dropped Count
} RVC_tzPGN_COMM_STS3;

// Communication Status 2
typedef struct RVC_zPGN_COMM_STS2
{
    uint32 ulTxFramesCnt;           // Transmitted Frames Count
    uint32 ulRxFramesCnt;           // Received Frames Count
} RVC_tzPGN_COMM_STS2;

// Communication Status 1
typedef struct RVC_zPGN_COMM_STS1
{
    uint32 ulTmrCnt;                // Timer Count
    uint16 uiRxErrCnt;              // Receive Error Count
    uint16 uiTxErrCnt;              // Transmit Error Count
} RVC_tzPGN_COMM_STS1;

// DC Source Status 3
typedef struct RVC_zPGN_DC_SRC_STS3
{
    RVC_teDC_SRC_ID teInst;         // Instance
    RVC_teRVC_DEVICE_PRIORITY teDevPri; // Device Priority
    uchar8 ucSoh;                   // State of Health
    uint16 uiCapRemain;             // Capacity Remaining
    uchar8 ucRelCap;                // Relative Capacity
    uint16 uiAcRmsRipple;           // AC RMS Ripple
} RVC_tzPGN_DC_SRC_STS3;

// DC Source Status 2
typedef struct RVC_zPGN_DC_SRC_STS2
{
    RVC_teDC_SRC_ID teInst;         // Instance
    RVC_teRVC_DEVICE_PRIORITY teDevPri; // Device Priority
    uint16 uiSrcTemp;               // Source Temperature
    uchar8 ucSoc;                   // State of Charge
    uint16 uiTimeRemain;            // Time Remaining
    RVC_teTIME_REMAIN_INTERP teTimeRemainInterp; // Time Remaining Interpretation
} RVC_tzPGN_DC_SRC_STS2;

// DC Source Status 1
typedef struct RVC_zPGN_DC_SRC_STS1
{
    RVC_teDC_SRC_ID teInst;         // Instance
    RVC_teRVC_DEVICE_PRIORITY teDevPri; // Device Priority
    uint16 uiDcV;                   // DC Voltage
    uint32 ulDcI;                   // DC Current
} RVC_tzPGN_DC_SRC_STS1;

// Set System Date and Time
typedef struct RVC_zPGN_SET_SYS_DATE_TIME
{
    uchar8 ucYr;                    // Year
    uchar8 ucMn;                    // Month
    uchar8 ucDate;                  // Date
    uchar8 ucDow;                   // Day of the week
    uchar8 ucHr;                    // Hour
    uchar8 ucMin;                   // Minute
    uchar8 ucSec;                   // Second
    uchar8 ucTz;                    // Time Zone
} RVC_tzPGN_SET_SYS_DATE_TIME;

// System Date and Time Status
typedef struct RVC_zPGN_SYS_DATE_TIME_STS
{
    uchar8 ucYr;                    // Year
    uchar8 ucMn;                    // Month
    uchar8 ucDate;                  // Date
    uchar8 ucDow;                   // Day of the week
    uchar8 ucHr;                    // Hour
    uchar8 ucMin;                   // Minute
    uchar8 ucSec;                   // Second
    uchar8 ucTz;                    // Time Zone
} RVC_tzPGN_SYS_DATE_TIME_STS;

