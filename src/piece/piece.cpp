#include "../../include/piece/piece.h"

Piece::Piece(Color color, Coordinate location, PieceType type)
    : color { color }
    , location { location }
    , type { type }
{
}

Piece::~Piece() { }

