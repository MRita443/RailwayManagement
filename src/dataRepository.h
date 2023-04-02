//
// Created by rita on 17-03-2023.
//

#ifndef RAILWAYMANAGEMENT_DATAREPOSITORY_H
#define RAILWAYMANAGEMENT_DATAREPOSITORY_H

#include <list>
#include <optional>
#include <algorithm>
#include "station.h"


class DataRepository {


private:
    stationTable stations;
    std::unordered_map<std::string, std::list<Station>> districtToStations;
    std::unordered_map<std::string, std::list<Station>> municipalityToStations;
    std::unordered_map<std::string, std::list<Station>> townshipToStations;

public:
    DataRepository();

    const stationTable &getStations() const;

    void setStations(const stationTable &Stations);

    const std::unordered_map<std::string, std::list<Station>> &getDistrictToStations() const;

    void setDistrictToStations(const std::unordered_map<std::string, std::list<Station>> &districtToStations);

    const std::unordered_map<std::string, std::list<Station>> &getTownshipToStations() const;

    void setTownshipToStations(const std::unordered_map<std::string, std::list<Station>> &townshipToStations);

    const std::unordered_map<std::string, std::list<Station>> &getMunicipalityToStations() const;

    void setMunicipalityToStations(const std::unordered_map<std::string, std::list<Station>> &municipalityToStations);

    std::optional<Station> findStation(const std::string &name);

    Station addStationEntry(const std::string &name, const std::string &district, const std::string &municipality,
                             const std::string &township,
                             const std::string &line);

    void addStationToDistrictEntry(const std::string &district, const Station &station);

    void addStationToMunicipalityEntry(const std::string &municipality, const Station &station);

    std::list<Station> findStationsInDistrict(const std::string &district);

    bool checkValidDistrict(const std::string &district);

    bool checkValidMunicipality(const std::string &municipality);

    std::list<Station> findStationsInMunicipality(const std::string &municipality);

    std::list<Station> findStationsInTownship(const std::string &township);

    void addStationToTownshipEntry(const std::string &township, const Station &station);

    bool checkValidTownship(const std::string &township);
};


#endif //RAILWAYMANAGEMENT_DATAREPOSITORY_H
