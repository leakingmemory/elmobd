#include <iostream>
#include <thread>
#include <chrono>
#include "SerialInterface.h"
#include "SerialCarDevice.h"

void PrintFuelSystemStatus(FuelSystemStatusValue v) {
    switch (v) {
        case FuelSystemStatusValue::OFF:
            std::cout << "Off";
            break;
        case FuelSystemStatusValue::OPEN_LOOP_INSUFFICIENT_ENG_TEMP:
            std::cout << "Open loop due to insufficien eng temp";
            break;
        case FuelSystemStatusValue::CLOSED_LOOP:
            std::cout << "Closed loop (normal mode)";
            break;
        case FuelSystemStatusValue::OPEN_LOOP_DUE_ENG_LOAD_OR_FUEL_CUT:
            std::cout << "Open loop due to engine load or cutoff";
            break;
        case FuelSystemStatusValue::OPEN_LOOP_DUE_FAILURE:
            std::cout << "Open loop due to failure";
            break;
        case FuelSystemStatusValue::CLOSED_LOOP_WITH_FAULT:
            std::cout << "Closed loop with fault";
            break;
        default:
            std::cout << "Invalid";
    }
}

int main() {
    SerialInterface serialInterface{"/dev/ttyUSB0"};
    serialInterface.SetSpeed(SerialSpeed::S38400);
    serialInterface.CommitAttributes();
    SerialCarDevice serialCarDevice{std::move(serialInterface)};
    std::cout << serialCarDevice.GetDeviceId() << "\n";
    std::cout << "Car voltage: " << serialCarDevice.GetVoltage() << "V\n";
    if (serialCarDevice.HasVIN()) {
        std::cout << "VIN: " << serialCarDevice.GetVIN() << "\n";
    }
    auto hasStatus = serialCarDevice.HasStatus();
    if (hasStatus) {
        OBDStatus status = serialCarDevice.GetStatus();
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
    bool anyO2{false};
    bool hasO2[8];
    for (int i = 0; i < 8; i++) {
        hasO2[i] = serialCarDevice.HasO2Sensor(i);
        if (hasO2[i]) {
            anyO2 = true;
        }
    }
    if (anyO2) {
        std::cout << "O2 sensors available: ";
        bool notFirst{false};
        for (int i = 0; i < 8; i++) {
            if (hasO2[i]) {
                if (notFirst) {
                    std::cout << ", ";
                } else {
                    notFirst = true;
                }
                std::cout << i;
            }
        }
        std::cout << "\n";
    }
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(5s);
    }
    auto hasFuelSystemStatus = serialCarDevice.HasFuelSystemStatus();
    auto hasCalculatedLoad = serialCarDevice.HasCalculatedLoad();
    auto hasCoolantTemp = serialCarDevice.HasCoolantTemperature();
    auto hasShortTermFuel1 = serialCarDevice.HasShortTermFuelTrimBank1();
    auto hasLongTermFuel1 = serialCarDevice.HasLongTermFuelTrimBank1();
    auto hasShortTermFuel2 = serialCarDevice.HasShortTermFuelTrimBank2();
    auto hasLongTermFuel2 = serialCarDevice.HasLongTermFuelTrimBank2();
    auto hasFuelGaugePress = serialCarDevice.HasFuelGaugePressure();
    auto hasIntakeManAbsPress = serialCarDevice.HasIntakeManifoldAbsPressure();
    auto hasRPM = serialCarDevice.HasRPM();
    auto hasSpeed = serialCarDevice.HasSpeed();
    auto hasTimingAdv = serialCarDevice.HasTimingAdvance();
    auto hasIntakeAirTemp = serialCarDevice.HasIntakeAirTemperature();
    auto hasMassFlow = serialCarDevice.HasMassAirFlow();
    auto hasThrottle = serialCarDevice.HasThrottlePos();
    FuelSystemStatus fuelSystemStatus{.system1 = FuelSystemStatusValue::INVALID, .system2 = FuelSystemStatusValue::INVALID};
    while (true) {
        auto rpm = hasRPM ? serialCarDevice.GetRPM() : -1;
        auto calcLoad = hasCalculatedLoad ? serialCarDevice.GetCalculatedLoad() : -1;
        auto coolant = hasCoolantTemp ? serialCarDevice.GetCoolantTemperature() : -1;
        auto shortTermFuel1 = hasShortTermFuel1 ? serialCarDevice.GetShortTermFuelTrimBank1() : -1;
        auto longTermFuel1 = hasLongTermFuel1 ? serialCarDevice.GetLongTermFuelTrimBank1() : -1;
        auto shortTermFuel2 = hasShortTermFuel2 ? serialCarDevice.GetShortTermFuelTrimBank2() : -1;
        auto longTermFuel2 = hasLongTermFuel2 ? serialCarDevice.GetLongTermFuelTrimBank2() : -1;
        auto fuelGaugePress = hasFuelGaugePress ? serialCarDevice.GetFuelGaugePressure() : -1;
        auto intakeManAbsPress = hasIntakeManAbsPress ? serialCarDevice.GetIntakeManifoldAbsPressure() : -1;
        auto speed = hasSpeed ? serialCarDevice.GetSpeed() : -1;
        auto timingAdv = hasTimingAdv ? serialCarDevice.GetTimingAdvance() : -1.0f;
        auto intakeAirTemp = hasIntakeAirTemp ? serialCarDevice.GetIntakeAirTemperature() : -1;
        auto massFlow = hasMassFlow ? serialCarDevice.GetMassAirFlow() : -1.0f;
        auto throttle = hasThrottle ? serialCarDevice.GetThrottlePos() : -1.0f;
        if (hasFuelSystemStatus) {
            fuelSystemStatus = serialCarDevice.GetFuelSystemStatus();
            std::cout << " Fuel sys 1/2=";
            PrintFuelSystemStatus(fuelSystemStatus.system1);
            std::cout << " / ";
            PrintFuelSystemStatus(fuelSystemStatus.system2);
        }
        if (hasCalculatedLoad) {
            std::cout << " calc-load=" << calcLoad << "%";
        }
        if (hasCoolantTemp) {
            std::cout << " coolant=" << coolant << "C";
        }
        if (hasShortTermFuel1 || hasLongTermFuel1 || hasShortTermFuel2 || hasLongTermFuel2) {
            std::cout << " fuel-trim(";
            bool prev = hasShortTermFuel1;
            if (prev) {
                std::cout << "short-term-bank1";
            }
            if (hasLongTermFuel1) {
                if (prev) {
                    std::cout << "/lt1";
                } else {
                    std::cout << "long-term-bank1";
                }
                prev = true;
            }
            if (hasShortTermFuel2) {
                if (prev) {
                    std::cout << "/st2";
                } else {
                    std::cout << "short-term-bank2";
                }
                prev = true;
            }
            if (hasLongTermFuel2) {
                if (prev) {
                    std::cout << "/lt2";
                } else {
                    std::cout << "long-term-bank2";
                }
            }
            std::cout << ")=";
            prev = hasShortTermFuel1;
            if (prev) {
                std::cout << shortTermFuel1 << "%";
            }
            if (hasLongTermFuel1) {
                if (prev) {
                    std::cout << "/";
                } else {
                    prev = true;
                }
                std::cout << longTermFuel1 << "%";
            }
            if (hasShortTermFuel2) {
                if (prev) {
                    std::cout << "/";
                } else {
                    prev = true;
                }
                std::cout << shortTermFuel2 << "%";
            }
            if (hasLongTermFuel2) {
                if (prev) {
                    std::cout << "/";
                }
                std::cout << longTermFuel2 << "%";
            }
        }
        if (hasFuelGaugePress) {
            std::cout << " fuel-press=" << fuelGaugePress << "kPa";
        }
        if (hasIntakeManAbsPress) {
            std::cout << " intake-man-press=" << intakeManAbsPress << "kPa";
        }
        if (hasRPM) {
            std::cout << " RPM=" << rpm;
        }
        if (hasSpeed) {
            std::cout << " speed=" << speed << "kmh";
        }
        if (hasTimingAdv) {
            std::cout << " timing-adv=" << timingAdv;
        }
        if (hasIntakeAirTemp) {
            std::cout << " intake-air-temp=" << intakeAirTemp << "C";
        }
        if (hasMassFlow) {
            std::cout << " flow=" << massFlow << "g/s";
        }
        if (hasThrottle) {
            std::cout << " throttle=" << throttle << "%";
        }
        if (anyO2) {
            std::cout << " O2(V/fuel-trim)=";
            bool notFirst{false};
            for (int i = 0; i < 8; i++) {
                if (hasO2[i]) {
                    if (notFirst) {
                        std::cout << ",";
                    } else {
                        notFirst = true;
                    }
                    auto sens = serialCarDevice.GetO2Sensor(i);
                    std::cout << sens.Voltage << "V/" << sens.ShortTermFuelTrim << "%";
                }
            }
        }
        std::cout << "\n";
    }
    return 0;
}
