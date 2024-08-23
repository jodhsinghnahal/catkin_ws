// export LD_LIBRARY_PATH=.
#include <stdio.h>     // Support for printf 
#include <pthread.h>   // Posix threads
// extern "C" {
#include "rvcudefs.h" //PGN definitions and Source Addresses
#include "xbudefs.h"
#include "xbgdefs.h"
#include "xbwincb.h"
#include "pgnudefs.h" //PGN receive and transmit flags
// #include "xbudefs.h" //Device function codes and group function
// }
#include <queue>
#include <iostream>
using namespace std;
#include <random>
#include <string.h>  
#include <unistd.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <map>
#include "print_functions3.h"
#include <sys/time.h>
#include<signal.h>
#include <chrono>
#include <thread>
// #include <mosquitto.h>
#include <cstring>
#include <thread>
#include <mutex>
#include <sstream>
#include <algorithm>
#include "XnetParams.h"
#include <fnmatch.h>
#include <utility>
#include "json.hpp"
#include <set>
#include <regex>
#include <fstream>

using nlohmann::json;

std::ofstream mqtt("mqtt.txt");
std::ofstream send("send.txt");
std::ofstream recv("recv.txt");
std::ofstream can("can.txt");
std::ofstream times("times.txt");
std::ofstream errors("errors.txt");


// void disable_sigalarm() {
//     sigset_t mask;
//     sigemptyset(&mask);
//     sigaddset(&mask, SIGALRM);

//     if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0) {
//         perror("Failed to block SIGALRM");
//     }
// }

// void enable_sigalarm() {
//     sigset_t mask;
//     sigemptyset(&mask);
//     sigaddset(&mask, SIGALRM);

//     if (sigprocmask(SIG_UNBLOCK, &mask, NULL) < 0) {
//         perror("Failed to unblock SIGALRM");
//     }
// }

// #include <nlohmann/json.hpp>

std::mutex devices_mutex;

// struct mosquitto *mosq = mosquitto_new(nullptr, true, nullptr);

auto start_time = std::chrono::high_resolution_clock::now();

#define HEAPSIZE 32768
// #define HEAPSIZE 8192


#define XB_MAX_NODES 64
#define TICKS_PER_SEC 100
#define SEC_PER_TICK (1.0 / TICKS_PER_SEC)
#define MIN_TICK_PERIOD 0.002

#define CAN_BAUD_20K 0
#define CAN_BAUD_125K 1
#define CAN_BAUD_250K 2
#define CAN_BAUD_500K 3
#define CAN_BAUD_1M 4

#define MyNetAddr RVC_eDFLT_SRC_ADDR_CONTROL_PANEL
#define UNIQUE_NUMBER  0x1FFFFF

#define eSTATUS_OK 1
#define eSTATUS_ERR 2

#define REREQ_TIMEOUT 5

int c;

typedef enum {
    STATE_IDLE,
    STATE_SENDING_PGN,
    STATE_WAITING_RESPONSE,
    STATE_PROCESSING_RESPONSE
} State;


#include <csignal>
#include <execinfo.h>
#include <cstdlib>

void signalHandler(int signum) {
    std::cerr << "Segmentation fault occurred. Exiting program." << std::endl;

    // Print a backtrace of the program execution
    void* array[10];
    size_t size = backtrace(array, 10);
    backtrace_symbols_fd(array, size, STDERR_FILENO);

    exit(signum);
}

long defaultTxTimers[2][3] = {
    {RVC_PGN_DIAG_MSG1, PGN_ADDR_GLOBAL, 500},
    {RVC_PGN_ISO_ADDR_CLAIM, PGN_ADDR_GLOBAL, 100}
};

// Models of ZRPM BMS
// std::vector<std::string> ZRPM_BMS_MODELS = {
//     "0884-0310-12", "0884-0410-12", "0884-0205-24", "0884-0100-51",
//     "0884-0310-12-01", "0884-0410-12-01"
// };

// // Models of SRNE MPPT Solar Charge Controllers
// std::vector<std::string> SRNE_MPPT_MODELS = {
//     "MC4860N15"
// };

XB_tzPGN_ISO_ADDR_CLAIM pDefaultName;

// Helper function to split string by delimiter
std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// int dupe_count = 0;

void printSubscriptions(const std::unordered_map<std::string, std::set<std::tuple<std::string, std::vector<std::tuple<std::string, std::string>>, std::string, std::string>>>& subscriptions) {
    for (const auto& entry : subscriptions) {
        std::cout << "Key: " << entry.first << std::endl;

        for (const auto& tupleEntry : entry.second) {
            std::cout << "Tuple Elements:" << std::endl;
            std::cout << "  - " << std::get<0>(tupleEntry) << std::endl;

            for (const auto& innerTuple : std::get<1>(tupleEntry)) {
                std::cout << "    Inner Tuple Elements:" << std::endl;
                std::cout << "      - " << std::get<0>(innerTuple) << " : " << std::get<1>(innerTuple) << std::endl;
            }

            std::cout << "  - " << std::get<2>(tupleEntry) << std::endl;
            std::cout << "  - " << std::get<3>(tupleEntry) << std::endl;
        }

        std::cout << "---------------------------" << std::endl;
    }
}

class SubscriptionList {
public:
    // std::vector<std::string> fixed_subscriptions = {"status",
    //                                 //  "inv*/OpState",
    //                                 //  "scc*/OpState",
    //                                 //  "invchg*/OpMode",
    //                                 //  "inv*/InvOpState",
    //                                 //  "invchg*/ChgOpState",
    //                                  "Alerts",
    //                                 //  "*batt*/HiVoltLimitSts", 
    //                                  };
    std::vector<std::string> active_subscriptions = {"status", "Alerts"};
    std::unordered_map<string, std::tuple<string, string>> ppn_subscriptions = {};
    std::unordered_map<std::string, std::set< std::tuple<std::string, std::vector<std::tuple<std::string, std::string>>, std::string, std::string>> > subscriptions = {};
    int state = STATE_IDLE;
    string make;
    string model;
    string serial;
    string name = "unk?";
    string func = "unk"; 
    string opmode = "Operating";
    string invstate = "";
    string chgstate = "";
    // string type = "INVCHG";
    bool is_dupe = false;
    int inst = -1; 
    int inst2 = -1; 
    int addr = -1;
    ParamCmdMap param_map;
    ParamCmdMap cmd_map;
    Fault fault_lookup;
    Warning warning_lookup;
    std::unordered_map<std::string, std::chrono::time_point<std::chrono::high_resolution_clock>> msgtimes;
    bool sent = false;

    void fnSendRequest(std::string pgn){
        uint32 ulPgn;
        
        if(PGNREVERSE.find(pgn) != PGNREVERSE.end()){
            // disable_sigalarm();
            // cout << "disable" << endl;
            ulPgn = PGNREVERSE[pgn].first;
            // cout << ulPgn << endl;

            if(ulPgn == RVC_PGN_PM_REQ){
                if(!sent){
                    // XBMSG_fnControlDynamic(ulPgn, true);
                    // cout << "153" << endl;
                    RVC_tzPGN_PM_REQ pmreq;
                    pmreq.teMsgId = RVC_ePROP_MSG_ID_REQUEST;
                    pmreq.teReqId = static_cast<RVC_tePROP_MSG_ID>(PGNREVERSE[pgn].second);
                    pmreq.ucParam1 = 0;
                    pmreq.ucParam2 = 0;
                    // pmreq.uiCrc = 0;
                    // cout << pmreq.teReqId << endl;
                    // cout << (int)addr << endl;

                    XBMSG_fnSendEx( ulPgn, PGNREVERSE["PmReq"].second, &pmreq, sizeof( pmreq ), addr );
                    sent = true;
                }
            }
            else{
                // cout << "163" << endl;
                // cout << ulPgn << endl;
                // cout << addr << endl;
                XBMSG_fnControlDynamic(ulPgn, true);
                XBMSG_fnISOReq(ulPgn, addr);
            }
            // enable_sigalarm();
            // cout << "enable" << endl;
        }
        else{
            cout << "no pgn" << endl;
            return;
        }
    }

    void fnPollSubscriptions() {
        for (auto& pgn : subscriptions) {
            auto& values = pgn.second;
            string pgn_mnem = pgn.first;
            if (msgtimes.find(pgn_mnem) == msgtimes.end() ||
                std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - msgtimes[pgn_mnem]).count() > REREQ_TIMEOUT) {
                if (pgn_mnem.empty()) {
                    continue;
                }

                // if (pgn_mnem.starts_with("PmAssocSts")) {
                //     // Extract association type and instance
                //     std::smatch match;
                //     const std::regex ASSOC_MATCHER(R"(^PmAssocSts(\w+)(\d)$)");
                //     if (std::regex_match(pgn_mnem, match, ASSOC_MATCHER)) {
                //         std::string assoc_type = match[1];
                //         int assoc_inst = std::stoi(match[2]);

                //         // devcon.fnRequestPmAssocSts(assoc_type, assoc_inst);

                //         std::cout << "Association type: " << association_type << std::endl;
                //         std::cout << "Instance number: " << instance_number << std::endl;
                //     }
                // } else {
                    // Simple ISO request or parameterless PmReq
                    // std::cout << "939" << std::endl;
                    // std::cout << pgn_mnem << std::endl;
                    times << "time send: " << pgn_mnem << " inst: " << inst << " time: " << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - msgtimes[pgn_mnem]).count() << endl;
                    fnSendRequest(pgn_mnem);

                    // Wait for response
                    // std::unique_lock<std::mutex> lock(mutex);
                    // if (!cv.wait_for(lock, std::chrono::milliseconds(RESPONSE_TIMEOUT),
                    //                      [this, pgn_mnem] { return received_responses.find(pgn_mnem) != received_responses.end(); })) {
                    //     Logger.warning(f"Timeout after requesting {pgn_mnem}");
                    // }
                    // received_responses.erase(pgn_mnem);
                // }
            }
        }
    }

    void fnInvChgOpState() {
        std::string opstate = "unknown";

        if (opmode == "Safe" || opmode == "Hibernate" || opmode == "Power Save" ||
            opmode == "Diagnostic" || opmode == "Remote Power Off") {
            opstate = "Safe";
        } else if (chgstate == "Bulk" || chgstate == "Absorption" || chgstate == "Float" ||
                chgstate == "Overcharge" || chgstate == "Equalize" || chgstate == "Constant Voltage Constant Current") {
            opstate = "Charging";
        } else if (invstate == "Invert") {
            opstate = "Inverting";
        } else {
            opstate = invstate;
        }

        string topic =  "xnet/sts/" + name + "/OpState";
        //mosquitto_publish(mosq, nullptr, topic.c_str(), opstate.size(), opstate.c_str(), 0, false);
    }

    void fnUpdateSubs(std::vector<std::string> subs){
        // cout << "309999" << endl;
    
    // for (const std::string& sub : subs) {
    //     std::cout << sub << std::endl;
    // }
        ppn_subscriptions.clear();
        subscriptions.clear();
        for (const std::string& topic : subs) {
            // std::cout << topic << std::endl;
            mqtt << topic << endl;
            // std::vector<std::string> parts = split(topic, '/');

            // if (parts.size() != 2) {
            //     std::cerr << "Invalid topic structure in update subs: " << topic << std::endl;
            //     return;
            // }

            // string topic_dev = parts[0];
            // string topic_param = parts[1];
            // cout << topic_dev << endl;
            // cout << topic << endl;

            // if (fnmatch(topic_param.c_str(), topic_dev.c_str(), 0) == 0){
            if(param_map.find(topic) != param_map.end()){
                auto [pgn, sig, qual, translator] = param_map[topic];
                if (pgn == "ProdIdent"){
                    mqtt << "it is prod" << endl;
                    continue;
                }

                if (pgn == "PmPpnReadRsp"){
                    mqtt << "it is PmPpnReadRsp" << endl;
                    ppn_subscriptions[sig] = std::make_tuple(translator, topic);
                }
                else{
                    mqtt << "insert- pgn: " << pgn << " translator: " << translator << " topic: " << topic << " qual: " << endl;
                    for (const auto& tuple : qual) {
                        mqtt << std::get<0>(tuple) << ", " << std::get<1>(tuple) << endl;
                    }
                    // if(subscriptions.find(pgn) == subscriptions.end()){
                    //     subscriptions[pgn] = std::vector< std::tuple<std::string, std::vector<std::tuple<std::string, std::string>>, std::string, std::string>>();
                    // }
                    // if (std::find(subscriptions[pgn].begin(), subscriptions[pgn].end(), (sig, qual, translator, topic_param)) == subscriptions[pgn].end()) {
                    subscriptions[pgn].insert(std::make_tuple(sig, qual, translator, topic));
                    mqtt << "inserted" << endl;
                    // }
                }
            }
            else{
                // cout << "couldnt find in param map" << endl;
            }
            //}

        }
    }

    void fnUpdateFixedSubscriptions(){
        if(func == "invchg"){
            active_subscriptions.push_back("OpState");
            active_subscriptions.push_back("OpMode");
            active_subscriptions.push_back("InvOpState");
            active_subscriptions.push_back("ChgOpState");
        }
        else if (func == "scc"){
            active_subscriptions.push_back("OpState");
        }
        else if(func == "bms"){
            active_subscriptions.push_back("HiVoltLimitSts");
        }

        fnUpdateSubs(active_subscriptions);
        // cout << "376" << endl;
        // printSubscriptions(subscriptions);

    }
    void fnUpdateSubscriptions(){
        fnUpdateSubs(active_subscriptions);
        // cout << "379" << endl;
        // printSubscriptions(subscriptions);

    }

    void fnChangeName(int newinst, int newinst2=-1, bool dupe=false){
        cout << "356" << endl;
        cout << newinst << endl;
        cout << dupe << endl;
        cout << func << endl;
        cout << addr << endl;
        string newname;
        string topic;

        if(func == "bms"){
            newname = "bank" + to_string(newinst) + "batt" + to_string(newinst2);
        }
        else{
            newname = func + to_string(newinst);
        }

        // if(is_dupe && !dupe){
        //     dupe_count--;
        // }
        if (dupe){
            newname = newname + "_node" + to_string(addr);
            is_dupe = true;
        }
        else{
            is_dupe = false;
        }
        cout << newname << endl;
        cout << name << endl;

        if (newname == name){
            // cout << "398" << endl;
            // cout << newname << endl;
            // cout << name << endl;
            return;
        }

        if (inst != -1 && func != "unk"){
            // if(is_dupe){
            //     dupe_count--;
            // }
            const char* message = "offline";
            topic =  "xnet/sts/" + name + "/status";
            //mosquitto_publish(mosq, nullptr, topic.c_str(), strlen(message), message, 0, true);
        }
        
        inst = newinst;
        inst2 = newinst2;
        name = newname;
        
        // if(is_dupe){
        //     dupe_count++;
        // }
        // cout << "412" << endl;
        // cout << dupe_count << endl;
        // json alerts;
        // alerts["faults"] = json::array();
        // alerts["warnings"] = json::array();

        // if(dupe_count >= 1){
        //     json fault = {
        //         {"code", 1},
        //         {"desc", "Duplicate device instances found"}
        //     };
        //     alerts["faults"].push_back(fault);
        // }
        // string message = alerts.dump();
        
        // //mosquitto_publish(mosq, nullptr, "xnet/sts/rvc/Alerts", message.size(), message.c_str(), 0, true);

        topic = "xnet/sts/" + name + "/status";
        //mosquitto_publish(mosq, nullptr, topic.c_str(), strlen("online"), "online", 0, true);
        topic = "xnet/sts/" + name + "/network";
        //mosquitto_publish(mosq, nullptr, topic.c_str(), strlen("rvc"), "rvc", 0, true);

        topic = "xnet/sts/" + name + "/Manufacturer";
        //mosquitto_publish(mosq, nullptr, topic.c_str(), make.size(), make.c_str(), 0, true);
        topic = "xnet/sts/" + name + "/Model";
        //mosquitto_publish(mosq, nullptr, topic.c_str(), model.size(), model.c_str(), 0, true);
        topic = "xnet/sts/" + name + "/SerialNumber";
        //mosquitto_publish(mosq, nullptr, topic.c_str(), serial.size(), serial.c_str(), 0, true);

    }
};

//{'PmDevMdSts': [('CurrentMode', [], None, 'OpMode')], 'InvSts': [('Sts', [], None, 'InvOpState')], 'ChgSts': [('OpState', [], None, 'ChgOpState')], 'DiagMsg1': [(None, [], None, 'Alerts')]}
// {'PmDevMdSts': [('CurrentMode', [], None, 'OpMode')], 'InvSts': [('Sts', [], None, 'InvOpState')], 'ChgSts': [('OpState', [], None, 'ChgOpState')], 'DiagMsg1': [(None, [], None, 'Alerts')], 'DCSrcSts1': [('DcV', [], None, 'BattVolts'), ('DcI', [], None, 'BattCurrent')], 'InvAcSts1': [('RmsV', [('Line', 'Line 1')], None, 'AcOut1Line1RmsVolts'), ('RmsI', [('Line', 'Line 1')], None, 'AcOut1Line1RmsCurrent')], 'ChgAcSts1': [('RmsV', [('Line', 'Line 1')], None, 'AcIn1Line1RmsVolts'), ('RmsI', [('Line', 'Line 1')], None, 'AcIn1Line1RmsCurrent')]}
std::unordered_map<uchar8, SubscriptionList> devices;

void fnPublishInstAlerts(){
    int i =0 ;
    json alerts;
    alerts["faults"] = json::array();
    alerts["warnings"] = json::array();

    for (auto& dev: devices){
        if (dev.second.is_dupe == true){
            i++;
        }
    }
    if ( i > 0){
        json fault = {
            {"code", 1},
            {"desc", "Duplicate device instances found"}
        };
        alerts["faults"].push_back(fault);
    }
    string message = alerts.dump();
    //mosquitto_publish(mosq, nullptr, "xnet/sts/rvc/Alerts", message.size(), message.c_str(), 0, true);
}

void fnHandleInstChange(SubscriptionList& device, int newinst, int newinst2 = -1) {
    bool dupes = false;
    // auto old_dupes;

    for (auto& dev : devices) {
        if (dev.second.addr != device.addr) {
            if (dev.second.func == device.func) {
                if ((dev.second.inst == newinst) && (dev.second.inst2 == newinst2)) {
                    dupes = true;
                    cout << "542" << endl;
                    cout << dev.second.inst << endl;
                    cout << dev.second.inst2 << endl;
                    cout << true << endl;
                    dev.second.fnChangeName(dev.second.inst, dev.second.inst2, true);
                }
                // if (devmgr.inst != nullptr) {
                //     if ((dm.inst == devmgr.inst) && (dm.inst2 == devmgr.inst2)) {
                //         old_dupes.push_back(&dm);
                //     }
                // }
            }
        }
    }

    cout << "552" << endl;
    cout << newinst << endl;
    cout << newinst2 << endl;
    cout << dupes << endl;
    device.fnChangeName(newinst, newinst2, dupes);
    fnPublishInstAlerts();
}

void SetName() {
    pDefaultName.ucDeviceInstanceLower = 0;
    pDefaultName.ucDeviceInstanceUpper = 0;
    pDefaultName.teDeviceFunction = XB_eFUNC_CODE_DIAGNOSTIC_TERMINAL;
    pDefaultName.ulUniqueNumber = UNIQUE_NUMBER;
}

void fnExternCheckErrorsCB(schar8 *pscTag) {
    std::cout << "Check Errors Callback called" << std::endl;
    errors << (int) pscTag << endl;
    errors << (int) *pscTag << endl;
    errors << (int) XBINIT_fnGetErrorNum() << endl;
}

// typedef enum eNETWORK_STATE
// {
//     XB_eNETWORK_STATE_ERROR_ACTIVE      =   0,
//     XB_eNETWORK_STATE_ERROR_PASSIVE     =   1,
//     XB_eNETWORK_STATE_BUS_OFF           =   2,
//     XB_eNETWORK_STATE_UNKNOWN           =   3
// } XB_teNETWORK_STATE;

void fnExternSetNetIndicCB(XB_teNETWORK_STATE teBusState, tucBOOL tucOn) {
    errors << "Set Net Indication Callback called" << std::endl;
    errors << "Bus state is: " << teBusState << endl;
    // Callback implementation
}

XB_tzSEND_TIMERS fnDefaultTxTimeouts(long (*DefaultTxTimers)[3] = defaultTxTimers) {
    printf("jkd\n");

    // Initialize default timers
    XB_tzSEND_TIMERS defaultTimers;
    int numElements = sizeof(defaultTimers.atzTimers) / sizeof(defaultTimers.atzTimers[0]);

    // Set default values
    for (int i = 0; i < numElements; i++) {
        defaultTimers.atzTimers[i].ulParmGrpNum = 0xffffffff;
        defaultTimers.atzTimers[i].ucXmitAddr = 0xff;
        defaultTimers.atzTimers[i].uiXmitPeriod = 0xffff;
    }

    // Override with specific values from DefaultTxTimers
    for (int i = 0; i < numElements; i++) {
        
        long pgn = DefaultTxTimers[i][0];
        long addr = DefaultTxTimers[i][1];
        long period = DefaultTxTimers[i][2];

        defaultTimers.atzTimers[i].ulParmGrpNum = pgn;
        defaultTimers.atzTimers[i].ucXmitAddr = addr;
        defaultTimers.atzTimers[i].uiXmitPeriod = period;
    }

    return defaultTimers;
}

XB_tzRECV_TIMERS fnDefaultRxTimeouts(){
    XB_tzRECV_TIMERS defaultTimers;
    int numElements = sizeof(defaultTimers.atzTimers) / sizeof(defaultTimers.atzTimers[0]);

    // Set default values
    for (int i = 0; i < numElements; i++) {
        defaultTimers.atzTimers[i].ulParmGrpNum = 0xFFFFFFFF;
        defaultTimers.atzTimers[i].uiTimeout = 0xFFFF;
    }
    defaultTimers.atzTimers[0].ulParmGrpNum = RVC_PGN_DIAG_MSG1;
    defaultTimers.atzTimers[0].uiTimeout = 700;

    return defaultTimers;
}

tuiSTATUS fnExternLoadCB(XB_tePARAM_TYPE teType, XB_teCFG_TYPE teCfgType, void *pData, uint16 uiSize) {
    std::cout << "Load Callback called" << std::endl;
    // Callback implementation
    if(teType == XB_ePARAM_TYPE_ADDRESS){
        errors << "Load Callback called 1" << std::endl;
        if (uiSize == 1){
            uchar8 buf[1] = { MyNetAddr };
            memmove(pData, buf, uiSize);
            return eSTATUS_OK;
        }
    }
    else if (teType == XB_ePARAM_TYPE_NAME){
        errors << "Load Callback called 2" << std::endl;
        if (uiSize == PGN_NAME_LEN){
            uchar8* shortname = (uchar8*)calloc(PGN_NAME_LEN, sizeof(uchar8));
            XBADDR_fnNAME2ShortNAME(&pDefaultName, shortname);
            memmove(pData, shortname, uiSize);
            free(shortname);
            return eSTATUS_OK;
        }
    }
    else if (teType == XB_ePARAM_TYPE_TX_TIMEOUTS){
        errors << "Load Callback called 3" << std::endl;
        XB_tzSEND_TIMERS defaulttimers = fnDefaultTxTimeouts();
        if(uiSize == sizeof(defaulttimers)){
            memmove(pData, &defaulttimers, uiSize);
            return eSTATUS_OK;
        }
        else{
            printf("In _rvc fnLoad TxTo %d %d", uiSize, sizeof(defaulttimers) );
        }
    }
    else if (teType == XB_ePARAM_TYPE_RX_TIMEOUTS){
        errors << "Load Callback called 4" << std::endl;
        XB_tzRECV_TIMERS defaulttimers = fnDefaultRxTimeouts();
        if(uiSize == sizeof(defaulttimers)){
            memmove(pData, &defaulttimers, uiSize);
            return eSTATUS_OK;
        }
        else{
            printf("In _rvc fnLoad RxTo %d %d", uiSize, sizeof(defaulttimers) );
        }
    }

    return eSTATUS_ERR; 
}

tuiSTATUS fnExternSaveCB(XB_tePARAM_TYPE teType, XB_teCFG_TYPE teCfgType, void *pData, uint16 uiSize) {
    errors << "Save Callback called" << std::endl;
    // Callback implementation
    return 0; 
}

void fnExternNodeChangeCB(XB_teNODE_CHANGE teChange, uchar8 ucOldAddr, uchar8 ucNewAddr, XB_tzPGN_ISO_ADDR_CLAIM *ptzNAME) {
    errors << "Node Change Callback called" << std::endl;
    // Callback implementation
    // std::unique_lock<std::mutex> lock(devices_mutex);
    if (teChange == XB_eNODE_CHANGE_NEW_NODE){
        c = ucNewAddr;
        devices[ucNewAddr].addr = ucNewAddr;
        // devices[ucNewAddr].subscriptions.push_back("hi");
        // for (const auto& device : devices) {
        // std::cout << "Device Address: " << static_cast<int>(device.first) << std::endl;
        // const auto& subscriptions = device.second.subscriptions;
        // std::cout << "Subscriptions:" << std::endl;
        // for (const auto& sub : subscriptions) {
        //     std::cout << " - " << sub << std::endl;
        // }
        // std::cout << std::endl;
        // }
        printf("New node found at address %d\n", ucNewAddr);
        errors << "New node found at address " << (int) ucNewAddr << std::endl;
    }
    else if (teChange == XB_eNODE_CHANGE_ADDRESS){
        printf("Node %d changed address to %d", ucOldAddr, ucNewAddr);
        errors << "Node " << (int)ucOldAddr << " changed address to " << (int)ucNewAddr;
        SubscriptionList& old_devmgr = devices[ucOldAddr];
        SubscriptionList& new_devmgr = devices[ucNewAddr];
        new_devmgr.addr = ucNewAddr;

        new_devmgr.func = old_devmgr.func;
        new_devmgr.inst = old_devmgr.inst;
        new_devmgr.inst2 = old_devmgr.inst2;
        new_devmgr.name = old_devmgr.name;
        new_devmgr.subscriptions = old_devmgr.subscriptions;

        devices.erase(ucOldAddr);
    }
    else if (teChange == XB_eNODE_CHANGE_BUMPED_OFF || teChange == XB_eNODE_CHANGE_TIMED_OUT){
        SubscriptionList& old_devmgr = devices[ucOldAddr];
        
        const char* message = "offline";
        string topic =  "xnet/sts/" + old_devmgr.name + "/status";
        //mosquitto_publish(mosq, nullptr, topic.c_str(), strlen(message), message, 0, true);

        int i = 0;
        int address = -1;
        if(old_devmgr.is_dupe){
            for(auto & dev: devices){
                if(dev.second.addr != ucOldAddr){
                    if ((dev.second.func == old_devmgr.func)
                            && (dev.second.inst == old_devmgr.inst)
                            && (dev.second.inst2 == old_devmgr.inst2)){
                                address = dev.second.addr;
                                i++;
                            }
                }
            }
        }

        if(i==1 && address != -1){
            cout << "745" << endl;
            cout << devices[address].inst << endl;
            cout << devices[address].inst2 << endl;
            cout << false << endl;

            devices[address].fnChangeName(devices[address].inst, devices[address].inst2, false);
        }

        devices.erase(ucOldAddr);
        errors << (int)ucOldAddr << " gone offline" << endl;
        cout << (int)ucOldAddr << " gone offline" << endl;
        fnPublishInstAlerts();

    }
}

static void fnInstallRxHandler( uint32 ulPgn,
				void ( *pfnRecv )( PGN_tzRECV_DATA *ptzRecv ) )
{
    PGN_tzPGN_INFO const * ptzPgnInfo;
    PGN_tzPGN_DESC *ptzDesc;

    // Get the Info record for this PGN
    ptzPgnInfo = XBPACK_fnGetPgnInfo( ulPgn, ( sint16 )XB_eGRP_FUNC_VALUE_DO_NOT_CARE );

    // If Info record found
    if ( ptzPgnInfo != NULL )
    {
        // Copy the rx handler to the descriptor for PGN
        ptzDesc = (PGN_tzPGN_DESC *) ptzPgnInfo->pktzDesc;
        if ( ptzDesc != NULL )
        {
            ptzDesc->pfnRecv = pfnRecv;
        }
    }
}

void fnListen( uint32 ulPgn,
	       tucBOOL bEnable, 
	       void ( *pfnRecv )( PGN_tzRECV_DATA *ptzRecv ) )
{
    // Make sure that the callback function is enabled if turning on
    if ( bEnable )
    {
        fnInstallRxHandler( ulPgn, pfnRecv );
    }

    XBMSG_fnControlDynamic( ulPgn, bEnable );
}

void fnRecvProdIdent(PGN_tzRECV_DATA *ptzRecv){
    cout << "579" << endl;
    if(PGNTABLE[make_pair(ptzRecv->ulPgn, ptzRecv->siGrpFunc)] == "ProdIdent"){
        devices[ptzRecv->ucSrcAddr].msgtimes["ProdIdent"] = std::chrono::high_resolution_clock::now();
        cout << "581" << endl;
    // const ProdIdent *msg = (const ProdIdent *)ptzRecv->pvStruct;
    ProdIdent msg(ptzRecv->pvStruct);
    cout << "749" << endl;

    uchar8 src = ptzRecv->ucSrcAddr;
    string make = devices[src].make = msg.fnGetValue("Make");
    string model = devices[src].model = msg.fnGetValue("Model");
    string serial = devices[src].serial = msg.fnGetValue("Serial");
                            // self.func, self.param_map, self.cmd_map, self.fault_lookup, self.warning_lookup = MODEL_MAP[self.model]
    if (MODEL_MAP.find(model) != MODEL_MAP.end()) {
        devices[src].func = std::get<0>(MODEL_MAP[model]);
        devices[src].param_map = std::get<1>(MODEL_MAP[model]);
        devices[src].cmd_map = std::get<2>(MODEL_MAP[model]);
        devices[src].fault_lookup = std::get<3>(MODEL_MAP[model]);
        devices[src].warning_lookup = std::get<4>(MODEL_MAP[model]);
    } else {
        cout << "not supporting device" << endl;
        cout << serial << endl;
    }
    // devices[ptzRecv->ucSrcAddr].func = std::get<0>MODEL_MAP[serial];
    // if(!devices[src].name.empty()){
    //     string name = devices[src].name;
    //     string topic = "xnet/sts/" + name + "/Manufacturer";
    //     //mosquitto_publish(mosq, nullptr, topic.c_str(), make.size(), make.c_str(), 0, true);
    //     topic = "xnet/sts/" + name + "/Model";
    //     //mosquitto_publish(mosq, nullptr, topic.c_str(), model.size(), model.c_str(), 0, true);
    //     topic = "xnet/sts/" + name + "/SerialNumber";
    //     //mosquitto_publish(mosq, nullptr, topic.c_str(), serial.size(), serial.c_str(), 0, true);

    // }
    }
}

void fnRecvInstSts(PGN_tzRECV_DATA *ptzRecv){
    cout << "597" << endl;
    if(PGNTABLE[make_pair(ptzRecv->ulPgn, ptzRecv->siGrpFunc)] == "InstSts"){
        devices[ptzRecv->ucSrcAddr].msgtimes["InstSts"] = std::chrono::high_resolution_clock::now();
        cout << "599" << endl;
        InstSts msg(ptzRecv->pvStruct);
        int newinst = msg.fnGetRawValue("BaseInst");
        if (devices[ptzRecv->ucSrcAddr].inst != newinst){
            fnHandleInstChange(devices[ptzRecv->ucSrcAddr], newinst);
        }
    }
}

void fnRecvSccSts(PGN_tzRECV_DATA *ptzRecv){
    if(PGNTABLE[make_pair(ptzRecv->ulPgn, ptzRecv->siGrpFunc)] == "SccSts"){
        cout << "599" << endl;
        SccSts msg(ptzRecv->pvStruct);
        int newinst = msg.fnGetRawValue("Inst");
        if (devices[ptzRecv->ucSrcAddr].inst != newinst){
            fnHandleInstChange(devices[ptzRecv->ucSrcAddr], newinst);
        }
    }
}

void fnRecvBattSts6(PGN_tzRECV_DATA *ptzRecv) {
    if(PGNTABLE[make_pair(ptzRecv->ulPgn, ptzRecv->siGrpFunc)] == "BattSts6"){
        cout << "599" << endl;
        BattSts6 msg(ptzRecv->pvStruct);
        int bankinst = msg.fnGetRawValue("DcInst");
        int battinst = msg.fnGetRawValue("BattInst");
        if (devices[ptzRecv->ucSrcAddr].inst != bankinst || devices[ptzRecv->ucSrcAddr].inst2 != battinst){
            fnHandleInstChange(devices[ptzRecv->ucSrcAddr], bankinst, battinst);
        }
    }
}

std::queue<PGN_tzRECV_DATA*> recvDataQueue;

void fnExternRecvCB(PGN_tzRECV_DATA *ptzRecv) {
    recvDataQueue.push(ptzRecv);
    return;

}

void recvCB() {
    // printall(ptzRecv);
    // std::this_thread::sleep_for(std::chrono::milliseconds(500));
    // cout << "770" << endl;
    // cout << ptzRecv->ulPgn << endl;
    // cout << ptzRecv->siGrpFunc << endl;
    // cout << PGNTABLE[make_pair(ptzRecv->ulPgn, ptzRecv->siGrpFunc)] << endl;
    // std::unique_lock<std::mutex> lock(devices_mutex);
    if (!recvDataQueue.empty()) {
    PGN_tzRECV_DATA* ptzRecv = recvDataQueue.front(); 
    recvDataQueue.pop(); 

    SubscriptionList& device = devices[ptzRecv->ucSrcAddr];
    if(RVC_PGN_PM_REQ == ptzRecv->ulPgn) {
        device.sent = false;
        cout << "800" << endl;
    }
    if(device.state == STATE_PROCESSING_RESPONSE){
        string pgn = PGNTABLE[make_pair(ptzRecv->ulPgn, ptzRecv->siGrpFunc)];

        device.msgtimes[pgn] = std::chrono::high_resolution_clock::now();
        // times << "inst: " << device.inst << " time recv: " << (device.msgtimes[pgn]).count() << endl;
        auto time_since_epoch = std::chrono::duration_cast<std::chrono::seconds>(device.msgtimes[pgn] - start_time).count();
        times << "pgn: " << pgn << " inst: " << device.inst << " time recv: " << time_since_epoch << endl;

        if (device.subscriptions.find(pgn) != device.subscriptions.end()) {
            // std::cout << "Key exists in the map." << std::endl;
            if (message_types.find(pgn) != message_types.end()) {
                std::shared_ptr<MessageBase> message_instance = message_types[pgn](ptzRecv->pvStruct);
                // cout << "836" << endl;
                // cout << pgn << endl;
                // cout << device.subscriptions[pgn].size() << endl;
                for (const auto& values : device.subscriptions[pgn]) {
                    if (message_instance->fnQualPasses(get<1>(values))){
                        if(pgn == "DiagMsg1"){
                            //DiagMsgHandler.fnUpdateAlerts(self, msg)
                            // cout << "  DiagMsg1" << endl;
                        }
                        // else if (pgn == "DCSrcSts1"){
                        //     cout << "834" << endl;
                        //     cout << pgn << endl;
                        //     string value = message_instance->fnGetValue(get<0>(values));
                        //     string topic = "xnet/sts/" + device.name + "/" + get<3>(values);
                        //     cout << topic << endl;
                        //     cout << value << endl;
                        //     //mosquitto_publish(mosq, nullptr, topic.c_str(), value.size(), value.c_str(), 0, true);
                        
                        // 834
                        // InvSts
                        // Handling MQTT Subscribe: xgw, []
                        // xnet/sts/invchg3_node67/E
                        // no pgn
                        // Segmentation fault occurred. Exiting program.

                        // }
                        else{                    
                            // cout << "834" << endl;
                            // cout << pgn << endl;
                            string signal = get<0>(values);
                            string func = get<2>(values);
                            string value = message_instance->fnGetValue(signal);
                            if (!func.empty()){
                                value = message_instance->fnTranslate(func);
                            }

                            string param = get<3>(values);
                            string topic = "xnet/sts/" + device.name + "/" + param;
                            // cout << topic << endl;
                            // cout << value << endl;
                            //fnTranslate
                            //translator
                            //mosquitto_publish(mosq, nullptr, topic.c_str(), value.size(), value.c_str(), 0, false);

                            if (param == "OpMode") {
                                device.opmode = value;
                                device.fnInvChgOpState();
                            } else if (param == "InvOpState") {
                                device.invstate = value;
                                device.fnInvChgOpState();
                            } else if (param == "ChgOpState") {
                                device.chgstate = value;
                                device.fnInvChgOpState();
                            } 
            
                        }
                    }
                    else{
                        cout << "qual fail" << endl;
                    }
                }
            }
            else{
                cout << "no message type " << pgn << endl;
            }
            
        } else {
            // std::cout << "Key does not exist in the map." << std::endl;
        }
    }
    }
    // if(PGNTABLE[make_pair(ptzRecv->ulPgn, ptzRecv->siGrpFunc)] == "InstSts"){

    // }
    // return;
}

bool init(){
    // Set each callback function
    void (*checkErrorsCB)(schar8 *) = &fnExternCheckErrorsCB;
    void (*setNetIndicCB)(XB_teNETWORK_STATE, tucBOOL) = &fnExternSetNetIndicCB;
    tuiSTATUS (*loadCB)(XB_tePARAM_TYPE, XB_teCFG_TYPE, void *, uint16) = &fnExternLoadCB;
    tuiSTATUS (*saveCB)(XB_tePARAM_TYPE, XB_teCFG_TYPE, void *, uint16) = &fnExternSaveCB;
    void (*nodeChangeCB)(XB_teNODE_CHANGE, uchar8, uchar8, XB_tzPGN_ISO_ADDR_CLAIM *) = &fnExternNodeChangeCB;
    void (*recvCB)(PGN_tzRECV_DATA *) = &fnExternRecvCB;

    // Assign callbacks
    XBWIN_fnSetCheckErrorsCB(checkErrorsCB);
    XBWIN_fnSetSetNetIndicCB(setNetIndicCB);
    XBWIN_fnSetLoadCB(loadCB);
    XBWIN_fnSetSaveCB(saveCB);
    XBWIN_fnSetNodeChangeCB(nodeChangeCB);
    XBWIN_fnSetRecvXbMsgCB(recvCB);
    XBWIN_fnSetRecvGeneralMsgCB(recvCB);
    fnListen( RVC_PGN_PROD_IDENT, TRUE, fnRecvProdIdent );
    fnListen( RVC_PGN_INST_STS, TRUE, fnRecvInstSts );
    fnListen( RVC_PGN_SCC_STS, TRUE, fnRecvSccSts );
    fnListen( RVC_PGN_BATT_STS6, TRUE, fnRecvBattSts6 );

    printf("%d\n", XBMEM_fnCtor(malloc(HEAPSIZE * sizeof(char)), HEAPSIZE));
    if (XBMEM_fnCtor(malloc(HEAPSIZE * sizeof(char)), HEAPSIZE) != true){
        printf("XBMEM_fnCtor failed\n");
        return false;
    }

    srand( time(NULL) );
    int range = 0xffffff + 1;
    int randseed = rand() % range;

    XBADDR_fnCtor(randseed, XB_MAX_NODES);

    if (XBINIT_fnInit(0, CAN_BAUD_250K, TICKS_PER_SEC) != true){        
        printf("XB init failed with error %d from source %d\n", XBINIT_fnGetErrorNum(), XBINIT_fnGetErrorSrc());
        XBPLATFORM_fnCheckErrors();
        XBMEM_fnDtor();
        return false;
    }

    XBINIT_fnNetCtrl( XB_eNET_CTRL_RESUME );

    if (XBADDR_fnBeginClaiming() != true){
        printf("Failed to start address claiming\n");
        XBPLATFORM_fnCheckErrors();
        XBMEM_fnDtor();
        return false;
    }

    return true;
}

void tick(){
    bool online = false;
    auto prev_time = std::chrono::high_resolution_clock::now();
    double sleep_time = SEC_PER_TICK;
    cout << "964" << endl;
    cout << sleep_time << endl;
    int perf_count = TICKS_PER_SEC;
    cout << perf_count << endl;


    while(1){
    // cout << "tick sending" << endl;
        XBINIT_fnTick();
        // std::this_thread::sleep_for(std::chrono::duration<double>(sleep_time));

        // if (!online) {
        //     if (XBADDR_fnOnLine()) {
        //         online = true;
        //     }
        // }

        // perf_count -= 1;
        // if (perf_count <= 0) {
        //     perf_count = TICKS_PER_SEC;
        //     auto now = std::chrono::high_resolution_clock::now();
        //     std::chrono::duration<double> elapsed = now - prev_time;
        //     double error = (1.0 - elapsed.count()) / TICKS_PER_SEC;
        //     // std::cout << "Tick error = " << error << std::endl;
        //     sleep_time += error;
        //     if (sleep_time < MIN_TICK_PERIOD) {
        //         // std::cout << "Tick handling overload" << std::endl;
        //         sleep_time = MIN_TICK_PERIOD;
        //     }

        //     prev_time = now;
        // }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

}

// void on_connect(struct mosquitto *mosq, void *obj, int reason_code) {
//     if (reason_code != 0) {
//         std::cerr << "Connection failed, reason: " << reason_code << std::endl;
//     } else {
//         std::cout << "Connected to the broker!" << std::endl;

//         const char* message = "online";
//         //mosquitto_publish(mosq, nullptr, "xnet/sts/rvc/status", strlen(message), message, 0, true);
//        // mosquitto_subscribe(mosq, nullptr, "xnet/cmd/#", 0);
//         //mosquitto_subscribe(mosq, nullptr, "xnet/#", 0);
//     }
// }

// Handler functions
void fnHandleMqttCmd(const std::string& devname, const std::string& param, const std::string& payload) {
    std::cout << "Handling MQTT Command: " << devname << ", " << param << ", " << payload << std::endl;
    // std::unique_lock<std::mutex> lock(devices_mutex);
    for (auto& device : devices) {
        if (device.second.name == devname) {
            if(device.second.cmd_map.find(param) != device.second.cmd_map.end()){
                auto [pgn_mnem, sig_mnem, qual, translator] = device.second.cmd_map[param];
                std::string sig_value = payload;

                // if (!translator.empty()){
                //     // translator(payload);
                // }

                if (pgn_mnem == "PmPpnWriteCmd"){

                }
                else{
                    if (message_types_def.find(pgn_mnem) != message_types_def.end()) {
                        std::shared_ptr<MessageBase> message_instance = message_types_def[pgn_mnem]();

                        message_instance->Print();
                        message_instance->fnSetValue("Inst", std::to_string(device.second.inst) );
                        if (!translator.empty()){
                            sig_value = message_instance->fnTranslate(translator, payload);
                        }
                        message_instance->fnSetValue(sig_mnem, sig_value );
                        message_instance->Print();
                        // RVC_tzPGN_CHG_CFG_CMD2 tzCtrlCmd;
                        // tzCtrlCmd.ucShoreBrkSz = 20;
                        if(PGNREVERSE.find(pgn_mnem) != PGNREVERSE.end()){
                            // disable_sigalarm();
                            // cout << "disable" << endl;
                            int ulPgn = PGNREVERSE[pgn_mnem].first;
                            int grpfunc = PGNREVERSE[pgn_mnem].second;
                            message_instance->fnSend(ulPgn, grpfunc, device.second.addr);
                            // XBMSG_fnSend( ulPgn, &message_instance->msg, sizeof( message_instance->msg ), device.second.addr );
                        }
                        // XBMSG_fnSend( RVC_PGN_CHG_CFG_CMD2, &tzCtrlCmd, sizeof( tzCtrlCmd ), device.second.addr );
                        // cout << tzCtrlCmd.ucShoreBrkSz << endl;
                        // cout << tzCtrlCmd.ucDefBattTemp << endl;
                        

                    }
                    else{
                        cout << "noo pgn" << endl;
                    }

                }
            }
            else{
                cout << "No Pgn.Signal for " << param << endl;
            }
        }
    }
    cout << "No connection to " << devname << endl;
}

void fnHandleMqttSub(const std::string& devname, const std::string& payload) {
    std::cout << "Handling MQTT Subscribe: " << devname << ", " << payload << std::endl;
    json parsedPayload = json::parse(payload);
    // std::unique_lock<std::mutex> lock(devices_mutex);
    for (auto& device : devices) {
        if (device.second.name == devname) {

            std::cout << "subscribing for device: " << devname << std::endl;
            std::cout << "subscribing from: " << parsedPayload << std::endl;

            auto& active_subscriptions = device.second.active_subscriptions;
        
            for(const auto& param : parsedPayload){
                string prm = param.get<string>();
                if (prm != "Alerts" && prm != "OpState"){
                    // active_subscriptions.push_back(devname + "/" + prm);
                    active_subscriptions.push_back(prm);
                }
            }
            
            device.second.fnUpdateSubscriptions();
            return;
        }
    }
}

void fnHandleMqttUnsub(const std::string& devname, const std::string& payload) {
    std::cout << "Handling MQTT Unsubscribe: " << devname << ", " << payload << std::endl;
    json parsedPayload = json::parse(payload);
    // std::unique_lock<std::mutex> lock(devices_mutex);
    for (auto& device : devices) {
        if (device.second.name == devname) {

            std::cout << "Unsubscribing for device: " << devname << std::endl;
            std::cout << "Unsubscribing from: " << payload << std::endl;

            if (payload.empty()){
                device.second.active_subscriptions.clear();
            }

            auto& active_subscriptions = device.second.active_subscriptions;

            for(const auto& param : parsedPayload){
                string prm = param.get<string>();
                auto it = std::find(active_subscriptions.begin(), active_subscriptions.end(), prm);
                if (it != active_subscriptions.end()) {
                    active_subscriptions.erase(it);
                    std::cout << "Unsubscribed from " << prm << std::endl;
                } else {
                    std::cout << "Subscription not found: " << prm << std::endl;
                }
            }
            device.second.fnUpdateSubscriptions();
            return;
        }
    }
    std::cerr << "Device not found: " << devname << std::endl;
}

// Callback for message received event
// void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) {
//     // std::cout << "Received message: " << (char*)msg->payload << " on topic " << msg->topic << std::endl;
//     std::string topic(msg->topic);
//     std::string payload(static_cast<char*>(msg->payload), msg->payloadlen);

//     std::vector<std::string> parts = split(topic, '/');
//     if (parts.size() < 3) {
//         std::cerr << "Invalid topic structure: " << topic << std::endl;
//         return;
//     }

//     std::string cmd = parts[1];
//     std::string devname = parts[2];
//     // std::cout << parts[0] << std::endl << parts[1] << std::endl << parts[2] << std::endl;
//     // cout << endl;

//     if (cmd == "cmd") {
//         if (parts.size() == 4) {
//             std::string param = parts[3];
//             fnHandleMqttCmd(devname, param, payload);
//         }
//     } else if (cmd == "sub") {
//         fnHandleMqttSub(devname, payload);
//     } else if (cmd == "unsub") {
//         fnHandleMqttUnsub(devname, payload);
//     }

// }

void sigint_handler(int signal) {
    const char* message = "offline";
    //mosquitto_publish(mosq, nullptr, "xnet/sts/rvc/status", strlen(message), message, 0, true);

    printf("Disconnecting from MQTT broker...\n");
    // mosquitto_disconnect(mosq);
// 
    // mosquitto_loop_stop(mosq, true);

    printf("Destroying MQTT client...\n");
    // mosquitto_destroy(mosq);

    printf("Cleaning up MQTT library...\n");
    // mosquitto_lib_cleanup();
// 
    // Add a debug message before exiting
    printf("Exiting...\n");

    exit(1);
}

void sigterm_handler(int signal) {
    const char* message = "offline";
    //mosquitto_publish(mosq, nullptr, "xnet/sts/rvc/status", strlen(message), message, 0, true);

    printf("\nReceived SIGTERM\n");

    // mosquitto_disconnect(mosq);
    // mosquitto_loop_stop(mosq, true);

    // mosquitto_destroy(mosq);
    // mosquitto_lib_cleanup();
    exit(1);
}

// void on_publish(struct mosquitto *mosq, void *obj, int mid) {
//     std::cout << "Message published." << std::endl;
// }

void checkCommStats() {
    // Call the function and get the pointer to the struct
    XB_tzXB_COMM_STATS* stats = XBMSG_fnGetCommStats();

    // Access the members of the struct through the pointer
    uchar8 rxTimeout = stats->ucMPRxTimeout; //1
    uchar8 txTimeout = stats->ucMPTxTimeout;
    uchar8 lateCts = stats->ucMPLateCts;
    uchar8 lateEom = stats->ucMPLateEom;
    uchar8 lateAbort = stats->ucMPLateAbort;
    uchar8 lateXfer = stats->ucMPLateXfer; //88
    uchar8 maxActiveDobs = stats->ucMaxActiveDobs;
    uint16 mallocFail = stats->uiMallocFail;
    uint16 heapFreeMin = stats->uiHeapFreeMin;

    // Example: Print out the communication stats
    can << "Rx Timeout: " << (int)rxTimeout << endl;
    can << "Tx Timeout: " <<(int) txTimeout << endl;
    can << "Late CTS: " << (int)lateCts << endl;
    can << "Late EOM: " << (int)lateEom << endl;
    can << "Late Abort: " << (int)lateAbort << endl;
    can << "Late Xfer: " << (int)lateXfer << endl;
    can << "Max Active Dobs: " << (int)maxActiveDobs << endl; //1 //33
    can << "Malloc Fail: " << (int)mallocFail << endl;
    can << "Heap Free Min: " << (int)heapFreeMin << endl; // 8048 //176

}


int main( void )
{    
    struct sigaction sigint_action, sigterm_action;

    // Set up the SIGINT handler
    sigint_action.sa_handler = sigint_handler;
    sigemptyset(&sigint_action.sa_mask);
    sigint_action.sa_flags = 0;
    sigaction(SIGINT, &sigint_action, NULL);

    // Set up the SIGTERM handler
    sigterm_action.sa_handler = sigterm_handler;
    sigemptyset(&sigterm_action.sa_mask);
    sigterm_action.sa_flags = 0;
    sigaction(SIGTERM, &sigterm_action, NULL);
    // mosquitto_lib_init();

    signal(SIGSEGV, signalHandler);

    // if (!mosq) {
    //     std::cerr << "Failed to create client instance." << std::endl;
    //     return 1;
    // }

    // Set the callbacks
    // mosquitto_connect_callback_set(mosq, on_connect);
    // mosquitto_message_callback_set(mosq, on_message);
    // mosquitto_subscribe_callback_set(mosq, on_subscribe);
    // //mosquitto_publish_callback_set(mosq, on_publish);
    
    const char* will_topic = "xnet/sts/rvc/status";
    const char* will_message = "offline";
    // if (mosquitto_will_set(mosq, will_topic, strlen(will_message), will_message, 0, true) != MOSQ_ERR_SUCCESS){
    //     std::cerr << "Unable to will message." << std::endl;
    //     return 1;
    // }

    // if (mosquitto_connect(mosq, "localhost", 1883, 5) != MOSQ_ERR_SUCCESS) {
    //     std::cerr << "Unable to connect." << std::endl;
    //     return 1;
    // }
    // mosquitto_loop_start(mosq);

    SetName();

    if (init() == false){
        return 0;
    }
    // struct sigaction sa;
    // sa.sa_handler = tick;
    // sa.sa_flags = 0;
    // sigemptyset(&sa.sa_mask);

    // struct itimerval new_timer;
    // struct itimerval old_timer;
  
    // new_timer.it_value.tv_sec = 0;
    // new_timer.it_value.tv_usec = 10000;
    // new_timer.it_interval.tv_sec = 0;
    // new_timer.it_interval.tv_usec = 10000; 

    // setitimer(ITIMER_REAL, &new_timer, &old_timer);

    // if (sigaction(SIGALRM, &sa, NULL) < 0) {
    //     perror("sigaction failed");
    //     return 1;
    // }
    XBTIMERS_fnEnablePeriodReports(true);

    std::thread timerThread(tick);


    while (true){
        checkCommStats();
        // std::unique_lock<std::mutex> lock(devices_mutex);
        for (auto& pair : devices) {
            uchar8 key = pair.first;
            SubscriptionList& value = pair.second;
            // cout << (int)key << endl;
            send << "Key: " << (int)key << endl;
            // cout << "Value: " << (int)value << endl;
            int& current_state = value.state;
            send << "State: " << current_state << endl;
            send << "Total devices: " << devices.size() << endl;
            switch (current_state) {
                case STATE_IDLE:
                    value.fnSendRequest("ProdIdent");
                    // fnSendRequest("InstSts", key);
                    // cout << value.Make << endl;
                    // cout << value.Model << endl;
                    // cout << value.Serial << endl;

                    send << "Sent ProdIdent" << endl;
                    current_state = STATE_WAITING_RESPONSE;
                    break;
                case STATE_WAITING_RESPONSE:
                    // cout << "STATE_PROCESSING_RESPONSE1" << endl;
                    if(value.make.empty()){
                        // cout << "STATE_WAITING_RESPONSE" << endl;
                    //                         cout << value.Make << endl;
                    // cout << value.Model << endl;
                    // cout << value.Serial << endl;
                    // cout << current_state << endl;
                    if(value.msgtimes.find("ProdIdent") == value.msgtimes.end() ||
                        std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - value.msgtimes["ProdIdent"]).count() > REREQ_TIMEOUT){
                        value.fnSendRequest("ProdIdent");
                        send << "Sent ProdIdent again" << endl;
                    }
                    // fnSendRequest("InstSts", key);
                    break;
                    }
                    else {
                        if(value.name == "unk?"){
                            cout << "1038" << endl;
                            // std::string modelToCheck = value.model;
                            
                            // if (std::find(ZRPM_BMS_MODELS.begin(), ZRPM_BMS_MODELS.end(), modelToCheck) != ZRPM_BMS_MODELS.end()) {
                            //     value.type = "ZRPM";
                            // } 
                            // else if (std::find(SRNE_MPPT_MODELS.begin(), SRNE_MPPT_MODELS.end(), modelToCheck) != SRNE_MPPT_MODELS.end()) {
                            //     value.type = "SRNE";
                            // }
                            // else{
                            //     fnSendRequest("InstSts", key);
                            // }
                            if(value.msgtimes.find("InstSts") == value.msgtimes.end() ||
                                std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - value.msgtimes["InstSts"]).count() > REREQ_TIMEOUT){
                                value.fnSendRequest("InstSts");
                                send << "Sent InstSts" << endl;
                            }
                            break;
                        }
                        else{
                            current_state = STATE_PROCESSING_RESPONSE;
                            send << "Update fixed subs" << endl;
                            value.fnUpdateFixedSubscriptions();
                        }
                    }
                    // if (value.Name.empty()){
                    //     fnSendRequest("InstSts", key);
                    //     break;
                    // }
                    // else{
                    //     cout << "STATE_PROCESSING_RESPONSE1" << endl;
                    // //                         cout << value.Make << endl;
                    // // cout << value.Model << endl;
                    // // cout << value.Serial << endl;
                    // // cout << current_state << endl;
                    // value.fnUpdateFixedSubscriptions();
                    // current_state = STATE_PROCESSING_RESPONSE;
                    // }
                case STATE_PROCESSING_RESPONSE:
                    // break;


                    // if (value.func == "invchg"){
                    //     if(value.model.compare(0, 7, "FEX_RVC") == 0){
                    //         fnSendRequest("PmInvSts", key);
                    //     }
                    // }
                    send << "poll subs" << endl;
                    value.fnPollSubscriptions();  
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }          
        }
        // lock.unlock();
        //         sigset_t mask, oldmask;
        // struct timespec ts;
        // ts.tv_sec = 1;  // 1 second
        // ts.tv_nsec = 0;
        // nanosleep(&ts, nullptr);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

    }

    printf("%d", PGN_ADDR_GLOBAL);

    cout << "kk";

    // mosquitto_disconnect(mosq);
    // mosquitto_loop_stop(mosq, true);

    // mosquitto_destroy(mosq);
    // mosquitto_lib_cleanup();
}




























































































































































































































































































































































































































































































































// New node found at address 67
// 770
// InvAcSts4
// 770
// InvAcSts3
// 770
// InvAcSts3
// 163
// 770
// InvAcSts1
// 770
// InvAcSts1
// 770
// InvSts
// 770
// DiagMsg1
// 770
// InvTempSts
// 579
// 581
// 749
// 1038
// 163
// 597
// 599
// 552
// 3
// -1
// 0
// 356
// 3
// 0
// invchg
// 67
// invchg3
// unk?
// 770
// DiagMsg1
// 770
// DCSrcSts2
// 770
// DCSrcSts1
// 770
// InvTempSts
// 770
// InvAcSts4
// 770
// InvAcSts3
// 770
// InvAcSts3
// 770
// InvAcSts1
// 770
// InvAcSts1
// 770
// InvSts
// 834
// InvSts
// xnet/sts/invchg3/InvOpState
// Invert
// 770
// DiagMsg1
// 770
// InvTempSts
// 770
// DiagMsg1
// 770
// InvTempSts
// 770
// InvAcSts4
// 770
// InvAcSts3
// 770
// InvAcSts3
// 770
// InvAcSts1
// 770
// InvAcSts1
// 770
// InvSts
// 834
// InvSts
// xnet/sts/invchg3/InvOpState
// Invert
// 770
// DiagMsg1
// 770
// InvTempSts
// 770
// DiagMsg1
// 770
// DCSrcSts2
// 770
// DCSrcSts1
// 770
// InvTempSts
// 770
// InvAcSts4
// 770
// InvAcSts3
// 770
// InvAcSts3
// 770
// InvAcSts1
// 770
// InvAcSts1
// 770
// InvSts
// 834
// InvSts
// xnet/sts/invchg3/InvOpState
// Invert
// 770
// DiagMsg1
// 770
// InvTempSts
// 770
// InvDcSts
// 770
// DiagMsg1
// 770
// InvTempSts
// 770
// ChgAcSts4
// 770
// ChgAcSts4
// 770
// ChgAcSts3
// 770
// ChgAcSts3
// 770
// ChgAcSts1
// 770
// ChgAcSts1
// 770
// InvAcSts4
// 770
// InvAcSts3
// 770
// InvAcSts3
// 770
// InvAcSts1
// 770
// InvAcSts1
// 770
// ChgSts
// 834
// ChgSts
// xnet/sts/invchg3/ChgOpState
// Not Charging
// 770
// InvSts
// 834
// InvSts
// xnet/sts/invchg3/InvOpState
// Invert
// 770
// DiagMsg1
// 770
// InvTempSts
// 770770
// DCSrcSts4
// DiagMsg1
// 770
// DCSrcSts2
// 770
// DCSrcSts1
// 770
// InvTempSts

// 770
// InvAcSts4
// 770
// InvAcSts3
// 770
// InvAcSts3
// 770
// InvAcSts1
// 770
// InvAcSts1
// 770
// InvSts
// 834
// InvSts
// xnet/sts/invchg3/InvOpState
// Invert
// 770
// DiagMsg1
// 770
// InvTempSts
// 770
// DiagMsg1
// 770
// InvTempSts
// 770
// InvAcSts4
// 770
// InvAcSts3
// 770
// InvAcSts3
// 770
// InvAcSts1
// 770
// InvAcSts1
// 770
// InvSts
// 834
// InvSts
// xnet/sts/invchg3/InvOpState
// Invert
// 770
// DiagMsg1
// 770
// InvTempSts
// 770
// DiagMsg1
// 770
// DCSrcSts2
// 163
// 163
// 153
// 6
// 67
// 163
// 770
// DCSrcSts1
// 770
// InvTempSts
// 770
// ChgSts
// 834
// ChgSts
// xnet/sts/invchg3/ChgOpState
// Not Charging
// 770
// InvSts
// 834
// InvSts
// xnet/sts/invchg3/InvOpState
// Invert
// 770
// DiagMsg1
// 770
// InvAcSts4
// 770
// InvAcSts3
// 770
// InvAcSts3
// 770
// InvAcSts1
// 770
// InvAcSts1
// 770
// InvSts
// 834
// InvSts
// xnet/sts/invchg3/InvOpState
// Invert
// 770
// DiagMsg1
// 770
// InvTempSts
// 770
// DiagMsg1
// 770
// InvTempSts
// 770
// InvAcSts4
// 770
// InvAcSts3
// 770
// InvAcSts3
// 770
// InvAcSts1
// 770
// InvAcSts1
// 770
// InvSts
// 834
// InvSts
// xnet/sts/invchg3/InvOpState
// Invert
// 770
// DiagMsg1
// 770
// InvTempSts
// 770
// InvDcSts
// 770
// DiagMsg1
// 770
// DCSrcSts2
// 770
// DCSrcSts1
// 770
// InvTempSts
// 770
// ChgAcSts4
// 770
// ChgAcSts4
// 770
// ChgAcSts3
// 770
// ChgAcSts3
// 770
// ChgAcSts1
// 770
// ChgAcSts1
// 770
// InvAcSts4
// 770
// InvAcSts3
// 770
// InvAcSts3
// 770
// InvAcSts1
// 770
// InvAcSts1
// 770
// ChgSts
// 834
// ChgSts
// xnet/sts/invchg3/ChgOpState
// Not Charging
// 770
// InvSts
// 834
// InvSts
// xnet/sts/invchg3/InvOpState
// Invert
// 770
// DiagMsg1
// 770
// InvTempSts
// ^CDisconnecting from MQTT broker...
// Destroying MQTT client...
// Cleaning up MQTT library...
// Exiting...