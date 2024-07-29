#ifndef COORDINATE_H
#define COORDINATE_H

#include <iostream>
#include <string>

struct Coordinate {
    int row;
    char column;
    Coordinate();
    Coordinate(std::string coord);
    Coordinate(int row, char column);
    Coordinate(int row, int column);
    bool operator<(const Coordinate& other) const;
    bool operator==(const Coordinate& other) const;
};

std::ostream& operator<<(std::ostream& os, const Coordinate& coord);

#endif
