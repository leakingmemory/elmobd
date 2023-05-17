//
// Created by sigsegv on 5/17/23.
//

#include "Bank1LongTermFuelTrimGauge.h"
#include "CarDatasource.h"

Bank1LongTermFuelTrimGauge::Bank1LongTermFuelTrimGauge(const std::shared_ptr<CarDatasource> &serialCarDevice) : LongTermFuelTrimGauge(serialCarDevice) {
    caption = "LFT1";
}

void Bank1LongTermFuelTrimGauge::Update() {
    auto trim = serialCarDevice->GetLongTermFuelTrimBank1();
    SetCurrentValue(trim);
}