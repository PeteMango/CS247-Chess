#ifndef BOARD_H
#define BOARD_H

#include "enum.h"
#include "struct/coordinate.h"
#include "struct/move.h"
#include <unordered_set>
#include <vector>

class Piece;

class Board {
    std::vector<std::vector<std::shared_ptr<Piece>>> board;
    std::unordered_set<std::shared_ptr<Piece>> white_pieces;
    std::unordered_set<std::shared_ptr<Piece>> black_pieces;
    std::shared_ptr<Piece> white_king;
    std::shared_ptr<Piece> black_king;
    bool verify_board();
    void place_piece(
        Color color, Coordinate square, PieceType type);
    void remove_piece(Coordinate square);

public:
    Board();
    bool is_valid_move(Move m);
    void make_move(Move m);
    void setup_board(std::istream& in);
    bool is_check();
    bool is_stalemate();
    bool is_checkmate();
    void get_attacked_squares_by_color(
        std::unordered_set<Coordinate>& s, Color c);
    void get_possible_moves_by_color(
        std::unordered_set<Move>& m, Color c);
};

#endif
