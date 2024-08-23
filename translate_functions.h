#include <iostream>
#include <string>
#include <map>
#include <cmath>

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

std::string fnChgEn2ChgCmdSts(const std::string& param_value) const{
    if( CHGEN_TO_CHGCMDSTS.count(param_value) <= 0 ){
        cout << "wrong param " << param_value << endl;
        return param_value;
    }
    else{
        return CHGEN_TO_CHGCMDSTS.at(param_value);
    }
}

std::string fnStartEqlz2ChgCmdSts(const std::string& param_value) const{
    if (param_value == "On") {
        return "Start Equalization";
    } else {
        return "Data Not Available";
    }
}

std::string fnSts2DefBattTemp(const std::string& param_value) const{
    std::string sts_value = fnGetValue("DefBattTemp");

    if( STS_TO_DEFBATTTEMP.count(sts_value) <= 0 ){
        cout << "wrong param " << sts_value << endl;
        return sts_value;
    }
    else{
        return STS_TO_DEFBATTTEMP.at(sts_value);
    }
}

std::string fnDefBattTemp2Cmd(const std::string& param_value) const{

    if(DEFBATTTEMP_TO_CMD.count(param_value) <= 0){
        cout << "wrong param " << param_value << endl;
        return "25 deg C";
    }
    else{
        return DEFBATTTEMP_TO_CMD.at(param_value);
    }
}

std::string fnSccSts2ChgOpState(const std::string& param_value) const{
    std::string sts_value = fnGetValue("OpState");
    if (sts_value == "Do Not Charge") {
        sts_value = "Not Charging";
    } else if (sts_value == "Undefined Source Decides") {
        sts_value = "Disabled";
    }
    return sts_value;
}

std::string fnInvSts2InvEnDis(const std::string& param_value) const{
    std::string sts_value = fnGetValue("Sts");
    return sts_value == "Disabled" ? "Off" : "On";
}

std::string fnPvArrayPower(const std::string& param_value) const{
    long double voltage = fnGetFloatValue("ArrayV");
    long double current = fnGetFloatValue("ArrayI");
    int power = static_cast<int>(std::round(voltage * current));

    return std::to_string(power) + " W";
}

std::string fnInvChgSts4OutACVolt2Enum(const std::string& param_value) const{
    long double float_value = fnGetFloatValue("OutACVolt");

    if (float_value < 109.0) {
        return "108 V";
    } else if (float_value > 115.0) {
        return "120 V";
    } else {
        return "110 V";
    }
    
}

std::string fnSts2InvFaultRecovery(const std::string& param_value) const{
    std::string sts_value = fnGetValue("OvrFltRecEnDis");

    if( STS_TO_INVFAULTRECOVERY.count(sts_value) <= 0){
        cout << "wrong param " << sts_value << endl;
        return sts_value;
    }
    else{
        return STS_TO_INVFAULTRECOVERY.at(sts_value);
    }
}

std::string fnInvFaultRecovery2Cmd(const std::string& param_value) const{
    if( INVFAULTRECOVERY_TO_CMD.count(param_value) <= 0 ){
        cout << "wrong param " << param_value << endl;
        return "Unknown";
    }
    else{
        return INVFAULTRECOVERY_TO_CMD.at(param_value);
    }
}

std::string fnSts2PwrSaveTime(const std::string& param_value) const{
    std::string sts_value = fnGetValue("PwrSvHr");
    if (sts_value.substr(0, 1) == "0") {
        sts_value = "Off";
    }

    return sts_value;
}

std::string fnPwrSaveTime2Cmd(const std::string& param_value) const{
    if (param_value == "Off") {
        return "0 h";
    }

    return param_value;
}

std::string fnSts2BatteryType(const std::string& param_value) const{
    std::string sts_value = fnGetValue("BattType");
    if (sts_value == "Custom Li-Ion For CC/CV") {
        sts_value = "Custom";
    }

    return sts_value;
}

std::string fnBatteryType2Cmd(const std::string& param_value) const{
    if (param_value == "Custom") {
        return "Custom Li-Ion For CC/CV";
    }

    return param_value;
}

std::string fnSts2ChargerEnable(const std::string& param_value) const{
    std::string sts_value = fnGetValue("OpState");
    if (sts_value == "Undefined Source Decides") {
        return "Off";
    } else {
        return "On";
    }
}

std::string fnChargerEnable2Cmd(const std::string& param_value) const{
    if (param_value == "On") {
        return "Enable Charger";
    } else {
        return "Disable";
    }
}

std::string fnSts2TempComp(const std::string& param_value) const{
    int int_value = -1 * fnGetRawValue("TempCompConst");
    std::string sts_value = std::to_string(int_value) + " mV/degC";

    return sts_value;
}

std::string fnTempComp2Cmd(const std::string& param_value) const{
    std::string cmd_value = param_value;

    cmd_value.erase(std::remove_if(cmd_value.begin(), cmd_value.end(), [](char c) { return c == '-'; }), cmd_value.end());

    return cmd_value;
}