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
    int tabuTenure = 10;         // Czas trwania zakazu na liście tabu 55 (5-15) ; 170 (10-25) ; 358 (15-35)
    int maxNoImprove = 50;       // Maksymalna liczba iteracji bez poprawy 55 (30-50) ; 170 (50-100) ; 358 ( 100 - 200)
    double timeLimit = 60.0;       // Limit czasu (w sekundach)
    double bestFindTime = 0.0;

    int (TabuSearch::*neighborhoodFunc)(int*, int*) = &swapNeighborhood; // Wskaźnik na funkcję definiującą sąsiedztwo
    int calculateCost(int* route);
    void generateRandomRoute(int* route);
    int swapNeighborhood(int* currentRoute, int* bestNeighbor);
    int insertNeighborhood(int* currentRoute, int* bestNeighbor);
    int reverseNeighborhood(int* currentRoute, int* bestNeighbor);
    int kSwapNeighborhood(int* currentRoute, int* bestNeighbor);

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
