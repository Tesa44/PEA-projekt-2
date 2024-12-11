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
        for (int i = 0; i < numCities - 1; ++i) {
            cost += distanceMatrix[route[i]][route[i + 1]];
        }
        cost += distanceMatrix[route[numCities - 1]][route[0]];
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

int TabuSearch::swapNeighborhood(int* currentRoute, int* bestNeighbor) {
    int bestCost = INT_MAX;
    int* tempRoute = new int[numCities];

    for (int i = 1; i < numCities - 1; ++i) {
        for (int j = i + 1; j < numCities; ++j) {
            std::copy(currentRoute, currentRoute + numCities, tempRoute);
            std::swap(tempRoute[i], tempRoute[j]); // Zamiana miast

            int cost = calculateCost(tempRoute);

            if ((tabuList[i][j] == 0 || cost < bestCost) && cost < bestCost) {
                bestCost = cost;
                std::copy(tempRoute, tempRoute + numCities, bestNeighbor);
            }
        }
    }

    delete[] tempRoute;
    return bestCost;
}

int TabuSearch::insertNeighborhood(int* currentRoute, int* bestNeighbor) {
    int bestCost = INT_MAX;
    int* tempRoute = new int[numCities];

    for (int i = 1; i < numCities; ++i) {
        for (int j = 0; j < numCities; ++j) {
            if (i == j) continue;

            std::copy(currentRoute, currentRoute + numCities, tempRoute);

            int temp = tempRoute[i];
            if (i < j) {
                for (int k = i; k < j; ++k) tempRoute[k] = tempRoute[k + 1];
            } else {
                for (int k = i; k > j; --k) tempRoute[k] = tempRoute[k - 1];
            }
            tempRoute[j] = temp;

            int cost = calculateCost(tempRoute);

            if ((tabuList[i][j] == 0 || cost < bestCost) && cost < bestCost) {
                bestCost = cost;
                std::copy(tempRoute, tempRoute + numCities, bestNeighbor);
            }
        }
    }

    delete[] tempRoute;
    return bestCost;
}

// int TabuSearch::reverseNeighborhood(int* currentRoute, int* bestNeighbor) {
//     int bestCost = INT_MAX;
//     int* tempRoute = new int[numCities];
//
//     for (int i = 1; i < numCities - 2; ++i) {
//         for (int j = i + 2; j < numCities; ++j) {
//             std::copy(currentRoute, currentRoute + numCities, tempRoute);
//
//             // Odwracanie fragmentu trasy
//             std::reverse(tempRoute + i, tempRoute + j + 1);
//
//             int cost = calculateCost(tempRoute);
//
//             // Uwzględnienie listy tabu i poprawienie warunków wyboru najlepszego sąsiedztwa
//             if ((tabuList[i][j] == 0 || cost < bestCost) && cost < bestCost) {
//                 bestCost = cost;
//                 std::copy(tempRoute, tempRoute + numCities, bestNeighbor);
//             }
//         }
//     }
//
//     delete[] tempRoute;
//     return bestCost;
// }

int TabuSearch::kSwapNeighborhood(int* currentRoute, int* bestNeighbor)
{
    int bestCost = INT_MAX;
    int* tempRoute = new int[numCities];

    for (int i = 1; i < numCities - 2; ++i) {
        for (int j = i + 1; j < numCities - 1; ++j) {
            for (int k = j + 1; k < numCities; ++k) {
                std::copy(currentRoute, currentRoute + numCities, tempRoute);

                // Przeprowadź zamianę pozycji trzech miast
                std::swap(tempRoute[i], tempRoute[j]);
                std::swap(tempRoute[j], tempRoute[k]);

                int cost = calculateCost(tempRoute);

                if ((tabuList[i][j] == 0 || cost < bestCost) && cost < bestCost) {
                    bestCost = cost;
                    std::copy(tempRoute, tempRoute + numCities, bestNeighbor);
                }
            }
        }
    }
    delete[] tempRoute;
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
        neighborhoodFunc = &TabuSearch::swapNeighborhood;
        break;
    case 2:
        neighborhoodFunc = &TabuSearch::insertNeighborhood;
        break;
    case 3:
        neighborhoodFunc = &TabuSearch::kSwapNeighborhood;
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

    int* bestRoute = new int[numCities + 1];
    // int* currentRoute = new int[numCities];
    int* currentRoute = generateGreedyRoute();
    std::copy(currentRoute, currentRoute + numCities, bestRoute);

    int bestCost = calculateCost(bestRoute);

    auto startTime = std::chrono::steady_clock::now();
    int noImprove = 0;

    while (true) {
        auto currentTime = std::chrono::steady_clock::now();
        double elapsedTime = std::chrono::duration<double>(currentTime - startTime).count();
        if (elapsedTime > timeLimit) break;

        int* bestNeighbor = new int[numCities];
        int neighborCost = (this->*neighborhoodFunc)(currentRoute, bestNeighbor);

        if (neighborCost < bestCost) {
            bestCost = neighborCost;
            std::copy(bestNeighbor, bestNeighbor + numCities, bestRoute);
            noImprove = 0;
            bestFindTime = elapsedTime;
            std::cout << elapsedTime << " [s] " << bestCost << std::endl;
        } else {
            noImprove++;
        }
        // std::cout << noImprove << " improves" << std::endl;
        // Aktualizacja listy tabu
        for (int i = 0; i < numCities; ++i) {
            for (int j = 0; j < numCities; ++j) {
                if (tabuList[i][j] > 0) tabuList[i][j]--;
            }
        }

        // Oznaczenie ostatniego ruchu jako tabu
        for (int i = 1; i < numCities - 1; ++i) {
            for (int j = i + 1; j < numCities; ++j) {
                if (currentRoute[i] != bestNeighbor[i] || currentRoute[j] != bestNeighbor[j]) {
                    tabuList[i][j] = tabuTenure;
                }
            }
        }

        std::copy(bestNeighbor, bestNeighbor + numCities, currentRoute);

        // Dywersyfikacja w przypadku braku poprawy
        if (noImprove >= maxNoImprove) {
            perturbRoute(currentRoute); // Modyfikujemy trasę zamiast losować nową
            // std::cout << "perturbRoute" << std::endl;
            noImprove = 0;
        }

        delete[] bestNeighbor;
    }

    bestRoute[numCities] = bestRoute[0]; // Powrót do pierwszego miasta
    std::cout << bestFindTime << " [s]" << std::endl;
    delete[] currentRoute;
    return bestRoute;
}

void TabuSearch::perturbRoute(int* route) {
    // Wprowadzenie subtelnych zmian w trasie
    int numPerturbations = numCities / 10; // Zmieniamy 10% trasy
    for (int k = 0; k < numPerturbations; ++k) {
        int i = rand() % (numCities - 1) + 1; // Pomijamy miasto startowe
        int j = rand() % (numCities - 1) + 1;
        if (i != j) {
            std::swap(route[i], route[j]); // Zamiana dwóch losowych miast
        }
    }
}

// int* TabuSearch::solve() {
//     tabuList = new int*[numCities];
//     for (int i = 0; i < numCities; ++i) {
//         tabuList[i] = new int[numCities]();
//     }
//
//     int* currentRoute = new int[numCities];
//     int* bestRoute = new int[numCities + 1];
//     //generateRandomRoute(currentRoute);
//     currentRoute = generateGreedyRoute();
//     std::copy(currentRoute, currentRoute + numCities, bestRoute);
//     int bestCost = calculateCost(bestRoute);
//
//     auto startTime = std::chrono::steady_clock::now();
//     int noImprove = 0;
//
//     while (true) {
//         auto currentTime = std::chrono::steady_clock::now();
//         double elapsedTime = std::chrono::duration<double>(currentTime - startTime).count();
//         if (elapsedTime > timeLimit) break;
//
//         int* bestNeighbor = new int[numCities];
//         int bestNeighborCost = INT_MAX;
//
//         for (int i = 1; i < numCities - 1; ++i) {
//             for (int j = i + 1; j < numCities; ++j) {
//                 if (tabuList[i][j] > 0) continue; // Pomijanie ruchów tabu
//
//                 int* tempNeighbor = new int[numCities];
//                 std::copy(currentRoute, currentRoute + numCities, tempNeighbor);
//                 std::swap(tempNeighbor[i], tempNeighbor[j]);
//
//                 int neighborCost = calculateCost(tempNeighbor);
//
//                 // Aspiracja - jeśli rozwiązanie lepsze niż dotychczasowe najlepsze
//                 if (neighborCost < bestCost || tabuList[i][j] == 0) {
//                     if (neighborCost < bestNeighborCost) {
//                         bestNeighborCost = neighborCost;
//                         std::copy(tempNeighbor, tempNeighbor + numCities, bestNeighbor);
//
//                     }
//                 }
//                 delete[] tempNeighbor;
//             }
//         }
//
//         // Aktualizacja najlepszej trasy
//         if (bestNeighborCost < bestCost) {
//             bestCost = bestNeighborCost;
//             std::copy(bestNeighbor, bestNeighbor + numCities, bestRoute);
//             noImprove = 0;
//             std::cout << bestNeighborCost << std::endl;
//             bestFindTime = elapsedTime * 10000;
//         } else {
//             noImprove++;
//         }
//
//         // Aktualizacja listy tabu
//         for (int i = 0; i < numCities; ++i) {
//             for (int j = 0; j < numCities; ++j) {
//                 if (tabuList[i][j] > 0) tabuList[i][j]--;
//             }
//         }
//
//         // Oznaczenie ostatniego ruchu jako tabu
//         for (int i = 1; i < numCities - 1; ++i) {
//             for (int j = i + 1; j < numCities; ++j) {
//                 if (currentRoute[i] != bestNeighbor[i] || currentRoute[j] != bestNeighbor[j]) {
//                     tabuList[i][j] = tabuTenure;
//                 }
//             }
//         }
//
//         std::copy(bestNeighbor, bestNeighbor + numCities, currentRoute);
//
//         // Dywersyfikacja w przypadku braku poprawy
//         if (noImprove >= maxNoImprove) {
//             generateRandomRoute(currentRoute);
//             noImprove = 0;
//         }
//
//         delete[] bestNeighbor;
//     }
//
//     bestRoute[numCities] = bestRoute[0]; // Powrót do pierwszego miasta
//     std::cout << bestFindTime << " [s]" <<std::endl;
//     delete[] currentRoute;
//     return bestRoute;
// };

void TabuSearch::twoOptSwap(int* route, int i, int j) {
    while (i < j) {
        std::swap(route[i], route[j]);
        i++;
        j--;
    }
}

// int* TabuSearch::solve() {
//     tabuList = new int*[numCities];
//     for (int i = 0; i < numCities; ++i) {
//         tabuList[i] = new int[numCities]();
//     }
//     int* bestRoute = new int[numCities + 1];
//     int* currentRoute = generateGreedyRoute();
//     std::copy(currentRoute, currentRoute + numCities, bestRoute);
//     int bestCost = calculateCost(bestRoute);
//
//     auto startTime = std::chrono::steady_clock::now();
//     int noImprove = 0;
//
//     while (true) {
//         auto currentTime = std::chrono::steady_clock::now();
//         double elapsedTime = std::chrono::duration<double>(currentTime - startTime).count();
//         if (elapsedTime > timeLimit) break;
//
//         int* bestNeighbor = new int[numCities];
//         int bestNeighborCost = INT_MAX;
//
//         for (int i = 1; i < numCities - 1; ++i) {
//             for (int j = i + 1; j < numCities; ++j) {
//
//                 int* tempNeighbor = new int[numCities];
//                 std::copy(currentRoute, currentRoute + numCities, tempNeighbor);
//                 std::swap(tempNeighbor[i], tempNeighbor[j]);
//
//                 int neighborCost = calculateCost(tempNeighbor);
//                 // Aspiracja - jeśli rozwiązanie lepsze niż dotychczasowe najlepsze
//                 if (neighborCost < bestCost || tabuList[i][j] == 0) {
//                     if (neighborCost < bestNeighborCost) {
//                         bestNeighborCost = neighborCost;
//                         std::copy(tempNeighbor, tempNeighbor + numCities, bestNeighbor);
//                     }
//                 }
//                 delete[] tempNeighbor;
//             }
//         }
//
//         // Aktualizacja najlepszej trasy
//         if (bestNeighborCost < bestCost) {
//             bestCost = bestNeighborCost;
//             std::copy(bestNeighbor, bestNeighbor + numCities, bestRoute);
//             noImprove = 0;
//             bestFindTime = elapsedTime;
//             std::cout << elapsedTime <<" [s]   " << bestCost<< std::endl;
//         } else {
//             noImprove++;
//         }
//
//         // Aktualizacja listy tabu
//         for (int i = 0; i < numCities; ++i) {
//             for (int j = 0; j < numCities; ++j) {
//                 if (tabuList[i][j] > 0) tabuList[i][j]--;
//             }
//         }
//
//         // Oznaczenie ostatniego ruchu jako tabu
//         for (int i = 1; i < numCities - 1; ++i) {
//             for (int j = i + 1; j < numCities; ++j) {
//                 if (currentRoute[i] != bestNeighbor[i] || currentRoute[j] != bestNeighbor[j]) {
//                     tabuList[i][j] = tabuTenure;
//                 }
//             }
//         }
//
//         std::copy(bestNeighbor, bestNeighbor + numCities, currentRoute);
//
//         // Dywersyfikacja w przypadku braku poprawy
//         if (noImprove >= maxNoImprove) {
//             perturbRoute(currentRoute); // Modyfikujemy trasę zamiast losować nową
//             noImprove = 0;
//         }
//
//         delete[] bestNeighbor;
//     }
//
//     bestRoute[numCities] = bestRoute[0]; // Powrót do pierwszego miasta
//     std::cout << bestFindTime << " [s]" << std::endl;
//     //std::cout << "z programu: " << bestCost << std::endl;
//     delete[] currentRoute;
//     // std::cout << "[ ";
//     // for (int i = 0; i < numCities + 1; ++i)
//     // {
//     //     std::cout << bestRoute[i] << " " << std::endl;
//     // }
//     // std::cout << " ]" << std::endl;
//     return bestRoute;
// }


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

void TabuSearch::setTenure(int newTenure)
{
    tabuTenure = newTenure;
}

void TabuSearch::setTimeLimit(double newTimeLimit)
{
    timeLimit = newTimeLimit;
}

int* TabuSearch::generateGreedyRoute() {
    int* bestRoute = new int[numCities+1];
    int bestCost = INT_MAX;

    for (int startCity = 0; startCity < numCities; ++startCity) {
        // Tymczasowa tablica na aktualną trasy
        int* tempRoute = new int[numCities+1];
        bool* visited = new bool[numCities]{false};

        // Inicjalizacja ścieżki
        tempRoute[0] = startCity;
        visited[startCity] = true;

        // Budowanie trasy zachłannej
        for (int i = 1; i < numCities; ++i) {
            int lastCity = tempRoute[i - 1];
            int nearestCity = -1;
            int minDistance = INT_MAX;

            for (int j = 0; j < numCities; ++j) {
                if (!visited[j] && distanceMatrix[lastCity][j] < minDistance) {
                    minDistance = distanceMatrix[lastCity][j];
                    nearestCity = j;
                }
            }

            tempRoute[i] = nearestCity;
            visited[nearestCity] = true;
        }
        //Droga powrotna
        tempRoute[numCities] = startCity;
        // Obliczenie kosztu trasy
        int tempCost = calculateCost(tempRoute);

        // Aktualizacja najlepszego rozwiązania
        if (tempCost < bestCost) {
            bestCost = tempCost;
            std::copy(tempRoute, tempRoute + numCities+1, bestRoute);
        }

        delete[] tempRoute;
        delete[] visited;
    }

    //saveResultToFile("wynik.txt", bestRoute, numCities);
    //std::cout << calculateCostFromFile("wynik.txt",distanceMatrix, numCities) << std::endl;
     //Przypisanie najlepszego rozwiązania do finalnej trasy
    //std::copy(bestRoute, bestRoute + numCities, route);
    //delete[] bestRoute;
    return bestRoute;
}
