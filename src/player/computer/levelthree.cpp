#include "player/computer/levelthree.h"
#include <algorithm>

LevelThree::LevelThree(std::shared_ptr<Game> game, Color color)
    : Computer(game, color)
{
}

LevelThree::~LevelThree() { }

void LevelThree::move()
{
    std::shared_ptr<Board> grid = this->game->get_board();

    std::set<std::pair<Coordinate, Coordinate>> possible_moves;
    grid->get_all_valid_moves(possible_moves, this->color);

    std::set<std::pair<Coordinate, Coordinate>> check, capture, escape;

    /* check for better moves than random */
    for (const std::pair<Coordinate, Coordinate>& p : possible_moves) {
        MoveFlags mf
            = this->game->get_board()->is_valid_move(p.first, p.second, this->color);
        if (!mf.valid) {
            continue;
        }
        if (mf.check) {
            check.insert(p);
        }
        if (mf.capture) {
            capture.insert(p);
        }
        if (mf.attacked_before and !mf.attacked_after) {
            escape.insert(p);
        }
    }

    std::set<std::pair<Coordinate, Coordinate>> check_capture, check_escape,
        capture_escape, all;

    /* moves that checks and captures */
    std::set_intersection(check.begin(), check.end(), capture.begin(), capture.end(),
        std::inserter(check_capture, check_capture.begin()));

    /* moves that checks and escapes */
    std::set_intersection(check.begin(), check.end(), escape.begin(), escape.end(),
        std::inserter(check_escape, check_escape.begin()));

    /* moves that captures and escapes */
    std::set_intersection(capture.begin(), capture.end(), escape.begin(),
        escape.end(), std::inserter(capture_escape, capture_escape.begin()));

    /* moves that do all three */
    std::set_intersection(check_capture.begin(), check_capture.end(), escape.begin(),
        escape.end(), std::inserter(all, all.begin()));

    std::vector<std::set<std::pair<Coordinate, Coordinate>>> moves;
    moves.push_back(all);
    /* escape */
    moves.push_back(capture_escape);
    moves.push_back(check_escape);
    moves.push_back(escape);

    /* capture */
    moves.push_back(check_capture);
    moves.push_back(capture);

    /* check */
    moves.push_back(check);
    moves.push_back(possible_moves);
    this->execute_moves_based_on_priority(moves);
}

