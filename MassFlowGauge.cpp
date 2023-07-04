//
// Created by sigsegv on 5/17/23.
//

#include "MassFlowGauge.h"
#include "CarDatasource.h"

MassFlowGauge::MassFlowGauge(const std::shared_ptr<CarDatasource> &serialCarDevice)  : serialCarDevice(serialCarDevice) {
    min = 0;
    max = 100;
    caption = "Air flow (g/s)";
}

void MassFlowGauge::Update() {
    auto mass = serialCarDevice->GetMassAirFlow();
    if (mass) {
        SetCurrentValue((float) *mass);
    } else {
        SetInvalid();
    }
}

PriorityCategory MassFlowGauge::GetPriorityCategory() const {
    return PriorityCategory::HIGH;
}