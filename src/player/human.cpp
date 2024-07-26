#include "../../include/player/human.h"

Human::Human(std::shared_ptr<Game> game, Color color)
    : Player(game, true, color)
{
}

void Human::make_move(
    Coordinate start, Coordinate end, PromotionType promotion)
{
}

void Human::resign() { }

