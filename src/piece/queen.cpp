#include "../../include/piece/queen.h"
#include "../../include/board.h"

Queen::Queen(Color color, Coordinate location, PieceType type,
    std::shared_ptr<Board> board)
    : Piece(color, location, type, board)
{
}

bool Queen::is_valid_move(Coordinate square) { return true; }

void Queen::get_attacking_squares(std::set<Coordinate>& s)
{
    for (auto i : this->directions) {
        std::pair<int, int> start
            = get_grid_indexes(this->location);
        start = add_pairs(start, i);

        while (coordinate_in_bounds(start)
            && this->board->get_grid()[start.first][start.second]
                == nullptr) {
            s.insert(Coordinate(start.first, start.second));
            start = add_pairs(start, i);
        }
        if (coordinate_in_bounds(start)) {
            s.insert(Coordinate(start.first, start.second));
        }
    }
};
