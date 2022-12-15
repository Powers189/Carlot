cmake_minimum_required(VERSION 3.16.3)
project(SimulationFinal)

set(CMAKE_CXX_STANDARD 14)

add_executable(SIM main.cpp Random.cpp Random.h Event.cpp Event.h ArrivalObject.cpp ArrivalObject.h)
