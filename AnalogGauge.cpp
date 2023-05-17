//
// Created by sigsegv on 5/16/23.
//

#include "AnalogGauge.h"
#include "X11GCImpl.h"
#include "X11DisplayImpl.h"
#include "X11WindowImpl.h"
#include <math.h>
#include <X11/Xlib.h>

void AnalogGauge::Init(std::shared_ptr<X11Window> window) {
    this->window = std::weak_ptr<X11Window>(window);
    this->gc = X11GC::Create(window);
    this->blankGc = X11GC::Create(window);
    this->blankGc->SetForegroundBlack();
}

void AnalogGauge::SetCurrentValue(float value) {
    std::lock_guard lock{currentValueMtx};
    currentValue = value;
}

void AnalogGauge::Mutate() {
    std::lock_guard lock{currentValueMtx};
    needleValue = ((currentValue * 2.0f) + needleValue) / 3.0f;
}

void AnalogGauge::DrawNeedle(const std::shared_ptr<X11GC> &usingGc, float value, int x, int y, int width, int height) {
    std::shared_ptr<X11Window> windowSh = this->window.lock();
    float radMin = ((float) this->degMin) * M_PI / 180.0f;
    float radSpan = (((float) this->degMax) * M_PI / 180.0f) - radMin;
    float span = ((float) max) - ((float) min);
    float centerX = ((float) width) * this->centerX;
    float centerY = ((float) height) * this->centerY;
    float needle = ((float) height) * this->needle;
    float k = (value - ((float) min)) / span;
    float rad = (k * radSpan) + radMin;
    float x1 = cos(rad) * (needle - 1.0f);
    float x2 = cos(rad) * needle;
    float x3 = x1;
    float y1 = sin(rad) * (needle - 1.0f);
    float y2 = sin(rad) * needle;
    float y3 = y1;
    float x0_2 = centerX + x;
    float x0_1 = x0_2 - 1.0f;
    float x0_3 = x0_2 + 1.0f;
    float y0_2 = centerY + y;
    float y0_1 = y0_2;
    float y0_3 = y0_2;
    x1 += x0_1;
    x2 += x0_2;
    x3 += x0_3;
    y1 += y0_1;
    y2 += y0_2;
    y3 += y0_3;
    auto *display = windowSh->display->Impl().display;
    auto &window = windowSh->Impl().window;
    auto &gc = usingGc->Impl().gc;
    XDrawLine(display, window, gc, x0_1, y0_1, x1, y1);
    XDrawLine(display, window, gc, x0_2, y0_2, x2, y2);
    XDrawLine(display, window, gc, x0_3, y0_3, x3, y3);
}

void AnalogGauge::DrawBackground(int x, int y, int width, int height) {
    std::shared_ptr<X11Window> windowSh = this->window.lock();
    float radMin = ((float) this->degMin) * M_PI / 180.0f;
    float radSpan = (((float) this->degMax) * M_PI / 180.0f) - radMin;
    float span = ((float) max) - ((float) min);
    float centerX = ((float) width) * this->centerX;
    float centerY = ((float) height) * this->centerY;
    float needle = ((float) height) * this->needle;
    float markLength = ((float) height) * this->markLength;
    auto *display = windowSh->display->Impl().display;
    auto &window = windowSh->Impl().window;
    auto &gc = this->gc->Impl().gc;
    for (int i = markOffset; i <= max; i += mark) {
        float k = (((float) i) - ((float) min)) / span;
        float rad = (k * radSpan) + radMin;
        float x1 = cos(rad) * needle;
        float x2 = cos(rad) * (needle - markLength);
        float y1 = sin(rad) * needle;
        float y2 = sin(rad) * (needle - markLength);
        x1 += centerX + x;
        x2 += centerX + x;
        y1 += centerY + y;
        y2 += centerY + y;
        XDrawLine(display, window, gc, x1, y1, x2, y2);
    }
}

void AnalogGauge::BlankForeground(int x, int y, int width, int height) {
    DrawNeedle(blankGc, needleValue, x, y, width, height);
}

void AnalogGauge::DrawForeground(int x, int y, int width, int height) {
    DrawNeedle(gc, needleValue, x, y, width, height);
}