//
// Created by sigsegv on 5/17/23.
//

#ifndef ELMOBD_BANK2SHORTTERMFUELTRIMGAUGE_H
#define ELMOBD_BANK2SHORTTERMFUELTRIMGAUGE_H

#include "ShortTermFuelTrimGauge.h"

class Bank2ShortTermFuelTrimGauge : public ShortTermFuelTrimGauge {
public:
    Bank2ShortTermFuelTrimGauge(const std::shared_ptr<SerialCarDevice> &serialCarDevice) : ShortTermFuelTrimGauge(serialCarDevice) {}
    void Update() override;
};


#endif //ELMOBD_BANK2SHORTTERMFUELTRIMGAUGE_H
