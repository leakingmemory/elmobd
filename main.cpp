#include <iostream>
#include <thread>
#include <chrono>
#include "SerialInterface.h"
#include "SerialCarDevice.h"

int main() {
    SerialInterface serialInterface{"/dev/ttyUSB0"};
    serialInterface.SetSpeed(SerialSpeed::S38400);
    serialInterface.CommitAttributes();
    SerialCarDevice serialCarDevice{std::move(serialInterface)};
    std::cout << serialCarDevice.GetDeviceId() << "\n";
    std::cout << "Car voltage: " << serialCarDevice.GetVoltage() << "V\n";
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(10s);
    }
    auto hasRPM = serialCarDevice.HasRPM();
    while (true) {
        auto rpm = hasRPM ? serialCarDevice.RPM() : -1;
        if (hasRPM) {
            std::cout << " RPM=" << rpm;
            std::cout << "\n";
        }
    }
    return 0;
}
