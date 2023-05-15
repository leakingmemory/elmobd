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
