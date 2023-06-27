//
// Created by sigsegv on 5/16/23.
//

#ifndef ELMOBD_X11DISPLAYIMPL_H
#define ELMOBD_X11DISPLAYIMPL_H

#include "X11Display.h"
#include <X11/Xlib.h>

class X11GCImpl;
class AnalogGauge;
class X11Window;
class WarningsPanel;
class ClearDtcButton;

class X11DisplayImpl : public X11Display {
    friend X11WindowImpl;
    friend X11Window;
    friend X11GCImpl;
    friend AnalogGauge;
    friend WarningsPanel;
    friend ClearDtcButton;
private:
    Display *display;
public:
    X11DisplayImpl(const std::string &screenName);
    ~X11DisplayImpl() override;
    const X11DisplayImpl &Impl() const override;
    int GetDefaultScreen() const override;
    int GetScreenWidth(int screenNum) const override;
    int GetScreenHeight(int screenNum) const override;
    void ResetScreenSaver() const override;
};


#endif //ELMOBD_X11DISPLAYIMPL_H
