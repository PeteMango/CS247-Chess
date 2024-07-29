#include "player/computer/leveltwo.h"
#include <algorithm> // For std::set_union and std::inserter

LevelTwo::LevelTwo(std::shared_ptr<Game> game, Color color)
    : Computer(game, color)
{
}

LevelTwo::~LevelTwo() { }

void LevelTwo::move()
{
    std::shared_ptr<Board> grid = this->game->get_board();

    std::set<std::pair<Coordinate, Coordinate>> possible_moves, better_moves;
    grid->get_all_valid_moves(possible_moves, this->color);

    /* check for better moves than random */
    for (std::pair<Coordinate, Coordinate> p : possible_moves) {
        MoveFlags mf = this->game->get_board()->is_valid_move(p.first, p.second);
        if (mf.check or mf.capture) {
            better_moves.insert(p);
        }
    }

    /* if there are superior moves, execute them */
    if (!better_moves.empty()) {
        return this->execute_move(better_moves);
    }

    /* execute any moves */
    return this->execute_move(possible_moves);
}
