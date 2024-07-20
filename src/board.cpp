#include "../include/board.h"

Board::Board() { }

bool Board::is_valid_move(Move m) { }

void Board::make_move(Move m) { }

void Board::setup_board(std::istream& in) { }

bool Board::is_check() { }

bool Board::is_stalemate() { }

bool Board::is_checkmate() { }

void Board::get_attacked_squares_by_color(
    std::unordered_set<Coordinate>& s, Color c)
{
}

void Board::get_possible_moves_by_color(
    std::unordered_set<Move>& m, Color c)
{
}

bool Board::verify_board() { }

void Board::place_piece(
    Color color, Coordinate square, PieceType type)
{
}

void Board::remove_piece(Coordinate square) { }

