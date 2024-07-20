#ifndef STRUCT_H
#define STRUCT_H

#include "enum.h"
#include <memory>

class Player;
class Piece;

struct Coordinate {
    int row;
    char column;
    Coordinate(int row, int column);
};

struct Players {
    std::unique_ptr<Player> white;
    std::unique_ptr<Player> black;
    Players(std::unique_ptr<Player> white,
        std::unique_ptr<Player> black);
};

struct Move {
    std::shared_ptr<Piece> piece;
    Coordinate start;
    Coordinate end;
    PromotionType promotion;
    Move(std::shared_ptr<Piece> piece, Coordinate start,
        Coordinate end, PromotionType promotion);
};

#endif
