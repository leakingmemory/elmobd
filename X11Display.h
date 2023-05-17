//
// Created by sigsegv on 5/16/23.
//

#ifndef ELMOBD_X11DISPLAY_H
#define ELMOBD_X11DISPLAY_H

#include <string>
#include <memory>

class X11WindowImpl;
class X11DisplayImpl;

class X11Display {
    friend X11WindowImpl;
    friend X11DisplayImpl;
private:
    X11Display();
public:
    static std::shared_ptr<X11Display> Create(std::string screenName);
    static std::shared_ptr<X11Display> Create();
    virtual ~X11Display() = default;
    virtual const X11DisplayImpl &Impl() const = 0;
    virtual int GetDefaultScreen() const = 0;
    virtual int GetScreenWidth(int screenNum) const = 0;
    virtual int GetScreenHeight(int screenNum) const = 0;
};


#endif //ELMOBD_X11DISPLAY_H
