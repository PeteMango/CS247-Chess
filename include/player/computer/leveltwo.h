#ifndef LEVELTWO_H
#define LEVELTWO_H

#include "board.h"
#include "game.h"
#include "player/computer.h"
#include "struct/coordinate.h"
#include <set>

class LevelTwo : public Computer {

public:
    LevelTwo(std::shared_ptr<Game> game, Color color);
    ~LevelTwo();
    void move() override;
};

#endif
