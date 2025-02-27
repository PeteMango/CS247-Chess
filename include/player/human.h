#ifndef HUMAN_H
#define HUMAN_H

#include "player.h"

class Human : public Player {
public:
    Human(std::shared_ptr<Game> game, Color color);
    void make_move(Coordinate start, Coordinate end,
        PromotionType promotion) override;
};

#endif
