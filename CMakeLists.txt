cmake_minimum_required(VERSION 3.13)
project(Transoprtation_theory)

set(CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_NAME} src/main.cpp
        src/IndexPair.cpp
        src/IndexPair.h
        src/IndexSet.cpp
        src/IndexSet.h
        src/MatrixInt.cpp
        src/MatrixInt.h
        src/TransportationTheorySolver.cpp
        src/TransportationTheorySolver.h
        src/VecInt.cpp
        src/VecInt.h)
