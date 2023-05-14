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
    void PIDS_A();
public:
    void RPM();
};


#endif //ELMOBD_SERIALCARDEVICE_H
