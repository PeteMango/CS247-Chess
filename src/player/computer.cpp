#include "../../include/player/computer.h"

Computer::Computer(std::shared_ptr<Game> game, Color color)
    : Player(game, false, color)
{
}

void Computer::move() { }

Computer::~Computer() { }

int Computer::random_number(int lower, int upper, int size)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(lower, upper);
    int num = distrib(gen);

    return num % size;
}
