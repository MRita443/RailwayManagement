//
// Created by rita on 28-02-2023.
//

#ifndef RAILWAYMANAGEMENT_MENU_H
#define RAILWAYMANAGEMENT_MENU_H


#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include "graph.h"
#include "dataRepository.h"

class Menu {
private:
    DataRepository dataRepository;
    Graph residualGraph;
    Graph graph;
    std::string static const stationsFilePath;
    std::string static const networkFilePath;
    unsigned static const COLUMN_WIDTH;
    unsigned static const COLUMNS_PER_LINE;

public:
    Menu();

    void extractStationsFile();

    void extractNetworkFile();

    void extractFileInfo();

    void initializeMenu();

    unsigned int serviceMetricsMenu();

    unsigned int costOptMenu();

    unsigned int failuresMenu();

    void mainMenu();

    std::vector<Edge *> edgeFailureMenu();

    static bool checkInput(unsigned int checkLength = 0);

    static void stationDoesntExist();

};


#endif //RAILWAYMANAGEMENT_MENU_H
