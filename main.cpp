#include <iostream>
#include "SerialInterface.h"
#include "SerialCarDevice.h"

int main() {
    SerialInterface serialInterface{"/dev/ttyUSB0"};
    serialInterface.SetSpeed(SerialSpeed::S38400);
    serialInterface.CommitAttributes();
    SerialCarDevice serialCarDevice{std::move(serialInterface)};
    std::cout << serialCarDevice.GetDeviceId() << "\n";
    std::cout << "Car voltage: " << serialCarDevice.GetVoltage() << "V\n";
    return 0;
}
