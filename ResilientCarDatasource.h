//
// Created by sigsegv on 5/19/23.
//

#ifndef ELMOBD_RESILIENTCARDATASOURCE_H
#define ELMOBD_RESILIENTCARDATASOURCE_H

#include "CarDatasource.h"
#include <memory>
#include <mutex>
#include <functional>

class WarningsData;
class WarningsList;

struct ResilientConnectionData {
    std::shared_ptr<CarDatasource> carDatasource{};
    std::shared_ptr<CarDatasource> capsCarDatasource{};
    std::shared_ptr<WarningsData> warningsData;
    std::shared_ptr<WarningsList> warningsList;
    bool connecting{false};

    ResilientConnectionData(const std::shared_ptr<WarningsData> &warningsData);
    ~ResilientConnectionData();
};

class ResilientCarDatasource : public CarDatasource, public std::enable_shared_from_this<ResilientCarDatasource> {
private:
    std::unique_ptr<std::mutex> mtx;
    std::unique_ptr<ResilientConnectionData> c;
public:
    ResilientCarDatasource(const std::shared_ptr<WarningsData> &warningsData);
private:
    virtual std::shared_ptr<CarDatasource> DoConnect() const = 0;
    void Connect() const;
protected:
    void Init();
public:
    void Disconnect();
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
    template <class T> std::optional<T> Resilient(const std::function<std::optional<T> (CarDatasource &conn)> &func) const {
        std::optional<T> value = {};
        if (Resilient([&value, &func] (auto &conn) {
            value = func(conn);
        })) {
            return value;
        }
        return {};
    }
    bool Resilient(const std::function<void (CarDatasource &conn)> &func) const;
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

class ResilientOBDCarDatasource : public ResilientCarDatasource {
private:
    std::string protocol{};
public:
    ResilientOBDCarDatasource(const std::shared_ptr<WarningsData> &warningsData);
    std::shared_ptr<CarDatasource> DoConnect() const override;
};

#endif //ELMOBD_RESILIENTCARDATASOURCE_H
