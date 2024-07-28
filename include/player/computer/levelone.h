#ifndef LEVELONE_H
#define LEVELONE_H

#include "board.h"
#include "game.h"
#include "player/computer.h"
#include "struct/coordinate.h"
#include <set>

class LevelOne : public Computer {

public:
    LevelOne(std::shared_ptr<Game> game, Color color);
    ~LevelOne();
    void move() override;
};

#endif
