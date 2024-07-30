#include "player/computer/levelfive.h"

LevelFive::LevelFive(std::shared_ptr<Game> game, Color color)
    : Computer(game, color)
{
}

LevelFive::~LevelFive() { }

void LevelFive::move()
{
    std::shared_ptr<Board> grid = this->game->get_board();

    /* get all valid moves */
    std::set<std::pair<Coordinate, Coordinate>> possible_moves;
    grid->get_all_valid_moves(possible_moves, this->color);
}
