#ifndef QUEEN_H
#define QUEEN_H

#include "piece.h"

class Queen : public Piece {
    std::vector<std::pair<int, int>> directions
        = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 },
              { 1, -1 }, { -1, 1 }, { -1, -1 } };

public:
    Queen(Color color, Coordinate location, PieceType type,
        std::shared_ptr<Board> board);
    bool is_valid_move(Coordinate square) override;
    void get_attacking_squares(std::set<Coordinate>& s) override;
};

#endif
