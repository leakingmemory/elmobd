//
// Created by sigsegv on 5/13/23.
//

#include <iostream>
#include "SerialCarDevice.h"
#include "SerialInterface.h"

class SerialCarException : public std::exception {
private:
    const char *error;
public:
    SerialCarException(const char *error) : error(error) {}
    const char * what() const noexcept override {
        return error;
    }
};

SerialCarDevice::SerialCarDevice(SerialInterface &&mv) : SerialCanDevice(std::move(mv)) {
}

bool SerialCarDevice::HasStatus() const {
    if ((pidsA & 0x80000000) != 0) {
        return true;
    } else {
        return false;
    }
}

bool SerialCarDevice::HasFuelSystemStatus() const {
    if ((pidsA & 0x20000000) != 0) {
        return true;
    } else {
        return false;
    }
}

bool SerialCarDevice::HasCalculatedLoad() const {
    if ((pidsA & 0x10000000) != 0) {
        return true;
    } else {
        return false;
    }
}

bool SerialCarDevice::HasCoolantTemperature() const {
    if ((pidsA & 0x08000000) != 0) {
        return true;
    } else {
        return false;
    }
}

bool SerialCarDevice::HasShortTermFuelTrimBank1() const {
    if ((pidsA & 0x04000000) != 0) {
        return true;
    } else {
        return false;
    }
}

bool SerialCarDevice::HasLongTermFuelTrimBank1() const {
    if ((pidsA & 0x02000000) != 0) {
        return true;
    } else {
        return false;
    }
}

bool SerialCarDevice::HasShortTermFuelTrimBank2() const {
    if ((pidsA & 0x01000000) != 0) {
        return true;
    } else {
        return false;
    }
}

bool SerialCarDevice::HasLongTermFuelTrimBank2() const {
    if ((pidsA & 0x00800000) != 0) {
        return true;
    } else {
        return false;
    }
}

bool SerialCarDevice::HasFuelGaugePressure() const {
    if ((pidsA & 0x00400000) != 0) {
        return true;
    } else {
        return false;
    }
}

bool SerialCarDevice::HasIntakeManifoldAbsPressure() const {
    if ((pidsA & 0x00200000) != 0) {
        return true;
    } else {
        return false;
    }
}

bool SerialCarDevice::HasRPM() const {
    if ((pidsA & 0x00100000) != 0) {
        return true;
    } else {
        return false;
    }
}

bool SerialCarDevice::HasVIN() const {
    if ((pid9s & 0x40000000) != 0) {
        return true;
    } else {
        return false;
    }
}

OBDStatus SerialCarDevice::GetStatus() const {
    serialInterface->Write("0101\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 200)) {
        throw SerialCarException("Status no resp");
    }
    auto msg = DecodeHex(ln);
    if (ReplyCmd(msg) != 0x101) {
        throw SerialCarException("Status wrong resp");
    }
    uint32_t statusdw = (uint32_t) PayloadInt(msg);
    bool compressionIgnission =                    (statusdw & 0x00080000);
    OBDStatus status{
        .DTCs = (uint8_t)                         ((statusdw & 0x7F000000) >> 24),
        .ComponentTestAvailable =                  (statusdw & 0x00040000) != 0,
        .FuelSystemTestAvailable =                 (statusdw & 0x00020000) != 0,
        .MisfireTestAvailable =                    (statusdw & 0x00010000) != 0,
        .EGROrVVTTestAvailable =                   (statusdw & 0x00008000) != 0,
        .OxygenSensorHeaterTestAvailable =        ((statusdw & 0x00004000) != 0) && !compressionIgnission,
        .OxygenSensorTestAvailable =              ((statusdw & 0x00002000) != 0) && !compressionIgnission,
        .GasolineParticulateFilterTestAvailable = ((statusdw & 0x00001000) != 0) && !compressionIgnission,
        .SecondaryAirSystemTestAvailable =        ((statusdw & 0x00000800) != 0) && !compressionIgnission,
        .ComponentTestDone =                       (statusdw & 0x00400000) != 0,
        .FuelSystemTestDone =                      (statusdw & 0x00200000) != 0,
        .MisfireTestDone =                         (statusdw & 0x00100000) != 0,
        .EGROrVVTTestDone =                        (statusdw & 0x00000080) != 0,
        .OxygenSensorHeaterTestDone =             ((statusdw & 0x00000040) != 0) && !compressionIgnission,
        .OxygenSensorTestDone =                   ((statusdw & 0x00000020) != 0) && !compressionIgnission,
        .GasolineParticulateFilterTestDone =      ((statusdw & 0x00000010) != 0) && !compressionIgnission,
        .SecondaryAirSystemTestDone =             ((statusdw & 0x00000008) != 0) && !compressionIgnission,
        .EvaporativeSystemTestAvailable =         ((statusdw & 0x00000400) != 0) && !compressionIgnission,
        .HeatedCatalystTestAvailable =            ((statusdw & 0x00000200) != 0) && !compressionIgnission,
        .CatalystTestAvailable =                  ((statusdw & 0x00000100) != 0) && !compressionIgnission,
        .PMFilterMonitoringTestAvailable =        ((statusdw & 0x00004000) != 0) && compressionIgnission,
        .ExhaustGasSensorTestAvailable =          ((statusdw & 0x00002000) != 0) && compressionIgnission,
        .BoostPressureTestAvailable =             ((statusdw & 0x00000800) != 0) && compressionIgnission,
        .NOxSCRMonitorTestAvailable =             ((statusdw & 0x00000200) != 0) && compressionIgnission,
        .NMHCCatalystTestAvailable =              ((statusdw & 0x00000100) != 0) && compressionIgnission,
        .EvaporativeSystemTestDone =              ((statusdw & 0x00000004) != 0) && !compressionIgnission,
        .HeatedCatalystTestDone =                 ((statusdw & 0x00000002) != 0) && !compressionIgnission,
        .CatalystTestDone =                       ((statusdw & 0x00000001) != 0) && !compressionIgnission,
        .PMFilterMonitoringTestDone =             ((statusdw & 0x00000040) != 0) && compressionIgnission,
        .ExhaustGasSensorTestDone =               ((statusdw & 0x00000020) != 0) && compressionIgnission,
        .BoostPressureTestDone =                  ((statusdw & 0x00000008) != 0) && compressionIgnission,
        .NOxSCRMonitorTestDone =                  ((statusdw & 0x00000002) != 0) && compressionIgnission,
        .NMHCCatalystTestDone =                   ((statusdw & 0x00000001) != 0) && compressionIgnission,
        .MIL =                                     (statusdw & 0x80000000) != 0,
        .CompressionIgnition = compressionIgnission
    };
    WaitForPrompt(buf, 1000);
    return status;
}

constexpr FuelSystemStatusValue Decode(uint8_t val) {
    switch (val) {
        case 0:
            return FuelSystemStatusValue::OFF;
        case 1:
            return FuelSystemStatusValue::OPEN_LOOP_INSUFFICIENT_ENG_TEMP;
        case 2:
            return FuelSystemStatusValue::CLOSED_LOOP;
        case 4:
            return FuelSystemStatusValue::OPEN_LOOP_DUE_ENG_LOAD_OR_FUEL_CUT;
        case 8:
            return FuelSystemStatusValue::OPEN_LOOP_DUE_FAILURE;
        case 16:
            return FuelSystemStatusValue::CLOSED_LOOP_WITH_FAULT;
        default:
            return FuelSystemStatusValue::INVALID;
    }
}

FuelSystemStatus SerialCarDevice::GetFuelSystemStatus() const {
    serialInterface->Write("0103\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 200)) {
        throw SerialCarException("Fuel system status no resp");
    }
    auto msg = DecodeHex(ln);
    if (ReplyCmd(msg) != 0x103) {
        throw SerialCarException("Fuel system status wrong resp");
    }
    uint16_t statusw = (uint16_t) PayloadInt(msg);
    uint8_t s1 = (uint8_t) ((statusw >> 8) & 0xFF);
    uint8_t s2 = (uint8_t) (statusw & 0xFF);
    return {
        .system1 = Decode(s1),
        .system2 = Decode(s2)
    };
}

int SerialCarDevice::GetCalculatedLoad() const {
    serialInterface->Write("0104\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 200)) {
        throw SerialCarException("Calculated load no resp");
    }
    auto msg = DecodeHex(ln);
    if (ReplyCmd(msg) != 0x104) {
        throw SerialCarException("Calculated load wrong resp");
    }
    auto cload = PayloadInt(msg);
    cload *= 100;
    cload /= 255;
    return cload;
}

int SerialCarDevice::GetCoolantTemperature() const {
    serialInterface->Write("0105\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 200)) {
        throw SerialCarException("Coolant temp no resp");
    }
    auto msg = DecodeHex(ln);
    if (ReplyCmd(msg) != 0x105) {
        throw SerialCarException("Coolant temp wrong resp");
    }
    int coolant = (int) PayloadInt(msg);
    coolant -= 40;
    return coolant;
}

int SerialCarDevice::GetShortTermFuelTrimBank1() const {
    serialInterface->Write("0106\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 200)) {
        throw SerialCarException("Short term fuel trim bank 1 no resp");
    }
    auto msg = DecodeHex(ln);
    if (ReplyCmd(msg) != 0x106) {
        throw SerialCarException("Short term fuel trim bank 1 wrong resp");
    }
    int trim = (int) PayloadInt(msg);
    trim *= 100;
    trim /= 255;
    trim -= 100;
    return trim;
}

int SerialCarDevice::GetLongTermFuelTrimBank1() const {
    serialInterface->Write("0107\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 200)) {
        throw SerialCarException("Long term fuel trim bank 1 no resp");
    }
    auto msg = DecodeHex(ln);
    if (ReplyCmd(msg) != 0x107) {
        throw SerialCarException("Long term fuel trim bank 1 wrong resp");
    }
    int trim = (int) PayloadInt(msg);
    trim *= 100;
    trim /= 255;
    trim -= 100;
    return trim;
}

int SerialCarDevice::GetShortTermFuelTrimBank2() const {
    serialInterface->Write("0108\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 200)) {
        throw SerialCarException("Short term fuel trim bank 2 no resp");
    }
    auto msg = DecodeHex(ln);
    if (ReplyCmd(msg) != 0x108) {
        throw SerialCarException("Short term fuel trim bank 2 wrong resp");
    }
    int trim = (int) PayloadInt(msg);
    trim *= 100;
    trim /= 255;
    trim -= 100;
    return trim;
}

int SerialCarDevice::GetLongTermFuelTrimBank2() const {
    serialInterface->Write("0109\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 200)) {
        throw SerialCarException("Long term fuel trim bank 2 no resp");
    }
    auto msg = DecodeHex(ln);
    if (ReplyCmd(msg) != 0x109) {
        throw SerialCarException("Long term fuel trim bank 2 wrong resp");
    }
    int trim = (int) PayloadInt(msg);
    trim *= 100;
    trim /= 255;
    trim -= 100;
    return trim;
}

int SerialCarDevice::GetFuelGaugePressure() const {
    serialInterface->Write("010A\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 200)) {
        throw SerialCarException("Fuel gauge pressure no resp");
    }
    auto msg = DecodeHex(ln);
    if (ReplyCmd(msg) != 0x10A) {
        throw SerialCarException("Fuel gauge pressure wrong resp");
    }
    int press = (int) PayloadInt(msg);
    press *= 3;
    return press;
}

int SerialCarDevice::GetIntakeManifoldAbsPressure() const {
    serialInterface->Write("010B\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 200)) {
        throw SerialCarException("Intake manifold abs pressure no resp");
    }
    auto msg = DecodeHex(ln);
    if (ReplyCmd(msg) != 0x10B) {
        throw SerialCarException("Intake manifold abs pressure wrong resp");
    }
    int press = (int) PayloadInt(msg);
    return press;
}

int SerialCarDevice::GetRPM() const {
    serialInterface->Write("010C\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 200)) {
        throw SerialCarException("RPM no resp");
    }
    auto msg = DecodeHex(ln);
    if (ReplyCmd(msg) != 0x10C) {
        throw SerialCarException("RPM wrong resp");
    }
    auto rawRpm = PayloadInt(msg);
    WaitForPrompt(buf, 1000);
    return (int) (rawRpm / 4);
}

std::string SerialCarDevice::GetVIN() const {
    serialInterface->Write("0902\r");
    std::vector<std::string> lns{};
    {
        std::string buf{};
        auto data = WaitForPrompt(buf, 7000);
        {
            auto iterator = data.begin();
            while (iterator != data.end()) {
                if (*iterator == ' ') {
                    data.erase(iterator);
                } else {
                    ++iterator;
                }
            }
        }
        lns = SplitLines(data);
    }
    std::string vin{};
    for (const auto ln : lns) {
        auto msg = DecodeHex(ln);
        if (ReplyCmd(msg) == 0x902) {
            vin.append(Payload(msg));
        }
    }
    return vin;
}
