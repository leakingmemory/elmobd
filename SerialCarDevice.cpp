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
    PIDS_A();
    RPM();
}

void SerialCarDevice::PIDS_A() {
    serialInterface->Write("0100\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln)) {
        throw SerialCarException("PIDS_A no resp");
    }
    std::cout << "PIDS_A: " << ln << "\n";
    WaitForPrompt(buf);
}

void SerialCarDevice::RPM() {
    serialInterface->Write("010C\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln)) {
        throw SerialCarException("RPM no resp");
    }
    std::cout << "RPM: " << ln << "\n";
    WaitForPrompt(buf);
}
