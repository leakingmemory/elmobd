//
// Created by sigsegv on 5/16/23.
//

#include "X11Display.h"
#include "X11Window.h"
#include "ManPressGauge.h"
#include "MassFlowGauge.h"
#include "IntakeAirTempGauge.h"
#include "Bank1ShortTermFuelTrimGauge.h"
#include "Bank2ShortTermFuelTrimGauge.h"
#include "Bank1LongTermFuelTrimGauge.h"
#include "Bank2LongTermFuelTrimGauge.h"
#include "SerialInterface.h"
#include "SerialCarDevice.h"
#include <memory>
#include <thread>
#include <chrono>
#include <iostream>

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

    auto display = X11Display::Create();
    auto screen = display->GetDefaultScreen();
    auto width = display->GetScreenWidth(screen);
    auto height = display->GetScreenHeight(screen);
    auto window = X11Window::Create(display, screen, width, height, 0, 0);
    std::vector<std::shared_ptr<Meter>> meters{};
    {
        int x = 0;
        if (serialCarDevice->HasIntakeManifoldAbsPressure()) {
            auto gauge = std::make_shared<ManPressGauge>(serialCarDevice);
            window->Add(gauge, x, 0, 200, 200);
            x += 200;
            meters.emplace_back(gauge);
        }
        if (serialCarDevice->HasMassAirFlow()) {
            auto gauge = std::make_shared<MassFlowGauge>(serialCarDevice);
            window->Add(gauge, x, 0, 200, 200);
            x += 200;
            meters.emplace_back(gauge);
        }
        if (serialCarDevice->HasIntakeAirTemperature() ||
            serialCarDevice->HasShortTermFuelTrimBank1() ||
            serialCarDevice->HasShortTermFuelTrimBank2() ||
            serialCarDevice->HasLongTermFuelTrimBank1() ||
            serialCarDevice->HasLongTermFuelTrimBank2()) {
            if (serialCarDevice->HasIntakeAirTemperature()) {
                auto gauge = std::make_shared<IntakeAirTempGauge>(serialCarDevice);
                window->Add(gauge, x, 0, 100, 100);
                meters.emplace_back(gauge);
            }
            if (serialCarDevice->HasShortTermFuelTrimBank1()) {
                auto gauge = std::make_shared<Bank1ShortTermFuelTrimGauge>(serialCarDevice);
                window->Add(gauge, x, 100, 50, 50);
                meters.emplace_back(gauge);
            }
            if (serialCarDevice->HasShortTermFuelTrimBank2()) {
                auto gauge = std::make_shared<Bank2ShortTermFuelTrimGauge>(serialCarDevice);
                window->Add(gauge, x + 50, 100, 50, 50);
                meters.emplace_back(gauge);
            }
            if (serialCarDevice->HasLongTermFuelTrimBank1()) {
                auto gauge = std::make_shared<Bank1LongTermFuelTrimGauge>(serialCarDevice);
                window->Add(gauge, x, 150, 50, 50);
                meters.emplace_back(gauge);
            }
            if (serialCarDevice->HasLongTermFuelTrimBank2()) {
                auto gauge = std::make_shared<Bank2LongTermFuelTrimGauge>(serialCarDevice);
                window->Add(gauge, x + 50, 150, 50, 50);
                meters.emplace_back(gauge);
            }
            x += 100;
        }
    }
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
    std::mutex terminateLock{};
    bool terminate{false};
    std::thread rendering{[&window, &terminateLock, &terminate] () {
        while (true) {
            window->RenderingCycle();
            std::this_thread::sleep_for(25ms);
            std::lock_guard lock{terminateLock};
            if (terminate) {
                break;
            }
        }
    }};

    try {
        std::vector<std::shared_ptr<Meter>> highpri{};
        std::vector<std::shared_ptr<Meter>> lowpri{};
        std::vector<std::shared_ptr<Meter>> vlowpri{};
        for (const auto &meter : meters) {
            auto pri = meter->GetPriorityCategory();
            switch (pri) {
                case PriorityCategory::HIGH:
                    highpri.emplace_back(meter);
                    break;
                case PriorityCategory::LOW:
                    lowpri.emplace_back(meter);
                    break;
                case PriorityCategory::VERY_LOW:
                    vlowpri.emplace_back(meter);
                    break;
            }
        }
        auto lowiter = lowpri.begin();
        auto vlowiter = vlowpri.begin();
        int counter = 0;
        while (true) {
            for (auto &meter : highpri) {
                meter->Update();
            }
            if (lowiter == lowpri.end()) {
                lowiter = lowpri.begin();
            }
            if (lowiter != lowpri.end()) {
                (*lowiter)->Update();
                ++lowiter;
            }
            if ((++counter % 10) == 0) {
                if (vlowiter == vlowpri.end()) {
                    vlowiter = vlowpri.begin();
                }
                if (vlowiter != vlowpri.end()) {
                    (*vlowiter)->Update();
                    ++vlowiter;
                }
            }
        }
    } catch (std::exception &e) {
        std::cerr << "std::exception: " << e.what();
    } catch (...) {
        std::cerr << "exception\n";
    }

    {
        std::lock_guard lock{terminateLock};
        terminate = true;
    }
    rendering.join();
}