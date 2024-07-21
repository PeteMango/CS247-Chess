#ifndef MOVE_H
#define MOVE_H

#include "../enum.h"
#include "coordinate.h"
#include <memory>

class Piece;

struct Move {
    std::shared_ptr<Piece> piece;
    Coordinate start;
    Coordinate end;
    PromotionType promotion;
    Move(std::shared_ptr<Piece> piece, Coordinate start,
        Coordinate end, PromotionType promotion);
};

#endif
