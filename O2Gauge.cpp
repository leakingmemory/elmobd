//
// Created by sigsegv on 5/18/23.
//

#include "O2Gauge.h"
#include "CarDatasource.h"

O2Gauge::O2Gauge(const std::shared_ptr<CarDatasource> &carDatasource, int sensor) : carDatasource(carDatasource), voltageGauge(), fuelTrimGauge(), sensor(sensor) {
}

void O2Gauge::Init(std::shared_ptr<X11Window> window) {
    voltageGauge.Init(window);
    fuelTrimGauge.Init(window);
}

bool O2Gauge::HasPendingMutation() {
    return voltageGauge.HasPendingMutation() || fuelTrimGauge.HasPendingMutation();
}

void O2Gauge::Mutate() {
    voltageGauge.Mutate();
    fuelTrimGauge.Mutate();
}

void O2Gauge::DrawBackground(int x, int y, int width, int height) {
    auto halfheight = height / 2;
    voltageGauge.DrawBackground(x, y, width, halfheight);
    fuelTrimGauge.DrawBackground(x, y + halfheight, width, halfheight);
}

void O2Gauge::BlankForeground(int x, int y, int width, int height) {
    auto halfheight = height / 2;
    voltageGauge.BlankForeground(x, y, width, halfheight);
    fuelTrimGauge.BlankForeground(x, y + halfheight, width, halfheight);
}

void O2Gauge::DrawForeground(int x, int y, int width, int height) {
    auto halfheight = height / 2;
    voltageGauge.DrawForeground(x, y, width, halfheight);
    fuelTrimGauge.DrawForeground(x, y + halfheight, width, halfheight);
}

void O2Gauge::Update() {
    auto o2 = carDatasource->GetO2Sensor(sensor);
    voltageGauge.SetCurrentValue(o2.Voltage);
    fuelTrimGauge.SetCurrentValue((float) o2.ShortTermFuelTrim);
}

PriorityCategory O2Gauge::GetPriorityCategory() const {
    return PriorityCategory::LOW;
}
