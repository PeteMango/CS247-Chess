#include "../../include/piece/pawn.h"
#include "../../include/board.h"

Pawn::Pawn(
    Color color, Coordinate location, PieceType type, std::shared_ptr<Board> board)
    : Piece(color, location, type, board)
{
    if (this->color == Color::WHITE) {
        this->directions.insert(this->directions.end(), { { 1, 0 } });
        this->captures.insert(this->captures.end(), { { 1, 1 }, { 1, -1 } });
    } else {
        this->directions.insert(this->directions.end(), { { -1, 0 } });
        this->captures.insert(this->captures.end(), { { -1, 1 }, { -1, -1 } });
    }
}

void Pawn::get_valid_moves(std::set<Coordinate>& s)
{
    /* handle captures */
    for (auto i : this->captures) {
        std::pair<int, int> start = get_grid_indexes(this->location);
        start = add_pairs(start, i);

        if (coordinate_in_bounds(start)
            and this->board->get_grid()[start.first][start.second]
            and this->board->get_grid()[start.first][start.second]->get_color()
                != this->color) {
            s.insert(Coordinate { start.first, start.second });
        }
    }
    /* handles moving forward */
    for (auto i : this->directions) {
        std::pair<int, int> start = get_grid_indexes(this->location);
        start = add_pairs(start, i);

        if (coordinate_in_bounds(start)
            and !this->board->get_grid()[start.first][start.second]) {
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