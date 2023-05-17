//
// Created by sigsegv on 5/17/23.
//

#include "Bank2LongTermFuelTrimGauge.h"
#include "CarDatasource.h"

Bank2LongTermFuelTrimGauge::Bank2LongTermFuelTrimGauge(const std::shared_ptr<CarDatasource> &serialCarDevice) : LongTermFuelTrimGauge(serialCarDevice) {
    caption = "LFT2";
}

void Bank2LongTermFuelTrimGauge::Update() {
    auto trim = serialCarDevice->GetLongTermFuelTrimBank2();
    SetCurrentValue(trim);
}