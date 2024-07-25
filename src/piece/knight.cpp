#include "../../include/piece/knight.h"

Knight::Knight(Color color, Coordinate location, PieceType type)
    : Piece(color, location, type)
{
}

bool Knight::is_valid_move(Coordinate square) { return true; }
