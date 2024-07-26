#ifndef KING_H
#define KING_H

#include "piece.h"
#include <set>

class King : public Piece {

public:
    King(Color color, Coordinate location, PieceType type,
        std::shared_ptr<Board> board);
    bool is_valid_move(Coordinate square) override;
    bool in_check(Coordinate square);
    bool is_checkmate(std::set<Coordinate>& attacked_squares);
    void get_attacking_squares(std::set<Coordinate>& s) override;
};

#endif
