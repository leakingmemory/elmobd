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
    if ((pidsA & 0x80000000)) {
        return true;
    } else {
        return false;
    }
}

bool SerialCarDevice::HasRPM() const {
    if ((pidsA & 0x100000) != 0) {
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

OBDStatus SerialCarDevice::Status() {
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

int SerialCarDevice::RPM() {
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

std::string SerialCarDevice::VIN() {
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
