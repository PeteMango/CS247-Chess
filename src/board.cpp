#include "../include/board.h"

// empty
Board::Board()
    : board { 8,
        std::vector<std::shared_ptr<Piece>>(8, nullptr) }
    , active_color { Color::WHITE }
    , white_king { nullptr }
    , black_king { nullptr }
    , en_passant_targets()
    , halfmove_clock { 0 }
    , fullmove_clock { 0 }
{
    this->white_pieces = std::set<std::shared_ptr<Piece>>();
    this->black_pieces = std::set<std::shared_ptr<Piece>>();

    this->castle_rights[Color::WHITE][CastleSide::KINGSIDE]
        = true;
    this->castle_rights[Color::WHITE][CastleSide::QUEENSIDE]
        = true;
    this->castle_rights[Color::BLACK][CastleSide::KINGSIDE]
        = true;
    this->castle_rights[Color::BLACK][CastleSide::QUEENSIDE]
        = true;
}

// default
Board::Board(int i) { }

// position
Board::Board(std::string fen) { }

bool Board::is_valid_move(Move m) { }

std::string Board::make_move(Move m) { }

void Board::setup_board(std::istream& in) { }

bool Board::is_check() { }

bool Board::is_stalemate() { }

bool Board::is_checkmate() { }

void Board::get_attacked_squares_by_color(
    std::set<Coordinate>& s, Color c)
{
}

void Board::get_possible_moves_by_color(
    std::set<Move>& m, Color c)
{
}

bool Board::verify_board() { }

void Board::place_piece(
    Color color, Coordinate square, PieceType type)
{
}

void Board::remove_piece(Coordinate square) { }

std::string Board::deserialize() { }

