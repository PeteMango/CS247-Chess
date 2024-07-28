#ifndef LEVELONE_H
#define LEVELONE_H

#include "board.h"
#include "game.h"
#include "player/computer.h"
#include "struct/coordinate.h"
#include <set>

class levelone : public Computer {

public:
    levelone(std::shared_ptr<Game> game, Color color);
    ~levelone();
    void move() override;
};

#endif
