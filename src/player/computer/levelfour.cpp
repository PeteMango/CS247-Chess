#include "player/computer/levelfour.h"

LevelFour::LevelFour(std::shared_ptr<Game> game, Color color)
    : Computer(game, color)
{
}

LevelFour::~LevelFour() { }

void LevelFour::move()
{
    return;
    throw std::runtime_error("not implemented");
}

