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
    auto newDtcs = carDatasource->GetDTCs();
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
                std::cout << "Cleared stored " << *iterator << "\n";
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
            std::cout << "New stored " << ndtc << "\n";
            dtcs.emplace_back(ndtc);
        }
    }
    std::vector<std::string> messages{};
    for (const auto &dtc : dtcs) {
        std::stringstream sstr{};
        sstr << "S ";
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
