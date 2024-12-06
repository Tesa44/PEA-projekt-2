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
    double coolingRate = 0.999;       // Współczynnik schładzania
    int coolingScheme = 1;        // Typ schładzania
    double timeLimit = 20.0;         // Limit czasu
    int maxIter = 100;         //Maksymalna liczba iteracji bez dywersyfikacji
    int currentCost;          // Koszt obecnej trasy
    int bestCost;             // Koszt najlepszej trasy


public:
    //SimAnnealing(int coolingScheme, double coolingRate, double timeLimit);

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
    void solve();
    void setCoolingScheme(int scheme);
    void saveResultToFile(const std::string& filename);
    void setTimeLimit(double newTimeLimit);
    void setCoolingRate(double newCoolingRate);
};




#endif //SIMANNEALING_H
