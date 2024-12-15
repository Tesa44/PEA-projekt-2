cmake_minimum_required(VERSION 3.29)
project(PEA_projekt_2)

set(CMAKE_CXX_STANDARD 20)

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
