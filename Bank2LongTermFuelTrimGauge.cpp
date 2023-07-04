//
// Created by sigsegv on 5/17/23.
//

#include "Bank2LongTermFuelTrimGauge.h"
#include "CarDatasource.h"

Bank2LongTermFuelTrimGauge::Bank2LongTermFuelTrimGauge(const std::shared_ptr<CarDatasource> &serialCarDevice, const std::shared_ptr<WarningsData> &warningsData) : LongTermFuelTrimGauge(serialCarDevice), warningsData(warningsData), warningsList(warningsData->Create()) {
    caption = "LFT2";
}

Bank2LongTermFuelTrimGauge::~Bank2LongTermFuelTrimGauge() {
    warningsData->Remove(warningsList);
}

void Bank2LongTermFuelTrimGauge::Update() {
    auto optTrim = serialCarDevice->GetLongTermFuelTrimBank2();
    std::vector<std::string> messages{};
    if (optTrim) {
        auto trim = *optTrim;
        SetCurrentValue((float) trim);
        if (trim < -25) {
            messages.emplace_back("FUEL RICH 2");
        }
        if (trim > 25) {
            messages.emplace_back("FUEL LEAN 2");
        }
    } else {
        SetInvalid();
    }
    std::lock_guard lock{warningsList->mtx};
    warningsList->messages = messages;
}