#include "../../include/piece/king.h"
#include "../../include/piece/rook.h"
#include "../../include/board.h"

King::King(
    Color color, Coordinate location, PieceType type, std::shared_ptr<Board> board)
    : Piece(color, location, type, board)
{
    this->directions.insert(this->directions.end(),
        { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { 1, -1 }, { -1, 1 },
            { -1, -1 } });
}

bool King::is_valid_move(Coordinate square)
{
    /* check if the space is occupied */
    std::pair<int, int> grid_index = get_grid_indexes(square);
    
    /* check for castle from the rook's castleable flag */
    auto piece = this->board->get_grid()[grid_index.first][grid_index.second];
    if (piece->get_piece_type() == PieceType::ROOK &&
        piece->get_color() == this->color) {

        auto rookPtr = std::dynamic_pointer_cast<Rook>(piece);
        if (rookPtr && rookPtr->castleable) {
            return true;
        }
    }

    /* the square is occupied by a piece that is not the rook */
    if (this->board->get_grid()[grid_index.first][grid_index.second]->get_color()
        == this->color) {
        return false;
    }

    /* TODO: handle if the new square is not attacked because it is blocked by the king */
    if(this->in_check(square)) {
        return false;
    }

    /* check if we can get there */
    std::set<Coordinate> s;
    this->get_attacking_squares(s);
    return (s.find(square) != s.end());
}

bool King::in_check(Coordinate square)
{
    std::set<Coordinate> attacked;

    if (this->color == Color::BLACK) {
        this->board->get_attacked_squares_by_color(attacked, Color::WHITE);
    } else {
        this->board->get_attacked_squares_by_color(attacked, Color::BLACK);
    }

    /* cannot move into a spot that would result in a check */
    if (attacked.find(square) != attacked.end()) {
        return true;
    }
    return false;
}

bool King::is_checkmate(std::set<Coordinate>& attacked_squares) { return true; }

void King::get_attacking_squares(std::set<Coordinate>& s)
{
    return this->single_move(this->directions, s);
};
