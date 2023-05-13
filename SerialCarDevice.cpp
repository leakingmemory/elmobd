//
// Created by sigsegv on 5/13/23.
//

#include "SerialCarDevice.h"

class SerialCarException : public std::exception {
private:
    const char *error;
public:
    SerialCarException(const char *error) : error(error) {}
    const char * what() const noexcept override {
        return error;
    }
};

SerialCarDevice::SerialCarDevice(SerialInterface &&mv) : SerialCanDevice(std::move(mv)) {
}