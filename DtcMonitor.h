//
// Created by sigsegv on 6/27/23.
//

#ifndef ELMOBD_DTCMONITOR_H
#define ELMOBD_DTCMONITOR_H

#include "WarningsData.h"
#include "Meter.h"

class CarDatasource;

class DtcMonitor : public Meter {
private:
    std::shared_ptr<CarDatasource> carDatasource;
    std::shared_ptr<WarningsData> warningsData;
    std::shared_ptr<WarningsList> warningsList;
    std::vector<std::string> dtcs{};
public:
    DtcMonitor(const std::shared_ptr<CarDatasource> &serialCarDevice, const std::shared_ptr<WarningsData> &warningsData);
    void Update() override;
    PriorityCategory GetPriorityCategory() const override;
};


#endif //ELMOBD_DTCMONITOR_H
