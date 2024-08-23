#include <iostream>
#include "rvcudefs.h" 
#include "xbudefs.h" 
#include "xbgdefs.h" 
#include "xbwincb.h" 
#include "pgnudefs.h" 

// Auto-generated class definition for RVC_tzPGN_ISO_ACK
typedef struct IsoAck {
    RVC_teACK_STS teCtrlByte;
    RVC_teGRP_FUNC_VALUE teGroupFunctionValue;
    uint32 ulParmGrpNum;
    void Print() const {
        std::cout << "PGN: IsoAck" << std::endl;
        std::cout << "teCtrlByte: " << teCtrlByte << std::endl;
        std::cout << "teGroupFunctionValue: " << teGroupFunctionValue << std::endl;
        std::cout << "ulParmGrpNum: " << ulParmGrpNum << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "CtrlByte") {
        }
        if (mnem == "GroupFunctionValue") {
        }
        if (mnem == "ParmGrpNum") {
        }
    }
} IsoAck;

// Auto-generated class definition for RVC_tzPGN_PM_PARAM_DEV_MD_CFG
typedef struct PmParamDevMdCfg {
    RVC_teCTRL_MODE teDesiredMode;
    uchar8 ucReserved;
    uint16 uiReserved;
    void Print() const {
        std::cout << "PGN: PmParamDevMdCfg" << std::endl;
        std::cout << "teDesiredMode: " << teDesiredMode << std::endl;
        std::cout << "ucReserved: " << static_cast<int>(ucReserved) << std::endl;
        std::cout << "uiReserved: " << uiReserved << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "DesiredMode") {
        }
        if (mnem == "Reserved") {
            return (ucReserved == 0xff) ? "No Data" : (ucReserved == 0xfe) ? "Out of range" : (ucReserved == 0xfd) ? "Reserved" : std::to_string(ucReserved);
        }
        if (mnem == "Reserved") {
            return (ucReserved == 0xffff) ? "No Data" : (ucReserved == 0xfffe) ? "Out of range" : (ucReserved == 0xfffd) ? "Reserved" : std::to_string(ucReserved);
        }
        if (mnem == "Reserved") {
            return (uiReserved == 0xff) ? "No Data" : (uiReserved == 0xfe) ? "Out of range" : (uiReserved == 0xfd) ? "Reserved" : std::to_string(uiReserved);
        }
        if (mnem == "Reserved") {
            return (uiReserved == 0xffff) ? "No Data" : (uiReserved == 0xfffe) ? "Out of range" : (uiReserved == 0xfffd) ? "Reserved" : std::to_string(uiReserved);
        }
    }
} PmParamDevMdCfg;

// Auto-generated class definition for RVC_tzPGN_PM_PARAM_ASSOC_CFG
typedef struct PmParamAssocCfg {
    RVC_teASSN_TYPE teAssocType;
    uchar8 ucAssocInst;
    uchar8 ucAssocId;
    uchar8 ucReserved;
    void Print() const {
        std::cout << "PGN: PmParamAssocCfg" << std::endl;
        std::cout << "teAssocType: " << teAssocType << std::endl;
        std::cout << "ucAssocInst: " << static_cast<int>(ucAssocInst) << std::endl;
        std::cout << "ucAssocId: " << static_cast<int>(ucAssocId) << std::endl;
        std::cout << "ucReserved: " << static_cast<int>(ucReserved) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "AssocType") {
        }
        if (mnem == "AssocInst") {
        }
        if (mnem == "AssocId") {
            return (ucAssocId == 0xff) ? "No Data" : (ucAssocId == 0xfe) ? "Out of range" : (ucAssocId == 0xfd) ? "Reserved" : std::to_string(ucAssocId);
        }
        if (mnem == "Reserved") {
            return (ucReserved == 0xff) ? "No Data" : (ucReserved == 0xfe) ? "Out of range" : (ucReserved == 0xfd) ? "Reserved" : std::to_string(ucReserved);
        }
    }
} PmParamAssocCfg;

// Auto-generated class definition for RVC_tzPGN_PM_PARAM_AUX_CFG
typedef struct PmParamAuxCfg {
    uchar8 ucOutputId;
    RVC_teAUX_DESIRED_STATE teDesiredState;
    RVC_teAUX_ACTIVE_LVL teActiveLevel;
    RVC_teAUX_TRIGGER_ID teSetTrigger;
    RVC_teAUX_TRIGGER_ID teClrTrigger;
    void Print() const {
        std::cout << "PGN: PmParamAuxCfg" << std::endl;
        std::cout << "ucOutputId: " << static_cast<int>(ucOutputId) << std::endl;
        std::cout << "teDesiredState: " << teDesiredState << std::endl;
        std::cout << "teActiveLevel: " << teActiveLevel << std::endl;
        std::cout << "teSetTrigger: " << teSetTrigger << std::endl;
        std::cout << "teClrTrigger: " << teClrTrigger << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "OutputId") {
        }
        if (mnem == "DesiredState") {
        }
        if (mnem == "ActiveLevel") {
        }
        if (mnem == "SetTrigger") {
        }
        if (mnem == "ClrTrigger") {
        }
    }
} PmParamAuxCfg;

// Auto-generated class definition for RVC_tzPGN_PM_PARAM_AUX_CFG_TRIG_LVL
typedef struct PmParamAuxCfgTrigLvl {
    RVC_teAUX_TRIGGER_ID teTriggerId;
    sint16 siTriggerLvl;
    uchar8 ucReserved;
    void Print() const {
        std::cout << "PGN: PmParamAuxCfgTrigLvl" << std::endl;
        std::cout << "teTriggerId: " << teTriggerId << std::endl;
        std::cout << "siTriggerLvl: " << siTriggerLvl << std::endl;
        std::cout << "ucReserved: " << static_cast<int>(ucReserved) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "TriggerId") {
        }
        if (mnem == "TriggerLvl") {
            return (siTriggerLvl == 0x7fff) ? "No Data" : (siTriggerLvl == 0x7ffe) ? "Out of range" : (siTriggerLvl == 0x7ffd) ? "Reserved" : std::to_string(siTriggerLvl);
        }
        if (mnem == "Reserved") {
            return (ucReserved == 0xff) ? "No Data" : (ucReserved == 0xfe) ? "Out of range" : (ucReserved == 0xfd) ? "Reserved" : std::to_string(ucReserved);
        }
    }
} PmParamAuxCfgTrigLvl;

// Auto-generated class definition for RVC_tzPGN_PM_PARAM_AUX_CFG_TRIG_DLY
typedef struct PmParamAuxCfgTrigDly {
    RVC_teAUX_TRIGGER_ID teTriggerId;
    uint16 uiTriggerDly;
    uchar8 ucReserved;
    void Print() const {
        std::cout << "PGN: PmParamAuxCfgTrigDly" << std::endl;
        std::cout << "teTriggerId: " << teTriggerId << std::endl;
        std::cout << "uiTriggerDly: " << uiTriggerDly << std::endl;
        std::cout << "ucReserved: " << static_cast<int>(ucReserved) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "TriggerId") {
        }
        if (mnem == "TriggerDly") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiTriggerDly + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "s");
            return (uiTriggerDly == 0xffff) ? "No Data" : (uiTriggerDly == 0xfffe) ? "Out of range" : (uiTriggerDly == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "Reserved") {
            return (ucReserved == 0xff) ? "No Data" : (ucReserved == 0xfe) ? "Out of range" : (ucReserved == 0xfd) ? "Reserved" : std::to_string(ucReserved);
        }
    }
} PmParamAuxCfgTrigDly;

// Auto-generated class definition for RVC_tzPGN_PM_PARAM_AUX_STS
typedef struct PmParamAuxSts {
    uchar8 ucOutputId;
    RVC_teAUX_OP_STATE teOpState;
    RVC_teAUX_ON_OFF_REASON teOnReason;
    RVC_teAUX_ON_OFF_REASON teOffReason;
    void Print() const {
        std::cout << "PGN: PmParamAuxSts" << std::endl;
        std::cout << "ucOutputId: " << static_cast<int>(ucOutputId) << std::endl;
        std::cout << "teOpState: " << teOpState << std::endl;
        std::cout << "teOnReason: " << teOnReason << std::endl;
        std::cout << "teOffReason: " << teOffReason << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "OutputId") {
        }
        if (mnem == "OpState") {
        }
        if (mnem == "OnReason") {
        }
        if (mnem == "OffReason") {
        }
    }
} PmParamAuxSts;

// Auto-generated class definition for RVC_tzPGN_ISO_REQ
typedef struct IsoReq {
    uint32 ulParmGrpNum;
    void Print() const {
        std::cout << "PGN: IsoReq" << std::endl;
        std::cout << "ulParmGrpNum: " << ulParmGrpNum << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "ParmGrpNum") {
        }
    }
} IsoReq;

// Auto-generated class definition for RVC_tzPGN_ISO_TP_XFER
typedef struct IsoTpXfer {
    uchar8 ucSeqNumMultiPacketFrame;
    uchar8 aucMultiPacketPacketizedData;
    void Print() const {
        std::cout << "PGN: IsoTpXfer" << std::endl;
        std::cout << "ucSeqNumMultiPacketFrame: " << static_cast<int>(ucSeqNumMultiPacketFrame) << std::endl;
        std::cout << "aucMultiPacketPacketizedData: " << std::endl;
        for (int i = 0; aucMultiPacketPacketizedData[i] != '\0'; i++) std::cout << aucMultiPacketPacketizedData[i] << " ";
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "SeqNumMultiPacketFrame") {
            return (ucSeqNumMultiPacketFrame == 0xff) ? "No Data" : (ucSeqNumMultiPacketFrame == 0xfe) ? "Out of range" : (ucSeqNumMultiPacketFrame == 0xfd) ? "Reserved" : std::to_string(ucSeqNumMultiPacketFrame);
        }
        if (mnem == "MultiPacketPacketizedData") {
        }
    }
} IsoTpXfer;

// Auto-generated class definition for RVC_tzPGN_ISO_TP_CONN_MAN_RTS
typedef struct IsoTpConnManRts {
    uchar8 ucRtsGroupFunctionCode;
    uint16 uiTotalMsgSz;
    uchar8 ucTotalNumFramesXmit;
    uint32 ulParmGrpNum;
    void Print() const {
        std::cout << "PGN: IsoTpConnManRts" << std::endl;
        std::cout << "ucRtsGroupFunctionCode: " << static_cast<int>(ucRtsGroupFunctionCode) << std::endl;
        std::cout << "uiTotalMsgSz: " << uiTotalMsgSz << std::endl;
        std::cout << "ucTotalNumFramesXmit: " << static_cast<int>(ucTotalNumFramesXmit) << std::endl;
        std::cout << "ulParmGrpNum: " << ulParmGrpNum << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "RtsGroupFunctionCode") {
            return (ucRtsGroupFunctionCode == 0xff) ? "No Data" : (ucRtsGroupFunctionCode == 0xfe) ? "Out of range" : (ucRtsGroupFunctionCode == 0xfd) ? "Reserved" : std::to_string(ucRtsGroupFunctionCode);
        }
        if (mnem == "TotalMsgSz") {
            return (uiTotalMsgSz == 0xffff) ? "No Data" : (uiTotalMsgSz == 0xfffe) ? "Out of range" : (uiTotalMsgSz == 0xfffd) ? "Reserved" : std::to_string(uiTotalMsgSz);
        }
        if (mnem == "TotalNumFramesXmit") {
            return (ucTotalNumFramesXmit == 0xff) ? "No Data" : (ucTotalNumFramesXmit == 0xfe) ? "Out of range" : (ucTotalNumFramesXmit == 0xfd) ? "Reserved" : std::to_string(ucTotalNumFramesXmit);
        }
        if (mnem == "ParmGrpNum") {
        }
    }
} IsoTpConnManRts;

// Auto-generated class definition for RVC_tzPGN_ISO_TP_CONN_MAN_CTS
typedef struct IsoTpConnManCts {
    uchar8 ucCtsGroupFunctionCode;
    uchar8 ucNumFramesThatCanBeSent;
    uchar8 ucNumNextFrameToBeSent;
    uint32 ulParmGrpNum;
    void Print() const {
        std::cout << "PGN: IsoTpConnManCts" << std::endl;
        std::cout << "ucCtsGroupFunctionCode: " << static_cast<int>(ucCtsGroupFunctionCode) << std::endl;
        std::cout << "ucNumFramesThatCanBeSent: " << static_cast<int>(ucNumFramesThatCanBeSent) << std::endl;
        std::cout << "ucNumNextFrameToBeSent: " << static_cast<int>(ucNumNextFrameToBeSent) << std::endl;
        std::cout << "ulParmGrpNum: " << ulParmGrpNum << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "CtsGroupFunctionCode") {
            return (ucCtsGroupFunctionCode == 0xff) ? "No Data" : (ucCtsGroupFunctionCode == 0xfe) ? "Out of range" : (ucCtsGroupFunctionCode == 0xfd) ? "Reserved" : std::to_string(ucCtsGroupFunctionCode);
        }
        if (mnem == "NumFramesThatCanBeSent") {
            return (ucNumFramesThatCanBeSent == 0xff) ? "No Data" : (ucNumFramesThatCanBeSent == 0xfe) ? "Out of range" : (ucNumFramesThatCanBeSent == 0xfd) ? "Reserved" : std::to_string(ucNumFramesThatCanBeSent);
        }
        if (mnem == "NumNextFrameToBeSent") {
            return (ucNumNextFrameToBeSent == 0xff) ? "No Data" : (ucNumNextFrameToBeSent == 0xfe) ? "Out of range" : (ucNumNextFrameToBeSent == 0xfd) ? "Reserved" : std::to_string(ucNumNextFrameToBeSent);
        }
        if (mnem == "ParmGrpNum") {
        }
    }
} IsoTpConnManCts;

// Auto-generated class definition for RVC_tzPGN_ISO_TP_CONN_MAN_EOM
typedef struct IsoTpConnManEom {
    uchar8 ucEomGroupFunctionCode;
    uint16 uiTotalMsgSz;
    uchar8 ucTotalNumFramesRxed;
    uint32 ulParmGrpNum;
    void Print() const {
        std::cout << "PGN: IsoTpConnManEom" << std::endl;
        std::cout << "ucEomGroupFunctionCode: " << static_cast<int>(ucEomGroupFunctionCode) << std::endl;
        std::cout << "uiTotalMsgSz: " << uiTotalMsgSz << std::endl;
        std::cout << "ucTotalNumFramesRxed: " << static_cast<int>(ucTotalNumFramesRxed) << std::endl;
        std::cout << "ulParmGrpNum: " << ulParmGrpNum << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "EomGroupFunctionCode") {
            return (ucEomGroupFunctionCode == 0xff) ? "No Data" : (ucEomGroupFunctionCode == 0xfe) ? "Out of range" : (ucEomGroupFunctionCode == 0xfd) ? "Reserved" : std::to_string(ucEomGroupFunctionCode);
        }
        if (mnem == "TotalMsgSz") {
            return (uiTotalMsgSz == 0xffff) ? "No Data" : (uiTotalMsgSz == 0xfffe) ? "Out of range" : (uiTotalMsgSz == 0xfffd) ? "Reserved" : std::to_string(uiTotalMsgSz);
        }
        if (mnem == "TotalNumFramesRxed") {
            return (ucTotalNumFramesRxed == 0xff) ? "No Data" : (ucTotalNumFramesRxed == 0xfe) ? "Out of range" : (ucTotalNumFramesRxed == 0xfd) ? "Reserved" : std::to_string(ucTotalNumFramesRxed);
        }
        if (mnem == "ParmGrpNum") {
        }
    }
} IsoTpConnManEom;

// Auto-generated class definition for RVC_tzPGN_ISO_TP_CONN_MAN_BAM
typedef struct IsoTpConnManBam {
    uchar8 ucBamGroupFunctionCode;
    uint16 uiTotalMsgSz;
    uchar8 ucTotalNumFramesToBeSent;
    uint32 ulParmGrpNum;
    void Print() const {
        std::cout << "PGN: IsoTpConnManBam" << std::endl;
        std::cout << "ucBamGroupFunctionCode: " << static_cast<int>(ucBamGroupFunctionCode) << std::endl;
        std::cout << "uiTotalMsgSz: " << uiTotalMsgSz << std::endl;
        std::cout << "ucTotalNumFramesToBeSent: " << static_cast<int>(ucTotalNumFramesToBeSent) << std::endl;
        std::cout << "ulParmGrpNum: " << ulParmGrpNum << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "BamGroupFunctionCode") {
            return (ucBamGroupFunctionCode == 0xff) ? "No Data" : (ucBamGroupFunctionCode == 0xfe) ? "Out of range" : (ucBamGroupFunctionCode == 0xfd) ? "Reserved" : std::to_string(ucBamGroupFunctionCode);
        }
        if (mnem == "TotalMsgSz") {
            return (uiTotalMsgSz == 0xffff) ? "No Data" : (uiTotalMsgSz == 0xfffe) ? "Out of range" : (uiTotalMsgSz == 0xfffd) ? "Reserved" : std::to_string(uiTotalMsgSz);
        }
        if (mnem == "TotalNumFramesToBeSent") {
            return (ucTotalNumFramesToBeSent == 0xff) ? "No Data" : (ucTotalNumFramesToBeSent == 0xfe) ? "Out of range" : (ucTotalNumFramesToBeSent == 0xfd) ? "Reserved" : std::to_string(ucTotalNumFramesToBeSent);
        }
        if (mnem == "ParmGrpNum") {
        }
    }
} IsoTpConnManBam;

// Auto-generated class definition for RVC_tzPGN_ISO_TP_CONN_MAN_ABORT
typedef struct IsoTpConnManAbort {
    uchar8 ucAbortGroupFunctionCode;
    uint32 ulParmGrpNum;
    void Print() const {
        std::cout << "PGN: IsoTpConnManAbort" << std::endl;
        std::cout << "ucAbortGroupFunctionCode: " << static_cast<int>(ucAbortGroupFunctionCode) << std::endl;
        std::cout << "ulParmGrpNum: " << ulParmGrpNum << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "AbortGroupFunctionCode") {
            return (ucAbortGroupFunctionCode == 0xff) ? "No Data" : (ucAbortGroupFunctionCode == 0xfe) ? "Out of range" : (ucAbortGroupFunctionCode == 0xfd) ? "Reserved" : std::to_string(ucAbortGroupFunctionCode);
        }
        if (mnem == "ParmGrpNum") {
        }
    }
} IsoTpConnManAbort;

// Auto-generated class definition for RVC_tzPGN_ISO_ADDR_CLAIM
typedef struct IsoAddrClaim {
    uint32 ulUniqueNumber;
    uint16 uiManufacturerCode;
    uchar8 ucDeviceInstanceLower;
    uchar8 ucDeviceInstanceUpper;
    RVC_teFUNC_CODE teDeviceFunction;
    uchar8 ucRsvBit;
    RVC_teDEVICE_CLASS teDeviceClass;
    uchar8 ucSystemInstance;
    RVC_teINDUSTRY_GRP teIndustryGroup;
    uchar8 ucSelfCfgAddr;
    void Print() const {
        std::cout << "PGN: IsoAddrClaim" << std::endl;
        std::cout << "ulUniqueNumber: " << ulUniqueNumber << std::endl;
        std::cout << "uiManufacturerCode: " << uiManufacturerCode << std::endl;
        std::cout << "ucDeviceInstanceLower: " << static_cast<int>(ucDeviceInstanceLower) << std::endl;
        std::cout << "ucDeviceInstanceUpper: " << static_cast<int>(ucDeviceInstanceUpper) << std::endl;
        std::cout << "teDeviceFunction: " << teDeviceFunction << std::endl;
        std::cout << "ucRsvBit: " << static_cast<int>(ucRsvBit) << std::endl;
        std::cout << "teDeviceClass: " << teDeviceClass << std::endl;
        std::cout << "ucSystemInstance: " << static_cast<int>(ucSystemInstance) << std::endl;
        std::cout << "teIndustryGroup: " << teIndustryGroup << std::endl;
        std::cout << "ucSelfCfgAddr: " << static_cast<int>(ucSelfCfgAddr) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "UniqueNumber") {
        }
        if (mnem == "ManufacturerCode") {
        }
        if (mnem == "DeviceInstanceLower") {
        }
        if (mnem == "DeviceInstanceUpper") {
        }
        if (mnem == "DeviceFunction") {
        }
        if (mnem == "RsvBit") {
        }
        if (mnem == "DeviceClass") {
        }
        if (mnem == "SystemInstance") {
        }
        if (mnem == "IndustryGroup") {
        }
        if (mnem == "SelfCfgAddr") {
        }
    }
} IsoAddrClaim;

// Auto-generated class definition for RVC_tzPGN_PM_ASSOC_CFG
typedef struct PmAssocCfg {
    RVC_tePROP_MSG_ID teMsgId;
    RVC_teASSN_TYPE teAssocType;
    uchar8 ucAssocInst;
    uchar8 ucAssocId;
    uint16 uiCrc;
    void Print() const {
        std::cout << "PGN: PmAssocCfg" << std::endl;
        std::cout << "teMsgId: " << teMsgId << std::endl;
        std::cout << "teAssocType: " << teAssocType << std::endl;
        std::cout << "ucAssocInst: " << static_cast<int>(ucAssocInst) << std::endl;
        std::cout << "ucAssocId: " << static_cast<int>(ucAssocId) << std::endl;
        std::cout << "uiCrc: " << uiCrc << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "MsgId") {
        }
        if (mnem == "AssocType") {
        }
        if (mnem == "AssocInst") {
        }
        if (mnem == "AssocId") {
            return (ucAssocId == 0xff) ? "No Data" : (ucAssocId == 0xfe) ? "Out of range" : (ucAssocId == 0xfd) ? "Reserved" : std::to_string(ucAssocId);
        }
        if (mnem == "Crc") {
            return (uiCrc == 0xffff) ? "No Data" : (uiCrc == 0xfffe) ? "Out of range" : (uiCrc == 0xfffd) ? "Reserved" : std::to_string(uiCrc);
        }
    }
} PmAssocCfg;

// Auto-generated class definition for RVC_tzPGN_PM_REQ
typedef struct PmReq {
    RVC_tePROP_MSG_ID teMsgId;
    RVC_tePROP_MSG_ID teReqId;
    uchar8 ucParam1;
    uchar8 ucParam2;
    uint16 uiCrc;
    void Print() const {
        std::cout << "PGN: PmReq" << std::endl;
        std::cout << "teMsgId: " << teMsgId << std::endl;
        std::cout << "teReqId: " << teReqId << std::endl;
        std::cout << "ucParam1: " << static_cast<int>(ucParam1) << std::endl;
        std::cout << "ucParam2: " << static_cast<int>(ucParam2) << std::endl;
        std::cout << "uiCrc: " << uiCrc << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "MsgId") {
        }
        if (mnem == "ReqId") {
        }
        if (mnem == "Param1") {
            return (ucParam1 == 0xff) ? "No Data" : (ucParam1 == 0xfe) ? "Out of range" : (ucParam1 == 0xfd) ? "Reserved" : std::to_string(ucParam1);
        }
        if (mnem == "Param2") {
            return (ucParam2 == 0xff) ? "No Data" : (ucParam2 == 0xfe) ? "Out of range" : (ucParam2 == 0xfd) ? "Reserved" : std::to_string(ucParam2);
        }
        if (mnem == "Crc") {
            return (uiCrc == 0xffff) ? "No Data" : (uiCrc == 0xfffe) ? "Out of range" : (uiCrc == 0xfffd) ? "Reserved" : std::to_string(uiCrc);
        }
    }
} PmReq;

// Auto-generated class definition for RVC_tzPGN_PM_ASSOC_STS
typedef struct PmAssocSts {
    RVC_tePROP_MSG_ID teMsgId;
    RVC_teASSN_TYPE teAssocType;
    uchar8 ucAssocInst;
    uchar8 ucAssocId;
    uint16 uiCrc;
    void Print() const {
        std::cout << "PGN: PmAssocSts" << std::endl;
        std::cout << "teMsgId: " << teMsgId << std::endl;
        std::cout << "teAssocType: " << teAssocType << std::endl;
        std::cout << "ucAssocInst: " << static_cast<int>(ucAssocInst) << std::endl;
        std::cout << "ucAssocId: " << static_cast<int>(ucAssocId) << std::endl;
        std::cout << "uiCrc: " << uiCrc << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "MsgId") {
        }
        if (mnem == "AssocType") {
        }
        if (mnem == "AssocInst") {
        }
        if (mnem == "AssocId") {
            return (ucAssocId == 0xff) ? "No Data" : (ucAssocId == 0xfe) ? "Out of range" : (ucAssocId == 0xfd) ? "Reserved" : std::to_string(ucAssocId);
        }
        if (mnem == "Crc") {
            return (uiCrc == 0xffff) ? "No Data" : (uiCrc == 0xfffe) ? "Out of range" : (uiCrc == 0xfffd) ? "Reserved" : std::to_string(uiCrc);
        }
    }
} PmAssocSts;

// Auto-generated class definition for RVC_tzPGN_PM_DEV_MD_CFG
typedef struct PmDevMdCfg {
    RVC_tePROP_MSG_ID teMsgId;
    RVC_teCTRL_MODE teDesiredMode;
    uint16 uiCrc;
    void Print() const {
        std::cout << "PGN: PmDevMdCfg" << std::endl;
        std::cout << "teMsgId: " << teMsgId << std::endl;
        std::cout << "teDesiredMode: " << teDesiredMode << std::endl;
        std::cout << "uiCrc: " << uiCrc << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "MsgId") {
        }
        if (mnem == "DesiredMode") {
        }
        if (mnem == "Crc") {
            return (uiCrc == 0xffff) ? "No Data" : (uiCrc == 0xfffe) ? "Out of range" : (uiCrc == 0xfffd) ? "Reserved" : std::to_string(uiCrc);
        }
    }
} PmDevMdCfg;

// Auto-generated class definition for RVC_tzPGN_PM_FXCC_CFG_CMD
typedef struct PmFxccCfgCmd {
    RVC_tePROP_MSG_ID teMsgId;
    uchar8 ucPwrSvHr;
    RVC_teIGNITION_CTRL_STS teInvIgnCtrl;
    RVC_teGENERIC_STSPAIR teTransMode;
    RVC_teGENERIC_STSPAIR teAlarmEnDis;
    RVC_teGENERIC_STSPAIR teOverFltRecEnDis;
    uint16 uiCrc;
    void Print() const {
        std::cout << "PGN: PmFxccCfgCmd" << std::endl;
        std::cout << "teMsgId: " << teMsgId << std::endl;
        std::cout << "ucPwrSvHr: " << static_cast<int>(ucPwrSvHr) << std::endl;
        std::cout << "teInvIgnCtrl: " << teInvIgnCtrl << std::endl;
        std::cout << "teTransMode: " << teTransMode << std::endl;
        std::cout << "teAlarmEnDis: " << teAlarmEnDis << std::endl;
        std::cout << "teOverFltRecEnDis: " << teOverFltRecEnDis << std::endl;
        std::cout << "uiCrc: " << uiCrc << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "MsgId") {
        }
        if (mnem == "PwrSvHr") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucPwrSvHr + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "h");
            return (ucPwrSvHr == 0xff) ? "No Data" : (ucPwrSvHr == 0xfe) ? "Out of range" : (ucPwrSvHr == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "InvIgnCtrl") {
        }
        if (mnem == "TransMode") {
        }
        if (mnem == "AlarmEnDis") {
        }
        if (mnem == "OverFltRecEnDis") {
        }
        if (mnem == "Crc") {
            return (uiCrc == 0xffff) ? "No Data" : (uiCrc == 0xfffe) ? "Out of range" : (uiCrc == 0xfffd) ? "Reserved" : std::to_string(uiCrc);
        }
    }
} PmFxccCfgCmd;

// Auto-generated class definition for RVC_tzPGN_PM_DEV_MD_STS
typedef struct PmDevMdSts {
    RVC_tePROP_MSG_ID teMsgId;
    RVC_teCTRL_MODE teCurrentMode;
    uint16 uiCrc;
    void Print() const {
        std::cout << "PGN: PmDevMdSts" << std::endl;
        std::cout << "teMsgId: " << teMsgId << std::endl;
        std::cout << "teCurrentMode: " << teCurrentMode << std::endl;
        std::cout << "uiCrc: " << uiCrc << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "MsgId") {
        }
        if (mnem == "CurrentMode") {
        }
        if (mnem == "Crc") {
            return (uiCrc == 0xffff) ? "No Data" : (uiCrc == 0xfffe) ? "Out of range" : (uiCrc == 0xfffd) ? "Reserved" : std::to_string(uiCrc);
        }
    }
} PmDevMdSts;

// Auto-generated class definition for RVC_tzPGN_PM_FXCC_CFG_STS
typedef struct PmFxccCfgSts {
    RVC_tePROP_MSG_ID teMsgId;
    uchar8 ucPwrSvHr;
    RVC_teIGNITION_CTRL_STS teInvIgnCtrl;
    RVC_teGENERIC_STSPAIR teTransMode;
    RVC_teGENERIC_STSPAIR teAlarmEnDis;
    RVC_teGENERIC_STSPAIR teOvrFltRecEnDis;
    uint16 uiCrc;
    void Print() const {
        std::cout << "PGN: PmFxccCfgSts" << std::endl;
        std::cout << "teMsgId: " << teMsgId << std::endl;
        std::cout << "ucPwrSvHr: " << static_cast<int>(ucPwrSvHr) << std::endl;
        std::cout << "teInvIgnCtrl: " << teInvIgnCtrl << std::endl;
        std::cout << "teTransMode: " << teTransMode << std::endl;
        std::cout << "teAlarmEnDis: " << teAlarmEnDis << std::endl;
        std::cout << "teOvrFltRecEnDis: " << teOvrFltRecEnDis << std::endl;
        std::cout << "uiCrc: " << uiCrc << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "MsgId") {
        }
        if (mnem == "PwrSvHr") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucPwrSvHr + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "h");
            return (ucPwrSvHr == 0xff) ? "No Data" : (ucPwrSvHr == 0xfe) ? "Out of range" : (ucPwrSvHr == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "InvIgnCtrl") {
        }
        if (mnem == "TransMode") {
        }
        if (mnem == "AlarmEnDis") {
        }
        if (mnem == "OvrFltRecEnDis") {
        }
        if (mnem == "Crc") {
            return (uiCrc == 0xffff) ? "No Data" : (uiCrc == 0xfffe) ? "Out of range" : (uiCrc == 0xfffd) ? "Reserved" : std::to_string(uiCrc);
        }
    }
} PmFxccCfgSts;

// Auto-generated class definition for RVC_tzPGN_PM_SW_VER_STS
typedef struct PmSwVerSts {
    RVC_tePROP_MSG_ID teMsgId;
    uchar8 ucProcInst;
    RVC_teSW_MOD teSwModIdent;
    uchar8 ucSwVerMsb;
    uchar8 ucSwVerIsb;
    uchar8 ucSwVerLsb;
    uchar8 ucSwBldNumMsb;
    uchar8 ucSwBldNumLsb;
    uint16 uiCrc;
    void Print() const {
        std::cout << "PGN: PmSwVerSts" << std::endl;
        std::cout << "teMsgId: " << teMsgId << std::endl;
        std::cout << "ucProcInst: " << static_cast<int>(ucProcInst) << std::endl;
        std::cout << "teSwModIdent: " << teSwModIdent << std::endl;
        std::cout << "ucSwVerMsb: " << static_cast<int>(ucSwVerMsb) << std::endl;
        std::cout << "ucSwVerIsb: " << static_cast<int>(ucSwVerIsb) << std::endl;
        std::cout << "ucSwVerLsb: " << static_cast<int>(ucSwVerLsb) << std::endl;
        std::cout << "ucSwBldNumMsb: " << static_cast<int>(ucSwBldNumMsb) << std::endl;
        std::cout << "ucSwBldNumLsb: " << static_cast<int>(ucSwBldNumLsb) << std::endl;
        std::cout << "uiCrc: " << uiCrc << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "MsgId") {
        }
        if (mnem == "ProcInst") {
        }
        if (mnem == "SwModIdent") {
        }
        if (mnem == "SwVerMsb") {
        }
        if (mnem == "SwVerIsb") {
        }
        if (mnem == "SwVerLsb") {
        }
        if (mnem == "SwBldNumMsb") {
        }
        if (mnem == "SwBldNumLsb") {
        }
        if (mnem == "Crc") {
            return (uiCrc == 0xffff) ? "No Data" : (uiCrc == 0xfffe) ? "Out of range" : (uiCrc == 0xfffd) ? "Reserved" : std::to_string(uiCrc);
        }
    }
} PmSwVerSts;

// Auto-generated class definition for RVC_tzPGN_PM_RPC_CMD
typedef struct PmRpcCmd {
    RVC_tePROP_MSG_ID teMsgId;
    uchar8 ucProcedureId;
    uint32 ulProcedureArg;
    uint16 uiCrc;
    void Print() const {
        std::cout << "PGN: PmRpcCmd" << std::endl;
        std::cout << "teMsgId: " << teMsgId << std::endl;
        std::cout << "ucProcedureId: " << static_cast<int>(ucProcedureId) << std::endl;
        std::cout << "ulProcedureArg: " << ulProcedureArg << std::endl;
        std::cout << "uiCrc: " << uiCrc << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "MsgId") {
        }
        if (mnem == "ProcedureId") {
        }
        if (mnem == "ProcedureArg") {
            return (ulProcedureArg == 0xffffffff) ? "No Data" : (ulProcedureArg == 0xfffffffe) ? "Out of range" : (ulProcedureArg == 0xfffffffd) ? "Reserved" : std::to_string(ulProcedureArg);
        }
        if (mnem == "Crc") {
            return (uiCrc == 0xffff) ? "No Data" : (uiCrc == 0xfffe) ? "Out of range" : (uiCrc == 0xfffd) ? "Reserved" : std::to_string(uiCrc);
        }
    }
} PmRpcCmd;

// Auto-generated class definition for RVC_tzPGN_PM_INV_LD_SH_STS
typedef struct PmInvLdShSts {
    RVC_tePROP_MSG_ID teMsgId;
    RVC_teAC_SRC_ID teAcSrcId;
    RVC_teAC_LINE_PM teLine;
    uchar8 ucInvOutPwr_1;
    uchar8 ucInvOutPwr_2;
    uchar8 ucInvOutPwr_3;
    uchar8 ucCurrent_1;
    uchar8 ucCurrent_2;
    uint16 uiCrc;
    void Print() const {
        std::cout << "PGN: PmInvLdShSts" << std::endl;
        std::cout << "teMsgId: " << teMsgId << std::endl;
        std::cout << "teAcSrcId: " << teAcSrcId << std::endl;
        std::cout << "teLine: " << teLine << std::endl;
        std::cout << "ucInvOutPwr_1: " << static_cast<int>(ucInvOutPwr_1) << std::endl;
        std::cout << "ucInvOutPwr_2: " << static_cast<int>(ucInvOutPwr_2) << std::endl;
        std::cout << "ucInvOutPwr_3: " << static_cast<int>(ucInvOutPwr_3) << std::endl;
        std::cout << "ucCurrent_1: " << static_cast<int>(ucCurrent_1) << std::endl;
        std::cout << "ucCurrent_2: " << static_cast<int>(ucCurrent_2) << std::endl;
        std::cout << "uiCrc: " << uiCrc << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "MsgId") {
        }
        if (mnem == "AcSrcId") {
        }
        if (mnem == "Line") {
        }
        if (mnem == "InvOutPwr_1") {
        }
        if (mnem == "InvOutPwr_2") {
        }
        if (mnem == "InvOutPwr_3") {
        }
        if (mnem == "Current_1") {
        }
        if (mnem == "Current_2") {
        }
        if (mnem == "Crc") {
            return (uiCrc == 0xffff) ? "No Data" : (uiCrc == 0xfffe) ? "Out of range" : (uiCrc == 0xfffd) ? "Reserved" : std::to_string(uiCrc);
        }
    }
} PmInvLdShSts;

// Auto-generated class definition for RVC_tzPGN_PM_INV_STS
typedef struct PmInvSts {
    RVC_tePROP_MSG_ID teMsgId;
    RVC_teAC_SRC_ID teAcSrcId;
    RVC_teINV_MODE teInvMode;
    RVC_teOP_STATE_INV teOpState;
    RVC_teCMD_PAIR teInvEnDis;
    uint16 uiCrc;
    void Print() const {
        std::cout << "PGN: PmInvSts" << std::endl;
        std::cout << "teMsgId: " << teMsgId << std::endl;
        std::cout << "teAcSrcId: " << teAcSrcId << std::endl;
        std::cout << "teInvMode: " << teInvMode << std::endl;
        std::cout << "teOpState: " << teOpState << std::endl;
        std::cout << "teInvEnDis: " << teInvEnDis << std::endl;
        std::cout << "uiCrc: " << uiCrc << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "MsgId") {
        }
        if (mnem == "AcSrcId") {
        }
        if (mnem == "InvMode") {
        }
        if (mnem == "OpState") {
        }
        if (mnem == "InvEnDis") {
        }
        if (mnem == "Crc") {
            return (uiCrc == 0xffff) ? "No Data" : (uiCrc == 0xfffe) ? "Out of range" : (uiCrc == 0xfffd) ? "Reserved" : std::to_string(uiCrc);
        }
    }
} PmInvSts;

// Auto-generated class definition for RVC_tzPGN_PM_CHG_STS
typedef struct PmChgSts {
    RVC_tePROP_MSG_ID teMsgId;
    RVC_teDC_SRC_ID teDcInOutAssocId;
    RVC_teGENERIC_STSPAIR teBattTempSensorPresent;
    RVC_teOP_STATE_CHG teOpState;
    RVC_teCHG_MODE teChgMode;
    RVC_teXANTREX_CHG_ALG teChgAlgorithm;
    sint16 siCtrlVolts;
    uint16 uiCrc;
    void Print() const {
        std::cout << "PGN: PmChgSts" << std::endl;
        std::cout << "teMsgId: " << teMsgId << std::endl;
        std::cout << "teDcInOutAssocId: " << teDcInOutAssocId << std::endl;
        std::cout << "teBattTempSensorPresent: " << teBattTempSensorPresent << std::endl;
        std::cout << "teOpState: " << teOpState << std::endl;
        std::cout << "teChgMode: " << teChgMode << std::endl;
        std::cout << "teChgAlgorithm: " << teChgAlgorithm << std::endl;
        std::cout << "siCtrlVolts: " << siCtrlVolts << std::endl;
        std::cout << "uiCrc: " << uiCrc << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "MsgId") {
        }
        if (mnem == "DcInOutAssocId") {
        }
        if (mnem == "BattTempSensorPresent") {
        }
        if (mnem == "OpState") {
        }
        if (mnem == "ChgMode") {
        }
        if (mnem == "ChgAlgorithm") {
        }
        if (mnem == "CtrlVolts") {
            char buffer[50];
            auto engval = 0.010000000707805157 * (float) (siCtrlVolts + 0.0);
            snprintf(buffer, sizeof(buffer), "%.2f %s", engval, "V");
            return (siCtrlVolts == 0x7fff) ? "No Data" : (siCtrlVolts == 0x7ffe) ? "Out of range" : (siCtrlVolts == 0x7ffd) ? "Reserved" : (buffer);
        }
        if (mnem == "Crc") {
            return (uiCrc == 0xffff) ? "No Data" : (uiCrc == 0xfffe) ? "Out of range" : (uiCrc == 0xfffd) ? "Reserved" : std::to_string(uiCrc);
        }
    }
} PmChgSts;

// Auto-generated class definition for RVC_tzPGN_PM_INV_CTRL_CMD
typedef struct PmInvCtrlCmd {
    RVC_tePROP_MSG_ID teMsgId;
    RVC_teAC_SRC_ID teAcSrcId;
    RVC_teCMD_PAIR teInvEnDis;
    RVC_teCMD_PAIR teSellToGridEnDis;
    RVC_teCMD_PAIR teForceSell;
    uint16 uiCrc;
    void Print() const {
        std::cout << "PGN: PmInvCtrlCmd" << std::endl;
        std::cout << "teMsgId: " << teMsgId << std::endl;
        std::cout << "teAcSrcId: " << teAcSrcId << std::endl;
        std::cout << "teInvEnDis: " << teInvEnDis << std::endl;
        std::cout << "teSellToGridEnDis: " << teSellToGridEnDis << std::endl;
        std::cout << "teForceSell: " << teForceSell << std::endl;
        std::cout << "uiCrc: " << uiCrc << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "MsgId") {
        }
        if (mnem == "AcSrcId") {
        }
        if (mnem == "InvEnDis") {
        }
        if (mnem == "SellToGridEnDis") {
        }
        if (mnem == "ForceSell") {
        }
        if (mnem == "Crc") {
            return (uiCrc == 0xffff) ? "No Data" : (uiCrc == 0xfffe) ? "Out of range" : (uiCrc == 0xfffd) ? "Reserved" : std::to_string(uiCrc);
        }
    }
} PmInvCtrlCmd;

// Auto-generated class definition for RVC_tzPGN_PM_AC_STS_RMS
typedef struct PmAcStsRms {
    RVC_tePROP_MSG_ID teMsgId;
    RVC_teAC_SRC_ID teAcSrcId;
    RVC_teAC_LINE_PM teLine;
    RVC_teAC_CONNECTION_PM teAcConn;
    RVC_teSRC_QUAL teQuality;
    uint16 uiFreq;
    uint16 uiCrc;
    void Print() const {
        std::cout << "PGN: PmAcStsRms" << std::endl;
        std::cout << "teMsgId: " << teMsgId << std::endl;
        std::cout << "teAcSrcId: " << teAcSrcId << std::endl;
        std::cout << "teLine: " << teLine << std::endl;
        std::cout << "teAcConn: " << teAcConn << std::endl;
        std::cout << "teQuality: " << teQuality << std::endl;
        std::cout << "uiFreq: " << uiFreq << std::endl;
        std::cout << "uiCrc: " << uiCrc << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "MsgId") {
        }
        if (mnem == "AcSrcId") {
        }
        if (mnem == "Line") {
        }
        if (mnem == "AcConn") {
        }
        if (mnem == "Quality") {
        }
        if (mnem == "Freq") {
            char buffer[50];
            auto engval = 0.009999999776482582 * (float) (uiFreq + 0.0);
            snprintf(buffer, sizeof(buffer), "%.2f %s", engval, "Hz");
            return (uiFreq == 0xffff) ? "No Data" : (uiFreq == 0xfffe) ? "Out of range" : (uiFreq == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "Crc") {
            return (uiCrc == 0xffff) ? "No Data" : (uiCrc == 0xfffe) ? "Out of range" : (uiCrc == 0xfffd) ? "Reserved" : std::to_string(uiCrc);
        }
    }
} PmAcStsRms;

// Auto-generated class definition for RVC_tzPGN_PM_AC_XFER_SW_STS
typedef struct PmAcXferSwSts {
    RVC_tePROP_MSG_ID teMsgId;
    RVC_teOP_STATE_AC_XFR_SWITCH teOpState;
    RVC_teAC_XFER_SW_MODE teXferSwMode;
    uint16 uiCrc;
    void Print() const {
        std::cout << "PGN: PmAcXferSwSts" << std::endl;
        std::cout << "teMsgId: " << teMsgId << std::endl;
        std::cout << "teOpState: " << teOpState << std::endl;
        std::cout << "teXferSwMode: " << teXferSwMode << std::endl;
        std::cout << "uiCrc: " << uiCrc << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "MsgId") {
        }
        if (mnem == "OpState") {
        }
        if (mnem == "XferSwMode") {
        }
        if (mnem == "Crc") {
            return (uiCrc == 0xffff) ? "No Data" : (uiCrc == 0xfffe) ? "Out of range" : (uiCrc == 0xfffd) ? "Reserved" : std::to_string(uiCrc);
        }
    }
} PmAcXferSwSts;

// Auto-generated class definition for RVC_tzPGN_PM_GET_CONFIG_PARAM
typedef struct PmGetConfigParam {
    RVC_tePROP_MSG_ID teMsgId;
    RVC_teCFG_PARAM_ID teCfgParamId;
    uint32 ulParamValue;
    uint16 uiCrc;
    void Print() const {
        std::cout << "PGN: PmGetConfigParam" << std::endl;
        std::cout << "teMsgId: " << teMsgId << std::endl;
        std::cout << "teCfgParamId: " << teCfgParamId << std::endl;
        std::cout << "ulParamValue: " << ulParamValue << std::endl;
        std::cout << "uiCrc: " << uiCrc << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "MsgId") {
        }
        if (mnem == "CfgParamId") {
        }
        if (mnem == "ParamValue") {
            return (ulParamValue == 0xffffffff) ? "No Data" : (ulParamValue == 0xfffffffe) ? "Out of range" : (ulParamValue == 0xfffffffd) ? "Reserved" : std::to_string(ulParamValue);
        }
        if (mnem == "Crc") {
            return (uiCrc == 0xffff) ? "No Data" : (uiCrc == 0xfffe) ? "Out of range" : (uiCrc == 0xfffd) ? "Reserved" : std::to_string(uiCrc);
        }
    }
} PmGetConfigParam;

// Auto-generated class definition for RVC_tzPGN_PM_SET_CONFIG_PARAM
typedef struct PmSetConfigParam {
    RVC_tePROP_MSG_ID teMsgId;
    RVC_teCFG_PARAM_ID teCfgParamId;
    uint32 ulParamValue;
    uint16 uiCrc;
    void Print() const {
        std::cout << "PGN: PmSetConfigParam" << std::endl;
        std::cout << "teMsgId: " << teMsgId << std::endl;
        std::cout << "teCfgParamId: " << teCfgParamId << std::endl;
        std::cout << "ulParamValue: " << ulParamValue << std::endl;
        std::cout << "uiCrc: " << uiCrc << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "MsgId") {
        }
        if (mnem == "CfgParamId") {
        }
        if (mnem == "ParamValue") {
            return (ulParamValue == 0xffffffff) ? "No Data" : (ulParamValue == 0xfffffffe) ? "Out of range" : (ulParamValue == 0xfffffffd) ? "Reserved" : std::to_string(ulParamValue);
        }
        if (mnem == "Crc") {
            return (uiCrc == 0xffff) ? "No Data" : (uiCrc == 0xfffe) ? "Out of range" : (uiCrc == 0xfffd) ? "Reserved" : std::to_string(uiCrc);
        }
    }
} PmSetConfigParam;

// Auto-generated class definition for RVC_tzPGN_PM_GET_STATUS_PARAM
typedef struct PmGetStatusParam {
    RVC_tePROP_MSG_ID teMsgId;
    RVC_teSTS_PARAM_ID teStsParamId;
    uint32 ulParamValue;
    uint16 uiCrc;
    void Print() const {
        std::cout << "PGN: PmGetStatusParam" << std::endl;
        std::cout << "teMsgId: " << teMsgId << std::endl;
        std::cout << "teStsParamId: " << teStsParamId << std::endl;
        std::cout << "ulParamValue: " << ulParamValue << std::endl;
        std::cout << "uiCrc: " << uiCrc << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "MsgId") {
        }
        if (mnem == "StsParamId") {
        }
        if (mnem == "ParamValue") {
            return (ulParamValue == 0xffffffff) ? "No Data" : (ulParamValue == 0xfffffffe) ? "Out of range" : (ulParamValue == 0xfffffffd) ? "Reserved" : std::to_string(ulParamValue);
        }
        if (mnem == "Crc") {
            return (uiCrc == 0xffff) ? "No Data" : (uiCrc == 0xfffe) ? "Out of range" : (uiCrc == 0xfffd) ? "Reserved" : std::to_string(uiCrc);
        }
    }
} PmGetStatusParam;

// Auto-generated class definition for RVC_tzPGN_PM_PPN_SESSION_CMD
typedef struct PmPpnSessionCmd {
    RVC_tePROP_MSG_ID teMsgId;
    RVC_teSESSION_STS teSessEnDis;
    void Print() const {
        std::cout << "PGN: PmPpnSessionCmd" << std::endl;
        std::cout << "teMsgId: " << teMsgId << std::endl;
        std::cout << "teSessEnDis: " << teSessEnDis << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "MsgId") {
        }
        if (mnem == "SessEnDis") {
        }
    }
} PmPpnSessionCmd;

// Auto-generated class definition for RVC_tzPGN_PM_PPN_SESSION_RSP
typedef struct PmPpnSessionRsp {
    RVC_tePROP_MSG_ID teMsgId;
    RVC_teSESSION_STS teSessEnDis;
    void Print() const {
        std::cout << "PGN: PmPpnSessionRsp" << std::endl;
        std::cout << "teMsgId: " << teMsgId << std::endl;
        std::cout << "teSessEnDis: " << teSessEnDis << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "MsgId") {
        }
        if (mnem == "SessEnDis") {
        }
    }
} PmPpnSessionRsp;

// Auto-generated class definition for RVC_tzPGN_PM_PPN_READ_CMD
typedef struct PmPpnReadCmd {
    RVC_tePROP_MSG_ID teMsgId;
    RVC_tePROPRIETARY_PARAM_NUM teParamId;
    void Print() const {
        std::cout << "PGN: PmPpnReadCmd" << std::endl;
        std::cout << "teMsgId: " << teMsgId << std::endl;
        std::cout << "teParamId: " << teParamId << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "MsgId") {
        }
        if (mnem == "ParamId") {
        }
    }
} PmPpnReadCmd;

// Auto-generated class definition for RVC_tzPGN_PM_PPN_READ_RSP
typedef struct PmPpnReadRsp {
    RVC_tePROP_MSG_ID teMsgId;
    RVC_tePROPRIETARY_PARAM_NUM teParamId;
    uchar8 ucValue0;
    uchar8 ucValue1;
    uchar8 ucValue2;
    uchar8 ucValue3;
    uchar8 ucValue4;
    uchar8 ucValue5;
    void Print() const {
        std::cout << "PGN: PmPpnReadRsp" << std::endl;
        std::cout << "teMsgId: " << teMsgId << std::endl;
        std::cout << "teParamId: " << teParamId << std::endl;
        std::cout << "ucValue0: " << static_cast<int>(ucValue0) << std::endl;
        std::cout << "ucValue1: " << static_cast<int>(ucValue1) << std::endl;
        std::cout << "ucValue2: " << static_cast<int>(ucValue2) << std::endl;
        std::cout << "ucValue3: " << static_cast<int>(ucValue3) << std::endl;
        std::cout << "ucValue4: " << static_cast<int>(ucValue4) << std::endl;
        std::cout << "ucValue5: " << static_cast<int>(ucValue5) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "MsgId") {
        }
        if (mnem == "ParamId") {
        }
        if (mnem == "Value0") {
            return (ucValue0 == 0xff) ? "No Data" : (ucValue0 == 0xfe) ? "Out of range" : (ucValue0 == 0xfd) ? "Reserved" : std::to_string(ucValue0);
        }
        if (mnem == "Value1") {
            return (ucValue1 == 0xff) ? "No Data" : (ucValue1 == 0xfe) ? "Out of range" : (ucValue1 == 0xfd) ? "Reserved" : std::to_string(ucValue1);
        }
        if (mnem == "Value2") {
            return (ucValue2 == 0xff) ? "No Data" : (ucValue2 == 0xfe) ? "Out of range" : (ucValue2 == 0xfd) ? "Reserved" : std::to_string(ucValue2);
        }
        if (mnem == "Value3") {
            return (ucValue3 == 0xff) ? "No Data" : (ucValue3 == 0xfe) ? "Out of range" : (ucValue3 == 0xfd) ? "Reserved" : std::to_string(ucValue3);
        }
        if (mnem == "Value4") {
            return (ucValue4 == 0xff) ? "No Data" : (ucValue4 == 0xfe) ? "Out of range" : (ucValue4 == 0xfd) ? "Reserved" : std::to_string(ucValue4);
        }
        if (mnem == "Value5") {
            return (ucValue5 == 0xff) ? "No Data" : (ucValue5 == 0xfe) ? "Out of range" : (ucValue5 == 0xfd) ? "Reserved" : std::to_string(ucValue5);
        }
    }
} PmPpnReadRsp;

// Auto-generated class definition for RVC_tzPGN_PM_PPN_WRITE_CMD
typedef struct PmPpnWriteCmd {
    RVC_tePROP_MSG_ID teMsgId;
    RVC_tePROPRIETARY_PARAM_NUM teParamId;
    uchar8 ucValue0;
    uchar8 ucValue1;
    uchar8 ucValue2;
    uchar8 ucValue3;
    uchar8 ucValue4;
    uchar8 ucValue5;
    void Print() const {
        std::cout << "PGN: PmPpnWriteCmd" << std::endl;
        std::cout << "teMsgId: " << teMsgId << std::endl;
        std::cout << "teParamId: " << teParamId << std::endl;
        std::cout << "ucValue0: " << static_cast<int>(ucValue0) << std::endl;
        std::cout << "ucValue1: " << static_cast<int>(ucValue1) << std::endl;
        std::cout << "ucValue2: " << static_cast<int>(ucValue2) << std::endl;
        std::cout << "ucValue3: " << static_cast<int>(ucValue3) << std::endl;
        std::cout << "ucValue4: " << static_cast<int>(ucValue4) << std::endl;
        std::cout << "ucValue5: " << static_cast<int>(ucValue5) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "MsgId") {
        }
        if (mnem == "ParamId") {
        }
        if (mnem == "Value0") {
            return (ucValue0 == 0xff) ? "No Data" : (ucValue0 == 0xfe) ? "Out of range" : (ucValue0 == 0xfd) ? "Reserved" : std::to_string(ucValue0);
        }
        if (mnem == "Value1") {
            return (ucValue1 == 0xff) ? "No Data" : (ucValue1 == 0xfe) ? "Out of range" : (ucValue1 == 0xfd) ? "Reserved" : std::to_string(ucValue1);
        }
        if (mnem == "Value2") {
            return (ucValue2 == 0xff) ? "No Data" : (ucValue2 == 0xfe) ? "Out of range" : (ucValue2 == 0xfd) ? "Reserved" : std::to_string(ucValue2);
        }
        if (mnem == "Value3") {
            return (ucValue3 == 0xff) ? "No Data" : (ucValue3 == 0xfe) ? "Out of range" : (ucValue3 == 0xfd) ? "Reserved" : std::to_string(ucValue3);
        }
        if (mnem == "Value4") {
            return (ucValue4 == 0xff) ? "No Data" : (ucValue4 == 0xfe) ? "Out of range" : (ucValue4 == 0xfd) ? "Reserved" : std::to_string(ucValue4);
        }
        if (mnem == "Value5") {
            return (ucValue5 == 0xff) ? "No Data" : (ucValue5 == 0xfe) ? "Out of range" : (ucValue5 == 0xfd) ? "Reserved" : std::to_string(ucValue5);
        }
    }
} PmPpnWriteCmd;

// Auto-generated class definition for RVC_tzPGN_PM_PPN_WRITE_RSP
typedef struct PmPpnWriteRsp {
    RVC_tePROP_MSG_ID teMsgId;
    RVC_tePROPRIETARY_PARAM_NUM teParamId;
    uchar8 ucValue0;
    uchar8 ucValue1;
    uchar8 ucValue2;
    uchar8 ucValue3;
    uchar8 ucValue4;
    uchar8 ucValue5;
    void Print() const {
        std::cout << "PGN: PmPpnWriteRsp" << std::endl;
        std::cout << "teMsgId: " << teMsgId << std::endl;
        std::cout << "teParamId: " << teParamId << std::endl;
        std::cout << "ucValue0: " << static_cast<int>(ucValue0) << std::endl;
        std::cout << "ucValue1: " << static_cast<int>(ucValue1) << std::endl;
        std::cout << "ucValue2: " << static_cast<int>(ucValue2) << std::endl;
        std::cout << "ucValue3: " << static_cast<int>(ucValue3) << std::endl;
        std::cout << "ucValue4: " << static_cast<int>(ucValue4) << std::endl;
        std::cout << "ucValue5: " << static_cast<int>(ucValue5) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "MsgId") {
        }
        if (mnem == "ParamId") {
        }
        if (mnem == "Value0") {
            return (ucValue0 == 0xff) ? "No Data" : (ucValue0 == 0xfe) ? "Out of range" : (ucValue0 == 0xfd) ? "Reserved" : std::to_string(ucValue0);
        }
        if (mnem == "Value1") {
            return (ucValue1 == 0xff) ? "No Data" : (ucValue1 == 0xfe) ? "Out of range" : (ucValue1 == 0xfd) ? "Reserved" : std::to_string(ucValue1);
        }
        if (mnem == "Value2") {
            return (ucValue2 == 0xff) ? "No Data" : (ucValue2 == 0xfe) ? "Out of range" : (ucValue2 == 0xfd) ? "Reserved" : std::to_string(ucValue2);
        }
        if (mnem == "Value3") {
            return (ucValue3 == 0xff) ? "No Data" : (ucValue3 == 0xfe) ? "Out of range" : (ucValue3 == 0xfd) ? "Reserved" : std::to_string(ucValue3);
        }
        if (mnem == "Value4") {
            return (ucValue4 == 0xff) ? "No Data" : (ucValue4 == 0xfe) ? "Out of range" : (ucValue4 == 0xfd) ? "Reserved" : std::to_string(ucValue4);
        }
        if (mnem == "Value5") {
            return (ucValue5 == 0xff) ? "No Data" : (ucValue5 == 0xfe) ? "Out of range" : (ucValue5 == 0xfd) ? "Reserved" : std::to_string(ucValue5);
        }
    }
} PmPpnWriteRsp;

// Auto-generated class definition for RVC_tzPGN_PM_PPN_NAK_RSP
typedef struct PmPpnNakRsp {
    RVC_tePROP_MSG_ID teMsgId;
    RVC_tePROPRIETARY_PARAM_NUM teParamId;
    void Print() const {
        std::cout << "PGN: PmPpnNakRsp" << std::endl;
        std::cout << "teMsgId: " << teMsgId << std::endl;
        std::cout << "teParamId: " << teParamId << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "MsgId") {
        }
        if (mnem == "ParamId") {
        }
    }
} PmPpnNakRsp;

// Auto-generated class definition for RVC_tzPGN_PM_LITHIONICS_CMD
typedef struct PmLithionicsCmd {
    RVC_tePROP_MSG_ID teMsgId;
    RVC_teDC_SRC_ID teInst;
    void Print() const {
        std::cout << "PGN: PmLithionicsCmd" << std::endl;
        std::cout << "teMsgId: " << teMsgId << std::endl;
        std::cout << "teInst: " << teInst << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "MsgId") {
        }
        if (mnem == "Inst") {
        }
    }
} PmLithionicsCmd;

// Auto-generated class definition for RVC_tzPGN_PM_LITHIONICS_STS
typedef struct PmLithionicsSts {
    RVC_tePROP_MSG_ID teMsgId;
    RVC_teDC_SRC_ID teInst;
    uchar8 ucMaxRecTemp;
    uchar8 ucMinRecTemp;
    RVC_teSTS_BIT teHighVoltageState;
    RVC_teSTS_BIT teChgSrcDet;
    RVC_teSTS_BIT teNeverDieResState;
    RVC_teSTS_BIT teOptoLoopOpen;
    RVC_teSTS_BIT teResVoltageRange;
    RVC_teSTS_BIT teLowVoltageState;
    RVC_teSTS_BIT teBattProtState;
    RVC_teSTS_BIT tePwrOffState;
    RVC_teSTS_BIT teAuxCntctState;
    RVC_teSTS_BIT teAuxCntctErr;
    RVC_teSTS_BIT tePreChgErr;
    RVC_teSTS_BIT teCntctFlutter;
    RVC_teSTS_BIT teAcPwrPres;
    RVC_teSTS_BIT teTsmChgPres;
    RVC_teSTS_BIT teTsmChgErr;
    RVC_teSTS_BIT teTempInterSensErr;
    RVC_teSTS_BIT teAgsrState;
    RVC_teSTS_BIT teHotTempState;
    RVC_teSTS_BIT teColdTempState;
    RVC_teSTS_BIT teAuxIn1State;
    RVC_teSTS_BIT teChgDisState;
    RVC_teSTS_BIT teOverCurrState;
    void Print() const {
        std::cout << "PGN: PmLithionicsSts" << std::endl;
        std::cout << "teMsgId: " << teMsgId << std::endl;
        std::cout << "teInst: " << teInst << std::endl;
        std::cout << "ucMaxRecTemp: " << static_cast<int>(ucMaxRecTemp) << std::endl;
        std::cout << "ucMinRecTemp: " << static_cast<int>(ucMinRecTemp) << std::endl;
        std::cout << "teHighVoltageState: " << teHighVoltageState << std::endl;
        std::cout << "teChgSrcDet: " << teChgSrcDet << std::endl;
        std::cout << "teNeverDieResState: " << teNeverDieResState << std::endl;
        std::cout << "teOptoLoopOpen: " << teOptoLoopOpen << std::endl;
        std::cout << "teResVoltageRange: " << teResVoltageRange << std::endl;
        std::cout << "teLowVoltageState: " << teLowVoltageState << std::endl;
        std::cout << "teBattProtState: " << teBattProtState << std::endl;
        std::cout << "tePwrOffState: " << tePwrOffState << std::endl;
        std::cout << "teAuxCntctState: " << teAuxCntctState << std::endl;
        std::cout << "teAuxCntctErr: " << teAuxCntctErr << std::endl;
        std::cout << "tePreChgErr: " << tePreChgErr << std::endl;
        std::cout << "teCntctFlutter: " << teCntctFlutter << std::endl;
        std::cout << "teAcPwrPres: " << teAcPwrPres << std::endl;
        std::cout << "teTsmChgPres: " << teTsmChgPres << std::endl;
        std::cout << "teTsmChgErr: " << teTsmChgErr << std::endl;
        std::cout << "teTempInterSensErr: " << teTempInterSensErr << std::endl;
        std::cout << "teAgsrState: " << teAgsrState << std::endl;
        std::cout << "teHotTempState: " << teHotTempState << std::endl;
        std::cout << "teColdTempState: " << teColdTempState << std::endl;
        std::cout << "teAuxIn1State: " << teAuxIn1State << std::endl;
        std::cout << "teChgDisState: " << teChgDisState << std::endl;
        std::cout << "teOverCurrState: " << teOverCurrState << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "MsgId") {
        }
        if (mnem == "Inst") {
        }
        if (mnem == "MaxRecTemp") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucMaxRecTemp + -40.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "deg C");
            return (ucMaxRecTemp == 0xff) ? "No Data" : (ucMaxRecTemp == 0xfe) ? "Out of range" : (ucMaxRecTemp == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "MinRecTemp") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucMinRecTemp + -40.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "deg C");
            return (ucMinRecTemp == 0xff) ? "No Data" : (ucMinRecTemp == 0xfe) ? "Out of range" : (ucMinRecTemp == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "HighVoltageState") {
        }
        if (mnem == "ChgSrcDet") {
        }
        if (mnem == "NeverDieResState") {
        }
        if (mnem == "OptoLoopOpen") {
        }
        if (mnem == "ResVoltageRange") {
        }
        if (mnem == "LowVoltageState") {
        }
        if (mnem == "BattProtState") {
        }
        if (mnem == "PwrOffState") {
        }
        if (mnem == "AuxCntctState") {
        }
        if (mnem == "AuxCntctErr") {
        }
        if (mnem == "PreChgErr") {
        }
        if (mnem == "CntctFlutter") {
        }
        if (mnem == "AcPwrPres") {
        }
        if (mnem == "TsmChgPres") {
        }
        if (mnem == "TsmChgErr") {
        }
        if (mnem == "TempInterSensErr") {
        }
        if (mnem == "AgsrState") {
        }
        if (mnem == "HotTempState") {
        }
        if (mnem == "ColdTempState") {
        }
        if (mnem == "AuxIn1State") {
        }
        if (mnem == "ChgDisState") {
        }
        if (mnem == "OverCurrState") {
        }
    }
} PmLithionicsSts;

// Auto-generated class definition for RVC_tzPGN_ISO_CMD_ADDR
typedef struct IsoCmdAddr {
    uint32 ulUniqueNumber;
    uint16 uiManufacturerCode;
    uchar8 ucDeviceInstanceLower;
    uchar8 ucDeviceInstanceUpper;
    RVC_teFUNC_CODE teDeviceFunction;
    uchar8 ucRsvBit;
    RVC_teDEVICE_CLASS teDeviceClass;
    uchar8 ucSystemInstance;
    RVC_teINDUSTRY_GRP teIndustryGroup;
    uchar8 ucSelfCfgAddr;
    RVC_teDFLT_SRC_ADDR tePrefSrcAddr;
    void Print() const {
        std::cout << "PGN: IsoCmdAddr" << std::endl;
        std::cout << "ulUniqueNumber: " << ulUniqueNumber << std::endl;
        std::cout << "uiManufacturerCode: " << uiManufacturerCode << std::endl;
        std::cout << "ucDeviceInstanceLower: " << static_cast<int>(ucDeviceInstanceLower) << std::endl;
        std::cout << "ucDeviceInstanceUpper: " << static_cast<int>(ucDeviceInstanceUpper) << std::endl;
        std::cout << "teDeviceFunction: " << teDeviceFunction << std::endl;
        std::cout << "ucRsvBit: " << static_cast<int>(ucRsvBit) << std::endl;
        std::cout << "teDeviceClass: " << teDeviceClass << std::endl;
        std::cout << "ucSystemInstance: " << static_cast<int>(ucSystemInstance) << std::endl;
        std::cout << "teIndustryGroup: " << teIndustryGroup << std::endl;
        std::cout << "ucSelfCfgAddr: " << static_cast<int>(ucSelfCfgAddr) << std::endl;
        std::cout << "tePrefSrcAddr: " << tePrefSrcAddr << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "UniqueNumber") {
        }
        if (mnem == "ManufacturerCode") {
        }
        if (mnem == "DeviceInstanceLower") {
        }
        if (mnem == "DeviceInstanceUpper") {
        }
        if (mnem == "DeviceFunction") {
        }
        if (mnem == "RsvBit") {
        }
        if (mnem == "DeviceClass") {
        }
        if (mnem == "SystemInstance") {
        }
        if (mnem == "IndustryGroup") {
        }
        if (mnem == "SelfCfgAddr") {
        }
        if (mnem == "PrefSrcAddr") {
        }
    }
} IsoCmdAddr;

// Auto-generated class definition for RVC_tzPGN_EXTRA_PROD_IDENT
typedef struct ExtraProdIdent {
    uchar8 ucData;
    void Print() const {
        std::cout << "PGN: ExtraProdIdent" << std::endl;
        std::cout << "ucData: " << static_cast<int>(ucData) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
    }
} ExtraProdIdent;

// Auto-generated class definition for RVC_tzPGN_PROD_IDENT
typedef struct ProdIdent {
    uint16 uiExtraCount;
    ExtraProdIdent ptzExtra;
    void Print() const {
        std::cout << "PGN: ProdIdent" << std::endl;
        std::cout << "uiExtraCount: " << uiExtraCount << std::endl;
        if (ptzExtra != nullptr) {
            std::cout << "ptzExtra: " << std::endl;
            for (uint16 i = 0; i < uiExtraCount; ++i) {
                std::cout << "    Extra[" << i << "]: " << std::endl;
                ProdIdentExtra[i].Print();
            }
            std::cout << " extra end " << std::endl;
        } else {
            std::cout << "  No ptzExtra" << std::endl;
        }
    }
    std::string fnGetValue(std::string mnem) {
    }
} ProdIdent;

// Auto-generated class definition for RVC_tzPGN_XB_RESET_CMD
typedef struct XbResetCmd {
    uchar8 ucProcessorInst;
    RVC_teXB_RST_CMD teCmd;
    void Print() const {
        std::cout << "PGN: XbResetCmd" << std::endl;
        std::cout << "ucProcessorInst: " << static_cast<int>(ucProcessorInst) << std::endl;
        std::cout << "teCmd: " << teCmd << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "ProcessorInst") {
        }
        if (mnem == "Cmd") {
        }
    }
} XbResetCmd;

// Auto-generated class definition for RVC_tzPGN_EXTRA_FILE_DNLD_BLK
typedef struct ExtraFileDnldBlk {
    uchar8 ucBlkData;
    void Print() const {
        std::cout << "PGN: ExtraFileDnldBlk" << std::endl;
        std::cout << "ucBlkData: " << static_cast<int>(ucBlkData) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
    }
} ExtraFileDnldBlk;

// Auto-generated class definition for RVC_tzPGN_FILE_DNLD_BLK
typedef struct FileDnldBlk {
    uint16 uiBlkNum;
    uint16 uiBlkSz;
    uint16 uiExtraCount;
    ExtraFileDnldBlk ptzExtra;
    void Print() const {
        std::cout << "PGN: FileDnldBlk" << std::endl;
        std::cout << "uiBlkNum: " << uiBlkNum << std::endl;
        std::cout << "uiBlkSz: " << uiBlkSz << std::endl;
        std::cout << "uiExtraCount: " << uiExtraCount << std::endl;
        if (ptzExtra != nullptr) {
            std::cout << "ptzExtra: " << std::endl;
            for (uint16 i = 0; i < uiExtraCount; ++i) {
                std::cout << "    Extra[" << i << "]: " << std::endl;
                FileDnldBlkExtra[i].Print();
            }
            std::cout << " extra end " << std::endl;
        } else {
            std::cout << "  No ptzExtra" << std::endl;
        }
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "BlkNum") {
            return (uiBlkNum == 0xffff) ? "No Data" : (uiBlkNum == 0xfffe) ? "Out of range" : (uiBlkNum == 0xfffd) ? "Reserved" : std::to_string(uiBlkNum);
        }
        if (mnem == "BlkSz") {
            return (uiBlkSz == 0xffff) ? "No Data" : (uiBlkSz == 0xfffe) ? "Out of range" : (uiBlkSz == 0xfffd) ? "Reserved" : std::to_string(uiBlkSz);
        }
    }
} FileDnldBlk;

// Auto-generated class definition for RVC_tzPGN_FILE_DNLD_INF
typedef struct FileDnldInf {
    RVC_teFUNC_CODE teDeviceFunction;
    uchar8 ucProcessorInst;
    RVC_teSW_MOD_ID teSwModId;
    uint32 ulSwVer;
    uint32 ulFileSz;
    uint16 uiCrc;
    uint16 uiExpectNumBlks;
    void Print() const {
        std::cout << "PGN: FileDnldInf" << std::endl;
        std::cout << "teDeviceFunction: " << teDeviceFunction << std::endl;
        std::cout << "ucProcessorInst: " << static_cast<int>(ucProcessorInst) << std::endl;
        std::cout << "teSwModId: " << teSwModId << std::endl;
        std::cout << "ulSwVer: " << ulSwVer << std::endl;
        std::cout << "ulFileSz: " << ulFileSz << std::endl;
        std::cout << "uiCrc: " << uiCrc << std::endl;
        std::cout << "uiExpectNumBlks: " << uiExpectNumBlks << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "DeviceFunction") {
        }
        if (mnem == "ProcessorInst") {
        }
        if (mnem == "SwModId") {
        }
        if (mnem == "SwVer") {
            return (ulSwVer == 0xffffffff) ? "No Data" : (ulSwVer == 0xfffffffe) ? "Out of range" : (ulSwVer == 0xfffffffd) ? "Reserved" : std::to_string(ulSwVer);
        }
        if (mnem == "FileSz") {
            return (ulFileSz == 0xffffffff) ? "No Data" : (ulFileSz == 0xfffffffe) ? "Out of range" : (ulFileSz == 0xfffffffd) ? "Reserved" : std::to_string(ulFileSz);
        }
        if (mnem == "Crc") {
            return (uiCrc == 0xffff) ? "No Data" : (uiCrc == 0xfffe) ? "Out of range" : (uiCrc == 0xfffd) ? "Reserved" : std::to_string(uiCrc);
        }
        if (mnem == "ExpectNumBlks") {
            return (uiExpectNumBlks == 0xffff) ? "No Data" : (uiExpectNumBlks == 0xfffe) ? "Out of range" : (uiExpectNumBlks == 0xfffd) ? "Reserved" : std::to_string(uiExpectNumBlks);
        }
    }
} FileDnldInf;

// Auto-generated class definition for RVC_tzPGN_SIMPLE_CALIB_CFG
typedef struct SimpleCalibCfg {
    uchar8 ucRsvBits;
    uchar8 ucMsgCount;
    uint32 ulStsPgn;
    RVC_teCAL_DATA_POINT teDataPtId;
    uint16 uiScale;
    uint16 uiOffset;
    uint32 ulValue;
    void Print() const {
        std::cout << "PGN: SimpleCalibCfg" << std::endl;
        std::cout << "ucRsvBits: " << static_cast<int>(ucRsvBits) << std::endl;
        std::cout << "ucMsgCount: " << static_cast<int>(ucMsgCount) << std::endl;
        std::cout << "ulStsPgn: " << ulStsPgn << std::endl;
        std::cout << "teDataPtId: " << teDataPtId << std::endl;
        std::cout << "uiScale: " << uiScale << std::endl;
        std::cout << "uiOffset: " << uiOffset << std::endl;
        std::cout << "ulValue: " << ulValue << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "RsvBits") {
        }
        if (mnem == "MsgCount") {
        }
        if (mnem == "StsPgn") {
        }
        if (mnem == "DataPtId") {
        }
        if (mnem == "Scale") {
            return (uiScale == 0xffff) ? "No Data" : (uiScale == 0xfffe) ? "Out of range" : (uiScale == 0xfffd) ? "Reserved" : std::to_string(uiScale);
        }
        if (mnem == "Offset") {
            return (uiOffset == 0xffff) ? "No Data" : (uiOffset == 0xfffe) ? "Out of range" : (uiOffset == 0xfffd) ? "Reserved" : std::to_string(uiOffset);
        }
        if (mnem == "Value") {
            return (ulValue == 0xffffffff) ? "No Data" : (ulValue == 0xfffffffe) ? "Out of range" : (ulValue == 0xfffffffd) ? "Reserved" : std::to_string(ulValue);
        }
    }
} SimpleCalibCfg;

// Auto-generated class definition for RVC_tzPGN_FILE_DNLD_STS
typedef struct FileDnldSts {
    RVC_teOP_STATE_LDR teOpState;
    RVC_teDL_RESULT_CODE teResultCode;
    uchar8 ucProcessorInst;
    uint16 uiModelNum;
    uint16 uiHwModRev;
    uint16 uiBlkNum;
    void Print() const {
        std::cout << "PGN: FileDnldSts" << std::endl;
        std::cout << "teOpState: " << teOpState << std::endl;
        std::cout << "teResultCode: " << teResultCode << std::endl;
        std::cout << "ucProcessorInst: " << static_cast<int>(ucProcessorInst) << std::endl;
        std::cout << "uiModelNum: " << uiModelNum << std::endl;
        std::cout << "uiHwModRev: " << uiHwModRev << std::endl;
        std::cout << "uiBlkNum: " << uiBlkNum << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "OpState") {
        }
        if (mnem == "ResultCode") {
        }
        if (mnem == "ProcessorInst") {
        }
        if (mnem == "ModelNum") {
            return (uiModelNum == 0xffff) ? "No Data" : (uiModelNum == 0xfffe) ? "Out of range" : (uiModelNum == 0xfffd) ? "Reserved" : std::to_string(uiModelNum);
        }
        if (mnem == "HwModRev") {
            return (uiHwModRev == 0xffff) ? "No Data" : (uiHwModRev == 0xfffe) ? "Out of range" : (uiHwModRev == 0xfffd) ? "Reserved" : std::to_string(uiHwModRev);
        }
        if (mnem == "BlkNum") {
            return (uiBlkNum == 0xffff) ? "No Data" : (uiBlkNum == 0xfffe) ? "Out of range" : (uiBlkNum == 0xfffd) ? "Reserved" : std::to_string(uiBlkNum);
        }
    }
} FileDnldSts;

// Auto-generated class definition for RVC_tzPGN_INST_STS
typedef struct InstSts {
    RVC_teDFLT_SRC_ADDR teDevType;
    uchar8 ucBaseInst;
    uchar8 ucMaxInst;
    uint16 uiBaseIntAddr;
    uint16 uiMaxIntAddr;
    void Print() const {
        std::cout << "PGN: InstSts" << std::endl;
        std::cout << "teDevType: " << teDevType << std::endl;
        std::cout << "ucBaseInst: " << static_cast<int>(ucBaseInst) << std::endl;
        std::cout << "ucMaxInst: " << static_cast<int>(ucMaxInst) << std::endl;
        std::cout << "uiBaseIntAddr: " << uiBaseIntAddr << std::endl;
        std::cout << "uiMaxIntAddr: " << uiMaxIntAddr << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "DevType") {
        }
        if (mnem == "BaseInst") {
            return (ucBaseInst == 0xff) ? "No Data" : (ucBaseInst == 0xfe) ? "Out of range" : (ucBaseInst == 0xfd) ? "Reserved" : std::to_string(ucBaseInst);
        }
        if (mnem == "MaxInst") {
            return (ucMaxInst == 0xff) ? "No Data" : (ucMaxInst == 0xfe) ? "Out of range" : (ucMaxInst == 0xfd) ? "Reserved" : std::to_string(ucMaxInst);
        }
        if (mnem == "BaseIntAddr") {
            return (uiBaseIntAddr == 0xffff) ? "No Data" : (uiBaseIntAddr == 0xfffe) ? "Out of range" : (uiBaseIntAddr == 0xfffd) ? "Reserved" : std::to_string(uiBaseIntAddr);
        }
        if (mnem == "MaxIntAddr") {
            return (uiMaxIntAddr == 0xffff) ? "No Data" : (uiMaxIntAddr == 0xfffe) ? "Out of range" : (uiMaxIntAddr == 0xfffd) ? "Reserved" : std::to_string(uiMaxIntAddr);
        }
    }
} InstSts;

// Auto-generated class definition for RVC_tzPGN_INST_ASSIGN
typedef struct InstAssign {
    RVC_teDFLT_SRC_ADDR teDevType;
    uchar8 ucBaseInst;
    uchar8 ucMaxInst;
    uint16 uiBaseIntAddr;
    uint16 uiMaxIntAddr;
    void Print() const {
        std::cout << "PGN: InstAssign" << std::endl;
        std::cout << "teDevType: " << teDevType << std::endl;
        std::cout << "ucBaseInst: " << static_cast<int>(ucBaseInst) << std::endl;
        std::cout << "ucMaxInst: " << static_cast<int>(ucMaxInst) << std::endl;
        std::cout << "uiBaseIntAddr: " << uiBaseIntAddr << std::endl;
        std::cout << "uiMaxIntAddr: " << uiMaxIntAddr << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "DevType") {
        }
        if (mnem == "BaseInst") {
            return (ucBaseInst == 0xff) ? "No Data" : (ucBaseInst == 0xfe) ? "Out of range" : (ucBaseInst == 0xfd) ? "Reserved" : std::to_string(ucBaseInst);
        }
        if (mnem == "MaxInst") {
            return (ucMaxInst == 0xff) ? "No Data" : (ucMaxInst == 0xfe) ? "Out of range" : (ucMaxInst == 0xfd) ? "Reserved" : std::to_string(ucMaxInst);
        }
        if (mnem == "BaseIntAddr") {
            return (uiBaseIntAddr == 0xffff) ? "No Data" : (uiBaseIntAddr == 0xfffe) ? "Out of range" : (uiBaseIntAddr == 0xfffd) ? "Reserved" : std::to_string(uiBaseIntAddr);
        }
        if (mnem == "MaxIntAddr") {
            return (uiMaxIntAddr == 0xffff) ? "No Data" : (uiMaxIntAddr == 0xfffe) ? "Out of range" : (uiMaxIntAddr == 0xfffd) ? "Reserved" : std::to_string(uiMaxIntAddr);
        }
    }
} InstAssign;

// Auto-generated class definition for RVC_tzPGN_VIRTUAL_TERMINAL
typedef struct VirtualTerminal {
    uchar8 ucAsciiChar1;
    uchar8 ucAsciiChar2;
    uchar8 ucAsciiChar3;
    uchar8 ucAsciiChar4;
    uchar8 ucAsciiChar5;
    uchar8 ucAsciiChar6;
    uchar8 ucAsciiChar7;
    uchar8 ucAsciiChar8;
    void Print() const {
        std::cout << "PGN: VirtualTerminal" << std::endl;
        std::cout << "ucAsciiChar1: " << static_cast<int>(ucAsciiChar1) << std::endl;
        std::cout << "ucAsciiChar2: " << static_cast<int>(ucAsciiChar2) << std::endl;
        std::cout << "ucAsciiChar3: " << static_cast<int>(ucAsciiChar3) << std::endl;
        std::cout << "ucAsciiChar4: " << static_cast<int>(ucAsciiChar4) << std::endl;
        std::cout << "ucAsciiChar5: " << static_cast<int>(ucAsciiChar5) << std::endl;
        std::cout << "ucAsciiChar6: " << static_cast<int>(ucAsciiChar6) << std::endl;
        std::cout << "ucAsciiChar7: " << static_cast<int>(ucAsciiChar7) << std::endl;
        std::cout << "ucAsciiChar8: " << static_cast<int>(ucAsciiChar8) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "AsciiChar1") {
            return (ucAsciiChar1 == 0xff) ? "No Data" : (ucAsciiChar1 == 0xfe) ? "Out of range" : (ucAsciiChar1 == 0xfd) ? "Reserved" : std::to_string(ucAsciiChar1);
        }
        if (mnem == "AsciiChar2") {
            return (ucAsciiChar2 == 0xff) ? "No Data" : (ucAsciiChar2 == 0xfe) ? "Out of range" : (ucAsciiChar2 == 0xfd) ? "Reserved" : std::to_string(ucAsciiChar2);
        }
        if (mnem == "AsciiChar3") {
            return (ucAsciiChar3 == 0xff) ? "No Data" : (ucAsciiChar3 == 0xfe) ? "Out of range" : (ucAsciiChar3 == 0xfd) ? "Reserved" : std::to_string(ucAsciiChar3);
        }
        if (mnem == "AsciiChar4") {
            return (ucAsciiChar4 == 0xff) ? "No Data" : (ucAsciiChar4 == 0xfe) ? "Out of range" : (ucAsciiChar4 == 0xfd) ? "Reserved" : std::to_string(ucAsciiChar4);
        }
        if (mnem == "AsciiChar5") {
            return (ucAsciiChar5 == 0xff) ? "No Data" : (ucAsciiChar5 == 0xfe) ? "Out of range" : (ucAsciiChar5 == 0xfd) ? "Reserved" : std::to_string(ucAsciiChar5);
        }
        if (mnem == "AsciiChar6") {
            return (ucAsciiChar6 == 0xff) ? "No Data" : (ucAsciiChar6 == 0xfe) ? "Out of range" : (ucAsciiChar6 == 0xfd) ? "Reserved" : std::to_string(ucAsciiChar6);
        }
        if (mnem == "AsciiChar7") {
            return (ucAsciiChar7 == 0xff) ? "No Data" : (ucAsciiChar7 == 0xfe) ? "Out of range" : (ucAsciiChar7 == 0xfd) ? "Reserved" : std::to_string(ucAsciiChar7);
        }
        if (mnem == "AsciiChar8") {
            return (ucAsciiChar8 == 0xff) ? "No Data" : (ucAsciiChar8 == 0xfe) ? "Out of range" : (ucAsciiChar8 == 0xfd) ? "Reserved" : std::to_string(ucAsciiChar8);
        }
    }
} VirtualTerminal;

// Auto-generated class definition for RVC_tzPGN_GEN_RST
typedef struct GenRst {
    RVC_teCMD_PAIR teReboot;
    RVC_teCMD_PAIR teClrFlts;
    RVC_teCMD_PAIR teRstDflt;
    RVC_teCMD_PAIR teRstStats;
    RVC_teCMD_PAIR teTestMd;
    RVC_teCMD_PAIR teRstOEM;
    RVC_teCMD_PAIR teRstLdr;
    void Print() const {
        std::cout << "PGN: GenRst" << std::endl;
        std::cout << "teReboot: " << teReboot << std::endl;
        std::cout << "teClrFlts: " << teClrFlts << std::endl;
        std::cout << "teRstDflt: " << teRstDflt << std::endl;
        std::cout << "teRstStats: " << teRstStats << std::endl;
        std::cout << "teTestMd: " << teTestMd << std::endl;
        std::cout << "teRstOEM: " << teRstOEM << std::endl;
        std::cout << "teRstLdr: " << teRstLdr << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Reboot") {
        }
        if (mnem == "ClrFlts") {
        }
        if (mnem == "RstDflt") {
        }
        if (mnem == "RstStats") {
        }
        if (mnem == "TestMd") {
        }
        if (mnem == "RstOEM") {
        }
        if (mnem == "RstLdr") {
        }
    }
} GenRst;

// Auto-generated class definition for RVC_tzPGN_PROD_INFO_STS
typedef struct ProdInfoSts {
    uchar8 ucMsgCount;
    uchar8 aucModelName;
    uchar8 aucFga;
    uint32 ulPwrLvl;
    sint32 slNomDcLvl;
    uint32 ulNomAcLvl;
    uint16 uiNomAcOutFreq;
    void Print() const {
        std::cout << "PGN: ProdInfoSts" << std::endl;
        std::cout << "ucMsgCount: " << static_cast<int>(ucMsgCount) << std::endl;
        std::cout << "aucModelName: " << std::endl;
        for (int i = 0; aucModelName[i] != '\0'; i++) std::cout << aucModelName[i] << " ";
        std::cout << "aucFga: " << std::endl;
        for (int i = 0; aucFga[i] != '\0'; i++) std::cout << aucFga[i] << " ";
        std::cout << "ulPwrLvl: " << ulPwrLvl << std::endl;
        std::cout << "slNomDcLvl: " << slNomDcLvl << std::endl;
        std::cout << "ulNomAcLvl: " << ulNomAcLvl << std::endl;
        std::cout << "uiNomAcOutFreq: " << uiNomAcOutFreq << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "MsgCount") {
        }
        if (mnem == "ModelName") {
        }
        if (mnem == "Fga") {
        }
        if (mnem == "PwrLvl") {
            char buffer[50];
            auto engval = 1.0 * (float) (ulPwrLvl + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "W");
            return (ulPwrLvl == 0xffffffff) ? "No Data" : (ulPwrLvl == 0xfffffffe) ? "Out of range" : (ulPwrLvl == 0xfffffffd) ? "Reserved" : (buffer);
        }
        if (mnem == "NomDcLvl") {
            char buffer[50];
            auto engval = 0.0010000000474974513 * (float) (slNomDcLvl + 0.0);
            snprintf(buffer, sizeof(buffer), "%.3f %s", engval, "V");
            return (slNomDcLvl == 0x7fffffff) ? "No Data" : (slNomDcLvl == 0x7ffffffe) ? "Out of range" : (slNomDcLvl == 0x7ffffffd) ? "Reserved" : (buffer);
        }
        if (mnem == "NomAcLvl") {
            char buffer[50];
            auto engval = 0.0010000000474974513 * (float) (ulNomAcLvl + 0.0);
            snprintf(buffer, sizeof(buffer), "%.3f %s", engval, "V");
            return (ulNomAcLvl == 0xffffffff) ? "No Data" : (ulNomAcLvl == 0xfffffffe) ? "Out of range" : (ulNomAcLvl == 0xfffffffd) ? "Reserved" : (buffer);
        }
        if (mnem == "NomAcOutFreq") {
            char buffer[50];
            auto engval = 0.009999999776482582 * (float) (uiNomAcOutFreq + 0.0);
            snprintf(buffer, sizeof(buffer), "%.2f %s", engval, "Hz");
            return (uiNomAcOutFreq == 0xffff) ? "No Data" : (uiNomAcOutFreq == 0xfffe) ? "Out of range" : (uiNomAcOutFreq == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} ProdInfoSts;

// Auto-generated class definition for RVC_tzPGN_EXTRA_SW_VER_STS
typedef struct ExtraSwVerSts {
    uchar8 ucProcessorInst;
    RVC_teSW_MOD_ID teSwModId;
    uint32 ulSwVer;
    uint16 uiSwBldNum;
    void Print() const {
        std::cout << "PGN: ExtraSwVerSts" << std::endl;
        std::cout << "ucProcessorInst: " << static_cast<int>(ucProcessorInst) << std::endl;
        std::cout << "teSwModId: " << teSwModId << std::endl;
        std::cout << "ulSwVer: " << ulSwVer << std::endl;
        std::cout << "uiSwBldNum: " << uiSwBldNum << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
    }
} ExtraSwVerSts;

// Auto-generated class definition for RVC_tzPGN_SW_VER_STS
typedef struct SwVerSts {
    uchar8 ucMsgCount;
    uint16 uiExtraCount;
    ExtraSwVerSts ptzExtra;
    void Print() const {
        std::cout << "PGN: SwVerSts" << std::endl;
        std::cout << "ucMsgCount: " << static_cast<int>(ucMsgCount) << std::endl;
        std::cout << "uiExtraCount: " << uiExtraCount << std::endl;
        if (ptzExtra != nullptr) {
            std::cout << "ptzExtra: " << std::endl;
            for (uint16 i = 0; i < uiExtraCount; ++i) {
                std::cout << "    Extra[" << i << "]: " << std::endl;
                SwVerStsExtra[i].Print();
            }
            std::cout << " extra end " << std::endl;
        } else {
            std::cout << "  No ptzExtra" << std::endl;
        }
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "MsgCount") {
        }
    }
} SwVerSts;

// Auto-generated class definition for RVC_tzPGN_CHG_STS3
typedef struct ChgSts3 {
    uchar8 ucChgInst;
    RVC_teDERATING_STS teDerateSts;
    RVC_teDERATING_REASON teDerateReason;
    void Print() const {
        std::cout << "PGN: ChgSts3" << std::endl;
        std::cout << "ucChgInst: " << static_cast<int>(ucChgInst) << std::endl;
        std::cout << "teDerateSts: " << teDerateSts << std::endl;
        std::cout << "teDerateReason: " << teDerateReason << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "ChgInst") {
            return (ucChgInst == 0xff) ? "No Data" : (ucChgInst == 0xfe) ? "Out of range" : (ucChgInst == 0xfd) ? "Reserved" : std::to_string(ucChgInst);
        }
        if (mnem == "DerateSts") {
        }
        if (mnem == "DerateReason") {
        }
    }
} ChgSts3;

// Auto-generated class definition for RVC_tzPGN_INV_TEMP_STS2
typedef struct InvTempSts2 {
    uchar8 ucInst;
    uint16 uiCtrlBrdTemp;
    uint16 uiCapTemp;
    uint16 uiAmbTemp;
    void Print() const {
        std::cout << "PGN: InvTempSts2" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiCtrlBrdTemp: " << uiCtrlBrdTemp << std::endl;
        std::cout << "uiCapTemp: " << uiCapTemp << std::endl;
        std::cout << "uiAmbTemp: " << uiAmbTemp << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "CtrlBrdTemp") {
            char buffer[50];
            auto engval = 0.03125 * (float) (uiCtrlBrdTemp + -8736.0);
            snprintf(buffer, sizeof(buffer), "%.2f %s", engval, "deg C");
            return (uiCtrlBrdTemp == 0xffff) ? "No Data" : (uiCtrlBrdTemp == 0xfffe) ? "Out of range" : (uiCtrlBrdTemp == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "CapTemp") {
            char buffer[50];
            auto engval = 0.03125 * (float) (uiCapTemp + -8736.0);
            snprintf(buffer, sizeof(buffer), "%.2f %s", engval, "deg C");
            return (uiCapTemp == 0xffff) ? "No Data" : (uiCapTemp == 0xfffe) ? "Out of range" : (uiCapTemp == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "AmbTemp") {
            char buffer[50];
            auto engval = 0.03125 * (float) (uiAmbTemp + -8736.0);
            snprintf(buffer, sizeof(buffer), "%.2f %s", engval, "deg C");
            return (uiAmbTemp == 0xffff) ? "No Data" : (uiAmbTemp == 0xfffe) ? "Out of range" : (uiAmbTemp == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} InvTempSts2;

// Auto-generated class definition for RVC_tzPGN_SCC_CFG_CMD5
typedef struct SccCfgCmd5 {
    uchar8 ucInst;
    uchar8 ucChgPrio;
    uchar8 ucExtTempOtSdThresh;
    uchar8 ucExtTempUtSdThresh;
    void Print() const {
        std::cout << "PGN: SccCfgCmd5" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "ucChgPrio: " << static_cast<int>(ucChgPrio) << std::endl;
        std::cout << "ucExtTempOtSdThresh: " << static_cast<int>(ucExtTempOtSdThresh) << std::endl;
        std::cout << "ucExtTempUtSdThresh: " << static_cast<int>(ucExtTempUtSdThresh) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "ChgPrio") {
        }
        if (mnem == "ExtTempOtSdThresh") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucExtTempOtSdThresh + -40.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "deg C");
            return (ucExtTempOtSdThresh == 0xff) ? "No Data" : (ucExtTempOtSdThresh == 0xfe) ? "Out of range" : (ucExtTempOtSdThresh == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "ExtTempUtSdThresh") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucExtTempUtSdThresh + -40.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "deg C");
            return (ucExtTempUtSdThresh == 0xff) ? "No Data" : (ucExtTempUtSdThresh == 0xfe) ? "Out of range" : (ucExtTempUtSdThresh == 0xfd) ? "Reserved" : (buffer);
        }
    }
} SccCfgCmd5;

// Auto-generated class definition for RVC_tzPGN_SCC_CFG_STS5
typedef struct SccCfgSts5 {
    uchar8 ucInst;
    uchar8 ucChgPrio;
    uchar8 ucExtTempOtSdThresh;
    uchar8 ucExtTempUtSdThresh;
    void Print() const {
        std::cout << "PGN: SccCfgSts5" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "ucChgPrio: " << static_cast<int>(ucChgPrio) << std::endl;
        std::cout << "ucExtTempOtSdThresh: " << static_cast<int>(ucExtTempOtSdThresh) << std::endl;
        std::cout << "ucExtTempUtSdThresh: " << static_cast<int>(ucExtTempUtSdThresh) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "ChgPrio") {
        }
        if (mnem == "ExtTempOtSdThresh") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucExtTempOtSdThresh + -40.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "deg C");
            return (ucExtTempOtSdThresh == 0xff) ? "No Data" : (ucExtTempOtSdThresh == 0xfe) ? "Out of range" : (ucExtTempOtSdThresh == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "ExtTempUtSdThresh") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucExtTempUtSdThresh + -40.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "deg C");
            return (ucExtTempUtSdThresh == 0xff) ? "No Data" : (ucExtTempUtSdThresh == 0xfe) ? "Out of range" : (ucExtTempUtSdThresh == 0xfd) ? "Reserved" : (buffer);
        }
    }
} SccCfgSts5;

// Auto-generated class definition for RVC_tzPGN_DC_SRC_CONN_STS
typedef struct DcSrcConnSts {
    uchar8 ucDevInst;
    uchar8 ucDevDSA;
    RVC_teDC_SRC_FUNC teFunction;
    uchar8 ucPrimaryDcInst;
    uchar8 ucSecondDcInst;
    void Print() const {
        std::cout << "PGN: DcSrcConnSts" << std::endl;
        std::cout << "ucDevInst: " << static_cast<int>(ucDevInst) << std::endl;
        std::cout << "ucDevDSA: " << static_cast<int>(ucDevDSA) << std::endl;
        std::cout << "teFunction: " << teFunction << std::endl;
        std::cout << "ucPrimaryDcInst: " << static_cast<int>(ucPrimaryDcInst) << std::endl;
        std::cout << "ucSecondDcInst: " << static_cast<int>(ucSecondDcInst) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "DevInst") {
            return (ucDevInst == 0xff) ? "No Data" : (ucDevInst == 0xfe) ? "Out of range" : (ucDevInst == 0xfd) ? "Reserved" : std::to_string(ucDevInst);
        }
        if (mnem == "DevDSA") {
            return (ucDevDSA == 0xff) ? "No Data" : (ucDevDSA == 0xfe) ? "Out of range" : (ucDevDSA == 0xfd) ? "Reserved" : std::to_string(ucDevDSA);
        }
        if (mnem == "Function") {
        }
        if (mnem == "PrimaryDcInst") {
            return (ucPrimaryDcInst == 0xff) ? "No Data" : (ucPrimaryDcInst == 0xfe) ? "Out of range" : (ucPrimaryDcInst == 0xfd) ? "Reserved" : std::to_string(ucPrimaryDcInst);
        }
        if (mnem == "SecondDcInst") {
            return (ucSecondDcInst == 0xff) ? "No Data" : (ucSecondDcInst == 0xfe) ? "Out of range" : (ucSecondDcInst == 0xfd) ? "Reserved" : std::to_string(ucSecondDcInst);
        }
    }
} DcSrcConnSts;

// Auto-generated class definition for RVC_tzPGN_DEVICE_STATE_SYNC
typedef struct DeviceStateSync {
    uint16 uiManufacturerCode;
    uchar8 ucPayloadID;
    uchar8 ucPayload1;
    uchar8 ucPayload2;
    uchar8 ucPayload3;
    uchar8 ucPayload4;
    uchar8 ucPayload5;
    void Print() const {
        std::cout << "PGN: DeviceStateSync" << std::endl;
        std::cout << "uiManufacturerCode: " << uiManufacturerCode << std::endl;
        std::cout << "ucPayloadID: " << static_cast<int>(ucPayloadID) << std::endl;
        std::cout << "ucPayload1: " << static_cast<int>(ucPayload1) << std::endl;
        std::cout << "ucPayload2: " << static_cast<int>(ucPayload2) << std::endl;
        std::cout << "ucPayload3: " << static_cast<int>(ucPayload3) << std::endl;
        std::cout << "ucPayload4: " << static_cast<int>(ucPayload4) << std::endl;
        std::cout << "ucPayload5: " << static_cast<int>(ucPayload5) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "ManufacturerCode") {
        }
        if (mnem == "PayloadID") {
            return (ucPayloadID == 0xff) ? "No Data" : (ucPayloadID == 0xfe) ? "Out of range" : (ucPayloadID == 0xfd) ? "Reserved" : std::to_string(ucPayloadID);
        }
        if (mnem == "Payload1") {
            return (ucPayload1 == 0xff) ? "No Data" : (ucPayload1 == 0xfe) ? "Out of range" : (ucPayload1 == 0xfd) ? "Reserved" : std::to_string(ucPayload1);
        }
        if (mnem == "Payload2") {
            return (ucPayload2 == 0xff) ? "No Data" : (ucPayload2 == 0xfe) ? "Out of range" : (ucPayload2 == 0xfd) ? "Reserved" : std::to_string(ucPayload2);
        }
        if (mnem == "Payload3") {
            return (ucPayload3 == 0xff) ? "No Data" : (ucPayload3 == 0xfe) ? "Out of range" : (ucPayload3 == 0xfd) ? "Reserved" : std::to_string(ucPayload3);
        }
        if (mnem == "Payload4") {
            return (ucPayload4 == 0xff) ? "No Data" : (ucPayload4 == 0xfe) ? "Out of range" : (ucPayload4 == 0xfd) ? "Reserved" : std::to_string(ucPayload4);
        }
        if (mnem == "Payload5") {
            return (ucPayload5 == 0xff) ? "No Data" : (ucPayload5 == 0xfe) ? "Out of range" : (ucPayload5 == 0xfd) ? "Reserved" : std::to_string(ucPayload5);
        }
    }
} DeviceStateSync;

// Auto-generated class definition for RVC_tzPGN_DC_SRC_CFG_CMD3
typedef struct DcSrcCfgCmd3 {
    uchar8 ucDevInst;
    uchar8 ucDevDSA;
    RVC_teDC_SRC_FUNC teFunction;
    uchar8 ucPrimaryDcInst;
    uchar8 ucSecondDcInst;
    void Print() const {
        std::cout << "PGN: DcSrcCfgCmd3" << std::endl;
        std::cout << "ucDevInst: " << static_cast<int>(ucDevInst) << std::endl;
        std::cout << "ucDevDSA: " << static_cast<int>(ucDevDSA) << std::endl;
        std::cout << "teFunction: " << teFunction << std::endl;
        std::cout << "ucPrimaryDcInst: " << static_cast<int>(ucPrimaryDcInst) << std::endl;
        std::cout << "ucSecondDcInst: " << static_cast<int>(ucSecondDcInst) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "DevInst") {
            return (ucDevInst == 0xff) ? "No Data" : (ucDevInst == 0xfe) ? "Out of range" : (ucDevInst == 0xfd) ? "Reserved" : std::to_string(ucDevInst);
        }
        if (mnem == "DevDSA") {
            return (ucDevDSA == 0xff) ? "No Data" : (ucDevDSA == 0xfe) ? "Out of range" : (ucDevDSA == 0xfd) ? "Reserved" : std::to_string(ucDevDSA);
        }
        if (mnem == "Function") {
        }
        if (mnem == "PrimaryDcInst") {
            return (ucPrimaryDcInst == 0xff) ? "No Data" : (ucPrimaryDcInst == 0xfe) ? "Out of range" : (ucPrimaryDcInst == 0xfd) ? "Reserved" : std::to_string(ucPrimaryDcInst);
        }
        if (mnem == "SecondDcInst") {
            return (ucSecondDcInst == 0xff) ? "No Data" : (ucSecondDcInst == 0xfe) ? "Out of range" : (ucSecondDcInst == 0xfd) ? "Reserved" : std::to_string(ucSecondDcInst);
        }
    }
} DcSrcCfgCmd3;

// Auto-generated class definition for RVC_tzPGN_BATT_SUMM
typedef struct BattSumm {
    RVC_teDC_SRC_ID teBattInst;
    RVC_teDC_SRC_ID teDcInst;
    uchar8 ucSeriesString;
    RVC_teMOD_COUNT teModuleCnt;
    uchar8 ucCellsPerModule;
    RVC_teV_STS teVoltSts;
    RVC_teTEMP_STS teTempSts;
    void Print() const {
        std::cout << "PGN: BattSumm" << std::endl;
        std::cout << "teBattInst: " << teBattInst << std::endl;
        std::cout << "teDcInst: " << teDcInst << std::endl;
        std::cout << "ucSeriesString: " << static_cast<int>(ucSeriesString) << std::endl;
        std::cout << "teModuleCnt: " << teModuleCnt << std::endl;
        std::cout << "ucCellsPerModule: " << static_cast<int>(ucCellsPerModule) << std::endl;
        std::cout << "teVoltSts: " << teVoltSts << std::endl;
        std::cout << "teTempSts: " << teTempSts << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "BattInst") {
        }
        if (mnem == "DcInst") {
        }
        if (mnem == "SeriesString") {
            return (ucSeriesString == 0xff) ? "No Data" : (ucSeriesString == 0xfe) ? "Out of range" : (ucSeriesString == 0xfd) ? "Reserved" : std::to_string(ucSeriesString);
        }
        if (mnem == "ModuleCnt") {
        }
        if (mnem == "CellsPerModule") {
            return (ucCellsPerModule == 0xff) ? "No Data" : (ucCellsPerModule == 0xfe) ? "Out of range" : (ucCellsPerModule == 0xfd) ? "Reserved" : std::to_string(ucCellsPerModule);
        }
        if (mnem == "VoltSts") {
        }
        if (mnem == "TempSts") {
        }
    }
} BattSumm;

// Auto-generated class definition for RVC_tzPGN_SCC_CFG_CMD4
typedef struct SccCfgCmd4 {
    uchar8 ucInst;
    uint16 uiBattOvRetV;
    uint16 uiBattUvRetV;
    uchar8 ucBattUvDly;
    uchar8 ucAbsorpTime;
    uchar8 ucTempCompConst;
    void Print() const {
        std::cout << "PGN: SccCfgCmd4" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiBattOvRetV: " << uiBattOvRetV << std::endl;
        std::cout << "uiBattUvRetV: " << uiBattUvRetV << std::endl;
        std::cout << "ucBattUvDly: " << static_cast<int>(ucBattUvDly) << std::endl;
        std::cout << "ucAbsorpTime: " << static_cast<int>(ucAbsorpTime) << std::endl;
        std::cout << "ucTempCompConst: " << static_cast<int>(ucTempCompConst) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "BattOvRetV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiBattOvRetV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiBattOvRetV == 0xffff) ? "No Data" : (uiBattOvRetV == 0xfffe) ? "Out of range" : (uiBattOvRetV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "BattUvRetV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiBattUvRetV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiBattUvRetV == 0xffff) ? "No Data" : (uiBattUvRetV == 0xfffe) ? "Out of range" : (uiBattUvRetV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "BattUvDly") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucBattUvDly + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "s");
            return (ucBattUvDly == 0xff) ? "No Data" : (ucBattUvDly == 0xfe) ? "Out of range" : (ucBattUvDly == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "AbsorpTime") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucAbsorpTime + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "minutes");
            return (ucAbsorpTime == 0xff) ? "No Data" : (ucAbsorpTime == 0xfe) ? "Out of range" : (ucAbsorpTime == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "TempCompConst") {
        }
    }
} SccCfgCmd4;

// Auto-generated class definition for RVC_tzPGN_SCC_CFG_STS4
typedef struct SccCfgSts4 {
    uchar8 ucInst;
    uint16 uiBattOvRetV;
    uint16 uiBattUvRetV;
    uchar8 ucBattUvDly;
    uchar8 ucAbsorpTime;
    uchar8 ucTempCompConst;
    void Print() const {
        std::cout << "PGN: SccCfgSts4" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiBattOvRetV: " << uiBattOvRetV << std::endl;
        std::cout << "uiBattUvRetV: " << uiBattUvRetV << std::endl;
        std::cout << "ucBattUvDly: " << static_cast<int>(ucBattUvDly) << std::endl;
        std::cout << "ucAbsorpTime: " << static_cast<int>(ucAbsorpTime) << std::endl;
        std::cout << "ucTempCompConst: " << static_cast<int>(ucTempCompConst) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "BattOvRetV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiBattOvRetV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiBattOvRetV == 0xffff) ? "No Data" : (uiBattOvRetV == 0xfffe) ? "Out of range" : (uiBattOvRetV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "BattUvRetV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiBattUvRetV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiBattUvRetV == 0xffff) ? "No Data" : (uiBattUvRetV == 0xfffe) ? "Out of range" : (uiBattUvRetV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "BattUvDly") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucBattUvDly + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "s");
            return (ucBattUvDly == 0xff) ? "No Data" : (ucBattUvDly == 0xfe) ? "Out of range" : (ucBattUvDly == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "AbsorpTime") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucAbsorpTime + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "minutes");
            return (ucAbsorpTime == 0xff) ? "No Data" : (ucAbsorpTime == 0xfe) ? "Out of range" : (ucAbsorpTime == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "TempCompConst") {
        }
    }
} SccCfgSts4;

// Auto-generated class definition for RVC_tzPGN_SCC_CFG_CMD3
typedef struct SccCfgCmd3 {
    uchar8 ucInst;
    uint16 uiBattUvWarnThresh;
    uint16 uiBattOvSdThresh;
    uint16 uiBattUvSdThresh;
    void Print() const {
        std::cout << "PGN: SccCfgCmd3" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiBattUvWarnThresh: " << uiBattUvWarnThresh << std::endl;
        std::cout << "uiBattOvSdThresh: " << uiBattOvSdThresh << std::endl;
        std::cout << "uiBattUvSdThresh: " << uiBattUvSdThresh << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "BattUvWarnThresh") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiBattUvWarnThresh + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiBattUvWarnThresh == 0xffff) ? "No Data" : (uiBattUvWarnThresh == 0xfffe) ? "Out of range" : (uiBattUvWarnThresh == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "BattOvSdThresh") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiBattOvSdThresh + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiBattOvSdThresh == 0xffff) ? "No Data" : (uiBattOvSdThresh == 0xfffe) ? "Out of range" : (uiBattOvSdThresh == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "BattUvSdThresh") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiBattUvSdThresh + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiBattUvSdThresh == 0xffff) ? "No Data" : (uiBattUvSdThresh == 0xfffe) ? "Out of range" : (uiBattUvSdThresh == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} SccCfgCmd3;

// Auto-generated class definition for RVC_tzPGN_SCC_CFG_STS3
typedef struct SccCfgSts3 {
    uchar8 ucInst;
    uint16 uiBattUvWarnThresh;
    uint16 uiBattOvSdThresh;
    uint16 uiBattUvSdThresh;
    void Print() const {
        std::cout << "PGN: SccCfgSts3" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiBattUvWarnThresh: " << uiBattUvWarnThresh << std::endl;
        std::cout << "uiBattOvSdThresh: " << uiBattOvSdThresh << std::endl;
        std::cout << "uiBattUvSdThresh: " << uiBattUvSdThresh << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "BattUvWarnThresh") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiBattUvWarnThresh + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiBattUvWarnThresh == 0xffff) ? "No Data" : (uiBattUvWarnThresh == 0xfffe) ? "Out of range" : (uiBattUvWarnThresh == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "BattOvSdThresh") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiBattOvSdThresh + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiBattOvSdThresh == 0xffff) ? "No Data" : (uiBattOvSdThresh == 0xfffe) ? "Out of range" : (uiBattOvSdThresh == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "BattUvSdThresh") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiBattUvSdThresh + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiBattUvSdThresh == 0xffff) ? "No Data" : (uiBattUvSdThresh == 0xfffe) ? "Out of range" : (uiBattUvSdThresh == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} SccCfgSts3;

// Auto-generated class definition for RVC_tzPGN_SCC_CFG_CMD2
typedef struct SccCfgCmd2 {
    uchar8 ucInst;
    uint16 uiBulkV;
    uint16 uiFloatV;
    uint16 uiChgRetV;
    void Print() const {
        std::cout << "PGN: SccCfgCmd2" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiBulkV: " << uiBulkV << std::endl;
        std::cout << "uiFloatV: " << uiFloatV << std::endl;
        std::cout << "uiChgRetV: " << uiChgRetV << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "BulkV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiBulkV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiBulkV == 0xffff) ? "No Data" : (uiBulkV == 0xfffe) ? "Out of range" : (uiBulkV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "FloatV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiFloatV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiFloatV == 0xffff) ? "No Data" : (uiFloatV == 0xfffe) ? "Out of range" : (uiFloatV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "ChgRetV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiChgRetV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiChgRetV == 0xffff) ? "No Data" : (uiChgRetV == 0xfffe) ? "Out of range" : (uiChgRetV == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} SccCfgCmd2;

// Auto-generated class definition for RVC_tzPGN_SCC_CFG_STS2
typedef struct SccCfgSts2 {
    uchar8 ucInst;
    uint16 uiBulkV;
    uint16 uiFloatV;
    uint16 uiChgRetV;
    void Print() const {
        std::cout << "PGN: SccCfgSts2" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiBulkV: " << uiBulkV << std::endl;
        std::cout << "uiFloatV: " << uiFloatV << std::endl;
        std::cout << "uiChgRetV: " << uiChgRetV << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "BulkV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiBulkV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiBulkV == 0xffff) ? "No Data" : (uiBulkV == 0xfffe) ? "Out of range" : (uiBulkV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "FloatV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiFloatV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiFloatV == 0xffff) ? "No Data" : (uiFloatV == 0xfffe) ? "Out of range" : (uiFloatV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "ChgRetV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiChgRetV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiChgRetV == 0xffff) ? "No Data" : (uiChgRetV == 0xfffe) ? "Out of range" : (uiChgRetV == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} SccCfgSts2;

// Auto-generated class definition for RVC_tzPGN_SCC_ARRAY_STS
typedef struct SccArraySts {
    uchar8 ucInst;
    uint16 uiArrayV;
    uint16 uiArrayI;
    void Print() const {
        std::cout << "PGN: SccArraySts" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiArrayV: " << uiArrayV << std::endl;
        std::cout << "uiArrayI: " << uiArrayI << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "ArrayV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiArrayV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiArrayV == 0xffff) ? "No Data" : (uiArrayV == 0xfffe) ? "Out of range" : (uiArrayV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "ArrayI") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiArrayI + -32000.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "A");
            return (uiArrayI == 0xffff) ? "No Data" : (uiArrayI == 0xfffe) ? "Out of range" : (uiArrayI == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} SccArraySts;

// Auto-generated class definition for RVC_tzPGN_SCC_BATT_STS
typedef struct SccBattSts {
    uchar8 ucInst;
    uchar8 ucDcSrcInst;
    uchar8 ucChgPriority;
    uint16 uiBattVolts;
    uint16 uiBattCurrent;
    uchar8 ucBattTemp;
    void Print() const {
        std::cout << "PGN: SccBattSts" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "ucDcSrcInst: " << static_cast<int>(ucDcSrcInst) << std::endl;
        std::cout << "ucChgPriority: " << static_cast<int>(ucChgPriority) << std::endl;
        std::cout << "uiBattVolts: " << uiBattVolts << std::endl;
        std::cout << "uiBattCurrent: " << uiBattCurrent << std::endl;
        std::cout << "ucBattTemp: " << static_cast<int>(ucBattTemp) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "DcSrcInst") {
        }
        if (mnem == "ChgPriority") {
        }
        if (mnem == "BattVolts") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiBattVolts + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiBattVolts == 0xffff) ? "No Data" : (uiBattVolts == 0xfffe) ? "Out of range" : (uiBattVolts == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "BattCurrent") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiBattCurrent + -32000.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "A");
            return (uiBattCurrent == 0xffff) ? "No Data" : (uiBattCurrent == 0xfffe) ? "Out of range" : (uiBattCurrent == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "BattTemp") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucBattTemp + -40.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "deg C");
            return (ucBattTemp == 0xff) ? "No Data" : (ucBattTemp == 0xfe) ? "Out of range" : (ucBattTemp == 0xfd) ? "Reserved" : (buffer);
        }
    }
} SccBattSts;

// Auto-generated class definition for RVC_tzPGN_SCC_STS6
typedef struct SccSts6 {
    uchar8 ucInst;
    uint16 uiOperatingDays;
    uint16 uiIntTemp;
    void Print() const {
        std::cout << "PGN: SccSts6" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiOperatingDays: " << uiOperatingDays << std::endl;
        std::cout << "uiIntTemp: " << uiIntTemp << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "OperatingDays") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiOperatingDays + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "days");
            return (uiOperatingDays == 0xffff) ? "No Data" : (uiOperatingDays == 0xfffe) ? "Out of range" : (uiOperatingDays == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "IntTemp") {
            char buffer[50];
            auto engval = 0.03125 * (float) (uiIntTemp + -8736.0);
            snprintf(buffer, sizeof(buffer), "%.2f %s", engval, "deg C");
            return (uiIntTemp == 0xffff) ? "No Data" : (uiIntTemp == 0xfffe) ? "Out of range" : (uiIntTemp == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} SccSts6;

// Auto-generated class definition for RVC_tzPGN_SCC_STS5
typedef struct SccSts5 {
    uchar8 ucInst;
    uint16 uiWeekAh;
    uint32 ulCumPwrGen;
    void Print() const {
        std::cout << "PGN: SccSts5" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiWeekAh: " << uiWeekAh << std::endl;
        std::cout << "ulCumPwrGen: " << ulCumPwrGen << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "WeekAh") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiWeekAh + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "AH");
            return (uiWeekAh == 0xffff) ? "No Data" : (uiWeekAh == 0xfffe) ? "Out of range" : (uiWeekAh == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "CumPwrGen") {
            char buffer[50];
            auto engval = 1.0 * (float) (ulCumPwrGen + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "kWh");
            return (ulCumPwrGen == 0xffffffff) ? "No Data" : (ulCumPwrGen == 0xfffffffe) ? "Out of range" : (ulCumPwrGen == 0xfffffffd) ? "Reserved" : (buffer);
        }
    }
} SccSts5;

// Auto-generated class definition for RVC_tzPGN_SCC_STS4
typedef struct SccSts4 {
    uchar8 ucInst;
    uint16 uiTodayAh;
    uint16 uiYesterdayAh;
    uint16 uiBeforeYesterdayAh;
    void Print() const {
        std::cout << "PGN: SccSts4" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiTodayAh: " << uiTodayAh << std::endl;
        std::cout << "uiYesterdayAh: " << uiYesterdayAh << std::endl;
        std::cout << "uiBeforeYesterdayAh: " << uiBeforeYesterdayAh << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "TodayAh") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiTodayAh + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "AH");
            return (uiTodayAh == 0xffff) ? "No Data" : (uiTodayAh == 0xfffe) ? "Out of range" : (uiTodayAh == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "YesterdayAh") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiYesterdayAh + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "AH");
            return (uiYesterdayAh == 0xffff) ? "No Data" : (uiYesterdayAh == 0xfffe) ? "Out of range" : (uiYesterdayAh == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "BeforeYesterdayAh") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiBeforeYesterdayAh + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "AH");
            return (uiBeforeYesterdayAh == 0xffff) ? "No Data" : (uiBeforeYesterdayAh == 0xfffe) ? "Out of range" : (uiBeforeYesterdayAh == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} SccSts4;

// Auto-generated class definition for RVC_tzPGN_SCC_STS3
typedef struct SccSts3 {
    uchar8 ucInst;
    uint16 uiRatedPvV;
    uint16 uiRatedPvI;
    uint16 uiRatedPvOverPwr;
    void Print() const {
        std::cout << "PGN: SccSts3" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiRatedPvV: " << uiRatedPvV << std::endl;
        std::cout << "uiRatedPvI: " << uiRatedPvI << std::endl;
        std::cout << "uiRatedPvOverPwr: " << uiRatedPvOverPwr << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "RatedPvV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiRatedPvV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiRatedPvV == 0xffff) ? "No Data" : (uiRatedPvV == 0xfffe) ? "Out of range" : (uiRatedPvV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "RatedPvI") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiRatedPvI + -32000.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "A");
            return (uiRatedPvI == 0xffff) ? "No Data" : (uiRatedPvI == 0xfffe) ? "Out of range" : (uiRatedPvI == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "RatedPvOverPwr") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiRatedPvOverPwr + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "W");
            return (uiRatedPvOverPwr == 0xffff) ? "No Data" : (uiRatedPvOverPwr == 0xfffe) ? "Out of range" : (uiRatedPvOverPwr == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} SccSts3;

// Auto-generated class definition for RVC_tzPGN_SCC_STS2
typedef struct SccSts2 {
    uchar8 ucInst;
    uint16 uiRatedBattV;
    uint16 uiRatedBattI;
    void Print() const {
        std::cout << "PGN: SccSts2" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiRatedBattV: " << uiRatedBattV << std::endl;
        std::cout << "uiRatedBattI: " << uiRatedBattI << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "RatedBattV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiRatedBattV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiRatedBattV == 0xffff) ? "No Data" : (uiRatedBattV == 0xfffe) ? "Out of range" : (uiRatedBattV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "RatedBattI") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiRatedBattI + -32000.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "A");
            return (uiRatedBattI == 0xffff) ? "No Data" : (uiRatedBattI == 0xfffe) ? "Out of range" : (uiRatedBattI == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} SccSts2;

// Auto-generated class definition for RVC_tzPGN_BATT_CMD
typedef struct BattCmd {
    RVC_teDC_SRC_ID teBattInst;
    RVC_teGENERIC_STSPAIR teDesiredLoadOnOffSts;
    RVC_teGENERIC_STSPAIR teDesiredChgOnOffSts;
    RVC_teGENERIC_STSPAIR teClrHistory;
    RVC_teGENERIC_STSPAIR teReserved;
    uchar8 ucRtnModuleCellDetail;
    RVC_teDC_SRC_ID teDcInst;
    void Print() const {
        std::cout << "PGN: BattCmd" << std::endl;
        std::cout << "teBattInst: " << teBattInst << std::endl;
        std::cout << "teDesiredLoadOnOffSts: " << teDesiredLoadOnOffSts << std::endl;
        std::cout << "teDesiredChgOnOffSts: " << teDesiredChgOnOffSts << std::endl;
        std::cout << "teClrHistory: " << teClrHistory << std::endl;
        std::cout << "teReserved: " << teReserved << std::endl;
        std::cout << "ucRtnModuleCellDetail: " << static_cast<int>(ucRtnModuleCellDetail) << std::endl;
        std::cout << "teDcInst: " << teDcInst << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "BattInst") {
        }
        if (mnem == "DesiredLoadOnOffSts") {
        }
        if (mnem == "DesiredChgOnOffSts") {
        }
        if (mnem == "ClrHistory") {
        }
        if (mnem == "Reserved") {
        }
        if (mnem == "RtnModuleCellDetail") {
            return (ucRtnModuleCellDetail == 0xff) ? "No Data" : (ucRtnModuleCellDetail == 0xfe) ? "Out of range" : (ucRtnModuleCellDetail == 0xfd) ? "Reserved" : std::to_string(ucRtnModuleCellDetail);
        }
        if (mnem == "DcInst") {
        }
    }
} BattCmd;

// Auto-generated class definition for RVC_tzPGN_BATT_STS11
typedef struct BattSts11 {
    RVC_teDC_SRC_ID teBattInst;
    RVC_teDC_SRC_ID teDcInst;
    RVC_teGENERIC_STSPAIR teDischgOnOffSts;
    RVC_teGENERIC_STSPAIR teChgOnOffSts;
    RVC_teGENERIC_STSPAIR teChgDetect;
    RVC_teGENERIC_STSPAIR teReservSts;
    uint16 uiNomFullCap;
    uint16 uiDcPwr;
    void Print() const {
        std::cout << "PGN: BattSts11" << std::endl;
        std::cout << "teBattInst: " << teBattInst << std::endl;
        std::cout << "teDcInst: " << teDcInst << std::endl;
        std::cout << "teDischgOnOffSts: " << teDischgOnOffSts << std::endl;
        std::cout << "teChgOnOffSts: " << teChgOnOffSts << std::endl;
        std::cout << "teChgDetect: " << teChgDetect << std::endl;
        std::cout << "teReservSts: " << teReservSts << std::endl;
        std::cout << "uiNomFullCap: " << uiNomFullCap << std::endl;
        std::cout << "uiDcPwr: " << uiDcPwr << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "BattInst") {
        }
        if (mnem == "DcInst") {
        }
        if (mnem == "DischgOnOffSts") {
        }
        if (mnem == "ChgOnOffSts") {
        }
        if (mnem == "ChgDetect") {
        }
        if (mnem == "ReservSts") {
        }
        if (mnem == "NomFullCap") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiNomFullCap + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "AH");
            return (uiNomFullCap == 0xffff) ? "No Data" : (uiNomFullCap == 0xfffe) ? "Out of range" : (uiNomFullCap == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "DcPwr") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiDcPwr + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "W");
            return (uiDcPwr == 0xffff) ? "No Data" : (uiDcPwr == 0xfffe) ? "Out of range" : (uiDcPwr == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} BattSts11;

// Auto-generated class definition for RVC_tzPGN_BATT_STS6
typedef struct BattSts6 {
    RVC_teDC_SRC_ID teBattInst;
    RVC_teDC_SRC_ID teDcInst;
    RVC_teGENERIC_STSPAIR teHiVoltLimitSts;
    RVC_teDISCONNECT_STS teHiVoltDisconSts;
    RVC_teGENERIC_STSPAIR teLowVoltLimitSts;
    RVC_teDISCONNECT_STS teLowVoltDisconSts;
    RVC_teGENERIC_STSPAIR teLowSocLimitSts;
    RVC_teDISCONNECT_STS teLowSocDisconSts;
    RVC_teGENERIC_STSPAIR teLowDcSrcTempLimitSts;
    RVC_teDISCONNECT_STS teLowDcSrcTempDisconSts;
    RVC_teGENERIC_STSPAIR teHiDcSrcTempLimitSts;
    RVC_teDISCONNECT_STS teHiDcSrcTempDisconSts;
    RVC_teGENERIC_STSPAIR teHiAmpDcSrcLimitSts;
    RVC_teDISCONNECT_STS teHiAmpDcSrcDisconSts;
    void Print() const {
        std::cout << "PGN: BattSts6" << std::endl;
        std::cout << "teBattInst: " << teBattInst << std::endl;
        std::cout << "teDcInst: " << teDcInst << std::endl;
        std::cout << "teHiVoltLimitSts: " << teHiVoltLimitSts << std::endl;
        std::cout << "teHiVoltDisconSts: " << teHiVoltDisconSts << std::endl;
        std::cout << "teLowVoltLimitSts: " << teLowVoltLimitSts << std::endl;
        std::cout << "teLowVoltDisconSts: " << teLowVoltDisconSts << std::endl;
        std::cout << "teLowSocLimitSts: " << teLowSocLimitSts << std::endl;
        std::cout << "teLowSocDisconSts: " << teLowSocDisconSts << std::endl;
        std::cout << "teLowDcSrcTempLimitSts: " << teLowDcSrcTempLimitSts << std::endl;
        std::cout << "teLowDcSrcTempDisconSts: " << teLowDcSrcTempDisconSts << std::endl;
        std::cout << "teHiDcSrcTempLimitSts: " << teHiDcSrcTempLimitSts << std::endl;
        std::cout << "teHiDcSrcTempDisconSts: " << teHiDcSrcTempDisconSts << std::endl;
        std::cout << "teHiAmpDcSrcLimitSts: " << teHiAmpDcSrcLimitSts << std::endl;
        std::cout << "teHiAmpDcSrcDisconSts: " << teHiAmpDcSrcDisconSts << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "BattInst") {
        }
        if (mnem == "DcInst") {
        }
        if (mnem == "HiVoltLimitSts") {
        }
        if (mnem == "HiVoltDisconSts") {
        }
        if (mnem == "LowVoltLimitSts") {
        }
        if (mnem == "LowVoltDisconSts") {
        }
        if (mnem == "LowSocLimitSts") {
        }
        if (mnem == "LowSocDisconSts") {
        }
        if (mnem == "LowDcSrcTempLimitSts") {
        }
        if (mnem == "LowDcSrcTempDisconSts") {
        }
        if (mnem == "HiDcSrcTempLimitSts") {
        }
        if (mnem == "HiDcSrcTempDisconSts") {
        }
        if (mnem == "HiAmpDcSrcLimitSts") {
        }
        if (mnem == "HiAmpDcSrcDisconSts") {
        }
    }
} BattSts6;

// Auto-generated class definition for RVC_tzPGN_BATT_STS4
typedef struct BattSts4 {
    RVC_teDC_SRC_ID teBattInst;
    RVC_teDC_SRC_ID teDcInst;
    RVC_teRVC_DESIRED_CHG_STATE teDesChgState;
    uint16 uiDesDcV;
    uint16 uiDesDcI;
    RVC_teRVC_BATT_TYPE teBattType;
    void Print() const {
        std::cout << "PGN: BattSts4" << std::endl;
        std::cout << "teBattInst: " << teBattInst << std::endl;
        std::cout << "teDcInst: " << teDcInst << std::endl;
        std::cout << "teDesChgState: " << teDesChgState << std::endl;
        std::cout << "uiDesDcV: " << uiDesDcV << std::endl;
        std::cout << "uiDesDcI: " << uiDesDcI << std::endl;
        std::cout << "teBattType: " << teBattType << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "BattInst") {
        }
        if (mnem == "DcInst") {
        }
        if (mnem == "DesChgState") {
        }
        if (mnem == "DesDcV") {
            return (uiDesDcV == 0xffff) ? "No Data" : (uiDesDcV == 0xfffe) ? "Out of range" : (uiDesDcV == 0xfffd) ? "Reserved" : std::to_string(uiDesDcV);
        }
        if (mnem == "DesDcI") {
            return (uiDesDcI == 0xffff) ? "No Data" : (uiDesDcI == 0xfffe) ? "Out of range" : (uiDesDcI == 0xfffd) ? "Reserved" : std::to_string(uiDesDcI);
        }
        if (mnem == "BattType") {
        }
    }
} BattSts4;

// Auto-generated class definition for RVC_tzPGN_BATT_STS3
typedef struct BattSts3 {
    RVC_teDC_SRC_ID teBattInst;
    RVC_teDC_SRC_ID teDcInst;
    uchar8 ucSoh;
    uint16 uiCapRemain;
    uchar8 ucRelCap;
    uint16 uiAcRmsRipple;
    void Print() const {
        std::cout << "PGN: BattSts3" << std::endl;
        std::cout << "teBattInst: " << teBattInst << std::endl;
        std::cout << "teDcInst: " << teDcInst << std::endl;
        std::cout << "ucSoh: " << static_cast<int>(ucSoh) << std::endl;
        std::cout << "uiCapRemain: " << uiCapRemain << std::endl;
        std::cout << "ucRelCap: " << static_cast<int>(ucRelCap) << std::endl;
        std::cout << "uiAcRmsRipple: " << uiAcRmsRipple << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "BattInst") {
        }
        if (mnem == "DcInst") {
        }
        if (mnem == "Soh") {
            char buffer[50];
            auto engval = 0.5 * (float) (ucSoh + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "%");
            return (ucSoh == 0xff) ? "No Data" : (ucSoh == 0xfe) ? "Out of range" : (ucSoh == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "CapRemain") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiCapRemain + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "AH");
            return (uiCapRemain == 0xffff) ? "No Data" : (uiCapRemain == 0xfffe) ? "Out of range" : (uiCapRemain == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "RelCap") {
            char buffer[50];
            auto engval = 0.5 * (float) (ucRelCap + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "%");
            return (ucRelCap == 0xff) ? "No Data" : (ucRelCap == 0xfe) ? "Out of range" : (ucRelCap == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "AcRmsRipple") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiAcRmsRipple + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "mV");
            return (uiAcRmsRipple == 0xffff) ? "No Data" : (uiAcRmsRipple == 0xfffe) ? "Out of range" : (uiAcRmsRipple == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} BattSts3;

// Auto-generated class definition for RVC_tzPGN_BATT_STS2
typedef struct BattSts2 {
    RVC_teDC_SRC_ID teBattInst;
    RVC_teDC_SRC_ID teDcInst;
    uint16 uiSrcTemp;
    uchar8 ucSoc;
    uint16 uiTimeRemain;
    RVC_teTIME_REMAIN_INTERP teTimeRemainInterp;
    void Print() const {
        std::cout << "PGN: BattSts2" << std::endl;
        std::cout << "teBattInst: " << teBattInst << std::endl;
        std::cout << "teDcInst: " << teDcInst << std::endl;
        std::cout << "uiSrcTemp: " << uiSrcTemp << std::endl;
        std::cout << "ucSoc: " << static_cast<int>(ucSoc) << std::endl;
        std::cout << "uiTimeRemain: " << uiTimeRemain << std::endl;
        std::cout << "teTimeRemainInterp: " << teTimeRemainInterp << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "BattInst") {
        }
        if (mnem == "DcInst") {
        }
        if (mnem == "SrcTemp") {
            char buffer[50];
            auto engval = 0.03125 * (float) (uiSrcTemp + -8736.0);
            snprintf(buffer, sizeof(buffer), "%.2f %s", engval, "deg C");
            return (uiSrcTemp == 0xffff) ? "No Data" : (uiSrcTemp == 0xfffe) ? "Out of range" : (uiSrcTemp == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "Soc") {
            char buffer[50];
            auto engval = 0.5 * (float) (ucSoc + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "%");
            return (ucSoc == 0xff) ? "No Data" : (ucSoc == 0xfe) ? "Out of range" : (ucSoc == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "TimeRemain") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiTimeRemain + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "Min");
            return (uiTimeRemain == 0xffff) ? "No Data" : (uiTimeRemain == 0xfffe) ? "Out of range" : (uiTimeRemain == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "TimeRemainInterp") {
        }
    }
} BattSts2;

// Auto-generated class definition for RVC_tzPGN_BATT_STS1
typedef struct BattSts1 {
    RVC_teDC_SRC_ID teBattInst;
    RVC_teDC_SRC_ID teDcInst;
    uint16 uiDcV;
    uint32 ulDcI;
    void Print() const {
        std::cout << "PGN: BattSts1" << std::endl;
        std::cout << "teBattInst: " << teBattInst << std::endl;
        std::cout << "teDcInst: " << teDcInst << std::endl;
        std::cout << "uiDcV: " << uiDcV << std::endl;
        std::cout << "ulDcI: " << ulDcI << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "BattInst") {
        }
        if (mnem == "DcInst") {
        }
        if (mnem == "DcV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiDcV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiDcV == 0xffff) ? "No Data" : (uiDcV == 0xfffe) ? "Out of range" : (uiDcV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "DcI") {
            char buffer[50];
            auto engval = 0.0010000000474974513 * (float) (ulDcI + -2000000000.0);
            snprintf(buffer, sizeof(buffer), "%.3f %s", engval, "A");
            return (ulDcI == 0xffffffff) ? "No Data" : (ulDcI == 0xfffffffe) ? "Out of range" : (ulDcI == 0xfffffffd) ? "Reserved" : (buffer);
        }
    }
} BattSts1;

// Auto-generated class definition for RVC_tzPGN_INV_CFG_CMD4
typedef struct InvCfgCmd4 {
    uchar8 ucInst;
    uint16 uiOutACVolt;
    uchar8 ucOutFreq;
    uint16 uiACOutPwrLim;
    uint16 uiACOutPwrTimeLim;
    void Print() const {
        std::cout << "PGN: InvCfgCmd4" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiOutACVolt: " << uiOutACVolt << std::endl;
        std::cout << "ucOutFreq: " << static_cast<int>(ucOutFreq) << std::endl;
        std::cout << "uiACOutPwrLim: " << uiACOutPwrLim << std::endl;
        std::cout << "uiACOutPwrTimeLim: " << uiACOutPwrTimeLim << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "OutACVolt") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiOutACVolt + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiOutACVolt == 0xffff) ? "No Data" : (uiOutACVolt == 0xfffe) ? "Out of range" : (uiOutACVolt == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "OutFreq") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucOutFreq + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "Hz");
            return (ucOutFreq == 0xff) ? "No Data" : (ucOutFreq == 0xfe) ? "Out of range" : (ucOutFreq == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "ACOutPwrLim") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiACOutPwrLim + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "W");
            return (uiACOutPwrLim == 0xffff) ? "No Data" : (uiACOutPwrLim == 0xfffe) ? "Out of range" : (uiACOutPwrLim == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "ACOutPwrTimeLim") {
            char buffer[50];
            auto engval = 0.5 * (float) (uiACOutPwrTimeLim + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "s");
            return (uiACOutPwrTimeLim == 0xffff) ? "No Data" : (uiACOutPwrTimeLim == 0xfffe) ? "Out of range" : (uiACOutPwrTimeLim == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} InvCfgCmd4;

// Auto-generated class definition for RVC_tzPGN_INV_CFG_STS4
typedef struct InvCfgSts4 {
    uchar8 ucInst;
    uint16 uiOutACVolt;
    uchar8 ucOutFreq;
    uint16 uiACOutPwrLim;
    uint16 uiACOutPwrTimeLim;
    void Print() const {
        std::cout << "PGN: InvCfgSts4" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiOutACVolt: " << uiOutACVolt << std::endl;
        std::cout << "ucOutFreq: " << static_cast<int>(ucOutFreq) << std::endl;
        std::cout << "uiACOutPwrLim: " << uiACOutPwrLim << std::endl;
        std::cout << "uiACOutPwrTimeLim: " << uiACOutPwrTimeLim << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "OutACVolt") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiOutACVolt + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiOutACVolt == 0xffff) ? "No Data" : (uiOutACVolt == 0xfffe) ? "Out of range" : (uiOutACVolt == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "OutFreq") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucOutFreq + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "Hz");
            return (ucOutFreq == 0xff) ? "No Data" : (ucOutFreq == 0xfe) ? "Out of range" : (ucOutFreq == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "ACOutPwrLim") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiACOutPwrLim + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "W");
            return (uiACOutPwrLim == 0xffff) ? "No Data" : (uiACOutPwrLim == 0xfffe) ? "Out of range" : (uiACOutPwrLim == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "ACOutPwrTimeLim") {
            char buffer[50];
            auto engval = 0.5 * (float) (uiACOutPwrTimeLim + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "s");
            return (uiACOutPwrTimeLim == 0xffff) ? "No Data" : (uiACOutPwrTimeLim == 0xfffe) ? "Out of range" : (uiACOutPwrTimeLim == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} InvCfgSts4;

// Auto-generated class definition for RVC_tzPGN_ALARM_CMD
typedef struct AlarmCmd {
    uchar8 ucInst;
    RVC_teDFLT_SRC_ADDR teDevType;
    RVC_teCMD_PAIR teReadyAlarm;
    RVC_teCMD_PAIR teAckAlarm;
    RVC_teCMD_PAIR teAutoRstEn;
    uchar8 ucDevInst;
    void Print() const {
        std::cout << "PGN: AlarmCmd" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teDevType: " << teDevType << std::endl;
        std::cout << "teReadyAlarm: " << teReadyAlarm << std::endl;
        std::cout << "teAckAlarm: " << teAckAlarm << std::endl;
        std::cout << "teAutoRstEn: " << teAutoRstEn << std::endl;
        std::cout << "ucDevInst: " << static_cast<int>(ucDevInst) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "DevType") {
        }
        if (mnem == "ReadyAlarm") {
        }
        if (mnem == "AckAlarm") {
        }
        if (mnem == "AutoRstEn") {
        }
        if (mnem == "DevInst") {
        }
    }
} AlarmCmd;

// Auto-generated class definition for RVC_tzPGN_ALARM_STS
typedef struct AlarmSts {
    uchar8 ucInst;
    RVC_teDFLT_SRC_ADDR teDevType;
    RVC_teGENERIC_STSPAIR teAlarmTrig;
    RVC_teGENERIC_STSPAIR teAlarmRdy;
    RVC_teGENERIC_STSPAIR teAlarmAckd;
    RVC_teGENERIC_STSPAIR teAlarmRstEn;
    uint16 uiElapsedTime;
    uchar8 ucDevInst;
    RVC_teGENERIC_STSPAIR teFtMonReq;
    RVC_teGENERIC_STSPAIR teAlarmForLog;
    RVC_teGENERIC_STSPAIR teAlarmForUser;
    void Print() const {
        std::cout << "PGN: AlarmSts" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teDevType: " << teDevType << std::endl;
        std::cout << "teAlarmTrig: " << teAlarmTrig << std::endl;
        std::cout << "teAlarmRdy: " << teAlarmRdy << std::endl;
        std::cout << "teAlarmAckd: " << teAlarmAckd << std::endl;
        std::cout << "teAlarmRstEn: " << teAlarmRstEn << std::endl;
        std::cout << "uiElapsedTime: " << uiElapsedTime << std::endl;
        std::cout << "ucDevInst: " << static_cast<int>(ucDevInst) << std::endl;
        std::cout << "teFtMonReq: " << teFtMonReq << std::endl;
        std::cout << "teAlarmForLog: " << teAlarmForLog << std::endl;
        std::cout << "teAlarmForUser: " << teAlarmForUser << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "DevType") {
        }
        if (mnem == "AlarmTrig") {
        }
        if (mnem == "AlarmRdy") {
        }
        if (mnem == "AlarmAckd") {
        }
        if (mnem == "AlarmRstEn") {
        }
        if (mnem == "ElapsedTime") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiElapsedTime + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "Min");
            return (uiElapsedTime == 0xffff) ? "No Data" : (uiElapsedTime == 0xfffe) ? "Out of range" : (uiElapsedTime == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "DevInst") {
        }
        if (mnem == "FtMonReq") {
        }
        if (mnem == "AlarmForLog") {
        }
        if (mnem == "AlarmForUser") {
        }
    }
} AlarmSts;

// Auto-generated class definition for RVC_tzPGN_CHG_STS2
typedef struct ChgSts2 {
    uchar8 ucChgInst;
    uchar8 ucDcSrcInst;
    RVC_teRVC_DEVICE_PRIORITY teChgPrio;
    uint16 uiChgV;
    uint16 uiChgI;
    uchar8 ucChgTemp;
    void Print() const {
        std::cout << "PGN: ChgSts2" << std::endl;
        std::cout << "ucChgInst: " << static_cast<int>(ucChgInst) << std::endl;
        std::cout << "ucDcSrcInst: " << static_cast<int>(ucDcSrcInst) << std::endl;
        std::cout << "teChgPrio: " << teChgPrio << std::endl;
        std::cout << "uiChgV: " << uiChgV << std::endl;
        std::cout << "uiChgI: " << uiChgI << std::endl;
        std::cout << "ucChgTemp: " << static_cast<int>(ucChgTemp) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "ChgInst") {
            return (ucChgInst == 0xff) ? "No Data" : (ucChgInst == 0xfe) ? "Out of range" : (ucChgInst == 0xfd) ? "Reserved" : std::to_string(ucChgInst);
        }
        if (mnem == "DcSrcInst") {
            return (ucDcSrcInst == 0xff) ? "No Data" : (ucDcSrcInst == 0xfe) ? "Out of range" : (ucDcSrcInst == 0xfd) ? "Reserved" : std::to_string(ucDcSrcInst);
        }
        if (mnem == "ChgPrio") {
        }
        if (mnem == "ChgV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiChgV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiChgV == 0xffff) ? "No Data" : (uiChgV == 0xfffe) ? "Out of range" : (uiChgV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "ChgI") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiChgI + -32000.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "A");
            return (uiChgI == 0xffff) ? "No Data" : (uiChgI == 0xfffe) ? "Out of range" : (uiChgI == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "ChgTemp") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucChgTemp + -40.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "deg C");
            return (ucChgTemp == 0xff) ? "No Data" : (ucChgTemp == 0xfe) ? "Out of range" : (ucChgTemp == 0xfd) ? "Reserved" : (buffer);
        }
    }
} ChgSts2;

// Auto-generated class definition for RVC_tzPGN_DC_SRC_CMD
typedef struct DcSrcCmd {
    RVC_teDC_SRC_ID teInst;
    RVC_teGENERIC_STSPAIR teDesPwrOnOffSts;
    RVC_teGENERIC_STSPAIR teDesChgOnOffSts;
    void Print() const {
        std::cout << "PGN: DcSrcCmd" << std::endl;
        std::cout << "teInst: " << teInst << std::endl;
        std::cout << "teDesPwrOnOffSts: " << teDesPwrOnOffSts << std::endl;
        std::cout << "teDesChgOnOffSts: " << teDesChgOnOffSts << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "DesPwrOnOffSts") {
        }
        if (mnem == "DesChgOnOffSts") {
        }
    }
} DcSrcCmd;

// Auto-generated class definition for RVC_tzPGN_DC_SRC_STS11
typedef struct DcSrcSts11 {
    RVC_teDC_SRC_ID teInst;
    RVC_teRVC_DEVICE_PRIORITY teDevPri;
    RVC_teGENERIC_STSPAIR teDischgOnOffSts;
    RVC_teGENERIC_STSPAIR teChgOnOffSts;
    RVC_teGENERIC_STSPAIR teChgDetect;
    RVC_teGENERIC_STSPAIR teReservSts;
    uint16 uiNomFullCap;
    uint16 uiDcPwr;
    void Print() const {
        std::cout << "PGN: DcSrcSts11" << std::endl;
        std::cout << "teInst: " << teInst << std::endl;
        std::cout << "teDevPri: " << teDevPri << std::endl;
        std::cout << "teDischgOnOffSts: " << teDischgOnOffSts << std::endl;
        std::cout << "teChgOnOffSts: " << teChgOnOffSts << std::endl;
        std::cout << "teChgDetect: " << teChgDetect << std::endl;
        std::cout << "teReservSts: " << teReservSts << std::endl;
        std::cout << "uiNomFullCap: " << uiNomFullCap << std::endl;
        std::cout << "uiDcPwr: " << uiDcPwr << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "DevPri") {
        }
        if (mnem == "DischgOnOffSts") {
        }
        if (mnem == "ChgOnOffSts") {
        }
        if (mnem == "ChgDetect") {
        }
        if (mnem == "ReservSts") {
        }
        if (mnem == "NomFullCap") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiNomFullCap + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "AH");
            return (uiNomFullCap == 0xffff) ? "No Data" : (uiNomFullCap == 0xfffe) ? "Out of range" : (uiNomFullCap == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "DcPwr") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiDcPwr + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "W");
            return (uiDcPwr == 0xffff) ? "No Data" : (uiDcPwr == 0xfffe) ? "Out of range" : (uiDcPwr == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} DcSrcSts11;

// Auto-generated class definition for RVC_tzPGN_SCC_EQLZ_CFG_CMD
typedef struct SccEqlzCfgCmd {
    uchar8 ucInst;
    uint16 uiEqlzV;
    uint16 uiEqlzTime;
    uchar8 ucEqlzIntvl;
    void Print() const {
        std::cout << "PGN: SccEqlzCfgCmd" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiEqlzV: " << uiEqlzV << std::endl;
        std::cout << "uiEqlzTime: " << uiEqlzTime << std::endl;
        std::cout << "ucEqlzIntvl: " << static_cast<int>(ucEqlzIntvl) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "EqlzV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiEqlzV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiEqlzV == 0xffff) ? "No Data" : (uiEqlzV == 0xfffe) ? "Out of range" : (uiEqlzV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "EqlzTime") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiEqlzTime + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "Min");
            return (uiEqlzTime == 0xffff) ? "No Data" : (uiEqlzTime == 0xfffe) ? "Out of range" : (uiEqlzTime == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "EqlzIntvl") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucEqlzIntvl + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "days");
            return (ucEqlzIntvl == 0xff) ? "No Data" : (ucEqlzIntvl == 0xfe) ? "Out of range" : (ucEqlzIntvl == 0xfd) ? "Reserved" : (buffer);
        }
    }
} SccEqlzCfgCmd;

// Auto-generated class definition for RVC_tzPGN_SCC_EQLZ_CFG_STS
typedef struct SccEqlzCfgSts {
    uchar8 ucInst;
    uint16 uiEqlzV;
    uint16 uiEqlzTime;
    uchar8 ucEqlzIntvl;
    void Print() const {
        std::cout << "PGN: SccEqlzCfgSts" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiEqlzV: " << uiEqlzV << std::endl;
        std::cout << "uiEqlzTime: " << uiEqlzTime << std::endl;
        std::cout << "ucEqlzIntvl: " << static_cast<int>(ucEqlzIntvl) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "EqlzV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiEqlzV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiEqlzV == 0xffff) ? "No Data" : (uiEqlzV == 0xfffe) ? "Out of range" : (uiEqlzV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "EqlzTime") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiEqlzTime + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "Min");
            return (uiEqlzTime == 0xffff) ? "No Data" : (uiEqlzTime == 0xfffe) ? "Out of range" : (uiEqlzTime == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "EqlzIntvl") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucEqlzIntvl + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "days");
            return (ucEqlzIntvl == 0xff) ? "No Data" : (ucEqlzIntvl == 0xfe) ? "Out of range" : (ucEqlzIntvl == 0xfd) ? "Reserved" : (buffer);
        }
    }
} SccEqlzCfgSts;

// Auto-generated class definition for RVC_tzPGN_SCC_EQLZ_STS
typedef struct SccEqlzSts {
    uchar8 ucInst;
    uint16 uiTimeRemain;
    RVC_teGENERIC_STSPAIR tePreChgSts;
    uchar8 ucTimeSinceEqlz;
    void Print() const {
        std::cout << "PGN: SccEqlzSts" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiTimeRemain: " << uiTimeRemain << std::endl;
        std::cout << "tePreChgSts: " << tePreChgSts << std::endl;
        std::cout << "ucTimeSinceEqlz: " << static_cast<int>(ucTimeSinceEqlz) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "TimeRemain") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiTimeRemain + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "Min");
            return (uiTimeRemain == 0xffff) ? "No Data" : (uiTimeRemain == 0xfffe) ? "Out of range" : (uiTimeRemain == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "PreChgSts") {
        }
        if (mnem == "TimeSinceEqlz") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucTimeSinceEqlz + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "days");
            return (ucTimeSinceEqlz == 0xff) ? "No Data" : (ucTimeSinceEqlz == 0xfe) ? "Out of range" : (ucTimeSinceEqlz == 0xfd) ? "Reserved" : (buffer);
        }
    }
} SccEqlzSts;

// Auto-generated class definition for RVC_tzPGN_SCC_CFG_CMD
typedef struct SccCfgCmd {
    uchar8 ucInst;
    RVC_teRVC_CHG_ALG teChgAlg;
    RVC_teRVC_CHG_MODE teChgMode;
    RVC_teCMD_PAIR teBattSensPres;
    RVC_teCMD_PAIR teLinkageMode;
    RVC_teRVC_BATT_TYPE teBattType;
    uint16 uiBattBankSz;
    uchar8 ucMaxChgI;
    void Print() const {
        std::cout << "PGN: SccCfgCmd" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teChgAlg: " << teChgAlg << std::endl;
        std::cout << "teChgMode: " << teChgMode << std::endl;
        std::cout << "teBattSensPres: " << teBattSensPres << std::endl;
        std::cout << "teLinkageMode: " << teLinkageMode << std::endl;
        std::cout << "teBattType: " << teBattType << std::endl;
        std::cout << "uiBattBankSz: " << uiBattBankSz << std::endl;
        std::cout << "ucMaxChgI: " << static_cast<int>(ucMaxChgI) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "ChgAlg") {
        }
        if (mnem == "ChgMode") {
        }
        if (mnem == "BattSensPres") {
        }
        if (mnem == "LinkageMode") {
        }
        if (mnem == "BattType") {
        }
        if (mnem == "BattBankSz") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiBattBankSz + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "AH");
            return (uiBattBankSz == 0xffff) ? "No Data" : (uiBattBankSz == 0xfffe) ? "Out of range" : (uiBattBankSz == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "MaxChgI") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucMaxChgI + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "A");
            return (ucMaxChgI == 0xff) ? "No Data" : (ucMaxChgI == 0xfe) ? "Out of range" : (ucMaxChgI == 0xfd) ? "Reserved" : (buffer);
        }
    }
} SccCfgCmd;

// Auto-generated class definition for RVC_tzPGN_SCC_CMD
typedef struct SccCmd {
    uchar8 ucInst;
    RVC_teRVC_CHG_CMD teSts;
    RVC_teCMD_PAIR teEnableOnPwrUp;
    RVC_teCMD_PAIR teClrHist;
    RVC_teRVC_FORCE_CHG teForceChg;
    void Print() const {
        std::cout << "PGN: SccCmd" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teSts: " << teSts << std::endl;
        std::cout << "teEnableOnPwrUp: " << teEnableOnPwrUp << std::endl;
        std::cout << "teClrHist: " << teClrHist << std::endl;
        std::cout << "teForceChg: " << teForceChg << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "Sts") {
        }
        if (mnem == "EnableOnPwrUp") {
        }
        if (mnem == "ClrHist") {
        }
        if (mnem == "ForceChg") {
        }
    }
} SccCmd;

// Auto-generated class definition for RVC_tzPGN_SCC_CFG_STS
typedef struct SccCfgSts {
    uchar8 ucInst;
    RVC_teRVC_CHG_ALG teChgAlg;
    RVC_teRVC_CHG_MODE teChgMode;
    RVC_teGENERIC_STSPAIR teBattSensPres;
    RVC_teGENERIC_STSPAIR teLinkMode;
    RVC_teRVC_BATT_TYPE teBattType;
    uint16 uiBattBankSz;
    uchar8 ucMaxChgI;
    void Print() const {
        std::cout << "PGN: SccCfgSts" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teChgAlg: " << teChgAlg << std::endl;
        std::cout << "teChgMode: " << teChgMode << std::endl;
        std::cout << "teBattSensPres: " << teBattSensPres << std::endl;
        std::cout << "teLinkMode: " << teLinkMode << std::endl;
        std::cout << "teBattType: " << teBattType << std::endl;
        std::cout << "uiBattBankSz: " << uiBattBankSz << std::endl;
        std::cout << "ucMaxChgI: " << static_cast<int>(ucMaxChgI) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "ChgAlg") {
        }
        if (mnem == "ChgMode") {
        }
        if (mnem == "BattSensPres") {
        }
        if (mnem == "LinkMode") {
        }
        if (mnem == "BattType") {
        }
        if (mnem == "BattBankSz") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiBattBankSz + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "AH");
            return (uiBattBankSz == 0xffff) ? "No Data" : (uiBattBankSz == 0xfffe) ? "Out of range" : (uiBattBankSz == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "MaxChgI") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucMaxChgI + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "A");
            return (ucMaxChgI == 0xff) ? "No Data" : (ucMaxChgI == 0xfe) ? "Out of range" : (ucMaxChgI == 0xfd) ? "Reserved" : (buffer);
        }
    }
} SccCfgSts;

// Auto-generated class definition for RVC_tzPGN_SCC_STS
typedef struct SccSts {
    uchar8 ucInst;
    uint16 uiChgV;
    uint16 uiChgI;
    uchar8 ucChgIPer;
    RVC_teRVC_DESIRED_CHG_STATE teOpState;
    RVC_teGENERIC_STSPAIR tePwrUpEnDis;
    RVC_teGENERIC_STSPAIR teClrHist;
    RVC_teRVC_FORCE_CHG teForceChg;
    void Print() const {
        std::cout << "PGN: SccSts" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiChgV: " << uiChgV << std::endl;
        std::cout << "uiChgI: " << uiChgI << std::endl;
        std::cout << "ucChgIPer: " << static_cast<int>(ucChgIPer) << std::endl;
        std::cout << "teOpState: " << teOpState << std::endl;
        std::cout << "tePwrUpEnDis: " << tePwrUpEnDis << std::endl;
        std::cout << "teClrHist: " << teClrHist << std::endl;
        std::cout << "teForceChg: " << teForceChg << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "ChgV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiChgV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiChgV == 0xffff) ? "No Data" : (uiChgV == 0xfffe) ? "Out of range" : (uiChgV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "ChgI") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiChgI + -32000.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "A");
            return (uiChgI == 0xffff) ? "No Data" : (uiChgI == 0xfffe) ? "Out of range" : (uiChgI == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "ChgIPer") {
            char buffer[50];
            auto engval = 0.5 * (float) (ucChgIPer + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "%");
            return (ucChgIPer == 0xff) ? "No Data" : (ucChgIPer == 0xfe) ? "Out of range" : (ucChgIPer == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "OpState") {
        }
        if (mnem == "PwrUpEnDis") {
        }
        if (mnem == "ClrHist") {
        }
        if (mnem == "ForceChg") {
        }
    }
} SccSts;

// Auto-generated class definition for RVC_tzPGN_INV_TEMP_STS
typedef struct InvTempSts {
    uchar8 ucInst;
    uint16 uiFet1Temp;
    uint16 uiXfmrTemp;
    uint16 uiFet2Temp;
    void Print() const {
        std::cout << "PGN: InvTempSts" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiFet1Temp: " << uiFet1Temp << std::endl;
        std::cout << "uiXfmrTemp: " << uiXfmrTemp << std::endl;
        std::cout << "uiFet2Temp: " << uiFet2Temp << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "Fet1Temp") {
            char buffer[50];
            auto engval = 0.03125 * (float) (uiFet1Temp + -8736.0);
            snprintf(buffer, sizeof(buffer), "%.2f %s", engval, "deg C");
            return (uiFet1Temp == 0xffff) ? "No Data" : (uiFet1Temp == 0xfffe) ? "Out of range" : (uiFet1Temp == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "XfmrTemp") {
            char buffer[50];
            auto engval = 0.03125 * (float) (uiXfmrTemp + -8736.0);
            snprintf(buffer, sizeof(buffer), "%.2f %s", engval, "deg C");
            return (uiXfmrTemp == 0xffff) ? "No Data" : (uiXfmrTemp == 0xfffe) ? "Out of range" : (uiXfmrTemp == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "Fet2Temp") {
            char buffer[50];
            auto engval = 0.03125 * (float) (uiFet2Temp + -8736.0);
            snprintf(buffer, sizeof(buffer), "%.2f %s", engval, "deg C");
            return (uiFet2Temp == 0xffff) ? "No Data" : (uiFet2Temp == 0xfffe) ? "Out of range" : (uiFet2Temp == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} InvTempSts;

// Auto-generated class definition for RVC_tzPGN_CHG_CFG_CMD4
typedef struct ChgCfgCmd4 {
    uchar8 ucInst;
    uint16 uiBulkTime;
    uint16 uiAbsorpTime;
    uint16 uiFloatTime;
    void Print() const {
        std::cout << "PGN: ChgCfgCmd4" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiBulkTime: " << uiBulkTime << std::endl;
        std::cout << "uiAbsorpTime: " << uiAbsorpTime << std::endl;
        std::cout << "uiFloatTime: " << uiFloatTime << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "BulkTime") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiBulkTime + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "Min");
            return (uiBulkTime == 0xffff) ? "No Data" : (uiBulkTime == 0xfffe) ? "Out of range" : (uiBulkTime == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "AbsorpTime") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiAbsorpTime + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "Min");
            return (uiAbsorpTime == 0xffff) ? "No Data" : (uiAbsorpTime == 0xfffe) ? "Out of range" : (uiAbsorpTime == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "FloatTime") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiFloatTime + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "Min");
            return (uiFloatTime == 0xffff) ? "No Data" : (uiFloatTime == 0xfffe) ? "Out of range" : (uiFloatTime == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} ChgCfgCmd4;

// Auto-generated class definition for RVC_tzPGN_CHG_CFG_STS4
typedef struct ChgCfgSts4 {
    uchar8 ucInst;
    uint16 uiBulkTime;
    uint16 uiAbsorpTime;
    uint16 uiFloatTime;
    void Print() const {
        std::cout << "PGN: ChgCfgSts4" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiBulkTime: " << uiBulkTime << std::endl;
        std::cout << "uiAbsorpTime: " << uiAbsorpTime << std::endl;
        std::cout << "uiFloatTime: " << uiFloatTime << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "BulkTime") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiBulkTime + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "Min");
            return (uiBulkTime == 0xffff) ? "No Data" : (uiBulkTime == 0xfffe) ? "Out of range" : (uiBulkTime == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "AbsorpTime") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiAbsorpTime + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "Min");
            return (uiAbsorpTime == 0xffff) ? "No Data" : (uiAbsorpTime == 0xfffe) ? "Out of range" : (uiAbsorpTime == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "FloatTime") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiFloatTime + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "Min");
            return (uiFloatTime == 0xffff) ? "No Data" : (uiFloatTime == 0xfffe) ? "Out of range" : (uiFloatTime == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} ChgCfgSts4;

// Auto-generated class definition for RVC_tzPGN_DC_SRC_STS6
typedef struct DcSrcSts6 {
    RVC_teDC_SRC_ID teDcInst;
    RVC_teRVC_DEVICE_PRIORITY teDevPri;
    RVC_teGENERIC_STSPAIR teHiVoltLimitSts;
    RVC_teDISCONNECT_STS teHiVoltDisconSts;
    RVC_teGENERIC_STSPAIR teLowVoltLimitSts;
    RVC_teDISCONNECT_STS teLowVoltDisconSts;
    RVC_teGENERIC_STSPAIR teLowSocLimitSts;
    RVC_teDISCONNECT_STS teLowSocDisconSts;
    RVC_teGENERIC_STSPAIR teLowDcSrcTempLimitSts;
    RVC_teDISCONNECT_STS teLowDcSrcTempDisconSts;
    RVC_teGENERIC_STSPAIR teHiDcSrcTempLimitSts;
    RVC_teDISCONNECT_STS teHiDcSrcTempDisconSts;
    RVC_teGENERIC_STSPAIR teHiAmpDcSrcLimitSts;
    RVC_teDISCONNECT_STS teHiAmpDcSrcDisconSts;
    void Print() const {
        std::cout << "PGN: DcSrcSts6" << std::endl;
        std::cout << "teDcInst: " << teDcInst << std::endl;
        std::cout << "teDevPri: " << teDevPri << std::endl;
        std::cout << "teHiVoltLimitSts: " << teHiVoltLimitSts << std::endl;
        std::cout << "teHiVoltDisconSts: " << teHiVoltDisconSts << std::endl;
        std::cout << "teLowVoltLimitSts: " << teLowVoltLimitSts << std::endl;
        std::cout << "teLowVoltDisconSts: " << teLowVoltDisconSts << std::endl;
        std::cout << "teLowSocLimitSts: " << teLowSocLimitSts << std::endl;
        std::cout << "teLowSocDisconSts: " << teLowSocDisconSts << std::endl;
        std::cout << "teLowDcSrcTempLimitSts: " << teLowDcSrcTempLimitSts << std::endl;
        std::cout << "teLowDcSrcTempDisconSts: " << teLowDcSrcTempDisconSts << std::endl;
        std::cout << "teHiDcSrcTempLimitSts: " << teHiDcSrcTempLimitSts << std::endl;
        std::cout << "teHiDcSrcTempDisconSts: " << teHiDcSrcTempDisconSts << std::endl;
        std::cout << "teHiAmpDcSrcLimitSts: " << teHiAmpDcSrcLimitSts << std::endl;
        std::cout << "teHiAmpDcSrcDisconSts: " << teHiAmpDcSrcDisconSts << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "DcInst") {
        }
        if (mnem == "DevPri") {
        }
        if (mnem == "HiVoltLimitSts") {
        }
        if (mnem == "HiVoltDisconSts") {
        }
        if (mnem == "LowVoltLimitSts") {
        }
        if (mnem == "LowVoltDisconSts") {
        }
        if (mnem == "LowSocLimitSts") {
        }
        if (mnem == "LowSocDisconSts") {
        }
        if (mnem == "LowDcSrcTempLimitSts") {
        }
        if (mnem == "LowDcSrcTempDisconSts") {
        }
        if (mnem == "HiDcSrcTempLimitSts") {
        }
        if (mnem == "HiDcSrcTempDisconSts") {
        }
        if (mnem == "HiAmpDcSrcLimitSts") {
        }
        if (mnem == "HiAmpDcSrcDisconSts") {
        }
    }
} DcSrcSts6;

// Auto-generated class definition for RVC_tzPGN_DC_SRC_STS4
typedef struct DcSrcSts4 {
    RVC_teDC_SRC_ID teInst;
    RVC_teRVC_DEVICE_PRIORITY teDevPri;
    RVC_teRVC_DESIRED_CHG_STATE teDesChgState;
    uint16 uiDesDcV;
    uint16 uiDesDcI;
    RVC_teRVC_BATT_TYPE teBattType;
    void Print() const {
        std::cout << "PGN: DcSrcSts4" << std::endl;
        std::cout << "teInst: " << teInst << std::endl;
        std::cout << "teDevPri: " << teDevPri << std::endl;
        std::cout << "teDesChgState: " << teDesChgState << std::endl;
        std::cout << "uiDesDcV: " << uiDesDcV << std::endl;
        std::cout << "uiDesDcI: " << uiDesDcI << std::endl;
        std::cout << "teBattType: " << teBattType << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "DevPri") {
        }
        if (mnem == "DesChgState") {
        }
        if (mnem == "DesDcV") {
            return (uiDesDcV == 0xffff) ? "No Data" : (uiDesDcV == 0xfffe) ? "Out of range" : (uiDesDcV == 0xfffd) ? "Reserved" : std::to_string(uiDesDcV);
        }
        if (mnem == "DesDcI") {
            return (uiDesDcI == 0xffff) ? "No Data" : (uiDesDcI == 0xfffe) ? "Out of range" : (uiDesDcI == 0xfffd) ? "Reserved" : std::to_string(uiDesDcI);
        }
        if (mnem == "BattType") {
        }
    }
} DcSrcSts4;

// Auto-generated class definition for RVC_tzPGN_EXTRA_DIAG_MSG1
typedef struct ExtraDiagMsg1 {
    uchar8 ucSpnMsb;
    uchar8 ucSpnIsb;
    RVC_teSAEJ1939FMI teFmi;
    uchar8 ucSpnLsb;
    uchar8 ucOccurrenceCnt;
    uchar8 ucProdIdExt;
    void Print() const {
        std::cout << "PGN: ExtraDiagMsg1" << std::endl;
        std::cout << "ucSpnMsb: " << static_cast<int>(ucSpnMsb) << std::endl;
        std::cout << "ucSpnIsb: " << static_cast<int>(ucSpnIsb) << std::endl;
        std::cout << "teFmi: " << teFmi << std::endl;
        std::cout << "ucSpnLsb: " << static_cast<int>(ucSpnLsb) << std::endl;
        std::cout << "ucOccurrenceCnt: " << static_cast<int>(ucOccurrenceCnt) << std::endl;
        std::cout << "ucProdIdExt: " << static_cast<int>(ucProdIdExt) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
    }
} ExtraDiagMsg1;

// Auto-generated class definition for RVC_tzPGN_DIAG_MSG1
typedef struct DiagMsg1 {
    RVC_teGENERIC_STSPAIR teOpStsProdOn;
    RVC_teGENERIC_STSPAIR teOpStsProdActive;
    RVC_teGENERIC_STSPAIR teOpStsYel;
    RVC_teGENERIC_STSPAIR teOpStsRed;
    RVC_teDFLT_SRC_ADDR teProdId;
    uint16 uiExtraCount;
    RVC_tzPGN_EXTRA_DIAG_MSG1 ptzExtra;
    void Print() const {
        std::cout << "PGN: DiagMsg1" << std::endl;
        std::cout << "teOpStsProdOn: " << teOpStsProdOn << std::endl;
        std::cout << "teOpStsProdActive: " << teOpStsProdActive << std::endl;
        std::cout << "teOpStsYel: " << teOpStsYel << std::endl;
        std::cout << "teOpStsRed: " << teOpStsRed << std::endl;
        std::cout << "teProdId: " << teProdId << std::endl;
        std::cout << "uiExtraCount: " << uiExtraCount << std::endl;
        if (ptzExtra != nullptr) {
            std::cout << "ptzExtra: " << std::endl;
            for (uint16 i = 0; i < uiExtraCount; ++i) {
                std::cout << "    Extra[" << i << "]: " << std::endl;
                DiagMsg1Extra[i].Print();
            }
            std::cout << " extra end " << std::endl;
        } else {
            std::cout << "  No ptzExtra" << std::endl;
        }
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "OpStsProdOn") {
        }
        if (mnem == "OpStsProdActive") {
        }
        if (mnem == "OpStsYel") {
        }
        if (mnem == "OpStsRed") {
        }
        if (mnem == "ProdId") {
        }
    }
} DiagMsg1;

// Auto-generated class definition for RVC_tzPGN_CHG_CFG_CMD3
typedef struct ChgCfgCmd3 {
    uchar8 ucInst;
    uint16 uiBulkV;
    uint16 uiAbsorpV;
    uint16 uiFloatV;
    uchar8 ucTempCompConst;
    void Print() const {
        std::cout << "PGN: ChgCfgCmd3" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiBulkV: " << uiBulkV << std::endl;
        std::cout << "uiAbsorpV: " << uiAbsorpV << std::endl;
        std::cout << "uiFloatV: " << uiFloatV << std::endl;
        std::cout << "ucTempCompConst: " << static_cast<int>(ucTempCompConst) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "BulkV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiBulkV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiBulkV == 0xffff) ? "No Data" : (uiBulkV == 0xfffe) ? "Out of range" : (uiBulkV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "AbsorpV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiAbsorpV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiAbsorpV == 0xffff) ? "No Data" : (uiAbsorpV == 0xfffe) ? "Out of range" : (uiAbsorpV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "FloatV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiFloatV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiFloatV == 0xffff) ? "No Data" : (uiFloatV == 0xfffe) ? "Out of range" : (uiFloatV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "TempCompConst") {
            return (ucTempCompConst == 0xff) ? "No Data" : (ucTempCompConst == 0xfe) ? "Out of range" : (ucTempCompConst == 0xfd) ? "Reserved" : std::to_string(ucTempCompConst);
        }
    }
} ChgCfgCmd3;

// Auto-generated class definition for RVC_tzPGN_CHG_CFG_STS3
typedef struct ChgCfgSts3 {
    uchar8 ucInst;
    uint16 uiBulkV;
    uint16 uiAbsorpV;
    uint16 uiFloatV;
    uchar8 ucTempCompConst;
    void Print() const {
        std::cout << "PGN: ChgCfgSts3" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiBulkV: " << uiBulkV << std::endl;
        std::cout << "uiAbsorpV: " << uiAbsorpV << std::endl;
        std::cout << "uiFloatV: " << uiFloatV << std::endl;
        std::cout << "ucTempCompConst: " << static_cast<int>(ucTempCompConst) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "BulkV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiBulkV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiBulkV == 0xffff) ? "No Data" : (uiBulkV == 0xfffe) ? "Out of range" : (uiBulkV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "AbsorpV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiAbsorpV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiAbsorpV == 0xffff) ? "No Data" : (uiAbsorpV == 0xfffe) ? "Out of range" : (uiAbsorpV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "FloatV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiFloatV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiFloatV == 0xffff) ? "No Data" : (uiFloatV == 0xfffe) ? "Out of range" : (uiFloatV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "TempCompConst") {
            return (ucTempCompConst == 0xff) ? "No Data" : (ucTempCompConst == 0xfe) ? "Out of range" : (ucTempCompConst == 0xfd) ? "Reserved" : std::to_string(ucTempCompConst);
        }
    }
} ChgCfgSts3;

// Auto-generated class definition for RVC_tzPGN_INV_CFG_CMD3
typedef struct InvCfgCmd3 {
    uchar8 ucInst;
    uint16 uiDcSrcShtDwnDly;
    RVC_teRVC_STACK_MODE teStackMd;
    uint16 uiDcUvSdRecLvl;
    uint16 uiGenSupportEngI;
    void Print() const {
        std::cout << "PGN: InvCfgCmd3" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiDcSrcShtDwnDly: " << uiDcSrcShtDwnDly << std::endl;
        std::cout << "teStackMd: " << teStackMd << std::endl;
        std::cout << "uiDcUvSdRecLvl: " << uiDcUvSdRecLvl << std::endl;
        std::cout << "uiGenSupportEngI: " << uiGenSupportEngI << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "DcSrcShtDwnDly") {
            char buffer[50];
            auto engval = 0.5 * (float) (uiDcSrcShtDwnDly + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "s");
            return (uiDcSrcShtDwnDly == 0xffff) ? "No Data" : (uiDcSrcShtDwnDly == 0xfffe) ? "Out of range" : (uiDcSrcShtDwnDly == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "StackMd") {
        }
        if (mnem == "DcUvSdRecLvl") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiDcUvSdRecLvl + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiDcUvSdRecLvl == 0xffff) ? "No Data" : (uiDcUvSdRecLvl == 0xfffe) ? "Out of range" : (uiDcUvSdRecLvl == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "GenSupportEngI") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiGenSupportEngI + -32000.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "A");
            return (uiGenSupportEngI == 0xffff) ? "No Data" : (uiGenSupportEngI == 0xfffe) ? "Out of range" : (uiGenSupportEngI == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} InvCfgCmd3;

// Auto-generated class definition for RVC_tzPGN_INV_CFG_STS3
typedef struct InvCfgSts3 {
    uchar8 ucInst;
    uint16 uiDcSrcShtDwnDly;
    RVC_teRVC_STACK_MODE teStackMd;
    uint16 uiDcUvSdRecLvl;
    uint16 uiGenSupportEngI;
    void Print() const {
        std::cout << "PGN: InvCfgSts3" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiDcSrcShtDwnDly: " << uiDcSrcShtDwnDly << std::endl;
        std::cout << "teStackMd: " << teStackMd << std::endl;
        std::cout << "uiDcUvSdRecLvl: " << uiDcUvSdRecLvl << std::endl;
        std::cout << "uiGenSupportEngI: " << uiGenSupportEngI << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "DcSrcShtDwnDly") {
            char buffer[50];
            auto engval = 0.5 * (float) (uiDcSrcShtDwnDly + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "s");
            return (uiDcSrcShtDwnDly == 0xffff) ? "No Data" : (uiDcSrcShtDwnDly == 0xfffe) ? "Out of range" : (uiDcSrcShtDwnDly == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "StackMd") {
        }
        if (mnem == "DcUvSdRecLvl") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiDcUvSdRecLvl + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiDcUvSdRecLvl == 0xffff) ? "No Data" : (uiDcUvSdRecLvl == 0xfffe) ? "Out of range" : (uiDcUvSdRecLvl == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "GenSupportEngI") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiGenSupportEngI + -32000.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "A");
            return (uiGenSupportEngI == 0xffff) ? "No Data" : (uiGenSupportEngI == 0xfffe) ? "Out of range" : (uiGenSupportEngI == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} InvCfgSts3;

// Auto-generated class definition for RVC_tzPGN_INV_DC_STS
typedef struct InvDcSts {
    uchar8 ucInst;
    uint16 uiDcVolt;
    uint16 uiDcAmps;
    void Print() const {
        std::cout << "PGN: InvDcSts" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiDcVolt: " << uiDcVolt << std::endl;
        std::cout << "uiDcAmps: " << uiDcAmps << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "DcVolt") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiDcVolt + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiDcVolt == 0xffff) ? "No Data" : (uiDcVolt == 0xfffe) ? "Out of range" : (uiDcVolt == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "DcAmps") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiDcAmps + -32000.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "A");
            return (uiDcAmps == 0xffff) ? "No Data" : (uiDcAmps == 0xfffe) ? "Out of range" : (uiDcAmps == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} InvDcSts;

// Auto-generated class definition for RVC_tzPGN_CHG_AC_FLT_CFG_CMD2
typedef struct ChgAcFltCfgCmd2 {
    uchar8 ucInst;
    RVC_teAC_LINE_NUM teLine;
    RVC_teGENERIC_STSPAIR teInOut;
    uchar8 ucHiFreqLimit;
    uchar8 ucLoFreqLimit;
    void Print() const {
        std::cout << "PGN: ChgAcFltCfgCmd2" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teLine: " << teLine << std::endl;
        std::cout << "teInOut: " << teInOut << std::endl;
        std::cout << "ucHiFreqLimit: " << static_cast<int>(ucHiFreqLimit) << std::endl;
        std::cout << "ucLoFreqLimit: " << static_cast<int>(ucLoFreqLimit) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "Line") {
        }
        if (mnem == "InOut") {
        }
        if (mnem == "HiFreqLimit") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucHiFreqLimit + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "Hz");
            return (ucHiFreqLimit == 0xff) ? "No Data" : (ucHiFreqLimit == 0xfe) ? "Out of range" : (ucHiFreqLimit == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "LoFreqLimit") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucLoFreqLimit + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "Hz");
            return (ucLoFreqLimit == 0xff) ? "No Data" : (ucLoFreqLimit == 0xfe) ? "Out of range" : (ucLoFreqLimit == 0xfd) ? "Reserved" : (buffer);
        }
    }
} ChgAcFltCfgCmd2;

// Auto-generated class definition for RVC_tzPGN_CHG_AC_FLT_CFG_CMD1
typedef struct ChgAcFltCfgCmd1 {
    uchar8 ucInst;
    RVC_teAC_LINE_NUM teLine;
    RVC_teGENERIC_STSPAIR teInOut;
    uchar8 ucExtremeLoV;
    uchar8 ucLoV;
    uchar8 ucHiV;
    uchar8 ucExtremeHiV;
    uchar8 ucQualTime;
    RVC_teGENERIC_STSPAIR teBypassMode;
    void Print() const {
        std::cout << "PGN: ChgAcFltCfgCmd1" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teLine: " << teLine << std::endl;
        std::cout << "teInOut: " << teInOut << std::endl;
        std::cout << "ucExtremeLoV: " << static_cast<int>(ucExtremeLoV) << std::endl;
        std::cout << "ucLoV: " << static_cast<int>(ucLoV) << std::endl;
        std::cout << "ucHiV: " << static_cast<int>(ucHiV) << std::endl;
        std::cout << "ucExtremeHiV: " << static_cast<int>(ucExtremeHiV) << std::endl;
        std::cout << "ucQualTime: " << static_cast<int>(ucQualTime) << std::endl;
        std::cout << "teBypassMode: " << teBypassMode << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "Line") {
        }
        if (mnem == "InOut") {
        }
        if (mnem == "ExtremeLoV") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucExtremeLoV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "V");
            return (ucExtremeLoV == 0xff) ? "No Data" : (ucExtremeLoV == 0xfe) ? "Out of range" : (ucExtremeLoV == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "LoV") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucLoV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "V");
            return (ucLoV == 0xff) ? "No Data" : (ucLoV == 0xfe) ? "Out of range" : (ucLoV == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "HiV") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucHiV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "V");
            return (ucHiV == 0xff) ? "No Data" : (ucHiV == 0xfe) ? "Out of range" : (ucHiV == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "ExtremeHiV") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucExtremeHiV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "V");
            return (ucExtremeHiV == 0xff) ? "No Data" : (ucExtremeHiV == 0xfe) ? "Out of range" : (ucExtremeHiV == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "QualTime") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucQualTime + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "s");
            return (ucQualTime == 0xff) ? "No Data" : (ucQualTime == 0xfe) ? "Out of range" : (ucQualTime == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "BypassMode") {
        }
    }
} ChgAcFltCfgCmd1;

// Auto-generated class definition for RVC_tzPGN_CHG_AC_FLT_CFG_STS2
typedef struct ChgAcFltCfgSts2 {
    uchar8 ucInst;
    RVC_teAC_LINE_NUM teLine;
    RVC_teGENERIC_STSPAIR teInOut;
    uchar8 ucHiFreqLimit;
    uchar8 ucLoFreqLimit;
    void Print() const {
        std::cout << "PGN: ChgAcFltCfgSts2" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teLine: " << teLine << std::endl;
        std::cout << "teInOut: " << teInOut << std::endl;
        std::cout << "ucHiFreqLimit: " << static_cast<int>(ucHiFreqLimit) << std::endl;
        std::cout << "ucLoFreqLimit: " << static_cast<int>(ucLoFreqLimit) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "Line") {
        }
        if (mnem == "InOut") {
        }
        if (mnem == "HiFreqLimit") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucHiFreqLimit + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "Hz");
            return (ucHiFreqLimit == 0xff) ? "No Data" : (ucHiFreqLimit == 0xfe) ? "Out of range" : (ucHiFreqLimit == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "LoFreqLimit") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucLoFreqLimit + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "Hz");
            return (ucLoFreqLimit == 0xff) ? "No Data" : (ucLoFreqLimit == 0xfe) ? "Out of range" : (ucLoFreqLimit == 0xfd) ? "Reserved" : (buffer);
        }
    }
} ChgAcFltCfgSts2;

// Auto-generated class definition for RVC_tzPGN_CHG_AC_FLT_CFG_STS1
typedef struct ChgAcFltCfgSts1 {
    uchar8 ucInst;
    RVC_teAC_LINE_NUM teLine;
    RVC_teGENERIC_STSPAIR teInOut;
    uchar8 ucExtremeLoV;
    uchar8 ucLoV;
    uchar8 ucHiV;
    uchar8 ucExtremeHiV;
    uchar8 ucQualTime;
    RVC_teGENERIC_STSPAIR teBypassMode;
    void Print() const {
        std::cout << "PGN: ChgAcFltCfgSts1" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teLine: " << teLine << std::endl;
        std::cout << "teInOut: " << teInOut << std::endl;
        std::cout << "ucExtremeLoV: " << static_cast<int>(ucExtremeLoV) << std::endl;
        std::cout << "ucLoV: " << static_cast<int>(ucLoV) << std::endl;
        std::cout << "ucHiV: " << static_cast<int>(ucHiV) << std::endl;
        std::cout << "ucExtremeHiV: " << static_cast<int>(ucExtremeHiV) << std::endl;
        std::cout << "ucQualTime: " << static_cast<int>(ucQualTime) << std::endl;
        std::cout << "teBypassMode: " << teBypassMode << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "Line") {
        }
        if (mnem == "InOut") {
        }
        if (mnem == "ExtremeLoV") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucExtremeLoV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "V");
            return (ucExtremeLoV == 0xff) ? "No Data" : (ucExtremeLoV == 0xfe) ? "Out of range" : (ucExtremeLoV == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "LoV") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucLoV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "V");
            return (ucLoV == 0xff) ? "No Data" : (ucLoV == 0xfe) ? "Out of range" : (ucLoV == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "HiV") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucHiV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "V");
            return (ucHiV == 0xff) ? "No Data" : (ucHiV == 0xfe) ? "Out of range" : (ucHiV == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "ExtremeHiV") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucExtremeHiV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "V");
            return (ucExtremeHiV == 0xff) ? "No Data" : (ucExtremeHiV == 0xfe) ? "Out of range" : (ucExtremeHiV == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "QualTime") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucQualTime + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "s");
            return (ucQualTime == 0xff) ? "No Data" : (ucQualTime == 0xfe) ? "Out of range" : (ucQualTime == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "BypassMode") {
        }
    }
} ChgAcFltCfgSts1;

// Auto-generated class definition for RVC_tzPGN_CHG_AC_STS4
typedef struct ChgAcSts4 {
    uchar8 ucInst;
    RVC_teAC_LINE_NUM teLine;
    RVC_teGENERIC_STSPAIR teInOut;
    RVC_teRVCV_FLT teVFlt;
    RVC_teGENERIC_STSPAIR teFltSurgeProt;
    RVC_teGENERIC_STSPAIR teFltHiFreq;
    RVC_teGENERIC_STSPAIR teFltLoFreq;
    RVC_teGENERIC_STSPAIR teBypassModeAct;
    RVC_teRVC_QUALIFICATION_STS teQualifStat;
    void Print() const {
        std::cout << "PGN: ChgAcSts4" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teLine: " << teLine << std::endl;
        std::cout << "teInOut: " << teInOut << std::endl;
        std::cout << "teVFlt: " << teVFlt << std::endl;
        std::cout << "teFltSurgeProt: " << teFltSurgeProt << std::endl;
        std::cout << "teFltHiFreq: " << teFltHiFreq << std::endl;
        std::cout << "teFltLoFreq: " << teFltLoFreq << std::endl;
        std::cout << "teBypassModeAct: " << teBypassModeAct << std::endl;
        std::cout << "teQualifStat: " << teQualifStat << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "Line") {
        }
        if (mnem == "InOut") {
        }
        if (mnem == "VFlt") {
        }
        if (mnem == "FltSurgeProt") {
        }
        if (mnem == "FltHiFreq") {
        }
        if (mnem == "FltLoFreq") {
        }
        if (mnem == "BypassModeAct") {
        }
        if (mnem == "QualifStat") {
        }
    }
} ChgAcSts4;

// Auto-generated class definition for RVC_tzPGN_INV_AC_FLT_CFG_CMD1
typedef struct InvAcFltCfgCmd1 {
    uchar8 ucInst;
    uchar8 ucExtremeLoV;
    uchar8 ucLoV;
    uchar8 ucHiV;
    uchar8 ucExtremeHiV;
    uchar8 ucQualTime;
    RVC_teGENERIC_STSPAIR teBypassMode;
    uchar8 ucRsvBits;
    void Print() const {
        std::cout << "PGN: InvAcFltCfgCmd1" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "ucExtremeLoV: " << static_cast<int>(ucExtremeLoV) << std::endl;
        std::cout << "ucLoV: " << static_cast<int>(ucLoV) << std::endl;
        std::cout << "ucHiV: " << static_cast<int>(ucHiV) << std::endl;
        std::cout << "ucExtremeHiV: " << static_cast<int>(ucExtremeHiV) << std::endl;
        std::cout << "ucQualTime: " << static_cast<int>(ucQualTime) << std::endl;
        std::cout << "teBypassMode: " << teBypassMode << std::endl;
        std::cout << "ucRsvBits: " << static_cast<int>(ucRsvBits) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "ExtremeLoV") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucExtremeLoV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "V");
            return (ucExtremeLoV == 0xff) ? "No Data" : (ucExtremeLoV == 0xfe) ? "Out of range" : (ucExtremeLoV == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "LoV") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucLoV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "V");
            return (ucLoV == 0xff) ? "No Data" : (ucLoV == 0xfe) ? "Out of range" : (ucLoV == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "HiV") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucHiV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "V");
            return (ucHiV == 0xff) ? "No Data" : (ucHiV == 0xfe) ? "Out of range" : (ucHiV == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "ExtremeHiV") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucExtremeHiV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "V");
            return (ucExtremeHiV == 0xff) ? "No Data" : (ucExtremeHiV == 0xfe) ? "Out of range" : (ucExtremeHiV == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "QualTime") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucQualTime + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "s");
            return (ucQualTime == 0xff) ? "No Data" : (ucQualTime == 0xfe) ? "Out of range" : (ucQualTime == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "BypassMode") {
        }
        if (mnem == "RsvBits") {
        }
    }
} InvAcFltCfgCmd1;

// Auto-generated class definition for RVC_tzPGN_INV_AC_FLT_CFG_STS1
typedef struct InvAcFltCfgSts1 {
    uchar8 ucInst;
    uchar8 ucExtremeLoV;
    uchar8 ucLoV;
    uchar8 ucHiV;
    uchar8 ucExtremeHiV;
    uchar8 ucQualTime;
    RVC_teGENERIC_STSPAIR teBypassMode;
    uchar8 ucRsvBits;
    void Print() const {
        std::cout << "PGN: InvAcFltCfgSts1" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "ucExtremeLoV: " << static_cast<int>(ucExtremeLoV) << std::endl;
        std::cout << "ucLoV: " << static_cast<int>(ucLoV) << std::endl;
        std::cout << "ucHiV: " << static_cast<int>(ucHiV) << std::endl;
        std::cout << "ucExtremeHiV: " << static_cast<int>(ucExtremeHiV) << std::endl;
        std::cout << "ucQualTime: " << static_cast<int>(ucQualTime) << std::endl;
        std::cout << "teBypassMode: " << teBypassMode << std::endl;
        std::cout << "ucRsvBits: " << static_cast<int>(ucRsvBits) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "ExtremeLoV") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucExtremeLoV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "V");
            return (ucExtremeLoV == 0xff) ? "No Data" : (ucExtremeLoV == 0xfe) ? "Out of range" : (ucExtremeLoV == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "LoV") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucLoV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "V");
            return (ucLoV == 0xff) ? "No Data" : (ucLoV == 0xfe) ? "Out of range" : (ucLoV == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "HiV") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucHiV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "V");
            return (ucHiV == 0xff) ? "No Data" : (ucHiV == 0xfe) ? "Out of range" : (ucHiV == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "ExtremeHiV") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucExtremeHiV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "V");
            return (ucExtremeHiV == 0xff) ? "No Data" : (ucExtremeHiV == 0xfe) ? "Out of range" : (ucExtremeHiV == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "QualTime") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucQualTime + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "s");
            return (ucQualTime == 0xff) ? "No Data" : (ucQualTime == 0xfe) ? "Out of range" : (ucQualTime == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "BypassMode") {
        }
        if (mnem == "RsvBits") {
        }
    }
} InvAcFltCfgSts1;

// Auto-generated class definition for RVC_tzPGN_INV_AC_STS4
typedef struct InvAcSts4 {
    uchar8 ucInst;
    RVC_teAC_LINE_NUM teLine;
    RVC_teGENERIC_STSPAIR teInOut;
    RVC_teRVCV_FLT teVFlt;
    RVC_teGENERIC_STSPAIR teFltSurgeProt;
    RVC_teGENERIC_STSPAIR teFltHiFreq;
    RVC_teGENERIC_STSPAIR teFltLoFreq;
    RVC_teGENERIC_STSPAIR teBypassModeAct;
    RVC_teRVC_QUALIFICATION_STS teQualifStat;
    void Print() const {
        std::cout << "PGN: InvAcSts4" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teLine: " << teLine << std::endl;
        std::cout << "teInOut: " << teInOut << std::endl;
        std::cout << "teVFlt: " << teVFlt << std::endl;
        std::cout << "teFltSurgeProt: " << teFltSurgeProt << std::endl;
        std::cout << "teFltHiFreq: " << teFltHiFreq << std::endl;
        std::cout << "teFltLoFreq: " << teFltLoFreq << std::endl;
        std::cout << "teBypassModeAct: " << teBypassModeAct << std::endl;
        std::cout << "teQualifStat: " << teQualifStat << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "Line") {
        }
        if (mnem == "InOut") {
        }
        if (mnem == "VFlt") {
        }
        if (mnem == "FltSurgeProt") {
        }
        if (mnem == "FltHiFreq") {
        }
        if (mnem == "FltLoFreq") {
        }
        if (mnem == "BypassModeAct") {
        }
        if (mnem == "QualifStat") {
        }
    }
} InvAcSts4;

// Auto-generated class definition for RVC_tzPGN_CHG_CFG_CMD2
typedef struct ChgCfgCmd2 {
    uchar8 ucInst;
    uchar8 ucMaxChgIPer;
    uchar8 ucChgRateLimitPerBankSz;
    uchar8 ucShoreBrkSz;
    uchar8 ucDefBattTemp;
    uint16 uiReChgVolt;
    void Print() const {
        std::cout << "PGN: ChgCfgCmd2" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "ucMaxChgIPer: " << static_cast<int>(ucMaxChgIPer) << std::endl;
        std::cout << "ucChgRateLimitPerBankSz: " << static_cast<int>(ucChgRateLimitPerBankSz) << std::endl;
        std::cout << "ucShoreBrkSz: " << static_cast<int>(ucShoreBrkSz) << std::endl;
        std::cout << "ucDefBattTemp: " << static_cast<int>(ucDefBattTemp) << std::endl;
        std::cout << "uiReChgVolt: " << uiReChgVolt << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "MaxChgIPer") {
            char buffer[50];
            auto engval = 0.5 * (float) (ucMaxChgIPer + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "%");
            return (ucMaxChgIPer == 0xff) ? "No Data" : (ucMaxChgIPer == 0xfe) ? "Out of range" : (ucMaxChgIPer == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "ChgRateLimitPerBankSz") {
            char buffer[50];
            auto engval = 0.5 * (float) (ucChgRateLimitPerBankSz + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "%");
            return (ucChgRateLimitPerBankSz == 0xff) ? "No Data" : (ucChgRateLimitPerBankSz == 0xfe) ? "Out of range" : (ucChgRateLimitPerBankSz == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "ShoreBrkSz") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucShoreBrkSz + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "A");
            return (ucShoreBrkSz == 0xff) ? "No Data" : (ucShoreBrkSz == 0xfe) ? "Out of range" : (ucShoreBrkSz == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "DefBattTemp") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucDefBattTemp + -40.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "deg C");
            return (ucDefBattTemp == 0xff) ? "No Data" : (ucDefBattTemp == 0xfe) ? "Out of range" : (ucDefBattTemp == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "ReChgVolt") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiReChgVolt + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiReChgVolt == 0xffff) ? "No Data" : (uiReChgVolt == 0xfffe) ? "Out of range" : (uiReChgVolt == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} ChgCfgCmd2;

// Auto-generated class definition for RVC_tzPGN_CHG_CFG_STS2
typedef struct ChgCfgSts2 {
    uchar8 ucInst;
    uchar8 ucMaxChgIPer;
    uchar8 ucChgRateLimitPerBankSz;
    uchar8 ucShoreBrkSz;
    uchar8 ucDefBattTemp;
    uint16 uiReChgVolt;
    void Print() const {
        std::cout << "PGN: ChgCfgSts2" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "ucMaxChgIPer: " << static_cast<int>(ucMaxChgIPer) << std::endl;
        std::cout << "ucChgRateLimitPerBankSz: " << static_cast<int>(ucChgRateLimitPerBankSz) << std::endl;
        std::cout << "ucShoreBrkSz: " << static_cast<int>(ucShoreBrkSz) << std::endl;
        std::cout << "ucDefBattTemp: " << static_cast<int>(ucDefBattTemp) << std::endl;
        std::cout << "uiReChgVolt: " << uiReChgVolt << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "MaxChgIPer") {
            char buffer[50];
            auto engval = 0.5 * (float) (ucMaxChgIPer + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "%");
            return (ucMaxChgIPer == 0xff) ? "No Data" : (ucMaxChgIPer == 0xfe) ? "Out of range" : (ucMaxChgIPer == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "ChgRateLimitPerBankSz") {
            char buffer[50];
            auto engval = 0.5 * (float) (ucChgRateLimitPerBankSz + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "%");
            return (ucChgRateLimitPerBankSz == 0xff) ? "No Data" : (ucChgRateLimitPerBankSz == 0xfe) ? "Out of range" : (ucChgRateLimitPerBankSz == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "ShoreBrkSz") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucShoreBrkSz + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "A");
            return (ucShoreBrkSz == 0xff) ? "No Data" : (ucShoreBrkSz == 0xfe) ? "Out of range" : (ucShoreBrkSz == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "DefBattTemp") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucDefBattTemp + -40.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "deg C");
            return (ucDefBattTemp == 0xff) ? "No Data" : (ucDefBattTemp == 0xfe) ? "Out of range" : (ucDefBattTemp == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "ReChgVolt") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiReChgVolt + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiReChgVolt == 0xffff) ? "No Data" : (uiReChgVolt == 0xfffe) ? "Out of range" : (uiReChgVolt == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} ChgCfgSts2;

// Auto-generated class definition for RVC_tzPGN_CHG_EQLZ_CFG_CMD
typedef struct ChgEqlzCfgCmd {
    uchar8 ucInst;
    uint16 uiEqlzV;
    uint16 uiEqlzTime;
    void Print() const {
        std::cout << "PGN: ChgEqlzCfgCmd" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiEqlzV: " << uiEqlzV << std::endl;
        std::cout << "uiEqlzTime: " << uiEqlzTime << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "EqlzV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiEqlzV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiEqlzV == 0xffff) ? "No Data" : (uiEqlzV == 0xfffe) ? "Out of range" : (uiEqlzV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "EqlzTime") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiEqlzTime + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "Min");
            return (uiEqlzTime == 0xffff) ? "No Data" : (uiEqlzTime == 0xfffe) ? "Out of range" : (uiEqlzTime == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} ChgEqlzCfgCmd;

// Auto-generated class definition for RVC_tzPGN_CHG_EQLZ_CFG_STS
typedef struct ChgEqlzCfgSts {
    uchar8 ucInst;
    uint16 uiEqlzV;
    uint16 uiEqlzTime;
    void Print() const {
        std::cout << "PGN: ChgEqlzCfgSts" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiEqlzV: " << uiEqlzV << std::endl;
        std::cout << "uiEqlzTime: " << uiEqlzTime << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "EqlzV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiEqlzV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiEqlzV == 0xffff) ? "No Data" : (uiEqlzV == 0xfffe) ? "Out of range" : (uiEqlzV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "EqlzTime") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiEqlzTime + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "Min");
            return (uiEqlzTime == 0xffff) ? "No Data" : (uiEqlzTime == 0xfffe) ? "Out of range" : (uiEqlzTime == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} ChgEqlzCfgSts;

// Auto-generated class definition for RVC_tzPGN_CHG_EQLZ_STS
typedef struct ChgEqlzSts {
    uchar8 ucInst;
    uint16 uiTimeRem;
    RVC_teRVC_PRE_CHG_STS tePreChgSts;
    void Print() const {
        std::cout << "PGN: ChgEqlzSts" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiTimeRem: " << uiTimeRem << std::endl;
        std::cout << "tePreChgSts: " << tePreChgSts << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "TimeRem") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiTimeRem + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "Min");
            return (uiTimeRem == 0xffff) ? "No Data" : (uiTimeRem == 0xfffe) ? "Out of range" : (uiTimeRem == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "PreChgSts") {
        }
    }
} ChgEqlzSts;

// Auto-generated class definition for RVC_tzPGN_AC_FLT_CTRL_CFG_CMD2
typedef struct AcFltCtrlCfgCmd2 {
    uchar8 ucInst;
    RVC_teGENERIC_STSPAIR teLine;
    RVC_teGENERIC_STSPAIR teInOut;
    uchar8 ucHiFreqLimit;
    uchar8 ucLoFreqLimit;
    void Print() const {
        std::cout << "PGN: AcFltCtrlCfgCmd2" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teLine: " << teLine << std::endl;
        std::cout << "teInOut: " << teInOut << std::endl;
        std::cout << "ucHiFreqLimit: " << static_cast<int>(ucHiFreqLimit) << std::endl;
        std::cout << "ucLoFreqLimit: " << static_cast<int>(ucLoFreqLimit) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "Line") {
        }
        if (mnem == "InOut") {
        }
        if (mnem == "HiFreqLimit") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucHiFreqLimit + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "Hz");
            return (ucHiFreqLimit == 0xff) ? "No Data" : (ucHiFreqLimit == 0xfe) ? "Out of range" : (ucHiFreqLimit == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "LoFreqLimit") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucLoFreqLimit + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "Hz");
            return (ucLoFreqLimit == 0xff) ? "No Data" : (ucLoFreqLimit == 0xfe) ? "Out of range" : (ucLoFreqLimit == 0xfd) ? "Reserved" : (buffer);
        }
    }
} AcFltCtrlCfgCmd2;

// Auto-generated class definition for RVC_tzPGN_AC_FLT_CTRL_CFG_CMD1
typedef struct AcFltCtrlCfgCmd1 {
    uchar8 ucInst;
    RVC_teGENERIC_STSPAIR teLine;
    RVC_teGENERIC_STSPAIR teInOut;
    uchar8 ucExtremeLoV;
    uchar8 ucLoV;
    uchar8 ucHiV;
    uchar8 ucExtremeHiV;
    uchar8 ucQualTime;
    RVC_teGENERIC_STSPAIR teBypassMode;
    void Print() const {
        std::cout << "PGN: AcFltCtrlCfgCmd1" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teLine: " << teLine << std::endl;
        std::cout << "teInOut: " << teInOut << std::endl;
        std::cout << "ucExtremeLoV: " << static_cast<int>(ucExtremeLoV) << std::endl;
        std::cout << "ucLoV: " << static_cast<int>(ucLoV) << std::endl;
        std::cout << "ucHiV: " << static_cast<int>(ucHiV) << std::endl;
        std::cout << "ucExtremeHiV: " << static_cast<int>(ucExtremeHiV) << std::endl;
        std::cout << "ucQualTime: " << static_cast<int>(ucQualTime) << std::endl;
        std::cout << "teBypassMode: " << teBypassMode << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "Line") {
        }
        if (mnem == "InOut") {
        }
        if (mnem == "ExtremeLoV") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucExtremeLoV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "V");
            return (ucExtremeLoV == 0xff) ? "No Data" : (ucExtremeLoV == 0xfe) ? "Out of range" : (ucExtremeLoV == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "LoV") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucLoV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "V");
            return (ucLoV == 0xff) ? "No Data" : (ucLoV == 0xfe) ? "Out of range" : (ucLoV == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "HiV") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucHiV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "V");
            return (ucHiV == 0xff) ? "No Data" : (ucHiV == 0xfe) ? "Out of range" : (ucHiV == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "ExtremeHiV") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucExtremeHiV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "V");
            return (ucExtremeHiV == 0xff) ? "No Data" : (ucExtremeHiV == 0xfe) ? "Out of range" : (ucExtremeHiV == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "QualTime") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucQualTime + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "s");
            return (ucQualTime == 0xff) ? "No Data" : (ucQualTime == 0xfe) ? "Out of range" : (ucQualTime == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "BypassMode") {
        }
    }
} AcFltCtrlCfgCmd1;

// Auto-generated class definition for RVC_tzPGN_AC_FLT_CTRL_CFG_STS2
typedef struct AcFltCtrlCfgSts2 {
    uchar8 ucInst;
    RVC_teGENERIC_STSPAIR teLine;
    RVC_teGENERIC_STSPAIR teInOut;
    uchar8 ucHiFreqLimit;
    uchar8 ucLoFreqLimit;
    void Print() const {
        std::cout << "PGN: AcFltCtrlCfgSts2" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teLine: " << teLine << std::endl;
        std::cout << "teInOut: " << teInOut << std::endl;
        std::cout << "ucHiFreqLimit: " << static_cast<int>(ucHiFreqLimit) << std::endl;
        std::cout << "ucLoFreqLimit: " << static_cast<int>(ucLoFreqLimit) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "Line") {
        }
        if (mnem == "InOut") {
        }
        if (mnem == "HiFreqLimit") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucHiFreqLimit + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "Hz");
            return (ucHiFreqLimit == 0xff) ? "No Data" : (ucHiFreqLimit == 0xfe) ? "Out of range" : (ucHiFreqLimit == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "LoFreqLimit") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucLoFreqLimit + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "Hz");
            return (ucLoFreqLimit == 0xff) ? "No Data" : (ucLoFreqLimit == 0xfe) ? "Out of range" : (ucLoFreqLimit == 0xfd) ? "Reserved" : (buffer);
        }
    }
} AcFltCtrlCfgSts2;

// Auto-generated class definition for RVC_tzPGN_AC_FLT_CTRL_CFG_STS1
typedef struct AcFltCtrlCfgSts1 {
    uchar8 ucInst;
    RVC_teGENERIC_STSPAIR teLine;
    RVC_teGENERIC_STSPAIR teInOut;
    uchar8 ucExtremeLoV;
    uchar8 ucLoV;
    uchar8 ucHiV;
    uchar8 ucExtremeHiV;
    uchar8 ucQualTime;
    RVC_teGENERIC_STSPAIR teBypassMode;
    void Print() const {
        std::cout << "PGN: AcFltCtrlCfgSts1" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teLine: " << teLine << std::endl;
        std::cout << "teInOut: " << teInOut << std::endl;
        std::cout << "ucExtremeLoV: " << static_cast<int>(ucExtremeLoV) << std::endl;
        std::cout << "ucLoV: " << static_cast<int>(ucLoV) << std::endl;
        std::cout << "ucHiV: " << static_cast<int>(ucHiV) << std::endl;
        std::cout << "ucExtremeHiV: " << static_cast<int>(ucExtremeHiV) << std::endl;
        std::cout << "ucQualTime: " << static_cast<int>(ucQualTime) << std::endl;
        std::cout << "teBypassMode: " << teBypassMode << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "Line") {
        }
        if (mnem == "InOut") {
        }
        if (mnem == "ExtremeLoV") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucExtremeLoV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "V");
            return (ucExtremeLoV == 0xff) ? "No Data" : (ucExtremeLoV == 0xfe) ? "Out of range" : (ucExtremeLoV == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "LoV") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucLoV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "V");
            return (ucLoV == 0xff) ? "No Data" : (ucLoV == 0xfe) ? "Out of range" : (ucLoV == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "HiV") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucHiV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "V");
            return (ucHiV == 0xff) ? "No Data" : (ucHiV == 0xfe) ? "Out of range" : (ucHiV == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "ExtremeHiV") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucExtremeHiV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "V");
            return (ucExtremeHiV == 0xff) ? "No Data" : (ucExtremeHiV == 0xfe) ? "Out of range" : (ucExtremeHiV == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "QualTime") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucQualTime + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "s");
            return (ucQualTime == 0xff) ? "No Data" : (ucQualTime == 0xfe) ? "Out of range" : (ucQualTime == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "BypassMode") {
        }
    }
} AcFltCtrlCfgSts1;

// Auto-generated class definition for RVC_tzPGN_AGS_CRIT_CMD_XFER_AC_V
typedef struct AgsCritCmdXferAcV {
    uchar8 ucInst;
    RVC_teCMD_PAIR teCmd;
    RVC_teCMD_PAIR teActSts;
    RVC_teRVCAGS_CRITERION_TYPE teCritType;
    uchar8 ucAcInst;
    uint16 uiAcVThresh;
    uchar8 ucTimeUnderThresh;
    void Print() const {
        std::cout << "PGN: AgsCritCmdXferAcV" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teCmd: " << teCmd << std::endl;
        std::cout << "teActSts: " << teActSts << std::endl;
        std::cout << "teCritType: " << teCritType << std::endl;
        std::cout << "ucAcInst: " << static_cast<int>(ucAcInst) << std::endl;
        std::cout << "uiAcVThresh: " << uiAcVThresh << std::endl;
        std::cout << "ucTimeUnderThresh: " << static_cast<int>(ucTimeUnderThresh) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "Cmd") {
        }
        if (mnem == "ActSts") {
        }
        if (mnem == "CritType") {
        }
        if (mnem == "AcInst") {
            return (ucAcInst == 0xff) ? "No Data" : (ucAcInst == 0xfe) ? "Out of range" : (ucAcInst == 0xfd) ? "Reserved" : std::to_string(ucAcInst);
        }
        if (mnem == "AcVThresh") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiAcVThresh + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiAcVThresh == 0xffff) ? "No Data" : (uiAcVThresh == 0xfffe) ? "Out of range" : (uiAcVThresh == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "TimeUnderThresh") {
            return (ucTimeUnderThresh == 0xff) ? "No Data" : (ucTimeUnderThresh == 0xfe) ? "Out of range" : (ucTimeUnderThresh == 0xfd) ? "Reserved" : std::to_string(ucTimeUnderThresh);
        }
    }
} AgsCritCmdXferAcV;

// Auto-generated class definition for RVC_tzPGN_AGS_CRIT_CMD_DC_SOC
typedef struct AgsCritCmdDcSoc {
    uchar8 ucInst;
    RVC_teCMD_PAIR teCmd;
    RVC_teCMD_PAIR teActSts;
    RVC_teRVCAGS_CRITERION_TYPE teCritType;
    uchar8 ucDcInst;
    uchar8 ucDcSocThresh;
    uchar8 ucTimeUnderThresh;
    void Print() const {
        std::cout << "PGN: AgsCritCmdDcSoc" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teCmd: " << teCmd << std::endl;
        std::cout << "teActSts: " << teActSts << std::endl;
        std::cout << "teCritType: " << teCritType << std::endl;
        std::cout << "ucDcInst: " << static_cast<int>(ucDcInst) << std::endl;
        std::cout << "ucDcSocThresh: " << static_cast<int>(ucDcSocThresh) << std::endl;
        std::cout << "ucTimeUnderThresh: " << static_cast<int>(ucTimeUnderThresh) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "Cmd") {
        }
        if (mnem == "ActSts") {
        }
        if (mnem == "CritType") {
        }
        if (mnem == "DcInst") {
            return (ucDcInst == 0xff) ? "No Data" : (ucDcInst == 0xfe) ? "Out of range" : (ucDcInst == 0xfd) ? "Reserved" : std::to_string(ucDcInst);
        }
        if (mnem == "DcSocThresh") {
            char buffer[50];
            auto engval = 0.5 * (float) (ucDcSocThresh + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "%");
            return (ucDcSocThresh == 0xff) ? "No Data" : (ucDcSocThresh == 0xfe) ? "Out of range" : (ucDcSocThresh == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "TimeUnderThresh") {
            return (ucTimeUnderThresh == 0xff) ? "No Data" : (ucTimeUnderThresh == 0xfe) ? "Out of range" : (ucTimeUnderThresh == 0xfd) ? "Reserved" : std::to_string(ucTimeUnderThresh);
        }
    }
} AgsCritCmdDcSoc;

// Auto-generated class definition for RVC_tzPGN_AGS_CRIT_CMD_DC_V
typedef struct AgsCritCmdDcV {
    uchar8 ucInst;
    RVC_teCMD_PAIR teCmd;
    RVC_teCMD_PAIR teActSts;
    RVC_teRVCAGS_CRITERION_TYPE teCritType;
    uchar8 ucDcInst;
    uint16 uiDcVThresh;
    uchar8 ucTimeUnderThresh;
    void Print() const {
        std::cout << "PGN: AgsCritCmdDcV" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teCmd: " << teCmd << std::endl;
        std::cout << "teActSts: " << teActSts << std::endl;
        std::cout << "teCritType: " << teCritType << std::endl;
        std::cout << "ucDcInst: " << static_cast<int>(ucDcInst) << std::endl;
        std::cout << "uiDcVThresh: " << uiDcVThresh << std::endl;
        std::cout << "ucTimeUnderThresh: " << static_cast<int>(ucTimeUnderThresh) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "Cmd") {
        }
        if (mnem == "ActSts") {
        }
        if (mnem == "CritType") {
        }
        if (mnem == "DcInst") {
            return (ucDcInst == 0xff) ? "No Data" : (ucDcInst == 0xfe) ? "Out of range" : (ucDcInst == 0xfd) ? "Reserved" : std::to_string(ucDcInst);
        }
        if (mnem == "DcVThresh") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiDcVThresh + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiDcVThresh == 0xffff) ? "No Data" : (uiDcVThresh == 0xfffe) ? "Out of range" : (uiDcVThresh == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "TimeUnderThresh") {
            return (ucTimeUnderThresh == 0xff) ? "No Data" : (ucTimeUnderThresh == 0xfe) ? "Out of range" : (ucTimeUnderThresh == 0xfd) ? "Reserved" : std::to_string(ucTimeUnderThresh);
        }
    }
} AgsCritCmdDcV;

// Auto-generated class definition for RVC_tzPGN_AGS_CRIT_CMD_AMB_TEMP
typedef struct AgsCritCmdAmbTemp {
    uchar8 ucInst;
    RVC_teCMD_PAIR teCmd;
    RVC_teCMD_PAIR teActSts;
    RVC_teRVCAGS_CRITERION_TYPE teCritType;
    uchar8 ucThermAmbInst;
    uint16 uiAmbTempThresh;
    uchar8 ucTimeUnderThresh;
    void Print() const {
        std::cout << "PGN: AgsCritCmdAmbTemp" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teCmd: " << teCmd << std::endl;
        std::cout << "teActSts: " << teActSts << std::endl;
        std::cout << "teCritType: " << teCritType << std::endl;
        std::cout << "ucThermAmbInst: " << static_cast<int>(ucThermAmbInst) << std::endl;
        std::cout << "uiAmbTempThresh: " << uiAmbTempThresh << std::endl;
        std::cout << "ucTimeUnderThresh: " << static_cast<int>(ucTimeUnderThresh) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "Cmd") {
        }
        if (mnem == "ActSts") {
        }
        if (mnem == "CritType") {
        }
        if (mnem == "ThermAmbInst") {
            return (ucThermAmbInst == 0xff) ? "No Data" : (ucThermAmbInst == 0xfe) ? "Out of range" : (ucThermAmbInst == 0xfd) ? "Reserved" : std::to_string(ucThermAmbInst);
        }
        if (mnem == "AmbTempThresh") {
            char buffer[50];
            auto engval = 0.03125 * (float) (uiAmbTempThresh + -8736.0);
            snprintf(buffer, sizeof(buffer), "%.2f %s", engval, "deg C");
            return (uiAmbTempThresh == 0xffff) ? "No Data" : (uiAmbTempThresh == 0xfffe) ? "Out of range" : (uiAmbTempThresh == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "TimeUnderThresh") {
            return (ucTimeUnderThresh == 0xff) ? "No Data" : (ucTimeUnderThresh == 0xfe) ? "Out of range" : (ucTimeUnderThresh == 0xfd) ? "Reserved" : std::to_string(ucTimeUnderThresh);
        }
    }
} AgsCritCmdAmbTemp;

// Auto-generated class definition for RVC_tzPGN_AGS_CRIT_CMD_DC_I
typedef struct AgsCritCmdDcI {
    uchar8 ucInst;
    RVC_teCMD_PAIR teCmd;
    RVC_teCMD_PAIR teActSts;
    RVC_teRVCAGS_CRITERION_TYPE teCritType;
    uchar8 ucDcInst;
    uint16 uiDcIThresh;
    uchar8 ucTimeOverThresh;
    void Print() const {
        std::cout << "PGN: AgsCritCmdDcI" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teCmd: " << teCmd << std::endl;
        std::cout << "teActSts: " << teActSts << std::endl;
        std::cout << "teCritType: " << teCritType << std::endl;
        std::cout << "ucDcInst: " << static_cast<int>(ucDcInst) << std::endl;
        std::cout << "uiDcIThresh: " << uiDcIThresh << std::endl;
        std::cout << "ucTimeOverThresh: " << static_cast<int>(ucTimeOverThresh) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "Cmd") {
        }
        if (mnem == "ActSts") {
        }
        if (mnem == "CritType") {
        }
        if (mnem == "DcInst") {
            return (ucDcInst == 0xff) ? "No Data" : (ucDcInst == 0xfe) ? "Out of range" : (ucDcInst == 0xfd) ? "Reserved" : std::to_string(ucDcInst);
        }
        if (mnem == "DcIThresh") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiDcIThresh + -32000.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "A");
            return (uiDcIThresh == 0xffff) ? "No Data" : (uiDcIThresh == 0xfffe) ? "Out of range" : (uiDcIThresh == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "TimeOverThresh") {
            return (ucTimeOverThresh == 0xff) ? "No Data" : (ucTimeOverThresh == 0xfe) ? "Out of range" : (ucTimeOverThresh == 0xfd) ? "Reserved" : std::to_string(ucTimeOverThresh);
        }
    }
} AgsCritCmdDcI;

// Auto-generated class definition for RVC_tzPGN_AGS_CRIT_STS_AMB_TEMP
typedef struct AgsCritStsAmbTemp {
    uchar8 ucInst;
    RVC_teGENERIC_STSPAIR teCurrDemandSts;
    RVC_teGENERIC_STSPAIR teActSts;
    RVC_teRVCAGS_CRITERION_TYPE teCritType;
    uchar8 ucThermAmbInst;
    uint16 uiAmbTempThresh;
    uchar8 ucTimeUnderThresh;
    void Print() const {
        std::cout << "PGN: AgsCritStsAmbTemp" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teCurrDemandSts: " << teCurrDemandSts << std::endl;
        std::cout << "teActSts: " << teActSts << std::endl;
        std::cout << "teCritType: " << teCritType << std::endl;
        std::cout << "ucThermAmbInst: " << static_cast<int>(ucThermAmbInst) << std::endl;
        std::cout << "uiAmbTempThresh: " << uiAmbTempThresh << std::endl;
        std::cout << "ucTimeUnderThresh: " << static_cast<int>(ucTimeUnderThresh) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "CurrDemandSts") {
        }
        if (mnem == "ActSts") {
        }
        if (mnem == "CritType") {
        }
        if (mnem == "ThermAmbInst") {
            return (ucThermAmbInst == 0xff) ? "No Data" : (ucThermAmbInst == 0xfe) ? "Out of range" : (ucThermAmbInst == 0xfd) ? "Reserved" : std::to_string(ucThermAmbInst);
        }
        if (mnem == "AmbTempThresh") {
            char buffer[50];
            auto engval = 0.03125 * (float) (uiAmbTempThresh + -8736.0);
            snprintf(buffer, sizeof(buffer), "%.2f %s", engval, "deg C");
            return (uiAmbTempThresh == 0xffff) ? "No Data" : (uiAmbTempThresh == 0xfffe) ? "Out of range" : (uiAmbTempThresh == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "TimeUnderThresh") {
            return (ucTimeUnderThresh == 0xff) ? "No Data" : (ucTimeUnderThresh == 0xfe) ? "Out of range" : (ucTimeUnderThresh == 0xfd) ? "Reserved" : std::to_string(ucTimeUnderThresh);
        }
    }
} AgsCritStsAmbTemp;

// Auto-generated class definition for RVC_tzPGN_AGS_CRIT_STS_DC_V
typedef struct AgsCritStsDcV {
    uchar8 ucInst;
    RVC_teGENERIC_STSPAIR teCurrDemandSts;
    RVC_teGENERIC_STSPAIR teActSts;
    RVC_teRVCAGS_CRITERION_TYPE teCritType;
    uchar8 ucDcInst;
    uint16 uiDcVThresh;
    uchar8 ucTimeUnderThresh;
    void Print() const {
        std::cout << "PGN: AgsCritStsDcV" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teCurrDemandSts: " << teCurrDemandSts << std::endl;
        std::cout << "teActSts: " << teActSts << std::endl;
        std::cout << "teCritType: " << teCritType << std::endl;
        std::cout << "ucDcInst: " << static_cast<int>(ucDcInst) << std::endl;
        std::cout << "uiDcVThresh: " << uiDcVThresh << std::endl;
        std::cout << "ucTimeUnderThresh: " << static_cast<int>(ucTimeUnderThresh) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "CurrDemandSts") {
        }
        if (mnem == "ActSts") {
        }
        if (mnem == "CritType") {
        }
        if (mnem == "DcInst") {
            return (ucDcInst == 0xff) ? "No Data" : (ucDcInst == 0xfe) ? "Out of range" : (ucDcInst == 0xfd) ? "Reserved" : std::to_string(ucDcInst);
        }
        if (mnem == "DcVThresh") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiDcVThresh + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiDcVThresh == 0xffff) ? "No Data" : (uiDcVThresh == 0xfffe) ? "Out of range" : (uiDcVThresh == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "TimeUnderThresh") {
            return (ucTimeUnderThresh == 0xff) ? "No Data" : (ucTimeUnderThresh == 0xfe) ? "Out of range" : (ucTimeUnderThresh == 0xfd) ? "Reserved" : std::to_string(ucTimeUnderThresh);
        }
    }
} AgsCritStsDcV;

// Auto-generated class definition for RVC_tzPGN_AGS_CRIT_STS_DC_SOC
typedef struct AgsCritStsDcSoc {
    uchar8 ucInst;
    RVC_teGENERIC_STSPAIR teCurrDemandSts;
    RVC_teGENERIC_STSPAIR teActSts;
    RVC_teRVCAGS_CRITERION_TYPE teCritType;
    uchar8 ucDcInst;
    uchar8 ucDcSocThresh;
    uchar8 ucTimeUnderThresh;
    void Print() const {
        std::cout << "PGN: AgsCritStsDcSoc" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teCurrDemandSts: " << teCurrDemandSts << std::endl;
        std::cout << "teActSts: " << teActSts << std::endl;
        std::cout << "teCritType: " << teCritType << std::endl;
        std::cout << "ucDcInst: " << static_cast<int>(ucDcInst) << std::endl;
        std::cout << "ucDcSocThresh: " << static_cast<int>(ucDcSocThresh) << std::endl;
        std::cout << "ucTimeUnderThresh: " << static_cast<int>(ucTimeUnderThresh) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "CurrDemandSts") {
        }
        if (mnem == "ActSts") {
        }
        if (mnem == "CritType") {
        }
        if (mnem == "DcInst") {
            return (ucDcInst == 0xff) ? "No Data" : (ucDcInst == 0xfe) ? "Out of range" : (ucDcInst == 0xfd) ? "Reserved" : std::to_string(ucDcInst);
        }
        if (mnem == "DcSocThresh") {
            char buffer[50];
            auto engval = 0.5 * (float) (ucDcSocThresh + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "%");
            return (ucDcSocThresh == 0xff) ? "No Data" : (ucDcSocThresh == 0xfe) ? "Out of range" : (ucDcSocThresh == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "TimeUnderThresh") {
            return (ucTimeUnderThresh == 0xff) ? "No Data" : (ucTimeUnderThresh == 0xfe) ? "Out of range" : (ucTimeUnderThresh == 0xfd) ? "Reserved" : std::to_string(ucTimeUnderThresh);
        }
    }
} AgsCritStsDcSoc;

// Auto-generated class definition for RVC_tzPGN_AGS_CRIT_STS_DC_I
typedef struct AgsCritStsDcI {
    uchar8 ucInst;
    RVC_teGENERIC_STSPAIR teCurrDemandSts;
    RVC_teGENERIC_STSPAIR teActSts;
    RVC_teRVCAGS_CRITERION_TYPE teCritType;
    uchar8 ucDcInst;
    uint16 uiDcIThresh;
    uchar8 ucTimeOverThresh;
    void Print() const {
        std::cout << "PGN: AgsCritStsDcI" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teCurrDemandSts: " << teCurrDemandSts << std::endl;
        std::cout << "teActSts: " << teActSts << std::endl;
        std::cout << "teCritType: " << teCritType << std::endl;
        std::cout << "ucDcInst: " << static_cast<int>(ucDcInst) << std::endl;
        std::cout << "uiDcIThresh: " << uiDcIThresh << std::endl;
        std::cout << "ucTimeOverThresh: " << static_cast<int>(ucTimeOverThresh) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "CurrDemandSts") {
        }
        if (mnem == "ActSts") {
        }
        if (mnem == "CritType") {
        }
        if (mnem == "DcInst") {
            return (ucDcInst == 0xff) ? "No Data" : (ucDcInst == 0xfe) ? "Out of range" : (ucDcInst == 0xfd) ? "Reserved" : std::to_string(ucDcInst);
        }
        if (mnem == "DcIThresh") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiDcIThresh + -32000.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "A");
            return (uiDcIThresh == 0xffff) ? "No Data" : (uiDcIThresh == 0xfffe) ? "Out of range" : (uiDcIThresh == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "TimeOverThresh") {
            return (ucTimeOverThresh == 0xff) ? "No Data" : (ucTimeOverThresh == 0xfe) ? "Out of range" : (ucTimeOverThresh == 0xfd) ? "Reserved" : std::to_string(ucTimeOverThresh);
        }
    }
} AgsCritStsDcI;

// Auto-generated class definition for RVC_tzPGN_AGS_CRIT_STS_XFER_AC_V
typedef struct AgsCritStsXferAcV {
    uchar8 ucInst;
    RVC_teGENERIC_STSPAIR teCurrDemandSts;
    RVC_teGENERIC_STSPAIR teActSts;
    RVC_teRVCAGS_CRITERION_TYPE teCritType;
    uchar8 ucAcInst;
    uint16 uiAcVThresh;
    uchar8 ucTimeUnderThresh;
    void Print() const {
        std::cout << "PGN: AgsCritStsXferAcV" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teCurrDemandSts: " << teCurrDemandSts << std::endl;
        std::cout << "teActSts: " << teActSts << std::endl;
        std::cout << "teCritType: " << teCritType << std::endl;
        std::cout << "ucAcInst: " << static_cast<int>(ucAcInst) << std::endl;
        std::cout << "uiAcVThresh: " << uiAcVThresh << std::endl;
        std::cout << "ucTimeUnderThresh: " << static_cast<int>(ucTimeUnderThresh) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "CurrDemandSts") {
        }
        if (mnem == "ActSts") {
        }
        if (mnem == "CritType") {
        }
        if (mnem == "AcInst") {
            return (ucAcInst == 0xff) ? "No Data" : (ucAcInst == 0xfe) ? "Out of range" : (ucAcInst == 0xfd) ? "Reserved" : std::to_string(ucAcInst);
        }
        if (mnem == "AcVThresh") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiAcVThresh + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiAcVThresh == 0xffff) ? "No Data" : (uiAcVThresh == 0xfffe) ? "Out of range" : (uiAcVThresh == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "TimeUnderThresh") {
            return (ucTimeUnderThresh == 0xff) ? "No Data" : (ucTimeUnderThresh == 0xfe) ? "Out of range" : (ucTimeUnderThresh == 0xfd) ? "Reserved" : std::to_string(ucTimeUnderThresh);
        }
    }
} AgsCritStsXferAcV;

// Auto-generated class definition for RVC_tzPGN_GEN_DEMAND_CMD
typedef struct GenDemandCmd {
    RVC_teGENERIC_STSPAIR teGenDemand;
    RVC_teGENERIC_STSPAIR teQuietTimeOverride;
    RVC_teGENERIC_STSPAIR teExtActRst;
    RVC_teGENERIC_STSPAIR teManOverride;
    RVC_teGENERIC_STSPAIR teGenLock;
    uchar8 ucSetQuietTimeBeginHr;
    uchar8 ucSetQuietTimeBeginMin;
    uchar8 ucSetQuietTimeEndHr;
    uchar8 ucSetQuietTimeEndMin;
    uchar8 ucSetMinCycTime;
    void Print() const {
        std::cout << "PGN: GenDemandCmd" << std::endl;
        std::cout << "teGenDemand: " << teGenDemand << std::endl;
        std::cout << "teQuietTimeOverride: " << teQuietTimeOverride << std::endl;
        std::cout << "teExtActRst: " << teExtActRst << std::endl;
        std::cout << "teManOverride: " << teManOverride << std::endl;
        std::cout << "teGenLock: " << teGenLock << std::endl;
        std::cout << "ucSetQuietTimeBeginHr: " << static_cast<int>(ucSetQuietTimeBeginHr) << std::endl;
        std::cout << "ucSetQuietTimeBeginMin: " << static_cast<int>(ucSetQuietTimeBeginMin) << std::endl;
        std::cout << "ucSetQuietTimeEndHr: " << static_cast<int>(ucSetQuietTimeEndHr) << std::endl;
        std::cout << "ucSetQuietTimeEndMin: " << static_cast<int>(ucSetQuietTimeEndMin) << std::endl;
        std::cout << "ucSetMinCycTime: " << static_cast<int>(ucSetMinCycTime) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "GenDemand") {
        }
        if (mnem == "QuietTimeOverride") {
        }
        if (mnem == "ExtActRst") {
        }
        if (mnem == "ManOverride") {
        }
        if (mnem == "GenLock") {
        }
        if (mnem == "SetQuietTimeBeginHr") {
            return (ucSetQuietTimeBeginHr == 0xff) ? "No Data" : (ucSetQuietTimeBeginHr == 0xfe) ? "Out of range" : (ucSetQuietTimeBeginHr == 0xfd) ? "Reserved" : std::to_string(ucSetQuietTimeBeginHr);
        }
        if (mnem == "SetQuietTimeBeginMin") {
            return (ucSetQuietTimeBeginMin == 0xff) ? "No Data" : (ucSetQuietTimeBeginMin == 0xfe) ? "Out of range" : (ucSetQuietTimeBeginMin == 0xfd) ? "Reserved" : std::to_string(ucSetQuietTimeBeginMin);
        }
        if (mnem == "SetQuietTimeEndHr") {
            return (ucSetQuietTimeEndHr == 0xff) ? "No Data" : (ucSetQuietTimeEndHr == 0xfe) ? "Out of range" : (ucSetQuietTimeEndHr == 0xfd) ? "Reserved" : std::to_string(ucSetQuietTimeEndHr);
        }
        if (mnem == "SetQuietTimeEndMin") {
            return (ucSetQuietTimeEndMin == 0xff) ? "No Data" : (ucSetQuietTimeEndMin == 0xfe) ? "Out of range" : (ucSetQuietTimeEndMin == 0xfd) ? "Reserved" : std::to_string(ucSetQuietTimeEndMin);
        }
        if (mnem == "SetMinCycTime") {
            return (ucSetMinCycTime == 0xff) ? "No Data" : (ucSetMinCycTime == 0xfe) ? "Out of range" : (ucSetMinCycTime == 0xfd) ? "Reserved" : std::to_string(ucSetMinCycTime);
        }
    }
} GenDemandCmd;

// Auto-generated class definition for RVC_tzPGN_GEN_DEMAND_STS
typedef struct GenDemandSts {
    RVC_teGENERIC_STSPAIR teGenDemand;
    RVC_teGENERIC_STSPAIR teIntGenDemand;
    RVC_teGENERIC_STSPAIR teNetGenDemand;
    RVC_teGENERIC_STSPAIR teExtActDetect;
    RVC_teGENERIC_STSPAIR teManOverrideDetect;
    RVC_teGENERIC_STSPAIR teQuietTime;
    RVC_teGENERIC_STSPAIR teQuietTimeOverride;
    RVC_teGENERIC_STSPAIR teGenLock;
    uchar8 ucQuietTimeBeginHr;
    uchar8 ucQuietTimeBeginMin;
    uchar8 ucQuietTimeEndHr;
    uchar8 ucQuietTimeEndMin;
    uchar8 ucMinCycTime;
    void Print() const {
        std::cout << "PGN: GenDemandSts" << std::endl;
        std::cout << "teGenDemand: " << teGenDemand << std::endl;
        std::cout << "teIntGenDemand: " << teIntGenDemand << std::endl;
        std::cout << "teNetGenDemand: " << teNetGenDemand << std::endl;
        std::cout << "teExtActDetect: " << teExtActDetect << std::endl;
        std::cout << "teManOverrideDetect: " << teManOverrideDetect << std::endl;
        std::cout << "teQuietTime: " << teQuietTime << std::endl;
        std::cout << "teQuietTimeOverride: " << teQuietTimeOverride << std::endl;
        std::cout << "teGenLock: " << teGenLock << std::endl;
        std::cout << "ucQuietTimeBeginHr: " << static_cast<int>(ucQuietTimeBeginHr) << std::endl;
        std::cout << "ucQuietTimeBeginMin: " << static_cast<int>(ucQuietTimeBeginMin) << std::endl;
        std::cout << "ucQuietTimeEndHr: " << static_cast<int>(ucQuietTimeEndHr) << std::endl;
        std::cout << "ucQuietTimeEndMin: " << static_cast<int>(ucQuietTimeEndMin) << std::endl;
        std::cout << "ucMinCycTime: " << static_cast<int>(ucMinCycTime) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "GenDemand") {
        }
        if (mnem == "IntGenDemand") {
        }
        if (mnem == "NetGenDemand") {
        }
        if (mnem == "ExtActDetect") {
        }
        if (mnem == "ManOverrideDetect") {
        }
        if (mnem == "QuietTime") {
        }
        if (mnem == "QuietTimeOverride") {
        }
        if (mnem == "GenLock") {
        }
        if (mnem == "QuietTimeBeginHr") {
            return (ucQuietTimeBeginHr == 0xff) ? "No Data" : (ucQuietTimeBeginHr == 0xfe) ? "Out of range" : (ucQuietTimeBeginHr == 0xfd) ? "Reserved" : std::to_string(ucQuietTimeBeginHr);
        }
        if (mnem == "QuietTimeBeginMin") {
            return (ucQuietTimeBeginMin == 0xff) ? "No Data" : (ucQuietTimeBeginMin == 0xfe) ? "Out of range" : (ucQuietTimeBeginMin == 0xfd) ? "Reserved" : std::to_string(ucQuietTimeBeginMin);
        }
        if (mnem == "QuietTimeEndHr") {
            return (ucQuietTimeEndHr == 0xff) ? "No Data" : (ucQuietTimeEndHr == 0xfe) ? "Out of range" : (ucQuietTimeEndHr == 0xfd) ? "Reserved" : std::to_string(ucQuietTimeEndHr);
        }
        if (mnem == "QuietTimeEndMin") {
            return (ucQuietTimeEndMin == 0xff) ? "No Data" : (ucQuietTimeEndMin == 0xfe) ? "Out of range" : (ucQuietTimeEndMin == 0xfd) ? "Reserved" : std::to_string(ucQuietTimeEndMin);
        }
        if (mnem == "MinCycTime") {
            return (ucMinCycTime == 0xff) ? "No Data" : (ucMinCycTime == 0xfe) ? "Out of range" : (ucMinCycTime == 0xfd) ? "Reserved" : std::to_string(ucMinCycTime);
        }
    }
} GenDemandSts;

// Auto-generated class definition for RVC_tzPGN_CHG_STATS_STS
typedef struct ChgStatsSts {
    uchar8 ucInst;
    void Print() const {
        std::cout << "PGN: ChgStatsSts" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
    }
} ChgStatsSts;

// Auto-generated class definition for RVC_tzPGN_CHG_CFG_CMD1
typedef struct ChgCfgCmd1 {
    uchar8 ucInst;
    RVC_teRVC_CHG_ALG teChgAlg;
    RVC_teRVC_CHG_MODE teChgMode;
    RVC_teGENERIC_STSPAIR teBattSensPres;
    RVC_teAC_LINE_NUM teChgInstLine;
    uint16 uiBattBankSz;
    RVC_teRVC_BATT_TYPE teBattType;
    uchar8 ucMaxChgI;
    void Print() const {
        std::cout << "PGN: ChgCfgCmd1" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teChgAlg: " << teChgAlg << std::endl;
        std::cout << "teChgMode: " << teChgMode << std::endl;
        std::cout << "teBattSensPres: " << teBattSensPres << std::endl;
        std::cout << "teChgInstLine: " << teChgInstLine << std::endl;
        std::cout << "uiBattBankSz: " << uiBattBankSz << std::endl;
        std::cout << "teBattType: " << teBattType << std::endl;
        std::cout << "ucMaxChgI: " << static_cast<int>(ucMaxChgI) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "ChgAlg") {
        }
        if (mnem == "ChgMode") {
        }
        if (mnem == "BattSensPres") {
        }
        if (mnem == "ChgInstLine") {
        }
        if (mnem == "BattBankSz") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiBattBankSz + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "AH");
            return (uiBattBankSz == 0xffff) ? "No Data" : (uiBattBankSz == 0xfffe) ? "Out of range" : (uiBattBankSz == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "BattType") {
        }
        if (mnem == "MaxChgI") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucMaxChgI + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "A");
            return (ucMaxChgI == 0xff) ? "No Data" : (ucMaxChgI == 0xfe) ? "Out of range" : (ucMaxChgI == 0xfd) ? "Reserved" : (buffer);
        }
    }
} ChgCfgCmd1;

// Auto-generated class definition for RVC_tzPGN_CHG_CMD
typedef struct ChgCmd {
    uchar8 ucInst;
    RVC_teRVC_CHG_CMD teSts;
    RVC_teCMD_PAIR teDfltStatePwrUp;
    RVC_teCMD_PAIR teAutoReChgEn;
    RVC_teRVC_FORCE_CHG teForceChg;
    uint16 uiCtrlVCCCV;
    uint16 uiCtrlICCCV;
    void Print() const {
        std::cout << "PGN: ChgCmd" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teSts: " << teSts << std::endl;
        std::cout << "teDfltStatePwrUp: " << teDfltStatePwrUp << std::endl;
        std::cout << "teAutoReChgEn: " << teAutoReChgEn << std::endl;
        std::cout << "teForceChg: " << teForceChg << std::endl;
        std::cout << "uiCtrlVCCCV: " << uiCtrlVCCCV << std::endl;
        std::cout << "uiCtrlICCCV: " << uiCtrlICCCV << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "Sts") {
        }
        if (mnem == "DfltStatePwrUp") {
        }
        if (mnem == "AutoReChgEn") {
        }
        if (mnem == "ForceChg") {
        }
        if (mnem == "CtrlVCCCV") {
            return (uiCtrlVCCCV == 0xffff) ? "No Data" : (uiCtrlVCCCV == 0xfffe) ? "Out of range" : (uiCtrlVCCCV == 0xfffd) ? "Reserved" : std::to_string(uiCtrlVCCCV);
        }
        if (mnem == "CtrlICCCV") {
            return (uiCtrlICCCV == 0xffff) ? "No Data" : (uiCtrlICCCV == 0xfffe) ? "Out of range" : (uiCtrlICCCV == 0xfffd) ? "Reserved" : std::to_string(uiCtrlICCCV);
        }
    }
} ChgCmd;

// Auto-generated class definition for RVC_tzPGN_CHG_CFG_STS1
typedef struct ChgCfgSts1 {
    uchar8 ucInst;
    RVC_teRVC_CHG_ALG teChgAlg;
    RVC_teRVC_CHG_MODE teChgMode;
    RVC_teGENERIC_STSPAIR teBattSensPres;
    RVC_teAC_LINE_NUM teChgInstLine;
    RVC_teRVC_BATT_TYPE teBattType;
    uint16 uiBattBankSz;
    uint16 uiMaxChgI;
    void Print() const {
        std::cout << "PGN: ChgCfgSts1" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teChgAlg: " << teChgAlg << std::endl;
        std::cout << "teChgMode: " << teChgMode << std::endl;
        std::cout << "teBattSensPres: " << teBattSensPres << std::endl;
        std::cout << "teChgInstLine: " << teChgInstLine << std::endl;
        std::cout << "teBattType: " << teBattType << std::endl;
        std::cout << "uiBattBankSz: " << uiBattBankSz << std::endl;
        std::cout << "uiMaxChgI: " << uiMaxChgI << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "ChgAlg") {
        }
        if (mnem == "ChgMode") {
        }
        if (mnem == "BattSensPres") {
        }
        if (mnem == "ChgInstLine") {
        }
        if (mnem == "BattType") {
        }
        if (mnem == "BattBankSz") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiBattBankSz + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "AH");
            return (uiBattBankSz == 0xffff) ? "No Data" : (uiBattBankSz == 0xfffe) ? "Out of range" : (uiBattBankSz == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "MaxChgI") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiMaxChgI + -32000.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "A");
            return (uiMaxChgI == 0xffff) ? "No Data" : (uiMaxChgI == 0xfffe) ? "Out of range" : (uiMaxChgI == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} ChgCfgSts1;

// Auto-generated class definition for RVC_tzPGN_CHG_STS
typedef struct ChgSts {
    uchar8 ucInst;
    uint16 uiChgV;
    uint16 uiChgI;
    uchar8 ucChgIPerMax;
    RVC_teRVC_CHG_OP_STATE teOpState;
    RVC_teCMD_PAIR teDefltStatePwrUp;
    RVC_teCMD_PAIR teAutoReChgEn;
    RVC_teRVC_FORCE_CHG teForceChg;
    void Print() const {
        std::cout << "PGN: ChgSts" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiChgV: " << uiChgV << std::endl;
        std::cout << "uiChgI: " << uiChgI << std::endl;
        std::cout << "ucChgIPerMax: " << static_cast<int>(ucChgIPerMax) << std::endl;
        std::cout << "teOpState: " << teOpState << std::endl;
        std::cout << "teDefltStatePwrUp: " << teDefltStatePwrUp << std::endl;
        std::cout << "teAutoReChgEn: " << teAutoReChgEn << std::endl;
        std::cout << "teForceChg: " << teForceChg << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "ChgV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiChgV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiChgV == 0xffff) ? "No Data" : (uiChgV == 0xfffe) ? "Out of range" : (uiChgV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "ChgI") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiChgI + -32000.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "A");
            return (uiChgI == 0xffff) ? "No Data" : (uiChgI == 0xfffe) ? "Out of range" : (uiChgI == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "ChgIPerMax") {
            char buffer[50];
            auto engval = 0.5 * (float) (ucChgIPerMax + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "%");
            return (ucChgIPerMax == 0xff) ? "No Data" : (ucChgIPerMax == 0xfe) ? "Out of range" : (ucChgIPerMax == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "OpState") {
        }
        if (mnem == "DefltStatePwrUp") {
        }
        if (mnem == "AutoReChgEn") {
        }
        if (mnem == "ForceChg") {
        }
    }
} ChgSts;

// Auto-generated class definition for RVC_tzPGN_CHG_AC_STS3
typedef struct ChgAcSts3 {
    uchar8 ucInst;
    RVC_teAC_LINE_NUM teLine;
    RVC_teGENERIC_STSPAIR teInOut;
    RVC_teRVCAC_WAVEFORM teWaveform;
    RVC_teRVC_PHASE_STS tePhsSts;
    uint16 uiRealPwr;
    uint16 uiReactivePwr;
    uchar8 ucHarmonicDist;
    uchar8 ucCompLeg;
    void Print() const {
        std::cout << "PGN: ChgAcSts3" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teLine: " << teLine << std::endl;
        std::cout << "teInOut: " << teInOut << std::endl;
        std::cout << "teWaveform: " << teWaveform << std::endl;
        std::cout << "tePhsSts: " << tePhsSts << std::endl;
        std::cout << "uiRealPwr: " << uiRealPwr << std::endl;
        std::cout << "uiReactivePwr: " << uiReactivePwr << std::endl;
        std::cout << "ucHarmonicDist: " << static_cast<int>(ucHarmonicDist) << std::endl;
        std::cout << "ucCompLeg: " << static_cast<int>(ucCompLeg) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "Line") {
        }
        if (mnem == "InOut") {
        }
        if (mnem == "Waveform") {
        }
        if (mnem == "PhsSts") {
        }
        if (mnem == "RealPwr") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiRealPwr + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "W");
            return (uiRealPwr == 0xffff) ? "No Data" : (uiRealPwr == 0xfffe) ? "Out of range" : (uiRealPwr == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "ReactivePwr") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiReactivePwr + -32000.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "VAR");
            return (uiReactivePwr == 0xffff) ? "No Data" : (uiReactivePwr == 0xfffe) ? "Out of range" : (uiReactivePwr == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "HarmonicDist") {
            char buffer[50];
            auto engval = 0.5 * (float) (ucHarmonicDist + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "%");
            return (ucHarmonicDist == 0xff) ? "No Data" : (ucHarmonicDist == 0xfe) ? "Out of range" : (ucHarmonicDist == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "CompLeg") {
        }
    }
} ChgAcSts3;

// Auto-generated class definition for RVC_tzPGN_CHG_AC_STS2
typedef struct ChgAcSts2 {
    uchar8 ucInst;
    RVC_teAC_LINE_NUM teLine;
    RVC_teGENERIC_STSPAIR teInOut;
    uint16 uiPkV;
    uint16 uiPkI;
    uint16 uiGndI;
    uchar8 ucCap;
    void Print() const {
        std::cout << "PGN: ChgAcSts2" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teLine: " << teLine << std::endl;
        std::cout << "teInOut: " << teInOut << std::endl;
        std::cout << "uiPkV: " << uiPkV << std::endl;
        std::cout << "uiPkI: " << uiPkI << std::endl;
        std::cout << "uiGndI: " << uiGndI << std::endl;
        std::cout << "ucCap: " << static_cast<int>(ucCap) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "Line") {
        }
        if (mnem == "InOut") {
        }
        if (mnem == "PkV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiPkV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiPkV == 0xffff) ? "No Data" : (uiPkV == 0xfffe) ? "Out of range" : (uiPkV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "PkI") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiPkI + -32000.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "A");
            return (uiPkI == 0xffff) ? "No Data" : (uiPkI == 0xfffe) ? "Out of range" : (uiPkI == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "GndI") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiGndI + -32000.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "A");
            return (uiGndI == 0xffff) ? "No Data" : (uiGndI == 0xfffe) ? "Out of range" : (uiGndI == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "Cap") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucCap + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "A");
            return (ucCap == 0xff) ? "No Data" : (ucCap == 0xfe) ? "Out of range" : (ucCap == 0xfd) ? "Reserved" : (buffer);
        }
    }
} ChgAcSts2;

// Auto-generated class definition for RVC_tzPGN_CHG_AC_STS1
typedef struct ChgAcSts1 {
    uchar8 ucInst;
    RVC_teAC_LINE_NUM teLine;
    RVC_teGENERIC_STSPAIR teInOut;
    uint16 uiRmsV;
    uint16 uiRmsI;
    uint16 uiFreq;
    RVC_teGENERIC_STSPAIR teFltOpnGnd;
    RVC_teGENERIC_STSPAIR teFltOpnNeutral;
    RVC_teGENERIC_STSPAIR teFltRevPolarity;
    RVC_teGENERIC_STSPAIR teFltGndFlt;
    void Print() const {
        std::cout << "PGN: ChgAcSts1" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teLine: " << teLine << std::endl;
        std::cout << "teInOut: " << teInOut << std::endl;
        std::cout << "uiRmsV: " << uiRmsV << std::endl;
        std::cout << "uiRmsI: " << uiRmsI << std::endl;
        std::cout << "uiFreq: " << uiFreq << std::endl;
        std::cout << "teFltOpnGnd: " << teFltOpnGnd << std::endl;
        std::cout << "teFltOpnNeutral: " << teFltOpnNeutral << std::endl;
        std::cout << "teFltRevPolarity: " << teFltRevPolarity << std::endl;
        std::cout << "teFltGndFlt: " << teFltGndFlt << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "Line") {
        }
        if (mnem == "InOut") {
        }
        if (mnem == "RmsV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiRmsV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiRmsV == 0xffff) ? "No Data" : (uiRmsV == 0xfffe) ? "Out of range" : (uiRmsV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "RmsI") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiRmsI + -32000.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "A");
            return (uiRmsI == 0xffff) ? "No Data" : (uiRmsI == 0xfffe) ? "Out of range" : (uiRmsI == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "Freq") {
            char buffer[50];
            auto engval = 0.0078125 * (float) (uiFreq + 0.0);
            snprintf(buffer, sizeof(buffer), "%.2f %s", engval, "Hz");
            return (uiFreq == 0xffff) ? "No Data" : (uiFreq == 0xfffe) ? "Out of range" : (uiFreq == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "FltOpnGnd") {
        }
        if (mnem == "FltOpnNeutral") {
        }
        if (mnem == "FltRevPolarity") {
        }
        if (mnem == "FltGndFlt") {
        }
    }
} ChgAcSts1;

// Auto-generated class definition for RVC_tzPGN_INV_OPS_STS
typedef struct InvOpsSts {
    uchar8 ucInst;
    uchar8 ucTotalInst;
    uint16 uiVoltage;
    uint16 uiCurrent;
    uchar8 ucTemp;
    void Print() const {
        std::cout << "PGN: InvOpsSts" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "ucTotalInst: " << static_cast<int>(ucTotalInst) << std::endl;
        std::cout << "uiVoltage: " << uiVoltage << std::endl;
        std::cout << "uiCurrent: " << uiCurrent << std::endl;
        std::cout << "ucTemp: " << static_cast<int>(ucTemp) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "TotalInst") {
            return (ucTotalInst == 0xff) ? "No Data" : (ucTotalInst == 0xfe) ? "Out of range" : (ucTotalInst == 0xfd) ? "Reserved" : std::to_string(ucTotalInst);
        }
        if (mnem == "Voltage") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiVoltage + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiVoltage == 0xffff) ? "No Data" : (uiVoltage == 0xfffe) ? "Out of range" : (uiVoltage == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "Current") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiCurrent + -32000.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "A");
            return (uiCurrent == 0xffff) ? "No Data" : (uiCurrent == 0xfffe) ? "Out of range" : (uiCurrent == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "Temp") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucTemp + -40.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "deg C");
            return (ucTemp == 0xff) ? "No Data" : (ucTemp == 0xfe) ? "Out of range" : (ucTemp == 0xfd) ? "Reserved" : (buffer);
        }
    }
} InvOpsSts;

// Auto-generated class definition for RVC_tzPGN_INV_HV_DC_BUS_STS
typedef struct InvHvDcBusSts {
    uchar8 ucInst;
    uchar8 ucTotalInst;
    uint16 uiVoltage;
    uint16 uiCurrent;
    uchar8 ucTemp;
    void Print() const {
        std::cout << "PGN: InvHvDcBusSts" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "ucTotalInst: " << static_cast<int>(ucTotalInst) << std::endl;
        std::cout << "uiVoltage: " << uiVoltage << std::endl;
        std::cout << "uiCurrent: " << uiCurrent << std::endl;
        std::cout << "ucTemp: " << static_cast<int>(ucTemp) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "TotalInst") {
            return (ucTotalInst == 0xff) ? "No Data" : (ucTotalInst == 0xfe) ? "Out of range" : (ucTotalInst == 0xfd) ? "Reserved" : std::to_string(ucTotalInst);
        }
        if (mnem == "Voltage") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiVoltage + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiVoltage == 0xffff) ? "No Data" : (uiVoltage == 0xfffe) ? "Out of range" : (uiVoltage == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "Current") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiCurrent + -32000.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "A");
            return (uiCurrent == 0xffff) ? "No Data" : (uiCurrent == 0xfffe) ? "Out of range" : (uiCurrent == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "Temp") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucTemp + -40.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "deg C");
            return (ucTemp == 0xff) ? "No Data" : (ucTemp == 0xfe) ? "Out of range" : (ucTemp == 0xfd) ? "Reserved" : (buffer);
        }
    }
} InvHvDcBusSts;

// Auto-generated class definition for RVC_tzPGN_INV_APS_STS
typedef struct InvApsSts {
    uchar8 ucInst;
    uchar8 ucTotalInst;
    uint16 uiVoltage;
    uint16 uiCurrent;
    uchar8 ucTemp;
    void Print() const {
        std::cout << "PGN: InvApsSts" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "ucTotalInst: " << static_cast<int>(ucTotalInst) << std::endl;
        std::cout << "uiVoltage: " << uiVoltage << std::endl;
        std::cout << "uiCurrent: " << uiCurrent << std::endl;
        std::cout << "ucTemp: " << static_cast<int>(ucTemp) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "TotalInst") {
            return (ucTotalInst == 0xff) ? "No Data" : (ucTotalInst == 0xfe) ? "Out of range" : (ucTotalInst == 0xfd) ? "Reserved" : std::to_string(ucTotalInst);
        }
        if (mnem == "Voltage") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiVoltage + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiVoltage == 0xffff) ? "No Data" : (uiVoltage == 0xfffe) ? "Out of range" : (uiVoltage == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "Current") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiCurrent + -32000.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "A");
            return (uiCurrent == 0xffff) ? "No Data" : (uiCurrent == 0xfffe) ? "Out of range" : (uiCurrent == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "Temp") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucTemp + -40.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "deg C");
            return (ucTemp == 0xff) ? "No Data" : (ucTemp == 0xfe) ? "Out of range" : (ucTemp == 0xfd) ? "Reserved" : (buffer);
        }
    }
} InvApsSts;

// Auto-generated class definition for RVC_tzPGN_INV_STATS_STS
typedef struct InvStatsSts {
    uchar8 ucInst;
    uint16 uiNumDcUvDetects;
    uint16 uiNumInvAcOutOvlds;
    uint16 uiNumLdSensEngage;
    uint16 uiLowestDcV;
    uint16 uiHighestDcV;
    uint16 uiLowestAcVIn;
    uint16 uiHighestAcVIn;
    uint16 uiLowestAcVOut;
    uint16 uiHighestAcVOut;
    void Print() const {
        std::cout << "PGN: InvStatsSts" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiNumDcUvDetects: " << uiNumDcUvDetects << std::endl;
        std::cout << "uiNumInvAcOutOvlds: " << uiNumInvAcOutOvlds << std::endl;
        std::cout << "uiNumLdSensEngage: " << uiNumLdSensEngage << std::endl;
        std::cout << "uiLowestDcV: " << uiLowestDcV << std::endl;
        std::cout << "uiHighestDcV: " << uiHighestDcV << std::endl;
        std::cout << "uiLowestAcVIn: " << uiLowestAcVIn << std::endl;
        std::cout << "uiHighestAcVIn: " << uiHighestAcVIn << std::endl;
        std::cout << "uiLowestAcVOut: " << uiLowestAcVOut << std::endl;
        std::cout << "uiHighestAcVOut: " << uiHighestAcVOut << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "NumDcUvDetects") {
            return (uiNumDcUvDetects == 0xffff) ? "No Data" : (uiNumDcUvDetects == 0xfffe) ? "Out of range" : (uiNumDcUvDetects == 0xfffd) ? "Reserved" : std::to_string(uiNumDcUvDetects);
        }
        if (mnem == "NumInvAcOutOvlds") {
            return (uiNumInvAcOutOvlds == 0xffff) ? "No Data" : (uiNumInvAcOutOvlds == 0xfffe) ? "Out of range" : (uiNumInvAcOutOvlds == 0xfffd) ? "Reserved" : std::to_string(uiNumInvAcOutOvlds);
        }
        if (mnem == "NumLdSensEngage") {
            return (uiNumLdSensEngage == 0xffff) ? "No Data" : (uiNumLdSensEngage == 0xfffe) ? "Out of range" : (uiNumLdSensEngage == 0xfffd) ? "Reserved" : std::to_string(uiNumLdSensEngage);
        }
        if (mnem == "LowestDcV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiLowestDcV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiLowestDcV == 0xffff) ? "No Data" : (uiLowestDcV == 0xfffe) ? "Out of range" : (uiLowestDcV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "HighestDcV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiHighestDcV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiHighestDcV == 0xffff) ? "No Data" : (uiHighestDcV == 0xfffe) ? "Out of range" : (uiHighestDcV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "LowestAcVIn") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiLowestAcVIn + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiLowestAcVIn == 0xffff) ? "No Data" : (uiLowestAcVIn == 0xfffe) ? "Out of range" : (uiLowestAcVIn == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "HighestAcVIn") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiHighestAcVIn + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiHighestAcVIn == 0xffff) ? "No Data" : (uiHighestAcVIn == 0xfffe) ? "Out of range" : (uiHighestAcVIn == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "LowestAcVOut") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiLowestAcVOut + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiLowestAcVOut == 0xffff) ? "No Data" : (uiLowestAcVOut == 0xfffe) ? "Out of range" : (uiLowestAcVOut == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "HighestAcVOut") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiHighestAcVOut + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiHighestAcVOut == 0xffff) ? "No Data" : (uiHighestAcVOut == 0xfffe) ? "Out of range" : (uiHighestAcVOut == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} InvStatsSts;

// Auto-generated class definition for RVC_tzPGN_INV_CFG_CMD2
typedef struct InvCfgCmd2 {
    uchar8 ucInst;
    uint16 uiDcSrcSdVMax;
    uint16 uiDcSrcWrnVMin;
    uint16 uiDcSrcWrnVMax;
    void Print() const {
        std::cout << "PGN: InvCfgCmd2" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiDcSrcSdVMax: " << uiDcSrcSdVMax << std::endl;
        std::cout << "uiDcSrcWrnVMin: " << uiDcSrcWrnVMin << std::endl;
        std::cout << "uiDcSrcWrnVMax: " << uiDcSrcWrnVMax << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "DcSrcSdVMax") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiDcSrcSdVMax + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiDcSrcSdVMax == 0xffff) ? "No Data" : (uiDcSrcSdVMax == 0xfffe) ? "Out of range" : (uiDcSrcSdVMax == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "DcSrcWrnVMin") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiDcSrcWrnVMin + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiDcSrcWrnVMin == 0xffff) ? "No Data" : (uiDcSrcWrnVMin == 0xfffe) ? "Out of range" : (uiDcSrcWrnVMin == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "DcSrcWrnVMax") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiDcSrcWrnVMax + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiDcSrcWrnVMax == 0xffff) ? "No Data" : (uiDcSrcWrnVMax == 0xfffe) ? "Out of range" : (uiDcSrcWrnVMax == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} InvCfgCmd2;

// Auto-generated class definition for RVC_tzPGN_INV_CFG_CMD1
typedef struct InvCfgCmd1 {
    uchar8 ucInst;
    uint16 uiLoadSensePwrThresh;
    uint16 uiLoadSenseIntvl;
    uint16 uiDcSrcSdVMin;
    void Print() const {
        std::cout << "PGN: InvCfgCmd1" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiLoadSensePwrThresh: " << uiLoadSensePwrThresh << std::endl;
        std::cout << "uiLoadSenseIntvl: " << uiLoadSenseIntvl << std::endl;
        std::cout << "uiDcSrcSdVMin: " << uiDcSrcSdVMin << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "LoadSensePwrThresh") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiLoadSensePwrThresh + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "W");
            return (uiLoadSensePwrThresh == 0xffff) ? "No Data" : (uiLoadSensePwrThresh == 0xfffe) ? "Out of range" : (uiLoadSensePwrThresh == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "LoadSenseIntvl") {
            char buffer[50];
            auto engval = 0.5 * (float) (uiLoadSenseIntvl + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "s");
            return (uiLoadSenseIntvl == 0xffff) ? "No Data" : (uiLoadSenseIntvl == 0xfffe) ? "Out of range" : (uiLoadSenseIntvl == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "DcSrcSdVMin") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiDcSrcSdVMin + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiDcSrcSdVMin == 0xffff) ? "No Data" : (uiDcSrcSdVMin == 0xfffe) ? "Out of range" : (uiDcSrcSdVMin == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} InvCfgCmd1;

// Auto-generated class definition for RVC_tzPGN_INV_CFG_STS2
typedef struct InvCfgSts2 {
    uchar8 ucInst;
    uint16 uiDcSrcSdVMax;
    uint16 uiDcSrcWrnVMin;
    uint16 uiDcSrcWrnVMax;
    void Print() const {
        std::cout << "PGN: InvCfgSts2" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiDcSrcSdVMax: " << uiDcSrcSdVMax << std::endl;
        std::cout << "uiDcSrcWrnVMin: " << uiDcSrcWrnVMin << std::endl;
        std::cout << "uiDcSrcWrnVMax: " << uiDcSrcWrnVMax << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "DcSrcSdVMax") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiDcSrcSdVMax + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiDcSrcSdVMax == 0xffff) ? "No Data" : (uiDcSrcSdVMax == 0xfffe) ? "Out of range" : (uiDcSrcSdVMax == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "DcSrcWrnVMin") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiDcSrcWrnVMin + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiDcSrcWrnVMin == 0xffff) ? "No Data" : (uiDcSrcWrnVMin == 0xfffe) ? "Out of range" : (uiDcSrcWrnVMin == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "DcSrcWrnVMax") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiDcSrcWrnVMax + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiDcSrcWrnVMax == 0xffff) ? "No Data" : (uiDcSrcWrnVMax == 0xfffe) ? "Out of range" : (uiDcSrcWrnVMax == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} InvCfgSts2;

// Auto-generated class definition for RVC_tzPGN_INV_CFG_STS1
typedef struct InvCfgSts1 {
    uchar8 ucInst;
    uint16 uiLoadSensePwrThresh;
    uint16 uiLoadSenseIntvl;
    uint16 uiDcSrcSdVMin;
    RVC_teCMD_PAIR teInvEnOnStartUp;
    RVC_teCMD_PAIR teLdSenseEnOnStartUp;
    RVC_teCMD_PAIR teAcPassThruEnOnStartUp;
    RVC_teCMD_PAIR teGenSupportEnOnStartUp;
    void Print() const {
        std::cout << "PGN: InvCfgSts1" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "uiLoadSensePwrThresh: " << uiLoadSensePwrThresh << std::endl;
        std::cout << "uiLoadSenseIntvl: " << uiLoadSenseIntvl << std::endl;
        std::cout << "uiDcSrcSdVMin: " << uiDcSrcSdVMin << std::endl;
        std::cout << "teInvEnOnStartUp: " << teInvEnOnStartUp << std::endl;
        std::cout << "teLdSenseEnOnStartUp: " << teLdSenseEnOnStartUp << std::endl;
        std::cout << "teAcPassThruEnOnStartUp: " << teAcPassThruEnOnStartUp << std::endl;
        std::cout << "teGenSupportEnOnStartUp: " << teGenSupportEnOnStartUp << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "LoadSensePwrThresh") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiLoadSensePwrThresh + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "W");
            return (uiLoadSensePwrThresh == 0xffff) ? "No Data" : (uiLoadSensePwrThresh == 0xfffe) ? "Out of range" : (uiLoadSensePwrThresh == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "LoadSenseIntvl") {
            char buffer[50];
            auto engval = 0.5 * (float) (uiLoadSenseIntvl + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "s");
            return (uiLoadSenseIntvl == 0xffff) ? "No Data" : (uiLoadSenseIntvl == 0xfffe) ? "Out of range" : (uiLoadSenseIntvl == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "DcSrcSdVMin") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiDcSrcSdVMin + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiDcSrcSdVMin == 0xffff) ? "No Data" : (uiDcSrcSdVMin == 0xfffe) ? "Out of range" : (uiDcSrcSdVMin == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "InvEnOnStartUp") {
        }
        if (mnem == "LdSenseEnOnStartUp") {
        }
        if (mnem == "AcPassThruEnOnStartUp") {
        }
        if (mnem == "GenSupportEnOnStartUp") {
        }
    }
} InvCfgSts1;

// Auto-generated class definition for RVC_tzPGN_INV_CMD
typedef struct InvCmd {
    uchar8 ucInst;
    RVC_teCMD_PAIR teInvEn;
    RVC_teCMD_PAIR teLoadSenseEn;
    RVC_teCMD_PAIR tePassThruEn;
    RVC_teCMD_PAIR teGenSupportEn;
    RVC_teCMD_PAIR teInvEnOnStartUp;
    RVC_teCMD_PAIR teLdSenseEnOnStartUp;
    RVC_teCMD_PAIR teAcPassThruEnOnStartUp;
    RVC_teCMD_PAIR teGenSupportOnStartUp;
    void Print() const {
        std::cout << "PGN: InvCmd" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teInvEn: " << teInvEn << std::endl;
        std::cout << "teLoadSenseEn: " << teLoadSenseEn << std::endl;
        std::cout << "tePassThruEn: " << tePassThruEn << std::endl;
        std::cout << "teGenSupportEn: " << teGenSupportEn << std::endl;
        std::cout << "teInvEnOnStartUp: " << teInvEnOnStartUp << std::endl;
        std::cout << "teLdSenseEnOnStartUp: " << teLdSenseEnOnStartUp << std::endl;
        std::cout << "teAcPassThruEnOnStartUp: " << teAcPassThruEnOnStartUp << std::endl;
        std::cout << "teGenSupportOnStartUp: " << teGenSupportOnStartUp << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "InvEn") {
        }
        if (mnem == "LoadSenseEn") {
        }
        if (mnem == "PassThruEn") {
        }
        if (mnem == "GenSupportEn") {
        }
        if (mnem == "InvEnOnStartUp") {
        }
        if (mnem == "LdSenseEnOnStartUp") {
        }
        if (mnem == "AcPassThruEnOnStartUp") {
        }
        if (mnem == "GenSupportOnStartUp") {
        }
    }
} InvCmd;

// Auto-generated class definition for RVC_tzPGN_INV_STS
typedef struct InvSts {
    uchar8 ucInst;
    RVC_teRVC_INV_STS teSts;
    RVC_teGENERIC_STSPAIR teBattTempSensPres;
    RVC_teGENERIC_STSPAIR teLoadSenseEn;
    RVC_teGENERIC_STSPAIR teInverterEn;
    RVC_teGENERIC_STSPAIR tePassthruEn;
    RVC_teGENERIC_STSPAIR teGenSupportEn;
    void Print() const {
        std::cout << "PGN: InvSts" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teSts: " << teSts << std::endl;
        std::cout << "teBattTempSensPres: " << teBattTempSensPres << std::endl;
        std::cout << "teLoadSenseEn: " << teLoadSenseEn << std::endl;
        std::cout << "teInverterEn: " << teInverterEn << std::endl;
        std::cout << "tePassthruEn: " << tePassthruEn << std::endl;
        std::cout << "teGenSupportEn: " << teGenSupportEn << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
            return (ucInst == 0xff) ? "No Data" : (ucInst == 0xfe) ? "Out of range" : (ucInst == 0xfd) ? "Reserved" : std::to_string(ucInst);
        }
        if (mnem == "Sts") {
        }
        if (mnem == "BattTempSensPres") {
        }
        if (mnem == "LoadSenseEn") {
        }
        if (mnem == "InverterEn") {
        }
        if (mnem == "PassthruEn") {
        }
        if (mnem == "GenSupportEn") {
        }
    }
} InvSts;

// Auto-generated class definition for RVC_tzPGN_INV_AC_STS3
typedef struct InvAcSts3 {
    uchar8 ucInst;
    RVC_teAC_LINE_NUM teLine;
    RVC_teGENERIC_STSPAIR teInOut;
    RVC_teRVCAC_WAVEFORM teWaveform;
    RVC_teRVC_PHASE_STS tePhsSts;
    uint16 uiRealPwr;
    uint16 uiReactivePwr;
    uchar8 ucHarmonicDist;
    uchar8 ucCompLeg;
    void Print() const {
        std::cout << "PGN: InvAcSts3" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teLine: " << teLine << std::endl;
        std::cout << "teInOut: " << teInOut << std::endl;
        std::cout << "teWaveform: " << teWaveform << std::endl;
        std::cout << "tePhsSts: " << tePhsSts << std::endl;
        std::cout << "uiRealPwr: " << uiRealPwr << std::endl;
        std::cout << "uiReactivePwr: " << uiReactivePwr << std::endl;
        std::cout << "ucHarmonicDist: " << static_cast<int>(ucHarmonicDist) << std::endl;
        std::cout << "ucCompLeg: " << static_cast<int>(ucCompLeg) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "Line") {
        }
        if (mnem == "InOut") {
        }
        if (mnem == "Waveform") {
        }
        if (mnem == "PhsSts") {
        }
        if (mnem == "RealPwr") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiRealPwr + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "W");
            return (uiRealPwr == 0xffff) ? "No Data" : (uiRealPwr == 0xfffe) ? "Out of range" : (uiRealPwr == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "ReactivePwr") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiReactivePwr + -32000.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "VAR");
            return (uiReactivePwr == 0xffff) ? "No Data" : (uiReactivePwr == 0xfffe) ? "Out of range" : (uiReactivePwr == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "HarmonicDist") {
            char buffer[50];
            auto engval = 0.5 * (float) (ucHarmonicDist + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "%");
            return (ucHarmonicDist == 0xff) ? "No Data" : (ucHarmonicDist == 0xfe) ? "Out of range" : (ucHarmonicDist == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "CompLeg") {
        }
    }
} InvAcSts3;

// Auto-generated class definition for RVC_tzPGN_INV_AC_STS2
typedef struct InvAcSts2 {
    uchar8 ucInst;
    RVC_teAC_LINE_NUM teLine;
    RVC_teGENERIC_STSPAIR teInOut;
    uint16 uiPkV;
    uint16 uiPkI;
    uint16 uiGndI;
    uchar8 ucCap;
    void Print() const {
        std::cout << "PGN: InvAcSts2" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teLine: " << teLine << std::endl;
        std::cout << "teInOut: " << teInOut << std::endl;
        std::cout << "uiPkV: " << uiPkV << std::endl;
        std::cout << "uiPkI: " << uiPkI << std::endl;
        std::cout << "uiGndI: " << uiGndI << std::endl;
        std::cout << "ucCap: " << static_cast<int>(ucCap) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "Line") {
        }
        if (mnem == "InOut") {
        }
        if (mnem == "PkV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiPkV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiPkV == 0xffff) ? "No Data" : (uiPkV == 0xfffe) ? "Out of range" : (uiPkV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "PkI") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiPkI + -32000.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "A");
            return (uiPkI == 0xffff) ? "No Data" : (uiPkI == 0xfffe) ? "Out of range" : (uiPkI == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "GndI") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiGndI + -32000.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "A");
            return (uiGndI == 0xffff) ? "No Data" : (uiGndI == 0xfffe) ? "Out of range" : (uiGndI == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "Cap") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucCap + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "A");
            return (ucCap == 0xff) ? "No Data" : (ucCap == 0xfe) ? "Out of range" : (ucCap == 0xfd) ? "Reserved" : (buffer);
        }
    }
} InvAcSts2;

// Auto-generated class definition for RVC_tzPGN_INV_AC_STS1
typedef struct InvAcSts1 {
    uchar8 ucInst;
    RVC_teAC_LINE_NUM teLine;
    RVC_teGENERIC_STSPAIR teInOut;
    uint16 uiRmsV;
    uint16 uiRmsI;
    uint16 uiFreq;
    RVC_teGENERIC_STSPAIR teFltOpnGnd;
    RVC_teGENERIC_STSPAIR teFltOpnNeutral;
    RVC_teGENERIC_STSPAIR teFltRevPolarity;
    RVC_teGENERIC_STSPAIR teFltGndFlt;
    void Print() const {
        std::cout << "PGN: InvAcSts1" << std::endl;
        std::cout << "ucInst: " << static_cast<int>(ucInst) << std::endl;
        std::cout << "teLine: " << teLine << std::endl;
        std::cout << "teInOut: " << teInOut << std::endl;
        std::cout << "uiRmsV: " << uiRmsV << std::endl;
        std::cout << "uiRmsI: " << uiRmsI << std::endl;
        std::cout << "uiFreq: " << uiFreq << std::endl;
        std::cout << "teFltOpnGnd: " << teFltOpnGnd << std::endl;
        std::cout << "teFltOpnNeutral: " << teFltOpnNeutral << std::endl;
        std::cout << "teFltRevPolarity: " << teFltRevPolarity << std::endl;
        std::cout << "teFltGndFlt: " << teFltGndFlt << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "Line") {
        }
        if (mnem == "InOut") {
        }
        if (mnem == "RmsV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiRmsV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiRmsV == 0xffff) ? "No Data" : (uiRmsV == 0xfffe) ? "Out of range" : (uiRmsV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "RmsI") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiRmsI + -32000.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "A");
            return (uiRmsI == 0xffff) ? "No Data" : (uiRmsI == 0xfffe) ? "Out of range" : (uiRmsI == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "Freq") {
            char buffer[50];
            auto engval = 0.0078125 * (float) (uiFreq + 0.0);
            snprintf(buffer, sizeof(buffer), "%.2f %s", engval, "Hz");
            return (uiFreq == 0xffff) ? "No Data" : (uiFreq == 0xfffe) ? "Out of range" : (uiFreq == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "FltOpnGnd") {
        }
        if (mnem == "FltOpnNeutral") {
        }
        if (mnem == "FltRevPolarity") {
        }
        if (mnem == "FltGndFlt") {
        }
    }
} InvAcSts1;

// Auto-generated class definition for RVC_tzPGN_GEN_START_CFG_CMD
typedef struct GenStartCfgCmd {
    RVC_teRVC_GEN_TYPE teGenType;
    uchar8 ucGenPrecrankTime;
    uchar8 ucGenMaxCrankTime;
    uchar8 ucGenStopTime;
    void Print() const {
        std::cout << "PGN: GenStartCfgCmd" << std::endl;
        std::cout << "teGenType: " << teGenType << std::endl;
        std::cout << "ucGenPrecrankTime: " << static_cast<int>(ucGenPrecrankTime) << std::endl;
        std::cout << "ucGenMaxCrankTime: " << static_cast<int>(ucGenMaxCrankTime) << std::endl;
        std::cout << "ucGenStopTime: " << static_cast<int>(ucGenStopTime) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "GenType") {
        }
        if (mnem == "GenPrecrankTime") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucGenPrecrankTime + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "s");
            return (ucGenPrecrankTime == 0xff) ? "No Data" : (ucGenPrecrankTime == 0xfe) ? "Out of range" : (ucGenPrecrankTime == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "GenMaxCrankTime") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucGenMaxCrankTime + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "s");
            return (ucGenMaxCrankTime == 0xff) ? "No Data" : (ucGenMaxCrankTime == 0xfe) ? "Out of range" : (ucGenMaxCrankTime == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "GenStopTime") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucGenStopTime + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "s");
            return (ucGenStopTime == 0xff) ? "No Data" : (ucGenStopTime == 0xfe) ? "Out of range" : (ucGenStopTime == 0xfd) ? "Reserved" : (buffer);
        }
    }
} GenStartCfgCmd;

// Auto-generated class definition for RVC_tzPGN_GEN_START_CFG_STS
typedef struct GenStartCfgSts {
    RVC_teRVC_GEN_TYPE teGenType;
    uchar8 ucGenPrecrankTime;
    uchar8 ucGenMaxCrankTime;
    uchar8 ucGenStopTime;
    void Print() const {
        std::cout << "PGN: GenStartCfgSts" << std::endl;
        std::cout << "teGenType: " << teGenType << std::endl;
        std::cout << "ucGenPrecrankTime: " << static_cast<int>(ucGenPrecrankTime) << std::endl;
        std::cout << "ucGenMaxCrankTime: " << static_cast<int>(ucGenMaxCrankTime) << std::endl;
        std::cout << "ucGenStopTime: " << static_cast<int>(ucGenStopTime) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "GenType") {
        }
        if (mnem == "GenPrecrankTime") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucGenPrecrankTime + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "s");
            return (ucGenPrecrankTime == 0xff) ? "No Data" : (ucGenPrecrankTime == 0xfe) ? "Out of range" : (ucGenPrecrankTime == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "GenMaxCrankTime") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucGenMaxCrankTime + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "s");
            return (ucGenMaxCrankTime == 0xff) ? "No Data" : (ucGenMaxCrankTime == 0xfe) ? "Out of range" : (ucGenMaxCrankTime == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "GenStopTime") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucGenStopTime + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "s");
            return (ucGenStopTime == 0xff) ? "No Data" : (ucGenStopTime == 0xfe) ? "Out of range" : (ucGenStopTime == 0xfd) ? "Reserved" : (buffer);
        }
    }
} GenStartCfgSts;

// Auto-generated class definition for RVC_tzPGN_GEN_CMD
typedef struct GenCmd {
    RVC_teRVC_GEN_CMD teCmd;
    void Print() const {
        std::cout << "PGN: GenCmd" << std::endl;
        std::cout << "teCmd: " << teCmd << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Cmd") {
        }
    }
} GenCmd;

// Auto-generated class definition for RVC_tzPGN_GEN_STS2
typedef struct GenSts2 {
    RVC_teGENERIC_STSPAIR teTempSdSw;
    RVC_teGENERIC_STSPAIR teOilPresSdSw;
    RVC_teGENERIC_STSPAIR teOilLvlSw;
    RVC_teGENERIC_STSPAIR teCautionLight;
    uchar8 ucEngCoolantTemp;
    uchar8 ucEngOilPress;
    uint16 uiEngRpm;
    uint16 uiFuelRate;
    void Print() const {
        std::cout << "PGN: GenSts2" << std::endl;
        std::cout << "teTempSdSw: " << teTempSdSw << std::endl;
        std::cout << "teOilPresSdSw: " << teOilPresSdSw << std::endl;
        std::cout << "teOilLvlSw: " << teOilLvlSw << std::endl;
        std::cout << "teCautionLight: " << teCautionLight << std::endl;
        std::cout << "ucEngCoolantTemp: " << static_cast<int>(ucEngCoolantTemp) << std::endl;
        std::cout << "ucEngOilPress: " << static_cast<int>(ucEngOilPress) << std::endl;
        std::cout << "uiEngRpm: " << uiEngRpm << std::endl;
        std::cout << "uiFuelRate: " << uiFuelRate << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "TempSdSw") {
        }
        if (mnem == "OilPresSdSw") {
        }
        if (mnem == "OilLvlSw") {
        }
        if (mnem == "CautionLight") {
        }
        if (mnem == "EngCoolantTemp") {
            char buffer[50];
            auto engval = 1.0 * (float) (ucEngCoolantTemp + -40.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "deg C");
            return (ucEngCoolantTemp == 0xff) ? "No Data" : (ucEngCoolantTemp == 0xfe) ? "Out of range" : (ucEngCoolantTemp == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "EngOilPress") {
            char buffer[50];
            auto engval = 4.0 * (float) (ucEngOilPress + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "kPa");
            return (ucEngOilPress == 0xff) ? "No Data" : (ucEngOilPress == 0xfe) ? "Out of range" : (ucEngOilPress == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "EngRpm") {
            return (uiEngRpm == 0xffff) ? "No Data" : (uiEngRpm == 0xfffe) ? "Out of range" : (uiEngRpm == 0xfffd) ? "Reserved" : std::to_string(uiEngRpm);
        }
        if (mnem == "FuelRate") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiFuelRate + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "lph");
            return (uiFuelRate == 0xffff) ? "No Data" : (uiFuelRate == 0xfffe) ? "Out of range" : (uiFuelRate == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} GenSts2;

// Auto-generated class definition for RVC_tzPGN_GEN_STS1
typedef struct GenSts1 {
    RVC_teRVC_INV_STS teSts;
    uint32 ulEngRunTime;
    uchar8 ucEngLoad;
    uint16 uiStartBattV;
    void Print() const {
        std::cout << "PGN: GenSts1" << std::endl;
        std::cout << "teSts: " << teSts << std::endl;
        std::cout << "ulEngRunTime: " << ulEngRunTime << std::endl;
        std::cout << "ucEngLoad: " << static_cast<int>(ucEngLoad) << std::endl;
        std::cout << "uiStartBattV: " << uiStartBattV << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Sts") {
        }
        if (mnem == "EngRunTime") {
            return (ulEngRunTime == 0xffffffff) ? "No Data" : (ulEngRunTime == 0xfffffffe) ? "Out of range" : (ulEngRunTime == 0xfffffffd) ? "Reserved" : std::to_string(ulEngRunTime);
        }
        if (mnem == "EngLoad") {
            char buffer[50];
            auto engval = 0.5 * (float) (ucEngLoad + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "%");
            return (ucEngLoad == 0xff) ? "No Data" : (ucEngLoad == 0xfe) ? "Out of range" : (ucEngLoad == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "StartBattV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiStartBattV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiStartBattV == 0xffff) ? "No Data" : (uiStartBattV == 0xfffe) ? "Out of range" : (uiStartBattV == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} GenSts1;

// Auto-generated class definition for RVC_tzPGN_COMM_STS3
typedef struct CommSts3 {
    uint16 uiBusOffErrCnt;
    uint16 uiRxFrameDropCnt;
    uint16 uiTxFrameDropCnt;
    void Print() const {
        std::cout << "PGN: CommSts3" << std::endl;
        std::cout << "uiBusOffErrCnt: " << uiBusOffErrCnt << std::endl;
        std::cout << "uiRxFrameDropCnt: " << uiRxFrameDropCnt << std::endl;
        std::cout << "uiTxFrameDropCnt: " << uiTxFrameDropCnt << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "BusOffErrCnt") {
            return (uiBusOffErrCnt == 0xffff) ? "No Data" : (uiBusOffErrCnt == 0xfffe) ? "Out of range" : (uiBusOffErrCnt == 0xfffd) ? "Reserved" : std::to_string(uiBusOffErrCnt);
        }
        if (mnem == "RxFrameDropCnt") {
            return (uiRxFrameDropCnt == 0xffff) ? "No Data" : (uiRxFrameDropCnt == 0xfffe) ? "Out of range" : (uiRxFrameDropCnt == 0xfffd) ? "Reserved" : std::to_string(uiRxFrameDropCnt);
        }
        if (mnem == "TxFrameDropCnt") {
            return (uiTxFrameDropCnt == 0xffff) ? "No Data" : (uiTxFrameDropCnt == 0xfffe) ? "Out of range" : (uiTxFrameDropCnt == 0xfffd) ? "Reserved" : std::to_string(uiTxFrameDropCnt);
        }
    }
} CommSts3;

// Auto-generated class definition for RVC_tzPGN_COMM_STS2
typedef struct CommSts2 {
    uint32 ulTxFramesCnt;
    uint32 ulRxFramesCnt;
    void Print() const {
        std::cout << "PGN: CommSts2" << std::endl;
        std::cout << "ulTxFramesCnt: " << ulTxFramesCnt << std::endl;
        std::cout << "ulRxFramesCnt: " << ulRxFramesCnt << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "TxFramesCnt") {
            return (ulTxFramesCnt == 0xffffffff) ? "No Data" : (ulTxFramesCnt == 0xfffffffe) ? "Out of range" : (ulTxFramesCnt == 0xfffffffd) ? "Reserved" : std::to_string(ulTxFramesCnt);
        }
        if (mnem == "RxFramesCnt") {
            return (ulRxFramesCnt == 0xffffffff) ? "No Data" : (ulRxFramesCnt == 0xfffffffe) ? "Out of range" : (ulRxFramesCnt == 0xfffffffd) ? "Reserved" : std::to_string(ulRxFramesCnt);
        }
    }
} CommSts2;

// Auto-generated class definition for RVC_tzPGN_COMM_STS1
typedef struct CommSts1 {
    uint32 ulTmrCnt;
    uint16 uiRxErrCnt;
    uint16 uiTxErrCnt;
    void Print() const {
        std::cout << "PGN: CommSts1" << std::endl;
        std::cout << "ulTmrCnt: " << ulTmrCnt << std::endl;
        std::cout << "uiRxErrCnt: " << uiRxErrCnt << std::endl;
        std::cout << "uiTxErrCnt: " << uiTxErrCnt << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "TmrCnt") {
            return (ulTmrCnt == 0xffffffff) ? "No Data" : (ulTmrCnt == 0xfffffffe) ? "Out of range" : (ulTmrCnt == 0xfffffffd) ? "Reserved" : std::to_string(ulTmrCnt);
        }
        if (mnem == "RxErrCnt") {
            return (uiRxErrCnt == 0xffff) ? "No Data" : (uiRxErrCnt == 0xfffe) ? "Out of range" : (uiRxErrCnt == 0xfffd) ? "Reserved" : std::to_string(uiRxErrCnt);
        }
        if (mnem == "TxErrCnt") {
            return (uiTxErrCnt == 0xffff) ? "No Data" : (uiTxErrCnt == 0xfffe) ? "Out of range" : (uiTxErrCnt == 0xfffd) ? "Reserved" : std::to_string(uiTxErrCnt);
        }
    }
} CommSts1;

// Auto-generated class definition for RVC_tzPGN_DC_SRC_STS3
typedef struct DcSrcSts3 {
    RVC_teDC_SRC_ID teInst;
    RVC_teRVC_DEVICE_PRIORITY teDevPri;
    uchar8 ucSoh;
    uint16 uiCapRemain;
    uchar8 ucRelCap;
    uint16 uiAcRmsRipple;
    void Print() const {
        std::cout << "PGN: DcSrcSts3" << std::endl;
        std::cout << "teInst: " << teInst << std::endl;
        std::cout << "teDevPri: " << teDevPri << std::endl;
        std::cout << "ucSoh: " << static_cast<int>(ucSoh) << std::endl;
        std::cout << "uiCapRemain: " << uiCapRemain << std::endl;
        std::cout << "ucRelCap: " << static_cast<int>(ucRelCap) << std::endl;
        std::cout << "uiAcRmsRipple: " << uiAcRmsRipple << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "DevPri") {
        }
        if (mnem == "Soh") {
            char buffer[50];
            auto engval = 0.5 * (float) (ucSoh + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "%");
            return (ucSoh == 0xff) ? "No Data" : (ucSoh == 0xfe) ? "Out of range" : (ucSoh == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "CapRemain") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiCapRemain + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "AH");
            return (uiCapRemain == 0xffff) ? "No Data" : (uiCapRemain == 0xfffe) ? "Out of range" : (uiCapRemain == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "RelCap") {
            char buffer[50];
            auto engval = 0.5 * (float) (ucRelCap + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "%");
            return (ucRelCap == 0xff) ? "No Data" : (ucRelCap == 0xfe) ? "Out of range" : (ucRelCap == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "AcRmsRipple") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiAcRmsRipple + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "mV");
            return (uiAcRmsRipple == 0xffff) ? "No Data" : (uiAcRmsRipple == 0xfffe) ? "Out of range" : (uiAcRmsRipple == 0xfffd) ? "Reserved" : (buffer);
        }
    }
} DcSrcSts3;

// Auto-generated class definition for RVC_tzPGN_DC_SRC_STS2
typedef struct DcSrcSts2 {
    RVC_teDC_SRC_ID teInst;
    RVC_teRVC_DEVICE_PRIORITY teDevPri;
    uint16 uiSrcTemp;
    uchar8 ucSoc;
    uint16 uiTimeRemain;
    RVC_teTIME_REMAIN_INTERP teTimeRemainInterp;
    void Print() const {
        std::cout << "PGN: DcSrcSts2" << std::endl;
        std::cout << "teInst: " << teInst << std::endl;
        std::cout << "teDevPri: " << teDevPri << std::endl;
        std::cout << "uiSrcTemp: " << uiSrcTemp << std::endl;
        std::cout << "ucSoc: " << static_cast<int>(ucSoc) << std::endl;
        std::cout << "uiTimeRemain: " << uiTimeRemain << std::endl;
        std::cout << "teTimeRemainInterp: " << teTimeRemainInterp << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "DevPri") {
        }
        if (mnem == "SrcTemp") {
            char buffer[50];
            auto engval = 0.03125 * (float) (uiSrcTemp + -8736.0);
            snprintf(buffer, sizeof(buffer), "%.2f %s", engval, "deg C");
            return (uiSrcTemp == 0xffff) ? "No Data" : (uiSrcTemp == 0xfffe) ? "Out of range" : (uiSrcTemp == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "Soc") {
            char buffer[50];
            auto engval = 0.5 * (float) (ucSoc + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "%");
            return (ucSoc == 0xff) ? "No Data" : (ucSoc == 0xfe) ? "Out of range" : (ucSoc == 0xfd) ? "Reserved" : (buffer);
        }
        if (mnem == "TimeRemain") {
            char buffer[50];
            auto engval = 1.0 * (float) (uiTimeRemain + 0.0);
            snprintf(buffer, sizeof(buffer), "%.0f %s", engval, "Min");
            return (uiTimeRemain == 0xffff) ? "No Data" : (uiTimeRemain == 0xfffe) ? "Out of range" : (uiTimeRemain == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "TimeRemainInterp") {
        }
    }
} DcSrcSts2;

// Auto-generated class definition for RVC_tzPGN_DC_SRC_STS1
typedef struct DcSrcSts1 {
    RVC_teDC_SRC_ID teInst;
    RVC_teRVC_DEVICE_PRIORITY teDevPri;
    uint16 uiDcV;
    uint32 ulDcI;
    void Print() const {
        std::cout << "PGN: DcSrcSts1" << std::endl;
        std::cout << "teInst: " << teInst << std::endl;
        std::cout << "teDevPri: " << teDevPri << std::endl;
        std::cout << "uiDcV: " << uiDcV << std::endl;
        std::cout << "ulDcI: " << ulDcI << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Inst") {
        }
        if (mnem == "DevPri") {
        }
        if (mnem == "DcV") {
            char buffer[50];
            auto engval = 0.05000000074505806 * (float) (uiDcV + 0.0);
            snprintf(buffer, sizeof(buffer), "%.1f %s", engval, "V");
            return (uiDcV == 0xffff) ? "No Data" : (uiDcV == 0xfffe) ? "Out of range" : (uiDcV == 0xfffd) ? "Reserved" : (buffer);
        }
        if (mnem == "DcI") {
            char buffer[50];
            auto engval = 0.0010000000474974513 * (float) (ulDcI + -2000000000.0);
            snprintf(buffer, sizeof(buffer), "%.3f %s", engval, "A");
            return (ulDcI == 0xffffffff) ? "No Data" : (ulDcI == 0xfffffffe) ? "Out of range" : (ulDcI == 0xfffffffd) ? "Reserved" : (buffer);
        }
    }
} DcSrcSts1;

// Auto-generated class definition for RVC_tzPGN_SET_SYS_DATE_TIME
typedef struct SetSysDateTime {
    uchar8 ucYr;
    uchar8 ucMn;
    uchar8 ucDate;
    uchar8 ucDow;
    uchar8 ucHr;
    uchar8 ucMin;
    uchar8 ucSec;
    uchar8 ucTz;
    void Print() const {
        std::cout << "PGN: SetSysDateTime" << std::endl;
        std::cout << "ucYr: " << static_cast<int>(ucYr) << std::endl;
        std::cout << "ucMn: " << static_cast<int>(ucMn) << std::endl;
        std::cout << "ucDate: " << static_cast<int>(ucDate) << std::endl;
        std::cout << "ucDow: " << static_cast<int>(ucDow) << std::endl;
        std::cout << "ucHr: " << static_cast<int>(ucHr) << std::endl;
        std::cout << "ucMin: " << static_cast<int>(ucMin) << std::endl;
        std::cout << "ucSec: " << static_cast<int>(ucSec) << std::endl;
        std::cout << "ucTz: " << static_cast<int>(ucTz) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Yr") {
            return (ucYr == 0xff) ? "No Data" : (ucYr == 0xfe) ? "Out of range" : (ucYr == 0xfd) ? "Reserved" : std::to_string(ucYr);
        }
        if (mnem == "Mn") {
            return (ucMn == 0xff) ? "No Data" : (ucMn == 0xfe) ? "Out of range" : (ucMn == 0xfd) ? "Reserved" : std::to_string(ucMn);
        }
        if (mnem == "Date") {
            return (ucDate == 0xff) ? "No Data" : (ucDate == 0xfe) ? "Out of range" : (ucDate == 0xfd) ? "Reserved" : std::to_string(ucDate);
        }
        if (mnem == "Dow") {
            return (ucDow == 0xff) ? "No Data" : (ucDow == 0xfe) ? "Out of range" : (ucDow == 0xfd) ? "Reserved" : std::to_string(ucDow);
        }
        if (mnem == "Hr") {
            return (ucHr == 0xff) ? "No Data" : (ucHr == 0xfe) ? "Out of range" : (ucHr == 0xfd) ? "Reserved" : std::to_string(ucHr);
        }
        if (mnem == "Min") {
            return (ucMin == 0xff) ? "No Data" : (ucMin == 0xfe) ? "Out of range" : (ucMin == 0xfd) ? "Reserved" : std::to_string(ucMin);
        }
        if (mnem == "Sec") {
            return (ucSec == 0xff) ? "No Data" : (ucSec == 0xfe) ? "Out of range" : (ucSec == 0xfd) ? "Reserved" : std::to_string(ucSec);
        }
        if (mnem == "Tz") {
            return (ucTz == 0xff) ? "No Data" : (ucTz == 0xfe) ? "Out of range" : (ucTz == 0xfd) ? "Reserved" : std::to_string(ucTz);
        }
    }
} SetSysDateTime;

// Auto-generated class definition for RVC_tzPGN_SYS_DATE_TIME_STS
typedef struct SysDateTimeSts {
    uchar8 ucYr;
    uchar8 ucMn;
    uchar8 ucDate;
    uchar8 ucDow;
    uchar8 ucHr;
    uchar8 ucMin;
    uchar8 ucSec;
    uchar8 ucTz;
    void Print() const {
        std::cout << "PGN: SysDateTimeSts" << std::endl;
        std::cout << "ucYr: " << static_cast<int>(ucYr) << std::endl;
        std::cout << "ucMn: " << static_cast<int>(ucMn) << std::endl;
        std::cout << "ucDate: " << static_cast<int>(ucDate) << std::endl;
        std::cout << "ucDow: " << static_cast<int>(ucDow) << std::endl;
        std::cout << "ucHr: " << static_cast<int>(ucHr) << std::endl;
        std::cout << "ucMin: " << static_cast<int>(ucMin) << std::endl;
        std::cout << "ucSec: " << static_cast<int>(ucSec) << std::endl;
        std::cout << "ucTz: " << static_cast<int>(ucTz) << std::endl;
    }
    std::string fnGetValue(std::string mnem) {
        if (mnem == "Yr") {
            return (ucYr == 0xff) ? "No Data" : (ucYr == 0xfe) ? "Out of range" : (ucYr == 0xfd) ? "Reserved" : std::to_string(ucYr);
        }
        if (mnem == "Mn") {
            return (ucMn == 0xff) ? "No Data" : (ucMn == 0xfe) ? "Out of range" : (ucMn == 0xfd) ? "Reserved" : std::to_string(ucMn);
        }
        if (mnem == "Date") {
            return (ucDate == 0xff) ? "No Data" : (ucDate == 0xfe) ? "Out of range" : (ucDate == 0xfd) ? "Reserved" : std::to_string(ucDate);
        }
        if (mnem == "Dow") {
            return (ucDow == 0xff) ? "No Data" : (ucDow == 0xfe) ? "Out of range" : (ucDow == 0xfd) ? "Reserved" : std::to_string(ucDow);
        }
        if (mnem == "Hr") {
            return (ucHr == 0xff) ? "No Data" : (ucHr == 0xfe) ? "Out of range" : (ucHr == 0xfd) ? "Reserved" : std::to_string(ucHr);
        }
        if (mnem == "Min") {
            return (ucMin == 0xff) ? "No Data" : (ucMin == 0xfe) ? "Out of range" : (ucMin == 0xfd) ? "Reserved" : std::to_string(ucMin);
        }
        if (mnem == "Sec") {
            return (ucSec == 0xff) ? "No Data" : (ucSec == 0xfe) ? "Out of range" : (ucSec == 0xfd) ? "Reserved" : std::to_string(ucSec);
        }
        if (mnem == "Tz") {
            return (ucTz == 0xff) ? "No Data" : (ucTz == 0xfe) ? "Out of range" : (ucTz == 0xfd) ? "Reserved" : std::to_string(ucTz);
        }
    }
} SysDateTimeSts;

void printall(PGN_tzRECV_DATA *ptzRecv){
if(PGNTABLE[ptzRecv->ulPgn] == "AcFltCtrlCfgCmd1") {
    const AcFltCtrlCfgCmd1 *msg = (const AcFltCtrlCfgCmd1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AcFltCtrlCfgCmd2") {
        const AcFltCtrlCfgCmd2 *msg = (const AcFltCtrlCfgCmd2 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "AcFltCtrlCfgSts1") {
        const AcFltCtrlCfgSts1 *msg = (const AcFltCtrlCfgSts1 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "AcFltCtrlCfgSts2") {
        const AcFltCtrlCfgSts2 *msg = (const AcFltCtrlCfgSts2 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdAmbTemp") {
        const AgsCritCmdAmbTemp *msg = (const AgsCritCmdAmbTemp *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdDcI") {
        const AgsCritCmdDcI *msg = (const AgsCritCmdDcI *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdDcSoc") {
        const AgsCritCmdDcSoc *msg = (const AgsCritCmdDcSoc *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdDcV") {
        const AgsCritCmdDcV *msg = (const AgsCritCmdDcV *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdXferAcV") {
        const AgsCritCmdXferAcV *msg = (const AgsCritCmdXferAcV *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsAmbTemp") {
        const AgsCritStsAmbTemp *msg = (const AgsCritStsAmbTemp *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsDcI") {
        const AgsCritStsDcI *msg = (const AgsCritStsDcI *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsDcSoc") {
        const AgsCritStsDcSoc *msg = (const AgsCritStsDcSoc *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsDcV") {
        const AgsCritStsDcV *msg = (const AgsCritStsDcV *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsXferAcV") {
        const AgsCritStsXferAcV *msg = (const AgsCritStsXferAcV *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "AlarmCmd") {
        const AlarmCmd *msg = (const AlarmCmd *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "AlarmSts") {
        const AlarmSts *msg = (const AlarmSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "BattCmd") {
        const BattCmd *msg = (const BattCmd *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts1") {
        const BattSts1 *msg = (const BattSts1 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts11") {
        const BattSts11 *msg = (const BattSts11 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts2") {
        const BattSts2 *msg = (const BattSts2 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts3") {
        const BattSts3 *msg = (const BattSts3 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts4") {
        const BattSts4 *msg = (const BattSts4 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts6") {
        const BattSts6 *msg = (const BattSts6 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSumm") {
        const BattSumm *msg = (const BattSumm *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcFltCfgCmd1") {
        const ChgAcFltCfgCmd1 *msg = (const ChgAcFltCfgCmd1 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcFltCfgCmd2") {
        const ChgAcFltCfgCmd2 *msg = (const ChgAcFltCfgCmd2 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcFltCfgSts1") {
        const ChgAcFltCfgSts1 *msg = (const ChgAcFltCfgSts1 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcFltCfgSts2") {
        const ChgAcFltCfgSts2 *msg = (const ChgAcFltCfgSts2 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcSts1") {
        const ChgAcSts1 *msg = (const ChgAcSts1 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcSts2") {
        const ChgAcSts2 *msg = (const ChgAcSts2 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcSts3") {
        const ChgAcSts3 *msg = (const ChgAcSts3 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcSts4") {
        const ChgAcSts4 *msg = (const ChgAcSts4 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgCmd1") {
        const ChgCfgCmd1 *msg = (const ChgCfgCmd1 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgCmd2") {
        const ChgCfgCmd2 *msg = (const ChgCfgCmd2 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgCmd3") {
        const ChgCfgCmd3 *msg = (const ChgCfgCmd3 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgCmd4") {
        const ChgCfgCmd4 *msg = (const ChgCfgCmd4 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgSts1") {
        const ChgCfgSts1 *msg = (const ChgCfgSts1 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgSts2") {
        const ChgCfgSts2 *msg = (const ChgCfgSts2 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgSts3") {
        const ChgCfgSts3 *msg = (const ChgCfgSts3 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgSts4") {
        const ChgCfgSts4 *msg = (const ChgCfgSts4 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCmd") {
        const ChgCmd *msg = (const ChgCmd *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgEqlzCfgCmd") {
        const ChgEqlzCfgCmd *msg = (const ChgEqlzCfgCmd *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgEqlzCfgSts") {
        const ChgEqlzCfgSts *msg = (const ChgEqlzCfgSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgEqlzSts") {
        const ChgEqlzSts *msg = (const ChgEqlzSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgStatsSts") {
        const ChgStatsSts *msg = (const ChgStatsSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgSts") {
        const ChgSts *msg = (const ChgSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgSts2") {
        const ChgSts2 *msg = (const ChgSts2 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgSts3") {
        const ChgSts3 *msg = (const ChgSts3 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "CommSts1") {
        const CommSts1 *msg = (const CommSts1 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "CommSts2") {
        const CommSts2 *msg = (const CommSts2 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "CommSts3") {
        const CommSts3 *msg = (const CommSts3 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcCfgCmd3") {
        const DcSrcCfgCmd3 *msg = (const DcSrcCfgCmd3 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcCmd") {
        const DcSrcCmd *msg = (const DcSrcCmd *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcConnSts") {
        const DcSrcConnSts *msg = (const DcSrcConnSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts1") {
        const DcSrcSts1 *msg = (const DcSrcSts1 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts11") {
        const DcSrcSts11 *msg = (const DcSrcSts11 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts2") {
        const DcSrcSts2 *msg = (const DcSrcSts2 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts3") {
        const DcSrcSts3 *msg = (const DcSrcSts3 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts4") {
        const DcSrcSts4 *msg = (const DcSrcSts4 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts6") {
        const DcSrcSts6 *msg = (const DcSrcSts6 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "DeviceStateSync") {
        const DeviceStateSync *msg = (const DeviceStateSync *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "DiagMsg1") {
        const DiagMsg1 *msg = (const DiagMsg1 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "FileDnldBlk") {
        const FileDnldBlk *msg = (const FileDnldBlk *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "FileDnldInf") {
        const FileDnldInf *msg = (const FileDnldInf *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "FileDnldSts") {
        const FileDnldSts *msg = (const FileDnldSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "GenCmd") {
        const GenCmd *msg = (const GenCmd *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "GenDemandCmd") {
        const GenDemandCmd *msg = (const GenDemandCmd *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "GenDemandSts") {
        const GenDemandSts *msg = (const GenDemandSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "GenRst") {
        const GenRst *msg = (const GenRst *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "GenStartCfgCmd") {
        const GenStartCfgCmd *msg = (const GenStartCfgCmd *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "GenStartCfgSts") {
        const GenStartCfgSts *msg = (const GenStartCfgSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "GenSts1") {
        const GenSts1 *msg = (const GenSts1 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "GenSts2") {
        const GenSts2 *msg = (const GenSts2 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "InstAssign") {
        const InstAssign *msg = (const InstAssign *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "InstSts") {
        const InstSts *msg = (const InstSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcFltCfgCmd1") {
        const InvAcFltCfgCmd1 *msg = (const InvAcFltCfgCmd1 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcFltCfgSts1") {
        const InvAcFltCfgSts1 *msg = (const InvAcFltCfgSts1 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcSts1") {
        const InvAcSts1 *msg = (const InvAcSts1 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcSts2") {
        const InvAcSts2 *msg = (const InvAcSts2 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcSts3") {
        const InvAcSts3 *msg = (const InvAcSts3 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcSts4") {
        const InvAcSts4 *msg = (const InvAcSts4 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "InvApsSts") {
        const InvApsSts *msg = (const InvApsSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgCmd1") {
        const InvCfgCmd1 *msg = (const InvCfgCmd1 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgCmd2") {
        const InvCfgCmd2 *msg = (const InvCfgCmd2 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgCmd3") {
        const InvCfgCmd3 *msg = (const InvCfgCmd3 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgCmd4") {
        const InvCfgCmd4 *msg = (const InvCfgCmd4 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgSts1") {
        const InvCfgSts1 *msg = (const InvCfgSts1 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgSts2") {
        const InvCfgSts2 *msg = (const InvCfgSts2 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgSts3") {
        const InvCfgSts3 *msg = (const InvCfgSts3 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgSts4") {
        const InvCfgSts4 *msg = (const InvCfgSts4 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCmd") {
        const InvCmd *msg = (const InvCmd *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "InvDcSts") {
        const InvDcSts *msg = (const InvDcSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "InvHvDcBusSts") {
        const InvHvDcBusSts *msg = (const InvHvDcBusSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "InvOpsSts") {
        const InvOpsSts *msg = (const InvOpsSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "InvStatsSts") {
        const InvStatsSts *msg = (const InvStatsSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "InvSts") {
        const InvSts *msg = (const InvSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "InvTempSts") {
        const InvTempSts *msg = (const InvTempSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "InvTempSts2") {
        const InvTempSts2 *msg = (const InvTempSts2 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoAck") {
        const IsoAck *msg = (const IsoAck *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoAddrClaim") {
        const IsoAddrClaim *msg = (const IsoAddrClaim *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoCmdAddr") {
        const IsoCmdAddr *msg = (const IsoCmdAddr *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoReq") {
        const IsoReq *msg = (const IsoReq *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManAbort") {
        const IsoTpConnManAbort *msg = (const IsoTpConnManAbort *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManBam") {
        const IsoTpConnManBam *msg = (const IsoTpConnManBam *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManCts") {
        const IsoTpConnManCts *msg = (const IsoTpConnManCts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManEom") {
        const IsoTpConnManEom *msg = (const IsoTpConnManEom *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManRts") {
        const IsoTpConnManRts *msg = (const IsoTpConnManRts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpXfer") {
        const IsoTpXfer *msg = (const IsoTpXfer *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmAcStsRms") {
        const PmAcStsRms *msg = (const PmAcStsRms *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmAcXferSwSts") {
        const PmAcXferSwSts *msg = (const PmAcXferSwSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmAssocCfg") {
        const PmAssocCfg *msg = (const PmAssocCfg *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmAssocSts") {
        const PmAssocSts *msg = (const PmAssocSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmChgSts") {
        const PmChgSts *msg = (const PmChgSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmDevMdCfg") {
        const PmDevMdCfg *msg = (const PmDevMdCfg *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmDevMdSts") {
        const PmDevMdSts *msg = (const PmDevMdSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmFxccCfgCmd") {
        const PmFxccCfgCmd *msg = (const PmFxccCfgCmd *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmFxccCfgSts") {
        const PmFxccCfgSts *msg = (const PmFxccCfgSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmGetConfigParam") {
        const PmGetConfigParam *msg = (const PmGetConfigParam *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmGetStatusParam") {
        const PmGetStatusParam *msg = (const PmGetStatusParam *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmInvCtrlCmd") {
        const PmInvCtrlCmd *msg = (const PmInvCtrlCmd *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmInvLdShSts") {
        const PmInvLdShSts *msg = (const PmInvLdShSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmInvSts") {
        const PmInvSts *msg = (const PmInvSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmLithionicsCmd") {
        const PmLithionicsCmd *msg = (const PmLithionicsCmd *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmLithionicsSts") {
        const PmLithionicsSts *msg = (const PmLithionicsSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAssocCfg") {
        const PmParamAssocCfg *msg = (const PmParamAssocCfg *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAuxCfg") {
        const PmParamAuxCfg *msg = (const PmParamAuxCfg *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAuxCfgTrigDly") {
        const PmParamAuxCfgTrigDly *msg = (const PmParamAuxCfgTrigDly *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAuxCfgTrigLvl") {
        const PmParamAuxCfgTrigLvl *msg = (const PmParamAuxCfgTrigLvl *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAuxSts") {
        const PmParamAuxSts *msg = (const PmParamAuxSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamDevMdCfg") {
        const PmParamDevMdCfg *msg = (const PmParamDevMdCfg *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnNakRsp") {
        const PmPpnNakRsp *msg = (const PmPpnNakRsp *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnReadCmd") {
        const PmPpnReadCmd *msg = (const PmPpnReadCmd *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnReadRsp") {
        const PmPpnReadRsp *msg = (const PmPpnReadRsp *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnSessionCmd") {
        const PmPpnSessionCmd *msg = (const PmPpnSessionCmd *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnSessionRsp") {
        const PmPpnSessionRsp *msg = (const PmPpnSessionRsp *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnWriteCmd") {
        const PmPpnWriteCmd *msg = (const PmPpnWriteCmd *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnWriteRsp") {
        const PmPpnWriteRsp *msg = (const PmPpnWriteRsp *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmReq") {
        const PmReq *msg = (const PmReq *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmRpcCmd") {
        const PmRpcCmd *msg = (const PmRpcCmd *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmSetConfigParam") {
        const PmSetConfigParam *msg = (const PmSetConfigParam *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "PmSwVerSts") {
        const PmSwVerSts *msg = (const PmSwVerSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "ProdIdent") {
        const ProdIdent *msg = (const ProdIdent *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "ProdInfoSts") {
        const ProdInfoSts *msg = (const ProdInfoSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "SccArraySts") {
        const SccArraySts *msg = (const SccArraySts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "SccBattSts") {
        const SccBattSts *msg = (const SccBattSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd") {
        const SccCfgCmd *msg = (const SccCfgCmd *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd2") {
        const SccCfgCmd2 *msg = (const SccCfgCmd2 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd3") {
        const SccCfgCmd3 *msg = (const SccCfgCmd3 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd4") {
        const SccCfgCmd4 *msg = (const SccCfgCmd4 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd5") {
        const SccCfgCmd5 *msg = (const SccCfgCmd5 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts") {
        const SccCfgSts *msg = (const SccCfgSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts2") {
        const SccCfgSts2 *msg = (const SccCfgSts2 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts3") {
        const SccCfgSts3 *msg = (const SccCfgSts3 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts4") {
        const SccCfgSts4 *msg = (const SccCfgSts4 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts5") {
        const SccCfgSts5 *msg = (const SccCfgSts5 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCmd") {
        const SccCmd *msg = (const SccCmd *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "SccEqlzCfgCmd") {
        const SccEqlzCfgCmd *msg = (const SccEqlzCfgCmd *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "SccEqlzCfgSts") {
        const SccEqlzCfgSts *msg = (const SccEqlzCfgSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "SccEqlzSts") {
        const SccEqlzSts *msg = (const SccEqlzSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts") {
        const SccSts *msg = (const SccSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts2") {
        const SccSts2 *msg = (const SccSts2 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts3") {
        const SccSts3 *msg = (const SccSts3 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts4") {
        const SccSts4 *msg = (const SccSts4 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts5") {
        const SccSts5 *msg = (const SccSts5 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts6") {
        const SccSts6 *msg = (const SccSts6 *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "SetSysDateTime") {
        const SetSysDateTime *msg = (const SetSysDateTime *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "SimpleCalibCfg") {
        const SimpleCalibCfg *msg = (const SimpleCalibCfg *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "SwVerSts") {
        const SwVerSts *msg = (const SwVerSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "SysDateTimeSts") {
        const SysDateTimeSts *msg = (const SysDateTimeSts *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "VirtualTerminal") {
        const VirtualTerminal *msg = (const VirtualTerminal *)ptzRecv->pvStruct;
        msg->Print();
    }
else if(PGNTABLE[ptzRecv->ulPgn] == "XbResetCmd") {
        const XbResetCmd *msg = (const XbResetCmd *)ptzRecv->pvStruct;
        msg->Print();
    }
}