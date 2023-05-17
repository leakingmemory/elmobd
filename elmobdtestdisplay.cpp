//
// Created by sigsegv on 5/18/23.
//

#include "ElmObdDisplay.h"
#include "CarDatasource.h"
#include <cstdlib>
#include <memory>

class TestDatasource : public CarDatasource {
public:
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
        return false;
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
        return false;
    }
    virtual OBDStatus GetStatus() const {
        return {};
    }
    virtual FuelSystemStatus GetFuelSystemStatus() const {
        return {};
    }
    virtual int GetCalculatedLoad() const {
        return 0;
    }
    virtual int GetCoolantTemperature() const {
        return 0;
    }
    virtual int GetShortTermFuelTrimBank1() const {
        float r = (float) rand();
        r /= RAND_MAX;
        r *= 200;
        r -= 100;
        return (int) r;
    }
    virtual int GetLongTermFuelTrimBank1() const {
        float r = (float) rand();
        r /= RAND_MAX;
        r *= 200;
        r -= 100;
        return (int) r;
    }
    virtual int GetShortTermFuelTrimBank2() const {
        float r = (float) rand();
        r /= RAND_MAX;
        r *= 200;
        r -= 100;
        return (int) r;
    }
    virtual int GetLongTermFuelTrimBank2() const {
        float r = (float) rand();
        r /= RAND_MAX;
        r *= 200;
        r -= 100;
        return (int) r;
    }
    virtual int GetFuelGaugePressure() const {
        return 0;
    }
    virtual int GetIntakeManifoldAbsPressure() const {
        float r = (float) rand();
        r /= RAND_MAX;
        r *= 92;
        r += 20;
        return (int) r;
    }
    virtual int GetRPM() const {
        return 0;
    }
    virtual int GetSpeed() const {
        return 0;
    }
    virtual float GetTimingAdvance() const {
        return 0;
    }
    virtual int GetIntakeAirTemperature() const {
        float r = (float) rand();
        r /= RAND_MAX;
        r *= 80;
        r -= 40;
        return (int) r;
    }
    virtual float GetMassAirFlow() const {
        float r = (float) rand();
        r /= RAND_MAX;
        r *= 120;
        return r;
    }
    virtual float GetThrottlePos() const {
        return 0;
    }
    virtual O2Sensor GetO2Sensor(int n) const {
        return {};
    }
    virtual bool HasVIN() const {
        return false;
    }
    virtual std::string GetVIN() const {
        return "";
    }
};

int main() {
    auto testSource = std::make_shared<TestDatasource>();
    ElmObdDisplay elmObdDisplay{testSource};
    elmObdDisplay.Run();
}