#include <iostream>
#include <string>

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

        cin >> choice;

        if (choice == 1) {
            if (pipeAdded) {
                cout << "Pipe already exists. Overwrite? (y/n): ";
                cin >> confirm;
                if (confirm != 'y' && confirm != 'Y') {
                    continue;
                }
            }

            cout << "Enter pipe name: ";
            cin.ignore();
            getline(cin, myPipe.name);

            cout << "Enter length (km): ";
            cin >> myPipe.length;

            cout << "Enter diameter (mm): ";
            cin >> myPipe.diameter;

            myPipe.underRepair = false;
            pipeAdded = true;
            cout << "Pipe added successfully!\n";

        }
        else if (choice == 2) {
            if (stationAdded) {
                cout << "Station already exists. Overwrite? (y/n): ";
                cin >> confirm;
                if (confirm != 'y' && confirm != 'Y') {
                    continue;
                }
            }

            cout << "Enter station name: ";
            cin.ignore();
            getline(cin, myStation.name);

            cout << "Enter total workshops: ";
            cin >> myStation.totalWorkshops;

            cout << "Enter active workshops: ";
            cin >> myStation.activeWorkshops;

            cout << "Enter station class: ";
            cin >> myStation.stationClass;

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