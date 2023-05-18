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
    auto trim = serialCarDevice->GetLongTermFuelTrimBank1();
    SetCurrentValue(trim);
    std::vector<std::string> messages{};
    if (trim < -25) {
        messages.emplace_back("FUEL RICH 1");
    }
    if (trim > 25) {
        messages.emplace_back("FUEL LEAN 1");
    }
    std::lock_guard lock{warningsList->mtx};
    warningsList->messages = messages;
}