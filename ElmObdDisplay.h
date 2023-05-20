//
// Created by sigsegv on 5/18/23.
//

#ifndef ELMOBD_ELMOBDDISPLAY_H
#define ELMOBD_ELMOBDDISPLAY_H

#include <memory>

class CarDatasource;
class WarningsData;

class ElmObdDisplay {
private:
    std::shared_ptr<CarDatasource> carDatasource;
    std::shared_ptr<WarningsData> warningsData;
public:
    ElmObdDisplay(const std::shared_ptr<CarDatasource> &carDatasource, const std::shared_ptr<WarningsData> &warningsData) : carDatasource(carDatasource), warningsData(warningsData) {}
    void Run() const;
};


#endif //ELMOBD_ELMOBDDISPLAY_H
