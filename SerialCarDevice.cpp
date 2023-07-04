//
// Created by sigsegv on 5/13/23.
//

#include <iostream>
#include <sstream>
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

SerialCarDevice::SerialCarDevice(SerialInterface &&mv, const std::string &protocol) : SerialCanDevice(std::move(mv), protocol) {
}

void SerialCarDevice::Disconnect() {
    SerialCanDevice::Disconnect();
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

bool SerialCarDevice::HasSpeed() const {
    if ((pidsA & 0x00080000) != 0) {
        return true;
    } else {
        return false;
    }
}

bool SerialCarDevice::HasTimingAdvance() const {
    if ((pidsA & 0x00040000) != 0) {
        return true;
    } else {
        return false;
    }
}

bool SerialCarDevice::HasIntakeAirTemperature() const {
    if ((pidsA & 0x00020000) != 0) {
        return true;
    } else {
        return false;
    }
}

bool SerialCarDevice::HasMassAirFlow() const {
    if ((pidsA & 0x00010000) != 0) {
        return true;
    } else {
        return false;
    }
}

bool SerialCarDevice::HasThrottlePos() const {
    if ((pidsA & 0x00008000) != 0) {
        return true;
    } else {
        return false;
    }
}

bool SerialCarDevice::HasO2Sensor(int n) const {
    if (n < 8 && (pidsA & (0x00001000 >> n)) != 0) {
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

std::optional<OBDStatus> SerialCarDevice::GetStatus() const {
    serialInterface->Write("0101\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 2000)) {
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
    WaitForPrompt(buf, 2000);
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

std::optional<FuelSystemStatus> SerialCarDevice::GetFuelSystemStatus() const {
    serialInterface->Write("0103\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 2000)) {
        throw SerialCarException("Fuel system status no resp");
    }
    auto msg = DecodeHex(ln);
    if (ReplyCmd(msg) != 0x103) {
        throw SerialCarException("Fuel system status wrong resp");
    }
    uint16_t statusw = (uint16_t) PayloadInt(msg);
    uint8_t s1 = (uint8_t) ((statusw >> 8) & 0xFF);
    uint8_t s2 = (uint8_t) (statusw & 0xFF);
    return {{
        .system1 = Decode(s1),
        .system2 = Decode(s2)
    }};
}

std::optional<int> SerialCarDevice::GetCalculatedLoad() const {
    serialInterface->Write("0104\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 2000)) {
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

std::optional<int> SerialCarDevice::GetCoolantTemperature() const {
    serialInterface->Write("0105\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 2000)) {
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

std::optional<int> SerialCarDevice::GetShortTermFuelTrimBank1() const {
    serialInterface->Write("0106\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 2000)) {
        throw SerialCarException("Short term fuel trim bank 1 no resp");
    }
    auto msg = DecodeHex(ln);
    if (ReplyCmd(msg) != 0x106) {
        throw SerialCarException("Short term fuel trim bank 1 wrong resp");
    }
    int trim = (int) PayloadInt(msg);
    trim *= 100;
    trim /= 128;
    trim -= 100;
    return trim;
}

std::optional<int> SerialCarDevice::GetLongTermFuelTrimBank1() const {
    serialInterface->Write("0107\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 2000)) {
        throw SerialCarException("Long term fuel trim bank 1 no resp");
    }
    auto msg = DecodeHex(ln);
    if (ReplyCmd(msg) != 0x107) {
        throw SerialCarException("Long term fuel trim bank 1 wrong resp");
    }
    int trim = (int) PayloadInt(msg);
    trim *= 100;
    trim /= 128;
    trim -= 100;
    return trim;
}

std::optional<int> SerialCarDevice::GetShortTermFuelTrimBank2() const {
    serialInterface->Write("0108\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 2000)) {
        throw SerialCarException("Short term fuel trim bank 2 no resp");
    }
    auto msg = DecodeHex(ln);
    if (ReplyCmd(msg) != 0x108) {
        throw SerialCarException("Short term fuel trim bank 2 wrong resp");
    }
    int trim = (int) PayloadInt(msg);
    trim *= 100;
    trim /= 128;
    trim -= 100;
    return trim;
}

std::optional<int> SerialCarDevice::GetLongTermFuelTrimBank2() const {
    serialInterface->Write("0109\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 2000)) {
        throw SerialCarException("Long term fuel trim bank 2 no resp");
    }
    auto msg = DecodeHex(ln);
    if (ReplyCmd(msg) != 0x109) {
        throw SerialCarException("Long term fuel trim bank 2 wrong resp");
    }
    int trim = (int) PayloadInt(msg);
    trim *= 100;
    trim /= 128;
    trim -= 100;
    return trim;
}

std::optional<int> SerialCarDevice::GetFuelGaugePressure() const {
    serialInterface->Write("010A\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 2000)) {
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

std::optional<int> SerialCarDevice::GetIntakeManifoldAbsPressure() const {
    serialInterface->Write("010B\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 2000)) {
        throw SerialCarException("Intake manifold abs pressure no resp");
    }
    auto msg = DecodeHex(ln);
    if (ReplyCmd(msg) != 0x10B) {
        throw SerialCarException("Intake manifold abs pressure wrong resp");
    }
    int press = (int) PayloadInt(msg);
    return press;
}

std::optional<int> SerialCarDevice::GetRPM() const {
    serialInterface->Write("010C\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 2000)) {
        throw SerialCarException("RPM no resp");
    }
    auto msg = DecodeHex(ln);
    if (ReplyCmd(msg) != 0x10C) {
        throw SerialCarException("RPM wrong resp");
    }
    auto rawRpm = PayloadInt(msg);
    WaitForPrompt(buf, 2000);
    return (int) (rawRpm / 4);
}

std::optional<int> SerialCarDevice::GetSpeed() const {
    serialInterface->Write("010D\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 2000)) {
        throw SerialCarException("Speed no resp");
    }
    auto msg = DecodeHex(ln);
    if (ReplyCmd(msg) != 0x10D) {
        throw SerialCarException("Speed wrong resp");
    }
    int speed = (int) PayloadInt(msg);
    WaitForPrompt(buf, 2000);
    return speed;
}

std::optional<float> SerialCarDevice::GetTimingAdvance() const {
    serialInterface->Write("010E\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 2000)) {
        throw SerialCarException("Timing advance no resp");
    }
    auto msg = DecodeHex(ln);
    if (ReplyCmd(msg) != 0x10E) {
        throw SerialCarException("Timing advance wrong resp");
    }
    float adv = (float) PayloadInt(msg);
    adv /= 2;
    adv -= 64.0f;
    WaitForPrompt(buf, 2000);
    return adv;
}

std::optional<int> SerialCarDevice::GetIntakeAirTemperature() const {
    serialInterface->Write("010F\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 2000)) {
        throw SerialCarException("Intake air temp no resp");
    }
    auto msg = DecodeHex(ln);
    if (ReplyCmd(msg) != 0x10F) {
        throw SerialCarException("Intake air temp wrong resp");
    }
    int temp = (int) PayloadInt(msg);
    temp -= 40;
    WaitForPrompt(buf, 2000);
    return temp;
}

std::optional<float> SerialCarDevice::GetMassAirFlow() const {
    serialInterface->Write("0110\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 2000)) {
        throw SerialCarException("Mass air flow no resp");
    }
    auto msg = DecodeHex(ln);
    if (ReplyCmd(msg) != 0x110) {
        throw SerialCarException("Mass air flow wrong resp");
    }
    float massflow = (float) PayloadInt(msg);
    massflow /= 100.0f;
    WaitForPrompt(buf, 2000);
    return massflow;
}

std::optional<float> SerialCarDevice::GetThrottlePos() const {
    serialInterface->Write("0111\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 2000)) {
        throw SerialCarException("Throttle pos no resp");
    }
    auto msg = DecodeHex(ln);
    if (ReplyCmd(msg) != 0x111) {
        throw SerialCarException("Throttle pos wrong resp");
    }
    float thr = (float) PayloadInt(msg);
    thr *= 100.0f;
    thr /= 255.0f;
    WaitForPrompt(buf, 2000);
    return thr;
}

std::optional<O2Sensor> SerialCarDevice::GetO2Sensor(int n) const {
    {
        std::string q;
        switch (n) {
            case 0:
                q = "0114\r";
                break;
            case 1:
                q = "0115\r";
                break;
            case 2:
                q = "0116\r";
                break;
            case 3:
                q = "0117\r";
                break;
            case 4:
                q = "0118\r";
                break;
            case 5:
                q = "0119\r";
                break;
            case 6:
                q = "011A\r";
                break;
            case 7:
                q = "011B\r";
                break;
            default:
                return {{.Voltage = 0.0f, .ShortTermFuelTrim = 0}};
        }
        serialInterface->Write(q);
    }
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 2000)) {
        throw SerialCarException("O2 no resp");
    }
    auto msg = DecodeHex(ln);
    if (ReplyCmd(msg) != (0x114 + n)) {
        throw SerialCarException("O2 wrong resp");
    }
    uint16_t raw = (uint16_t) PayloadInt(msg);
    float voltage = (float) ((raw >> 8) & 0xFF);
    int fuelTrim = (int) (raw & 0xFF);
    voltage /= 200;
    fuelTrim *= 100;
    fuelTrim /= 128;
    fuelTrim -= 100;
    WaitForPrompt(buf, 2000);
    return {{.Voltage = voltage, .ShortTermFuelTrim = fuelTrim}};
}

std::optional<std::string> SerialCarDevice::GetVIN() const {
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

bool SerialCarDevice::ClearDTCEtc() {
    serialInterface->Write("04\r");
    std::string buf{};
    WaitForPrompt(buf , 7000);
    return true;
}

std::optional<std::vector<std::string>> SerialCarDevice::GetDTCs() const {
    serialInterface->Write("03\r");
    std::vector<std::string> lns{};
    {
        std::string buf{};
        auto data = WaitForPrompt(buf, 9000);
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
    std::vector<unsigned int> codes{};
    for (const auto ln : lns) {
        auto msg = DecodeHex(ln);
        if (ReplyMode(msg) == 3) {
            auto rawcodes = PayloadMode(msg);
            auto iterator = rawcodes.begin();
            while (iterator != rawcodes.end()) {
                unsigned int a = (unsigned int) *iterator;
                ++iterator;
                if (iterator != rawcodes.end()) {
                    unsigned int b = (unsigned int) *iterator;
                    ++iterator;
                    a = (a << 8) & 0xff00;
                    b = b & 0xff;
                    codes.emplace_back(a | b);
                }
            }
        }
    }
    std::vector<std::string> strcodes{};
    for (auto code : codes) {
        if (code == 0) {
            continue;
        }
        std::stringstream sstr{};
        auto category = (code >> 14) & 0x0003;
        switch (category) {
            case 0:
                sstr << "P";
                break;
            case 1:
                sstr << "C";
                break;
            case 2:
                sstr << "B";
                break;
            case 3:
                sstr << "U";
                break;
        }
        code = code & 0x3FFF;
        sstr << std::hex << code;
        strcodes.emplace_back(sstr.str());
    }
    return strcodes;
}

std::optional<std::vector<std::string>> SerialCarDevice::GetPendingDTCs() const {
    serialInterface->Write("07\r");
    std::vector<std::string> lns{};
    {
        std::string buf{};
        auto data = WaitForPrompt(buf, 9000);
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
    std::vector<unsigned int> codes{};
    for (const auto ln : lns) {
        auto msg = DecodeHex(ln);
        if (ReplyMode(msg) == 7) {
            auto rawcodes = PayloadMode(msg);
            auto iterator = rawcodes.begin();
            while (iterator != rawcodes.end()) {
                unsigned int a = (unsigned int) *iterator;
                ++iterator;
                if (iterator != rawcodes.end()) {
                    unsigned int b = (unsigned int) *iterator;
                    ++iterator;
                    a = (a << 8) & 0xff00;
                    b = b & 0xff;
                    codes.emplace_back(a | b);
                }
            }
        }
    }
    std::vector<std::string> strcodes{};
    for (auto code : codes) {
        if (code == 0) {
            continue;
        }
        std::stringstream sstr{};
        auto category = (code >> 14) & 0x0003;
        switch (category) {
            case 0:
                sstr << "P";
                break;
            case 1:
                sstr << "C";
                break;
            case 2:
                sstr << "B";
                break;
            case 3:
                sstr << "U";
                break;
        }
        code = code & 0x3FFF;
        sstr << std::hex << code;
        strcodes.emplace_back(sstr.str());
    }
    return strcodes;
}
