#ifndef BISHOP_H
#define BISHOP_H

#include "piece.h"

class Bishop : public Piece {

public:
    Bishop();
    bool is_valid_move(Coordinate square) override;
};

#endif
