#ifndef ROOK_H
#define ROOK_H

#include "piece.h"

class Rook : public Piece {
    bool has_moved;

public:
    Rook();
    bool is_valid_move(Coordinate square) override;
};

#endif
