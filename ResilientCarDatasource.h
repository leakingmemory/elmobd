//
// Created by sigsegv on 5/19/23.
//

#ifndef ELMOBD_RESILIENTCARDATASOURCE_H
#define ELMOBD_RESILIENTCARDATASOURCE_H

#include "CarDatasource.h"
#include <memory>
#include <mutex>
#include <functional>
#include <thread>

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
    template <class T> T Resilient(const std::function<T (CarDatasource &conn)> &func, T invalidValue) const {
        std::shared_ptr<CarDatasource> conn;
        bool connecting;
        {
            std::lock_guard lock{*mtx};
            conn = c->carDatasource;
            connecting = c->connecting;
        }
        if (conn) {
            try {
                return func(*conn);
            } catch (...) {
                Connect();
            }
        } else if (!connecting) {
            Connect();
        }
        {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1s);
        }
        return invalidValue;
    }
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
    void ClearDTCEtc() override;
    std::vector<std::string> GetDTCs() const override;
    std::vector<std::string> GetPendingDTCs() const override;
};

class ResilientOBDCarDatasource : public ResilientCarDatasource {
private:
    std::string protocol{};
public:
    ResilientOBDCarDatasource(const std::shared_ptr<WarningsData> &warningsData);
    std::shared_ptr<CarDatasource> DoConnect() const override;
};

#endif //ELMOBD_RESILIENTCARDATASOURCE_H
