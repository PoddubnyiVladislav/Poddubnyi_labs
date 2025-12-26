#pragma once


#include <string>
#include <fstream>

class Logger {
private:
    std::ofstream logFile;
    std::ofstream userInputLogFile;

public:
    Logger(const std::string& filename);
    ~Logger();
    void logCommand(const std::string& command);
    void logUserInput(const std::string& input);
};

