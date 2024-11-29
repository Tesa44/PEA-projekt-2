//
// Created by Mateusz on 28.11.2024.
//

#ifndef SIMANNEALING_H
#define SIMANNEALING_H
#include <string>


class SimAnnealing {
private:
    int numCities;            // Liczba miast
    int** distanceMatrix;  // Macierz odległości
    int* currentRoute;        // Obecna trasa
    int* bestRoute;           // Najlepsza trasa
    double temperature;       // Bieżąca temperatura
    double coolingRate;       // Współczynnik schładzania
    int coolingScheme;        // Typ schładzania
    double timeLimit;         // Limit czasu
    int currentCost;          // Koszt obecnej trasy
    int bestCost;             // Koszt najlepszej trasy


public:
    SimAnnealing(int coolingScheme, double coolingRate, double timeLimit);

    ~SimAnnealing() {
        if (distanceMatrix) {
            for (int i = 0; i < numCities; ++i) {
                delete[] distanceMatrix[i];
            }
            delete[] distanceMatrix;
        }
        delete[] currentRoute;
        delete[] bestRoute;
    }
    void loadFromFile(const std::string& filename);
    void initializeRoute();
    void calculateInitialTemperature();
    int calculateCost(int* route);
    void perturbRoute(int* route);
    void run();
    void setCoolingScheme(int scheme);
    void saveResultToFile(const std::string& filename);
};




#endif //SIMANNEALING_H
