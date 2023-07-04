//
// Created by sigsegv on 5/17/23.
//

#include "Bank1ShortTermFuelTrimGauge.h"
#include "CarDatasource.h"

Bank1ShortTermFuelTrimGauge::Bank1ShortTermFuelTrimGauge(const std::shared_ptr<CarDatasource> &serialCarDevice) : ShortTermFuelTrimGauge(serialCarDevice) {
    caption = "SFT1";
}

void Bank1ShortTermFuelTrimGauge::Update() {
    auto trim = serialCarDevice->GetShortTermFuelTrimBank1();
    if (trim) {
        SetCurrentValue((float) *trim);
    } else {
        SetInvalid();
    }
}
