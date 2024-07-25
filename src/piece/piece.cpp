#include "../../include/piece/piece.h"

Piece::Piece(Color color, Coordinate location, PieceType type)
    : color { color }
    , location { location }
    , type { type }
{
}

Piece::~Piece() { }
Color Piece::get_color() { return this->color; }
PieceType Piece::get_piece_type() { return this->type; }
