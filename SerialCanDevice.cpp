//
// Created by sigsegv on 5/13/23.
//

#include <iostream>
#include "SerialCanDevice.h"
#include "SerialInterface.h"

class SerialCanException : public std::exception {
private:
    const char *error;
public:
    SerialCanException(const char *error) : error(error) {}
    const char * what() const noexcept override {
        return error;
    }
};

SerialCanDevice::SerialCanDevice(SerialInterface &&mv) {
    serialInterface = std::make_unique<SerialInterface>(std::move(mv));
    Drain();
    Reset();
    EchoOff();
    HeadersOn();
    LinefeedsOff();
}

void SerialCanDevice::Drain() {
    while (serialInterface->Read() != "") {
    }
}

bool SerialCanDevice::WaitForLine(std::string &buf, std::string &ln) {
    while (true) {
        auto pos = buf.find('\r');
        auto delim = '\r';
        if (pos != std::string::npos) {
            auto lfpos = buf.find('\n');
            if (lfpos != std::string::npos && lfpos < pos) {
                pos = lfpos;
                delim = '\n';
            }
        } else {
            pos = buf.find('\n');
            delim = '\n';
        }
        if (pos != std::string::npos) {
            ln = buf.substr(0, pos);
            auto remaining = buf.substr(pos + 1);
            if (pos < (buf.size() - 1)) {
                auto ch = remaining.at(0);
                if ((ch == '\n' || ch == '\r') && ch != delim) {
                    auto r = remaining.substr(1);
                    remaining = r;
                }
            }
            buf = remaining;
            return true;
        }
        auto str = serialInterface->Read();
        buf.append(str);
    }
}

std::string SerialCanDevice::WaitForPrompt(std::string &buf) {
    std::string msg{};
    while (true) {
        if (buf.ends_with(">")) {
            auto m = buf.substr(0, buf.size() - 1);
            buf = "";
            msg.append(m);
            return msg;
        } else {
            msg.append(buf);
            buf = "";
        }
        auto str = serialInterface->Read();
        buf.append(str);
    }
}

void SerialCanDevice::Reset() {
    serialInterface->Write("ATZ\r");
    std::string buf{};
    if (!WaitForLine(buf, deviceId)) {
        throw SerialCanException("Reset dev id response missing");
    }
    while (!deviceId.starts_with("ELM")) {
        if (!WaitForLine(buf, deviceId)) {
            throw SerialCanException("Reset dev id response missing");
        }
    }
    WaitForPrompt(buf);
    if (!buf.empty()) {
        throw SerialCanException("Reset dev babble");
    }
}

void SerialCanDevice::EchoOff() {
    serialInterface->Write("ATE0\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln)) {
        throw SerialCanException("Echo off no resp");
    }
    if (ln == "OK") {
        WaitForPrompt(buf);
        return;
    }
    if (ln != "ATE0") {
        throw SerialCanException("Echo off error");
    }
    if (!WaitForLine(buf, ln)) {
        throw SerialCanException("Echo off no resp");
    }
    if (ln != "OK") {
        throw SerialCanException("Echo off error");
    }
    WaitForPrompt(buf);
}

void SerialCanDevice::SimpleCommand(const std::string &cmd) {
    {
        std::string str{cmd};
        str.append("\r");
        serialInterface->Write(str);
    }
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln)) {
        throw SerialCanException("Cmd no resp");
    }
    if (ln != "OK") {
        throw SerialCanException("Cmd error");
    }
    WaitForPrompt(buf);
}

float SerialCanDevice::GetVoltage() {
    serialInterface->Write("AT RV\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln)) {
        throw SerialCanException("Voltage no resp (adapter)");
    }
    size_t parse;
    float volt = std::stof(ln, &parse);
    if (parse == 0) {
        throw SerialCanException("Voltage wrong resp (adapter)");
    }
    auto remain = ln.substr(parse);
    if (remain != "v" && remain != "V") {
        throw SerialCanException("Voltage wrong resp (adapter)");
    }
    WaitForPrompt(buf);
    return volt;
}
