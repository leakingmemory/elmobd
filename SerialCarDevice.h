//
// Created by sigsegv on 5/13/23.
//

#ifndef ELMOBD_SERIALCARDEVICE_H
#define ELMOBD_SERIALCARDEVICE_H

#include "SerialCanDevice.h"

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

class SerialCarDevice : public SerialCanDevice {
public:
    SerialCarDevice(SerialInterface &&mv);
private:
public:
    bool HasStatus() const;
    bool HasFuelSystemStatus() const;
    bool HasCalculatedLoad() const;
    bool HasCoolantTemperature() const;
    bool HasShortTermFuelTrimBank1() const;
    bool HasLongTermFuelTrimBank1() const;
    bool HasShortTermFuelTrimBank2() const;
    bool HasLongTermFuelTrimBank2() const;
    bool HasFuelGaugePressure() const;
    bool HasIntakeManifoldAbsPressure() const;
    bool HasRPM() const;
    OBDStatus GetStatus() const;
    FuelSystemStatus GetFuelSystemStatus() const;
    int GetCalculatedLoad() const;
    int GetCoolantTemperature() const;
    int GetShortTermFuelTrimBank1() const;
    int GetLongTermFuelTrimBank1() const;
    int GetShortTermFuelTrimBank2() const;
    int GetLongTermFuelTrimBank2() const;
    int GetFuelGaugePressure() const;
    int GetIntakeManifoldAbsPressure() const;
    int GetRPM() const;
    bool HasVIN() const;
    std::string GetVIN() const;
};


#endif //ELMOBD_SERIALCARDEVICE_H
