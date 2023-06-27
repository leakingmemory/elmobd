//
// Created by sigsegv on 5/16/23.
//

#ifndef ELMOBD_WIDGET_H
#define ELMOBD_WIDGET_H

#include <memory>

class X11Display;
class X11Window;

class Widget {
public:
    virtual void Init(std::shared_ptr<X11Window> window) = 0;
    virtual bool HasPendingMutation() = 0;
    virtual void Mutate() = 0;
    virtual void DrawBackground(int x, int y, int width, int height) = 0;
    virtual void BlankForeground(int x, int y, int width, int height) = 0;
    virtual void DrawForeground(int x, int y, int width, int height) = 0;
    virtual void Clicked() = 0;
};

#endif //ELMOBD_WIDGET_H
