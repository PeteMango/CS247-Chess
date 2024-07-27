#ifndef COMPUTER_H
#define COMPUTER_H

#include "player.h"

class Computer : public Player {

public:
    Computer(std::shared_ptr<Game> game, Color color);
    void move() override;
};
#endif
