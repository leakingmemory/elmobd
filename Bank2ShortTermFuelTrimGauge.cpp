//
// Created by sigsegv on 5/17/23.
//

#include "Bank2ShortTermFuelTrimGauge.h"
#include "CarDatasource.h"

void Bank2ShortTermFuelTrimGauge::Update() {
    auto trim = serialCarDevice->GetShortTermFuelTrimBank2();
    SetCurrentValue(trim);
}
