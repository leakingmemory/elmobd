//
// Created by sigsegv on 5/13/23.
//

#ifndef ELMOBD_SERIALCARDEVICE_H
#define ELMOBD_SERIALCARDEVICE_H

#include "SerialCanDevice.h"
#include "CarDatasource.h"

class SerialCarDevice : public SerialCanDevice, public CarDatasource {
public:
    SerialCarDevice(SerialInterface &&mv, const std::string &protocol = "");
private:
public:
    bool HasStatus() const override;
    bool HasFuelSystemStatus() const override;
    bool HasCalculatedLoad() const override;
    bool HasCoolantTemperature() const override;
    bool HasShortTermFuelTrimBank1() const override;
    bool HasLongTermFuelTrimBank1() const override;
    bool HasShortTermFuelTrimBank2() const override;
    bool HasLongTermFuelTrimBank2() const override;
    bool HasFuelGaugePressure() const override;
    bool HasIntakeManifoldAbsPressure() const override;
    bool HasRPM() const override;
    bool HasSpeed() const override;
    bool HasTimingAdvance() const override;
    bool HasIntakeAirTemperature() const override;
    bool HasMassAirFlow() const override;
    bool HasThrottlePos() const override;
    bool HasO2Sensor(int n) const override;
    OBDStatus GetStatus() const override;
    FuelSystemStatus GetFuelSystemStatus() const override;
    int GetCalculatedLoad() const override;
    int GetCoolantTemperature() const override;
    int GetShortTermFuelTrimBank1() const override;
    int GetLongTermFuelTrimBank1() const override;
    int GetShortTermFuelTrimBank2() const override;
    int GetLongTermFuelTrimBank2() const override;
    int GetFuelGaugePressure() const override;
    int GetIntakeManifoldAbsPressure() const override;
    int GetRPM() const override;
    int GetSpeed() const override;
    float GetTimingAdvance() const override;
    int GetIntakeAirTemperature() const override;
    float GetMassAirFlow() const override;
    float GetThrottlePos() const override;
    O2Sensor GetO2Sensor(int n) const override;
    bool HasVIN() const override;
    std::string GetVIN() const override;
};


#endif //ELMOBD_SERIALCARDEVICE_H
