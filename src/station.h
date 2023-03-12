//
// Created by rita on 28-02-2023.
//

#ifndef RAILWAYMANAGEMENT_STATION_H
#define RAILWAYMANAGEMENT_STATION_H

#include <string>

class Station {
private:
    const std::string name;
    const std::string district;
    const std::string municipality;
    const std::string line;
public:
    [[nodiscard]] const std::string &getName() const;
    [[nodiscard]] const std::string &getDistrict() const;
    [[nodiscard]] const std::string &getMunicipality() const;
    [[nodiscard]] const std::string &getLine() const;
};


#endif //RAILWAYMANAGEMENT_STATION_H
