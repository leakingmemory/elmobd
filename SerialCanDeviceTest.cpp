//
// Created by sigsegv on 5/14/23.
//

#include "SerialCanDevice.h"

void assert(bool tv) {
    if (!tv) {
        throw std::exception();
    }
}

int main() {
    std::string msg = SerialCanDevice::DecodeHex("48 6B 10 41 00 BF BF B9 94 2B");
    assert(SerialCanDevice::ElmHeader(msg) == 0x486B10);
    assert(SerialCanDevice::ReplyCmd(msg) == 0x100);
    assert(SerialCanDevice::PayloadInt(msg) == 0xBFBFB994);
}