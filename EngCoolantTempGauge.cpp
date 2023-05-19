//
// Created by sigsegv on 5/18/23.
//

#include "EngCoolantTempGauge.h"
#include "CarDatasource.h"
#include "WarningsData.h"

EngCoolantTempGauge::EngCoolantTempGauge(const std::shared_ptr<CarDatasource> &serialCarDevice, const std::shared_ptr<WarningsData> &warningsData) : serialCarDevice(serialCarDevice), warningsData(warningsData), warningsList(warningsData->Create()) {
    min = -40;
    max = 120;
    mark = 20;
    showValueMark = 40;
    markOffset = -40;
    caption = "Coolant (C)";
}

EngCoolantTempGauge::~EngCoolantTempGauge() {
    warningsData->Remove(warningsList);
}

void EngCoolantTempGauge::Update() {
    auto coolant = serialCarDevice->GetCoolantTemperature();
    SetCurrentValue((float) coolant);
    std::vector<std::string> messages;
    if (coolant > 99) {
        messages.emplace_back("ENG OVERHEAT (COOL NOW)");
    } else if (coolant > 93) {
        messages.emplace_back("ENG OVERHEAT");
    } else if (coolant < 5) {
        messages.emplace_back("ENG VERY LOW TEMP");
    } else if (coolant < 40) {
        messages.emplace_back("ENG LOW TEMP");
    }
    std::lock_guard lock{warningsList->mtx};
    warningsList->messages = messages;
}

PriorityCategory EngCoolantTempGauge::GetPriorityCategory() const {
    return PriorityCategory::VERY_LOW;
}
