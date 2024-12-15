
#include "SimAnnealing.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <chrono>
#include <random>
#include <algorithm>
using namespace std;

// void SimAnnealing::loadFromFile(const std::string &filename) {
//     ifstream file(filename);
//
//     if(!file.good()) {
//         cout << endl << "W projekcie nie ma pliku o takiej nazwie." << endl;
//         file.close();
//     }
//
//     std::string line;
//     numCities = 0;
//
//     while (std::getline(file, line)) {
//         if (line.find("DIMENSION") != std::string::npos) {
//             // Zczytanie rozmiaru macierzy z linijki z "DIMENSION"
//             numCities = std::stoi(line.substr(line.find(":") + 1));
//         }
//         if (line.find("EDGE_WEIGHT_SECTION") != std::string::npos) {
//             break;
//         }
//     }
//
//     if (numCities == 0) {
//         std::cerr << "Error: Matrix size not found in file" << std::endl;
//         return;
//     }
//
//     // Allocate dynamic 2D array
//     distanceMatrix = new int*[numCities];
//     for (int i = 0; i < numCities; ++i) {
//         distanceMatrix[i] = new int[numCities];
//     }
//
//     // Read the matrix values
//     int row = 0, col = 0;
//     while (std::getline(file, line) && row < numCities) {
//         std::istringstream stream(line);
//         int value;
//         while (stream >> value) {
//             distanceMatrix[row][col++] = value;
//             if (col == numCities) {
//                 col = 0;
//                 row++;
//             }
//         }
//     }
//
//     file.close();
//
//     // Wyswietlenie macierzy
//     cout << endl << "Macierz kosztow:" << endl;
//     for (int i = 0; i < numCities; i++) {
//         for (int j = 0; j < numCities; j++) {
//             int number = distanceMatrix[i][j];
//             cout << number << " ";
//             if (number < 100) cout << " ";
//             if (number > 0 && number < 10) cout << " ";
//         }
//         cout << endl;
//     }
// }

void SimAnnealing::loadFromFile(const std::string& filename) {
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


int SimAnnealing::calculateCost(int* path) {
    int cost = 0;

    for (int i = 0; i < numCities - 1; i++) {
        cost += distanceMatrix[path[i]][path[i + 1]];
    }
    cost += distanceMatrix[path[numCities - 1]][path[0]];

    return cost;
}

void SimAnnealing::getRandomNeighbor(int* currentRoute,int* neighborRoute, std::mt19937& gen) {
    std::copy(currentRoute, currentRoute + numCities, neighborRoute);
    std::uniform_int_distribution<> dist(0, numCities - 1);
    int i = dist(gen);
    int j = dist(gen);
    while (i == j) {
        j = dist(gen);
    }

    // Przesunięcie elementów
    swapElements(neighborRoute, numCities, i, j);
}

void SimAnnealing::swapElements(int* neighbor, int numCities, int i, int j) {
    if (i < j) {
        // Przenoszenie elementu `i` na pozycję `j` w prawo
        int temp = neighbor[i];
        for (int k = i; k < j; ++k) {
            neighbor[k] = neighbor[k + 1];
        }
        neighbor[j] = temp;
    } else if (i > j) {
        // Przenoszenie elementu `i` na pozycję `j` w lewo
        int temp = neighbor[i];
        for (int k = i; k > j; --k) {
            neighbor[k] = neighbor[k - 1];
        }
        neighbor[j] = temp;
    }
}

int* SimAnnealing::solve() {

    int* bestRoute = new int[numCities+1];
    int* currentSolution = new int[numCities];

    std::iota(currentSolution, currentSolution + numCities, 0);
    std::random_device rd;
    std::mt19937 gen(rd());

    shuffle(currentSolution, currentSolution + numCities, gen);
    copy(currentSolution,currentSolution + numCities,bestRoute);

    int bestCost = calculateCost(bestRoute);
    currentCost = bestCost;

    calculateInitialTemperature();

    auto startTime = std::chrono::steady_clock::now();
    maxIter = numCities * numCities;
    int iteration = 0;
    double elapsedTime = 0;
    while (elapsedTime <= timeLimit && temperature > 1e-8) {
        auto currentTime = std::chrono::steady_clock::now();
        elapsedTime = std::chrono::duration<double>(currentTime - startTime).count();

        for (int l = 0; l < maxIter; l++) {
            int* neighbor = new int[numCities];

            // neighbor - znalezienie rozwiazania w sasiedztwie currentSolution
            getRandomNeighbor(currentSolution, neighbor, gen);
            int neighborCost = calculateCost(neighbor);

            // Wyznaczenie prawdopodobienstwa zaakceptowania rozwiazania
            double delta = neighborCost - currentCost;
            double acceptanceProbability = delta < 0 ? 1.0 : std::exp(-delta / temperature);

            // Akceptacja nowego rozwiazania, jesli neighborCost < currentCost lub na podstawie prawdopodobienstwa
            if (std::generate_canonical<double, 10>(gen) < acceptanceProbability) {
                copy(neighbor, neighbor + numCities, currentSolution);
                currentCost = neighborCost;
            }

            // Aktualizacja najlepszego rozwiazania
            if (currentCost < bestCost) {
                copy(currentSolution, currentSolution + numCities, bestRoute);
                bestRoute[numCities] = bestRoute[0];
                bestCost = currentCost;
                bestFindTime = elapsedTime;
            }
            delete[] neighbor;
        }

        // Aktualizacja parametru kontrolnego T
        switch (coolingScheme) {
            case 1:
                temperature *= coolingRate;
                break;
            case 2:
                temperature /= (1 + 0.001 * temperature);
                break;
            case 3:
                temperature = temperature / std::log(iteration + 2);
                break;
        }
        iteration++;

    }

    cout << endl << "Wartosc koncowa parametru kontrolnego Tk: " << temperature << endl;
    cout << "Wartosc wyrazenia exp(-1/Tk): " << std::exp(-1 / temperature) << endl;
    cout << "Czas znalezienia najlepszego rozwiazania: " << bestFindTime << " sekund" << endl;

    delete[] currentSolution;

    return bestRoute;
}

void SimAnnealing::setCoolingScheme(int scheme) {
    coolingScheme = scheme;
}


void SimAnnealing::setTimeLimit(double newTimeLimit) {
    timeLimit = newTimeLimit;
}

void SimAnnealing::setCoolingRate(double newCoolingRate) {
    coolingRate = newCoolingRate;
}

void SimAnnealing::setDistanceMatrix(int **newDistanceMatrix) {
    distanceMatrix = newDistanceMatrix;
}

void SimAnnealing::setNumCities(int newNumCities) {
    numCities = newNumCities;
}

void SimAnnealing::calculateInitialTemperature() {
    int maxCost = 0;
    for (int i = 0; i < numCities; i++) {
        for (int j = 0; j < numCities; j++) {
            if (i != j) {
                maxCost = std::max(maxCost, distanceMatrix[i][j]);
            }
        }
    }
    temperature = maxCost;
    if (maxCost < numCities)
    {
        temperature = numCities*2;
    }

}
