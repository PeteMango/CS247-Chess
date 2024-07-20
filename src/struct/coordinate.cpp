#include "../../include/struct/coordinate.h"

Coordinate::Coordinate()
    : row { -1 }
    , column { '_' }
{
}

// TODO: error checking
Coordinate::Coordinate(int row, char column)
    : row { row }
    , column { column }
{
}
