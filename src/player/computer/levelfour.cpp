#include "player/computer/levelfour.h"
#include <limits.h>

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

    std::set<std::pair<Coordinate, Coordinate>> good_captures;
    std::set<std::pair<Coordinate, Coordinate>> good_checks;
    std::set<std::pair<Coordinate, Coordinate>> good_moves;
    std::set<std::pair<Coordinate, Coordinate>> good_captures_moves_checks;
    std::set<std::pair<Coordinate, Coordinate>> good_captures_checks;
    std::set<std::pair<Coordinate, Coordinate>> good_checks_moves;
    std::set<std::pair<Coordinate, Coordinate>> good_captures_moves;

    int weight_difference_min = INT_MAX;
    int weight_difference_max = INT_MIN;
    int sum = 0;
    for (const std::pair<Coordinate, Coordinate>& p : possible_moves) {
        MoveFlags mf = this->game->get_board()->is_valid_move(p.first, p.second);
        if (!mf.valid) {
            continue;
        }
        if (mf.is_checkmate) {
            std::set<std::pair<Coordinate, Coordinate>> temp;
            temp.insert(p);
            return this->execute_move(temp);
        }
        if (mf.capture_value_difference >= 0) {
            good_captures.insert(p);
        }
        weight_difference_min
            = std::min(weight_difference_min, mf.weight_difference);
        weight_difference_max
            = std::max(weight_difference_max, mf.weight_difference);
        sum += mf.weight_difference;
    }

    int mean = sum / possible_moves.size();
    for (const std::pair<Coordinate, Coordinate>& p : possible_moves) {
        MoveFlags mf = this->game->get_board()->is_valid_move(p.first, p.second);
        if (!mf.valid) {
            continue;
        }
        if (mean <= mf.weight_difference) {
            good_moves.insert(p);
        }
    }

    for (const std::pair<Coordinate, Coordinate>& p : possible_moves) {
        MoveFlags mf = this->game->get_board()->is_valid_move(p.first, p.second);
        if (!mf.valid) {
            continue;
        }
        if (mf.good_check) {
            good_checks.insert(p);
        }
    }

    std::set_intersection(good_captures.begin(), good_captures.end(),
        good_checks.begin(), good_checks.end(),
        std::inserter(good_captures_checks, good_captures_checks.begin()));

    // std::set_intersection(good_captures.begin(), good_captures.end(),
    //     good_moves.begin(), good_moves.end(),
    //     std::inserter(good_captures_moves, good_captures_moves.begin()));

    // std::set_intersection(good_checks.begin(), good_checks.end(),
    // good_moves.begin(),
    //     good_moves.end(),
    //     std::inserter(good_checks_moves, good_checks_moves.begin()));

    // std::set_intersection(good_checks_moves.begin(), good_checks_moves.end(),
    //     good_captures_moves.begin(), good_captures_moves.end(),
    //     std::inserter(
    //         good_captures_moves_checks, good_captures_moves_checks.begin()));

    // if (!good_captures_moves_checks.empty()) {
    //     return this->execute_move(good_captures_moves_checks);
    // }

    if (!good_captures_checks.empty()) {
        return this->execute_move(good_captures_checks);
    }

    // if (!good_captures_moves.empty()) {
    //     return this->execute_move(good_captures_moves);
    // }

    // if (!good_checks_moves.empty()) {
    //     return this->execute_move(good_checks_moves);
    // }

    if (!good_captures.empty()) {
        return this->execute_move(good_captures);
    }

    if (!good_checks.empty()) {
        return this->execute_move(good_checks);
    }

    if (!good_moves.empty()) {
        return this->execute_move(good_moves);
    }
    /* return any moves possible */
    return this->execute_move(possible_moves);
}

