//
// Created by sigsegv on 5/16/23.
//

#ifndef ELMOBD_ANALOGGAUGE_H
#define ELMOBD_ANALOGGAUGE_H

#include <mutex>
#include "Widget.h"

class X11GC;

class AnalogGauge : public Widget {
private:
    std::mutex currentValueMtx{};
    std::weak_ptr<X11Window> window;
    std::shared_ptr<X11GC> gc;
    std::shared_ptr<X11GC> blankGc;
protected:
    float max{100.0f};
    float min{0.0f};
    int markOffset{0};
    int mark{10};
    int showValueMark{20};
    int degMin{237};
    int degMax{303};
    float centerX{0.5f};
    float centerY{0.9f};
    float needle{0.9f};
    float markLength{0.05f};
    float captionX{0.5};
    float captionY{0.5};
    std::string caption{};
private:
    float currentValue{75.0f};
    float needleValue{1.0f};
public:
    void Init(std::shared_ptr<X11Window> window) override;
    void DrawNeedle(const std::shared_ptr<X11GC> &usingGc, float value, int x, int y, int width, int height);
    void SetCurrentValue(float value);
    bool HasPendingMutation() override;
    void Mutate() override;
    void DrawBackground(int x, int y, int width, int height) override;
    void BlankForeground(int x, int y, int width, int height) override;
    void DrawForeground(int x, int y, int width, int height) override;
    virtual void Clicked() override;
};


#endif //ELMOBD_ANALOGGAUGE_H
