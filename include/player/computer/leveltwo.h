#ifndef LEVELTWO_H
#define LEVELTWO_H

#include "player/computer.h"

class LevelTwo : public Computer {

public:
    LevelTwo(std::shared_ptr<Game> game, Color color);
    ~LevelTwo();
    void move() override;
};

#endif
