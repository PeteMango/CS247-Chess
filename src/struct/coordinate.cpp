#include "../../include/struct/coordinate.h"

Coordinate::Coordinate()
    : row { -1 }
    , column { '_' }
{
}

// TODO: error checking
Coordinate::Coordinate(std::string coord)
    : row { coord[1] - '0' }
    , column { coord[0] }
{
}

// TODO: error checking
Coordinate::Coordinate(int row, char column)
    : row { row }
    , column { column }
{
}

Coordinate::Coordinate(int row, int column)
    : row { row + 1 }
    , column { static_cast<char>(column + 'a') }
{
}

bool Coordinate::operator<(const Coordinate& other) const
{
    if (row == other.row) {
        return column < other.column;
    }
    return row < other.row;
}

bool Coordinate::operator==(const Coordinate& other) const
{
    return row == other.row && column == other.column;
}

std::ostream& operator<<(std::ostream& os, const Coordinate& coord)
{
    os << coord.column << coord.row;
    return os;
}
