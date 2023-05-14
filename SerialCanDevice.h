//
// Created by sigsegv on 5/13/23.
//

#ifndef ELMOBD_SERIALCANDEVICE_H
#define ELMOBD_SERIALCANDEVICE_H

#include <memory>
#include <map>
#include <string>

class SerialInterface;

class SerialCanDevice {
protected:
    std::unique_ptr<SerialInterface> serialInterface;
    std::string deviceId{};
    std::map<std::string,std::string> protocols{};
    std::string protocol{};
public:
    SerialCanDevice(SerialInterface &&mv);
    ~SerialCanDevice();
private:
    void Drain();
protected:
    bool WaitForLine(std::string &buf, std::string &ln);
    std::string WaitForPrompt(std::string &buf);
private:
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
    bool AutoProtocol();
    bool TrySetProtocol(const std::string &proto);
    std::string GetDeviceId() {
        return deviceId;
    }
};

#endif //ELMOBD_SERIALCANDEVICE_H
