//
// Created by sigsegv on 5/13/23.
//

#include "SerialInterface.h"
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

SerialInterface::SerialInterface(const char *dev) : fd(open(dev, O_RDWR | O_NOCTTY | O_SYNC)) {
    if (fd < 0) {
        throw std::exception();
    }
    SetSpeed(SerialSpeed::S9600);
    SetDefaults();
}

SerialInterface::~SerialInterface() {
    if (fd >= 0) {
        std::cout << "Closing serial connection (d)\n";
        close(fd);
        fd = -1;
    }
}

SerialInterface &SerialInterface::operator=(SerialInterface &&mv) {
    if (&mv != this) {
        if (this->fd >= 0) {
            std::cout << "Closing serial connection (mv)\n";
            close(this->fd);
        }
        this->fd = mv.fd;
        mv.fd = -1;
    }
    return *this;
}

void SerialInterface::Close() {
    if (fd >= 0) {
        std::cout << "Closing serial connection\n";
        close(fd);
        fd = -1;
    }
}

typeof(B230400) GetSpeed(SerialSpeed speed) {
    switch (speed) {
        case SerialSpeed::S1200:
            return B1200;
        case SerialSpeed::S2400:
            return B2400;
        case SerialSpeed::S4800:
            return B4800;
        case SerialSpeed::S9600:
            return B9600;
        case SerialSpeed::S19200:
            return B19200;
        case SerialSpeed::S38400:
            return B38400;
        case SerialSpeed::S57600:
            return B57600;
        case SerialSpeed::S115200:
            return B115200;
        case SerialSpeed::S230400:
            return B230400;
    }
    return B0;
}

void SerialInterface::SetInputSpeed(SerialSpeed speed) {
    auto tspeed = GetSpeed(speed);
    cfsetispeed(&tty, tspeed);
}

void SerialInterface::SetOutputSpeed(SerialSpeed speed) {
    auto tspeed = GetSpeed(speed);
    cfsetospeed(&tty, tspeed);
}

void SerialInterface::SetParity(int parity) {
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag |= (parity & (PARENB | PARODD));
}

void SerialInterface::SetDefaults() {
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK;         // disable break processing
    tty.c_lflag = 0;                // no signaling chars, no echo,
    // no canonical processing
    tty.c_oflag = 0;                // no remapping, no delays
    tty.c_cc[VMIN]  = 0;            // read doesn't block
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
    // enable reading
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;
}

void SerialInterface::CommitAttributes() {
    if (tcsetattr (fd, TCSANOW, &tty) != 0)
    {
        throw std::exception();
    }
}

std::string SerialInterface::Read() {
    char buf[16];
    int n = read(fd, buf, sizeof(buf));
    std::string str{};
    if (n < 0) {
        throw std::exception();
    }
    if (n != 0) {
        str.append(buf, n);
    }
    return str;
}

void SerialInterface::Write(const std::string &str) {
    write(fd, str.c_str(), str.size());
}
