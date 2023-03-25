//
// Created by rita on 28-02-2023.
//

#include "station.h"

Station::Station(const std::string &name, const std::string &district, const std::string &municipality,
                 const std::string &line) : name(name), district(district), municipality(municipality), line(line) {}

Station::Station(const std::string &name) : name(name) {}

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

void Station::setName(const std::string &name) {
    Station::name = name;
}

void Station::setDistrict(const std::string &district) {
    Station::district = district;
}

void Station::setMunicipality(const std::string &municipality) {
    Station::municipality = municipality;
}

void Station::setLine(const std::string &line) {
    Station::line = line;
}

Station &Station::operator=(const Station &station) {
    this->name = station.name;
    this->district = station.district;
    this->municipality = station.municipality;
    this->line = station.line;
    return *this;
}
