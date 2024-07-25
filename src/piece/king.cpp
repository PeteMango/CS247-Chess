#include "../../include/piece/king.h"

King::King(Color color, Coordinate location, PieceType type)
    : Piece(color, location, type)
    , has_moved { false }
{
}

bool King::is_valid_move(Coordinate square) { return true; }

bool King::is_check(std::set<Coordinate>& attacked_squares)
{
    return true;
}

bool King::is_checkmate(std::set<Coordinate>& attacked_squares)
{
    return true;
}
