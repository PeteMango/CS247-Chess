#include "player/computer/levelfive.h"

LevelFive::LevelFive(std::shared_ptr<Game> game, Color color)
    : Computer(game, color)
{
}

LevelFive::~LevelFive() { }

void LevelFive::move()
{
    std::shared_ptr<Board> grid = this->game->get_board();

    std::set<std::pair<Coordinate, Coordinate>> possible_moves;
    grid->get_all_valid_moves(possible_moves, this->color);

    // TODO: add piece weights (good trades)

    /* only good capture */
    std::set<std::pair<Coordinate, Coordinate>> capture, check, escape, good_trade;

    /* check for better moves than random */
    for (const std::pair<Coordinate, Coordinate>& p : possible_moves) {
        MoveFlags mf
            = this->game->get_board()->is_valid_move(p.first, p.second, this->color);
        if (!mf.valid) {
            continue;
        }
        if (mf.is_checkmate) {
            std::set<std::pair<Coordinate, Coordinate>> temp;
            temp.insert(p);
            return this->execute_move(temp);
        }
        if (mf.attacked_before and !mf.attacked_after) {
            escape.insert(p);
        }
        if (mf.check and !mf.attacked_after) {
            check.insert(p);
        }
        if (mf.capture and !mf.attacked_after) {
            capture.insert(p);
        }
        if (mf.good_trade) {
            good_trade.insert(p);
        }
    }

    std::set<std::pair<Coordinate, Coordinate>> both;
    std::set_intersection(capture.begin(), capture.end(), check.begin(), check.end(),
        std::inserter(both, both.begin()));

    if (!both.empty()) {
        int max_gain = -100;
        for (const std::pair<Coordinate, Coordinate>& p : both) {
            MoveFlags mf = this->game->get_board()->is_valid_move(
                p.first, p.second, this->color);
            max_gain = std::max(max_gain, mf.gain);
        }
        std::set<std::pair<Coordinate, Coordinate>> to_execute;
        for (const std::pair<Coordinate, Coordinate>& p : both) {
            MoveFlags mf = this->game->get_board()->is_valid_move(
                p.first, p.second, this->color);
            if (mf.gain == max_gain) {
                to_execute.insert(p);
            }
        }
        return this->execute_move(to_execute);
    }
    if (!good_trade.empty()) {
        int max_gain = -100;
        for (const std::pair<Coordinate, Coordinate>& p : good_trade) {
            MoveFlags mf = this->game->get_board()->is_valid_move(
                p.first, p.second, this->color);
            max_gain = std::max(max_gain, mf.gain);
        }
        std::set<std::pair<Coordinate, Coordinate>> to_execute;
        for (const std::pair<Coordinate, Coordinate>& p : good_trade) {
            MoveFlags mf = this->game->get_board()->is_valid_move(
                p.first, p.second, this->color);
            if (mf.gain == max_gain) {
                to_execute.insert(p);
            }
        }
        return this->execute_move(to_execute);
    }

    if (!capture.empty()) {
        int max_gain = -100;
        for (const std::pair<Coordinate, Coordinate>& p : capture) {
            MoveFlags mf = this->game->get_board()->is_valid_move(
                p.first, p.second, this->color);
            max_gain = std::max(max_gain, mf.gain);
        }
        std::set<std::pair<Coordinate, Coordinate>> to_execute;
        for (const std::pair<Coordinate, Coordinate>& p : capture) {
            MoveFlags mf = this->game->get_board()->is_valid_move(
                p.first, p.second, this->color);
            if (mf.gain == max_gain) {
                to_execute.insert(p);
            }
        }
        return this->execute_move(to_execute);
    }

    if (this->game->get_board()->get_pieces(this->color).size() > 3) {
        if (!check.empty()) {
            return this->execute_move(check);
        }
        if (!escape.empty()) {
            return this->execute_move(escape);
        }
    } else {
        if (!escape.empty()) {
            return this->execute_move(escape);
        }
        if (!check.empty()) {
            return this->execute_move(check);
        }
    }

    return this->execute_move(possible_moves);
}
