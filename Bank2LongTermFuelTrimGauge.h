//
// Created by sigsegv on 5/17/23.
//

#ifndef ELMOBD_BANK2LONGTERMFUELTRIMGAUGE_H
#define ELMOBD_BANK2LONGTERMFUELTRIMGAUGE_H

#include "LongTermFuelTrimGauge.h"

class Bank2LongTermFuelTrimGauge : public LongTermFuelTrimGauge {
public:
    Bank2LongTermFuelTrimGauge(const std::shared_ptr<CarDatasource> &serialCarDevice);
    void Update() override;
};


#endif //ELMOBD_BANK2LONGTERMFUELTRIMGAUGE_H
