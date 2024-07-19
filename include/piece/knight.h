#ifndef KNIGHT_H
#define KNIGHT_H

#include "piece.h"

class Knight : public Piece {

public:
    Knight();
    bool is_valid_move(Coordinate square) override;
};

#endif
