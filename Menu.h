//
// Created by Mateusz on 25.11.2024.
//

#ifndef MENU_H
#define MENU_H
#include <string>
using namespace std;

class Menu {
    char mainChoice;
    char algorithmChoice;
    char exitConfirmChoice;
    void mainMenu();
    void algorithmsMenu();
    int inputLen();
    int n;
    bool checkChoices(char choice, char choiceQuantity);
    string inputPath();
    int inputNumberOfTests();
    void exitConfirmMenu();
};



#endif //MENU_H
