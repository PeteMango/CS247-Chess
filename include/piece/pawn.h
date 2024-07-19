#ifndef PAWN_H
#define PAWN_H

#include "piece.h"

class Pawn : public Piece {
    int double_move_number;

public:
    Pawn();
    bool is_valid_move(Coordinate square) override;
};

#endif
