//
// Created by sigsegv on 5/17/23.
//

#ifndef ELMOBD_METER_H
#define ELMOBD_METER_H

enum class PriorityCategory {
    HIGH,
    LOW,
    VERY_LOW
};

class Meter {
public:
    virtual void Update() = 0;
    virtual PriorityCategory GetPriorityCategory() const = 0;
};


#endif //ELMOBD_METER_H
