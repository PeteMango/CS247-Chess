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

class Game;

class Board : public std::enable_shared_from_this<Board> {
    // core
    std::vector<std::vector<std::shared_ptr<Piece>>> grid;
    Color active_color;
    bool en_passant_enabled;
    std::shared_ptr<Coordinate> en_passant_target;
    std::map<Color, std::map<CastleSide, bool>> castle_rights;
    int halfmove_clock;
    int fullmove_clock;
    std::weak_ptr<Game> game;
    // helper fields
    std::set<std::shared_ptr<Piece>> white_pieces;
    std::set<std::shared_ptr<Piece>> black_pieces;
    std::shared_ptr<Piece> white_king;
    std::shared_ptr<Piece> black_king;

    std::string serialize();

public:
    Board(std::shared_ptr<Game> game, bool default_board = true);
    // deserialize
    Board(std::shared_ptr<Game> game, const std::string& fen);
    std::shared_ptr<Piece> create_piece(
        Color color, Coordinate square, PieceType type);

    // board setup helpers
    void place_piece(Color color, Coordinate square, PieceType type);
    void remove_piece(Coordinate square);
    void verify_setup();
    void setup_default_board();
    void clean_board();

    // make move helpers
    MoveFlags is_valid_move(Coordinate start, Coordinate end);
    std::string make_move(Coordinate start, Coordinate end, PromotionType promotion);
    void add_piece(std::shared_ptr<Piece> p);
    void destroy_piece(std::shared_ptr<Piece> p);
    bool is_promotion(Coordinate start, Coordinate end);
    void get_threatened_squares_by_color(std::set<Coordinate>& s, Color c);
    Coordinate get_enpassant_coordinate();
    bool is_valid_enpassant(Coordinate start, Coordinate end);
    bool is_valid_castle(Coordinate start, Coordinate end);
    bool is_double_pawn_move(Coordinate start, Coordinate end);
    bool is_double_king_move(Coordinate start, Coordinate end);
    void add_enpassant_target(std::shared_ptr<Coordinate> c);
    Coordinate get_enpassant_taken_piece_coordinate();
    std::shared_ptr<Coordinate> get_enpassant_square_coordinate(Coordinate c);
    Coordinate get_castle_rook(Color c, CastleSide cs);
    void get_all_valid_moves(
        std::set<std::pair<Coordinate, Coordinate>>& s, Color c);
    std::set<std::shared_ptr<Piece>> get_pieces(Color c);

    std::vector<std::vector<std::shared_ptr<Piece>>>& get_grid();
    Color get_active_color();
    void toggle_active_color();
    void increment_halfmove_clock();
    void increment_fullmove_clock();

    bool is_check(Color c);
    bool is_stalemate(Color c);
    bool is_checkmate(Color c);
};

#endif
