#ifndef PLAYER_H
#define PLAYER_H

#include "../enum.h"
#include "../struct/coordinate.h"
#include <memory>

class Game;

class Player {
    std::shared_ptr<Player> opponent;
    std::shared_ptr<Game> game;

public:
    Player(std::shared_ptr<Player> opponent,
        std::shared_ptr<Game> game);
    virtual ~Player();
    virtual void make_move(Coordinate start, Coordinate end,
        PromotionType promotion)
        = 0;
};

#endif
