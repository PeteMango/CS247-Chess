#include "player/computer/levelone.h"

LevelOne::LevelOne(std::shared_ptr<Game> game, Color color)
    : Computer(game, color)
{
}

LevelOne::~LevelOne() { }

void LevelOne::move()
{
    // Get the game board
    std::shared_ptr<Board> grid = this->game->get_board();

    // Get all valid moves
    std::set<std::pair<Coordinate, Coordinate>> possible_moves;
    grid->get_all_valid_moves(possible_moves, this->color);

    if (possible_moves.empty()) {
        return;
    }

    int index
        = this->random_number(0, possible_moves.size() - 1, possible_moves.size());

    auto it = possible_moves.begin();
    std::advance(it, index);

    this->game->make_move(it->first, it->second, PromotionType::QUEEN);
}
