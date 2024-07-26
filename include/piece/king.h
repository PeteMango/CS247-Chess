#ifndef KING_H
#define KING_H

#include "piece.h"
#include <set>

class King : public Piece {
    std::vector<std::pair<int, int>> directions
        = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 },
              { 1, -1 }, { -1, 1 }, { -1, -1 } };

public:
    King(Color color, Coordinate location, PieceType type,
        std::shared_ptr<Board> board);
    bool is_valid_move(Coordinate square) override;
    bool is_check(std::set<Coordinate>& attacked_squares);
    bool is_checkmate(std::set<Coordinate>& attacked_squares);
    void get_attacking_squares(std::set<Coordinate>& s) override;
};

#endif
