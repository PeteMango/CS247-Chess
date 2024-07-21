#include "../../include/player/human.h"

Human::Human(
    std::shared_ptr<Player> opponent, std::shared_ptr<Game> game)
    : Player(opponent, game, true)
{
}

void Human::make_move(
    Coordinate start, Coordinate end, PromotionType promotion)
{
}

void Human::resign() { }

