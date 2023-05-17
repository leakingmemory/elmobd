//
// Created by sigsegv on 5/17/23.
//

#ifndef ELMOBD_FUELTRIMGAUGE_H
#define ELMOBD_FUELTRIMGAUGE_H

#include "AnalogGauge.h"
#include "Meter.h"

class SerialCarDevice;

class FuelTrimGauge : public AnalogGauge, public Meter {
protected:
    std::shared_ptr<SerialCarDevice> serialCarDevice;
public:
    FuelTrimGauge(const std::shared_ptr<SerialCarDevice> &serialCarDevice);
    void Update() override = 0;
    PriorityCategory GetPriorityCategory() const override = 0;
};


#endif //ELMOBD_FUELTRIMGAUGE_H
