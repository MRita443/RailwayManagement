//
// Created by rita on 28-02-2023.
//

#include "station.h"

//Getters

const std::string &Station::getName() const {
    return name;
}

const std::string &Station::getDistrict() const {
    return district;
}

const std::string &Station::getMunicipality() const {
    return municipality;
}

const std::string &Station::getLine() const {
    return line;
}
