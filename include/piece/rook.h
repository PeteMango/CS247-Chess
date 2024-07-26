#ifndef ROOK_H
#define ROOK_H

#include "piece.h"

class Rook : public Piece {
    std::vector<std::pair<int, int>> directions
        = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };

public:
    Rook(Color color, Coordinate location, PieceType type,
        std::shared_ptr<Board> board);
    bool is_valid_move(Coordinate square) override;
    void get_attacking_squares(std::set<Coordinate>& s) override;
};

#endif
