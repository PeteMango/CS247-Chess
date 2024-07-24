#ifndef BOARD_H
#define BOARD_H

#include "enum.h"
#include "struct/coordinate.h"
#include "struct/move.h"
#include <map>
#include <set>
#include <vector>

class Piece;

class Board {
    // https://www.chess.com/terms/fen-chess
    std::vector<std::vector<std::shared_ptr<Piece>>> board;
    Color active_color;
    std::set<std::shared_ptr<Piece>> white_pieces;
    std::set<std::shared_ptr<Piece>> black_pieces;
    std::shared_ptr<Piece> white_king;
    std::shared_ptr<Piece> black_king;
    std::map<Color, std::map<CastleSide, bool>> castle_rights;
    std::set<Coordinate> en_passant_targets;
    int halfmove_clock;
    int fullmove_clock;
    bool verify_board();
    std::string deserialize();

public:
    Board();
    Board(int i);
    // serialize
    Board(std::string fen);
    bool is_valid_move(Move m);
    std::string make_move(Move m);
    void setup_board(std::istream& in);
    bool is_check();
    bool is_stalemate();
    bool is_checkmate();
    void get_attacked_squares_by_color(
        std::set<Coordinate>& s, Color c);
    void get_possible_moves_by_color(std::set<Move>& m, Color c);
    void place_piece(
        Color color, Coordinate square, PieceType type);
    void remove_piece(Coordinate square);
};

#endif
