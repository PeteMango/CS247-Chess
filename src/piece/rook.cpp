#include "../../include/piece/rook.h"
#include "../../include/board.h"

Rook::Rook(
    Color color, Coordinate location, PieceType type, std::shared_ptr<Board> board)
    : Piece(color, location, type, board)
{
    this->directions.insert(
        this->directions.end(), { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } });
}

bool Rook::is_valid_move(Coordinate square)
{
    std::pair<int, int> grid_index = get_grid_indexes(square);

    /* castle, this is the only move where it is okay to go to an occupied square,
    this requires the 'castleable' flag to be set beforehand */
    if (this->castleable
        and this->board->get_grid()[grid_index.first][grid_index.second]->get_color()
            == this->color
        and this->board->get_grid()[grid_index.first][grid_index.second]
                ->get_piece_type()
            == PieceType::KING) {
        return true;
    }

    /* check if the square is occupied */
    if (this->board->get_grid()[grid_index.first][grid_index.second]->get_color()
        == this->color) {
        return false;
    }

    /* check if we can get there */
    std::set<Coordinate> s;
    this->get_attacking_squares(s);
    return (s.find(square) != s.end());
}

void Rook::get_attacking_squares(std::set<Coordinate>& s)
{
    return this->multiple_moves(this->directions, s);
};
