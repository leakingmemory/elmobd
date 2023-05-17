//
// Created by sigsegv on 5/17/23.
//

#include "IntakeAirTempGauge.h"
#include "CarDatasource.h"

IntakeAirTempGauge::IntakeAirTempGauge(const std::shared_ptr<CarDatasource> &serialCarDevice) : serialCarDevice(serialCarDevice) {
    min = -40;
    max = 40;
    mark = 10;
    markOffset = -40;
    caption = "IAT (Celsius)";
}

void IntakeAirTempGauge::Update() {
    auto temp = serialCarDevice->GetIntakeAirTemperature();
    SetCurrentValue(temp);
}

PriorityCategory IntakeAirTempGauge::GetPriorityCategory() const {
    return PriorityCategory::LOW;
}
