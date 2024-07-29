#include "../../include/player/player.h"
#include <stdexcept>

Player::Player(std::shared_ptr<Game> game, bool is_human, Color color)
    : game { game }
    , is_human { is_human }
    , color { color }
{
}

Player::~Player() { }

void Player::make_move(Coordinate start, Coordinate end, PromotionType promotion)
{
    throw std::runtime_error("abstract player class call make move");
}

void Player::move() { throw std::runtime_error("abstract player class call move"); }

bool Player::player_is_human() { return this->is_human; }

Color Player::get_color() { return this->color; }

void Player::resign() { }
