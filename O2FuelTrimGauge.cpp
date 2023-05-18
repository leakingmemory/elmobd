//
// Created by sigsegv on 5/18/23.
//

#include "O2FuelTrimGauge.h"

O2FuelTrimGauge::O2FuelTrimGauge() {
    min = -100;
    max = 100;
    markOffset = -100;
    mark = 25;
    caption = "O2 FT";
}
