#ifndef BISHOP_H
#define BISHOP_H

#include "piece.h"

class Bishop : public Piece {
    std::vector<std::pair<int, int>> directions
        = { { 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 } };

public:
    Bishop(Color color, Coordinate location, PieceType type,
        std::shared_ptr<Board> board);
    bool is_valid_move(Coordinate square) override;
    void get_attacking_squares(std::set<Coordinate>& s) override;
};

#endif
