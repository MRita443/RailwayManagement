//
// Created by rita on 28-02-2023.
//

#include "station.h"

#include <utility>

Station::Station(std::string name, std::string district, std::string municipality,
                 std::string township,
                 std::string line) : name(std::move(name)), district(std::move(district)), municipality(std::move(municipality)),
                                            township(std::move(township)), line(std::move(line)) {}

Station::Station(std::string name) : name(std::move(name)) {}

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

const std::string &Station::getTownship() const {
    return township;
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

void Station::setTownship(const std::string &township) {
    Station::township = township;
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




