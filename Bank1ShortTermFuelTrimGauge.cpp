//
// Created by sigsegv on 5/17/23.
//

#include "Bank1ShortTermFuelTrimGauge.h"
#include "CarDatasource.h"

void Bank1ShortTermFuelTrimGauge::Update() {
    auto trim = serialCarDevice->GetShortTermFuelTrimBank1();
    SetCurrentValue(trim);
}
