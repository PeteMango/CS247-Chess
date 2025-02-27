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

Piece::~Piece() = default;

void Piece::single_move(
    std::vector<std::pair<int, int>> possible, std::set<Coordinate>& s)
{
    /* handler for pawn, king, knight that can move to fixed spots */
    for (auto i : possible) {
        std::pair<int, int> start = get_grid_indexes(this->location);

        /* increment the move */
        start = add_pairs(start, i);

        /* handles regular moves & captures */
        if (coordinate_in_bounds(start)) {
            s.insert(Coordinate { start.first, start.second });
        }
    }
}

void Piece::multiple_moves(
    std::vector<std::pair<int, int>> possible, std::set<Coordinate>& s)
{
    /* handler for rook, bishop, queen that can move multiple squares */
    for (auto i : possible) {
        std::pair<int, int> start = get_grid_indexes(this->location);
        start = add_pairs(start, i);

        while (coordinate_in_bounds(start)) {
            s.insert(Coordinate { start.first, start.second });

            /* has a piece here, so stop searching */
            auto board = this->board.lock();
            if (board->get_grid()[start.first][start.second]) {
                break;
            }

            /* increment the current coordinate */
            start = add_pairs(start, i);
        }
    }
}

void Piece::print_attack()
{
    std::set<Coordinate> s;
    this->get_threatened_squares(s);

    std::cout << "can attack: " << std::endl;
    for (const Coordinate& c : s) {
        std::cout << c.row << c.column << std::endl;
    }
}

void Piece::print_move()
{
    std::set<Coordinate> s;
    this->get_valid_moves(s);

    std::cout << "can move: " << std::endl;
    for (const Coordinate& c : s) {
        std::cout << c.row << c.column << std::endl;
    }
}

void Piece::filter_moves(std::set<Coordinate>& s)
{
    std::set<Coordinate> good;
    for (const Coordinate& c : s) {
        std::pair<int, int> index
            = get_grid_indexes(c); /* filter out the piece that coordinates that
                                      already has an allied piece */
        auto board = this->board.lock();
        if (!board->get_grid()[index.first][index.second]
            or (board->get_grid()[index.first][index.second]
                and board->get_grid()[index.first][index.second]->get_color()
                    != this->color)) {
            good.insert(c);
        }
    }
    s.insert(good.begin(), good.end());
}

void Piece::get_valid_moves(std::set<Coordinate>& s)
{
    this->get_threatened_squares(s);
    this->filter_moves(s);
}

bool Piece::is_valid_move(Coordinate c)
{
    std::set<Coordinate> s;
    this->get_valid_moves(s);
    return s.find(c) != s.end();
}

char Piece::get_piece_character()
{
    char c = PieceTypeToString.at(this->type)[0];
    if (this->color == Color::BLACK) {
        return std::tolower(c);
    }
    return c;
}

/* getter methods */
Color Piece::get_color() { return this->color; }
PieceType Piece::get_piece_type() { return this->type; }
Coordinate Piece::get_coordinate() { return this->location; }
void Piece::update_location(Coordinate square) { this->location = square; }
