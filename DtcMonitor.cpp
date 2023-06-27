//
// Created by sigsegv on 6/27/23.
//

#include "DtcMonitor.h"
#include "CarDatasource.h"
#include "DtcCodeMap.h"
#include <iostream>
#include <sstream>

DtcMonitor::DtcMonitor(const std::shared_ptr<CarDatasource> &serialCarDevice,
                       const std::shared_ptr<WarningsData> &warningsData) : carDatasource(serialCarDevice), warningsData(warningsData), warningsList(warningsData->Create()) {
}

void DtcMonitor::Update() {
    auto newDtcs = GetCodes();
    {
        auto iterator = dtcs.begin();
        while (iterator != dtcs.end()) {
            bool found{false};
            for (const auto &dtc : newDtcs) {
                if (dtc == *iterator) {
                    found = true;
                    break;
                }
            }
            if (found) {
                ++iterator;
            } else {
                std::cout << "Cleared stored " << GetPrefix() << *iterator << "\n";
                dtcs.erase(iterator);
            }
        }
    }
    for (const auto &ndtc : newDtcs) {
        bool found{false};
        for (const auto &dtc : dtcs) {
            if (ndtc == dtc) {
                found = true;
                break;
            }
        }
        if (!found) {
            std::cout << "New stored " << GetPrefix() << ndtc << "\n";
            dtcs.emplace_back(ndtc);
        }
    }
    std::vector<std::string> messages{};
    for (const auto &dtc : dtcs) {
        std::stringstream sstr{};
        sstr << GetPrefix();
        sstr << dtc;
        auto msg = DtcCodeMap::GetMsg(dtc);
        if (!msg.empty()) {
            sstr << " " << msg;
        }
        messages.emplace_back(sstr.str());
    }
    std::lock_guard lock{warningsList->mtx};
    warningsList->messages = messages;
}

PriorityCategory DtcMonitor::GetPriorityCategory() const {
    return PriorityCategory::VERY_LOW;
}

std::string DtcStoredMonitor::GetPrefix() const {
    return "S ";
}

std::vector<std::string> DtcStoredMonitor::GetCodes() const {
    return carDatasource->GetDTCs();
}

std::string DtcPendingMonitor::GetPrefix() const {
    return "P ";
}

std::vector<std::string> DtcPendingMonitor::GetCodes() const {
    return carDatasource->GetPendingDTCs();
}
