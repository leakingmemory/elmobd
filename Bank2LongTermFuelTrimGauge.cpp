//
// Created by sigsegv on 5/17/23.
//

#include "Bank2LongTermFuelTrimGauge.h"
#include "CarDatasource.h"

void Bank2LongTermFuelTrimGauge::Update() {
    auto trim = serialCarDevice->GetLongTermFuelTrimBank2();
    SetCurrentValue(trim);
}