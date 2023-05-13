//
// Created by sigsegv on 5/13/23.
//

#ifndef ELMOBD_SERIALCANDEVICE_H
#define ELMOBD_SERIALCANDEVICE_H

#include <memory>

class SerialInterface;

class SerialCanDevice {
private:
    std::unique_ptr<SerialInterface> serialInterface;
    std::string deviceId{};
public:
    SerialCanDevice(SerialInterface &&mv);
private:
    void Drain();
    bool WaitForLine(std::string &buf, std::string &ln);
    std::string WaitForPrompt(std::string &buf);
    void Reset();
    void EchoOff();
public:
    void SimpleCommand(const std::string &cmd);
private:
    void HeadersOn() {
        SimpleCommand("ATH1");
    }
    void LinefeedsOff() {
        SimpleCommand("ATL0");
    }
public:
    float GetVoltage();
    std::string GetDeviceId() {
        return deviceId;
    }
};

#endif //ELMOBD_SERIALCANDEVICE_H
