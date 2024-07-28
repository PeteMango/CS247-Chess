#include "../../include/player/human.h"
#include "../../include/board.h"
#include "../../include/game.h"
#include "../../include/util.h"
#include "enum.h"

Human::Human(std::shared_ptr<Game> game, Color color)
    : Player(game, true, color)
{
}

void Human::make_move(Coordinate start, Coordinate end, PromotionType promotion)
{
    this->game->get_board()->make_move(start, end, promotion);
}
