//
// Created by sigsegv on 5/16/23.
//

#ifndef ELMOBD_X11WINDOW_H
#define ELMOBD_X11WINDOW_H

#include <memory>
#include <vector>

class X11Display;
class X11WindowImpl;
class Widget;
class X11GCImpl;
class AnalogGauge;

struct WindowWidget {
    int x, y, width, height;
    std::shared_ptr<Widget> widget;
};

class X11Window {
    friend X11WindowImpl;
    friend X11GCImpl;
    friend AnalogGauge;
private:
    std::shared_ptr<X11Display> display;
    std::vector<WindowWidget> widgets;
    int screenNum;
    X11Window(const std::shared_ptr<X11Display> &display, int screenNum) : display(display), screenNum(screenNum) {}
public:
    static std::shared_ptr<X11Window> Create(const std::shared_ptr<X11Display> &display, int screenNum, int width, int height, int x, int y);
    virtual ~X11Window() = default;
    virtual const X11WindowImpl &Impl() const = 0;
    virtual std::shared_ptr<X11WindowImpl> ImplRef() = 0;
    void Add(const std::shared_ptr<Widget> &widget, int x, int y, int width, int height);
    void RenderingCycle();
    int GetScreenNum() const {
        return screenNum;
    }
};

#endif //ELMOBD_X11WINDOW_H
