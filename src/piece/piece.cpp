#include "../../include/piece/piece.h"
#include "../../include/board.h"

Piece::Piece(
    Color color, Coordinate location, PieceType type, std::shared_ptr<Board> board)
    : color { color }
    , location { location }
    , type { type }
    , board { board }
{
}

Color Piece::get_color() { return this->color; }
PieceType Piece::get_piece_type() { return this->type; }
Coordinate Piece::get_coordinate() { return this->location; }

void Piece::single_move(
    std::vector<std::pair<int, int>> directions, std::set<Coordinate>& s)
{
    /* handler for pawn, king, knight that can move to fixed spots */
    for (auto i : this->directions) {
        std::pair<int, int> start = get_grid_indexes(this->location);
        start = add_pairs(start, i);
        if (coordinate_in_bounds(start)) {
            s.insert(Coordinate(start.first, start.second));
        }
    }
}

void Piece::multiple_moves(
    std::vector<std::pair<int, int>> directions, std::set<Coordinate>& s)
{
    /* handler for rook, bishop, queen that can move multiple squares */
    for (auto i : this->directions) {
        std::pair<int, int> start = get_grid_indexes(this->location);
        start = add_pairs(start, i);

        while (coordinate_in_bounds(start)
            and this->board->get_grid()[start.first][start.second] == nullptr) {
            s.insert(Coordinate(start.first, start.second));
            start = add_pairs(start, i);
        }
        if (coordinate_in_bounds(start)) {
            s.insert(Coordinate(start.first, start.second));
        }
    }
}

void Piece::print_attacking() {
    std::set<Coordinate> s;
    this->get_attacking_squares(s);

    std::cout << "can attack: " << std::endl;
    for(const Coordinate &c : s) {
        std::cout << c.row << c.column << std::endl;
    }
}