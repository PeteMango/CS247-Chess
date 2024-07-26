#ifndef KNIGHT_H
#define KNIGHT_H

#include "piece.h"

class Knight : public Piece {

public:
    Knight(Color color, Coordinate location, PieceType type,
        std::shared_ptr<Board> board);
    void get_threatened_squares(std::set<Coordinate>& s) override;
};

#endif
