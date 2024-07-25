#include "../../include/piece/piece.h"

Piece::Piece(Color color, Coordinate location, PieceType type)
    : color { color }
    , location { location }
    , type { type }
{
}

Piece::~Piece() { }

Color Piece::getColor() { return this->color; }
PieceType Piece::getPieceType() { return this->type; }

