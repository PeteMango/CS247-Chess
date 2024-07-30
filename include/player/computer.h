#ifndef COMPUTER_H
#define COMPUTER_H

#include "board.h"
#include "game.h"
#include "player.h"
#include "struct/coordinate.h"
#include "struct/move.h"
#include <algorithm>
#include <iterator>
#include <random>
#include <set>

class Computer : public Player {
protected:
    int random_number(int lower, int upper, int size);

public:
    Computer(std::shared_ptr<Game> game, Color color);
    ~Computer() = 0;
    void move() override;
    void execute_moves_based_on_priority(
        std::vector<std::set<std::pair<Coordinate, Coordinate>>> moves);
    void execute_move(std::set<std::pair<Coordinate, Coordinate>> moves);
};
#endif
