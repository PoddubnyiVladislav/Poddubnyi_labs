#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>
#include <iostream>
#include <limits>
#include <map>
#include "Pipe.h"
#include "CS.h"
#include "globals.h"


template<typename T>
bool isValidInput(T& result, const std::string& prompt = "", bool showError = true) {
    std::string input;
    if (!prompt.empty()) {
        std::cout << prompt;
    }
    std::getline(std::cin, input);

    std::stringstream ss(input);
    T value;

    if (!(ss >> value) || !ss.eof()) {
        if (showError) {
            std::cout << "Invalid input! Please try again.\n";
        }
        return false;
    }

    result = value;
    return true;
}

template<>
bool isValidInput<std::string>(std::string& result, const std::string& prompt, bool showError);

void clearInputBuffer();
std::string trim(const std::string& str);

int getNextPipeId();
int getNextStationId();

#endif