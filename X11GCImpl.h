//
// Created by sigsegv on 5/17/23.
//

#ifndef ELMOBD_X11GCIMPL_H
#define ELMOBD_X11GCIMPL_H

#include "X11GC.h"
#include <X11/Xlib.h>
#include <memory>

class X11Display;
class X11Window;
class AnalogGauge;

class X11GCImpl : public X11GC {
    friend AnalogGauge;
private:
    std::shared_ptr<X11Display> display;
    GC gc;
    int screen;
public:
    X11GCImpl(const std::shared_ptr<X11Window> &window);
    ~X11GCImpl() override;
    const X11GCImpl &Impl() const override;
    void SetForegroundBlack() override;
};

#endif //ELMOBD_X11GCIMPL_H
