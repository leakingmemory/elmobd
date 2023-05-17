//
// Created by sigsegv on 5/17/23.
//

#include "FuelTrimGauge.h"

FuelTrimGauge::FuelTrimGauge(const std::shared_ptr<SerialCarDevice> &serialCarDevice) : serialCarDevice(serialCarDevice) {
    min = -100;
    max = 100;
    markOffset = -100;
    mark = 25;
}