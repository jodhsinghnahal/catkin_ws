#include <string>
#include <map>
#include <regex>
#include <vector>

// Global variables
std::string TOPIC_PREFIX = "xnet";

std::map<std::string, std::string> FUNC_MAP = {
    {"Inverter Charger", "invchg"},
    {"Charger", "scc"},
    {"Inverter", "inv"},
    {"Battery Monitor", "battmon"},
    {"Battery Management System", "bms"},
    {"Service Tool", "service"}
};

std::regex SUBSCRIPTION_MATCHER(R"(^([^/]+)/(.*)$)");
std::regex ASSOC_MATCHER(R"(^PmAssocSts(\w+)(\d)$)");

std::map<std::string, std::string> CHGEN_TO_CHGCMDSTS = {
    {"On", "Enable Charger"},
    {"Off", "Disable"}
};

std::map<std::string, std::string> DEFBATTTEMP_TO_CMD = {
    {"Cold (10 deg C)", "10 deg C"},
    {"Warm (25 deg C)", "25 deg C"},
    {"Hot (40 deg C)", "40 deg C"}
};

std::map<std::string, std::string> STS_TO_DEFBATTTEMP = {
    {"10 deg C", "Cold (10 deg C)"},
    {"25 deg C", "Warm (25 deg C)"},
    {"40 deg C", "Hot (40 deg C)"}
};

std::map<std::string, std::string> STS_TO_INVFAULTRECOVERY = {
    {"On", "Auto"},
    {"Off", "Manual"}
};

std::map<std::string, std::string> INVFAULTRECOVERY_TO_CMD = {
    {"Auto", "On"},
    {"Manual", "Off"}
};

// const double REREQ_TIMEOUT = 5.0;
// std::mutex IDENT_LOCK;

std::regex SETTING_MATCHER(R"(^\s*(-?\d+\.?\d*))");

std::vector<std::string> ZRPM_BMS_MODELS = {"0884-0310-12", "0884-0410-12", "0884-0205-24", "0884-0100-51",
                                            "0884-0310-12-01", "0884-0410-12-01"};

std::vector<std::string> SRNE_MPPT_MODELS = {"MC4860N15"};

const int DC_PRIORITY_TIMEOUT = 6;
const int PPN_CYCLE_SKIPS = 5;

const double RSP_DEFAULT_TIMEOUT = 1.0;
const double PPN_CMD_TIMEOUT = 0.5;
const int PPN_CMD_MAX_TRIES = 4;
const double PPN_REQ_TIMEOUT = 0.5;
const int PPN_REQ_MAX_TRIES = 3;

const std::string SERIALNUM_PATH = "/etc/xgw_serialnum";
const std::string MODEL_PATH = "/etc/xgw_model";

// Transmit timers
// std::vector<std::pair<int, int, int>> TxTimers = {
//     { _rvc.RVC_PGN_ISO_ADDR_CLAIM, _rvc.PGN_ADDR_GLOBAL, 100 },
//     { _rvc.RVC_PGN_DIAG_MSG1, _rvc.PGN_ADDR_GLOBAL, 500 }
// };

// Logging
// std::shared_ptr<spdlog::logger> Logger;

// Function definitions

// void fnSetupLogging(const std::string& default_level = "INFO") {
//     // ... (implementation as in Python)
// }

std::string fnChgEn2ChgCmdSts(const std::string& param_value) {
    return CHGEN_TO_CHGCMDSTS.at(param_value);
}

std::string fnStartEqlz2ChgCmdSts(const std::string& param_value) {
    return param_value == "On" ? "Start Equalization" : "Data Not Available";
}

std::string fnSts2DefBattTemp(const std::string& msg) {
    std::string sts_value = msg.fnGetValue("DefBattTemp");
    return STS_TO_DEFBATTTEMP.at(sts_value);
}

std::string fnDefBattTemp2Cmd(const std::string& param_value) {
    return DEFBATTTEMP_TO_CMD.at(param_value);
}

std::string fnSccSts2ChgOpState(const std::string& msg) {
    std::string sts_value = msg.fnGetValue("OpState");
    if (sts_value == "Do Not Charge") {
        sts_value = "Not Charging";
    } else if (sts_value == "Undefined Source Decides") {
        sts_value = "Disabled";
    }
    return sts_value;
}

std::string fnInvSts2InvEnDis(const std::string& msg) {
    std::string sts_value = msg.fnGetValue("Sts");
    return sts_value == "Disabled" ? "Off" : "On";
}

double fnPvArrayPower(const std::string& msg) {
    double voltage = msg.fnGetFloatValue("ArrayV");
    double current = msg.fnGetFloatValue("ArrayI");
    return voltage * current;
}

std::string fnXnetFunc(const std::string& rvcfunc) {
    if (FUNC_MAP.count(rvcfunc)) {
        return FUNC_MAP.at(rvcfunc);
    }
    return rvcfunc.replace(" ", "").lower();
}

bool fnQualPasses(const std::string& msg, const std::vector<std::pair<std::string, std::string>>& qual) {
    for (const auto& qualtup : qual) {
        const std::string& sig = qualtup.first;
        const std::string& testval = qualtup.second;
        const std::string& val = msg.fnGetValue(sig);
        if (testval != val) {
            return false;
        }
    }
    return true;
}

std::string fnInvChgOpState(const std::string& opmode, const std::string& invstate, const std::string& chgstate) {
    std::string opstate = "unknown";
    if (opmode == "Safe" || opmode == "Hibernate" || opmode == "Power Save" || opmode == "Diagnostic" || opmode == "Remote Power Off") {
        opstate = "Safe";
    } else if (chgstate == "Bulk" || chgstate == "Absorption" || chgstate == "Float" || chgstate == "Overcharge" ||
               chgstate == "Equalize" || chgstate == "Constant Voltage Constant Current") {
        opstate = "Charging";
    } else if (invstate == "Invert") {
        opstate = "Inverting";
    }
    return opstate;
}

std::string fnInvOpState(const std::string& invstate) {
    return invstate == "Invert" ? "Inverting" : invstate;
}

std::string fnInvChgSts4OutACVolt2Enum(const std::string& msg) {
    std::string sts_value = msg.fnGetValue("OutACVolt");
    std::smatch match;
    if (std::regex_match(sts_value, match, SETTING_MATCHER)) {
        float float_value = std::stof(match[1]);
        if (float_value < 109.0) {
            sts_value = "108 V";
        } else if (float_value > 115.0) {
            sts_value = "120 V";
        } else {
            sts_value = "110 V";
        }
    }
    return sts_value;
}

std::string fnSts2InvFaultRecovery(const std::string& msg) {
    std::string sts_value = msg.fnGetValue("OvrFltRecEnDis");
    return STS_TO_INVFAULTRECOVERY.at(sts_value);
}

std::string fnInvFaultRecovery2Cmd(const std::string& param_value) {
    return INVFAULTRECOVERY_TO_CMD.at(param_value