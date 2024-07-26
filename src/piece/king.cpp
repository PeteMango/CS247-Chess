#include "../../include/piece/king.h"

King::King(Color color, Coordinate location, PieceType type,
    std::shared_ptr<Board> board)
    : Piece(color, location, type, board)
{
}

bool King::is_valid_move(Coordinate square) { return true; }

bool King::is_check(std::set<Coordinate>& attacked_squares)
{
    return true;
}

bool King::is_checkmate(std::set<Coordinate>& attacked_squares)
{
    return true;
}

void King::get_attacking_squares(std::set<Coordinate>& s)
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
