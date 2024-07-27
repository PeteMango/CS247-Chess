#ifndef PLAYER_H
#define PLAYER_H

#include "../enum.h"
#include "../struct/coordinate.h"
#include <memory>

class Game;

class Player {
protected:
    std::shared_ptr<Game> game;
    bool is_human;
    Color color;

public:
    Player(
        std::shared_ptr<Game> game, bool is_human, Color color);
    virtual ~Player() = 0;
    virtual void make_move(Coordinate start, Coordinate end,
        PromotionType promotion);
    virtual void move();
    void resign();
    bool player_is_human();
    Color get_color();
};

#endif
