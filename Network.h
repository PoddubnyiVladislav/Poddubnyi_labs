#pragma once

#include <vector>
#include <map>
#include <set>
#include <queue>
#include "Pipe.h"
#include "CS.h"

struct Connection {
    int pipeId;
    int startCSId;
    int endCSId;
    int diameter;
    double capacity;
    double weight;

    Connection(int pipeId, int startCSId, int endCSId, int diameter, double capacity = 0.0, double weight = 0.0)
        : pipeId(pipeId), startCSId(startCSId), endCSId(endCSId), diameter(diameter), capacity(capacity), weight(weight) {
    }
};

class GasNetwork {
private:
    std::map<int, std::vector<Connection>> adjacencyList; 
    std::map<int, bool> usedPipes; 

    bool dfsForFlow(int current, int sink,
        std::map<int, bool>& visited,
        std::map<int, int>& parent,
        const std::map<int, std::vector<Connection>>& residualGraph) const;
    double findBottleneckCapacity(const std::vector<int>& path, const std::map<int, std::vector<Connection>>& residualGraph) const;
    void updateResidualGraph(std::map<int, std::vector<Connection>>& residualGraph, const std::vector<int>& path, double flow);

public:
    GasNetwork();

    bool addConnection(int pipeId, int startCSId, int endCSId, int diameter);
    bool removeConnection(int pipeId);
    void displayNetwork() const;
    std::vector<int> topologicalSort() const;
    bool isAcyclic() const;

    const std::map<int, std::vector<Connection>>& getAdjacencyList() const { return adjacencyList; }
    const std::map<int, bool>& getUsedPipes() const { return usedPipes; }
    bool isPipeUsed(int pipeId) const;
    std::vector<int> findAvailablePipes(const std::map<int, Pipe>& pipes, int diameter) const;

    double calculateMaxFlowFF(int sourceCSId, int sinkCSId, const std::map<int, Pipe>& pipes) const;
    std::pair<double, std::vector<int>> findShortestPath(int startCSId, int endCSId, const std::map<int, Pipe>& pipes) const;
    void updateConnectionParameters(const std::map<int, Pipe>& pipes);
};