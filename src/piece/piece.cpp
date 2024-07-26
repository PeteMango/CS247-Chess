#include "../../include/piece/piece.h"

Piece::Piece(Color color, Coordinate location, PieceType type,
    std::shared_ptr<Board> board)
    : color { color }
    , location { location }
    , type { type }
    , board { board }
{
}

Piece::~Piece() { }
Color Piece::get_color() { return this->color; }
PieceType Piece::get_piece_type() { return this->type; }
Coordinate Piece::get_coordinate() { return this->location; }
void Piece::update_location(Coordinate square)
{
    this->location = square;
}
