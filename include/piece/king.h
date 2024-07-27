#ifndef KING_H
#define KING_H

#include "piece.h"
#include <set>

class King : public Piece {

public:
    King(Color color, Coordinate location, PieceType type,
        std::shared_ptr<Board> board);
    void get_threatened_squares(std::set<Coordinate>& s) override;
    bool in_check(Coordinate square);
    bool is_checkmate(std::set<Coordinate>& attacked_squares);
};

#endif
