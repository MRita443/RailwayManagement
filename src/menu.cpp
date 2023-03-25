//
// Created by rita on 28-02-2023.
//

#include "menu.h"
#include "station.h"

using namespace std;

unsigned const Menu::COLUMN_WIDTH = 50;
unsigned const Menu::COLUMNS_PER_LINE = 3;
string const Menu::stationsFilePath = "../dataset/stations.csv";
string const Menu::networkFilePath = "../dataset/network.csv";

Menu::Menu() = default;

/**
 * Delegates initialization of the menu, calling the appropriate functions for information extraction and output
 * TODO: Time Complexity
 */
void Menu::initializeMenu() {
    extractFileInfo();
    mainMenu();
}

/**
 * Delegates extracting file info, calling the appropriate functions for each file
 * TODO: Time Complexity
 */
void Menu::extractFileInfo() {
    //extractStationsFile();
    //extractNetworkFile();
}

/**
 * Checks if the input given by the user is appropriate or not
 * Time Complexity: O(1)
 * @param checkLength - Integer indicating if the length of the input should be checked or not, and, if so, its valid max length
 * @return Returns true if the input is appropriate and false if it isn't
 */
bool Menu::checkInput(unsigned int checkLength) {

    /*
    checkLength = 0 Don't check length
    checkLength = 1 Check for length 1
    checkLength = 2 Check for max length 2
    ...
    */

    if (!cin) // User didn't input what expected
    {
        cin.clear(); // Reset failbit
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //Skip bad input
        cout << "Please enter an appropriate input." << endl;
        return false;
    }

    if (checkLength > 0) {
        string line;
        getline(cin, line);
        if (line.length() >= checkLength) {
            cout << "Please enter an appropriate input." << endl;
            return false;
        }
    }
    return true;
}

/**
 * Outputs to the screen a message indicating that the given Station doesn't exist
 * Time Complexity: O(1)
 */
void Menu::stationDoesntExist() {
    cout << "A station with this name doesn't exist!" << endl;
}

/**
 * Outputs main menu screen and calls other menu screens according to user input
 */
void Menu::mainMenu() {

    unsigned char commandIn = '\0';
    string line;

    while (commandIn != 'q') {
        if (commandIn == '\0') { //If program just started or returned from a different menu, print header

            //Header
            cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << setfill('-') << right << "RAILWAY NETWO";
            cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << left << "RK MANAGEMENT" << endl;

            cout << setw(COLUMN_WIDTH) << setfill(' ') << "Basic Service Metrics: [1]" << setw(COLUMN_WIDTH)
                 << "Operation Cost Optimization: [2]" << setw(COLUMN_WIDTH)
                 << "Reliability and Sensitivity to Line Failures: [3]" << endl;
            cout << setw(COLUMN_WIDTH) << "Quit: [q]" << endl;
        }
        cout << endl << "Press the appropriate key to the function you'd like to access: ";
        cin >> commandIn;
        if (!checkInput(1)) {
            commandIn = '\0';
            continue;
        }
        switch (commandIn) {
            case '1': {
                //commandIn = serviceMetricsMenu();
                break;
            }
            case '2': {
                //commandIn = costOptMenu();
                break;
            }
            case '3': {
                //commandIn = sensitivityMenu();
                break;
            }
            case 'q': {
                cout << "Thank you for using our Railway Network Management System!";
                break;
            }
            default: {
                cout << "Please press one of listed keys." << endl;
                break;
            }
        }
    }
}

/**
 * Extracts and stores the information of stations.csv
 * Time Complexity: //TODO
 */
void Menu::extractStationsFile() {
    {
        ifstream stations(stationsFilePath);

        string currentParam, currentLine;
        string name, district, municipality, township, line;

        int counter = 0;

        getline(stations, currentParam); //Ignore first line with just descriptors

        while (getline(stations, currentLine)) {
            istringstream iss(currentLine);
            while (getline(iss, currentParam, ',')) {
                switch (counter++) {
                    case 0: {
                        name = currentParam;
                        break;
                    }
                    case 1: {
                        district = currentParam;
                        break;
                    }
                    case 2: {
                        municipality = currentParam;
                        break;
                    }
                    case 3: {
                        township = currentParam;
                        break;
                    }
                    case 4: {
                        line = currentParam;
                        counter = 0;
                        break;
                    }
                }
                if (counter == 0) {
                    Station newStation = dataRepository.addStationEntry(name, district, municipality, township, line);
                    graph.addVertex(newStation);
                    dataRepository.addAirportToCityEntry(city, country, newStation);

                }
            }
        }
    }
}