//
// Created by sigsegv on 6/27/23.
//

#ifndef ELMOBD_DTCMONITOR_H
#define ELMOBD_DTCMONITOR_H

#include "WarningsData.h"
#include "Meter.h"

class CarDatasource;

class DtcMonitor : public Meter {
protected:
    std::shared_ptr<CarDatasource> carDatasource;
private:
    std::shared_ptr<WarningsData> warningsData;
    std::shared_ptr<WarningsList> warningsList;
    std::vector<std::string> dtcs{};
public:
    DtcMonitor(const std::shared_ptr<CarDatasource> &serialCarDevice, const std::shared_ptr<WarningsData> &warningsData);
    void Update() override;
    PriorityCategory GetPriorityCategory() const override;
    virtual std::string GetPrefix() const = 0;
    virtual std::vector<std::string> GetCodes() const = 0;
};

class DtcStoredMonitor : public DtcMonitor {
public:
    DtcStoredMonitor(const std::shared_ptr<CarDatasource> &serialCarDevice, const std::shared_ptr<WarningsData> &warningsData) : DtcMonitor(serialCarDevice, warningsData) {}
    std::string GetPrefix() const override;
    std::vector<std::string> GetCodes() const override;
};

class DtcPendingMonitor : public DtcMonitor {
public:
    DtcPendingMonitor(const std::shared_ptr<CarDatasource> &serialCarDevice, const std::shared_ptr<WarningsData> &warningsData) : DtcMonitor(serialCarDevice, warningsData) {}
    std::string GetPrefix() const override;
    std::vector<std::string> GetCodes() const override;
};

#endif //ELMOBD_DTCMONITOR_H
