//
// Created by sigsegv on 5/17/23.
//

#ifndef ELMOBD_MANPRESSGAUGE_H
#define ELMOBD_MANPRESSGAUGE_H

#include "AnalogGauge.h"
#include "Meter.h"

class SerialCarDevice;

class ManPressGauge : public AnalogGauge, public Meter {
private:
    std::shared_ptr<SerialCarDevice> serialCarDevice;
public:
    ManPressGauge(const std::shared_ptr<SerialCarDevice> &serialCarDevice);
    void Update() override;
    PriorityCategory GetPriorityCategory() const override;
};


#endif //ELMOBD_MANPRESSGAUGE_H
