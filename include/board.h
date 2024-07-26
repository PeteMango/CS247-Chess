#ifndef BOARD_H
#define BOARD_H

#include "enum.h"
#include "piece/piece.h"
#include "struct/coordinate.h"
#include "struct/move.h"
#include <map>
#include <set>
#include <sstream>
#include <vector>

class Board : public std::enable_shared_from_this<Board> {
    // https://www.chess.com/terms/fen-chess
    std::vector<std::vector<std::shared_ptr<Piece>>> grid;
    Color active_color;
    bool en_passant_enabled;
    std::unique_ptr<Coordinate> en_passant_targets;
    std::set<std::shared_ptr<Piece>> white_pieces;
    std::set<std::shared_ptr<Piece>> black_pieces;
    std::shared_ptr<Piece> white_king;
    std::shared_ptr<Piece> black_king;
    std::map<Color, std::map<CastleSide, bool>> castle_rights;
    int halfmove_clock;
    int fullmove_clock;
    std::string serialize();

public:
    Board(bool default_board = true);
    // deserialize
    Board(const std::string& fen);
    bool is_valid_move(Move m);
    std::string make_move(Move m);
    void setup_board(std::istream& in);
    bool is_check(Color c);
    bool is_stalemate();
    bool is_checkmate();
    void get_attacked_squares_by_color(std::set<Coordinate>& s, Color c);
    void get_possible_moves_by_color(std::set<Move>& m, Color c);
    void place_piece(Color color, Coordinate square, PieceType type);
    void remove_piece(Coordinate square);
    std::vector<std::vector<std::shared_ptr<Piece>>>& get_grid();
    void verify_board();
    void setup_default_board();
    void clean_board();
    Color get_active_color();
    void toggle_active_color();
    void delete_piece(std::shared_ptr<Piece> p, Color color);
    void add_piece(std::shared_ptr<Piece> p, Color color);
    bool is_promotion(Coordinate start, Coordinate end);
    std::shared_ptr<Piece> create_piece(
        Color color, Coordinate square, PieceType type);
};

#endif
