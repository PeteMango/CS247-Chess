#include "../../include/piece/king.h"
#include "../../include/board.h"
#include "../../include/piece/rook.h"

King::King(
    Color color, Coordinate location, PieceType type, std::shared_ptr<Board> board)
    : Piece(color, location, type, board)
{
    this->directions.insert(this->directions.end(),
        { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { 1, -1 }, { -1, 1 },
            { -1, -1 } });
}

bool King::in_check(Coordinate square)
{
    std::set<Coordinate> attacked;
    if (this->color == Color::BLACK) {
        this->board->get_threatened_squares_by_color(attacked, Color::WHITE);
    } else {
        this->board->get_threatened_squares_by_color(attacked, Color::BLACK);
    }

    /* cannot move into a spot that would result in a check */
    if (attacked.find(square) != attacked.end()) {
        return true;
    }
    return false;
}
bool King::is_checkmate(std::set<Coordinate>& attacked_squares) { return true; }

void King::get_threatened_squares(std::set<Coordinate>& s)
{
    return this->single_move(this->directions, s);
};
