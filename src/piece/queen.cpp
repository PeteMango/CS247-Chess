#include "../../include/piece/queen.h"
#include "../../include/board.h"

Queen::Queen(
    Color color, Coordinate location, PieceType type, std::shared_ptr<Board> board)
    : Piece(color, location, type, board)
{
    this->directions.insert(this->directions.end(),
        { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { 1, -1 }, { -1, 1 },
            { -1, -1 } });
}

void Queen::get_threatened_squares(std::set<Coordinate>& s)
{
    return this->multiple_moves(this->directions, s);
};
