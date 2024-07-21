#ifndef KING_H
#define KING_H

#include "piece.h"
#include <set>

class King : public Piece {
    bool has_moved;

public:
    King(Color color, Coordinate location, PieceType type);
    bool is_valid_move(Coordinate square) override;
    bool is_check(std::set<Coordinate>& attacked_squares);
    bool is_checkmate(std::set<Coordinate>& attacked_squares);
};

#endif
