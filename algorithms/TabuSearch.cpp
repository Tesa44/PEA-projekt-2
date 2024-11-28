//
// Created by Mateusz on 25.11.2024.
//

#include "TabuSearch.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <climits>
#include <chrono>


int TabuSearch::calculateCost(int* route) {
        int cost = 0;
        for (int i = 0; i < numCities ; ++i) {
            cost += distanceMatrix[route[i]][route[i + 1]];
        }
        //cost += distanceMatrix[route[numCities - 1]][route[0]];
        return cost;
}

void TabuSearch::generateRandomRoute(int* route) {
    route[0] = 0; //Zawsze zaczynamy od miasta 0
        for (int i = 1; i < numCities; ++i) route[i] = i;
        for (int i = 1; i < numCities; ++i) {
            int j = rand() % (numCities - 1) + 1;
            std::swap(route[i], route[j]);
        }
    }

int TabuSearch::swapNeighborhood(int* currentRoute, int numCities, int* bestNeighbor, int** distanceMatrix) {
        int bestCost = INT_MAX;
        for (int i = 1; i < numCities - 1; ++i) {
            for (int j = i + 1; j < numCities; ++j) {
                int* tempRoute = new int[numCities];
                std::copy(currentRoute, currentRoute + numCities, tempRoute);
                std::swap(tempRoute[i], tempRoute[j]);  //Zamiana miast
                int cost = 0;
                for (int k = 0; k < numCities - 1; ++k) {
                    cost += distanceMatrix[tempRoute[k]][tempRoute[k + 1]];
                }
                cost += distanceMatrix[tempRoute[numCities - 1]][tempRoute[0]];

                if (cost < bestCost) {
                    bestCost = cost;
                    std::copy(tempRoute, tempRoute + numCities, bestNeighbor);
                }
                delete[] tempRoute;
            }
        }
        return bestCost;
    }

int TabuSearch::insertNeighborhood(int* currentRoute, int numCities, int* bestNeighbor, int** distanceMatrix) {
        int bestCost = INT_MAX;
        for (int i = 1; i < numCities; ++i) {
            for (int j = 0; j < numCities; ++j) {
                if (i == j) continue;

                int* tempRoute = new int[numCities];
                int idx = 0;
                for (int k = 0; k < numCities; ++k) {
                    if (k == i) continue;
                    tempRoute[idx++] = currentRoute[k];
                }
                for (int k = numCities - 1; k > j; --k) {
                    tempRoute[k] = tempRoute[k - 1];
                }
                tempRoute[j] = currentRoute[i];

                int cost = 0;
                for (int k = 0; k < numCities - 1; ++k) {
                    cost += distanceMatrix[tempRoute[k]][tempRoute[k + 1]];
                }
                cost += distanceMatrix[tempRoute[numCities - 1]][tempRoute[0]];

                if (cost < bestCost) {
                    bestCost = cost;
                    std::copy(tempRoute, tempRoute + numCities, bestNeighbor);
                }
                delete[] tempRoute;
            }
        }
        return bestCost;
    }

int TabuSearch::reverseNeighborhood(int* currentRoute, int numCities, int* bestNeighbor, int** distanceMatrix) {
        int bestCost = INT_MAX;
        for (int i = 1; i < numCities - 1; ++i) {
            for (int j = i + 1; j < numCities; ++j) {
                int* tempRoute = new int[numCities];
                std::copy(currentRoute, currentRoute + numCities, tempRoute);
                std::reverse(tempRoute + i, tempRoute + j + 1);

                int cost = 0;
                for (int k = 0; k < numCities - 1; ++k) {
                    cost += distanceMatrix[tempRoute[k]][tempRoute[k + 1]];
                }
                cost += distanceMatrix[tempRoute[numCities - 1]][tempRoute[0]];

                if (cost < bestCost) {
                    bestCost = cost;
                    std::copy(tempRoute, tempRoute + numCities, bestNeighbor);
                }
                delete[] tempRoute;
            }
        }
        return bestCost;
    }

void TabuSearch::loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) throw std::runtime_error("Nie można otworzyć pliku: " + filename);

        std::string line;
        while (std::getline(file, line)) {
            //Usuwanie białych znaków z początku i końca linii
            line.erase(0, line.find_first_not_of(" \t\n\r"));
            line.erase(line.find_last_not_of(" \t\n\r") + 1);

            if (line.empty() || line[0] == '#') {
                continue; // Ignoruj puste linie i komentarze
            }
            if (line.find("DIMENSION") != std::string::npos) {
                std::string temp;
                std::istringstream iss;
                iss.str(line);
                iss >> temp; // DIMENSION
                iss >> numCities;   //Liczba miast
            } else if (line.find("EDGE_WEIGHT_SECTION") != std::string::npos) {
                break;
            }
        }

        distanceMatrix = new int*[numCities];
        for (int i = 0; i < numCities; ++i) {
            distanceMatrix[i] = new int[numCities];
            for (int j = 0; j < numCities; ++j) {
                //Wczytywanie wartości macierzy z pliku
                file >> distanceMatrix[i][j];
            }
        }
    }

void TabuSearch::setNeighborhood(int choice) {
        switch (choice) {
            case 1:
                neighborhoodFunc = &swapNeighborhood;
                break;
            case 2:
                neighborhoodFunc = &insertNeighborhood;
                break;
            case 3:
                neighborhoodFunc = &reverseNeighborhood;
                break;
            default:
                throw std::invalid_argument("Niepoprawny wybór sąsiedztwa");
        }
    }

int* TabuSearch::solve() {
        tabuList = new int*[numCities];
        for (int i = 0; i < numCities; ++i) {
            tabuList[i] = new int[numCities]();
        }

        int* currentRoute = new int[numCities];
        int* bestRoute = new int[numCities + 1];
        generateRandomRoute(currentRoute);
        std::copy(currentRoute, currentRoute + numCities, bestRoute);
        int bestCost = calculateCost(bestRoute);

        auto startTime = std::chrono::steady_clock::now();
        int noImprove = 0;

        while (true) {
            auto currentTime = std::chrono::steady_clock::now();
            double elapsedTime = std::chrono::duration<double>(currentTime - startTime).count();
            if (elapsedTime > timeLimit) break;

            int* bestNeighbor = new int[numCities];
            int neighborCost = neighborhoodFunc(currentRoute, numCities, bestNeighbor, distanceMatrix);

            if (neighborCost < bestCost) {
                bestCost = neighborCost;
                std::copy(bestNeighbor, bestNeighbor + numCities, bestRoute);
                noImprove = 0;
            } else {
                noImprove++;
            }

            delete[] bestNeighbor;
            if (noImprove >= maxNoImprove) {
                generateRandomRoute(currentRoute); // Dywersyfikacja
                noImprove = 0;
            }
        }
        bestRoute[numCities] = 0;

        delete[] currentRoute;
        return bestRoute;
};

int TabuSearch::getNumCities()
{
    return numCities;
}

void TabuSearch::saveResultToFile(const std::string& filename, int* route, int numCities) {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Nie można otworzyć pliku do zapisu: " + filename);
    }

    for (int i = 0; i < numCities + 1; ++i) {
        file << route[i] << " ";
    }
    file.close();
}

int TabuSearch::calculateCostFromFile(const std::string& filename, int** distanceMatrix, int numCities) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Nie można otworzyć pliku do odczytu: " + filename);
    }

    // Wczytaj trasę z pliku
    int* route = new int[numCities + 1];
    for (int i = 0; i < numCities + 1; ++i) {
        file >> route[i];
    }
    file.close();

    // Oblicz koszt trasy
    int totalCost = 0;
    for (int i = 0; i < numCities; ++i) {
        totalCost += distanceMatrix[route[i]][route[i + 1]];
    }

    delete[] route; // Zwolnij pamięć
    return totalCost;
}

int** TabuSearch::getDistanceMatrix()
{
    return distanceMatrix;
}

