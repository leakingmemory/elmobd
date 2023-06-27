//
// Created by sigsegv on 5/18/23.
//

#ifndef ELMOBD_CARDATASOURCE_H
#define ELMOBD_CARDATASOURCE_H

#include <cstdint>
#include <string>
#include <vector>

struct OBDStatus {
    uint8_t DTCs;
    /**/
    bool ComponentTestAvailable : 1;
    bool FuelSystemTestAvailable : 1;
    bool MisfireTestAvailable : 1;
    bool EGROrVVTTestAvailable : 1;
    bool OxygenSensorHeaterTestAvailable : 1;
    bool OxygenSensorTestAvailable : 1;
    bool GasolineParticulateFilterTestAvailable : 1;
    bool SecondaryAirSystemTestAvailable : 1;
    /**/
    bool ComponentTestDone : 1;
    bool FuelSystemTestDone : 1;
    bool MisfireTestDone : 1;
    bool EGROrVVTTestDone : 1;
    bool OxygenSensorHeaterTestDone : 1;
    bool OxygenSensorTestDone : 1;
    bool GasolineParticulateFilterTestDone : 1;
    bool SecondaryAirSystemTestDone : 1;
    /**/
    bool EvaporativeSystemTestAvailable : 1;
    bool HeatedCatalystTestAvailable : 1;
    bool CatalystTestAvailable : 1;
    bool PMFilterMonitoringTestAvailable : 1;
    bool ExhaustGasSensorTestAvailable : 1;
    bool BoostPressureTestAvailable : 1;
    bool NOxSCRMonitorTestAvailable : 1;
    bool NMHCCatalystTestAvailable : 1;
    /**/
    bool EvaporativeSystemTestDone : 1;
    bool HeatedCatalystTestDone : 1;
    bool CatalystTestDone : 1;
    bool PMFilterMonitoringTestDone : 1;
    bool ExhaustGasSensorTestDone : 1;
    bool BoostPressureTestDone : 1;
    bool NOxSCRMonitorTestDone : 1;
    bool NMHCCatalystTestDone : 1;
    /**/
    bool MIL : 1; // Check engine light
    bool CompressionIgnition : 1;
};

enum class FuelSystemStatusValue {
    OFF,
    OPEN_LOOP_INSUFFICIENT_ENG_TEMP,
    CLOSED_LOOP, // O2 sensor determ fuel mix
    OPEN_LOOP_DUE_ENG_LOAD_OR_FUEL_CUT,
    OPEN_LOOP_DUE_FAILURE,
    CLOSED_LOOP_WITH_FAULT,
    INVALID
};

struct FuelSystemStatus {
    FuelSystemStatusValue system1;
    FuelSystemStatusValue system2;
};

struct O2Sensor {
    float Voltage;
    int ShortTermFuelTrim;
};

class CarDatasource {
public:
    virtual void Disconnect() = 0;
    virtual bool HasStatus() const = 0;
    virtual bool HasFuelSystemStatus() const = 0;
    virtual bool HasCalculatedLoad() const = 0;
    virtual bool HasCoolantTemperature() const = 0;
    virtual bool HasShortTermFuelTrimBank1() const = 0;
    virtual bool HasLongTermFuelTrimBank1() const = 0;
    virtual bool HasShortTermFuelTrimBank2() const = 0;
    virtual bool HasLongTermFuelTrimBank2() const = 0;
    virtual bool HasFuelGaugePressure() const = 0;
    virtual bool HasIntakeManifoldAbsPressure() const = 0;
    virtual bool HasRPM() const = 0;
    virtual bool HasSpeed() const = 0;
    virtual bool HasTimingAdvance() const = 0;
    virtual bool HasIntakeAirTemperature() const = 0;
    virtual bool HasMassAirFlow() const = 0;
    virtual bool HasThrottlePos() const = 0;
    virtual bool HasO2Sensor(int n) const = 0;
    virtual OBDStatus GetStatus() const = 0;
    virtual FuelSystemStatus GetFuelSystemStatus() const = 0;
    virtual int GetCalculatedLoad() const = 0;
    virtual int GetCoolantTemperature() const = 0;
    virtual int GetShortTermFuelTrimBank1() const = 0;
    virtual int GetLongTermFuelTrimBank1() const = 0;
    virtual int GetShortTermFuelTrimBank2() const = 0;
    virtual int GetLongTermFuelTrimBank2() const = 0;
    virtual int GetFuelGaugePressure() const = 0;
    virtual int GetIntakeManifoldAbsPressure() const = 0;
    virtual int GetRPM() const = 0;
    virtual int GetSpeed() const = 0;
    virtual float GetTimingAdvance() const = 0;
    virtual int GetIntakeAirTemperature() const = 0;
    virtual float GetMassAirFlow() const = 0;
    virtual float GetThrottlePos() const = 0;
    virtual O2Sensor GetO2Sensor(int n) const = 0;
    virtual bool HasVIN() const = 0;
    virtual std::string GetVIN() const = 0;
    virtual void ClearDTCEtc() = 0;
    virtual std::vector<std::string> GetDTCs() const = 0;
    virtual std::vector<std::string> GetPendingDTCs() const = 0;
};

#endif //ELMOBD_CARDATASOURCE_H
