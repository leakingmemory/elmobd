//
// Created by sigsegv on 5/17/23.
//

#include "Bank1LongTermFuelTrimGauge.h"
#include "CarDatasource.h"

Bank1LongTermFuelTrimGauge::Bank1LongTermFuelTrimGauge(const std::shared_ptr<CarDatasource> &serialCarDevice, const std::shared_ptr<WarningsData> &warningsData) : LongTermFuelTrimGauge(serialCarDevice), warningsData(warningsData), warningsList(warningsData->Create()) {
    caption = "LFT1";
}

Bank1LongTermFuelTrimGauge::~Bank1LongTermFuelTrimGauge() {
    warningsData->Remove(warningsList);
}

void Bank1LongTermFuelTrimGauge::Update() {
    auto optTrim = serialCarDevice->GetLongTermFuelTrimBank1();
    std::vector<std::string> messages{};
    if (optTrim) {
        auto trim = *optTrim;
        SetCurrentValue((float) trim);
        if (trim < -25) {
            messages.emplace_back("FUEL RICH 1");
        }
        if (trim > 25) {
            messages.emplace_back("FUEL LEAN 1");
        }
    } else {
        SetInvalid();
    }
    std::lock_guard lock{warningsList->mtx};
    warningsList->messages = messages;
}