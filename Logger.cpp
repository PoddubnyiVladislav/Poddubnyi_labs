#include "Logger.h"

Logger::Logger(const std::string& filename) {
    logFile.open(filename, std::ios::app);

    std::string userInputFilename = "user_input_" + filename;
    userInputLogFile.open(userInputFilename, std::ios::app);
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
    if (userInputLogFile.is_open()) {
        userInputLogFile.close();
    }
}

void Logger::logCommand(const std::string& command) {
    if (logFile.is_open()) {
        logFile << "COMMAND: " << command << "\n";
        logFile.flush();
    }
}

void Logger::logUserInput(const std::string& input) {
    if (logFile.is_open()) {
        logFile << "USER_INPUT: " << input << "\n";
        logFile.flush();
    }
    if (userInputLogFile.is_open()) {
        userInputLogFile << input << "\n";
        userInputLogFile.flush();
    }
}