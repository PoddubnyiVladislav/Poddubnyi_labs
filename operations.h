#pragma once

#include <map>
#include <vector>
#include "Pipe.h"
#include "CS.h"
#include "Logger.h"
#include "utils.h"
#include "Network.h"

class Operations {
public:

    Operations() = default;

 
    void addPipe(std::map<int, Pipe>& pipes, Logger& logger);
    void editPipe(std::map<int, Pipe>& pipes, Logger& logger);
    std::vector<int> findPipesByName(const std::map<int, Pipe>& pipes, const std::string& name);
    std::vector<int> findPipesByStatus(const std::map<int, Pipe>& pipes, bool status);
    std::vector<int> findPipesById(const std::map<int, Pipe>& pipes, int id);
    void batchEditPipes(std::map<int, Pipe>& pipes, GasNetwork& network, Logger& logger);

    void addCompressStation(std::map<int, CompressStation>& stations, Logger& logger);
    void editCompressStation(std::map<int, CompressStation>& stations, Logger& logger);
    std::vector<int> findStationsByName(const std::map<int, CompressStation>& stations, const std::string& name);
    std::vector<int> findStationsByUnusedPercentage(const std::map<int, CompressStation>& stations, double minPercent, double maxPercent = 100.0);
    std::vector<int> findStationsById(const std::map<int, CompressStation>& stations, int id);


    void viewAllObjects(const std::map<int, Pipe>& pipes, const std::map<int, CompressStation>& stations);
    void connectCS(std::map<int, Pipe>& pipes, std::map<int, CompressStation>& stations,
        GasNetwork& network, Logger& logger);
    void disconnectCS(GasNetwork& network, Logger& logger);
    void showNetwork(const GasNetwork& network);
    void topologicalSortNetwork(const GasNetwork& network);

    void calculateMaxFlowBetweenCS(const GasNetwork& network,
        const std::map<int, Pipe>& pipes,
        const std::map<int, CompressStation>& stations,
        Logger& logger);

    void findShortestPathBetweenCS(const GasNetwork& network,
        const std::map<int, Pipe>& pipes,
        const std::map<int, CompressStation>& stations,
        Logger& logger);

};