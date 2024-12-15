//
// Created by Mateusz on 25.11.2024.
//

#include "App.h"

#include <iostream>
#include <sstream>
#include <conio.h>

#include "algorithms/SimAnnealing.h"



void App::run()
{
    //Inicjalizacja
    // TabuSearch ts(10,100,20.0);
    ts.setNeighborhood(1);
    // SimAnnealing sm(1, 0.95, 20.0);

    bool isMatrix = false;
    string path;
    double timeLimit;
    int tsChoice;
    int smChoice;
    bool exit = false;

    while (!exit)
    {
        menu.mainMenu();

        switch (menu.mainChoice) {
        case '1':
            if (isMatrix) {
                runAlgorithms();
            }
            else {
                cout << "Nie masz zadnej zapisanej macierzy!" << endl;
            }
            break;
        case '2':
            path = menu.inputPath();
            //ts.loadFromFile(path);
            model.loadFromFile(path);
            ts.setDistanceMatrix(model.getDistanceMatrix());
            ts.setNumCities(model.getNumCities());
            //sm.loadFromFile(path);
            sw.setDistanceMatrix(model.getDistanceMatrix());
            sw.setNumCities(model.getNumCities());
            isMatrix = true;
            break;
        case '3':
            timeLimit = menu.inputStop();
            ts.setTimeLimit(timeLimit);
            sw.setTimeLimit(timeLimit);
            break;
        case '4':
            menu.tsMenu();
            tsChoice = menu.tsChoice - '0';
            ts.setNeighborhood(tsChoice);
            break;
        case '5':
            menu.swMenu();
            smChoice = menu.swChoice - '0';
            sw.setCoolingScheme(smChoice);
            break;
        case '6':
            sw.setCoolingRate(menu.inputFactorA());
            break;
        case '7':
            ts.generateGreedyRoute();
            break;
        case '8':
            exit = true;
            break;
        }

    }
}

void App::runAlgorithms()
{
        menu.algorithmsMenu();
        int* solution;
        string filepath = "wynik.txt";
        srand(time(NULL));
        switch (menu.algorithmChoice) {
        case '1':
                solution = ts.solve();
            break;
        case '2':
            solution = sw.solve();
            break;
            default:
                cout << "Error" << endl;
        }
        model.saveResultToFile(filepath,solution,model.getNumCities());
        std::cout << "Najkrotsza sciezka: " << model.calculateCostFromFile(filepath,model.getDistanceMatrix(),model.getNumCities()) << endl;

        delete[] solution;

        std::cout << "Kliknij dowolny klawisz aby wrocic do menu" << std::endl;
        getch();

}

