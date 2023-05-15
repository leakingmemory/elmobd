#include <iostream>
#include <thread>
#include <chrono>
#include "SerialInterface.h"
#include "SerialCarDevice.h"

int main() {
    SerialInterface serialInterface{"/dev/ttyUSB0"};
    serialInterface.SetSpeed(SerialSpeed::S38400);
    serialInterface.CommitAttributes();
    SerialCarDevice serialCarDevice{std::move(serialInterface)};
    std::cout << serialCarDevice.GetDeviceId() << "\n";
    std::cout << "Car voltage: " << serialCarDevice.GetVoltage() << "V\n";
    if (serialCarDevice.HasVIN()) {
        std::cout << "VIN: " << serialCarDevice.VIN() << "\n";
    }
    auto hasStatus = serialCarDevice.HasStatus();
    if (hasStatus) {
        OBDStatus status = serialCarDevice.Status();
        std::cout << "MIL/CEL - Check engine light: " << (status.MIL ? "Illuminated/Fault" : "Off/Good") << "\n";
        std::cout << "Num confirmed DTCs: " << ((int )status.DTCs) << "\n";
        std::cout << "Ignition type / fuel type: " << (status.CompressionIgnition ? "Compression ignition, typical diesel\n" : "Spark ignition, typical gasoline\n");
        std::cout << "Available tests: ";
        if (status.ComponentTestAvailable) {
            std::cout << " component";
        }
        if (status.FuelSystemTestAvailable) {
            std::cout << " fuel-system";
        }
        if (status.MisfireTestAvailable) {
            std::cout << " misfire";
        }
        if (status.EGROrVVTTestAvailable) {
            std::cout << " EGR/VVT";
        }
        if (status.OxygenSensorHeaterTestAvailable) {
            std::cout << " oxygen-sensor-heater";
        }
        if (status.OxygenSensorTestAvailable) {
            std::cout << " oxygen-sensor";
        }
        if (status.GasolineParticulateFilterTestAvailable) {
            std::cout << " gasoline-particulate-filter";
        }
        if (status.SecondaryAirSystemTestAvailable) {
            std::cout << " secondary-air-system";
        }
        if (status.EvaporativeSystemTestAvailable) {
            std::cout << " evaporative-system";
        }
        if (status.HeatedCatalystTestAvailable) {
            std::cout << " heated-catalyst";
        }
        if (status.CatalystTestAvailable) {
            std::cout << " catalyst";
        }
        if (status.PMFilterMonitoringTestAvailable) {
            std::cout << "PM-filter-monitoring";
        }
        if (status.ExhaustGasSensorTestAvailable) {
            std::cout << " exhaust-gas-sensor";
        }
        if (status.BoostPressureTestAvailable) {
            std::cout << " boost-pressure";
        }
        if (status.NOxSCRMonitorTestAvailable) {
            std::cout << " NOx/SCR-monitor";
        }
        if (status.NMHCCatalystTestAvailable) {
            std::cout << " NMHC-catalyst";
        }
        std::cout << "\n";
        std::cout << "Completed tests: ";
        if (status.ComponentTestAvailable && status.ComponentTestDone) {
            std::cout << " component";
        }
        if (status.FuelSystemTestAvailable && status.FuelSystemTestDone) {
            std::cout << " fuel-system";
        }
        if (status.MisfireTestAvailable && status.MisfireTestDone) {
            std::cout << " misfire";
        }
        if (status.EGROrVVTTestAvailable && status.EGROrVVTTestDone) {
            std::cout << " EGR/VVT";
        }
        if (status.OxygenSensorHeaterTestAvailable && status.OxygenSensorHeaterTestDone) {
            std::cout << " oxygen-sensor-heater";
        }
        if (status.OxygenSensorTestAvailable && status.OxygenSensorTestDone) {
            std::cout << " oxygen-sensor";
        }
        if (status.GasolineParticulateFilterTestAvailable && status.GasolineParticulateFilterTestDone) {
            std::cout << " gasoline-particulate-filter";
        }
        if (status.SecondaryAirSystemTestAvailable && status.SecondaryAirSystemTestDone) {
            std::cout << " secondary-air-system";
        }
        if (status.EvaporativeSystemTestAvailable && status.EvaporativeSystemTestDone) {
            std::cout << " evaporative-system";
        }
        if (status.HeatedCatalystTestAvailable && status.HeatedCatalystTestDone) {
            std::cout << " heated-catalyst";
        }
        if (status.CatalystTestAvailable && status.CatalystTestDone) {
            std::cout << " catalyst";
        }
        if (status.PMFilterMonitoringTestAvailable && status.PMFilterMonitoringTestDone) {
            std::cout << "PM-filter-monitoring";
        }
        if (status.ExhaustGasSensorTestAvailable && status.ExhaustGasSensorTestDone) {
            std::cout << " exhaust-gas-sensor";
        }
        if (status.BoostPressureTestAvailable && status.BoostPressureTestDone) {
            std::cout << " boost-pressure";
        }
        if (status.NOxSCRMonitorTestAvailable && status.NOxSCRMonitorTestDone) {
            std::cout << " NOx/SCR-monitor";
        }
        if (status.NMHCCatalystTestAvailable && status.NMHCCatalystTestDone) {
            std::cout << " NMHC-catalyst";
        }
        std::cout << "\n";
    }
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(5s);
    }
    auto hasRPM = serialCarDevice.HasRPM();
    while (true) {
        auto rpm = hasRPM ? serialCarDevice.RPM() : -1;
        if (hasRPM) {
            std::cout << " RPM=" << rpm;
            std::cout << "\n";
        } else {
            std::cout << "\n";
        }
    }
    return 0;
}
