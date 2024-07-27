#ifndef QUEEN_H
#define QUEEN_H

#include "piece.h"

class Queen : public Piece {

public:
    Queen(Color color, Coordinate location, PieceType type,
        std::shared_ptr<Board> board);
    void get_threatened_squares(std::set<Coordinate>& s) override;
};

#endif
