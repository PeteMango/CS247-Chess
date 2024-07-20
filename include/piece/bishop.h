#ifndef BISHOP_H
#define BISHOP_H

#include "piece.h"

class Bishop : public Piece {

public:
    Bishop(Color color, Coordinate location, PieceType type);
    bool is_valid_move(Coordinate square) override;
};

#endif
