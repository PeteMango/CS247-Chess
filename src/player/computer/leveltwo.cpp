#include "player/computer/leveltwo.h"

LevelTwo::LevelTwo(std::shared_ptr<Game> game, Color color)
    : Computer(game, color)
{
}

LevelTwo::~LevelTwo() { }

void LevelTwo::move()
{
    std::shared_ptr<Board> grid = this->game->get_board();

    std::set<std::pair<Coordinate, Coordinate>> possible_moves;
    grid->get_all_valid_moves(possible_moves, this->color);

    std::set<std::pair<Coordinate, Coordinate>> better_moves;
    for (std::pair<Coordinate, Coordinate> p : possible_moves) {
        if (this->game->get_board()->is_valid_move(p.first, p.second).check
            or this->game->get_board()->is_valid_move(p.first, p.second).capture) {
            better_moves.insert(p);
        }
    }

    if (better_moves.empty()) {
        int index
            = this->random_number(0, possible_moves.size(), possible_moves.size());

        auto it = possible_moves.begin();
        std::advance(it, index);

        std::cout << it->first.column << it->first.row << " " << it->second.column
                  << it->second.row << std::endl;
        this->game->make_move(it->first, it->second, PromotionType::QUEEN);
        return;
    }
    int index = this->random_number(0, better_moves.size(), better_moves.size());

    auto it = better_moves.begin();
    std::advance(it, index);

    this->game->make_move(it->first, it->second, PromotionType::QUEEN);
}
