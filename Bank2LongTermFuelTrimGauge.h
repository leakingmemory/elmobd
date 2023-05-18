//
// Created by sigsegv on 5/17/23.
//

#ifndef ELMOBD_BANK2LONGTERMFUELTRIMGAUGE_H
#define ELMOBD_BANK2LONGTERMFUELTRIMGAUGE_H

#include "LongTermFuelTrimGauge.h"
#include "WarningsData.h"

class Bank2LongTermFuelTrimGauge : public LongTermFuelTrimGauge {
private:
    std::shared_ptr<WarningsData> warningsData;
    std::shared_ptr<WarningsList> warningsList;
public:
    Bank2LongTermFuelTrimGauge(const std::shared_ptr<CarDatasource> &serialCarDevice, const std::shared_ptr<WarningsData> &warningsData);
    ~Bank2LongTermFuelTrimGauge();
    void Update() override;
};


#endif //ELMOBD_BANK2LONGTERMFUELTRIMGAUGE_H
