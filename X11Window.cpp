//
// Created by sigsegv on 5/16/23.
//

#include "X11Window.h"
#include "X11DisplayImpl.h"
#include "X11WindowImpl.h"
#include "Widget.h"

std::shared_ptr<X11Window> X11Window::Create(const std::shared_ptr<X11Display> &display, int screenNum, int width, int height, int x, int y) {
    return std::make_shared<X11WindowImpl>(display, screenNum, width, height, x, y);
}

void X11Window::Add(const std::shared_ptr<Widget> &widget) {
    widgets.emplace_back(widget);
    widget->Init(ImplRef());
}

void X11Window::RenderingCycle() {
    for (const auto &widget : widgets) {
        widget->BlankForeground(0, 0, 200, 200);
        widget->Mutate();
        widget->DrawForeground(0, 0, 200, 200);
        widget->DrawBackground(0, 0, 200, 200);
        widget->DrawForeground(0, 0, 200, 200);
    }
    auto *display = this->display->Impl().display;
    XFlush(display);
    XSync(display, False);
}
