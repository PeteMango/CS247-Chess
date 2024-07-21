#include "../../include/piece/bishop.h"

Bishop::Bishop(Color color, Coordinate location, PieceType type)
    : Piece(color, location, type)
{
}

bool Bishop::is_valid_move(Coordinate square) { }
