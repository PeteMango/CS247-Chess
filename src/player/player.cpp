#include "../../include/player/player.h"

Player::Player(std::shared_ptr<Player> opponent,
    std::shared_ptr<Game> game, bool is_human)
    : opponent { opponent }
    , game { game }
    , is_human { is_human }
{
}

Player::~Player() { }

bool Player::player_is_human() { }
