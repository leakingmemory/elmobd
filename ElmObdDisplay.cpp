//
// Created by sigsegv on 5/18/23.
//

#include "ElmObdDisplay.h"
#include "X11Display.h"
#include "X11Window.h"
#include "Meter.h"
#include "CarDatasource.h"
#include "ManPressGauge.h"
#include "MassFlowGauge.h"
#include "IntakeAirTempGauge.h"
#include "Bank1ShortTermFuelTrimGauge.h"
#include "Bank2ShortTermFuelTrimGauge.h"
#include "Bank1LongTermFuelTrimGauge.h"
#include "Bank2LongTermFuelTrimGauge.h"
#include "O2Gauge.h"
#include "EngCoolantTempGauge.h"
#include "DtcMonitor.h"
#include "WarningsPanel.h"
#include <thread>
#include <chrono>
#include <iostream>
#include <X11/Xlib.h>

void ElmObdDisplay::Run() const {
    auto display = X11Display::Create();
    auto screen = display->GetDefaultScreen();
    auto width = display->GetScreenWidth(screen);
    auto height = display->GetScreenHeight(screen);
    auto window = X11Window::Create(display, screen, width, height, 0, 0);
    std::vector<std::shared_ptr<Meter>> meters{};
    {
        int maxX = 0;
        int x = 0;
        if (carDatasource->HasIntakeManifoldAbsPressure()) {
            auto gauge = std::make_shared<ManPressGauge>(carDatasource);
            window->Add(gauge, x, 0, 200, 200);
            x += 220;
            meters.emplace_back(gauge);
        }
        if (carDatasource->HasMassAirFlow()) {
            auto gauge = std::make_shared<MassFlowGauge>(carDatasource);
            window->Add(gauge, x, 0, 200, 200);
            x += 220;
            meters.emplace_back(gauge);
        }
        if (carDatasource->HasIntakeAirTemperature() ||
            carDatasource->HasShortTermFuelTrimBank1() ||
            carDatasource->HasShortTermFuelTrimBank2() ||
            carDatasource->HasLongTermFuelTrimBank1() ||
            carDatasource->HasLongTermFuelTrimBank2()) {
            if (carDatasource->HasIntakeAirTemperature()) {
                auto gauge = std::make_shared<IntakeAirTempGauge>(carDatasource, warningsData);
                window->Add(gauge, x, 0, 100, 100);
                meters.emplace_back(gauge);
            }
            if (carDatasource->HasShortTermFuelTrimBank1()) {
                auto gauge = std::make_shared<Bank1ShortTermFuelTrimGauge>(carDatasource);
                window->Add(gauge, x, 100, 50, 50);
                meters.emplace_back(gauge);
            }
            if (carDatasource->HasShortTermFuelTrimBank2()) {
                auto gauge = std::make_shared<Bank2ShortTermFuelTrimGauge>(carDatasource);
                window->Add(gauge, x + 55, 100, 50, 50);
                meters.emplace_back(gauge);
            }
            if (carDatasource->HasLongTermFuelTrimBank1()) {
                auto gauge = std::make_shared<Bank1LongTermFuelTrimGauge>(carDatasource, warningsData);
                window->Add(gauge, x, 150, 50, 50);
                meters.emplace_back(gauge);
            }
            if (carDatasource->HasLongTermFuelTrimBank2()) {
                auto gauge = std::make_shared<Bank2LongTermFuelTrimGauge>(carDatasource, warningsData);
                window->Add(gauge, x + 55, 150, 50, 50);
                meters.emplace_back(gauge);
            }
            x += 110;
        }
        if (x > maxX) {
            maxX = x;
        }
        x = 0;
        for (int sensor = 0; sensor < 8; sensor++) {
            if (carDatasource->HasO2Sensor(sensor)) {
                auto gauge = std::make_shared<O2Gauge>(carDatasource, sensor);
                window->Add(gauge, x, 200, 50, 100);
                meters.emplace_back(gauge);
                x += 55;
            }
        }
        if (carDatasource->HasCoolantTemperature()) {
            auto gauge = std::make_shared<EngCoolantTempGauge>(carDatasource, warningsData);
            window->Add(gauge, x, 200, 100, 100);
            meters.emplace_back(gauge);
            x += 110;
        }
        if (x < maxX) {
            x = maxX;
        }
        {
            auto gauge = std::make_shared<WarningsPanel>(warningsData);
            window->Add(gauge, x, 0, 200, 300);
            meters.emplace_back(gauge);
        }
    }
    {
        auto dtcMeter = std::make_shared<DtcMonitor>(carDatasource, warningsData);
        meters.emplace_back(dtcMeter);
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
                if ((++counter % 100) == 0) {
                    display->ResetScreenSaver();
                }
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