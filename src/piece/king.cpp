#include "../../include/piece/king.h"
#include "../../include/board.h"
#include "../../include/piece/rook.h"
#include "util.h"

King::King(
    Color color, Coordinate location, PieceType type, std::shared_ptr<Board> board)
    : Piece(color, location, type, board)
{
    this->directions.insert(this->directions.end(),
        { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { 1, -1 }, { -1, 1 },
            { -1, -1 } });
}

bool King::in_check()
{
    std::set<Coordinate> attacked;
    auto board = this->board.lock();
    board->get_threatened_squares_by_color(attacked, toggle_color(this->color));

    /* cannot move into a spot that would result in a check */
    if (attacked.find(this->location) != attacked.end()) {
        return true;
    }
    return false;
}
bool King::in_checkmate()
{
    std::set<Coordinate> s;
    auto board = this->board.lock();
    board->get_threatened_squares_by_color(s, toggle_color(this->color));
    if (s.find(this->location) == s.end()) {
        return false;
    }
    for (auto i : this->directions) {
        Coordinate cur = this->location;
        std::pair<int, int> cur_idx = get_grid_indexes(cur);
        cur_idx = add_pairs(cur_idx, i);
        if (!coordinate_in_bounds(cur_idx)) {
            continue;
        }
        // make the move
        Coordinate move = Coordinate(cur_idx.first, cur_idx.second);
        if (board->new_valid_move(cur, move, this->color).valid) {
            return false;
        }
    }
    return this->in_check();
}

void King::get_threatened_squares(std::set<Coordinate>& s)
{
    return this->single_move(this->directions, s);
};
