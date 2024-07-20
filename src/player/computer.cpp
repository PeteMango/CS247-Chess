#include "../../include/player/computer.h"

Computer::Computer(std::shared_ptr<Player> opponent,
    std::shared_ptr<Game> game, int level)
    : Player(opponent, game, false)
    , level { level }
{
}

void Computer::make_move(
    Coordinate start, Coordinate end, PromotionType promotion)
{
}
