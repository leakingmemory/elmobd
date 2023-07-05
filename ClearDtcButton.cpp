//
// Created by sigsegv on 6/27/23.
//

#include "ClearDtcButton.h"
#include "X11WindowImpl.h"
#include "X11DisplayImpl.h"
#include "X11GCImpl.h"
#include "SerialCarDevice.h"
#include <iostream>

void ClearDtcButton::Init(std::shared_ptr<X11Window> window) {
    this->window = std::weak_ptr<X11Window>(window);
    this->gc = X11GC::Create(window);
    this->blankGc = X11GC::Create(window);
    this->blankGc->SetForegroundBlack();
}

bool ClearDtcButton::HasPendingMutation() {
    return true;
}

void ClearDtcButton::Mutate() {
}

void ClearDtcButton::DrawBackground(int x, int y, int width, int height) {
    std::shared_ptr<X11Window> windowSh = this->window.lock();
    auto *display = windowSh->display->Impl().display;
    auto &window = windowSh->Impl().window;
    auto &gc = this->gc->Impl().gc;
    XDrawLine(display, window, gc, x, y, x + width - 1, y);
    XDrawLine(display, window, gc, x + width - 1, y, x + width - 1, y + height - 1);
    XDrawLine(display, window, gc, x, y + height - 1, x + width - 1, y + height - 1);
    XDrawLine(display, window, gc, x, y, x, y + height - 1);
}

void ClearDtcButton::DrawForeground(const std::shared_ptr<X11GC> &usingGc, int x, int y, int width, int height) {
    std::shared_ptr<X11Window> windowSh = this->window.lock();
    auto *display = windowSh->display->Impl().display;
    auto &window = windowSh->Impl().window;
    auto &gc = usingGc->Impl().gc;
    std::string msg{"Clear DTCs"};
    XTextItem textItem{
            .chars = (char *) msg.c_str(),
            .nchars = (int) msg.size(),
            .delta = 0,
            .font = None
    };
    XDrawText(display, window, gc, x + 5, y + 20, &textItem, 1);
}

void ClearDtcButton::BlankForeground(int x, int y, int width, int height) {
    DrawForeground(blankGc, x, y, width, height);
}

void ClearDtcButton::DrawForeground(int x, int y, int width, int height) {
    DrawForeground(gc, x, y, width, height);
}

void ClearDtcButton::Clicked() {
    std::cout << "Clearing DTCs\n";
    std::string error{};
    {
        std::lock_guard lock{*mtx};
        serialCarDevice->ClearDTCEtc();
        error = serialCarDevice->GetLastError();
    }
    if (!error.empty()) {
        std::cerr << "clear dtc error: " << error << "\n";
    }
}