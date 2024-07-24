#ifndef COORDINATE_H
#define COORDINATE_H

#include <string>

struct Coordinate {
    int row;
    char column;
    Coordinate();
    Coordinate(std::string coord);
    Coordinate(int row, char column);
};

#endif
