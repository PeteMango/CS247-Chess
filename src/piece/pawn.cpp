#include "../../include/piece/pawn.h"
#include "../../include/board.h"

Pawn::Pawn(
    Color color, Coordinate location, PieceType type, std::shared_ptr<Board> board)
    : Piece(color, location, type, board)
{
    if (this->color == Color::WHITE) {
        this->directions.insert(this->directions.end(), { { 1, 1 }, { 1, -1 } });
    } else {
        this->directions.insert(this->directions.end(), { { -1, 1 }, { -1, -1 } });
    }
}

bool Pawn::is_valid_move(Coordinate square)
{
    /* check if the space is occupied */
    std::pair<int, int> grid_index = get_grid_indexes(square);
    if (this->board->get_grid()[grid_index.first][grid_index.second]->get_color()
        == this->color) {
        return false;
    }

    /* check if we can get there */
    std::set<Coordinate> s;
    this->get_attacking_squares(s);
    return (s.find(square) != s.end());
}

void Pawn::get_attacking_squares(std::set<Coordinate>& s)
{
    return this->single_move(this->directions, s);
};
