#include "../../include/piece/queen.h"

Queen::Queen(Color color, Coordinate location, PieceType type)
    : Piece(color, location, type)
{
}

bool Queen::is_valid_move(Coordinate square) { return true; }
