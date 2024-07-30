#ifndef LEVELFIVE_H
#define LEVELFIVE_H

#include "player/computer.h"

class LevelFive : public Computer {

public:
    LevelFive(std::shared_ptr<Game> game, Color color);
    ~LevelFive();
    void move() override;
};

#endif
