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
    auto dtcs = serialCarDevice.GetDTCs();
    for (auto dtc : dtcs) {
        std::cout << dtc << "\n";
    }
}