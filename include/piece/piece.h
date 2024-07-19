#ifndef PIECE_H
#define PIECE_H

#include "../enum.h"
#include "../struct.h"

class Piece {
    Color color;
    Coordinate location;
    PieceType type;

public:
    virtual ~Piece();
    virtual bool is_valid_move(Coordinate square) = 0;
};

#endif
