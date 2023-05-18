//
// Created by sigsegv on 5/17/23.
//

#ifndef ELMOBD_BANK1LONGTERMFUELTRIMGAUGE_H
#define ELMOBD_BANK1LONGTERMFUELTRIMGAUGE_H

#include "LongTermFuelTrimGauge.h"
#include "WarningsData.h"

class Bank1LongTermFuelTrimGauge : public LongTermFuelTrimGauge {
private:
    std::shared_ptr<WarningsData> warningsData;
    std::shared_ptr<WarningsList> warningsList;
public:
    Bank1LongTermFuelTrimGauge(const std::shared_ptr<CarDatasource> &serialCarDevice, const std::shared_ptr<WarningsData> &warningsData);
    ~Bank1LongTermFuelTrimGauge();
    void Update() override;
};


#endif //ELMOBD_BANK1LONGTERMFUELTRIMGAUGE_H
