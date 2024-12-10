//
// Created by Mateusz on 25.11.2024.
//

#ifndef TABUSEARCH_H
#define TABUSEARCH_H
#include <limits>
#include <string>


class TabuSearch {

private:
    int** distanceMatrix;   // Macierz odległości
    int** tabuList;         // Lista tabu (alokacja dynamiczna)
    int numCities;          // Liczba miast
    int tabuTenure = 15;         // Czas trwania zakazu na liście tabu
    int maxNoImprove = 200000;       // Maksymalna liczba iteracji bez poprawy
    double timeLimit = 20.0;       // Limit czasu (w sekundach)
    double bestFindTime = 0.0;

    int (*neighborhoodFunc)(int*, int, int*, int**) = &swapNeighborhood; // Wskaźnik na funkcję definiującą sąsiedztwo
    int calculateCost(int* route);
    void generateRandomRoute(int* route);
    static int swapNeighborhood(int* CurrentRoute, int numCities, int* bestNeighbor, int** distanceMatrix);
    static int insertNeighborhood(int* currentRoute, int numCities, int* bestNeighbor, int** distanceMatrix);
    static int reverseNeighborhood(int* currentRoute, int numCities, int* bestNeighbor, int** distanceMatrix);

    void generateSwapNeighborhood(int* solution);
    void generateReverseNeighborhood(int* solution);
    void generateInsertNeighborhood(int* solution);

    void perturbRoute(int* route);
    void twoOptSwap(int* route, int i, int k);


public:
    // TabuSearch(int tenure = 10, int maxIter = 1000, double timeLimitSec = 10.0)
    //     : tabuTenure(tenure), maxNoImprove(maxIter), timeLimit(timeLimitSec), neighborhoodFunc(nullptr)
    // {
    //     distanceMatrix = nullptr;
    //     tabuList = nullptr;
    // }


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
    void setTenure(int newTenure);
    void setTimeLimit(double newTimeLimit);
    int* generateGreedyRoute();
    void generateNeighborhood(int* currentSolution, int neighborhoodFunc);

};



#endif //TABUSEARCH_H
