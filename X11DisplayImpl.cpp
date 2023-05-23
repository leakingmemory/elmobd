//
// Created by sigsegv on 5/16/23.
//

#include "X11DisplayImpl.h"
#include <iostream>

X11DisplayImpl::X11DisplayImpl(const std::string &screenName) {
    display = XOpenDisplay(screenName.c_str());
    if (display == nullptr) {
        std::cerr << "Unable to open connection to X11: " << display << "\n";
        return;
    }
}

X11DisplayImpl::~X11DisplayImpl() noexcept {
    if (display != nullptr) {
        XCloseDisplay(display);
        display = nullptr;
    }
}

const X11DisplayImpl &X11DisplayImpl::Impl() const {
    return *this;
}

int X11DisplayImpl::GetDefaultScreen() const {
    return DefaultScreen(display);
}

int X11DisplayImpl::GetScreenWidth(int screenNum) const {
    return DisplayWidth(display, screenNum);
}

int X11DisplayImpl::GetScreenHeight(int screenNum) const {
    return DisplayHeight(display, screenNum);
}

void X11DisplayImpl::ResetScreenSaver() const {
    XResetScreenSaver(display);
}
