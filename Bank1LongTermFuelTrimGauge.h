//
// Created by sigsegv on 5/17/23.
//

#ifndef ELMOBD_BANK1LONGTERMFUELTRIMGAUGE_H
#define ELMOBD_BANK1LONGTERMFUELTRIMGAUGE_H

#include "LongTermFuelTrimGauge.h"

class Bank1LongTermFuelTrimGauge : public LongTermFuelTrimGauge {
public:
    Bank1LongTermFuelTrimGauge(const std::shared_ptr<CarDatasource> &serialCarDevice) : LongTermFuelTrimGauge(serialCarDevice) {}
    void Update() override;
};


#endif //ELMOBD_BANK1LONGTERMFUELTRIMGAUGE_H
