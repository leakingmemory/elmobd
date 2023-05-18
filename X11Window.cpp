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

void X11Window::Add(const std::shared_ptr<Widget> &widget, int x, int y, int width, int height) {
    WindowWidget windowWidget{.x = x, .y = y, .width = width, .height = height, .widget = widget};
    widgets.emplace_back(windowWidget);
    widget->Init(ImplRef());
}

void X11Window::RenderingCycle() {
    for (const auto &widget : widgets) {
        if (!widget.widget->HasPendingMutation()) {
            continue;
        }
        widget.widget->BlankForeground(widget.x, widget.y, widget.width, widget.height);
        widget.widget->Mutate();
        widget.widget->DrawForeground(widget.x, widget.y, widget.width, widget.height);
        widget.widget->DrawBackground(widget.x, widget.y, widget.width, widget.height);
        widget.widget->DrawForeground(widget.x, widget.y, widget.width, widget.height);
    }
    auto *display = this->display->Impl().display;
    XFlush(display);
    XSync(display, False);
}
