#include "operations.h"
#include <algorithm>
#include <iostream>

void Operations::addPipe(std::map<int, Pipe>& pipes, Logger& logger) {
    std::string name;
    float length;
    int diameter;

    if (!isValidInput(name, "Enter name of pipe:\n")) {
        std::cout << "Invalid name!\n\n";
        return;
    }
    logger.logUserInput(name);

    if (!isValidInput(length, "Enter length of pipe:\n") || length <= 0) {
        std::cout << "Invalid length!\n\n";
        return;
    }
    logger.logUserInput(std::to_string(length));

    if (!isValidInput(diameter, "Enter diameter of pipe:\n") || diameter <= 0) {
        std::cout << "Invalid diameter!\n\n";
        return;
    }
    logger.logUserInput(std::to_string(diameter));

    int nextId = getNextPipeId();
    Pipe newPipe(nextId, name, length, diameter);
    pipes[newPipe.getId()] = newPipe;
    std::cout << "Pipe created successfully! ID: " << newPipe.getId() << "\n\n";
}

void Operations::addCompressStation(std::map<int, CompressStation>& stations, Logger& logger) {
    std::string name, class_cs;
    int workshops, workshops_in_work;

    if (!isValidInput(name, "Enter name of CS:\n")) {
        std::cout << "Invalid name!\n\n";
        return;
    }
    logger.logUserInput(name);

    if (!isValidInput(workshops, "Enter number of workshops:\n") || workshops <= 0) {
        std::cout << "Invalid number!\n\n";
        return;
    }
    logger.logUserInput(std::to_string(workshops));

    if (!isValidInput(workshops_in_work, "Enter number of workshops in work:\n")) {
        std::cout << "Invalid number!\n\n";
        return;
    }
    logger.logUserInput(std::to_string(workshops_in_work));

    if (workshops_in_work > workshops || workshops_in_work < 0) {
        std::cout << "Invalid number of workshops in work!\n\n";
        return;
    }

    if (!isValidInput(class_cs, "Enter class of CS:\n")) {
        std::cout << "Invalid class!\n\n";
        return;
    }
    logger.logUserInput(class_cs);

    int nextId = getNextStationId();
    CompressStation newCS(nextId, name, workshops, workshops_in_work, class_cs);
    stations[newCS.getId()] = newCS;
    std::cout << "CS created successfully! ID: " << newCS.getId() << "\n\n";
}

void Operations::viewAllObjects(const std::map<int, Pipe>& pipes, const std::map<int, CompressStation>& stations) {
    std::cout << "=== PIPES ===\n";
    if (pipes.empty()) {
        std::cout << "No pipes available.\n";
    }
    else {
        for (const auto& pair : pipes) {
            pair.second.displayInfo();
        }
    }

    std::cout << "=== COMPRESS STATIONS ===\n";
    if (stations.empty()) {
        std::cout << "No compress stations available.\n";
    }
    else {
        for (const auto& pair : stations) {
            pair.second.displayInfo();
        }
    }
    std::cout << std::endl;
}

std::vector<int> Operations::findPipesByName(const std::map<int, Pipe>& pipes, const std::string& name) {
    std::vector<int> result;
    for (const auto& pair : pipes) {
        if (pair.second.getName().find(name) != std::string::npos) {
            result.push_back(pair.first);
        }
    }
    return result;
}

std::vector<int> Operations::findPipesByStatus(const std::map<int, Pipe>& pipes, bool status) {
    std::vector<int> result;
    for (const auto& pair : pipes) {
        if (pair.second.getStatus() == status) {
            result.push_back(pair.first);
        }
    }
    return result;
}

std::vector<int> Operations::findPipesById(const std::map<int, Pipe>& pipes, int id) {
    std::vector<int> result;
    if (pipes.find(id) != pipes.end()) {
        result.push_back(id);
    }
    return result;
}

std::vector<int> Operations::findStationsByName(const std::map<int, CompressStation>& stations, const std::string& name) {
    std::vector<int> result;
    for (const auto& pair : stations) {
        if (pair.second.getName().find(name) != std::string::npos) {
            result.push_back(pair.first);
        }
    }
    return result;
}

std::vector<int> Operations::findStationsByUnusedPercentage(const std::map<int, CompressStation>& stations, double minPercent, double maxPercent) {
    std::vector<int> result;
    for (const auto& pair : stations) {
        double percent = pair.second.getUnusedPercentage();
        if (percent >= minPercent && percent <= maxPercent) {
            result.push_back(pair.first);
        }
    }
    return result;
}

std::vector<int> Operations::findStationsById(const std::map<int, CompressStation>& stations, int id) {
    std::vector<int> result;
    if (stations.find(id) != stations.end()) {
        result.push_back(id);
    }
    return result;
}

void Operations::editPipe(std::map<int, Pipe>& pipes, Logger& logger) {
    if (pipes.empty()) {
        std::cout << "No pipes available to edit.\n\n";
        return;
    }

    int pipeId;
    if (!isValidInput(pipeId, "Enter pipe ID to edit: ")) {
        std::cout << "Invalid ID!\n\n";
        return;
    }
    logger.logUserInput(std::to_string(pipeId));

    if (pipes.find(pipeId) == pipes.end()) {
        std::cout << "Pipe with ID " << pipeId << " not found.\n\n";
        return;
    }

    Pipe& pipe = pipes[pipeId];

    std::cout << "\nCurrent pipe data:\n";
    pipe.displayInfo();

    int fieldChoice;
    do {
        std::cout << "Select field to edit:\n";
        std::cout << "1. Name (current: " << pipe.getName() << ")\n";
        std::cout << "2. Length (current: " << pipe.getLength() << ")\n";
        std::cout << "3. Diameter (current: " << pipe.getDiameter() << ")\n";
        std::cout << "4. Repair status (current: " << (pipe.getStatus() ? "Under renovation" : "Working") << ")\n";
        std::cout << "0. Finish editing\n";

        if (!isValidInput(fieldChoice, "Enter your choice: ")) {
            std::cout << "Invalid choice!\n\n";
            continue;
        }
        logger.logUserInput(std::to_string(fieldChoice));

        switch (fieldChoice) {
        case 1: {
            std::string newName;
            if (!isValidInput(newName, "Enter new name: ")) {
                std::cout << "Invalid name!\n\n";
                break;
            }
            pipe.setName(newName);
            logger.logUserInput(newName);
            std::cout << "Name updated successfully!\n\n";
            break;
        }

        case 2: {
            float newLength;
            if (!isValidInput(newLength, "Enter new length: ")) {
                std::cout << "Invalid length!\n\n";
                break;
            }
            if (newLength <= 0) {
                std::cout << "Length must be positive!\n\n";
                break;
            }
            pipe.setLength(newLength);
            logger.logUserInput(std::to_string(newLength));
            std::cout << "Length updated successfully!\n\n";
            break;
        }

        case 3: {
            int newDiameter;
            if (!isValidInput(newDiameter, "Enter new diameter: ")) {
                std::cout << "Invalid diameter!\n\n";
                break;
            }
            if (newDiameter <= 0) {
                std::cout << "Diameter must be positive!\n\n";
                break;
            }
            pipe.setDiameter(newDiameter);
            logger.logUserInput(std::to_string(newDiameter));
            std::cout << "Diameter updated successfully!\n\n";
            break;
        }

        case 4: {
            std::cout << "Current status: " << (pipe.getStatus() ? "UNDER RENOVATION" : "WORKING") << "\n";
            std::cout << "Select new repair status:\n";
            std::cout << "1. Put UNDER RENOVATION (repair ON)\n";
            std::cout << "2. Set to WORKING (repair OFF)\n";

            int statusChoice;
            if (!isValidInput(statusChoice, "Enter your choice: ")) {
                std::cout << "Invalid choice!\n\n";
                break;
            }
            logger.logUserInput(std::to_string(statusChoice));

            bool newStatus;
            std::string statusText;

            switch (statusChoice) {
            case 1:
                newStatus = true;
                statusText = "UNDER RENOVATION";
                break;
            case 2:
                newStatus = false;
                statusText = "WORKING";
                break;
            default:
                std::cout << "Invalid choice!\n\n";
                return;
            }

            pipe.setStatus(newStatus);
            std::cout << "Repair status updated to: " << statusText << "\n\n";
            break;
        }

        case 0:
            std::cout << "Editing completed.\n\n";
            break;

        default:
            std::cout << "Invalid choice! Please enter 0-4.\n\n";
            break;
        }
    } while (fieldChoice != 0);
}

void Operations::editCompressStation(std::map<int, CompressStation>& stations, Logger& logger) {
    if (stations.empty()) {
        std::cout << "No compress stations available to edit.\n\n";
        return;
    }

    int stationId;
    if (!isValidInput(stationId, "Enter CS ID to edit: ")) {
        std::cout << "Invalid ID!\n\n";
        return;
    }
    logger.logUserInput(std::to_string(stationId));

    if (stations.find(stationId) == stations.end()) {
        std::cout << "CS with ID " << stationId << " not found.\n\n";
        return;
    }

    CompressStation& station = stations[stationId];

    std::cout << "\nCurrent CS data:\n";
    station.displayInfo();

    int fieldChoice;
    do {
        std::cout << "Select field to edit:\n";
        std::cout << "1. Name (current: " << station.getName() << ")\n";
        std::cout << "2. Number of workshops (current: " << station.getNumberOfWorkshops() << ")\n";
        std::cout << "3. Number of workshops in work (current: " << station.getNumberOfWorkshopsInWork() << ")\n";
        std::cout << "4. Class (current: " << station.getClass() << ")\n";
        std::cout << "0. Finish editing\n";

        if (!isValidInput(fieldChoice, "Enter your choice: ")) {
            std::cout << "Invalid choice!\n\n";
            continue;
        }
        logger.logUserInput(std::to_string(fieldChoice));

        switch (fieldChoice) {
        case 1: {
            std::string newName;
            if (!isValidInput(newName, "Enter new name: ")) {
                std::cout << "Invalid name!\n\n";
                break;
            }
            station.setName(newName);
            logger.logUserInput(newName);
            std::cout << "Name updated successfully!\n\n";
            break;
        }

        case 2: {
            int newWorkshops;
            if (!isValidInput(newWorkshops, "Enter new number of workshops: ")) {
                std::cout << "Invalid number!\n\n";
                break;
            }
            if (newWorkshops <= 0) {
                std::cout << "Number of workshops must be positive!\n\n";
                break;
            }

            if (station.getNumberOfWorkshopsInWork() > newWorkshops) {
                std::cout << "Warning: Current number of workshops in work ("
                    << station.getNumberOfWorkshopsInWork()
                    << ") exceeds new total (" << newWorkshops
                    << "). Please update workshops in work first.\n\n";
                break;
            }

            station.setNumberOfWorkshops(newWorkshops);
            logger.logUserInput(std::to_string(newWorkshops));
            std::cout << "Number of workshops updated successfully!\n\n";
            break;
        }

        case 3: {
            int newWorkshopsInWork;
            if (!isValidInput(newWorkshopsInWork, "Enter new number of workshops in work: ")) {
                std::cout << "Invalid number!\n\n";
                break;
            }

            if (newWorkshopsInWork > station.getNumberOfWorkshops()) {
                std::cout << "Error: Number of workshops in work cannot exceed total number of workshops ("
                    << station.getNumberOfWorkshops() << ").\n\n";
                break;
            }

            if (newWorkshopsInWork < 0) {
                std::cout << "Error: Number of workshops in work cannot be negative.\n\n";
                break;
            }

            station.setNumberOfWorkshopsInWork(newWorkshopsInWork);
            logger.logUserInput(std::to_string(newWorkshopsInWork));
            std::cout << "Number of workshops in work updated successfully!\n\n";
            break;
        }

        case 4: {
            std::string newClass;
            if (!isValidInput(newClass, "Enter new class: ")) {
                std::cout << "Invalid class!\n\n";
                break;
            }
            station.setClass(newClass);
            logger.logUserInput(newClass);
            std::cout << "Class updated successfully!\n\n";
            break;
        }

        case 0:
            std::cout << "Editing completed.\n\n";
            break;

        default:
            std::cout << "Invalid choice! Please enter 0-4.\n\n";
            break;
        }
    } while (fieldChoice != 0);
}

void Operations::batchEditPipes(std::map<int, Pipe>& pipes, GasNetwork& network, Logger& logger) {
    if (pipes.empty()) {
        std::cout << "No pipes available to edit.\n\n";
        return;
    }

    std::cout << "Batch edit pipes - search pipes to edit:\n"
        << "1. By ID\n"
        << "2. By name\n"
        << "3. By repair status\n";

    int searchChoice;
    if (!isValidInput(searchChoice, "Enter your choice: ")) {
        std::cout << "Invalid choice!\n\n";
        return;
    }
    logger.logUserInput(std::to_string(searchChoice));

    std::vector<int> foundPipes;

    if (searchChoice == 1) {
        int searchId;
        if (!isValidInput(searchId, "Enter pipe ID to search: ")) {
            std::cout << "Invalid ID!\n\n";
            return;
        }
        logger.logUserInput(std::to_string(searchId));
        foundPipes = findPipesById(pipes, searchId);

    }
    else if (searchChoice == 2) {
        std::string name;
        if (!isValidInput(name, "Enter name to search: ")) {
            std::cout << "Invalid name!\n\n";
            return;
        }
        logger.logUserInput(name);
        foundPipes = findPipesByName(pipes, name);

    }
    else if (searchChoice == 3) {
        std::cout << "Search by status:\n"
            << "1. Under renovation\n"
            << "2. Working\n";

        int statusChoice;
        if (!isValidInput(statusChoice, "Enter your choice: ")) {
            std::cout << "Invalid choice!\n\n";
            return;
        }
        logger.logUserInput(std::to_string(statusChoice));

        foundPipes = findPipesByStatus(pipes, statusChoice == 1);

    }
    else {
        std::cout << "Invalid choice!\n\n";
        return;
    }

    if (foundPipes.empty()) {
        std::cout << "No pipes to edit.\n\n";
        return;
    }

    std::cout << "Found " << foundPipes.size() << " pipes:\n";
    for (int id : foundPipes) {
        std::cout << "ID: " << id << " - " << pipes[id].getName()
            << " - " << (pipes[id].getStatus() ? "UNDER RENOVATION" : "WORKING") << "\n";
    }

    std::cout << "\nChoose editing option:\n";
    std::cout << "1. Edit all found pipes\n";
    std::cout << "2. Select specific pipes to edit\n";
    std::cout << "3. Cancel\n";

    int choice;
    if (!isValidInput(choice, "Enter your choice: ")) {
        std::cout << "Invalid choice!\n\n";
        return;
    }
    logger.logUserInput(std::to_string(choice));

    std::vector<int> pipesToEdit;

    if (choice == 1) {
        pipesToEdit = foundPipes;
    }
    else if (choice == 2) {
        std::cout << "Enter pipe IDs to edit (separated by spaces, 0 to finish):\n";
        int id;
        while (std::cin >> id && id != 0) {
            if (std::find(foundPipes.begin(), foundPipes.end(), id) != foundPipes.end()) {
                pipesToEdit.push_back(id);
                logger.logUserInput(std::to_string(id));
            }
            else {
                std::cout << "ID " << id << " not in found pipes.\n";
            }
        }
        clearInputBuffer();
    }
    else {
        std::cout << "Operation cancelled.\n\n";
        return;
    }

    if (pipesToEdit.empty()) {
        std::cout << "No pipes selected for editing.\n\n";
        return;
    }

    std::cout << "\nChoose what to edit:\n";
    std::cout << "1. Change repair status\n";
    std::cout << "2. Delete pipes\n";

    if (!isValidInput(choice, "Enter your choice: ")) {
        std::cout << "Invalid choice!\n\n";
        return;
    }
    logger.logUserInput(std::to_string(choice));

    if (choice == 1) {
        std::cout << "\nCurrent status of selected pipes:\n";
        for (int id : pipesToEdit) {
            std::cout << "ID: " << id << " - " << pipes[id].getName()
                << " - " << (pipes[id].getStatus() ? "UNDER RENOVATION" : "WORKING") << "\n";
        }

        std::cout << "\nSet repair status for all selected pipes:\n";
        std::cout << "1. Put UNDER RENOVATION (repair ON)\n";
        std::cout << "2. Set to WORKING (repair OFF)\n";
        std::cout << "3. TOGGLE status (reverse current status)\n";
        std::cout << "0. Cancel\n";

        int statusChoice;
        if (!isValidInput(statusChoice, "Enter your choice: ")) {
            std::cout << "Invalid choice!\n\n";
            return;
        }
        logger.logUserInput(std::to_string(statusChoice));

        if (statusChoice == 0) {
            std::cout << "Operation cancelled.\n\n";
            return;
        }

        int changedCount = 0;

        switch (statusChoice) {
        case 1: {
            for (int id : pipesToEdit) {
                if (!pipes[id].getStatus()) {
                    pipes[id].setStatus(true);
                    changedCount++;
                }
            }
            std::cout << "Put " << changedCount << " pipes UNDER RENOVATION.\n\n";
            break;
        }

        case 2: {
            for (int id : pipesToEdit) {
                if (pipes[id].getStatus()) {
                    pipes[id].setStatus(false);
                    changedCount++;
                }
            }
            std::cout << "Set " << changedCount << " pipes to WORKING status.\n\n";
            break;
        }

        case 3: {
            for (int id : pipesToEdit) {
                pipes[id].setStatus(!pipes[id].getStatus());
                changedCount++;
            }
            std::cout << "Toggled status for " << changedCount << " pipes.\n\n";
            break;
        }

        default:
            std::cout << "Invalid choice! Operation cancelled.\n\n";
            break;
        }
    }
    else if (choice == 2) {

        std::vector<int> pipesUsedInNetwork;
        for (int id : pipesToEdit) {
            if (network.isPipeUsed(id)) {
                pipesUsedInNetwork.push_back(id);
            }
        }

        if (!pipesUsedInNetwork.empty()) {
            std::cout << "Cannot delete pipes that are used in the network:\n";
            for (int id : pipesUsedInNetwork) {
                std::cout << "Pipe " << id << " is used in network connections\n";
            }
            std::cout << "Please disconnect them from network first.\n\n";
            return;
        }

        std::cout << "Are you sure you want to delete " << pipesToEdit.size() << " pipes? (Y/N): ";
        char confirm;
        std::cin >> confirm;
        clearInputBuffer();
        logger.logUserInput(std::string(1, confirm));

        if (tolower(confirm) == 'y') {
            for (int id : pipesToEdit) {
                pipes.erase(id);
            }
            std::cout << "Deleted " << pipesToEdit.size() << " pipes.\n\n";
        }
        else {
            std::cout << "Deletion cancelled.\n\n";
        }
    }
    else {
        std::cout << "Invalid choice!\n\n";
    }
}


void Operations::connectCS(std::map<int, Pipe>& pipes, std::map<int, CompressStation>& stations,
    GasNetwork& network, Logger& logger) {

    if (stations.size() < 2) {
        std::cout << "Need at least 2 compress stations to create connection!\n\n";
        return;
    }

    std::cout << "Available compress stations:\n";
    for (const auto& pair : stations) {
        std::cout << "ID: " << pair.first << " - " << pair.second.getName() << "\n";
    }

    int startCSId, endCSId, diameter;

    if (!isValidInput(startCSId, "Enter START CS ID: ")) {
        std::cout << "Invalid CS ID!\n\n";
        return;
    }
    logger.logUserInput(std::to_string(startCSId));

    if (stations.find(startCSId) == stations.end()) {
        std::cout << "Start CS with ID " << startCSId << " not found!\n\n";
        return;
    }

    if (!isValidInput(endCSId, "Enter END CS ID: ")) {
        std::cout << "Invalid CS ID!\n\n";
        return;
    }
    logger.logUserInput(std::to_string(endCSId));

    if (stations.find(endCSId) == stations.end()) {
        std::cout << "End CS with ID " << endCSId << " not found!\n\n";
        return;
    }

    std::cout << "Available diameters: 500, 700, 1000, 1400\n";
    if (!isValidInput(diameter, "Enter diameter: ")) {
        std::cout << "Invalid diameter!\n\n";
        return;
    }
    logger.logUserInput(std::to_string(diameter));

    if (diameter != 500 && diameter != 700 && diameter != 1000 && diameter != 1400) {
        std::cout << "Invalid diameter! Must be 500, 700, 1000, or 1400.\n\n";
        return;
    }


    std::vector<int> availablePipes = network.findAvailablePipes(pipes, diameter);

    int pipeId;
    if (!availablePipes.empty()) {
 
        std::cout << "Available pipes with diameter " << diameter << ":\n";
        for (int id : availablePipes) {
            std::cout << "ID: " << id << " - " << pipes[id].getName() << "\n";
        }

        if (!isValidInput(pipeId, "Enter pipe ID to use: ")) {
            std::cout << "Invalid pipe ID!\n\n";
            return;
        }
        logger.logUserInput(std::to_string(pipeId));

        if (std::find(availablePipes.begin(), availablePipes.end(), pipeId) == availablePipes.end()) {
            std::cout << "Invalid pipe selection!\n\n";
            return;
        }
    }
    else {
 
        std::cout << "No available pipes with diameter " << diameter << ". Creating new pipe...\n";

        std::string name;
        float length;

        if (!isValidInput(name, "Enter name for new pipe: ")) {
            std::cout << "Invalid name!\n\n";
            return;
        }
        logger.logUserInput(name);

        if (!isValidInput(length, "Enter length for new pipe: ") || length <= 0) {
            std::cout << "Invalid length!\n\n";
            return;
        }
        logger.logUserInput(std::to_string(length));

        pipeId = getNextPipeId();
        Pipe newPipe(pipeId, name, length, diameter);
        pipes[pipeId] = newPipe;

        std::cout << "Created new pipe with ID: " << pipeId << "\n";
    }


    if (network.addConnection(pipeId, startCSId, endCSId, diameter)) {
        std::cout << "Successfully connected CS " << startCSId << " to CS " << endCSId
            << " using pipe " << pipeId << "\n\n";
    }
    else {
        std::cout << "Failed to create connection!\n\n";
    }
}

void Operations::disconnectCS(GasNetwork& network, Logger& logger) {
    int pipeId;

    if (!isValidInput(pipeId, "Enter pipe ID to disconnect: ")) {
        std::cout << "Invalid pipe ID!\n\n";
        return;
    }
    logger.logUserInput(std::to_string(pipeId));

    if (network.removeConnection(pipeId)) {
        std::cout << "Successfully disconnected pipe " << pipeId << " from network.\n\n";
    }
    else {
        std::cout << "Pipe " << pipeId << " is not connected in the network!\n\n";
    }
}

void Operations::showNetwork(const GasNetwork& network) {
    network.displayNetwork();
}

void Operations::topologicalSortNetwork(const GasNetwork& network) {
    std::vector<int> sorted = network.topologicalSort();

    if (sorted.empty()) {
        std::cout << "Network contains cycles! Cannot perform topological sort.\n\n";
    }
    else {
        std::cout << "Topological order of compress stations:\n";
        for (size_t i = 0; i < sorted.size(); ++i) {
            std::cout << (i + 1) << ". CS " << sorted[i] << "\n";
        }
        std::cout << "\n";
    }
}

void Operations::calculateMaxFlowBetweenCS(const GasNetwork& network,
    const std::map<int, Pipe>& pipes,
    const std::map<int, CompressStation>& stations,
    Logger& logger) {
    if (stations.size() < 2) {
        std::cout << "Need at least 2 compress stations!\n\n";
        return;
    }

    std::cout << "Available compress stations:\n";
    for (const auto& pair : stations) {
        std::cout << "ID: " << pair.first << " - " << pair.second.getName() << "\n";
    }

    int sourceId, sinkId;

    if (!isValidInput(sourceId, "Enter SOURCE CS ID: ")) {
        std::cout << "Invalid CS ID!\n\n";
        return;
    }
    logger.logUserInput(std::to_string(sourceId));

    if (stations.find(sourceId) == stations.end()) {
        std::cout << "Source CS with ID " << sourceId << " not found!\n\n";
        return;
    }

    if (!isValidInput(sinkId, "Enter SINK CS ID: ")) {
        std::cout << "Invalid CS ID!\n\n";
        return;
    }
    logger.logUserInput(std::to_string(sinkId));

    if (stations.find(sinkId) == stations.end()) {
        std::cout << "Sink CS with ID " << sinkId << " not found!\n\n";
        return;
    }

    if (sourceId == sinkId) {
        std::cout << "Source and sink cannot be the same!\n\n";
        return;
    }

    double maxFlow = network.calculateMaxFlowFF(sourceId, sinkId, pipes);

    std::cout << "\n=== MAXIMUM FLOW CALCULATION ===\n";
    std::cout << "From CS " << sourceId << " to CS " << sinkId << "\n";
    std::cout << "Maximum flow: " << maxFlow << " million cubic m/day\n";

    if (maxFlow == 0.0) {
        std::cout << "No path exists or all pipes are under renovation!\n";
    }

    std::cout << "================================\n\n";
}

void Operations::findShortestPathBetweenCS(const GasNetwork& network,
    const std::map<int, Pipe>& pipes,
    const std::map<int, CompressStation>& stations,
    Logger& logger) {
    if (stations.size() < 2) {
        std::cout << "Need at least 2 compress stations!\n\n";
        return;
    }

    std::cout << "Available compress stations:\n";
    for (const auto& pair : stations) {
        std::cout << "ID: " << pair.first << " - " << pair.second.getName() << "\n";
    }

    int startId, endId;

    if (!isValidInput(startId, "Enter START CS ID: ")) {
        std::cout << "Invalid CS ID!\n\n";
        return;
    }
    logger.logUserInput(std::to_string(startId));

    if (stations.find(startId) == stations.end()) {
        std::cout << "Start CS with ID " << startId << " not found!\n\n";
        return;
    }

    if (!isValidInput(endId, "Enter END CS ID: ")) {
        std::cout << "Invalid CS ID!\n\n";
        return;
    }
    logger.logUserInput(std::to_string(endId));

    if (stations.find(endId) == stations.end()) {
        std::cout << "End CS with ID " << endId << " not found!\n\n";
        return;
    }

    auto result = network.findShortestPath(startId, endId, pipes);
    double distance = result.first;
    const auto& path = result.second;

    std::cout << "\n=== SHORTEST PATH CALCULATION ===\n";
    std::cout << "From CS " << startId << " to CS " << endId << "\n";

    if (distance < std::numeric_limits<double>::max()) {
        std::cout << "Total distance: " << distance << " units\n";
        std::cout << "Path: ";
        for (size_t i = 0; i < path.size(); ++i) {
            std::cout << "CS " << path[i];
            if (i < path.size() - 1) std::cout << " -> ";
        }
        std::cout << "\n";
    }
    else {
        std::cout << "No valid path exists!\n";
        std::cout << "Possible reasons:\n";
        std::cout << "1. No connection between stations\n";
        std::cout << "2. All connecting pipes are under renovation\n";
    }

    std::cout << "=================================\n\n";
}