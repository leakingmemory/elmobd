//
// Created by sigsegv on 5/17/23.
//

#ifndef ELMOBD_X11WINDOWIMPL_H
#define ELMOBD_X11WINDOWIMPL_H

#include "X11Window.h"
#include <X11/Xlib.h>
#include <memory>

class X11Display;
class X11GCImpl;
class AnalogGauge;
class WarningsPanel;

class X11WindowImpl : public X11Window, public std::enable_shared_from_this<X11WindowImpl> {
    friend X11GCImpl;
    friend AnalogGauge;
    friend WarningsPanel;
private:
    Window window;
public:
    X11WindowImpl(const std::shared_ptr<X11Display> &display, int screen_num, int width, int height, int x, int y);
    const X11WindowImpl &Impl() const override;
    std::shared_ptr<X11WindowImpl> ImplRef() override;
    void EventsLoop() override;
};


#endif //ELMOBD_X11WINDOWIMPL_H
