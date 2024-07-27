#ifndef ROOK_H
#define ROOK_H

#include "piece.h"

class Rook : public Piece {

public:
    bool castleable;
    Rook(Color color, Coordinate location, PieceType type,
        std::shared_ptr<Board> board);
    void get_threatened_squares(std::set<Coordinate>& s) override;
};

#endif
