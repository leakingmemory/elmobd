//
// Created by sigsegv on 5/13/23.
//

#ifndef ELMOBD_SERIALCANDEVICE_H
#define ELMOBD_SERIALCANDEVICE_H

#include <memory>
#include <map>
#include <string>
#include <vector>

class SerialInterface;

class SerialCanDevice {
protected:
    std::unique_ptr<SerialInterface> serialInterface;
    std::string deviceId{};
    std::map<std::string,std::string> protocols{};
    std::string protocol{};
    uint32_t pidsA{0}; // 01 - 20
    uint32_t pidsB{0}; // 21 - 40
    uint32_t pidsC{0}; // 41 - 60
    uint32_t pidsD{0}; // 61 - 80
    uint32_t pidsE{0}; // 81 - A0
    uint32_t pidsF{0}; // A1 - C0
    uint32_t pid9s{0};
public:
    SerialCanDevice(SerialInterface &&mv);
    void DetectProtocol();
    ~SerialCanDevice();
    static std::string DecodeHex(const std::string &);
    static uint32_t ElmHeader(const std::string &msg);
    static uint16_t ReplyCmd(const std::string &msg);
    static uint64_t PayloadInt(const std::string &msg);
    static std::string Payload(const std::string &msg);
private:
    void Drain();
protected:
    bool WaitForLine(std::string &buf, std::string &ln, int timeout_ms);
    std::string WaitForPrompt(std::string &buf, int timeout_ms);
    static std::vector<std::string> SplitLines(const std::string &str);
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
    bool PIDS_A();
    void PIDS_B();
    void PIDS_C();
    void PIDS_D();
    void PIDS_E();
    void PIDS_F();
    void PID9s();
    bool HasPIDS_B();
    bool HasPIDS_C();
    bool HasPIDS_D();
    bool HasPIDS_E();
    bool HasPIDS_F();
    bool AutoProtocol();
    bool TrySetProtocol(const std::string &proto);
    std::string GetDeviceId() {
        return deviceId;
    }
};

#endif //ELMOBD_SERIALCANDEVICE_H
