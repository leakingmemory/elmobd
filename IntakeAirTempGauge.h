//
// Created by sigsegv on 5/17/23.
//

#ifndef ELMOBD_INTAKEAIRTEMPGAUGE_H
#define ELMOBD_INTAKEAIRTEMPGAUGE_H

#include "AnalogGauge.h"
#include "Meter.h"

class SerialCarDevice;

class IntakeAirTempGauge : public AnalogGauge, public Meter {
private:
    std::shared_ptr<SerialCarDevice> serialCarDevice;
public:
    IntakeAirTempGauge(const std::shared_ptr<SerialCarDevice> &serialCarDevice);
    void Update() override;
    PriorityCategory GetPriorityCategory() const override;
};

#endif //ELMOBD_INTAKEAIRTEMPGAUGE_H
