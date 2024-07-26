#include "../../include/piece/pawn.h"

Pawn::Pawn(Color color, Coordinate location, PieceType type,
    std::shared_ptr<Board> board)
    : Piece(color, location, type, board)
{
    if (this->color == Color::WHITE) {
        this->directions = { { 1, 1 }, { 1, -1 } };
    } else {
        this->directions = { { -1, 1 }, { -1, -1 } };
    }
}

bool Pawn::is_valid_move(Coordinate square) { return true; }

void Pawn::get_attacking_squares(std::set<Coordinate>& s)
{
    for (auto i : this->directions) {
        std::pair<int, int> start
            = get_grid_indexes(this->location);
        start = add_pairs(start, i);
        if (coordinate_in_bounds(start)) {
            s.insert(Coordinate(start.first, start.second));
        }
    }
};
