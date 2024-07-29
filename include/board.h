#ifndef BOARD_H
#define BOARD_H

#include "enum.h"
#include "piece/piece.h"
#include "struct/coordinate.h"
#include "struct/move.h"
#include <array>
#include <map>
#include <set>
#include <sstream>
#include <vector>

class Game;

class Board : public std::enable_shared_from_this<Board> {
    /* core */
    std::vector<std::vector<std::shared_ptr<Piece>>> grid;
    std::shared_ptr<Game> game;
    Color active_color;

    /* enpassant */
    bool en_passant_enabled;
    std::shared_ptr<Coordinate> en_passant_target;

    /* castle */
    std::map<Color, std::map<CastleSide, bool>> castle_rights;

    /* move clocks */
    int halfmove_clock, fullmove_clock;

    /* pieces */
    std::set<std::shared_ptr<Piece>> white_pieces;
    std::set<std::shared_ptr<Piece>> black_pieces;

    /* pointers to black/white king */
    std::shared_ptr<Piece> white_king;
    std::shared_ptr<Piece> black_king;

    /* board -> fen */
    std::string serialize();

    /* board for bot4 */
    // clang-format off
    std::map<PieceType, int> piece_weight = {
        { PieceType::PAWN, 100 },
        { PieceType::KNIGHT, 300 },
        { PieceType::BISHOP, 350 },
        { PieceType::ROOK, 500 },
        { PieceType::QUEEN, 900 },
        { PieceType::KING, 100000 }
    };
    // clang-format on

public:
    /* board creation */
    Board(std::shared_ptr<Game> game, bool default_board = true);
    Board(std::shared_ptr<Game> game, const std::string& fen);
    void setup_board(std::istream& in);

    /* board helper piece */
    void place_piece(Color color, Coordinate square, PieceType type);
    void remove_piece(Coordinate square);
    void verify_board();
    void setup_default_board();
    void clean_board();

    /* making moves */
    MoveFlags is_valid_move(Coordinate start, Coordinate end);
    void get_all_valid_moves(
        std::set<std::pair<Coordinate, Coordinate>>& s, Color c);
    std::string make_move(Coordinate start, Coordinate end, PromotionType promotion);

    /* piece add/deletion */
    std::shared_ptr<Piece> create_piece(
        Color color, Coordinate square, PieceType type);
    void add_piece(std::shared_ptr<Piece> p);
    void delete_piece(std::shared_ptr<Piece> p);
    bool is_promotion(Coordinate start, Coordinate end);
    void get_threatened_squares_by_color(std::set<Coordinate>& s, Color c);
    Coordinate get_enpassant_coordinate();

    /* special moves check */
    bool is_valid_enpassant(Coordinate start, Coordinate end);
    bool is_valid_castle(Coordinate start, Coordinate end);
    bool is_double_pawn_move(Coordinate start, Coordinate end);
    bool is_double_king_move(Coordinate start, Coordinate end);
    void add_enpassant_target(std::shared_ptr<Coordinate> c);
    Coordinate get_enpassant_taken_piece_coordinate();
    std::shared_ptr<Coordinate> get_enpassant_square_coordinate(Coordinate c);
    Coordinate get_castle_rook(Color c, CastleSide cs);

    /* getters */
    std::set<std::shared_ptr<Piece>> get_pieces(Color c);
    std::vector<std::vector<std::shared_ptr<Piece>>>& get_grid();

    /* make move helpers */
    Color get_active_color();
    void toggle_active_color();
    void increment_halfmove_clock();
    void increment_fullmove_clock();

    /* end game functions */
    bool is_check(Color c);
    bool is_stalemate(Color c);
    bool is_checkmate(Color c);

    /* bot logic */
    int evaluate_position(Color toplay);
    int piece_activity();
    int pawn_structure();
    int king_safety();
};

#endif
