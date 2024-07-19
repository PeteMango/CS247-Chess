#ifndef COMPUTER_H
#define COMPUTER_H

#include "player.h"

class Computer : public Player {
    int level;

public:
    Computer();
    void make_move(Coordinate start, Coordinate end,
        PromotionType promotion) override;
};
#endif
