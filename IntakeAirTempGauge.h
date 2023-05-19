//
// Created by sigsegv on 5/17/23.
//

#ifndef ELMOBD_INTAKEAIRTEMPGAUGE_H
#define ELMOBD_INTAKEAIRTEMPGAUGE_H

#include "AnalogGauge.h"
#include "Meter.h"

class CarDatasource;
class WarningsData;
class WarningsList;

class IntakeAirTempGauge : public AnalogGauge, public Meter {
private:
    std::shared_ptr<CarDatasource> serialCarDevice;
    std::shared_ptr<WarningsData> warningsData;
    std::shared_ptr<WarningsList> warningsList;
public:
    IntakeAirTempGauge(const std::shared_ptr<CarDatasource> &serialCarDevice, const std::shared_ptr<WarningsData> &warningsData);
    ~IntakeAirTempGauge();
    void Update() override;
    PriorityCategory GetPriorityCategory() const override;
};

#endif //ELMOBD_INTAKEAIRTEMPGAUGE_H
