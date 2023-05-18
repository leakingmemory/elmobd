//
// Created by sigsegv on 5/18/23.
//

#include "O2VoltageGauge.h"

O2VoltageGauge::O2VoltageGauge() {
    min = 0.0f;
    max = 2.0f;
    mark = 1;
    showValueMark = 1;
    caption = "O2 (V)";
}
