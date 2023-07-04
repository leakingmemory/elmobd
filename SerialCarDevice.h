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
    void Disconnect() override;
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
    std::optional<OBDStatus> GetStatus() const override;
    std::optional<FuelSystemStatus> GetFuelSystemStatus() const override;
    std::optional<int> GetCalculatedLoad() const override;
    std::optional<int> GetCoolantTemperature() const override;
    std::optional<int> GetShortTermFuelTrimBank1() const override;
    std::optional<int> GetLongTermFuelTrimBank1() const override;
    std::optional<int> GetShortTermFuelTrimBank2() const override;
    std::optional<int> GetLongTermFuelTrimBank2() const override;
    std::optional<int> GetFuelGaugePressure() const override;
    std::optional<int> GetIntakeManifoldAbsPressure() const override;
    std::optional<int> GetRPM() const override;
    std::optional<int> GetSpeed() const override;
    std::optional<float> GetTimingAdvance() const override;
    std::optional<int> GetIntakeAirTemperature() const override;
    std::optional<float> GetMassAirFlow() const override;
    std::optional<float> GetThrottlePos() const override;
    std::optional<O2Sensor> GetO2Sensor(int n) const override;
    bool HasVIN() const override;
    std::optional<std::string> GetVIN() const override;
    bool ClearDTCEtc() override;
    std::optional<std::vector<std::string>> GetDTCs() const override;
    std::optional<std::vector<std::string>> GetPendingDTCs() const override;
};


#endif //ELMOBD_SERIALCARDEVICE_H
