
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>

struct Signal {
    int ddid;
    std::string mnem;
    double offset;
    double scale;
    std::string typename_;
    std::string units;
};

struct PGNEntry {
    std::vector<Signal> extrasignals;
    std::string extrastruct;
    std::vector<Signal> fixedsignals;
    std::string fixedstruct;
    int id;
    int maxfield;
    std::string mnem;
    int mode;
    int repeats;
    int timing;
};

int main() {
    std::map<std::pair<int, int>, PGNEntry> PGNLOOKUP;


    PGNLOOKUP[{59392, 0}] = {
        {},
        "",
        {{8991, "CtrlByte", 0.0, 1.0, "enum", "None"}, {8996, "GroupFunctionValue", 0.0, 1.0, "enum", "None"}, {9034, "ParmGrpNum", 0.0, 1.0, "bit(n)", "None"}},
        "iiI",
        2677,
        4,
        "IsoAck",
        0,
        0,
        0
    };


    PGNLOOKUP[{59648, 1}] = {
        {},
        "",
        {{-725723354, "DesiredMode", 0.0, 1.0, "enum", "None"}, {9258, "Reserved", 0.0, 1.0, "uint8", "None"}, {9200, "Reserved", 0.0, 1.0, "uint16", "None"}},
        "iBH",
        1988024735,
        2,
        "PmParamDevMdCfg",
        1,
        0,
        0
    };


    PGNLOOKUP[{59648, 2}] = {
        {},
        "",
        {{1546649241, "AssocType", 0.0, 1.0, "enum", "None"}, {9036, "AssocInst", 0.0, 1.0, "bit(n)", "None"}, {-1133693210, "AssocId", 0.0, 1.0, "uint8", "None"}, {9258, "Reserved", 0.0, 1.0, "uint8", "None"}},
        "iBBBx",
        1485904628,
        3,
        "PmParamAssocCfg",
        2,
        0,
        0
    };


    // Example: Accessing and printing some data
    for (const auto& entry : PGNLOOKUP) {
        const auto& key = entry.first;
        const auto& value = entry.second;

        std::cout << "PGN: " << key.first << ", Mode: " << key.second << "\n";
        std::cout << "Mnem: " << value.mnem << ", ID: " << value.id << "\n";
        for (const auto& signal : value.fixedsignals) {
            std::cout << "Signal mnem: " << signal.mnem << ", ddid: " << signal.ddid << "\n";
        }
        std::cout << "\n";
    }

    return 0;
}
