//
// Created by sigsegv on 5/17/23.
//

#include "IntakeAirTempGauge.h"
#include "SerialCarDevice.h"

IntakeAirTempGauge::IntakeAirTempGauge(const std::shared_ptr<SerialCarDevice> &serialCarDevice) : serialCarDevice(serialCarDevice) {
    min = -40;
    max = 40;
    mark = 10;
    markOffset = -40;
}

void IntakeAirTempGauge::Update() {
    auto temp = serialCarDevice->GetIntakeAirTemperature();
    SetCurrentValue(temp);
}

PriorityCategory IntakeAirTempGauge::GetPriorityCategory() const {
    return PriorityCategory::LOW;
}