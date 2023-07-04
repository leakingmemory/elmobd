//
// Created by sigsegv on 5/17/23.
//

#include "IntakeAirTempGauge.h"
#include "CarDatasource.h"
#include "WarningsData.h"

IntakeAirTempGauge::IntakeAirTempGauge(const std::shared_ptr<CarDatasource> &serialCarDevice, const std::shared_ptr<WarningsData> &warningsData) : serialCarDevice(serialCarDevice), warningsData(warningsData), warningsList(warningsData->Create())  {
    min = -40;
    max = 40;
    mark = 10;
    markOffset = -40;
    caption = "IAT (Celsius)";
}

IntakeAirTempGauge::~IntakeAirTempGauge() {
    warningsData->Remove(warningsList);
}

void IntakeAirTempGauge::Update() {
    auto optTemp = serialCarDevice->GetIntakeAirTemperature();
    std::vector<std::string> messages{};
    if (optTemp) {
        auto temp = *optTemp;
        SetCurrentValue((float) temp);
        if (temp > -2 && temp < 2) {
            messages.emplace_back("ICING COND");
        }
    } else {
        SetInvalid();
    }
    std::lock_guard lock{warningsList->mtx};
    warningsList->messages = messages;
}

PriorityCategory IntakeAirTempGauge::GetPriorityCategory() const {
    return PriorityCategory::LOW;
}
