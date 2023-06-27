//
// Created by sigsegv on 5/18/23.
//

#ifndef ELMOBD_WARNINGSPANEL_H
#define ELMOBD_WARNINGSPANEL_H

#include "Widget.h"
#include "Meter.h"
#include <vector>

class X11GC;
class WarningsData;

class WarningsPanel : public Widget, public Meter {
private:
    std::mutex currentValueMtx{};
    std::weak_ptr<X11Window> window;
    std::shared_ptr<X11GC> gc;
    std::shared_ptr<X11GC> blankGc;
    std::shared_ptr<WarningsData> warningsData;
    std::vector<std::string> messages;
    std::vector<std::string> displayedMessages;
public:
    WarningsPanel(const std::shared_ptr<WarningsData> &warningsData);
    void Init(std::shared_ptr<X11Window> window) override;
    bool HasPendingMutation() override;
    void Mutate() override;
    void DrawBackground(int x, int y, int width, int height) override;
    void DrawForeground(const std::shared_ptr<X11GC> &usingGc, int x, int y, int width, int height);
    void BlankForeground(int x, int y, int width, int height) override;
    void DrawForeground(int x, int y, int width, int height) override;
    void Update() override;
    PriorityCategory GetPriorityCategory() const override;
    void Clicked() override;
};

#endif //ELMOBD_WARNINGSPANEL_H
