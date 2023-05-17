//
// Created by sigsegv on 5/17/23.
//

#include "ManPressGauge.h"
#include "CarDatasource.h"

ManPressGauge::ManPressGauge(const std::shared_ptr<CarDatasource> &serialCarDevice)  : serialCarDevice(serialCarDevice) {
    min = 20.0f;
    max = 110.0f;
    caption = "Man press (kPa)";
}

void ManPressGauge::Update() {
    float kpa = (float) serialCarDevice->GetIntakeManifoldAbsPressure();
    SetCurrentValue(kpa);
}

PriorityCategory ManPressGauge::GetPriorityCategory() const {
    return PriorityCategory::HIGH;
}
