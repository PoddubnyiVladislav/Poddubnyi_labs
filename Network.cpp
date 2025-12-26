#include "Network.h"
#include <iostream>
#include <algorithm>
#include <queue>
#include <stack>
#include <limits>


GasNetwork::GasNetwork() {}

bool GasNetwork::addConnection(int pipeId, int startCSId, int endCSId, int diameter) {

    if (startCSId == endCSId) {
        std::cout << "Error: Cannot connect CS to itself!\n";
        return false;
    }

 
    if (usedPipes.find(pipeId) != usedPipes.end()) {
        std::cout << "Error: Pipe " << pipeId << " is already used in the network!\n";
        return false;
    }

    if (ALLOWED_DIAMETERS.find(diameter) == ALLOWED_DIAMETERS.end()) {
        std::cout << "Error: Invalid diameter! Allowed diameters: ";
        for (int d : ALLOWED_DIAMETERS) std::cout << d << " ";
        std::cout << "\n";
        return false;
    }

    adjacencyList[startCSId];
    adjacencyList[endCSId];

    

    double capacity = 0.0;
    auto capIt = PIPE_CAPACITY.find(diameter);
    if (capIt != PIPE_CAPACITY.end()) {
        capacity = capIt->second;
    }

    Connection conn(pipeId, startCSId, endCSId, diameter, capacity, 0.0);
    adjacencyList[startCSId].push_back(conn);
    usedPipes[pipeId] = true;

    if (!isAcyclic()) {
        
        removeConnection(pipeId);
        std::cout << "Error: Connection creates a cycle in the network!\n";
        return false;
    }

    return true;
}

bool GasNetwork::removeConnection(int pipeId) {
    if (usedPipes.find(pipeId) == usedPipes.end()) {
        return false;
    }


    for (auto& pair : adjacencyList) {
        auto& connections = pair.second;
        connections.erase(
            std::remove_if(connections.begin(), connections.end(),
                [pipeId](const Connection& conn) { return conn.pipeId == pipeId; }),
            connections.end()
        );
    }

   


    usedPipes.erase(pipeId);
    return true;
}

void GasNetwork::displayNetwork() const {
    if (adjacencyList.empty()) {
        std::cout << "Network is empty.\n";
        return;
    }


    std::map<int, int> incomingCount;
    for (const auto& pair : adjacencyList) {
        for (const auto& conn : pair.second) {
            incomingCount[conn.endCSId]++;
        }
    }

    std::cout << "=== GAS TRANSPORT NETWORK ===\n";
    for (const auto& pair : adjacencyList) {
        int csId = pair.first;
        bool hasOutgoing = !pair.second.empty();
        bool hasIncoming = (incomingCount.find(csId) != incomingCount.end() && incomingCount[csId] > 0);
        bool isIsolated = !hasOutgoing && !hasIncoming;

        std::cout << "CS " << csId;

        if (isIsolated) {
            std::cout << " [ISOLATED]";
        }
        else if (pair.second.empty()) {
            std::cout << " [NO OUTGOING CONNECTIONS]";
        }
        else {
            std::cout << " -> ";
            for (const auto& conn : pair.second) {
                std::cout << "CS " << conn.endCSId << " (Pipe " << conn.pipeId
                    << ", Diameter " << conn.diameter << ") ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "=============================\n";
}

std::vector<int> GasNetwork::topologicalSort() const {
    std::vector<int> result;
    std::map<int, int> inDegree;
    std::queue<int> zeroInDegree;
    std::set<int> allVertices;

    for (const auto& pair : adjacencyList) {
        allVertices.insert(pair.first);
        for (const auto& conn : pair.second) {
            allVertices.insert(conn.endCSId);
        }
    }

    for (int vertex : allVertices) {
        inDegree[vertex] = 0;
    }

    for (const auto& pair : adjacencyList) {
        for (const auto& conn : pair.second) {
            inDegree[conn.endCSId]++;
        }
    }

    for (const auto& pair : inDegree) {
        if (pair.second == 0) {
            zeroInDegree.push(pair.first);
        }
    }

    while (!zeroInDegree.empty()) {
        int csId = zeroInDegree.front();
        zeroInDegree.pop();
        result.push_back(csId);

        if (adjacencyList.find(csId) != adjacencyList.end()) {
            for (const auto& conn : adjacencyList.at(csId)) {
                inDegree[conn.endCSId]--;
                if (inDegree[conn.endCSId] == 0) {
                    zeroInDegree.push(conn.endCSId);
                }
            }
        }
    }

    if (result.size() != allVertices.size()) {
        return std::vector<int>();
    }

    return result;
}

bool GasNetwork::isAcyclic() const {
    return !topologicalSort().empty();
}

bool GasNetwork::isPipeUsed(int pipeId) const {
    return usedPipes.find(pipeId) != usedPipes.end();
}

std::vector<int> GasNetwork::findAvailablePipes(const std::map<int, Pipe>& pipes, int diameter) const {
    std::vector<int> availablePipes;

    for (const auto& pair : pipes) {
        const Pipe& pipe = pair.second;
        if (pipe.getDiameter() == diameter && !isPipeUsed(pipe.getId()) && !pipe.getStatus()) {
            availablePipes.push_back(pipe.getId());
        }
    }

    return availablePipes;
}

std::pair<double, std::vector<int>> GasNetwork::findShortestPath(
    int startCSId, int endCSId, const std::map<int, Pipe>& pipes) const {

    if (adjacencyList.find(startCSId) == adjacencyList.end() ||
        adjacencyList.find(endCSId) == adjacencyList.end()) {
        return { std::numeric_limits<double>::max(), {} };
    }

    std::map<int, double> distances;
    std::map<int, int> previous;
    std::set<std::pair<double, int>> priorityQueue;

    for (const auto& node : adjacencyList) {
        distances[node.first] = std::numeric_limits<double>::max();
        for (const auto& conn : node.second) {
            distances[conn.endCSId] = std::numeric_limits<double>::max();
        }
    }

    distances[startCSId] = 0.0;
    priorityQueue.insert({ 0.0, startCSId });


    while (!priorityQueue.empty()) {
        auto current = *priorityQueue.begin();
        priorityQueue.erase(priorityQueue.begin());

        double currentDist = current.first;
        int currentNode = current.second;

        if (currentNode == endCSId) break;
        if (currentDist > distances[currentNode]) continue;

  
        if (adjacencyList.find(currentNode) != adjacencyList.end()) {
            for (const auto& conn : adjacencyList.at(currentNode)) {
             
                double edgeWeight = std::numeric_limits<double>::max();

                auto pipeIt = pipes.find(conn.pipeId);
                if (pipeIt != pipes.end()) {
                    if (pipeIt->second.getStatus()) {
                   
                        edgeWeight = std::numeric_limits<double>::max();
                    }
                    else {
                      
                        edgeWeight = pipeIt->second.getLength();
                    }
                }

                if (edgeWeight < std::numeric_limits<double>::max()) {
                    double newDist = currentDist + edgeWeight;

                    if (newDist < distances[conn.endCSId]) {
                        priorityQueue.erase({ distances[conn.endCSId], conn.endCSId });
                        distances[conn.endCSId] = newDist;
                        previous[conn.endCSId] = currentNode;
                        priorityQueue.insert({ newDist, conn.endCSId });
                    }
                }
            }
        }
    }

   
    std::vector<int> path;
    if (distances[endCSId] < std::numeric_limits<double>::max()) {
        for (int at = endCSId; at != startCSId; at = previous[at]) {
            path.push_back(at);
        }
        path.push_back(startCSId);
        std::reverse(path.begin(), path.end());
    }

    return { distances[endCSId], path };
}



double GasNetwork::calculateMaxFlowFF(int sourceCSId, int sinkCSId,
    const std::map<int, Pipe>& pipes) const {
    if (adjacencyList.empty() ||
        adjacencyList.find(sourceCSId) == adjacencyList.end() ||
        adjacencyList.find(sinkCSId) == adjacencyList.end()) {
        return 0.0;
    }

    
    std::map<int, std::map<int, double>> capacity;

    
    for (const auto& node : adjacencyList) {
        int u = node.first;
        for (const auto& conn : node.second) {
            int v = conn.endCSId;

         
            double cap = 0.0;
            auto pipeIt = pipes.find(conn.pipeId);
            if (pipeIt != pipes.end()) {
                
                if (!pipeIt->second.getStatus()) {
                    auto capIt = PIPE_CAPACITY.find(conn.diameter);
                    if (capIt != PIPE_CAPACITY.end()) {
                        cap = capIt->second;
                    }
                }
             
            }

            capacity[u][v] = cap;
            capacity[v][u] = 0.0; 
        }
    }

    double maxFlow = 0.0;

   
    while (true) {
        
        std::queue<int> q;
        std::map<int, int> parent;
        std::map<int, bool> visited;

        q.push(sourceCSId);
        visited[sourceCSId] = true;
        parent[sourceCSId] = -1;

        bool foundPath = false;

        while (!q.empty() && !foundPath) {
            int u = q.front();
            q.pop();

            for (const auto& node : adjacencyList) {
                for (const auto& conn : node.second) {
                    if (conn.startCSId == u) {
                        int v = conn.endCSId;

                        if (capacity.find(u) != capacity.end() &&
                            capacity[u].find(v) != capacity[u].end() &&
                            !visited[v] && capacity[u][v] > 0) {
                            visited[v] = true;
                            parent[v] = u;
                            q.push(v);

                            if (v == sinkCSId) {
                                foundPath = true;
                                break;
                            }
                        }
                    }
                }
                if (foundPath) break;
            }
        }

    
        if (!foundPath) break;

        
        double pathFlow = std::numeric_limits<double>::max();
        for (int v = sinkCSId; v != sourceCSId; v = parent[v]) {
            int u = parent[v];
            pathFlow = std::min(pathFlow, capacity[u][v]);
        }

        
        for (int v = sinkCSId; v != sourceCSId; v = parent[v]) {
            int u = parent[v];
            capacity[u][v] -= pathFlow;
            capacity[v][u] += pathFlow; 
        }

        maxFlow += pathFlow;
    }

    return maxFlow;
}