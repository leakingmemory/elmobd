//
// Created by sigsegv on 5/17/23.
//

#ifndef ELMOBD_BANK1SHORTTERMFUELTRIMGAUGE_H
#define ELMOBD_BANK1SHORTTERMFUELTRIMGAUGE_H

#include "ShortTermFuelTrimGauge.h"

class Bank1ShortTermFuelTrimGauge : public ShortTermFuelTrimGauge {
public:
    Bank1ShortTermFuelTrimGauge(const std::shared_ptr<CarDatasource> &serialCarDevice);
    void Update() override;
};


#endif //ELMOBD_BANK1SHORTTERMFUELTRIMGAUGE_H
