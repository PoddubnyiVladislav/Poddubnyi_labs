#include "file_op.h"
#include <fstream>
#include <iostream>
#include "globals.h"
#include "Network.h"

void saveData(const std::string& filename, const std::map<int, Pipe>& pipes,
    const std::map<int, CompressStation>& stations, const GasNetwork& network, Logger& logger) {
    std::ofstream file(filename);

    if (file.is_open()) {
        file << COUNTERS_TAG << "\n";
        file << g_nextPipeId << "\n";
        file << g_nextStationId << "\n";
        file << COUNTERS_END_TAG << "\n\n";

        for (const auto& pair : pipes) {
            file << PIPE_START_TAG << "\n";
            file << pair.second;
            file << PIPE_END_TAG << "\n\n";
        }

        for (const auto& pair : stations) {
            file << STATION_START_TAG << "\n";
            file << pair.second;
            file << STATION_END_TAG << "\n\n";
        }

        file << NETWORK_START_TAG << "\n";
        const auto& adjList = network.getAdjacencyList();
        for (const auto& pair : adjList) {
            for (const auto& conn : pair.second) {
                file << CONNECTION_START_TAG << "\n";
                file << conn.pipeId << "\n";
                file << conn.startCSId << "\n";
                file << conn.endCSId << "\n";
                file << conn.diameter << "\n";
                file << CONNECTION_END_TAG << "\n\n";
            }
        }
        file << NETWORK_END_TAG << "\n";

        file.close();
        std::cout << "Data saved to " << filename << "\n\n";
    }
    else {
        std::cout << "Error saving data to file.\n\n";
    }
}


void loadData(const std::string& filename, std::map<int, Pipe>& pipes,
    std::map<int, CompressStation>& stations, GasNetwork& network, Logger& logger) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Error opening file for reading!\n\n";
        return;
    }

    pipes.clear();
    stations.clear();

    std::string line;
    bool counters_loaded = false;
    bool reading_network = false;

    try {
        while (std::getline(file, line)) {
            size_t start = line.find_first_not_of(" \t\n\r\f\v");
            if (start == std::string::npos) continue;
            size_t end = line.find_last_not_of(" \t\n\r\f\v");
            line = line.substr(start, end - start + 1);

            if (line.empty()) continue;

            if (line == COUNTERS_TAG && !counters_loaded) {
                file >> g_nextPipeId;
                file >> g_nextStationId;
                counters_loaded = true;

                std::getline(file, line);
                std::getline(file, line);
                continue;
            }
            else if (line == PIPE_START_TAG) {
                int id;
                std::string name;
                float length;
                int diameter;
                bool status;

                file >> id;
                file.ignore();
                std::getline(file, name);
                file >> length >> diameter >> status;

                Pipe pipe(id, name, length, diameter);
                if (status) {
                    pipe.setStatus(true);
                }

                pipes[pipe.getId()] = pipe;

                std::getline(file, line);
                std::getline(file, line);
            }
            else if (line == STATION_START_TAG) {
                int id;
                std::string name;
                int workshops, workshops_in_work;
                std::string class_cs;

                file >> id;
                file.ignore();
                std::getline(file, name);
                file >> workshops >> workshops_in_work;
                file.ignore();
                std::getline(file, class_cs);

                CompressStation station(id, name, workshops, workshops_in_work, class_cs);
                stations[station.getId()] = station;

                std::getline(file, line);
            }

            else if (line == NETWORK_START_TAG) {
                reading_network = true;
                continue;
            }

            else if (line == NETWORK_END_TAG) {
                reading_network = false;
                continue;
            }

            else if (line == CONNECTION_START_TAG && reading_network) {
                int pipeId, startCSId, endCSId, diameter;
                file >> pipeId >> startCSId >> endCSId >> diameter;
                network.addConnection(pipeId, startCSId, endCSId, diameter);
                std::getline(file, line); 
            }

        }

        if (!counters_loaded) {
            g_nextPipeId = 1;
            g_nextStationId = 1;

            for (const auto& pair : pipes) {
                if (pair.first >= g_nextPipeId) {
                    g_nextPipeId = pair.first + 1;
                }
            }

            for (const auto& pair : stations) {
                if (pair.first >= g_nextStationId) {
                    g_nextStationId = pair.first + 1;
                }
            }
        }

        std::cout << "Data loaded successfully from " << filename << "\n\n";

    }
    catch (const std::exception& e) {
        std::cout << "Error reading file: " << e.what() << "\n" << std::endl;
        pipes.clear();
        stations.clear();
        g_nextPipeId = 1;
        g_nextStationId = 1;
    }

    file.close();
}