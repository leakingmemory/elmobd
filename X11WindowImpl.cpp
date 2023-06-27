//
// Created by sigsegv on 5/17/23.
//

#include "X11WindowImpl.h"
#include "X11DisplayImpl.h"
#include "Widget.h"
#include <iostream>

X11WindowImpl::X11WindowImpl(const std::shared_ptr<X11Display> &display, int screen_num, int width, int height, int x, int y) : X11Window(display, screen_num) {
    auto *disp = display->Impl().display;
    window = XCreateSimpleWindow(disp, RootWindow(disp, screen_num), x, y, width, height, 0, WhitePixel(disp, screen_num), BlackPixel(disp, screen_num));
    XMapWindow(disp, window);
    XFlush(disp);
}

const X11WindowImpl &X11WindowImpl::Impl() const {
    return *this;
}

std::shared_ptr<X11WindowImpl> X11WindowImpl::ImplRef() {
    return shared_from_this();
}

void X11WindowImpl::EventsLoop() {
    XSelectInput(display->Impl().display, window, ButtonPressMask) ;

    while (true) {
        XEvent event;
        XNextEvent(display->Impl().display, &event);
        if (event.type == ButtonPress && event.xbutton.button == Button1) {
            int x = event.xbutton.x;
            int y = event.xbutton.y;
            for (auto widget : widgets) {
                if (widget.x <= x && x < (widget.x + widget.width) &&
                    widget.y <= y && y < (widget.y + widget.height)) {
                    widget.widget->Clicked();
                }
            }
        }
    }
}