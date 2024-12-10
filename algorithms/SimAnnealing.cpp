// //
// // Created by Mateusz on 28.11.2024.
// //
//
// #include "SimAnnealing.h"
//
//
// #include <iostream>
// #include <fstream>
// #include <sstream>
// #include <cmath>
// #include <cstdlib>
// #include <ctime>
// #include <string>
// #include <limits>
// #include <vector>
// #include <chrono> // Do pomiaru czasu
//
// // SimAnnealing::SimAnnealing(int coolingScheme, double coolingRate, double timeLimit)
// //     : coolingScheme(coolingScheme), coolingRate(coolingRate), timeLimit(timeLimit),
// //       distanceMatrix(nullptr), currentRoute(nullptr), bestRoute(nullptr),
// //       numCities(0), temperature(0.0), currentCost(std::numeric_limits<int>::max()),
// //       bestCost(std::numeric_limits<int>::max()) {
// //     // Konstruktor z parametrami
// //     if (coolingScheme < 1 || coolingScheme > 3) {
// //         throw std::invalid_argument("Nieprawidłowy schemat schładzania: wybierz 1, 2 lub 3.");
// //     }
// //     if (coolingRate <= 0.0 || coolingRate >= 1.0) {
// //         throw std::invalid_argument("Współczynnik schładzania powinien być w zakresie (0, 1).");
// //     }
// //     if (timeLimit <= 0.0) {
// //         throw std::invalid_argument("Limit czasu musi być większy od zera.");
// //     }
// // };
//
//
// void SimAnnealing::loadFromFile(const std::string& filename) {
//         std::ifstream file(filename);
//         if (!file) {
//             throw std::runtime_error("Nie można otworzyć pliku: " + filename);
//         }
//
//         std::string line;
//         bool isEdgeWeightSection = false;
//         int valueCount = 0;
//
//         while (std::getline(file, line)) {
//             line.erase(0, line.find_first_not_of(" \t\n\r"));
//             line.erase(line.find_last_not_of(" \t\n\r") + 1);
//
//             if (line.empty() || line[0] == '#') continue;
//
//             if (line.find("DIMENSION") != std::string::npos) {
//                 std::string temp;
//                 std::istringstream iss(line);
//                 iss >> temp;       // DIMENSION
//                 iss >> numCities;  // Liczba miast
//             } else if (line.find("EDGE_WEIGHT_SECTION") != std::string::npos) {
//                 isEdgeWeightSection = true;
//                 distanceMatrix = new int*[numCities];
//                 for (int i = 0; i < numCities; ++i) {
//                     distanceMatrix[i] = new int[numCities];
//                 }
//             } else if (isEdgeWeightSection) {
//                 std::istringstream iss(line);
//                 int value;
//                 while (iss >> value) {
//                     int row = valueCount / numCities;
//                     int col = valueCount % numCities;
//                     distanceMatrix[row][col] = value;
//                     valueCount++;
//                 }
//             }
//         }
//
//         if (valueCount != numCities * numCities) {
//             throw std::runtime_error("Niepoprawny format pliku: niekompletna macierz odległości");
//         }
//
//         currentRoute = new int[numCities];
//         bestRoute = new int[numCities];
// }
//
// void SimAnnealing::initializeRoute() {
//         for (int i = 0; i < numCities; ++i) {
//             currentRoute[i] = i;
//         }
//
//         for (int i = 0; i < numCities; ++i) {
//             int j = rand() % numCities;
//             std::swap(currentRoute[i], currentRoute[j]);
//         }
//
//         std::copy(currentRoute, currentRoute + numCities, bestRoute);
//         currentCost = calculateCost(currentRoute);
//         bestCost = currentCost;
// }
//
// void SimAnnealing::calculateInitialTemperature() {
//         int* route1 = new int[numCities];
//         int* route2 = new int[numCities];
//
//         for (int i = 0; i < numCities; ++i) {
//             route1[i] = i;
//             route2[i] = i;
//         }
//
//         for (int i = 0; i < numCities; ++i) {
//             std::swap(route1[i], route1[rand() % numCities]);
//             std::swap(route2[i], route2[rand() % numCities]);
//         }
//
//         int cost1 = calculateCost(route1);
//         int cost2 = calculateCost(route2);
//         // perturbRoute(route1);
//         // int cost2 = calculateCost(route1);
//
//         double deltaCost = std::abs(cost1 - cost2);
//         temperature = -deltaCost / std::log(0.8);
//         //temperature = 10 * (-deltaCost / std::log(0.8));
//         std::cout << "temperatura początkowa: " << temperature << std::endl;
//         delete[] route1;
//         delete[] route2;
//     }
//
//     int SimAnnealing::calculateCost(int* route) {
//         int cost = 0;
//         for (int i = 0; i < numCities - 1; ++i) {
//             cost += distanceMatrix[route[i]][route[i + 1]];
//         }
//         cost += distanceMatrix[route[numCities - 1]][route[0]];
//         return cost;
// }
//
// void SimAnnealing::perturbRoute(int* route) {
//         int i = rand() % numCities;
//         int j = rand() % numCities;
//         while (i == j) {
//             j = rand() % numCities;
//         }
//         std::swap(route[i], route[j]);
//
//         // Losowe odwrócenie fragmentu trasy (2-opt)
//         // int i = rand() % numCities;
//         // int j = rand() % numCities;
//         // if (i > j) std::swap(i, j); // Upewnij się, że i < j
//         // std::reverse(route + i, route + j + 1);
// }
//
//
//
//
// // void SimAnnealing::solve() {
// //     initializeRoute();
// //     calculateInitialTemperature();
// //
// //     auto startTime = std::chrono::steady_clock::now();
// //     int iteration = 0;
// //     double elapsedTime = 0.0;
// //     while ( elapsedTime <= timeLimit) {
// //         auto currentTime = std::chrono::steady_clock::now();
// //         elapsedTime = std::chrono::duration<double>(currentTime - startTime).count();
// //
// //         int* newRoute = new int[numCities];
// //         std::copy(currentRoute, currentRoute + numCities, newRoute);
// //
// //         perturbRoute(newRoute);
// //         int newCost = calculateCost(newRoute);
// //
// //         double acceptanceProbability = exp((currentCost - newCost) / temperature);
// //
// //         if (newCost < currentCost || (rand() / double(RAND_MAX)) < acceptanceProbability) {
// //             std::copy(newRoute, newRoute + numCities, currentRoute);
// //             currentCost = newCost;
// //
// //             if (newCost < bestCost) {
// //                 std::copy(newRoute, newRoute + numCities, bestRoute);
// //                 bestCost = newCost;
// //             }
// //         }
// //
// //         // Aktualizacja temperatury
// //         switch (coolingScheme) {
// //         case 1:
// //             temperature *= coolingRate;
// //             break;
// //         case 2:
// //             temperature /= (1 + 0.001 * temperature);
// //             break;
// //         case 3:
// //             temperature = temperature / std::log(iteration + 2);
// //             break;
// //         }
// //
// //         delete[] newRoute;
// //         iteration++;
// //     }
// //
// //     std::cout << "Ostateczna temperatura: " << temperature << "\n";
// //     std::cout << "Wartość exp(-1/Tk): " << exp(-1 / temperature) << "\n";
// //     std::cout << "Najlepszy koszt: " << bestCost << "\n";
// // }
// void SimAnnealing::solve() {
//     initializeRoute();
//     calculateInitialTemperature();
//
//     auto startTime = std::chrono::steady_clock::now();
//     int iteration = 0;
//     double elapsedTime = 0.0;
//
//     while (elapsedTime <= timeLimit) {
//         auto currentTime = std::chrono::steady_clock::now();
//         elapsedTime = std::chrono::duration<double>(currentTime - startTime).count();
//
//         int* newRoute = new int[numCities];
//         std::copy(currentRoute, currentRoute + numCities, newRoute);
//
//         perturbRoute(newRoute);
//         int newCost = calculateCost(newRoute);
//
//         double acceptanceProbability = exp((currentCost - newCost) / temperature);
//             // std::cout << acceptanceProbability <<"   "<<temperature << std::endl;
//         if (newCost < currentCost || (rand() / double(RAND_MAX)) < acceptanceProbability) {
//             std::copy(newRoute, newRoute + numCities, currentRoute);
//             currentCost = newCost;
//
//             if (newCost < bestCost) {
//                 std::copy(newRoute, newRoute + numCities, bestRoute);
//                 bestCost = newCost;
//             }
//         }
//
//         // *** Mechanizm dywersyfikacji ***
//         if (iteration % maxIter == 0 && currentCost == bestCost) {
//             perturbRoute(currentRoute); // Wprowadzenie losowej zmiany w trasie
//             currentCost = calculateCost(currentRoute); // Przeliczenie kosztu dla nowej trasy
//         }
//
//         // Aktualizacja temperatury
//         switch (coolingScheme) {
//         case 1:
//             temperature *= coolingRate;
//             break;
//         case 2:
//             temperature /= (1 + 0.001 * temperature);
//             break;
//         case 3:
//             temperature = temperature / std::log(iteration + 2);
//             break;
//         }
//
//         delete[] newRoute;
//         iteration++;
//     }
//
//     std::cout << "Ostateczna temperatura: " << temperature << "\n";
//     std::cout << "Wartość exp(-1/Tk): " << exp(-1 / temperature) << "\n";
//     std::cout << "Najlepszy koszt: " << bestCost << "\n";
// }
//
//
//
//
// void SimAnnealing::setCoolingScheme(int scheme) {
//         coolingScheme = scheme;
//     }
//
// void SimAnnealing::saveResultToFile(const std::string& filename) {
//         std::ofstream file(filename);
//         if (!file) {
//             throw std::runtime_error("Nie można otworzyć pliku do zapisu: " + filename);
//         }
//
//         for (int i = 0; i < numCities; ++i) {
//             file << bestRoute[i] << " ";
//         }
//         file << bestRoute[0] << "\n";
//         file.close();
// }
//
// void SimAnnealing::setTimeLimit(double newTimeLimit)
// {
//     timeLimit = newTimeLimit;
// }
//
// void SimAnnealing::setCoolingRate(double newCoolingRate)
// {
//     coolingRate = newCoolingRate;
// }
//
//

//
// Created by Mateusz on 28.11.2024.
//

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

void SimAnnealing::loadFromFile(const std::string &filename) {
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
            distanceMatrix = new int *[numCities];
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

void SimAnnealing::initializeGreedyRoute() {
    std::vector<bool> visited(numCities, false);
    currentRoute[0] = 0; // zaczynamy od miasta 0
    visited[0] = true;

    for (int i = 1; i < numCities; ++i) {
        int lastCity = currentRoute[i - 1];
        int nearestCity = -1;
        int minDist = INT_MAX;

        // Znajdź najbliższe nieodwiedzone miasto
        for (int j = 0; j < numCities; ++j) {
            if (!visited[j] && distanceMatrix[lastCity][j] < minDist) {
                minDist = distanceMatrix[lastCity][j];
                nearestCity = j;
            }
        }

        currentRoute[i] = nearestCity;
        visited[nearestCity] = true;
    }

    std::copy(currentRoute, currentRoute + numCities, bestRoute);
    currentCost = calculateCost(currentRoute);
    bestCost = currentCost;
}

void SimAnnealing::apply2Opt(int *route) {
    for (int i = 0; i < numCities - 1; ++i) {
        for (int j = i + 1; j < numCities; ++j) {
            if (i != j) {
                // Sprawdź, czy zamiana segmentów poprawi trasę
                int delta = distanceMatrix[route[i]][route[i + 1]] + distanceMatrix[route[j]][route[(j + 1) % numCities]]
                            - distanceMatrix[route[i]][route[j]] - distanceMatrix[route[i + 1]][route[(j + 1) % numCities]];
                if (delta > 0) {
                    std::reverse(route + i + 1, route + j + 1);  // Zastosuj 2-opt
                }
            }
        }
    }
}

void SimAnnealing::calculateInitialTemperature() {
    int *route1 = new int[numCities];
    int *route2 = new int[numCities];
    double totalDelta = 0.0;

    for (int i = 0; i < numCities; ++i) {
        route1[i] = i;
        route2[i] = i;
    }

    for (int i = 0; i < 100; ++i) {
        perturbRoute(route1);
        perturbRoute(route2);
        totalDelta += std::abs(calculateCost(route1) - calculateCost(route2));
    }

    // Zmieniono bazę logarytmu na wartość dodatnią
    temperature = (totalDelta / 100) / std::log(1.25);

    if (temperature <= 0) {
        temperature = 1.0;  // Zapewniamy dodatnią temperaturę startową
    }

    std::cout << "Temperatura początkowa: " << temperature << std::endl;

    delete[] route1;
    delete[] route2;
}

int SimAnnealing::calculateCost(int *route) {
    int cost = 0;
    for (int i = 0; i < numCities - 1; ++i) {
        cost += distanceMatrix[route[i]][route[i + 1]];
    }
    cost += distanceMatrix[route[numCities - 1]][route[0]];
    return cost;
}

void SimAnnealing::perturbRoute(int *route) {
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dist(0, numCities - 1);

    int i = dist(gen);
    int j = dist(gen);

    while (i == j) j = dist(gen);

    if (rand() % 2 == 0) {
        std::swap(route[i], route[j]);
    } else {
        if (i > j) std::swap(i, j);
        std::reverse(route + i, route + j + 1);
    }

    // Zabezpieczenie przed brakiem zmian
    if (std::equal(currentRoute, currentRoute + numCities, route)) {
        perturbRoute(route);
    }
}

int* SimAnnealing::solve() {
    initializeGreedyRoute();  // Inicjalizacja trasy za pomocą metody zachłannej
    calculateInitialTemperature();

    auto startTime = std::chrono::steady_clock::now();
    int iteration = 0;
    int noImprovementCount = 0;
    const int maxNoImprovement = 1000;
    double elapsedTime = 0.0;

    while (elapsedTime < timeLimit) {
        auto currentTime = std::chrono::steady_clock::now();
        elapsedTime = std::chrono::duration<double>(currentTime - startTime).count();

        int *newRoute = new int[numCities];
        std::copy(currentRoute, currentRoute + numCities, newRoute);

        for (int i = 0; i < 10; ++i) {  // Zwiększenie liczby perturbacji na iterację
            perturbRoute(newRoute);  // Zastosowanie zwykłej perturbacji
            apply2Opt(newRoute);  // Zastosowanie 2-opt dla dalszej poprawy trasy
        }

        int newCost = calculateCost(newRoute);

        if (newCost < currentCost || ((rand() / double(RAND_MAX)) < exp((currentCost - newCost) / temperature))) {
            std::copy(newRoute, newRoute + numCities, currentRoute);
            currentCost = newCost;

            if (newCost < bestCost) {
                std::copy(newRoute, newRoute + numCities, bestRoute);
                bestCost = newCost;
                noImprovementCount = 0;
                bestFindTime = elapsedTime;
            }
        } else {
            noImprovementCount++;
        }

        delete[] newRoute;

        // Adaptacyjne chłodzenie
        temperature *= 1 - (coolingRate * (1 - bestCost / currentCost));  // Zmienione chłodzenie
        iteration++;

        // if (iteration % 100 == 0) {
        //     std::cout << "Iteracja: " << iteration
        //               << " | Aktualny koszt: " << currentCost
        //               << " | Najlepszy koszt: " << bestCost
        //               << " | Temperatura: " << temperature << "\n";
        // }
    }

    // std::cout << "Najlepszy koszt: " << bestCost << "\n";
    // std::cout << "Najlepsza trasa: ";
    // for (int i = 0; i < numCities; ++i) {
    //     std::cout << bestRoute[i] << " ";
    // }
    // std::cout << bestRoute[0] << "\n";
    std::cout << "Ostateczna temperatura: " << temperature << "\n";
    std::cout << "Wartość exp(-1/Tk): " << exp(-1 / temperature) << "\n";
    std::cout << "Znaleziono w : " <<bestFindTime << "[s]"<< std::endl;
    return bestRoute;
}

void SimAnnealing::setCoolingScheme(int scheme) {
    coolingScheme = scheme;
}

void SimAnnealing::saveResultToFile(const std::string &filename) {
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

void SimAnnealing::setTimeLimit(double newTimeLimit) {
    timeLimit = newTimeLimit;
}

void SimAnnealing::setCoolingRate(double newCoolingRate) {
    coolingRate = newCoolingRate;
}

