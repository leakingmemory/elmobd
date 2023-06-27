//
// Created by sigsegv on 6/27/23.
//

#ifndef ELMOBD_CLEARDTCBUTTON_H
#define ELMOBD_CLEARDTCBUTTON_H

#include <mutex>
#include "Widget.h"
#include "X11GC.h"

class X11Window;
class CarDatasource;

class ClearDtcButton : public Widget {
private:
    std::mutex *mtx;
    std::shared_ptr<CarDatasource> serialCarDevice;
    std::weak_ptr<X11Window> window;
    std::shared_ptr<X11GC> gc;
    std::shared_ptr<X11GC> blankGc;
public:
    ClearDtcButton(std::shared_ptr<CarDatasource> serialCarDevice, std::mutex &mtx) : serialCarDevice(serialCarDevice), mtx(&mtx) {}
    void Init(std::shared_ptr<X11Window> window) override;
    bool HasPendingMutation() override;
    void Mutate() override;
    void DrawBackground(int x, int y, int width, int height) override;
    void DrawForeground(const std::shared_ptr<X11GC> &usingGc, int x, int y, int width, int height);
    void BlankForeground(int x, int y, int width, int height) override;
    void DrawForeground(int x, int y, int width, int height) override;
    void Clicked() override;
};


#endif //ELMOBD_CLEARDTCBUTTON_H
