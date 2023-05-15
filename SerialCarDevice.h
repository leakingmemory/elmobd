//
// Created by sigsegv on 5/13/23.
//

#ifndef ELMOBD_SERIALCARDEVICE_H
#define ELMOBD_SERIALCARDEVICE_H

#include "SerialCanDevice.h"

class SerialCarDevice : public SerialCanDevice {
public:
    SerialCarDevice(SerialInterface &&mv);
private:
public:
    bool HasRPM() const;
    int RPM();
    bool HasVIN() const;
    std::string VIN();
};


#endif //ELMOBD_SERIALCARDEVICE_H
