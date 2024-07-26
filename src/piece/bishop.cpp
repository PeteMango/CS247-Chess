#include "../../include/piece/bishop.h"
#include "../../include/board.h"

Bishop::Bishop(
    Color color, Coordinate location, PieceType type, std::shared_ptr<Board> board)
    : Piece(color, location, type, board)
{
    this->directions.insert(
        this->directions.end(), { { 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 } });
}

void Bishop::get_threatened_squares(std::set<Coordinate>& s)
{
    return this->single_move(this->directions, s);
};
