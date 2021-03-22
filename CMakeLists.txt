cmake_minimum_required(VERSION 3.1)
project(LSPpp)

set(CMAKE_CXX_STANDARD 14)

include_directories(LPSpp)

add_executable(LSPpp
        LPSpp/Instruction.cpp
        LPSpp/Instruction.h
        LPSpp/Main.cpp
        LPSpp/Util.h
        LPSpp/Vm.cpp
        LPSpp/Vm.h)
