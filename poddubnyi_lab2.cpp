#include <iostream>
#include <string>
#include <limits>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

const string PIPE_IDENTIFIER = "[PIPE]";
const string STATION_IDENTIFIER = "[STATION]";

struct Pipe {
    int id = 0;
    string name = "";
    int length = 0;
    int diameter = 0;
    bool underRepair = false;
};

struct CompressorStation {
    int id = 0;
    string name = "";
    unsigned int totalWorkshops = 0;
    unsigned int activeWorkshops = 0;
    int stationClass = 0;
};

vector<Pipe> pipes;
vector<CompressorStation> stations;
int nextPipeId = 1;
int nextStationId = 1;

template<typename T>
T getValidatedNumber(const string& prompt, T minValue = 1, T maxValue = numeric_limits<T>::max()) {
    string input;
    T value;

    while (true) {
        cout << prompt;
        getline(cin, input);

        stringstream ss(input);

        if (ss >> value) {
            char remaining;
            if (ss >> remaining) {
                cout << "Invalid input! Please enter only a number without extra characters.\n";
                continue;
            }

            if (value < minValue || value > maxValue) {
                cout << "Invalid input! Please enter a number between " << minValue << " and " << maxValue << ".\n";
                continue;
            }

            return value;
        }
        else {
            cout << "Invalid input! Please enter a valid number.\n";
        }
    }
}

bool getConfirmation(const string& message) {
    string input;
    while (true) {
        cout << message << " (y/n): ";
        getline(cin, input);

        if (input == "y" || input == "Y") {
            return true;
        }
        else if (input == "n" || input == "N") {
            return false;
        }
        else {
            cout << "Invalid input! Please enter 'y' or 'n'.\n";
        }
    }
}

int findPipeIndexById(int id) {
    for (size_t i = 0; i < pipes.size(); ++i) {
        if (pipes[i].id == id) {
            return i;
        }
    }
    return -1;
}

int findStationIndexById(int id) {
    for (size_t i = 0; i < stations.size(); ++i) {
        if (stations[i].id == id) {
            return i;
        }
    }
    return -1;
}

void displayAllPipes() {
    if (pipes.empty()) {
        cout << "No pipes available.\n";
        return;
    }

    cout << "\n=== ALL PIPES ===\n";
    for (const auto& pipe : pipes) {
        cout << "ID: " << pipe.id
            << " | Name: " << pipe.name
            << " | Length: " << pipe.length << " km"
            << " | Diameter: " << pipe.diameter << " mm"
            << " | Under repair: " << (pipe.underRepair ? "Yes" : "No") << "\n";
    }
}

void displayAllStations() {
    if (stations.empty()) {
        cout << "No stations available.\n";
        return;
    }

    cout << "\n=== ALL COMPRESSOR STATIONS ===\n";
    for (const auto& station : stations) {
        cout << "ID: " << station.id
            << " | Name: " << station.name
            << " | Workshops: " << station.activeWorkshops << "/" << station.totalWorkshops
            << " | Class: " << station.stationClass << "\n";
    }
}

void addPipe() {
    Pipe newPipe;
    newPipe.id = nextPipeId++;
    
    cout << "Enter pipe name: ";
    getline(cin, newPipe.name);
    newPipe.length = getValidatedNumber<int>("Enter length (km, must be positive): ", 1);
    newPipe.diameter = getValidatedNumber<int>("Enter diameter (mm, must be positive): ", 1);
    newPipe.underRepair = false;
    
    pipes.push_back(newPipe);
    cout << "Pipe added successfully! (ID: " << newPipe.id << ")\n";
}

void addStation() {
    CompressorStation newStation;
    newStation.id = nextStationId++;
    
    cout << "Enter station name: ";
    getline(cin, newStation.name);
    newStation.totalWorkshops = getValidatedNumber<unsigned int>("Enter total workshops: ", 1);
    newStation.activeWorkshops = getValidatedNumber<unsigned int>(
        "Enter active workshops: ", 0, newStation.totalWorkshops);
    newStation.stationClass = getValidatedNumber<int>("Enter station class: ", 1);
    
    stations.push_back(newStation);
    cout << "Station added successfully! (ID: " << newStation.id << ")\n";
}

void editPipeStatus() {
    if (pipes.empty()) {
        cout << "No pipes available to edit!\n";
        return;
    }

    displayAllPipes();
    int pipeId = getValidatedNumber<int>("\nEnter pipe ID to edit: ");
    
    int pipeIndex = findPipeIndexById(pipeId);
    if (pipeIndex == -1) {
        cout << "Pipe with ID " << pipeId << " not found!\n";
        return;
    }

    Pipe& pipe = pipes[pipeIndex];
    cout << "Current repair status: " << (pipe.underRepair ? "Under repair" : "Operational") << endl;
    
    if (getConfirmation("Change repair status?")) {
        pipe.underRepair = !pipe.underRepair;
        cout << "Status changed successfully!\n";
    }
}

void editStationWorkshops() {
    if (stations.empty()) {
        cout << "No stations available to edit!\n";
        return;
    }

    displayAllStations();
    int stationId = getValidatedNumber<int>("\nEnter station ID to edit: ");
    
    int stationIndex = findStationIndexById(stationId);
    if (stationIndex == -1) {
        cout << "Station with ID " << stationId << " not found!\n";
        return;
    }

    CompressorStation& station = stations[stationIndex];
    cout << "Current workshops: " << station.activeWorkshops << "/" << station.totalWorkshops << " active\n";
    cout << "1. Start workshop\n2. Stop workshop\nChoose action: ";

    int action = getValidatedNumber("", 1, 2);
    unsigned int changeAmount = getValidatedNumber<unsigned int>("Enter number of workshops: ", 1);

    if (action == 1) {
        if (station.activeWorkshops + changeAmount <= station.totalWorkshops) {
            station.activeWorkshops += changeAmount;
            cout << changeAmount << " workshop(s) started\n";
        }
        else {
            cout << "Cannot start more than " << station.totalWorkshops - station.activeWorkshops << " workshops\n";
        }
    }
    else {
        if (changeAmount <= station.activeWorkshops) {
            station.activeWorkshops -= changeAmount;
            cout << changeAmount << " workshop(s) stopped\n";
        }
        else {
            cout << "Cannot stop more than " << station.activeWorkshops << " workshops\n";
        }
    }
}

void deletePipe() {
    if (pipes.empty()) {
        cout << "No pipes available to delete!\n";
        return;
    }

    displayAllPipes();
    int pipeId = getValidatedNumber<int>("\nEnter pipe ID to delete: ");
    
    int pipeIndex = findPipeIndexById(pipeId);
    if (pipeIndex == -1) {
        cout << "Pipe with ID " << pipeId << " not found!\n";
        return;
    }

    cout << "You are about to delete pipe: " << pipes[pipeIndex].name << " (ID: " << pipeId << ")\n";
    if (getConfirmation("Are you sure?")) {
        pipes.erase(pipes.begin() + pipeIndex);
        cout << "Pipe deleted successfully!\n";
    }
}

void deleteStation() {
    if (stations.empty()) {
        cout << "No stations available to delete!\n";
        return;
    }

    displayAllStations();
    int stationId = getValidatedNumber<int>("\nEnter station ID to delete: ");
    
    int stationIndex = findStationIndexById(stationId);
    if (stationIndex == -1) {
        cout << "Station with ID " << stationId << " not found!\n";
        return;
    }

    cout << "You are about to delete station: " << stations[stationIndex].name << " (ID: " << stationId << ")\n";
    if (getConfirmation("Are you sure?")) {
        stations.erase(stations.begin() + stationIndex);
        cout << "Station deleted successfully!\n";
    }
}

void saveData() {
    string filename;
    cout << "Enter filename to save (without extension): ";
    getline(cin, filename);
    filename += ".txt";

    ifstream testFile(filename);
    if (testFile.good()) {
        testFile.close();
        if (!getConfirmation("File already exists. Overwrite?")) {
            cout << "Save cancelled.\n";
            return;
        }
    }

    ofstream outFile(filename);
    if (!outFile) {
        cout << "Error: Could not create file " << filename << endl;
        return;
    }

    // Сохраняем следующее ID для восстановления
    outFile << "[NEXT_PIPE_ID]" << endl << nextPipeId << endl;
    outFile << "[NEXT_STATION_ID]" << endl << nextStationId << endl;

    for (const auto& pipe : pipes) {
        outFile << PIPE_IDENTIFIER << endl;
        outFile << pipe.id << endl;
        outFile << pipe.name << endl;
        outFile << pipe.length << endl;
        outFile << pipe.diameter << endl;
        outFile << pipe.underRepair << endl;
    }

    for (const auto& station : stations) {
        outFile << STATION_IDENTIFIER << endl;
        outFile << station.id << endl;
        outFile << station.name << endl;
        outFile << station.totalWorkshops << endl;
        outFile << station.activeWorkshops << endl;
        outFile << station.stationClass << endl;
    }

    outFile.close();
    cout << "Data successfully saved to " << filename << endl;
    cout << "Saved: " << pipes.size() << " pipes, " << stations.size() << " stations\n";
}

void loadData() {
    string filename;
    cout << "Enter filename to load (without extension): ";
    getline(cin, filename);
    filename += ".txt";

    ifstream inFile(filename);
    if (!inFile) {
        cout << "Error: Could not open file " << filename << endl;
        return;
    }

    if (!pipes.empty() || !stations.empty()) {
        if (!getConfirmation("Current data will be overwritten. Continue?")) {
            cout << "Load cancelled.\n";
            inFile.close();
            return;
        }
    }

    pipes.clear();
    stations.clear();
    string line;

    while (getline(inFile, line)) {
        if (line == "[NEXT_PIPE_ID]") {
            string idStr;
            getline(inFile, idStr);
            nextPipeId = stoi(idStr);
        }
        else if (line == "[NEXT_STATION_ID]") {
            string idStr;
            getline(inFile, idStr);
            nextStationId = stoi(idStr);
        }
        else if (line == PIPE_IDENTIFIER) {
            Pipe pipe;
            string idStr, lengthStr, diameterStr, repairStr;
            
            getline(inFile, idStr);
            getline(inFile, pipe.name);
            getline(inFile, lengthStr);
            getline(inFile, diameterStr);
            getline(inFile, repairStr);

            pipe.id = stoi(idStr);
            pipe.length = stoi(lengthStr);
            pipe.diameter = stoi(diameterStr);
            pipe.underRepair = (repairStr == "1");
            pipes.push_back(pipe);
        }
        else if (line == STATION_IDENTIFIER) {
            CompressorStation station;
            string idStr, totalStr, activeStr, classStr;
            
            getline(inFile, idStr);
            getline(inFile, station.name);
            getline(inFile, totalStr);
            getline(inFile, activeStr);
            getline(inFile, classStr);

            station.id = stoi(idStr);
            station.totalWorkshops = stoul(totalStr);
            station.activeWorkshops = stoul(activeStr);
            station.stationClass = stoi(classStr);
            stations.push_back(station);
        }
    }

    inFile.close();
    cout << "Data successfully loaded from " << filename << endl;
    cout << "Loaded: " << pipes.size() << " pipes, " << stations.size() << " stations\n";
}

void viewAllObjects() {
    cout << "\n=== CURRENT STATE ===\n";
    displayAllPipes();
    displayAllStations();
}

int main() {
    int choice = -1;

    while (true) {
        cout << "\nMain Menu:\n"
            << "1. Add Pipe\n"
            << "2. Add Compressor Station\n"
            << "3. View All Objects\n"
            << "4. Edit Pipe Status\n"
            << "5. Edit Station Workshops\n"
            << "6. Delete Pipe\n"
            << "7. Delete Station\n"
            << "8. Save Data\n"
            << "9. Load Data\n"
            << "0. Exit\n"
            << "Choose action: ";

        string input;
        getline(cin, input);
        stringstream ss(input);

        if (!(ss >> choice)) {
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        char remaining;
        if (ss >> remaining) {
            cout << "Invalid input! Please enter only a number without extra characters.\n";
            continue;
        }

        switch (choice) {
        case 1:
            addPipe();
            break;

        case 2:
            addStation();
            break;

        case 3:
            viewAllObjects();
            break;

        case 4:
            editPipeStatus();
            break;

        case 5:
            editStationWorkshops();
            break;

        case 6:
            deletePipe();
            break;

        case 7:
            deleteStation();
            break;

        case 8:
            saveData();
            break;

        case 9:
            loadData();
            break;

        case 0:
            cout << "Exiting program...\n";
            return 0;

        default:
            cout << "Invalid choice! Try again.\n";
        }
    }

    return 0;
}
