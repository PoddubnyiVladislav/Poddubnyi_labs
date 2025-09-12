#include <iostream>
#include <string>
#include <limits>
#include <sstream>

using namespace std;

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

        // Пытаемся прочитать число
        if (ss >> value) {
            // Проверяем, что после числа нет других символов
            char remaining;
            if (ss >> remaining) {
                cout << "Invalid input! Please enter only a number without extra characters.\n";
                continue;
            }

            // Проверяем диапазон
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

int main() {
    Pipe myPipe;
    CompressorStation myStation;
    bool pipeAdded = false;
    bool stationAdded = false;
    int choice = -1;
    char confirm;

    while (true) {
        cout << "\nMain Menu:\n"
            << "1. Add Pipe\n"
            << "2. Add Compressor Station\n"
            << "3. View All Objects\n"
            << "0. Exit\n"
            << "Choose action: ";

        string input;
        getline(cin, input);
        stringstream ss(input);

        if (!(ss >> choice)) {
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        // Проверяем, что после числа нет других символов
        char remaining;
        if (ss >> remaining) {
            cout << "Invalid input! Please enter only a number without extra characters.\n";
            continue;
        }

        if (choice == 1) {
            if (pipeAdded) {
                cout << "Pipe already exists. Overwrite? (y/n): ";
                getline(cin, input);
                if (input != "y" && input != "Y") {
                    continue;
                }
            }

            cout << "Enter pipe name: ";
            getline(cin, myPipe.name);

            // Валидация длины трубы
            myPipe.length = getValidatedNumber<int>("Enter length (km, must be positive): ", 1);

            // Валидация диаметра трубы
            myPipe.diameter = getValidatedNumber<int>("Enter diameter (mm, must be positive): ", 1);

            myPipe.underRepair = false;
            pipeAdded = true;
            cout << "Pipe added successfully!\n";

        }
        else if (choice == 2) {
            if (stationAdded) {
                cout << "Station already exists. Overwrite? (y/n): ";
                getline(cin, input);
                if (input != "y" && input != "Y") {
                    continue;
                }
            }

            cout << "Enter station name: ";
            getline(cin, myStation.name);

            // Валидация общего количества цехов
            myStation.totalWorkshops = getValidatedNumber<unsigned int>("Enter total workshops: ", 1);

            // Валидация активных цехов (не может быть больше общего количества)
            myStation.activeWorkshops = getValidatedNumber<unsigned int>(
                "Enter active workshops: ",
                0,
                myStation.totalWorkshops
            );

            // Валидация класса станции
            myStation.stationClass = getValidatedNumber<int>("Enter station class: ", 1);

            stationAdded = true;
            cout << "Station added successfully!\n";

        }
        else if (choice == 3) {
            if (pipeAdded) {
                cout << "\nPipe: " << myPipe.name
                    << "\nLength: " << myPipe.length << " km"
                    << "\nDiameter: " << myPipe.diameter << " mm"
                    << "\nUnder repair: " << (myPipe.underRepair ? "Yes" : "No") << "\n";
            }
            else {
                cout << "No pipe added yet.\n";
            }

            if (stationAdded) {
                cout << "\nCompressor Station: " << myStation.name
                    << "\nTotal workshops: " << myStation.totalWorkshops
                    << "\nActive workshops: " << myStation.activeWorkshops
                    << "\nStation class: " << myStation.stationClass << "\n";
            }
            else {
                cout << "No station added yet.\n";
            }

        }
        else if (choice == 0) {
            cout << "Exiting program...\n";
            break;

        }
        else {
            cout << "Invalid choice! Try again.\n";
        }
    }

    return 0;
}