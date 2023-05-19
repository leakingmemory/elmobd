//
// Created by sigsegv on 5/18/23.
//

#ifndef ELMOBD_ENGCOOLANTTEMPGAUGE_H
#define ELMOBD_ENGCOOLANTTEMPGAUGE_H

#include "AnalogGauge.h"
#include "Meter.h"
#include <memory>

class CarDatasource;
class WarningsData;
class WarningsList;

class EngCoolantTempGauge : public AnalogGauge, public Meter {
private:
    std::shared_ptr<CarDatasource> serialCarDevice;
    std::shared_ptr<WarningsData> warningsData;
    std::shared_ptr<WarningsList> warningsList;
public:
    EngCoolantTempGauge(const std::shared_ptr<CarDatasource> &serialCarDevice, const std::shared_ptr<WarningsData> &warningsData);
    ~EngCoolantTempGauge();
    void Update() override;
    PriorityCategory GetPriorityCategory() const override;
};


#endif //ELMOBD_ENGCOOLANTTEMPGAUGE_H
