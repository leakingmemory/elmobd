//
// Created by sigsegv on 5/16/23.
//

#include "AnalogGauge.h"
#include "X11GCImpl.h"
#include "X11DisplayImpl.h"
#include "X11WindowImpl.h"
#include <math.h>
#include <X11/Xlib.h>
#include <sstream>

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

static float NeedleValue(float currentValue, float needleValue) {
    return ((currentValue * 2.0f) + needleValue) / 3.0f;
}

bool AnalogGauge::HasPendingMutation() {
    std::lock_guard lock{currentValueMtx};
    bool pending = abs(NeedleValue(currentValue, needleValue) - needleValue) > 0.001;
    return pending;
}

void AnalogGauge::Mutate() {
    std::lock_guard lock{currentValueMtx};
    needleValue = NeedleValue(currentValue, needleValue);
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
    auto *font = XQueryFont(display, XGContextFromGC(gc));
    for (int i = markOffset; i <= max; i += mark) {
        float k = (((float) i) - ((float) min)) / span;
        float rad = (k * radSpan) + radMin;
        float x1 = cos(rad) * needle;
        float x2 = cos(rad) * (needle - markLength);
        float x3 = cos(rad) * (needle - (markLength * 1.5f) - 10.0f);
        float y1 = sin(rad) * needle;
        float y2 = sin(rad) * (needle - markLength);
        float y3 = sin(rad) * (needle - (markLength * 1.5f) - 10.0f);
        x1 += centerX + x;
        x2 += centerX + x;
        x3 += centerX + x;
        y1 += centerY + y;
        y2 += centerY + y;
        y3 += centerY + y;
        XDrawLine(display, window, gc, x1, y1, x2, y2);
        if (((i - markOffset) % showValueMark) == 0) {
            std::string str{};
            {
                std::stringstream strs{};
                strs << i;
                str = strs.str();
            }
            auto strw = XTextWidth(font, str.c_str(), str.size());
            x3 -= strw / 2;
            XTextItem textItem{
                    .chars = (char *) str.c_str(),
                    .nchars = (int) str.size(),
                    .delta = 0,
                    .font = None
            };
            XDrawText(display, window, gc, (int) x3, (int) y3, &textItem, 1);
        }
    }
    if (!caption.empty()) {
        float tx = captionX * width;
        float ty = captionY * height;
        tx += x;
        ty += y;
        auto tw = XTextWidth(font, caption.c_str(), caption.size());
        tx -= tw / 2;
        XTextItem textItem{
            .chars = (char *) caption.c_str(),
            .nchars = (int) caption.size(),
            .delta = 0,
            .font = None
        };
        XDrawText(display, window, gc, tx, ty, &textItem, 1);
    }
}

void AnalogGauge::BlankForeground(int x, int y, int width, int height) {
    DrawNeedle(blankGc, needleValue, x, y, width, height);
}

void AnalogGauge::DrawForeground(int x, int y, int width, int height) {
    DrawNeedle(gc, needleValue, x, y, width, height);
}

void AnalogGauge::Clicked() {
}