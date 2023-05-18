cmake_minimum_required(VERSION 3.25)
project(elmobd)

set(CMAKE_CXX_STANDARD 20)

add_library(elmobdobj OBJECT SerialInterface.cpp SerialInterface.h SerialCanDevice.cpp SerialCanDevice.h
        SerialCarDevice.cpp SerialCarDevice.h SerialCarDevice.cpp SerialCarDevice.h CarDatasource.h)

add_executable(elmobd main.cpp)
target_link_libraries(elmobd PRIVATE elmobdobj)

add_library(elmobddisplayobj OBJECT X11Display.cpp X11Display.h X11Window.cpp X11Window.h
        X11DisplayImpl.cpp X11DisplayImpl.h Widget.h AnalogGauge.cpp AnalogGauge.h X11GC.cpp X11GC.h
        X11GCImpl.cpp X11GCImpl.h X11WindowImpl.cpp X11WindowImpl.h Meter.h
        ManPressGauge.cpp ManPressGauge.h MassFlowGauge.cpp MassFlowGauge.h
        IntakeAirTempGauge.cpp IntakeAirTempGauge.h FuelTrimGauge.cpp FuelTrimGauge.h
        ShortTermFuelTrimGauge.cpp ShortTermFuelTrimGauge.h
        Bank1ShortTermFuelTrimGauge.cpp Bank1ShortTermFuelTrimGauge.h
        Bank2ShortTermFuelTrimGauge.cpp Bank2ShortTermFuelTrimGauge.h
        LongTermFuelTrimGauge.cpp LongTermFuelTrimGauge.h
        Bank1LongTermFuelTrimGauge.cpp Bank1LongTermFuelTrimGauge.h
        Bank2LongTermFuelTrimGauge.cpp Bank2LongTermFuelTrimGauge.h  ElmObdDisplay.cpp ElmObdDisplay.h
        O2FuelTrimGauge.cpp O2FuelTrimGauge.h  O2VoltageGauge.cpp O2VoltageGauge.h O2Gauge.cpp O2Gauge.h
        EngCoolantTempGauge.cpp EngCoolantTempGauge.h)

add_executable(elmobddisplay elmobddisplay.cpp)
target_link_libraries(elmobddisplay PRIVATE elmobdobj)
target_link_libraries(elmobddisplay PRIVATE elmobddisplayobj)
target_link_libraries(elmobddisplay PRIVATE -lX11)

add_executable(elmobdtestdisplay elmobdtestdisplay.cpp CarDatasource.h)
target_link_libraries(elmobdtestdisplay PRIVATE elmobdobj)
target_link_libraries(elmobdtestdisplay PRIVATE elmobddisplayobj)
target_link_libraries(elmobdtestdisplay PRIVATE -lX11)

enable_testing()

add_executable(SerialCanDeviceTest SerialCanDeviceTest.cpp SerialCanDevice.cpp SerialCanDevice.h
        SerialInterface.cpp SerialInterface.h)

add_test(SerialCanDeviceTest COMMAND SerialCanDeviceTest)
