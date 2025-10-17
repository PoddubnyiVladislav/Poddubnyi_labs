#include <iostream>
#include <string>
#include <limits>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <cctype>
#include <set>
#include <ctime>

using namespace std;

const string PIPE_IDENTIFIER = "[PIPE]";
const string STATION_IDENTIFIER = "[STATION]";

// Класс для логирования
class Logger {
private:
    ofstream logFile;
    string getCurrentTime() {
        time_t now = time(0);
        char timeStr[100];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&now));
        return string(timeStr);
    }

public:
    Logger() {
        logFile.open("app_log.txt", ios::app);
    }

    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    void log(const string& action, const string& details = "") {
        if (logFile.is_open()) {
            logFile << "[" << getCurrentTime() << "] " << action;
            if (!details.empty()) {
                logFile << " - " << details;
            }
            logFile << endl;
        }
        // Также выводим в консоль для удобства отладки
        cout << "[LOG] " << action;
        if (!details.empty()) {
            cout << " - " << details;
        }
        cout << endl;
    }
};

Logger logger;

struct Pipe {
    int id = 0;
    string name = "";
    int length = 0;
    int diameter = 0;
    bool underRepair = false;
    
    void display() const {
        cout << "ID: " << id
             << " | Name: " << name
             << " | Length: " << length << " km"
             << " | Diameter: " << diameter << " mm"
             << " | Under repair: " << (underRepair ? "Yes" : "No");
    }
};

struct CompressorStation {
    int id = 0;
    string name = "";
    unsigned int totalWorkshops = 0;
    unsigned int activeWorkshops = 0;
    int stationClass = 0;
    
    void display() const {
        cout << "ID: " << id
             << " | Name: " << name
             << " | Workshops: " << activeWorkshops << "/" << totalWorkshops
             << " | Class: " << stationClass;
    }
    
    double getUnusedPercentage() const {
        if (totalWorkshops == 0) return 0.0;
        return (1.0 - (double)activeWorkshops / totalWorkshops) * 100.0;
    }
};

// Класс для управления данными
class DataManager {
private:
    vector<Pipe> pipes;
    vector<CompressorStation> stations;
    unordered_set<int> usedPipeIds;
    unordered_set<int> usedStationIds;
    int nextPipeId = 1;
    int nextStationId = 1;

    int generateUniqueId(unordered_set<int>& usedIds, int& nextId) {
        while (usedIds.find(nextId) != usedIds.end()) {
            nextId++;
        }
        int newId = nextId;
        usedIds.insert(newId);
        nextId++;
        return newId;
    }

    void releaseId(unordered_set<int>& usedIds, int id) {
        usedIds.erase(id);
    }

    // безопасная toLower
    string toLower(const string& str) const {
        string result = str;
        transform(result.begin(), result.end(), result.begin(), [](unsigned char c){ return (char)tolower(c); });
        return result;
    }

public:
    // Геттеры
    const vector<Pipe>& getPipes() const { return pipes; }
    const vector<CompressorStation>& getStations() const { return stations; }
    bool hasPipes() const { return !pipes.empty(); }
    bool hasStations() const { return !stations.empty(); }

    // Работа с трубами
    void addPipe(const Pipe& pipe) {
        pipes.push_back(pipe);
        logger.log("Pipe added", "ID: " + to_string(pipe.id) + ", Name: " + pipe.name);
    }

    void deletePipe(int id) {
        auto it = find_if(pipes.begin(), pipes.end(), [id](const Pipe& p) { return p.id == id; });
        if (it != pipes.end()) {
            logger.log("Pipe deleted", "ID: " + to_string(id) + ", Name: " + it->name);
            releaseId(usedPipeIds, id);
            pipes.erase(it);
        }
    }

    // неконстантная версия
    Pipe* getPipeById(int id) {
        auto it = find_if(pipes.begin(), pipes.end(), [id](const Pipe& p) { return p.id == id; });
        return it != pipes.end() ? &(*it) : nullptr;
    }
    // константная версия (для вызова на const DataManager&)
    const Pipe* getPipeById(int id) const {
        auto it = find_if(pipes.begin(), pipes.end(), [id](const Pipe& p) { return p.id == id; });
        return it != pipes.end() ? &(*it) : nullptr;
    }

    vector<int> findPipesByName(const string& name) const {
        vector<int> result;
        string searchLower = toLower(name);
        for (const auto& pipe : pipes) {
            if (toLower(pipe.name).find(searchLower) != string::npos) {
                result.push_back(pipe.id);
            }
        }
        return result;
    }

    vector<int> findPipesByRepairStatus(bool status) const {
        vector<int> result;
        for (const auto& pipe : pipes) {
            if (pipe.underRepair == status) {
                result.push_back(pipe.id);
            }
        }
        return result;
    }

    void batchUpdatePipes(const vector<int>& pipeIds, bool newStatus) {
        for (int id : pipeIds) {
            if (Pipe* pipe = getPipeById(id)) {
                pipe->underRepair = newStatus;
            }
        }
        logger.log("Batch pipe update", "Updated " + to_string(pipeIds.size()) + " pipes to status: " + (newStatus ? "under repair" : "operational"));
    }

    // Работа со станциями
    void addStation(const CompressorStation& station) {
        stations.push_back(station);
        logger.log("Station added", "ID: " + to_string(station.id) + ", Name: " + station.name);
    }

    void deleteStation(int id) {
        auto it = find_if(stations.begin(), stations.end(), [id](const CompressorStation& s) { return s.id == id; });
        if (it != stations.end()) {
            logger.log("Station deleted", "ID: " + to_string(id) + ", Name: " + it->name);
            releaseId(usedStationIds, id);
            stations.erase(it);
        }
    }

    CompressorStation* getStationById(int id) {
        auto it = find_if(stations.begin(), stations.end(), [id](const CompressorStation& s) { return s.id == id; });
        return it != stations.end() ? &(*it) : nullptr;
    }
    const CompressorStation* getStationById(int id) const {
        auto it = find_if(stations.begin(), stations.end(), [id](const CompressorStation& s) { return s.id == id; });
        return it != stations.end() ? &(*it) : nullptr;
    }

    vector<int> findStationsByName(const string& name) const {
        vector<int> result;
        string searchLower = toLower(name);
        for (const auto& station : stations) {
            if (toLower(station.name).find(searchLower) != string::npos) {
                result.push_back(station.id);
            }
        }
        return result;
    }

    vector<int> findStationsByUnusedPercentage(double minPercent, double maxPercent) const {
        vector<int> result;
        for (const auto& station : stations) {
            double unusedPercent = station.getUnusedPercentage();
            if (unusedPercent >= minPercent && unusedPercent <= maxPercent) {
                result.push_back(station.id);
            }
        }
        return result;
    }

    // Создание новых объектов с автоматической генерацией ID
    Pipe createNewPipe() {
        Pipe pipe;
        pipe.id = generateUniqueId(usedPipeIds, nextPipeId);
        return pipe;
    }

    CompressorStation createNewStation() {
        CompressorStation station;
        station.id = generateUniqueId(usedStationIds, nextStationId);
        return station;
    }

    // Сохранение и загрузка
    void saveToFile(const string& filename) {
        ofstream outFile(filename);
        if (!outFile) {
            throw runtime_error("Could not create file " + filename);
        }

        outFile << "[NEXT_PIPE_ID]" << endl << nextPipeId << endl;
        outFile << "[NEXT_STATION_ID]" << endl << nextStationId << endl;
        
        outFile << "[USED_PIPE_IDS]" << endl;
        for (int id : usedPipeIds) outFile << id << " ";
        outFile << endl;
        
        outFile << "[USED_STATION_IDS]" << endl;
        for (int id : usedStationIds) outFile << id << " ";
        outFile << endl;

        for (const auto& pipe : pipes) {
            outFile << PIPE_IDENTIFIER << endl;
            outFile << pipe.id << endl;
            outFile << pipe.name << endl;
            outFile << pipe.length << endl;
            outFile << pipe.diameter << endl;
            outFile << (pipe.underRepair ? 1 : 0) << endl;
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
        logger.log("Data saved", "File: " + filename + ", Pipes: " + to_string(pipes.size()) + ", Stations: " + to_string(stations.size()));
    }

    void loadFromFile(const string& filename) {
        ifstream inFile(filename);
        if (!inFile) {
            throw runtime_error("Could not open file " + filename);
        }

        // Сохраняем старые данные для возможности отката
        auto oldPipes = pipes;
        auto oldStations = stations;
        auto oldUsedPipeIds = usedPipeIds;
        auto oldUsedStationIds = usedStationIds;
        int oldNextPipeId = nextPipeId;
        int oldNextStationId = nextStationId;

        try {
            pipes.clear();
            stations.clear();
            usedPipeIds.clear();
            usedStationIds.clear();

            string line;
            while (getline(inFile, line)) {
                if (line == "[NEXT_PIPE_ID]") {
                    if (!getline(inFile, line)) break;
                    nextPipeId = stoi(line);
                }
                else if (line == "[NEXT_STATION_ID]") {
                    if (!getline(inFile, line)) break;
                    nextStationId = stoi(line);
                }
                else if (line == "[USED_PIPE_IDS]") {
                    if (!getline(inFile, line)) break;
                    stringstream ss(line);
                    int id;
                    while (ss >> id) usedPipeIds.insert(id);
                }
                else if (line == "[USED_STATION_IDS]") {
                    if (!getline(inFile, line)) break;
                    stringstream ss(line);
                    int id;
                    while (ss >> id) usedStationIds.insert(id);
                }
                else if (line == PIPE_IDENTIFIER) {
                    Pipe pipe;
                    if (!getline(inFile, line)) break; pipe.id = stoi(line);
                    if (!getline(inFile, pipe.name)) break;
                    if (!getline(inFile, line)) break; pipe.length = stoi(line);
                    if (!getline(inFile, line)) break; pipe.diameter = stoi(line);
                    if (!getline(inFile, line)) break; pipe.underRepair = (line == "1");
                    pipes.push_back(pipe);
                }
                else if (line == STATION_IDENTIFIER) {
                    CompressorStation station;
                    if (!getline(inFile, line)) break; station.id = stoi(line);
                    if (!getline(inFile, station.name)) break;
                    if (!getline(inFile, line)) break; station.totalWorkshops = stoul(line);
                    if (!getline(inFile, line)) break; station.activeWorkshops = stoul(line);
                    if (!getline(inFile, line)) break; station.stationClass = stoi(line);
                    stations.push_back(station);
                }
            }
            inFile.close();
            logger.log("Data loaded", "File: " + filename + ", Pipes: " + to_string(pipes.size()) + ", Stations: " + to_string(stations.size()));
        }
        catch (const exception& e) {
            // Восстанавливаем старые данные при ошибке
            pipes = oldPipes;
            stations = oldStations;
            usedPipeIds = oldUsedPipeIds;
            usedStationIds = oldUsedStationIds;
            nextPipeId = oldNextPipeId;
            nextStationId = oldNextStationId;
            inFile.close();
            throw runtime_error("Error loading file: " + string(e.what()));
        }
    }
};

// Функции для ввода данных
template<typename T>
T getValidatedInput(const string& prompt, T minValue = numeric_limits<T>::lowest(), T maxValue = numeric_limits<T>::max()) {
    string input;
    T value;

    while (true) {
        if (!prompt.empty()) cout << prompt;
        getline(cin, input);

        stringstream ss(input);
        string extra;
        if (ss >> value && !(ss >> extra)) { // Проверяем что ввод полностью корректен
            if (value >= minValue && value <= maxValue) {
                return value;
            }
        }
        cout << "Invalid input! Please enter a valid number between " << minValue << " and " << maxValue << ".\n";
    }
}

bool getConfirmation(const string& message) {
    string input;
    while (true) {
        cout << message << " (y/n): ";
        getline(cin, input);

        if (input == "y" || input == "Y") return true;
        if (input == "n" || input == "N") return false;
        cout << "Invalid input! Please enter 'y' or 'n'.\n";
    }
}

string getFilename() {
    string filename;
    cout << "Enter filename: ";
    getline(cin, filename);
    return filename;
}

// Функции для отображения
void displayPipes(const vector<Pipe>& pipes) {
    if (pipes.empty()) {
        cout << "No pipes available.\n";
        return;
    }
    cout << "\n=== PIPES ===\n";
    for (const auto& pipe : pipes) {
        pipe.display();
        cout << "\n";
    }
}

void displayStations(const vector<CompressorStation>& stations) {
    if (stations.empty()) {
        cout << "No stations available.\n";
        return;
    }
    cout << "\n=== COMPRESSOR STATIONS ===\n";
    for (const auto& station : stations) {
        station.display();
        cout << " | Unused: " << station.getUnusedPercentage() << "%\n";
    }
}

void displayObjectsByIds(const DataManager& dm, const vector<int>& pipeIds, const vector<int>& stationIds) {
    cout << "\n=== SELECTED OBJECTS ===\n";
    
    if (!pipeIds.empty()) {
        cout << "Pipes:\n";
        for (int id : pipeIds) {
            if (const Pipe* pipe = dm.getPipeById(id)) {
                cout << "  "; pipe->display(); cout << "\n";
            }
        }
    }
    
    if (!stationIds.empty()) {
        cout << "Stations:\n";
        for (int id : stationIds) {
            if (const CompressorStation* station = dm.getStationById(id)) {
                cout << "  "; station->display(); cout << " | Unused: " << station->getUnusedPercentage() << "%\n";
            }
        }
    }
}

// Функции для пакетных операций
vector<int> selectPipesForBatchOperation(DataManager& dm) {
    if (!dm.hasPipes()) {
        cout << "No pipes available!\n";
        return {};
    }

    cout << "\n=== BATCH PIPE SELECTION ===\n";
    cout << "1. Search by name\n";
    cout << "2. Search by repair status\n";
    cout << "0. Cancel\n";
    
    int choice = getValidatedInput<int>("Choose search type: ", 0, 2);
    if (choice == 0) return {};

    vector<int> foundIds;
    switch (choice) {
        case 1: {
            cout << "Enter pipe name to search: ";
            string name;
            getline(cin, name);
            foundIds = dm.findPipesByName(name);
            break;
        }
        case 2: {
            cout << "Search for:\n1. Under repair\n2. Operational\n";
            int statusChoice = getValidatedInput<int>("Choose status: ", 1, 2);
            foundIds = dm.findPipesByRepairStatus(statusChoice == 1);
            break;
        }
    }

    if (foundIds.empty()) {
        cout << "No pipes found.\n";
        return {};
    }

    displayObjectsByIds(dm, foundIds, {});
    cout << "\nFound " << foundIds.size() << " pipe(s)\n";

    cout << "Selection mode:\n1. Select all\n2. Select specific pipes\n0. Cancel\n";
    int selectMode = getValidatedInput<int>("Choose selection mode: ", 0, 2);
    
    if (selectMode == 0) return {};
    if (selectMode == 1) return foundIds;

    // Выбор конкретных труб
    cout << "Enter pipe IDs to select (separated by spaces): ";
    string input;
    getline(cin, input);
    
    stringstream ss(input);
    set<int> selectedIds;
    int id;
    
    while (ss >> id) {
        if (find(foundIds.begin(), foundIds.end(), id) != foundIds.end()) {
            selectedIds.insert(id);
        } else {
            cout << "Pipe ID " << id << " not in search results. Skipping.\n";
        }
    }

    return vector<int>(selectedIds.begin(), selectedIds.end());
}

void batchEditPipes(DataManager& dm) {
    vector<int> selectedIds = selectPipesForBatchOperation(dm);
    if (selectedIds.empty()) return;

    cout << "\nBatch editing " << selectedIds.size() << " pipe(s)\n";
    cout << "1. Mark as under repair\n";
    cout << "2. Mark as operational\n";
    cout << "3. Toggle repair status\n";
    cout << "0. Cancel\n";
    
    int action = getValidatedInput<int>("Choose action: ", 0, 3);
    if (action == 0) return;

    bool newStatus;
    switch (action) {
        case 1: newStatus = true; break;
        case 2: newStatus = false; break;
        case 3: 
            // Для toggle нужно обрабатывать каждую трубу отдельно
            for (int id : selectedIds) {
                if (Pipe* pipe = dm.getPipeById(id)) {
                    pipe->underRepair = !pipe->underRepair;
                }
            }
            logger.log("Batch pipe toggle", "Toggled " + to_string(selectedIds.size()) + " pipes");
            cout << "Toggled repair status for " << selectedIds.size() << " pipes\n";
            return;
        default:
            return;
    }

    dm.batchUpdatePipes(selectedIds, newStatus);
    cout << "Updated " << selectedIds.size() << " pipes to " << (newStatus ? "under repair" : "operational") << "\n";
}

void batchDeletePipes(DataManager& dm) {
    vector<int> selectedIds = selectPipesForBatchOperation(dm);
    if (selectedIds.empty()) return;

    cout << "You are about to delete " << selectedIds.size() << " pipe(s)\n";
    displayObjectsByIds(dm, selectedIds, {});
    
    if (getConfirmation("Are you sure you want to delete these pipes?")) {
        for (int id : selectedIds) {
            dm.deletePipe(id);
        }
        cout << "Deleted " << selectedIds.size() << " pipe(s)\n";
    }
}

// Основное меню
void showMainMenu() {
    cout << "\n=== GAS NETWORK MANAGEMENT ===\n"
         << "1. Add Pipe\n"
         << "2. Add Compressor Station\n"
         << "3. View All Objects\n"
         << "4. Edit Pipe\n"
         << "5. Edit Station\n"
         << "6. Delete Pipe\n"
         << "7. Delete Station\n"
         << "8. Search Pipes\n"
         << "9. Search Stations\n"
         << "10. Batch Edit Pipes\n"
         << "11. Batch Delete Pipes\n"
         << "12. Save Data\n"
         << "13. Load Data\n"
         << "0. Exit\n"
         << "Choose action: ";
}

int main() {
    DataManager dm;
    logger.log("Application started");

    try {
        int choice = -1;
        while (choice != 0) {
            showMainMenu();
            choice = getValidatedInput<int>("", 0, 13);

            try {
                switch (choice) {
                    case 1: {
                        Pipe newPipe = dm.createNewPipe();
                        cout << "Enter pipe name: ";
                        getline(cin, newPipe.name);
                        newPipe.length = getValidatedInput<int>("Enter length (km): ", 1);
                        newPipe.diameter = getValidatedInput<int>("Enter diameter (mm): ", 1);
                        newPipe.underRepair = false;
                        dm.addPipe(newPipe);
                        cout << "Pipe added successfully! (ID: " << newPipe.id << ")\n";
                        break;
                    }

                    case 2: {
                        CompressorStation newStation = dm.createNewStation();
                        cout << "Enter station name: ";
                        getline(cin, newStation.name);
                        newStation.totalWorkshops = getValidatedInput<unsigned int>("Enter total workshops: ", 1);
                        newStation.activeWorkshops = getValidatedInput<unsigned int>(
                            "Enter active workshops: ", 0, newStation.totalWorkshops);
                        newStation.stationClass = getValidatedInput<int>("Enter station class: ", 1);
                        dm.addStation(newStation);
                        cout << "Station added successfully! (ID: " << newStation.id << ")\n";
                        break;
                    }

                    case 3: {
                        displayPipes(dm.getPipes());
                        displayStations(dm.getStations());
                        break;
                    }

                    case 4: {
                        if (!dm.hasPipes()) {
                            cout << "No pipes available!\n";
                            break;
                        }
                        displayPipes(dm.getPipes());
                        int id = getValidatedInput<int>("Enter pipe ID to edit: ");
                        if (Pipe* pipe = dm.getPipeById(id)) {
                            cout << "Current status: " << (pipe->underRepair ? "Under repair" : "Operational") << endl;
                            if (getConfirmation("Change status?")) {
                                pipe->underRepair = !pipe->underRepair;
                                logger.log("Pipe status changed", "ID: " + to_string(id) + " to " + (pipe->underRepair ? "under repair" : "operational"));
                                cout << "Status changed successfully!\n";
                            }
                        } else {
                            cout << "Pipe not found!\n";
                        }
                        break;
                    }

                    case 5: {
                        if (!dm.hasStations()) {
                            cout << "No stations available!\n";
                            break;
                        }
                        displayStations(dm.getStations());
                        int id = getValidatedInput<int>("Enter station ID to edit: ");
                        if (CompressorStation* station = dm.getStationById(id)) {
                            cout << "Current workshops: " << station->activeWorkshops << "/" << station->totalWorkshops << endl;
                            cout << "1. Start workshop\n2. Stop workshop\n";
                            int action = getValidatedInput<int>("Choose action: ", 1, 2);
                            unsigned int count = getValidatedInput<unsigned int>("Enter number of workshops: ", 1);
                            
                            if (action == 1) {
                                if (station->activeWorkshops + count <= station->totalWorkshops) {
                                    station->activeWorkshops += count;
                                    logger.log("Station workshops started", "ID: " + to_string(id) + ", count: " + to_string(count));
                                    cout << count << " workshop(s) started\n";
                                } else {
                                    cout << "Cannot start more workshops!\n";
                                }
                            } else {
                                if (count <= station->activeWorkshops) {
                                    station->activeWorkshops -= count;
                                    logger.log("Station workshops stopped", "ID: " + to_string(id) + ", count: " + to_string(count));
                                    cout << count << " workshop(s) stopped\n";
                                } else {
                                    cout << "Cannot stop more workshops!\n";
                                }
                            }
                        } else {
                            cout << "Station not found!\n";
                        }
                        break;
                    }

                    case 6: {
                        if (!dm.hasPipes()) {
                            cout << "No pipes available!\n";
                            break;
                        }
                        displayPipes(dm.getPipes());
                        int id = getValidatedInput<int>("Enter pipe ID to delete: ");
                        if (dm.getPipeById(id)) {
                            if (getConfirmation("Are you sure you want to delete this pipe?")) {
                                dm.deletePipe(id);
                                cout << "Pipe deleted successfully!\n";
                            }
                        } else {
                            cout << "Pipe not found!\n";
                        }
                        break;
                    }

                    case 7: {
                        if (!dm.hasStations()) {
                            cout << "No stations available!\n";
                            break;
                        }
                        displayStations(dm.getStations());
                        int id = getValidatedInput<int>("Enter station ID to delete: ");
                        if (dm.getStationById(id)) {
                            if (getConfirmation("Are you sure you want to delete this station?")) {
                                dm.deleteStation(id);
                                cout << "Station deleted successfully!\n";
                            }
                        } else {
                            cout << "Station not found!\n";
                        }
                        break;
                    }

                    case 8: {
                        if (!dm.hasPipes()) {
                            cout << "No pipes available!\n";
                            break;
                        }
                        cout << "Search pipes by:\n1. Name\n2. Repair status\n";
                        int searchType = getValidatedInput<int>("Choose search type: ", 1, 2);
                        vector<int> results;
                        if (searchType == 1) {
                            cout << "Enter name to search: ";
                            string name;
                            getline(cin, name);
                            results = dm.findPipesByName(name);
                        } else {
                            cout << "Search for:\n1. Under repair\n2. Operational\n";
                            int status = getValidatedInput<int>("Choose status: ", 1, 2);
                            results = dm.findPipesByRepairStatus(status == 1);
                        }
                        displayObjectsByIds(dm, results, {});
                        break;
                    }

                    case 9: {
                        if (!dm.hasStations()) {
                            cout << "No stations available!\n";
                            break;
                        }
                        cout << "Search stations by:\n1. Name\n2. Unused percentage\n";
                        int searchType = getValidatedInput<int>("Choose search type: ", 1, 2);
                        vector<int> results;
                        if (searchType == 1) {
                            cout << "Enter name to search: ";
                            string name;
                            getline(cin, name);
                            results = dm.findStationsByName(name);
                        } else {
                            double minPercent = getValidatedInput<double>("Enter minimum unused percentage (0-100): ", 0.0, 100.0);
                            double maxPercent = getValidatedInput<double>("Enter maximum unused percentage: ", minPercent, 100.0);
                            results = dm.findStationsByUnusedPercentage(minPercent, maxPercent);
                        }
                        displayObjectsByIds(dm, {}, results);
                        break;
                    }

                    case 10:
                        batchEditPipes(dm);
                        break;

                    case 11:
                        batchDeletePipes(dm);
                        break;

                    case 12: {
                        string filename = getFilename();
                        ifstream testFile(filename);
                        if (testFile.good()) {
                            testFile.close();
                            if (!getConfirmation("File exists. Overwrite?")) {
                                break;
                            }
                        }
                        dm.saveToFile(filename);
                        cout << "Data saved successfully!\n";
                        break;
                    }

                    case 13: {
                        string filename = getFilename();
                        ifstream testFile(filename);
                        if (!testFile.good()) {
                            cout << "File does not exist!\n";
                            break;
                        }
                        testFile.close();
                        if (dm.hasPipes() || dm.hasStations()) {
                            if (!getConfirmation("Current data will be lost. Continue?")) {
                                break;
                            }
                        }
                        dm.loadFromFile(filename);
                        cout << "Data loaded successfully!\n";
                        break;
                    }

                    case 0:
                        cout << "Exiting...\n";
                        break;
                }
            } catch (const exception& e) {
                cout << "Error: " << e.what() << endl;
                logger.log("Error", e.what());
            }
        }
    } catch (const exception& e) {
        cout << "Fatal error: " << e.what() << endl;
        logger.log("Fatal error", e.what());
    }

    logger.log("Application closed");
    return 0;
}
