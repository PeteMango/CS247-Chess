#include "../../include/player/computer.h"

Computer::Computer(std::shared_ptr<Game> game, Color color)
    : Player(game, false, color)
{
}

void Computer::make_move(
    Coordinate start, Coordinate end, PromotionType promotion)
{
}
