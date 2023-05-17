//
// Created by sigsegv on 5/17/23.
//

#include "X11WindowImpl.h"
#include "X11DisplayImpl.h"

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
