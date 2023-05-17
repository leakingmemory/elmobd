//
// Created by sigsegv on 5/17/23.
//

#include "Bank2ShortTermFuelTrimGauge.h"
#include "CarDatasource.h"

Bank2ShortTermFuelTrimGauge::Bank2ShortTermFuelTrimGauge(const std::shared_ptr<CarDatasource> &serialCarDevice) : ShortTermFuelTrimGauge(serialCarDevice) {
    caption = "SFT2";
}

void Bank2ShortTermFuelTrimGauge::Update() {
    auto trim = serialCarDevice->GetShortTermFuelTrimBank2();
    SetCurrentValue(trim);
}
