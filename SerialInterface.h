//
// Created by sigsegv on 5/13/23.
//

#ifndef ELMOBD_SERIALINTERFACE_H
#define ELMOBD_SERIALINTERFACE_H

#include <string>
#include <termios.h>

enum class SerialSpeed {
    S1200,
    S2400,
    S4800,
    S9600,
    S19200,
    S38400,
    S57600,
    S115200,
    S230400
};

class SerialInterface {
private:
    int fd;
    struct termios tty;
public:
    SerialInterface() : fd(-1) {}
    SerialInterface(const char *dev);
    SerialInterface(const std::string &dev) : SerialInterface(dev.c_str()) {}
    ~SerialInterface();
    SerialInterface(SerialInterface &&mv) : fd(mv.fd) {
        mv.fd = -1;
    }
    SerialInterface(const SerialInterface &cp) = delete;
    SerialInterface &operator =(SerialInterface &&mv);
    SerialInterface &operator =(const SerialInterface &cp) = delete;
    void Close();
    void SetOutputSpeed(SerialSpeed speed);
    void SetInputSpeed(SerialSpeed speed);
    void SetSpeed(SerialSpeed speed) {
        SetOutputSpeed(speed);
        SetInputSpeed(speed);
    }
    void SetParity(int parity);
    void SetDefaults();
    void CommitAttributes();

    std::string Read();
    void Write(const std::string &str);
};


#endif //ELMOBD_SERIALINTERFACE_H
