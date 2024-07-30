#include "player/computer/levelfour.h"

LevelFour::LevelFour(std::shared_ptr<Game> game, Color color)
    : Computer(game, color)
{
}

LevelFour::~LevelFour() { }

void LevelFour::move()
{
    std::shared_ptr<Board> grid = this->game->get_board();

    std::set<std::pair<Coordinate, Coordinate>> possible_moves;
    grid->get_all_valid_moves(possible_moves, this->color);

    /* check for better moves than random */
    for (const std::pair<Coordinate, Coordinate>& p : possible_moves) {
        MoveFlags mf = this->game->get_board()->is_valid_move(p.first, p.second);
        std::cout << p.first << " " << p.second << " " << mf.score_difference << " "
                  << mf.protected_diff << " " << mf.attacked_diff << std::endl;
        // std::cout << p.first.column << p.first.row << " " << p.second.column
        //   << p.second.row << " " << mf.score_difference << std::endl;
    }

    /* return any moves possible */
    return this->execute_move(possible_moves);
}

