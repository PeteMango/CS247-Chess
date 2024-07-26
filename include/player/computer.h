#ifndef COMPUTER_H
#define COMPUTER_H

#include "player.h"

class Computer : public Player {

public:
    Computer(std::shared_ptr<Game> game, Color color);
    void make_move(Coordinate start, Coordinate end,
        PromotionType promotion) override;
};
#endif
