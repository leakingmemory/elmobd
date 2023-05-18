//
// Created by sigsegv on 5/18/23.
//

#ifndef ELMOBD_ENGCOOLANTTEMPGAUGE_H
#define ELMOBD_ENGCOOLANTTEMPGAUGE_H

#include "AnalogGauge.h"
#include "Meter.h"

class CarDatasource;

class EngCoolantTempGauge : public AnalogGauge, public Meter {
private:
    std::shared_ptr<CarDatasource> serialCarDevice;
public:
    EngCoolantTempGauge(const std::shared_ptr<CarDatasource> &serialCarDevice);
    void Update() override;
    PriorityCategory GetPriorityCategory() const override;
};


#endif //ELMOBD_ENGCOOLANTTEMPGAUGE_H
