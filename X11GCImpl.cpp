//
// Created by sigsegv on 5/17/23.
//

#include "X11GCImpl.h"
#include "X11DisplayImpl.h"
#include "X11WindowImpl.h"

X11GCImpl::X11GCImpl(const std::shared_ptr<X11Window> &window) : display(window->display), screen(window->GetScreenNum()) {
    unsigned long valuemask = 0;
    XGCValues values = {};
    unsigned int line_width = 1;
    int line_style = LineSolid;
    int cap_style = CapButt;
    int join_style = JoinBevel;

    auto *disp = display->Impl().display;
    auto &win = window->Impl().window;

    gc = XCreateGC(disp, win, valuemask, &values);
    if (((intptr_t) gc) < 0) {
        throw std::exception();
    }
    XSetForeground(disp, gc, WhitePixel(disp, screen));
    XSetBackground(disp, gc, BlackPixel(disp, screen));
    XSetLineAttributes(disp, gc, line_width, line_style, cap_style,  join_style);
    XSetFillStyle(disp, gc, FillSolid);
    XFlush(disp);
    XSync(disp, False);
}

X11GCImpl::~X11GCImpl() noexcept {
}

const X11GCImpl &X11GCImpl::Impl() const {
    return *this;
}

void X11GCImpl::SetForegroundBlack() {
    auto *disp = display->Impl().display;
    XSetForeground(disp, gc, BlackPixel(disp, screen));
    XFlush(disp);
    XSync(disp, False);
}

void X11GCImpl::SetForegroundRed() {
    XColor xcolor = {
            .red = 65535,
            .green = 0,
            .blue = 0,
            .flags = (DoRed | DoGreen | DoBlue)
    };
    auto *disp = display->Impl().display;
    XAllocColor(disp, DefaultColormap(disp, screen), &xcolor);
    XSetForeground(disp, gc, xcolor.pixel);
    XFlush(disp);
    XSync(disp, False);
}
