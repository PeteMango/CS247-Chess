#include "../../include/piece/pawn.h"
#include "../../include/board.h"

Pawn::Pawn(
    Color color, Coordinate location, PieceType type, std::shared_ptr<Board> board)
    : Piece(color, location, type, board)
{
    if (this->color == Color::WHITE) {
        this->directions.insert(this->directions.end(), { { 1, 0 }, { 2, 0 } });
        this->captures.insert(this->captures.end(), { { 1, 1 }, { 1, -1 } });
    } else {
        this->directions.insert(this->directions.end(), { { -1, 0 }, { -2, 0 } });
        this->captures.insert(this->captures.end(), { { -1, 1 }, { -1, -1 } });
    }
}

void Pawn::get_valid_moves(std::set<Coordinate>& s)
{
    /* has moved, can no longer double move */
    // TODO: definitely fix
    if ((this->color == Color::WHITE and this->location.row != 2)
        or (this->color == Color::BLACK and this->location.row != 7)) {
        if (this->directions.size() >= 2) {
            this->directions.pop_back();
        }
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

