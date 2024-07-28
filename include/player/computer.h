#ifndef COMPUTER_H
#define COMPUTER_H

#include "player.h"
#include <random>

class Computer : public Player {
protected:
    int random_number(int lower, int upper, int size);

public:
    Computer(std::shared_ptr<Game> game, Color color);
    ~Computer();
    void move() override;
};
#endif
