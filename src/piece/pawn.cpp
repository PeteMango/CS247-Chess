#include "../../include/piece/pawn.h"

Pawn::Pawn(Color color, Coordinate location, PieceType type)
    : Piece(color, location, type)
    , double_move_number { -1 }
{
}

bool Pawn::is_valid_move(Coordinate square) { }
