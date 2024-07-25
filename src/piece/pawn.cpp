#include "../../include/piece/pawn.h"

Pawn::Pawn(Color color, Coordinate location, PieceType type)
    : Piece(color, location, type)
{
}

bool Pawn::is_valid_move(Coordinate square) { return true; }
