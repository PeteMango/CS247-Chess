#ifndef COMPUTER_H
#define COMPUTER_H

#include "player.h"

class Computer : public Player {
    int level;

public:
    Computer(std::shared_ptr<Player> opponent,
        std::shared_ptr<Game> game, int level);
    void make_move(Coordinate start, Coordinate end,
        PromotionType promotion) override;
};
#endif
