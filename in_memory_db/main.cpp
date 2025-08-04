// main.cpp
#include <iostream>
#include "gui/CLIInterface.hpp"
#include "gui/GUIInterface.hpp"

int main() {
    std::cout << "Please select the mode:\n";
    std::cout << "1. CLI interface\n";
    std::cout << "2. GUI interface\n";
    std::cout << "Please enter option (1 or 2):";

    int choice;
    std::cin >> choice;

    if (choice == 1) {
        gui::CLIInterface cli;
        cli.run();
    } else if (choice == 2) {
        gui::GUIInterface gui;
        gui.run();
    } else {
        std::cout << "Invalid option, program terminated.\n";
    }

    return 0;
}

