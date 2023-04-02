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
 * Time Complexity: O(n*v), where n is the number of lines of network.csv and v is the number of lines in stations.csv
 */
void Menu::extractFileInfo() {
    extractStationsFile();
    extractNetworkFile();
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
                commandIn = serviceMetricsMenu();
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
 * Time Complexity: 0(n) (average case) | O(n²) (worst case), where n is the number of lines of stations.csv
 */
bool Menu::extractStationsFile() {
    {
        ifstream stations(stationsFilePath);

        string currentParam, currentLine;
        string name, district, municipality, township, line;

        int counter = 0;

        getline(stations, currentParam); //Ignore first line with just descriptors

        while (getline(stations, currentLine)) {
            currentLine.erase(currentLine.end() - 1); //Remove \r
            istringstream iss(currentLine);
            while (getline(iss, currentParam, ',')) {
                if (currentParam.find('"') != string::npos) { //If the line contains "
                    string leftover;
                    currentParam.erase(currentParam.begin()); //Erase first "
                    if (currentParam.find('"') != string::npos) { //Closing " already in string
                        currentParam.erase(currentParam.end() - 1); //Remove last "
                    } else {
                        getline(iss, leftover, '"'); //Get leftover string until closing "
                        currentParam.append(leftover);
                        getline(iss, leftover, ','); //Remove trailing ,
                    }
                }
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
                    if (!graph.addVertex(name)) break;
                    Station newStation = dataRepository.addStationEntry(name, district, municipality, township, line);
                    dataRepository.addStationToMunicipalityEntry(municipality, newStation);
                    dataRepository.addStationToDistrictEntry(district, newStation);
                }
            }
        }
    }
}


/**
 * Extracts and stores the information of flights.csv
 * Time Complexity: 0(n*v), where n is the number of lines of network.csv and v is the number of nodes in graph
 * @return Returns true if the whole file was successfully read
 */
bool Menu::extractNetworkFile() {

    ifstream network(networkFilePath);

    string currentParam, currentLine;
    string sourceName, targetName;
    Service service;
    double capacity;

    int counter = 0;

    getline(network, currentParam); //Ignore first line with just descriptors

    while (getline(network, currentLine)) {
        currentLine.erase(currentLine.end() - 1); //Remove \r
        istringstream iss(currentLine);
        while (getline(iss, currentParam, ',')) {
            if (currentParam.find('"') != string::npos) { //If the line contains "
                string leftover;
                currentParam.erase(currentParam.begin()); //Erase first "
                if (currentParam.find('"') != string::npos) { //Closing " already in string
                    currentParam.erase(currentParam.end() - 1); //Remove last "
                } else {
                    getline(iss, leftover, '"'); //Get leftover string until closing "
                    currentParam.append(leftover);
                }
            }
            switch (counter++) {
                case 0: {
                    sourceName = currentParam;
                    if (sourceName == "Tomar")
                        int a = 1;
                    break;
                }
                case 1: {
                    targetName = currentParam;
                    break;
                }
                case 2: {
                    capacity = stod(currentParam);
                    break;
                }
                case 3: {
                    service = currentParam == "STANDARD" ? Service::STANDARD : Service::ALFA_PENDULAR;
                    counter = 0;
                    break;
                }
            }
            if (counter == 0) {
                if (!graph.addBidirectionalEdge(sourceName, targetName, capacity, service)) {
                    cout << "An error occurred. Please try again";
                    return false;
                }
            }
        }
    }
    return true;
}


/**
 * Outputs basic service metrics menu screen and decides graph function calls according to user input
 * @return - Last inputted command, or '\0' for previous menu command
 */
unsigned int Menu::serviceMetricsMenu() {
    unsigned char commandIn = '\0';

    while (commandIn != 'q') {
        if (commandIn == '\0') {
            //Header
            cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << setfill('-') << right << "BASIC SERVI";
            cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << left << "CE METRICS" << endl;
            cout << setw(COLUMN_WIDTH) << setfill(' ') << "Two specific stations: [1]" << setw(COLUMN_WIDTH)
                 << "All valid pairs of stations: [2]" << setw(COLUMN_WIDTH) << "Reaching a specific station: [3]"
                 << endl;
            cout << setw(COLUMN_WIDTH) << setfill(' ') << "Top districts: [4]" << setw(COLUMN_WIDTH)
                 << "Top townships: [5]" << setw(COLUMN_WIDTH) << "Top municipalities: [6]" << endl;
            cout << setw(COLUMN_WIDTH) << "Back: [b]" << setw(COLUMN_WIDTH) << "Quit: [q]" << endl;
        }

        while (commandIn != 'q') {
            cout << endl << "Please select how to input the location whose max number of trains you'd like to check: ";
            cin >> commandIn;

            if (!checkInput(1)) {
                commandIn = '\0';
                continue;
            }
            switch (commandIn) {
                case '1': {
                    string departureName;
                    cout << "Enter the name of the departure station: ";
                    cin >> departureName;
                    if (!checkInput()) break;
                    optional<Station> departureStation = dataRepository.findStation(departureName);
                    if (!departureStation.has_value()) {
                        stationDoesntExist();
                        break;
                    }

                    string arrivalName;
                    cout << "Enter the name of the arrival station: ";
                    cin >> arrivalName;
                    if (!checkInput()) break;
                    optional<Station> arrivalStation = dataRepository.findStation(arrivalName);
                    if (!arrivalStation.has_value()) {
                        stationDoesntExist();
                        break;
                    }
                    cout /*<< graph.edmondsKarp(departureName, arrivalName) TODO: Change Edmonds-Karp return type*/
                            << " trains can simultaneously travel between "
                            << departureName
                            << " and " << arrivalName << "." << endl;
                    break;
                }
                case '2': {
                    pair<list<pair<Station, Station>>, unsigned int> result; //= TODO: Edmonds-Karp through all pairs of stations
                    for (const pair<Station, Station> &p: result.first) {
                        cout << result.second << " trains can simultaneously travel between "
                             << p.first.getName() << " and " << p.second.getName() << "." << endl;
                    }
                    break;
                }
                case '3': {
                    string departureName;
                    cout << "Enter the name of the departure station: ";
                    cin >> departureName;
                    if (!checkInput()) break;
                    optional<Station> departureStation = dataRepository.findStation(departureName);
                    if (!departureStation.has_value()) {
                        stationDoesntExist();
                        break;
                    }
                    cout << /*graph. TODO: Multi-source Edmonds-Karp<<*/ " trains can simultaneously arrive at "
                         << departureName << "." << endl;
                    break;
                }
                case '4': {
                    //TODO: Top Districts Edmonds-Karp
                    break;
                }
                case '5': {
                    //TODO: Top Townships Edmonds-Karp
                    break;
                }
                case '6': {
                    //TODO: Top Municipalities Edmonds-Karp
                    break;
                }
                case 'b': {
                    return '\0';
                }
                case 'q': {
                    cout << "Thank you for using our Railway Network Management System!" << endl;
                    break;
                }
                default:
                    cout << "Please press one of listed keys." << endl;
                    break;
            }
        }
    }
    return commandIn;
}
