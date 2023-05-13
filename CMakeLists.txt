cmake_minimum_required(VERSION 3.25)
project(elmobd)

set(CMAKE_CXX_STANDARD 20)

add_executable(elmobd main.cpp SerialInterface.cpp SerialInterface.h SerialCanDevice.cpp SerialCanDevice.h)
