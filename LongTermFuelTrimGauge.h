//
// Created by sigsegv on 5/17/23.
//

#ifndef ELMOBD_LONGTERMFUELTRIMGAUGE_H
#define ELMOBD_LONGTERMFUELTRIMGAUGE_H

#include "FuelTrimGauge.h"

class LongTermFuelTrimGauge : public FuelTrimGauge {
public:
    LongTermFuelTrimGauge(const std::shared_ptr<SerialCarDevice> &serialCarDevice) : FuelTrimGauge(serialCarDevice) {}
    void Update() override = 0;
    PriorityCategory GetPriorityCategory() const override;
};


#endif //ELMOBD_LONGTERMFUELTRIMGAUGE_H
