#ifndef ROOK_H
#define ROOK_H

#include "piece.h"

class Rook : public Piece {

public:
    Rook(Color color, Coordinate location, PieceType type);
    bool is_valid_move(Coordinate square) override;
};

#endif
