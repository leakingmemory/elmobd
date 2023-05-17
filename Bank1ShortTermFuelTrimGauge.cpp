//
// Created by sigsegv on 5/17/23.
//

#include "Bank1ShortTermFuelTrimGauge.h"
#include "SerialCarDevice.h"

void Bank1ShortTermFuelTrimGauge::Update() {
    auto trim = serialCarDevice->GetShortTermFuelTrimBank1();
    SetCurrentValue(trim);
}