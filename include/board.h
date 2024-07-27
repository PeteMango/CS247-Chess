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
    // core
    std::vector<std::vector<std::shared_ptr<Piece>>> grid;
    Color active_color;
    bool en_passant_enabled;
    std::unique_ptr<Coordinate> en_passant_targets;
    std::map<Color, std::map<CastleSide, bool>> castle_rights;
    int halfmove_clock;
    int fullmove_clock;
    // helper fields
    std::set<std::shared_ptr<Piece>> white_pieces;
    std::set<std::shared_ptr<Piece>> black_pieces;
    std::shared_ptr<Piece> white_king;
    std::shared_ptr<Piece> black_king;

    std::string serialize();

public:
    Board(bool default_board = true);
    // deserialize
    Board(const std::string& fen);
    void setup_board(std::istream& in);
    std::shared_ptr<Piece> create_piece(
        Color color, Coordinate square, PieceType type);

    // board setup helpers
    void place_piece(Color color, Coordinate square, PieceType type);
    void remove_piece(Coordinate square);
    void verify_board();
    void setup_default_board();
    void clean_board();

    // make move helpers
    /* bool is_valid_move(Move m); */
    bool is_valid_move(Coordinate start, Coordinate end);
    std::string make_move(Move m);
    void add_piece(std::shared_ptr<Piece> p);
    void delete_piece(std::shared_ptr<Piece> p);
    bool is_promotion(Coordinate start, Coordinate end);

    std::vector<std::vector<std::shared_ptr<Piece>>>& get_grid();
    Color get_active_color();
    void toggle_active_color();

    bool is_check(Color c);
    bool is_stalemate();
    bool is_checkmate();
    void get_attacked_squares_by_color(std::set<Coordinate>& s, Color c);
    void get_possible_moves_by_color(std::set<Move>& m, Color c);

    void add_enpassant(Coordinate c);
};

#endif
