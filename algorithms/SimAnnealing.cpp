//
// Created by Mateusz on 28.11.2024.
//

#include "SimAnnealing.h"


#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>
#include <limits>
#include <vector>
#include <chrono> // Do pomiaru czasu

SimAnnealing::SimAnnealing(int coolingScheme, double coolingRate, double timeLimit)
    : coolingScheme(coolingScheme), coolingRate(coolingRate), timeLimit(timeLimit),
      distanceMatrix(nullptr), currentRoute(nullptr), bestRoute(nullptr),
      numCities(0), temperature(0.0), currentCost(std::numeric_limits<int>::max()),
      bestCost(std::numeric_limits<int>::max()) {
    // Konstruktor z parametrami
    if (coolingScheme < 1 || coolingScheme > 3) {
        throw std::invalid_argument("Nieprawidłowy schemat schładzania: wybierz 1, 2 lub 3.");
    }
    if (coolingRate <= 0.0 || coolingRate >= 1.0) {
        throw std::invalid_argument("Współczynnik schładzania powinien być w zakresie (0, 1).");
    }
    if (timeLimit <= 0.0) {
        throw std::invalid_argument("Limit czasu musi być większy od zera.");
    }
};


void SimAnnealing::loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            throw std::runtime_error("Nie można otworzyć pliku: " + filename);
        }

        std::string line;
        bool isEdgeWeightSection = false;
        int valueCount = 0;

        while (std::getline(file, line)) {
            line.erase(0, line.find_first_not_of(" \t\n\r"));
            line.erase(line.find_last_not_of(" \t\n\r") + 1);

            if (line.empty() || line[0] == '#') continue;

            if (line.find("DIMENSION") != std::string::npos) {
                std::string temp;
                std::istringstream iss(line);
                iss >> temp;       // DIMENSION
                iss >> numCities;  // Liczba miast
            } else if (line.find("EDGE_WEIGHT_SECTION") != std::string::npos) {
                isEdgeWeightSection = true;
                distanceMatrix = new int*[numCities];
                for (int i = 0; i < numCities; ++i) {
                    distanceMatrix[i] = new int[numCities];
                }
            } else if (isEdgeWeightSection) {
                std::istringstream iss(line);
                int value;
                while (iss >> value) {
                    int row = valueCount / numCities;
                    int col = valueCount % numCities;
                    distanceMatrix[row][col] = value;
                    valueCount++;
                }
            }
        }

        if (valueCount != numCities * numCities) {
            throw std::runtime_error("Niepoprawny format pliku: niekompletna macierz odległości");
        }

        currentRoute = new int[numCities];
        bestRoute = new int[numCities];
}

void SimAnnealing::initializeRoute() {
        for (int i = 0; i < numCities; ++i) {
            currentRoute[i] = i;
        }

        for (int i = 0; i < numCities; ++i) {
            int j = rand() % numCities;
            std::swap(currentRoute[i], currentRoute[j]);
        }

        std::copy(currentRoute, currentRoute + numCities, bestRoute);
        currentCost = calculateCost(currentRoute);
        bestCost = currentCost;
}

void SimAnnealing::calculateInitialTemperature() {
        int* route1 = new int[numCities];
        int* route2 = new int[numCities];

        for (int i = 0; i < numCities; ++i) {
            route1[i] = i;
            route2[i] = i;
        }

        for (int i = 0; i < numCities; ++i) {
            std::swap(route1[i], route1[rand() % numCities]);
            std::swap(route2[i], route2[rand() % numCities]);
        }

        int cost1 = calculateCost(route1);
        int cost2 = calculateCost(route2);

        double deltaCost = std::abs(cost1 - cost2);
        temperature = -deltaCost / std::log(0.8);
        std::cout << "temperatura początkowa: " << temperature << std::endl;
        delete[] route1;
        delete[] route2;
    }

    int SimAnnealing::calculateCost(int* route) {
        int cost = 0;
        for (int i = 0; i < numCities - 1; ++i) {
            cost += distanceMatrix[route[i]][route[i + 1]];
        }
        cost += distanceMatrix[route[numCities - 1]][route[0]];
        return cost;
}

void SimAnnealing::perturbRoute(int* route) {
        int i = rand() % numCities;
        int j = rand() % numCities;
        while (i == j) {
            j = rand() % numCities;
        }
        std::swap(route[i], route[j]);
    }


void SimAnnealing::run() {
    initializeRoute();
    calculateInitialTemperature();

    auto startTime = std::chrono::steady_clock::now();
    int iteration = 0;

    while (temperature > 0.01) {
        auto currentTime = std::chrono::steady_clock::now();
        double elapsedTime = std::chrono::duration<double>(currentTime - startTime).count();
        if (elapsedTime >= timeLimit) {
            std::cout << "Osiągnięto limit czasu: " << timeLimit << " sekund.\n";
            break;
        }

        int* newRoute = new int[numCities];
        std::copy(currentRoute, currentRoute + numCities, newRoute);

        perturbRoute(newRoute);
        int newCost = calculateCost(newRoute);

        double acceptanceProbability = exp((currentCost - newCost) / temperature);

        if (newCost < currentCost || (rand() / double(RAND_MAX)) < acceptanceProbability) {
            std::copy(newRoute, newRoute + numCities, currentRoute);
            currentCost = newCost;

            if (newCost < bestCost) {
                std::copy(newRoute, newRoute + numCities, bestRoute);
                bestCost = newCost;
            }
        }

        // Aktualizacja temperatury
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

        delete[] newRoute;
        iteration++;
    }

    std::cout << "Ostateczna temperatura: " << temperature << "\n";
    std::cout << "Wartość exp(-1/Tk): " << exp(-1 / temperature) << "\n";
    std::cout << "Najlepszy koszt: " << bestCost << "\n";
}




void SimAnnealing::setCoolingScheme(int scheme) {
        coolingScheme = scheme;
    }

void SimAnnealing::saveResultToFile(const std::string& filename) {
        std::ofstream file(filename);
        if (!file) {
            throw std::runtime_error("Nie można otworzyć pliku do zapisu: " + filename);
        }

        for (int i = 0; i < numCities; ++i) {
            file << bestRoute[i] << " ";
        }
        file << bestRoute[0] << "\n";
        file.close();
}

