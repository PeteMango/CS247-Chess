#include "player/computer/levelone.h"

LevelOne::LevelOne(std::shared_ptr<Game> game, Color color)
    : Computer(game, color)
{
}

LevelOne::~LevelOne() { }

void LevelOne::move()
{
    std::shared_ptr<Board> grid = this->game->get_board();

    /* get all valid moves */
    std::set<std::pair<Coordinate, Coordinate>> possible_moves;
    grid->get_all_valid_moves(possible_moves, this->color);

    /* if there are moves, make a random one */
    if (!possible_moves.empty()) {
        return this->execute_move(possible_moves);
    }
    throw std::runtime_error("should have possible moves");
}
