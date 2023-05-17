//
// Created by sigsegv on 5/17/23.
//

#include "Bank1LongTermFuelTrimGauge.h"
#include "SerialCarDevice.h"

void Bank1LongTermFuelTrimGauge::Update() {
    auto trim = serialCarDevice->GetLongTermFuelTrimBank1();
    SetCurrentValue(trim);
}