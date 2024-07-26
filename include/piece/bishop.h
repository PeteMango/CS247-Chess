#ifndef BISHOP_H
#define BISHOP_H

#include "piece.h"

class Bishop : public Piece {

public:
    Bishop(Color color, Coordinate location, PieceType type,
        std::shared_ptr<Board> board);
    void get_threatened_squares(std::set<Coordinate>& s) override;
};

#endif
