//
// Created by sigsegv on 5/20/23.
//

#include "SerialInterface.h"
#include "SerialCarDevice.h"

int main() {
    SerialInterface serialInterface{"/dev/ttyUSB0"};
    serialInterface.SetSpeed(SerialSpeed::S38400);
    serialInterface.CommitAttributes();
    SerialCarDevice serialCarDevice{std::move(serialInterface)};
    serialCarDevice.ClearDTCEtc();
}