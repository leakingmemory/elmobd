//
// Created by sigsegv on 5/19/23.
//

#include "ResilientCarDatasource.h"
#include "SerialInterface.h"
#include "SerialCarDevice.h"
#include "WarningsData.h"
#include <iostream>
#include <thread>
#include <chrono>

ResilientConnectionData::ResilientConnectionData(const std::shared_ptr<WarningsData> &warningsData) : warningsData(warningsData), warningsList(warningsData->Create()) {
}

ResilientConnectionData::~ResilientConnectionData() {
    warningsData->Remove(warningsList);
}

ResilientCarDatasource::ResilientCarDatasource(const std::shared_ptr<WarningsData> &warningsData) : mtx(std::make_unique<std::mutex>()), c(std::make_unique<ResilientConnectionData>(warningsData)) {
}

void ResilientCarDatasource::Connect() const {
    {
        std::lock_guard lock{*mtx};
        if (c->connecting) {
            return;
        }
        c->connecting = true;
        auto prevDatasource = c->carDatasource;
        c->carDatasource = {};
        if (prevDatasource) {
            prevDatasource->Disconnect();
        }
    }
    {
        std::vector<std::string> msg{};
        msg.emplace_back("ECU CONNECTION LOST");
        std::lock_guard lock{c->warningsList->mtx};
        c->warningsList->messages = msg;
    }
    std::shared_ptr<const ResilientCarDatasource> self = shared_from_this();
    std::thread connect{[self] () {
        {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(2s);
        }
        bool connected{false};
        try {
            auto connection = self->DoConnect();
            std::lock_guard lock{*(self->mtx)};
            self->c->carDatasource = connection;
            self->c->capsCarDatasource = connection;
            connected = true;
        } catch (...) {
            std::cerr << "Failed to connect\n";
        }
        if (connected) {
            std::vector<std::string> msg{};
            std::lock_guard lock{self->c->warningsList->mtx};
            self->c->warningsList->messages = msg;
        }
        std::lock_guard lock{*(self->mtx)};
        self->c->connecting = false;
    }};
    connect.detach();
}

void ResilientCarDatasource::Init() {
    bool connected{false};
    while (!connected) {
        try {
            c->carDatasource = DoConnect();
            connected = true;
        } catch (...) {
            std::cout << "Retrying...\n";
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1s);
        }
    }
    c->capsCarDatasource = c->carDatasource;
}

void ResilientCarDatasource::Disconnect() {
    throw std::exception();
}

bool ResilientCarDatasource::HasStatus() const {
    std::lock_guard lock{*mtx};
    return c->capsCarDatasource->HasStatus();
}
bool ResilientCarDatasource::HasFuelSystemStatus() const {
    std::lock_guard lock{*mtx};
    return c->capsCarDatasource->HasFuelSystemStatus();
}
bool ResilientCarDatasource::HasCalculatedLoad() const {
    std::lock_guard lock{*mtx};
    return c->capsCarDatasource->HasCalculatedLoad();
}
bool ResilientCarDatasource::HasCoolantTemperature() const {
    std::lock_guard lock{*mtx};
    return c->capsCarDatasource->HasCoolantTemperature();
}
bool ResilientCarDatasource::HasShortTermFuelTrimBank1() const {
    std::lock_guard lock{*mtx};
    return c->capsCarDatasource->HasShortTermFuelTrimBank1();
}
bool ResilientCarDatasource::HasLongTermFuelTrimBank1() const {
    std::lock_guard lock{*mtx};
    return c->capsCarDatasource->HasLongTermFuelTrimBank1();
}
bool ResilientCarDatasource::HasShortTermFuelTrimBank2() const {
    std::lock_guard lock{*mtx};
    return c->capsCarDatasource->HasShortTermFuelTrimBank2();
}
bool ResilientCarDatasource::HasLongTermFuelTrimBank2() const {
    std::lock_guard lock{*mtx};
    return c->capsCarDatasource->HasLongTermFuelTrimBank2();
}
bool ResilientCarDatasource::HasFuelGaugePressure() const {
    std::lock_guard lock{*mtx};
    return c->capsCarDatasource->HasFuelGaugePressure();
}
bool ResilientCarDatasource::HasIntakeManifoldAbsPressure() const {
    std::lock_guard lock{*mtx};
    return c->capsCarDatasource->HasIntakeManifoldAbsPressure();
}
bool ResilientCarDatasource::HasRPM() const {
    std::lock_guard lock{*mtx};
    return c->capsCarDatasource->HasRPM();
}
bool ResilientCarDatasource::HasSpeed() const {
    std::lock_guard lock{*mtx};
    return c->capsCarDatasource->HasSpeed();
}
bool ResilientCarDatasource::HasTimingAdvance() const {
    std::lock_guard lock{*mtx};
    return c->capsCarDatasource->HasTimingAdvance();
}
bool ResilientCarDatasource::HasIntakeAirTemperature() const {
    std::lock_guard lock{*mtx};
    return c->capsCarDatasource->HasIntakeAirTemperature();
}
bool ResilientCarDatasource::HasMassAirFlow() const {
    std::lock_guard lock{*mtx};
    return c->capsCarDatasource->HasMassAirFlow();
}
bool ResilientCarDatasource::HasThrottlePos() const {
    std::lock_guard lock{*mtx};
    return c->capsCarDatasource->HasThrottlePos();
}
bool ResilientCarDatasource::HasO2Sensor(int n) const {
    std::lock_guard lock{*mtx};
    return c->capsCarDatasource->HasO2Sensor(n);
}
OBDStatus ResilientCarDatasource::GetStatus() const {
    std::shared_ptr<CarDatasource> conn;
    bool connecting;
    {
        std::lock_guard lock{*mtx};
        conn = c->carDatasource;
        connecting = c->connecting;
    }
    if (conn) {
        try {
            return conn->GetStatus();
        } catch (...) {
            Connect();
        }
    } else if (!connecting) {
        Connect();
    }
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    return {};
}
FuelSystemStatus ResilientCarDatasource::GetFuelSystemStatus() const {
    std::shared_ptr<CarDatasource> conn;
    bool connecting;
    {
        std::lock_guard lock{*mtx};
        conn = c->carDatasource;
        connecting = c->connecting;
    }
    if (conn) {
        try {
            return conn->GetFuelSystemStatus();
        } catch (...) {
            Connect();
        }
    } else if (!connecting) {
        Connect();
    }
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    return {};
}
int ResilientCarDatasource::GetCalculatedLoad() const {
    std::shared_ptr<CarDatasource> conn;
    bool connecting;
    {
        std::lock_guard lock{*mtx};
        conn = c->carDatasource;
        connecting = c->connecting;
    }
    if (conn) {
        try {
            return conn->GetCalculatedLoad();
        } catch (...) {
            Connect();
        }
    } else if (!connecting) {
        Connect();
    }
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    return -1;
}
int ResilientCarDatasource::GetCoolantTemperature() const {
    std::shared_ptr<CarDatasource> conn;
    bool connecting;
    {
        std::lock_guard lock{*mtx};
        conn = c->carDatasource;
        connecting = c->connecting;
    }
    if (conn) {
        try {
            return conn->GetCoolantTemperature();
        } catch (...) {
            Connect();
        }
    } else if (!connecting) {
        Connect();
    }
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    return -1;
}
int ResilientCarDatasource::GetShortTermFuelTrimBank1() const {
    std::shared_ptr<CarDatasource> conn;
    bool connecting;
    {
        std::lock_guard lock{*mtx};
        conn = c->carDatasource;
        connecting = c->connecting;
    }
    if (conn) {
        try {
            return conn->GetShortTermFuelTrimBank1();
        } catch (...) {
            Connect();
        }
    } else if (!connecting) {
        Connect();
    }
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    return 155;
}
int ResilientCarDatasource::GetLongTermFuelTrimBank1() const {
    std::shared_ptr<CarDatasource> conn;
    bool connecting;
    {
        std::lock_guard lock{*mtx};
        conn = c->carDatasource;
        connecting = c->connecting;
    }
    if (conn) {
        try {
            return conn->GetLongTermFuelTrimBank1();
        } catch (...) {
            Connect();
        }
    } else if (!connecting) {
        Connect();
    }
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    return 155;
}
int ResilientCarDatasource::GetShortTermFuelTrimBank2() const {
    std::shared_ptr<CarDatasource> conn;
    bool connecting;
    {
        std::lock_guard lock{*mtx};
        conn = c->carDatasource;
        connecting = c->connecting;
    }
    if (conn) {
        try {
            return conn->GetShortTermFuelTrimBank2();
        } catch (...) {
            Connect();
        }
    } else if (!connecting) {
        Connect();
    }
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    return 155;
}
int ResilientCarDatasource::GetLongTermFuelTrimBank2() const {
    std::shared_ptr<CarDatasource> conn;
    bool connecting;
    {
        std::lock_guard lock{*mtx};
        conn = c->carDatasource;
        connecting = c->connecting;
    }
    if (conn) {
        try {
            return conn->GetLongTermFuelTrimBank2();
        } catch (...) {
            Connect();
        }
    } else if (!connecting) {
        Connect();
    }
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    return 155;
}
int ResilientCarDatasource::GetFuelGaugePressure() const {
    std::shared_ptr<CarDatasource> conn;
    bool connecting;
    {
        std::lock_guard lock{*mtx};
        conn = c->carDatasource;
        connecting = c->connecting;
    }
    if (conn) {
        try {
            return conn->GetFuelGaugePressure();
        } catch (...) {
            Connect();
        }
    } else if (!connecting) {
        Connect();
    }
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    return -1;
}
int ResilientCarDatasource::GetIntakeManifoldAbsPressure() const {
    std::shared_ptr<CarDatasource> conn;
    bool connecting;
    {
        std::lock_guard lock{*mtx};
        conn = c->carDatasource;
        connecting = c->connecting;
    }
    if (conn) {
        try {
            return conn->GetIntakeManifoldAbsPressure();
        } catch (...) {
            Connect();
        }
    } else if (!connecting) {
        Connect();
    }
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    return -1;
}
int ResilientCarDatasource::GetRPM() const {
    std::shared_ptr<CarDatasource> conn;
    bool connecting;
    {
        std::lock_guard lock{*mtx};
        conn = c->carDatasource;
        connecting = c->connecting;
    }
    if (conn) {
        try {
            return conn->GetRPM();
        } catch (...) {
            Connect();
        }
    } else if (!connecting) {
        Connect();
    }
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    return -1;
}
int ResilientCarDatasource::GetSpeed() const {
    std::shared_ptr<CarDatasource> conn;
    bool connecting;
    {
        std::lock_guard lock{*mtx};
        conn = c->carDatasource;
        connecting = c->connecting;
    }
    if (conn) {
        try {
            return conn->GetSpeed();
        } catch (...) {
            Connect();
        }
    } else if (!connecting) {
        Connect();
    }
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    return -1;
}
float ResilientCarDatasource::GetTimingAdvance() const {
    std::shared_ptr<CarDatasource> conn;
    bool connecting;
    {
        std::lock_guard lock{*mtx};
        conn = c->carDatasource;
        connecting = c->connecting;
    }
    if (conn) {
        try {
            return conn->GetTimingAdvance();
        } catch (...) {
            Connect();
        }
    } else if (!connecting) {
        Connect();
    }
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    return -1;
}
int ResilientCarDatasource::GetIntakeAirTemperature() const {
    std::shared_ptr<CarDatasource> conn;
    bool connecting;
    {
        std::lock_guard lock{*mtx};
        conn = c->carDatasource;
        connecting = c->connecting;
    }
    if (conn) {
        try {
            return conn->GetIntakeAirTemperature();
        } catch (...) {
            Connect();
        }
    } else if (!connecting) {
        Connect();
    }
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    return -1;
}
float ResilientCarDatasource::GetMassAirFlow() const {
    std::shared_ptr<CarDatasource> conn;
    bool connecting;
    {
        std::lock_guard lock{*mtx};
        conn = c->carDatasource;
        connecting = c->connecting;
    }
    if (conn) {
        try {
            return conn->GetMassAirFlow();
        } catch (...) {
            Connect();
        }
    } else if (!connecting) {
        Connect();
    }
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    return -1;
}
float ResilientCarDatasource::GetThrottlePos() const {
    std::shared_ptr<CarDatasource> conn;
    bool connecting;
    {
        std::lock_guard lock{*mtx};
        conn = c->carDatasource;
        connecting = c->connecting;
    }
    if (conn) {
        try {
            return conn->GetThrottlePos();
        } catch (...) {
            Connect();
        }
    } else if (!connecting) {
        Connect();
    }
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    return -1;
}
O2Sensor ResilientCarDatasource::GetO2Sensor(int n) const {
    std::shared_ptr<CarDatasource> conn;
    bool connecting;
    {
        std::lock_guard lock{*mtx};
        conn = c->carDatasource;
        connecting = c->connecting;
    }
    if (conn) {
        try {
            return conn->GetO2Sensor(n);
        } catch (...) {
            Connect();
        }
    } else if (!connecting) {
        Connect();
    }
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    return {};
}
bool ResilientCarDatasource::HasVIN() const {
    std::lock_guard lock{*mtx};
    return c->capsCarDatasource->HasVIN();
}
std::string ResilientCarDatasource::GetVIN() const {
    std::shared_ptr<CarDatasource> conn;
    bool connecting;
    {
        std::lock_guard lock{*mtx};
        conn = c->carDatasource;
        connecting = c->connecting;
    }
    if (conn) {
        try {
            return conn->GetVIN();
        } catch (...) {
            Connect();
        }
    } else if (!connecting) {
        Connect();
    }
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    return "";
}

std::vector<std::string> ResilientCarDatasource::GetDTCs() const {
    std::shared_ptr<CarDatasource> conn;
    bool connecting;
    {
        std::lock_guard lock{*mtx};
        conn = c->carDatasource;
        connecting = c->connecting;
    }
    if (conn) {
        try {
            return conn->GetDTCs();
        } catch (...) {
            Connect();
        }
    } else if (!connecting) {
        Connect();
    }
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    return {};
}

std::vector<std::string> ResilientCarDatasource::GetPendingDTCs() const {
    std::shared_ptr<CarDatasource> conn;
    bool connecting;
    {
        std::lock_guard lock{*mtx};
        conn = c->carDatasource;
        connecting = c->connecting;
    }
    if (conn) {
        try {
            return conn->GetPendingDTCs();
        } catch (...) {
            Connect();
        }
    } else if (!connecting) {
        Connect();
    }
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    return {};
}

void ResilientCarDatasource::ClearDTCEtc() {
    std::shared_ptr<CarDatasource> conn;
    bool connecting;
    {
        std::lock_guard lock{*mtx};
        conn = c->carDatasource;
        connecting = c->connecting;
    }
    if (conn) {
        try {
            conn->ClearDTCEtc();
        } catch (...) {
            Connect();
        }
    } else if (!connecting) {
        Connect();
    }
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
}

ResilientOBDCarDatasource::ResilientOBDCarDatasource(const std::shared_ptr<WarningsData> &warningsData) : ResilientCarDatasource(warningsData) {
    {
        const char *envproto = getenv("PROTOCOL");
        if (envproto != nullptr) {
            protocol = envproto;
        }
    }
    Init();
}

std::shared_ptr<CarDatasource> ResilientOBDCarDatasource::DoConnect() const {
    SerialInterface serialInterface{"/dev/ttyUSB0"};
    serialInterface.SetSpeed(SerialSpeed::S38400);
    serialInterface.CommitAttributes();
    std::shared_ptr<SerialCarDevice> serialCarDevice = std::make_shared<SerialCarDevice>(std::move(serialInterface), protocol);
    return serialCarDevice;
}
