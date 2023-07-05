//
// Created by sigsegv on 5/18/23.
//

#include "ElmObdDisplay.h"
#include "CarDatasource.h"
#include "ResilientCarDatasource.h"
#include "WarningsData.h"
#include <cstdlib>
#include <memory>

static void RandomException() {
    float r = (float) rand();
    r /= RAND_MAX;
    if (r < 0.05) {
        throw std::exception();
    }
}

class TestDatasource : public CarDatasource {
private:
    std::vector<std::string> errorCodes{};
    std::vector<std::string> pendingCodes{};
public:
    TestDatasource() {
        errorCodes.emplace_back("P171");
        errorCodes.emplace_back("P174");
        errorCodes.emplace_back("P332");
        pendingCodes.emplace_back("P101");
        pendingCodes.emplace_back("P374");
    }
    void Disconnect() override {
    }
    virtual bool HasStatus() const {
        return false;
    }
    virtual bool HasFuelSystemStatus() const {
        return false;
    }
    virtual bool HasCalculatedLoad() const {
        return false;
    }
    virtual bool HasCoolantTemperature() const {
        return true;
    }
    virtual bool HasShortTermFuelTrimBank1() const {
        return true;
    }
    virtual bool HasLongTermFuelTrimBank1() const {
        return true;
    }
    virtual bool HasShortTermFuelTrimBank2() const {
        return true;
    }
    virtual bool HasLongTermFuelTrimBank2() const {
        return true;
    }
    virtual bool HasFuelGaugePressure() const {
        return false;
    }
    virtual bool HasIntakeManifoldAbsPressure() const {
        return true;
    }
    virtual bool HasRPM() const {
        return false;
    }
    virtual bool HasSpeed() const {
        return false;
    }
    virtual bool HasTimingAdvance() const {
        return false;
    }
    virtual bool HasIntakeAirTemperature() const {
        return true;
    }
    virtual bool HasMassAirFlow() const {
        return true;
    }
    virtual bool HasThrottlePos() const {
        return false;
    }
    virtual bool HasO2Sensor(int n) const {
        return n == 0 || n == 1 || n == 4 || n == 5;
    }
    virtual std::optional<OBDStatus> GetStatus() const {
        return {};
    }
    virtual std::optional<FuelSystemStatus> GetFuelSystemStatus() const {
        return {};
    }
    virtual std::optional<int> GetCalculatedLoad() const {
        return {};
    }
    virtual std::optional<int> GetCoolantTemperature() const {
        RandomException();
        float r = (float) rand();
        r /= RAND_MAX;
        r *= 150;
        r -= 40;
        return (int) r;
    }
    virtual std::optional<int> GetShortTermFuelTrimBank1() const {
        RandomException();
        float r = (float) rand();
        r /= RAND_MAX;
        r *= 200;
        r -= 100;
        return (int) r;
    }
    virtual std::optional<int> GetLongTermFuelTrimBank1() const {
        RandomException();
        float r = (float) rand();
        r /= RAND_MAX;
        r *= 200;
        r -= 100;
        return (int) r;
    }
    virtual std::optional<int> GetShortTermFuelTrimBank2() const {
        RandomException();
        float r = (float) rand();
        r /= RAND_MAX;
        r *= 200;
        r -= 100;
        return (int) r;
    }
    virtual std::optional<int> GetLongTermFuelTrimBank2() const {
        RandomException();
        float r = (float) rand();
        r /= RAND_MAX;
        r *= 200;
        r -= 100;
        return (int) r;
    }
    virtual std::optional<int> GetFuelGaugePressure() const {
        return 0;
    }
    virtual std::optional<int> GetIntakeManifoldAbsPressure() const {
        RandomException();
        float r = (float) rand();
        r /= RAND_MAX;
        r *= 92;
        r += 20;
        return (int) r;
    }
    virtual std::optional<int> GetRPM() const {
        return {};
    }
    virtual std::optional<int> GetSpeed() const {
        return {};
    }
    virtual std::optional<float> GetTimingAdvance() const {
        return {};
    }
    virtual std::optional<int> GetIntakeAirTemperature() const {
        RandomException();
        float r = (float) rand();
        r /= RAND_MAX;
        r *= 80;
        r -= 40;
        return (int) r;
    }
    virtual std::optional<float> GetMassAirFlow() const {
        RandomException();
        float r = (float) rand();
        r /= RAND_MAX;
        r *= 120;
        return r;
    }
    virtual std::optional<float> GetThrottlePos() const {
        return 0;
    }
    virtual std::optional<O2Sensor> GetO2Sensor(int n) const {
        RandomException();
        O2Sensor s{};
        {
            float r = (float) rand();
            r /= RAND_MAX;
            r *= 1.25;
            s.Voltage = r;
        }
        {
            float r = (float) rand();
            r /= RAND_MAX;
            r *= 200;
            r -= 100;
            s.ShortTermFuelTrim = (int) r;
        }
        return s;
    }
    virtual bool HasVIN() const {
        return false;
    }
    virtual std::optional<std::string> GetVIN() const {
        return {};
    }
    virtual bool ClearDTCEtc() {
        errorCodes.clear();
        pendingCodes.clear();
        return true;
    }
    virtual std::optional<std::vector<std::string>> GetDTCs() const {
        return errorCodes;
    }
    virtual std::optional<std::vector<std::string>> GetPendingDTCs() const {
        return pendingCodes;
    }
};

std::shared_ptr<TestDatasource> datasource{};

class ResilientTestCarDatasource : public ResilientCarDatasource {
public:
    ResilientTestCarDatasource(const std::shared_ptr<WarningsData> &warningsData) : ResilientCarDatasource(warningsData) {
        Init();
    }
    std::shared_ptr<CarDatasource> DoConnect() const override {
        if (!datasource) {
            datasource = std::make_shared<TestDatasource>();
        }
        return datasource;
    }
};

int main() {
    auto warningsData = std::make_shared<WarningsData>();
    auto testSource = std::make_shared<ResilientTestCarDatasource>(warningsData);
    ElmObdDisplay elmObdDisplay{testSource, warningsData};
    elmObdDisplay.Run();
}