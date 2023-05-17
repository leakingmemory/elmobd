//
// Created by sigsegv on 5/16/23.
//

#include "SerialInterface.h"
#include "SerialCarDevice.h"
#include "ElmObdDisplay.h"
#include <memory>

int main() {
    std::string protocol{};
    {
        const char *envproto = getenv("PROTOCOL");
        if (envproto != nullptr) {
            protocol = envproto;
        }
    }
    SerialInterface serialInterface{"/dev/ttyUSB0"};
    serialInterface.SetSpeed(SerialSpeed::S38400);
    serialInterface.CommitAttributes();
    std::shared_ptr<SerialCarDevice> serialCarDevice = std::make_shared<SerialCarDevice>(std::move(serialInterface), protocol);

    ElmObdDisplay elmObdDisplay{serialCarDevice};
    elmObdDisplay.Run();
}