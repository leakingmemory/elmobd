//
// Created by sigsegv on 5/18/23.
//

#ifndef ELMOBD_ELMOBDDISPLAY_H
#define ELMOBD_ELMOBDDISPLAY_H

#include <memory>

class CarDatasource;

class ElmObdDisplay {
private:
    std::shared_ptr<CarDatasource> carDatasource;
public:
    ElmObdDisplay(std::shared_ptr<CarDatasource> carDatasource) : carDatasource(carDatasource) {}
    void Run() const;
};


#endif //ELMOBD_ELMOBDDISPLAY_H
