//
// Created by sigsegv on 5/16/23.
//

#include "X11Display.h"
#include "X11DisplayImpl.h"

const char *getDisplayName() {
    const char *d = getenv("DISPLAY");
    if (d == nullptr) {
        d = "";
    }
    return d;
}

X11Display::X11Display() {
}

std::shared_ptr<X11Display> X11Display::Create(std::string screenName) {
    return std::make_shared<X11DisplayImpl>(screenName);
}

std::shared_ptr<X11Display> X11Display::Create() {
    return Create(getDisplayName());
}
