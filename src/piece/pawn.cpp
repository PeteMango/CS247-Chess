#include "../../include/piece/pawn.h"
#include "../../include/board.h"
#include "util.h"

Pawn::Pawn(
    Color color, Coordinate location, PieceType type, std::shared_ptr<Board> board)
    : Piece(color, location, type, board)
    , can_double { true }
{
    if (this->color == Color::WHITE) {
        this->directions.insert(this->directions.end(), { { 1, 0 } });
        this->captures.insert(this->captures.end(), { { 1, 1 }, { 1, -1 } });
        this->double_direction = std::make_pair(1, 0);
    } else {
        this->directions.insert(this->directions.end(), { { -1, 0 } });
        this->captures.insert(this->captures.end(), { { -1, 1 }, { -1, -1 } });
        this->double_direction = std::make_pair(-1, 0);
    }
}

void Pawn::get_valid_moves(std::set<Coordinate>& s)
{
    /* has moved, can no longer double move */
    // TODO: definitely fix
    if ((this->color == Color::WHITE and this->location.row != 2)
        or (this->color == Color::BLACK and this->location.row != 7)) {
        this->can_double = false;
    }

    /* handle captures */
    for (auto i : this->captures) {
        std::pair<int, int> start = get_grid_indexes(this->location);
        start = add_pairs(start, i);

        auto board = this->board.lock();
        if (coordinate_in_bounds(start)
            and board->get_grid()[start.first][start.second]
            and board->get_grid()[start.first][start.second]->get_color()
                != this->color) {
            s.insert(Coordinate { start.first, start.second });
        }
    }
    /* handles moving forward */
    for (auto i : this->directions) {
        std::pair<int, int> start = get_grid_indexes(this->location);
        start = add_pairs(start, i);

        auto board = this->board.lock();
        if (coordinate_in_bounds(start)
            and !board->get_grid()[start.first][start.second]) {
            s.insert(Coordinate { start.first, start.second });
        }
    }

    if (this->can_double) {
        std::pair<int, int> start = get_grid_indexes(this->location);
        auto board = this->board.lock();
        bool add = true;
        for (int i = 0; i < 2; i++) {
            start = add_pairs(start, this->double_direction);
            if (!coordinate_in_bounds(start)) {
                add = false;
                break;
            }
            if (board->get_grid()[start.first][start.second]) {
                add = false;
                break;
            }
        }
        if (add) {
            s.insert(Coordinate { start.first, start.second });
        }
    }
}

void Pawn::get_threatened_squares(std::set<Coordinate>& s)
{
    /* handle captures */
    for (auto i : this->captures) {
        std::pair<int, int> start = get_grid_indexes(this->location);
        start = add_pairs(start, i);

        if (coordinate_in_bounds(start)) {
            s.insert(Coordinate { start.first, start.second });
        }
    }
}

std::vector<std::pair<int, int>>& Pawn::get_captures() { return this->captures; }

