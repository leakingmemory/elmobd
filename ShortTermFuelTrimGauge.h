//
// Created by sigsegv on 5/17/23.
//

#ifndef ELMOBD_SHORTTERMFUELTRIMGAUGE_H
#define ELMOBD_SHORTTERMFUELTRIMGAUGE_H

#include "FuelTrimGauge.h"

class ShortTermFuelTrimGauge : public FuelTrimGauge {
public:
    ShortTermFuelTrimGauge(const std::shared_ptr<SerialCarDevice> &serialCarDevice) :
            FuelTrimGauge(serialCarDevice) {}
    void Update() override = 0;
    PriorityCategory GetPriorityCategory() const override;
};


#endif //ELMOBD_SHORTTERMFUELTRIMGAUGE_H
