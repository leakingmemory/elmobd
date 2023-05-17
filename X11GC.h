//
// Created by sigsegv on 5/17/23.
//

#ifndef ELMOBD_X11GC_H
#define ELMOBD_X11GC_H

#include <memory>

class X11GCImpl;
class X11Display;
class X11Window;

class X11GC {
public:
    virtual ~X11GC() = default;
    static std::shared_ptr<X11GC> Create(const std::shared_ptr<X11Window> &window);
    virtual const X11GCImpl &Impl() const = 0;
    virtual void SetForegroundBlack() = 0;
};


#endif //ELMOBD_X11GC_H
