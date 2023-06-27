//
// Created by sigsegv on 6/27/23.
//

#include "DtcCodeMap.h"

std::string DtcCodeMap::GetMsg(const std::string &code) {
    if (code == "P171") {
        return "FUEL LEAN (B1)";
    }
    if (code == "P174") {
        return "FUEL LEAN (B2)";
    }
    if (code == "P332") {
        return "KNOCK SENS CIRC (B2)";
    }
    return "";
}