#ifndef PAWN_H
#define PAWN_H

#include "piece.h"

class Pawn : public Piece {

    std::vector<std::pair<int, int>> directions;

public:
    Pawn(Color color, Coordinate location, PieceType type,
        std::shared_ptr<Board> board);
    bool is_valid_move(Coordinate square) override;
    void get_attacking_squares(std::set<Coordinate>& s) override;
};

#endif
