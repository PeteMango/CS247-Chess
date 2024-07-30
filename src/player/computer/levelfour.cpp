#include "player/computer/levelfour.h"
#include <algorithm>

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

    // TODO: add piece weights (good trades)

    std::set<std::pair<Coordinate, Coordinate>> check, capture, escape, trade;

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
        if (mf.good_trade) {
            trade.insert(p);
        }
    }

    std::set<std::pair<Coordinate, Coordinate>> check_capture, check_escape,
        check_trade, capture_escape, capture_trade, escape_trade;

    std::set<std::pair<Coordinate, Coordinate>> check_capture_escape,
        check_capture_trade, check_escape_trade, capture_escape_trade;

    std::set<std::pair<Coordinate, Coordinate>> all;

    // 2
    std::set_intersection(check.begin(), check.end(), capture.begin(), capture.end(),
        std::inserter(check_capture, check_capture.begin()));
    std::set_intersection(check.begin(), check.end(), escape.begin(), escape.end(),
        std::inserter(check_escape, check_escape.begin()));
    std::set_intersection(check.begin(), check.end(), trade.begin(), trade.end(),
        std::inserter(check_trade, check_trade.begin()));
    std::set_intersection(capture.begin(), capture.end(), escape.begin(),
        escape.end(), std::inserter(capture_escape, capture_escape.begin()));
    std::set_intersection(capture.begin(), capture.end(), trade.begin(), trade.end(),
        std::inserter(capture_trade, capture_trade.begin()));
    std::set_intersection(escape.begin(), escape.end(), trade.begin(), trade.end(),
        std::inserter(escape_trade, escape_trade.begin()));

    // 3
    std::set_intersection(check_capture.begin(), check_capture.end(),
        capture_escape.begin(), capture_escape.end(),
        std::inserter(check_capture_escape, check_capture.begin()));
    std::set_intersection(check_capture.begin(), check_capture.end(),
        capture_trade.begin(), capture_trade.end(),
        std::inserter(check_capture_trade, check_capture_trade.begin()));
    std::set_intersection(check_escape.begin(), check_escape.end(),
        escape_trade.begin(), escape_trade.end(),
        std::inserter(check_escape_trade, check_escape_trade.begin()));
    std::set_intersection(capture_escape.begin(), capture_escape.end(),
        escape_trade.begin(), escape_trade.end(),
        std::inserter(capture_escape_trade, capture_escape_trade.begin()));

    // 4
    std::set_intersection(check_capture_escape.begin(), check_capture_escape.end(),
        capture_escape_trade.begin(), capture_escape_trade.end(),
        std::inserter(all, all.begin()));

    std::vector<std::set<std::pair<Coordinate, Coordinate>>> moves;
    moves.push_back(all);
    // all not including suicide captures
    /* moves.push_back(check_escape_trade); */
    /* moves.push_back(escape_trade); */
    /* moves.push_back(check_trade); */
    /* moves.push_back(check_escape); */
    // suicide captures
    /* moves.push_back(capture_escape_trade); */
    /* moves.push_back(check_capture_trade); */
    /* moves.push_back(check_capture_escape); */
    /* moves.push_back(capture_trade); */
    /* moves.push_back(capture_escape); */
    /* moves.push_back(check_capture); */
    moves.push_back(trade);
    moves.push_back(escape);
    moves.push_back(check);
    moves.push_back(capture);
    moves.push_back(possible_moves);
    this->execute_moves_based_on_priority(moves);
}
