cmake_minimum_required(VERSION 3.29)
project(PEA_projekt_2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(PEA_projekt_2 main.cpp
        Model.cpp
        Model.h
        App.cpp
        App.h
        Helpers.cpp
        Helpers.h
        algorithms/TabuSearch.cpp
        algorithms/TabuSearch.h
        Menu.cpp
        Menu.h
        algorithms/SimAnnealing.cpp
        algorithms/SimAnnealing.h
        algorithms/Greedy.cpp
        algorithms/Greedy.h)
