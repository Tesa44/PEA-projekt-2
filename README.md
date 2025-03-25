# Tabu search and simulated annealing algorithms solving ATSP
 eng below
## Wstęp
Asymetryczny problem komiwojażera (ATSP, z ang. Asymmetric Traveling 
Salesman Problem) stanowi jedno z kluczowych wyzwań optymalizacyjnych 
w teorii grafów oraz badań operacyjnych. Zadanie to polega na wyznaczeniu 
najkrótszej możliwej trasy, która pozwala komiwojażerowi odwiedzić 
wszystkie zadane miasta dokładnie raz i powrócić do punktu początkowego, 
przy czym odległości między miastami mogą się różnić w zależności od 
kierunku przejścia (asymetria). ATSP znajduje szerokie zastosowanie w 
logistyce, planowaniu tras oraz optymalizacji produkcji, a także w systemach 
komunikacji miejskiej i transporcie.
Ze względu na swoją złożoność obliczeniową (problem należy do klasy NPtrudnych), rozwiązywanie ATSP wymaga zastosowania specjalistycznych 
algorytmów, takich jak algorytmy przeglądu zupełnego, podziału i ograniczeń 
lub programowania dynamicznego. Celem niniejszego badania jest ocena 
efektywności wybranych algorytmów stosowanych do rozwiązywania ATSP, 
poprzez analizę ich wyników w kontekście jakości uzyskanych rozwiązań. 

## Tabu search
Tabu Search to metaheurystyczny algorytm optymalizacyjny, który 
wykorzystuje strategię pamięci, aby unikać lokalnych minimów i 
przeszukiwać przestrzeń rozwiązań bardziej efektywnie. Proces rozpoczyna 
się od wyboru początkowego rozwiązania, które może być wybrane losowo 
lub na podstawie prostszej heurystyki, na przykład metody najbliższego 
sąsiada. Następnie algorytm generuje sąsiednie rozwiązania, czyli niewielkie 
modyfikacje bieżącego rozwiązania. Spośród nich wybierane jest najlepsze 
rozwiązanie, przy czym algorytm unika ruchów, które są zapisane na liście 
tabu. Lista tabu pełni funkcję pamięci, która blokuje niedawno wykonane 
ruchy na określony czas, aby uniknąć powtarzania tych samych ścieżek 
poszukiwań. Dodatkowo stosowane są mechanizmy aspiracji, które pozwalają 
na złamanie reguł tabu, jeśli prowadzi to do istotnie lepszego rozwiązania. W 
przypadku długiego braku poprawy w wynikach algorytm wprowadza 
mechanizm dywersyfikacji, który pozwala na eksplorację nowych, odległych 
obszarów przestrzeni rozwiązań.

## Sumulated annealing
Symulowane Wyżarzanie to metaheurystyka inspirowana procesem 
wyżarzania metali, w którym materiał jest powoli schładzany w kontrolowany 
sposób, aby osiągnąć minimalną energię układu. Algorytm rozpoczyna się od 
początkowego rozwiązania i ustalenia wysokiej temperatury początkowej. Dla 
każdego rozwiązania generowane jest sąsiednie rozwiązanie poprzez 
niewielką modyfikację obecnego. Jeśli nowe rozwiązanie jest lepsze, algorytm 
je akceptuje, a jeśli jest gorsze, zostaje zaakceptowane z pewnym 
prawdopodobieństwem zależnym od różnicy jakości rozwiązań i aktualnej 
temperatury. Wysoka temperatura na początku działania algorytmu umożliwia 
akceptowanie gorszych rozwiązań, co pozwala na wychodzenie z lokalnych 
minimów. W miarę postępu algorytmu temperatura jest stopniowo obniżana 
zgodnie z funkcją chłodzenia, co zmniejsza prawdopodobieństwo akceptacji 
gorszych rozwiązań i powoduje konwergencję do optymalnego lub blisko 
optymalnego rozwiązania. Proces kończy się, gdy temperatura osiągnie zadany 
poziom lub upłynie określony czas.

### Uruchomienie programu
1. Sklonuj repozytorium:
    ```sh
   git clone https://github.com/Tesa44/TabuSearch-SimulatedAnnealing.git
2. Otwórz projekt w CLion
3. Skonfiguruj kompilator
4. Zbuduj projekt
5. Uruchom plik main.cpp

Przy pierwszym odpaleniu programu należy wczytać macierz kosztów za pomocą opcji numer 2.
Gdy mamy wczytaną macierz możemy przejść do algorytmów opcją numer 1. W każdej chwili możemy używać inne opcje w menu
Wynik zapisywany jest do pliku "wynik.txt", który znajduje się w folderze cmake-build-release.

## Dokumentacja
Pełny przebieg badania znajduje się w pliku `PEA-projekt-2.pdf`

## Introduction
The Asymmetric Traveling Salesman Problem (ATSP) is one of the key optimization challenges in graph theory and operations research. The task involves determining the shortest possible route that allows a salesman to visit all given cities exactly once and return to the starting point, with distances between cities varying depending on the direction of travel (asymmetry). ATSP has wide applications in logistics, route planning, production optimization, and urban transportation systems. Due to its computational complexity (the problem belongs to the NP-hard class), solving ATSP requires specialized algorithms, such as exhaustive search, branch and bound, or dynamic programming. The goal of this study is to evaluate the efficiency of selected algorithms used to solve ATSP by analyzing their results in terms of solution quality.
## Tabu Search

Tabu Search is a metaheuristic optimization algorithm that employs memory strategies to avoid local minima and explore the solution space more effectively. The process begins with the selection of an initial solution, which can be chosen randomly or based on a simpler heuristic, such as the nearest neighbor method. The algorithm then generates neighboring solutions by making small modifications to the current solution. Among these, the best solution is chosen while avoiding moves that are recorded in the tabu list.

The tabu list serves as memory, preventing recently performed moves from being repeated for a certain period, thereby avoiding redundant search paths. Additionally, aspiration mechanisms are used to override tabu restrictions if a significantly better solution is found. If no improvement is observed for a long time, a diversification mechanism is introduced to explore new, distant regions of the solution space.

## Simulated Annealing
Simulated Annealing is a metaheuristic inspired by the annealing process in metallurgy, where a material is slowly cooled in a controlled manner to reach its lowest energy state. The algorithm starts with an initial solution and sets a high initial temperature. For each solution, a neighboring solution is generated by making a small modification to the current one. If the new solution is better, the algorithm accepts it. If it is worse, it is accepted with a certain probability, which depends on the difference in quality between the solutions and the current temperature. A high temperature at the start of the algorithm allows the acceptance of worse solutions, helping to escape local minima. As the algorithm progresses, the temperature gradually decreases according to a cooling function, reducing the probability of accepting worse solutions and leading to convergence toward an optimal or near-optimal solution. The process ends when the temperature reaches a predefined threshold or a specified amount of time has elapsed.

### Running the Program
1. Clone the repo
    ```sh
   git clone https://github.com/Tesa44/TabuSearch-SimulatedAnnealing.git
2. Ppen project in CLion
3. Configure compiler
4. Build the project
5. Run main.cpp

### Menu
On the first program launch, the cost matrix must be loaded using option 2.
Once the matrix is loaded, you can proceed to the algorithms using option 1. Other menu options can be used at any time.
The result is saved in the file "wynik.txt", which is located in the cmake-build-release folder.

## Documentation
The full research process is documented in the file `PEA-projekt-2.pdf`.
