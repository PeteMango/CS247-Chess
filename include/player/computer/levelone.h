#ifndef LEVELONE_H
#define LEVELONE_H

#include "player/computer.h"

class LevelOne : public Computer {

public:
    LevelOne(std::shared_ptr<Game> game, Color color);
    ~LevelOne();
    void move() override;
};

#endif
