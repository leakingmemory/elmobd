//
// Created by sigsegv on 6/27/23.
//
#include "SerialInterface.h"
#include "SerialCarDevice.h"
#include <iostream>

int main() {
    SerialInterface serialInterface{"/dev/ttyUSB0"};
    serialInterface.SetSpeed(SerialSpeed::S38400);
    serialInterface.CommitAttributes();
    SerialCarDevice serialCarDevice{std::move(serialInterface)};
    {
        auto dtcs = serialCarDevice.GetDTCs();
        std::cout << "Stored DTCs:\n";
        for (auto dtc: dtcs) {
            std::cout << dtc << "\n";
        }
    }
    {
        auto dtcs = serialCarDevice.GetPendingDTCs();
        std::cout << "Pending DTCs:\n";
        for (auto dtc: dtcs) {
            std::cout << dtc << "\n";
        }
    }
}