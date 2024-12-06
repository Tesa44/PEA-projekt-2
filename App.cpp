//
// Created by Mateusz on 25.11.2024.
//

#include "App.h"

#include <iostream>


#include "algorithms/SimAnnealing.h"


void App::run()
{
    //Inicjalizacja
    // TabuSearch ts(10,100,20.0);
    //ts.setNeighborhood(1);
    // SimAnnealing sm(1, 0.95, 20.0);

    bool isMatrix = false;
    string path;
    double timeLimit;
    int tsChoice;
    int smChoice;
    while (true)
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
            ts.loadFromFile(path);
            sm.loadFromFile(path);
            isMatrix = true;
            break;
        case '3':
            timeLimit = menu.inputStop();
            ts.setTimeLimit(timeLimit);
            sm.setTimeLimit(timeLimit);
            break;
        case '4':
            menu.tsMenu();
            tsChoice = menu.tsChoice - '0';
            ts.setNeighborhood(tsChoice);
            break;
        case '5':
            menu.swMenu();
            smChoice = menu.swChoice - '0';
            sm.setCoolingScheme(smChoice);
            break;
        case '6':
            sm.setCoolingRate(menu.inputFactorA());
            break;
        case '7':
            ts.generateGreedyRoute();
            break;
        }
    }
}

void App::runAlgorithms()
{
    while (menu.exitConfirmChoice != '2')
    {
        menu.algorithmsMenu();
        int* solution;
        switch (menu.algorithmChoice) {
        case '1':
            for (int i = 0; i <= 5; i++)
            {
                solution = ts.solve();
                // std::cout << ts.getNumCities() << std::endl;
                //      std::cout << "Najlepsza trasa: ";
                //      for (int i = 0; i < ts.getNumCities() + 1; ++i) {
                //          std::cout << solution[i] << " ";

                //      }
                //      std::cout << '\n';
                ts.saveResultToFile("wynik.txt",solution,ts.getNumCities());
                std:: cout << ts.calculateCostFromFile("wynik.txt",ts.getDistanceMatrix(),ts.getNumCities()) << std::endl;
            }
            break;
        case '2':
            for (int i = 0; i <= 5; i++)
            {
                sm.solve();
                sm.saveResultToFile("wynik.txt");
                std:: cout << ts.calculateCostFromFile("wynik.txt",ts.getDistanceMatrix(),ts.getNumCities()) << std::endl;
            }
            break;
        }
        std:: cout << ts.calculateCostFromFile("wynik.txt",ts.getDistanceMatrix(),ts.getNumCities()) << std::endl;
        menu.exitConfirmMenu();
        delete[] solution;
    }
}

