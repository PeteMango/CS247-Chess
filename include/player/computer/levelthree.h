#ifndef LEVELTHREE_H
#define LEVELTHREE_H

#include "player/computer.h"

class LevelThree : public Computer {

public:
    LevelThree(std::shared_ptr<Game> game, Color color);
    ~LevelThree();
    void move() override;
};

#endif
