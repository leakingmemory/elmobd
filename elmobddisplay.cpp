//
// Created by sigsegv on 5/16/23.
//

#include "X11Display.h"
#include "X11Window.h"
#include "ManPressGauge.h"
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
        auto gauge = std::make_shared<ManPressGauge>(serialCarDevice);
        window->Add(gauge);
        meters.emplace_back(gauge);
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