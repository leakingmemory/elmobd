//
// Created by sigsegv on 5/13/23.
//

#include <iostream>
#include <thread>
#include <chrono>
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

SerialCanDevice::SerialCanDevice(SerialInterface &&mv, const std::string &protocol) {
    serialInterface = std::make_unique<SerialInterface>(std::move(mv));
    {
        auto retry = 10;
        while (true) {
            try {
                Drain();
                Reset();
                break;
            } catch (...) {
                if (--retry == 0) {
                    throw;
                }
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(2s);
                serialInterface->CommitAttributes();
                std::this_thread::sleep_for(1s);
            }
        }
    }
    HeadersOn();
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
    if (protocol.empty()) {
        DetectProtocol();
    } else {
        SetProtocol(protocol);
    }
    std::cout << "Found PIDS_A " << std::hex << pidsA << std::dec << "\n";
    if (HasPIDS_B()) {
        PIDS_B();
        std::cout << "Found PIDS_B " << std::hex << pidsB << std::dec << "\n";
    }
    if (HasPIDS_C()) {
        PIDS_C();
        std::cout << "Found PIDS_C " << std::hex << pidsC << std::dec << "\n";
    }
    if (HasPIDS_D()) {
        PIDS_D();
        std::cout << "Found PIDS_D " << std::hex << pidsD << std::dec << "\n";
    }
    if (HasPIDS_E()) {
        PIDS_E();
        std::cout << "Found PIDS_E " << std::hex << pidsE << std::dec << "\n";
    }
    if (HasPIDS_F()) {
        PIDS_F();
        std::cout << "Found PIDS_F " << std::hex << pidsD << std::dec << "\n";
    }
    PID9s();
    std::cout << "Found PID9s " << std::hex << pid9s << std::dec << "\n";
}

void SerialCanDevice::SetProtocol(const std::string &protocol) {
    if (protocol.size() == 1) {
        if (!TrySetProtocol(protocol)) {
            throw SerialCanException("Unable to set protocol");
        }
        this->protocol = protocol;
    } else {
        for (auto p : protocols) {
            if (p.second == protocol) {
                if (!TrySetProtocol(p.first)) {
                    throw SerialCanException("Unable to set protocol");
                }
                this->protocol = p.first;
                return;
            }
        }
        throw SerialCanException("Unknown protocol");
    }
}

void SerialCanDevice::DetectProtocol() {
    if (AutoProtocol()) {
        return;
    }
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

constexpr int hexch(char ch) {
    if (ch >= 'a' && ch <= 'f') {
        return 10 + ch - 'a';
    }
    if (ch >= 'A' && ch <= 'F') {
        return 10 + ch - 'A';
    }
    if (ch >= '0' && ch <= '9') {
        return ch - '0';
    }
    return -1;
}

static_assert(hexch('0') == 0);
static_assert(hexch('1') == 1);
static_assert(hexch('2') == 2);
static_assert(hexch('3') == 3);
static_assert(hexch('4') == 4);
static_assert(hexch('5') == 5);
static_assert(hexch('6') == 6);
static_assert(hexch('7') == 7);
static_assert(hexch('8') == 8);
static_assert(hexch('9') == 9);
static_assert(hexch('a') == 10);
static_assert(hexch('b') == 11);
static_assert(hexch('c') == 12);
static_assert(hexch('d') == 13);
static_assert(hexch('e') == 14);
static_assert(hexch('f') == 15);
static_assert(hexch('A') == 10);
static_assert(hexch('B') == 11);
static_assert(hexch('C') == 12);
static_assert(hexch('D') == 13);
static_assert(hexch('E') == 14);
static_assert(hexch('F') == 15);
static_assert(hexch('G') == -1);

std::string SerialCanDevice::DecodeHex(const std::string &ihex) {
    std::string hex{ihex};
    {
        auto iterator = hex.begin();
        while (iterator != hex.end()) {
            if (*iterator == ' ') {
                hex.erase(iterator);
                continue;
            }
            ++iterator;
        }
    }
    std::string bytes{};
    {
        auto iterator = hex.begin();
        while (iterator != hex.end()) {
            auto hvu = hexch(*iterator);
            ++iterator;
            if (iterator == hex.end()) {
                throw SerialCanException("Hex bytes, count error, pair");
            }
            auto hvl = hexch(*iterator);
            if (hvu < 0 || hvl < 0) {
                throw SerialCanException("Hex bytes invalid");
            }
            ++iterator;
            char hv = (char) ((hvu << 4) + hvl);
            bytes.append((const char *) &hv, 1);
        }
    }
    return bytes;
}

uint32_t SerialCanDevice::ElmHeader(const std::string &msg) {
    if (msg.size() < 3) {
        return 0;
    }
    uint32_t h = (unsigned char) (msg.at(0));
    h = h << 8;
    h |= (unsigned char) (msg.at(1));
    h = h << 8;
    h |= (unsigned char) (msg.at(2));
    return h;
}

uint16_t SerialCanDevice::ReplyCmd(const std::string &msg) {
    if (msg.size() < 5) {
        return 0;
    }
    uint16_t h = (unsigned char) (msg.at(3));
    h = h - 0x40;
    h = h << 8;
    h |= (unsigned char) (msg.at(4));
    return h;
}

uint64_t SerialCanDevice::PayloadInt(const std::string &msg) {
    if (msg.size() < 7) {
        return 0;
    }
    auto n = msg.size() - 1;
    uint64_t res{0};
    for (int i = 5; i < n; i++) {
        res = res << 8;
        res |= (unsigned char) (msg.at(i));
    }
    return res;
}

std::string SerialCanDevice::Payload(const std::string &msg) {
    if (msg.size() < 7) {
        return "";
    }
    return msg.substr(5, msg.size() - 6);
}

void SerialCanDevice::Drain() {
    while (serialInterface->Read() != "") {
    }
}

bool SerialCanDevice::WaitForLine(std::string &buf, std::string &ln, int timeout_ms) const {
    auto timer = std::chrono::system_clock::now();
    {
        std::chrono::milliseconds duration{timeout_ms};
        timer += duration;
    }
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
        if (std::chrono::system_clock::now() > timer) {
            return false;
        }
        auto str = serialInterface->Read();
        buf.append(str);
    }
}

std::string SerialCanDevice::WaitForPrompt(std::string &buf, int timeout_ms) const {
    auto timer = std::chrono::system_clock::now();
    {
        std::chrono::milliseconds duration{timeout_ms};
        timer += duration;
    }
    std::string msg{};
    while (true) {
        if (buf.ends_with("\r>")) {
            auto m = buf.substr(0, buf.size() - 2);
            buf = "";
            msg.append(m);
            return msg;
        } else {
            msg.append(buf);
            buf = "";
        }
        if (std::chrono::system_clock::now() > timer) {
            buf = msg;
            return "";
        }
        auto str = serialInterface->Read();
        buf.append(str);
    }
}

std::vector<std::string> SerialCanDevice::SplitLines(const std::string &str) {
    std::vector<std::string> lns{};
    std::remove_const<typeof(std::string::npos)>::type base = 0;
    while (true) {
        typeof(base) split = str.find('\r', base);
        if (split == std::string::npos) {
            break;
        }
        {
            auto token = str.substr(base, split - base);
            base = split + 1;
            lns.emplace_back(token);
        }
        if (base >= str.size()) {
            return lns;
        }
    }
    {
        auto token = str.substr(base);
        lns.emplace_back(token);
    }
    return lns;
}

void SerialCanDevice::Reset() {
    Drain();
    serialInterface->Write("ATZ\r");
    std::string buf{};
    WaitForLine(buf, deviceId, 5000);
    if (!deviceId.starts_with("ELM")) {
        WaitForLine(buf, deviceId, 2000);
    }
    WaitForPrompt(buf, 2000);
    Drain();
    buf = "";
    serialInterface->Write("ATE0\r");
    WaitForPrompt(buf, 2000);
    Drain();
    buf = "";
    serialInterface->Write("ATL0\r");
    WaitForPrompt(buf, 2000);
    Drain();
    buf = "";
    EchoOff();
    LinefeedsOff();
}

void SerialCanDevice::EchoOff() {
    serialInterface->Write("ATE0\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 1000)) {
        throw SerialCanException("Echo off no resp");
    }
    if (ln == "OK") {
        WaitForPrompt(buf, 1000);
        return;
    }
    if (ln != "ATE0") {
        throw SerialCanException("Echo off error");
    }
    if (!WaitForLine(buf, ln, 1000)) {
        throw SerialCanException("Echo off no resp");
    }
    if (ln != "OK") {
        throw SerialCanException("Echo off error");
    }
    WaitForPrompt(buf, 1000);
}

void SerialCanDevice::SimpleCommand(const std::string &cmd) {
    {
        std::string str{cmd};
        str.append("\r");
        serialInterface->Write(str);
    }
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 200)) {
        throw SerialCanException("Cmd no resp");
    }
    if (ln != "OK") {
        throw SerialCanException("Cmd error");
    }
    WaitForPrompt(buf, 1000);
}

float SerialCanDevice::GetVoltage() {
    serialInterface->Write("AT RV\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 100)) {
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
    WaitForPrompt(buf, 1000);
    return volt;
}

bool SerialCanDevice::PIDS_A() {
    serialInterface->Write("0100\r");
    std::string buf{};
    std::string data = WaitForPrompt(buf, 7000);
    if (data.empty()) {
        data = WaitForPrompt(buf, 14000);
    }
    while (data.ends_with("\r")) {
        data = data.substr(0, data.size() - 1);
    }
    auto cutpos = data.find_last_of('\r');
    if (cutpos != std::string::npos) {
        data = data.substr(cutpos + 1);
    }
    if (data.empty()) {
        return false;
    }
    try {
        auto bindata = DecodeHex(data);
        if (ReplyCmd(bindata) != 0x100) {
            return false;
        }
        pidsA = PayloadInt(bindata);
        if (pidsA != 0) {
            return true;
        } else {
            return false;
        }
    } catch (...) {
        return false;
    }
}

void SerialCanDevice::PIDS_B() {
    serialInterface->Write("0120\r");
    std::string buf{};
    std::string data = WaitForPrompt(buf, 2000);
    while (data.ends_with("\r")) {
        data = data.substr(0, data.size() - 1);
    }
    auto cutpos = data.find_last_of('\r');
    if (cutpos != std::string::npos) {
        data = data.substr(cutpos + 1);
    }
    if (data.empty()) {
        throw SerialCanException("PIDS_B no resp");
    }
    auto bindata = DecodeHex(data);
    if (ReplyCmd(bindata) != 0x120) {
        throw SerialCanException("PIDS_B wrong resp");
    }
    pidsB = PayloadInt(bindata);
}

void SerialCanDevice::PIDS_C() {
    serialInterface->Write("0140\r");
    std::string buf{};
    std::string data = WaitForPrompt(buf, 2000);
    while (data.ends_with("\r")) {
        data = data.substr(0, data.size() - 1);
    }
    auto cutpos = data.find_last_of('\r');
    if (cutpos != std::string::npos) {
        data = data.substr(cutpos + 1);
    }
    if (data.empty()) {
        throw SerialCanException("PIDS_C no resp");
    }
    auto bindata = DecodeHex(data);
    if (ReplyCmd(bindata) != 0x140) {
        throw SerialCanException("PIDS_C wrong resp");
    }
    pidsC = PayloadInt(bindata);
}

void SerialCanDevice::PIDS_D() {
    serialInterface->Write("0160\r");
    std::string buf{};
    std::string data = WaitForPrompt(buf, 2000);
    while (data.ends_with("\r")) {
        data = data.substr(0, data.size() - 1);
    }
    auto cutpos = data.find_last_of('\r');
    if (cutpos != std::string::npos) {
        data = data.substr(cutpos + 1);
    }
    if (data.empty()) {
        throw SerialCanException("PIDS_D no resp");
    }
    auto bindata = DecodeHex(data);
    if (ReplyCmd(bindata) != 0x160) {
        throw SerialCanException("PIDS_D wrong resp");
    }
    pidsD = PayloadInt(bindata);
}

void SerialCanDevice::PIDS_E() {
    serialInterface->Write("0180\r");
    std::string buf{};
    std::string data = WaitForPrompt(buf, 2000);
    while (data.ends_with("\r")) {
        data = data.substr(0, data.size() - 1);
    }
    auto cutpos = data.find_last_of('\r');
    if (cutpos != std::string::npos) {
        data = data.substr(cutpos + 1);
    }
    if (data.empty()) {
        throw SerialCanException("PIDS_E no resp");
    }
    auto bindata = DecodeHex(data);
    if (ReplyCmd(bindata) != 0x180) {
        throw SerialCanException("PIDS_E wrong resp");
    }
    pidsE = PayloadInt(bindata);
}

void SerialCanDevice::PIDS_F() {
    serialInterface->Write("01A0\r");
    std::string buf{};
    std::string data = WaitForPrompt(buf, 2000);
    while (data.ends_with("\r")) {
        data = data.substr(0, data.size() - 1);
    }
    auto cutpos = data.find_last_of('\r');
    if (cutpos != std::string::npos) {
        data = data.substr(cutpos + 1);
    }
    if (data.empty()) {
        throw SerialCanException("PIDS_F no resp");
    }
    auto bindata = DecodeHex(data);
    if (ReplyCmd(bindata) != 0x1A0) {
        throw SerialCanException("PIDS_F wrong resp");
    }
    pidsF = PayloadInt(bindata);
}

void SerialCanDevice::PID9s() {
    serialInterface->Write("0900\r");
    std::string buf{};
    std::string data = WaitForPrompt(buf, 2000);
    while (data.ends_with("\r")) {
        data = data.substr(0, data.size() - 1);
    }
    auto cutpos = data.find_last_of('\r');
    if (cutpos != std::string::npos) {
        data = data.substr(cutpos + 1);
    }
    if (data.empty()) {
        throw SerialCanException("PID9s no resp");
    }
    auto bindata = DecodeHex(data);
    if (ReplyCmd(bindata) != 0x900) {
        throw SerialCanException("PID9s wrong resp");
    }
    pid9s = PayloadInt(bindata);
}

bool SerialCanDevice::HasPIDS_B() {
    return pidsA & 1;
}

bool SerialCanDevice::HasPIDS_C() {
    return pidsB & 1;
}

bool SerialCanDevice::HasPIDS_D() {
    return pidsC & 1;
}

bool SerialCanDevice::HasPIDS_E() {
    return pidsD & 1;
}

bool SerialCanDevice::HasPIDS_F() {
    return pidsE & 1;
}

bool SerialCanDevice::AutoProtocol() {
    if (GetVoltage() < 6) {
        throw SerialCanException("Not connected to OBD socket");
    }
    SimpleCommand("ATSP0");
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    if (!PIDS_A()) {
        std::cerr << "Unable to connect with auto protocol\n";
        return false;
    }
    serialInterface->Write("ATDPN");
    {
        std::string buf{};
        std::string ln{};
        if (!WaitForLine(buf, ln, 2000)) {
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
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    if (!PIDS_A()) {
        std::cerr << "Unable to connect using proto " << proto << "\n";
        return false;
    }
    std::cout << "Connected using proto " << proto << "\n";
    return true;
}
