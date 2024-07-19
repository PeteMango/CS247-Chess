#ifndef QUEEN_H
#define QUEEN_H

#include "piece.h"

class Queen : public Piece {

public:
    Queen();
    bool is_valid_move(Coordinate square) override;
};

#endif
