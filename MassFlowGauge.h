//
// Created by sigsegv on 5/17/23.
//

#ifndef ELMOBD_MASSFLOWGAUGE_H
#define ELMOBD_MASSFLOWGAUGE_H

#include "AnalogGauge.h"
#include "Meter.h"

class CarDatasource;

class MassFlowGauge : public AnalogGauge, public Meter {
private:
    std::shared_ptr<CarDatasource> serialCarDevice;
public:
    MassFlowGauge(const std::shared_ptr<CarDatasource> &serialCarDevice);
    void Update() override;
    PriorityCategory GetPriorityCategory() const override;
};


#endif //ELMOBD_MASSFLOWGAUGE_H
