#include "utils.h"
#include "globals.h"


template<>
bool isValidInput<std::string>(std::string& result, const std::string& prompt, bool showError) {
    if (!prompt.empty()) {
        std::cout << prompt;
    }
    std::getline(std::cin, result);
    return !result.empty();
}

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) return "";

    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(start, end - start + 1);
}

int getNextPipeId() {
    int currentId = g_nextPipeId;
    g_nextPipeId++;
    return currentId;
}

int getNextStationId() {
    int currentId = g_nextStationId;
    g_nextStationId++;
    return currentId;
}