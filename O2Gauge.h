//
// Created by sigsegv on 5/18/23.
//

#ifndef ELMOBD_O2GAUGE_H
#define ELMOBD_O2GAUGE_H

#include "Widget.h"
#include "Meter.h"
#include "O2VoltageGauge.h"
#include "O2FuelTrimGauge.h"

class CarDatasource;

class O2Gauge : public Widget, public Meter {
private:
    std::shared_ptr<CarDatasource> carDatasource;
    O2VoltageGauge voltageGauge;
    O2FuelTrimGauge fuelTrimGauge;
    int sensor;
public:
    O2Gauge(const std::shared_ptr<CarDatasource> &carDatasource, int sensor);
    void Init(std::shared_ptr<X11Window> window) override;
    bool HasPendingMutation() override;
    void Mutate() override;
    void DrawBackground(int x, int y, int width, int height) override;
    void BlankForeground(int x, int y, int width, int height) override;
    void DrawForeground(int x, int y, int width, int height) override;
    void Update() override;
    PriorityCategory GetPriorityCategory() const override;
    void Clicked();
};


#endif //ELMOBD_O2GAUGE_H
