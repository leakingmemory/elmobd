#include <iostream>
#include "SerialInterface.h"
#include "SerialCanDevice.h"

int main() {
    SerialInterface serialInterface{"/dev/ttyUSB0"};
    serialInterface.SetSpeed(SerialSpeed::S38400);
    serialInterface.CommitAttributes();
    SerialCanDevice serialCanDevice{std::move(serialInterface)};
    std::cout << serialCanDevice.GetDeviceId() << "\n";
    std::cout << "Car voltage: " << serialCanDevice.GetVoltage() << "V\n";
    return 0;
}
