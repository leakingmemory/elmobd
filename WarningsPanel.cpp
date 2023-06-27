//
// Created by sigsegv on 5/18/23.
//

#include "WarningsPanel.h"
#include "X11GCImpl.h"
#include "X11WindowImpl.h"
#include "X11DisplayImpl.h"
#include "WarningsData.h"
#include <X11/Xlib.h>

WarningsPanel::WarningsPanel(const std::shared_ptr<WarningsData> &warningsData) : warningsData(warningsData) {
    messages.emplace_back("DATA FEED");
}

void WarningsPanel::Init(std::shared_ptr<X11Window> window) {
    this->window = std::weak_ptr<X11Window>(window);
    this->gc = X11GC::Create(window);
    this->blankGc = X11GC::Create(window);
    this->blankGc->SetForegroundBlack();
}

bool WarningsPanel::HasPendingMutation() {
    std::lock_guard lock{currentValueMtx};
    bool pending = displayedMessages.size() != messages.size();
    if (pending) {
        return true;
    }
    for (const auto &msg : messages) {
        bool found{false};
        for (const auto &dispmsg : displayedMessages) {
            if (msg == dispmsg) {
                found = true;
                break;
            }
        }
        if (!found) {
            return true;
        }
    }
    return pending;
}

void WarningsPanel::Mutate() {
    std::lock_guard lock{currentValueMtx};
    displayedMessages = messages;
}

void WarningsPanel::DrawBackground(int x, int y, int width, int height) {
    std::shared_ptr<X11Window> windowSh = this->window.lock();
    auto *display = windowSh->display->Impl().display;
    auto &window = windowSh->Impl().window;
    auto &gc = this->gc->Impl().gc;
    XDrawLine(display, window, gc, x, y, x + width - 1, y);
    XDrawLine(display, window, gc, x + width - 1, y, x + width - 1, y + height - 1);
    XDrawLine(display, window, gc, x, y + height - 1, x + width - 1, y + height - 1);
    XDrawLine(display, window, gc, x, y, x, y + height - 1);
}

void WarningsPanel::DrawForeground(const std::shared_ptr<X11GC> &usingGc, int x, int y, int width, int height) {
    std::shared_ptr<X11Window> windowSh = this->window.lock();
    auto *display = windowSh->display->Impl().display;
    auto &window = windowSh->Impl().window;
    auto &gc = usingGc->Impl().gc;
    //auto *font = XQueryFont(display, XGContextFromGC(gc));
    int offY = 20;
    int offLim = height - 25;
    for (const auto &msg : displayedMessages) {
        if (offY > offLim) {
            break;
        }
        XTextItem textItem{
                .chars = (char *) msg.c_str(),
                .nchars = (int) msg.size(),
                .delta = 0,
                .font = None
        };
        XDrawText(display, window, gc, x + 5, y + offY, &textItem, 1);
        offY += 20;
    }
}

void WarningsPanel::BlankForeground(int x, int y, int width, int height) {
    DrawForeground(blankGc, x, y, width, height);
}

void WarningsPanel::DrawForeground(int x, int y, int width, int height) {
    DrawForeground(gc, x, y, width, height);
}

void WarningsPanel::Update() {
    auto messages = warningsData->GetMessages();
    std::lock_guard lock{currentValueMtx};
    this->messages = messages;
}

PriorityCategory WarningsPanel::GetPriorityCategory() const {
    return PriorityCategory::HIGH;
}

void WarningsPanel::Clicked() {
}