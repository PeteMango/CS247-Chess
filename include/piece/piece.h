#ifndef PIECE_H
#define PIECE_H

#include "../enum.h"
#include "../struct/coordinate.h"

class Piece {
    Color color;
    Coordinate location;
    PieceType type;

public:
    Piece(Color color, Coordinate location, PieceType type);
    virtual ~Piece();
    virtual bool is_valid_move(Coordinate square) = 0;
};

#endif
