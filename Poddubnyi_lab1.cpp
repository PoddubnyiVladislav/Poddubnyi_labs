#include <iostream>
#include <string>
#include <limits>
#include <sstream>
#include <fstream>

using namespace std;

// Константы для идентификации объектов в файле
const string PIPE_IDENTIFIER = "[PIPE]";
const string STATION_IDENTIFIER = "[STATION]";

struct Pipe {
    string name = "";
    int length = 0;
    int diameter = 0;
    bool underRepair = false;
};

struct CompressorStation {
    string name = "";
    unsigned int totalWorkshops = 0;
    unsigned int activeWorkshops = 0;
    int stationClass = 0;
};

// Функция для чтения целой строки и проверки, что она содержит только число
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

// Функция для подтверждения действия
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

// Функция сохранения данных
void saveData(const Pipe& pipe, const CompressorStation& station, bool pipeExists, bool stationExists) {
    string filename;
    cout << "Enter filename to save (without extension): ";
    getline(cin, filename);
    filename += ".txt";

    // Проверка существования файла
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

    // Сохранение трубы
    if (pipeExists) {
        outFile << PIPE_IDENTIFIER << endl;
        outFile << pipe.name << endl;
        outFile << pipe.length << endl;
        outFile << pipe.diameter << endl;
        outFile << pipe.underRepair << endl;
    }

    // Сохранение станции
    if (stationExists) {
        outFile << STATION_IDENTIFIER << endl;
        outFile << station.name << endl;
        outFile << station.totalWorkshops << endl;
        outFile << station.activeWorkshops << endl;
        outFile << station.stationClass << endl;
    }

    outFile.close();
    cout << "Data successfully saved to " << filename << endl;
}

// Функция загрузки данных
void loadData(Pipe& pipe, CompressorStation& station, bool& pipeExists, bool& stationExists) {
    string filename;
    cout << "Enter filename to load (without extension): ";
    getline(cin, filename);
    filename += ".txt";

    ifstream inFile(filename);
    if (!inFile) {
        cout << "Error: Could not open file " << filename << endl;
        return;
    }

    // Проверка на перезапись существующих данных
    if (pipeExists || stationExists) {
        if (!getConfirmation("Current data will be overwritten. Continue?")) {
            cout << "Load cancelled.\n";
            inFile.close();
            return;
        }
    }

    string line;
    Pipe tempPipe;
    CompressorStation tempStation;
    bool loadingPipe = false;
    bool loadingStation = false;
    int lineCount = 0;

    while (getline(inFile, line)) {
        if (line == PIPE_IDENTIFIER) {
            loadingPipe = true;
            loadingStation = false;
            lineCount = 0;
            continue;
        }
        else if (line == STATION_IDENTIFIER) {
            loadingStation = true;
            loadingPipe = false;
            lineCount = 0;
            continue;
        }

        if (loadingPipe) {
            switch (lineCount) {
            case 0: tempPipe.name = line; break;
            case 1: tempPipe.length = stoi(line); break;
            case 2: tempPipe.diameter = stoi(line); break;
            case 3: tempPipe.underRepair = (line == "1"); break;
            }
            lineCount++;
            if (lineCount == 4) {
                pipe = tempPipe;
                pipeExists = true;
                loadingPipe = false;
            }
        }
        else if (loadingStation) {
            switch (lineCount) {
            case 0: tempStation.name = line; break;
            case 1: tempStation.totalWorkshops = stoul(line); break;
            case 2: tempStation.activeWorkshops = stoul(line); break;
            case 3: tempStation.stationClass = stoi(line); break;
            }
            lineCount++;
            if (lineCount == 4) {
                station = tempStation;
                stationExists = true;
                loadingStation = false;
            }
        }
    }

    inFile.close();
    cout << "Data successfully loaded from " << filename << endl;
}

// Функция отображения всех объектов
void viewAllObjects(const Pipe& pipe, const CompressorStation& station, bool pipeExists, bool stationExists) {
    if (pipeExists) {
        cout << "\nPipe: " << pipe.name
            << "\nLength: " << pipe.length << " km"
            << "\nDiameter: " << pipe.diameter << " mm"
            << "\nUnder repair: " << (pipe.underRepair ? "Yes" : "No") << "\n";
    }
    else {
        cout << "No pipe added yet.\n";
    }

    if (stationExists) {
        cout << "\nCompressor Station: " << station.name
            << "\nTotal workshops: " << station.totalWorkshops
            << "\nActive workshops: " << station.activeWorkshops
            << "\nStation class: " << station.stationClass << "\n";
    }
    else {
        cout << "No station added yet.\n";
    }
}

int main() {
    Pipe myPipe;
    CompressorStation myStation;
    bool pipeAdded = false;
    bool stationAdded = false;
    int choice = -1;

    while (true) {
        cout << "\nMain Menu:\n"
            << "1. Add Pipe\n"
            << "2. Add Compressor Station\n"
            << "3. View All Objects\n"
            << "4. Save Data\n"
            << "5. Load Data\n"
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
        case 1: {
            if (pipeAdded) {
                if (!getConfirmation("Pipe already exists. Overwrite?")) {
                    break;
                }
            }

            cout << "Enter pipe name: ";
            getline(cin, myPipe.name);

            myPipe.length = getValidatedNumber<int>("Enter length (km, must be positive): ", 1);
            myPipe.diameter = getValidatedNumber<int>("Enter diameter (mm, must be positive): ", 1);
            myPipe.underRepair = false;
            pipeAdded = true;
            cout << "Pipe added successfully!\n";
            break;
        }

        case 2: {
            if (stationAdded) {
                if (!getConfirmation("Station already exists. Overwrite?")) {
                    break;
                }
            }

            cout << "Enter station name: ";
            getline(cin, myStation.name);

            myStation.totalWorkshops = getValidatedNumber<unsigned int>("Enter total workshops: ", 1);
            myStation.activeWorkshops = getValidatedNumber<unsigned int>(
                "Enter active workshops: ", 0, myStation.totalWorkshops);
            myStation.stationClass = getValidatedNumber<int>("Enter station class: ", 1);
            stationAdded = true;
            cout << "Station added successfully!\n";
            break;
        }

        case 3:
            viewAllObjects(myPipe, myStation, pipeAdded, stationAdded);
            break;

        case 4:
            saveData(myPipe, myStation, pipeAdded, stationAdded);
            break;

        case 5:
            loadData(myPipe, myStation, pipeAdded, stationAdded);
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