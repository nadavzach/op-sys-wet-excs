cmake_minimum_required(VERSION 3.16.5)
project(op_sys_wet_excs)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(op_sys_wet_excs
        commands.cpp
        commands.h
        signals.cpp
        signals.h
        smash.cpp)
