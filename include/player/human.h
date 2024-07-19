#ifndef HUMAN_H
#define HUMAN_H

#include "player.h"

class Human : public Player {
public:
    Human();
    void make_move(Coordinate start, Coordinate end,
        PromotionType promotion) override;
    void resign();
};

#endif
