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
    if (AutoProtocol()) {
        return;
    }
    protocols.insert_or_assign("1", "SAE_J1850_PWM");
    protocols.insert_or_assign("2", "SAE_J1850_VPW");
    protocols.insert_or_assign("3", "ISO_9141_2");
    protocols.insert_or_assign("4", "ISO_14230_4_5baud");
    protocols.insert_or_assign("5", "ISO_14320_4_fast");
    protocols.insert_or_assign("6", "ISO_14320_4_11bit_500k");
    protocols.insert_or_assign("7", "ISO_14230_4_29bit_500k");
    protocols.insert_or_assign("8", "ISO_14230_4_11bit_250k");
    protocols.insert_or_assign("9", "ISO_14230_4_29bit_250k");
    protocols.insert_or_assign("A", "SAE_J1939");
    if (TrySetProtocol("6")) {
        protocol = "6";
        return;
    }
    if (TrySetProtocol("8")) {
        protocol = "8";
        return;
    }
    if (TrySetProtocol("1")) {
        protocol = "1";
        return;
    }
    if (TrySetProtocol("7")) {
        protocol = "7";
        return;
    }
    if (TrySetProtocol("9")) {
        protocol = "9";
        return;
    }
    if (TrySetProtocol("2")) {
        protocol = "2";
        return;
    }
    if (TrySetProtocol("3")) {
        protocol = "3";
        return;
    }
    if (TrySetProtocol("4")) {
        protocol = "4";
        return;
    }
    if (TrySetProtocol("5")) {
        protocol = "5";
        return;
    }
    if (TrySetProtocol("A")) {
        protocol = "A";
        return;
    }
    throw SerialCanException("None of the protocols");
}

SerialCanDevice::~SerialCanDevice() {
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

bool SerialCanDevice::AutoProtocol() {
    if (GetVoltage() < 6) {
        throw SerialCanException("Not connected to OBD socket");
    }
    SimpleCommand("ATSP0");
    serialInterface->Write("0100\r");
    {
        std::string buf{};
        std::string chatter = WaitForPrompt(buf);
        if (chatter.find("UNABLE TO CONNECT") != std::string::npos) {
            std::cerr << "Unable to connect with auto protocol\n";
            return false;
        }
    }
    serialInterface->Write("ATDPN");
    {
        std::string buf{};
        std::string ln{};
        if (!WaitForLine(buf, ln)) {
            throw SerialCanException("ATDPN no resp");
        }
        std::cout << "Auto protocol selected: " << ln << "\n";
        protocol = ln;
        return true;
    }
}

bool SerialCanDevice::TrySetProtocol(const std::string &proto) {
    if (GetVoltage() < 6) {
        throw SerialCanException("Not connected to OBD socket");
    }
    {
        std::string cmd{"ATTP"};
        cmd.append(proto);
        SimpleCommand(cmd);
    }
    serialInterface->Write("0100\r");
    {
        std::string buf{};
        std::string chatter = WaitForPrompt(buf);
        if (chatter.find("UNABLE TO CONNECT") != std::string::npos) {
            std::cerr << "Unable to connect using proto " << proto << "\n";
            return false;
        }
    }
    std::cout << "Connected using proto " << proto << "\n";
    return true;
}
