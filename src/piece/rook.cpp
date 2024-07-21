#include "../../include/piece/rook.h"

Rook::Rook(Color color, Coordinate location, PieceType type)
    : Piece(color, location, type)
    , has_moved { false }
{
}

bool Rook::is_valid_move(Coordinate square) { }
