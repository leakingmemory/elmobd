//
// Created by sigsegv on 5/17/23.
//

#include "MassFlowGauge.h"
#include "SerialCarDevice.h"

MassFlowGauge::MassFlowGauge(const std::shared_ptr<SerialCarDevice> &serialCarDevice)  : serialCarDevice(serialCarDevice) {
    min = 0;
    max = 100;
}

void MassFlowGauge::Update() {
    float mass = (float) serialCarDevice->GetMassAirFlow();
    SetCurrentValue(mass);
}

PriorityCategory MassFlowGauge::GetPriorityCategory() const {
    return PriorityCategory::HIGH;
}