//
// Created by sigsegv on 5/18/23.
//

#include "EngCoolantTempGauge.h"
#include "CarDatasource.h"

EngCoolantTempGauge::EngCoolantTempGauge(const std::shared_ptr<CarDatasource> &serialCarDevice) : serialCarDevice(serialCarDevice) {
    min = -40;
    max = 120;
    mark = 20;
    showValueMark = 40;
    markOffset = -40;
    caption = "Coolant (C)";
}

void EngCoolantTempGauge::Update() {
    auto coolant = serialCarDevice->GetCoolantTemperature();
    SetCurrentValue((float) coolant);
}

PriorityCategory EngCoolantTempGauge::GetPriorityCategory() const {
    return PriorityCategory::VERY_LOW;
}
