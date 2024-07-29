#include "player/computer/levelthree.h"

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
        MoveFlags mf = this->game->get_board()->is_valid_move(p.first, p.second);
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
    std::set_union(check.begin(), check.end(), capture.begin(), capture.end(),
        std::inserter(check_capture, check_capture.begin()));

    /* moves that checks and escapes */
    std::set_union(check.begin(), check.end(), escape.begin(), escape.end(),
        std::inserter(check_escape, check_escape.begin()));

    /* moves that captures and escapes */
    std::set_union(capture.begin(), capture.end(), escape.begin(), escape.end(),
        std::inserter(capture_escape, capture_escape.begin()));

    /* moves that do all three */
    std::set_union(check_capture.begin(), check_capture.end(), escape.begin(),
        escape.end(), std::inserter(all, all.begin()));

    /* priority to moves that does 3/3 */
    if (!all.empty()) {
        return this->execute_move(all);
    }

    /* next in line is moves that do 2/3 */
    if (!check_escape.empty()) {
        return this->execute_move(check_escape);
    }
    if (!check_capture.empty()) {
        return this->execute_move(check_capture);
    }
    if (!capture_escape.empty()) {
        return this->execute_move(capture_escape);
    }

    /* finally, do moves that satisfiy 1/3, in this order s*/
    if (!check.empty()) {
        return this->execute_move(check);
    }
    if (!capture.empty()) {
        return this->execute_move(capture);
    }
    if (!escape.empty()) {
        return this->execute_move(escape);
    }

    /* return any moves possible */
    return this->execute_move(possible_moves);
}

