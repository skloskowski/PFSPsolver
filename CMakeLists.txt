cmake_minimum_required(VERSION 4.1)
project(PFSPsolver)

set(CMAKE_CXX_STANDARD 20)

add_executable(PFSPsolver src/main.cpp
        src/PFSPInstance.cpp
        src/PFSPInstance.h
        src/Solver.cpp
        src/Solver.h
        src/GAConfig.cpp
        src/GAConfig.h
)
