//
// Created by Mateusz on 25.11.2024.
//

#ifndef APP_H
#define APP_H
#include "Menu.h"
#include "Model.h"
#include "algorithms/SimAnnealing.h"
#include "algorithms/TabuSearch.h"


class App {
public:
    void runAlgorithms();
    void run();

private:
    int coolingScheme = 1;    // Schemat schładzania (1, 2, 3)
    double coolingRate = 0.95; // Współczynnik zmiany temperatury
    double timeLimit = 20.0;  // Limit czasu w sekundach

    Menu menu;
    TabuSearch ts;
    SimAnnealing sw;
    Model model;

};



#endif //APP_H
