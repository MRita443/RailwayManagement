//
// Created by rita on 28-02-2023.
//

#ifndef RAILWAYMANAGEMENT_STATION_H
#define RAILWAYMANAGEMENT_STATION_H

#include <string>
#include <unordered_map>
#include <unordered_set>

class Station {
private:
    std::string name;
    std::string district;
    std::string municipality;
    std::string township;
    std::string line;
public:
    Station(std::string name, std::string district, std::string municipality,
            std::string township,
            std::string line);

    explicit Station(std::string name);

    [[nodiscard]] const std::string &getName() const;

    [[nodiscard]] const std::string &getDistrict() const;

    [[nodiscard]] const std::string &getMunicipality() const;

    [[nodiscard]] const std::string &getTownship() const;

    [[nodiscard]] const std::string &getLine() const;

    void setName(const std::string &name);

    void setDistrict(const std::string &district);

    void setMunicipality(const std::string &municipality);

    void setTownship(const std::string &township);

    void setLine(const std::string &line);

    Station &operator=(const Station &station);
};


struct StationHash {
    std::size_t operator()(const Station &Station) const {
        return std::hash<std::string>()(Station.getName());
    }
};

struct StationEquals {
    bool operator()(const Station &Station1, const Station &Station2) const {
        return Station1.getName() == Station2.getName();
    }
};

typedef std::unordered_set<Station, StationHash, StationEquals> stationTable;

template<typename T>
using StationMap = std::unordered_map<Station, T, StationHash, StationEquals>;


#endif //RAILWAYMANAGEMENT_STATION_H
