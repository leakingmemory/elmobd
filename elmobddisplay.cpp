//
// Created by sigsegv on 5/16/23.
//

#include "ResilientCarDatasource.h"
#include "ElmObdDisplay.h"
#include "WarningsData.h"
#include <memory>

int main() {
    std::shared_ptr<WarningsData> warningsData = std::make_shared<WarningsData>();
    std::shared_ptr<CarDatasource> resilientCarDatasource = std::make_shared<ResilientOBDCarDatasource>(warningsData);

    ElmObdDisplay elmObdDisplay{resilientCarDatasource, warningsData};
    elmObdDisplay.Run();
}