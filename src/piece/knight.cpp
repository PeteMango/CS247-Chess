#include "../../include/piece/knight.h"
#include "../../include/board.h"

Knight::Knight(
    Color color, Coordinate location, PieceType type, std::shared_ptr<Board> board)
    : Piece(color, location, type, board)
{
    this->directions.insert(this->directions.end(),
        { { 1, 2 }, { 1, -2 }, { -1, 2 }, { -1, -2 }, { 2, 1 }, { 2, -1 }, { -2, 1 },
            { -2, -1 } });
}

void Knight::get_threatened_squares(std::set<Coordinate>& s)
{
    return this->single_move(this->directions, s);
};

