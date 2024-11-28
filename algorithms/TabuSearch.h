//
// Created by Mateusz on 25.11.2024.
//

#ifndef TABUSEARCH_H
#define TABUSEARCH_H
#include <string>


class TabuSearch {

private:
    int** distanceMatrix;   // Macierz odległości
    int** tabuList;         // Lista tabu (alokacja dynamiczna)
    int numCities;          // Liczba miast
    int tabuTenure;         // Czas trwania zakazu na liście tabu
    int maxNoImprove;       // Maksymalna liczba iteracji bez poprawy
    double timeLimit;       // Limit czasu (w sekundach)
    int (*neighborhoodFunc)(int*, int, int*, int**); // Wskaźnik na funkcję definiującą sąsiedztwo
    int calculateCost(int* route);
    void generateRandomRoute(int* route);
    static int swapNeighborhood(int* CurrentRoute, int numCities, int* bestNeighbor, int** distanceMatrix);
    static int insertNeighborhood(int* currentRoute, int numCities, int* bestNeighbor, int** distanceMatrix);
    static int reverseNeighborhood(int* currentRoute, int numCities, int* bestNeighbor, int** distanceMatrix);

public:
    TabuSearch(int tenure = 10, int maxIter = 1000, double timeLimitSec = 10.0)
        : tabuTenure(tenure), maxNoImprove(maxIter), timeLimit(timeLimitSec), neighborhoodFunc(nullptr)
    {
        distanceMatrix = nullptr;
        tabuList = nullptr;
    }
    ~TabuSearch()
    {
        if (distanceMatrix) {
            for (int i = 0; i < numCities; ++i) delete[] distanceMatrix[i];
            delete[] distanceMatrix;
        }
        if (tabuList) {
            for (int i = 0; i < numCities; ++i) delete[] tabuList[i];
            delete[] tabuList;
        }
    }
    void loadFromFile(const std::string& fileName);
    void setNeighborhood(int choice);
    int* solve();
    int getNumCities();
    void saveResultToFile(const std::string& filename, int* route, int numCities);
    int calculateCostFromFile(const std::string& filename, int** distanceMatrix, int numCities);
    int** getDistanceMatrix();

};



#endif //TABUSEARCH_H
