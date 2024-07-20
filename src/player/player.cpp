#include "../../include/player/player.h"

Player::Player(
    std::shared_ptr<Player> opponent, std::shared_ptr<Game> game)
    : opponent { opponent }
    , game { game }
{
}

Player::~Player() { }
