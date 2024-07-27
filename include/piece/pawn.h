#ifndef PAWN_H
#define PAWN_H

#include "piece.h"

class Pawn : public Piece {
    std::vector<std::pair<int, int>> captures;

public:
    Pawn(Color color, Coordinate location, PieceType type,
        std::shared_ptr<Board> board);
    void get_valid_moves(std::set<Coordinate>& s) override;
    void get_threatened_squares(std::set<Coordinate>& s) override;
    std::vector<std::pair<int, int>>& get_captures();
};

#endif
