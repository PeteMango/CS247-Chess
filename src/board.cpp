#include "../include/board.h"
#include "../include/piece/bishop.h"
#include "../include/piece/king.h"
#include "../include/piece/knight.h"
#include "../include/piece/pawn.h"
#include "../include/piece/queen.h"
#include "../include/piece/rook.h"
#include "../include/util.h"
#include "chess.h"
#include "enum.h"
#include "game.h"
#include "piece/piece.h"
#include <iostream>
#include <memory>
#include <stdexcept>

// empty
Board::Board(std::shared_ptr<Game> game, bool default_board)
    : grid { 8, std::vector<std::shared_ptr<Piece>>(8, nullptr) }
    , active_color { Color::WHITE }
    , en_passant_enabled { default_board }
    , en_passant_target()
    , halfmove_clock { 0 }
    , fullmove_clock { 0 }
    , game { game }
    , white_king { nullptr }
    , black_king { nullptr }
{
    this->white_pieces = std::set<std::shared_ptr<Piece>>();
    this->black_pieces = std::set<std::shared_ptr<Piece>>();
    if (default_board) {
        this->castle_rights[Color::WHITE][CastleSide::KINGSIDE] = true;
        this->castle_rights[Color::WHITE][CastleSide::QUEENSIDE] = true;
        this->castle_rights[Color::BLACK][CastleSide::KINGSIDE] = true;
        this->castle_rights[Color::BLACK][CastleSide::QUEENSIDE] = true;
    } else {
        this->castle_rights[Color::WHITE][CastleSide::KINGSIDE] = false;
        this->castle_rights[Color::WHITE][CastleSide::QUEENSIDE] = false;
        this->castle_rights[Color::BLACK][CastleSide::KINGSIDE] = false;
        this->castle_rights[Color::BLACK][CastleSide::QUEENSIDE] = false;
    }
}

void Board::clean_board()
{
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (this->grid[i][j] == nullptr) {
                continue;
            }
            this->grid[i][j] = nullptr;
        }
    }
    this->white_king = nullptr;
    this->black_king = nullptr;
    this->white_pieces.clear();
    this->black_pieces.clear();
    // TODO: en passant, castle rights, clocks
}

void Board::setup_default_board()
{
    this->clean_board();
    // kings
    this->place_piece(Color::WHITE, Coordinate("e1"), PieceType::KING);
    this->place_piece(Color::BLACK, Coordinate("e8"), PieceType::KING);
    // queens
    this->place_piece(Color::WHITE, Coordinate("d1"), PieceType::QUEEN);
    this->place_piece(Color::BLACK, Coordinate("d8"), PieceType::QUEEN);
    // rooks
    this->place_piece(Color::WHITE, Coordinate("a1"), PieceType::ROOK);
    this->place_piece(Color::WHITE, Coordinate("h1"), PieceType::ROOK);
    this->place_piece(Color::BLACK, Coordinate("a8"), PieceType::ROOK);
    this->place_piece(Color::BLACK, Coordinate("h8"), PieceType::ROOK);
    // bishops
    this->place_piece(Color::WHITE, Coordinate("c1"), PieceType::BISHOP);
    this->place_piece(Color::WHITE, Coordinate("f1"), PieceType::BISHOP);
    this->place_piece(Color::BLACK, Coordinate("c8"), PieceType::BISHOP);
    this->place_piece(Color::BLACK, Coordinate("f8"), PieceType::BISHOP);
    // knights
    this->place_piece(Color::WHITE, Coordinate("b1"), PieceType::KNIGHT);
    this->place_piece(Color::WHITE, Coordinate("g1"), PieceType::KNIGHT);
    this->place_piece(Color::BLACK, Coordinate("b8"), PieceType::KNIGHT);
    this->place_piece(Color::BLACK, Coordinate("g8"), PieceType::KNIGHT);
    // pawns
    for (int i = 0; i < 8; i++) {
        char col = static_cast<char>(i + 'a');
        this->place_piece(Color::WHITE, Coordinate(2, col), PieceType::PAWN);
        this->place_piece(Color::BLACK, Coordinate(7, col), PieceType::PAWN);
    }
    this->verify_setup();
}

Board::Board(std::shared_ptr<Game> game, const std::string& fen)
    : en_passant_enabled { true }
    , halfmove_clock { 0 }
    , fullmove_clock { 0 }
    , game { game }
{
    std::istringstream fenStream(fen);
    std::string curLine;
    int row = 0;

    // Read the board position part of the FEN string
    while (std::getline(fenStream, curLine, '/') && row < 8) {
        int col = 0;
        for (const char& c : curLine) {
            if (c >= '0' && c <= '9') {
                int emptySquares = c - '0';
                for (int j = 0; j < emptySquares; j++) {
                    this->grid[row][col] = nullptr;
                    col++;
                }
            } else {
                this->grid[row][col] = this->create_piece(
                    (c >= 'a' and c <= 'z') ? Color::BLACK : Color::WHITE,
                    Coordinate { row, col }, string_to_piecetype(std::string { c }));
                col++;
            }
        }
        row++;
    }

    /* set active color */
    char activeColor;
    fenStream >> activeColor;
    if (activeColor == 'b') {
        this->active_color = Color::BLACK;
    } else {
        this->active_color = Color::WHITE;
    }

    /* scan in the castle rights */
    std::string castleRights;
    fenStream >> castleRights;
    if (castleRights.find('K') != std::string::npos) {
        this->castle_rights[Color::WHITE][CastleSide::KINGSIDE] = true;
    }
    if (castleRights.find('Q') != std::string::npos) {
        this->castle_rights[Color::WHITE][CastleSide::QUEENSIDE] = true;
    }
    if (castleRights.find('j') != std::string::npos) {
        this->castle_rights[Color::BLACK][CastleSide::KINGSIDE] = true;
    }
    if (castleRights.find('q') != std::string::npos) {
        this->castle_rights[Color::BLACK][CastleSide::QUEENSIDE] = true;
    }

    /* empassant target square */
    std::string empassant;
    fenStream >> empassant;
    if (empassant != "-") {
        this->en_passant_target = std::make_unique<Coordinate>(
            empassant[1] - '0', empassant[0] - 'a' + 1);
    }

    /* half/full move clocks */
    fenStream >> this->halfmove_clock >> this->fullmove_clock;
}

MoveFlags Board::is_valid_move(Coordinate start, Coordinate end, Color c)
{
    std::pair<int, int> start_idx = get_grid_indexes(start),
                        end_idx = get_grid_indexes(end);

    /* starting square cannot be empty */
    if (this->grid[start_idx.first][start_idx.second] == nullptr) {
        return MoveFlags { false };
    }

    /* starting square has incorrect piece */
    std::shared_ptr<Piece> p = grid[start_idx.first][start_idx.second];
    if (p->get_color() != c) {
        return MoveFlags { false };
    }

    /* check if enpassants or castle is possible */
    bool valid_enpassant = this->is_valid_enpassant(start, end),
         valid_castle = this->is_valid_castle(start, end);

    /* invalid move, castle and enpassant */
    if (!p->is_valid_move(end) and !valid_castle and !valid_enpassant) {
        return MoveFlags { false };
    }

    std::shared_ptr<Piece> new_p
        = this->create_piece(p->get_color(), end, p->get_piece_type());
    std::shared_ptr<Piece> taken_piece = nullptr;

    bool capture = false;
    bool good_trade = false;
    int gain = 0;
    if (this->grid[end_idx.first][end_idx.second] != nullptr) {
        taken_piece = this->grid[end_idx.first][end_idx.second];
        if (taken_piece->get_color() == p->get_color()) {
            return MoveFlags { false };
        }
        capture = true;
        if (this->piece_weights[taken_piece->get_piece_type()]
            >= this->piece_weights[p->get_piece_type()]) {
            good_trade = true;
            gain = this->piece_weights[taken_piece->get_piece_type()]
                - this->piece_weights[p->get_piece_type()];
        }
        this->destroy_piece(taken_piece);
    } else if (valid_enpassant) {
        std::pair<int, int> taken_piece_idx
            = get_grid_indexes(this->get_enpassant_taken_piece_coordinate());
        taken_piece = this->grid[taken_piece_idx.first][taken_piece_idx.second];
        capture = true;
        this->destroy_piece(taken_piece);
    }

    std::set<Coordinate> ally_attack_before, enemy_attack_before;
    this->get_threatened_squares_by_color(ally_attack_before, this->active_color);
    this->get_threatened_squares_by_color(
        enemy_attack_before, toggle_color(this->active_color));

    bool attacked_before = (enemy_attack_before.find(p->get_coordinate())
        != enemy_attack_before.end());

    /* make the actual move */
    this->destroy_piece(p);
    this->add_piece(new_p);

    bool is_checkmate = false;
    if (this->is_checkmate(toggle_color(c))) {
        is_checkmate = true;
    }

    std::set<Coordinate> ally_attack_after, enemy_attack_after;
    this->get_threatened_squares_by_color(ally_attack_after, this->active_color);
    this->get_threatened_squares_by_color(
        enemy_attack_after, toggle_color(this->active_color));

    bool attacked_after
        = enemy_attack_after.find(Coordinate { end_idx.first, end_idx.second })
        != enemy_attack_after.end();

    /* check if ally king is in check after making the move */
    bool invalid = false;
    if (p->get_color() == Color::WHITE and this->white_king
        and enemy_attack_after.find(this->white_king->get_coordinate())
            != enemy_attack_after.end()) {
        invalid = true;
    } else if (p->get_color() == Color::BLACK and this->black_king
        and enemy_attack_after.find(this->black_king->get_coordinate())
            != enemy_attack_after.end()) {
        invalid = true;
    }

    /* check if our move checks the enemy king */
    bool check = false;
    if (p->get_color() == Color::WHITE and this->black_king
        and ally_attack_after.find(this->black_king->get_coordinate())
            != ally_attack_after.end()) {
        check = true;
    } else if (p->get_color() == Color::BLACK and this->white_king
        and ally_attack_after.find(this->white_king->get_coordinate())
            != ally_attack_after.end()) {
        check = true;
    }

    /* restore original game state if new state is invalid */
    this->destroy_piece(new_p);
    this->add_piece(p);
    if (taken_piece) {
        this->add_piece(taken_piece);
    }

    /* return flags */
    if (invalid) {
        return MoveFlags { false };
    }
    return MoveFlags { true, check, capture, attacked_before, attacked_after,
        good_trade, is_checkmate, gain };
}

MoveFlags Board::checkmate_valid_move(Coordinate start, Coordinate end, Color c)
{
    std::pair<int, int> start_idx = get_grid_indexes(start),
                        end_idx = get_grid_indexes(end);

    /* starting square cannot be empty */
    if (this->grid[start_idx.first][start_idx.second] == nullptr) {
        return MoveFlags { false };
    }

    /* starting square has incorrect piece */
    std::shared_ptr<Piece> p = grid[start_idx.first][start_idx.second];
    if (p->get_color() != c) {
        return MoveFlags { false };
    }

    /* invalid move, castle and enpassant */
    if (!p->is_valid_move(end)) {
        return MoveFlags { false };
    }

    std::shared_ptr<Piece> new_p
        = this->create_piece(p->get_color(), end, p->get_piece_type());
    std::shared_ptr<Piece> taken_piece = nullptr;

    if (this->grid[end_idx.first][end_idx.second] != nullptr) {
        taken_piece = this->grid[end_idx.first][end_idx.second];
        if (taken_piece->get_color() == p->get_color()) {
            return MoveFlags { false };
        }
        this->destroy_piece(taken_piece);
    }

    /* make the actual move */
    this->destroy_piece(p);
    this->add_piece(new_p);

    std::set<Coordinate> enemy_attack_after;
    this->get_threatened_squares_by_color(enemy_attack_after, toggle_color(c));

    /* check if ally king is in check after making the move */
    bool invalid = false;
    if (p->get_color() == Color::WHITE and this->white_king
        and enemy_attack_after.find(this->white_king->get_coordinate())
            != enemy_attack_after.end()) {
        invalid = true;
    } else if (p->get_color() == Color::BLACK and this->black_king
        and enemy_attack_after.find(this->black_king->get_coordinate())
            != enemy_attack_after.end()) {
        invalid = true;
    }

    /* restore original game state if new state is invalid */
    this->destroy_piece(new_p);
    this->add_piece(p);
    if (taken_piece) {
        this->add_piece(taken_piece);
    }

    /* return flags */
    if (invalid) {
        return MoveFlags { false };
    }
    return MoveFlags { true, false, false, false, false, false, false, 0 };
}

std::string Board::make_move(
    Coordinate start, Coordinate end, PromotionType promotion)
{
    auto game = this->game.lock();
    std::pair<int, int> starting_idx = get_grid_indexes(start);
    std::pair<int, int> ending_idx = get_grid_indexes(end);
    std::shared_ptr<Piece> p = this->grid[starting_idx.first][starting_idx.second];

    /* capture */
    if (this->grid[ending_idx.first][ending_idx.second] != nullptr) {
        std::shared_ptr<Piece> taken
            = this->grid[ending_idx.first][ending_idx.second];
        if (taken->get_piece_type() == PieceType::ROOK) {
            if (this->get_castle_rook(taken->get_color(), CastleSide::KINGSIDE)
                == taken->get_coordinate()) {
                this->castle_rights[taken->get_color()][CastleSide::KINGSIDE]
                    = false;
            } else if (this->get_castle_rook(
                           taken->get_color(), CastleSide::QUEENSIDE)
                == taken->get_coordinate()) {
                this->castle_rights[taken->get_color()][CastleSide::QUEENSIDE]
                    = false;
            }
        }
        this->destroy_piece(taken);
    } else if (this->is_valid_enpassant(start, end)) {
        Coordinate c = this->get_enpassant_taken_piece_coordinate();
        std::pair<int, int> enpassant_taken_piece_idx = get_grid_indexes(c);
        std::shared_ptr<Piece> enpassant_taken_piece
            = this->grid[enpassant_taken_piece_idx.first]
                        [enpassant_taken_piece_idx.second];
        this->destroy_piece(enpassant_taken_piece);
    } else if (this->is_valid_castle(start, end)) {
        Coordinate rook_coord;
        std::pair<int, int> d;
        /* kingside */
        if (start.column < end.column) {
            rook_coord = this->get_castle_rook(p->get_color(), CastleSide::KINGSIDE);
            d = { 0, -2 };
        }
        /* queenside */
        else {
            rook_coord
                = this->get_castle_rook(p->get_color(), CastleSide::QUEENSIDE);
            d = { 0, 3 };
        }
        std::pair<int, int> rook_idx = get_grid_indexes(rook_coord);
        std::shared_ptr<Piece> rook = this->grid[rook_idx.first][rook_idx.second];

        std::pair<int, int> new_rook_idx = add_pairs(rook_idx, d);
        Coordinate new_rook_coord
            = Coordinate(new_rook_idx.first, new_rook_idx.second);
        std::shared_ptr<Piece> new_rook
            = this->create_piece(p->get_color(), new_rook_coord, PieceType::ROOK);
        this->destroy_piece(rook);
        this->add_piece(new_rook);
    }

    /* if its a double move it can be an en-passant */
    if (this->is_double_pawn_move(start, end)) {
        std::shared_ptr<Coordinate> enpassant
            = this->get_enpassant_square_coordinate(end);
        this->add_enpassant_target(enpassant);
    } else {
        this->add_enpassant_target(nullptr);
    }

    /* check promotion */
    std::shared_ptr<Piece> new_p = nullptr;
    if (this->is_promotion(start, end)) {
        new_p = this->create_piece(
            p->get_color(), end, PromotionTypeToPieceType(promotion));
    } else {
        new_p = this->create_piece(p->get_color(), end, p->get_piece_type());
    }
    this->destroy_piece(p);
    this->add_piece(new_p);

    /* if moved king, then revoke the possibility of a castle */
    if (p->get_piece_type() == PieceType::KING) {
        for (auto& i : this->castle_rights[p->get_color()]) {
            i.second = false;
        }
    } else if (p->get_piece_type() == PieceType::ROOK) {
        Coordinate c1 = this->get_castle_rook(p->get_color(), CastleSide::KINGSIDE);
        Coordinate c2 = this->get_castle_rook(p->get_color(), CastleSide::QUEENSIDE);
        if (start == c1) {
            this->castle_rights[p->get_color()][CastleSide::KINGSIDE] = false;
        } else if (start == c2) {
            this->castle_rights[p->get_color()][CastleSide::QUEENSIDE] = false;
        }
    }

    /* increment clocks */
    if (this->get_active_color() == Color::BLACK) {
        this->increment_halfmove_clock();
    }
    this->increment_fullmove_clock();
    this->toggle_active_color();

    DisplayStatus status = DisplayStatus::NONE;
    Color color = this->active_color;
    if (this->is_checkmate((this->active_color))) {
        status = DisplayStatus::CHECKMATE;
        // winner
        color = toggle_color(this->active_color);
        game->end_game(color_to_result(color));
    } else if (this->is_stalemate((this->active_color))) {
        status = DisplayStatus::STALEMATE;
        game->end_game(Result::DRAW);
    } else if (this->is_check((this->active_color))) {
        status = DisplayStatus::CHECK;
        // color in check
        color = (this->active_color);
    }
    game->get_chess()->notify_displays();
    game->get_chess()->notify_status(status, color);

    /* serialize board */
    return this->serialize();
}

void Board::get_threatened_squares_by_color(std::set<Coordinate>& s, Color c)
{
    if (c == Color::WHITE) {
        for (auto it = this->white_pieces.begin(); it != this->white_pieces.end();
             ++it) {
            (*it)->get_threatened_squares(s);
        }
    } else {
        for (auto it = this->black_pieces.begin(); it != this->black_pieces.end();
             ++it) {
            (*it)->get_threatened_squares(s);
        }
    }
}

void Board::toggle_active_color()
{
    this->active_color = toggle_color(this->active_color);
}

void Board::increment_halfmove_clock() { this->halfmove_clock++; }
void Board::increment_fullmove_clock() { this->fullmove_clock++; }

// returns whether c is in check
bool Board::is_check(Color c)
{
    if (c == Color::WHITE && this->white_king == nullptr) {
        return false;
    } else if (c == Color::BLACK && this->black_king == nullptr) {
        return false;
    }
    std::shared_ptr<King> king = nullptr;
    if (c == Color::WHITE) {
        king = std::dynamic_pointer_cast<King>(this->white_king);
    } else {
        king = std::dynamic_pointer_cast<King>(this->black_king);
    }
    return king->in_check();
}

bool Board::is_stalemate(Color c)
{
    std::set<std::pair<Coordinate, Coordinate>> s;
    this->get_all_valid_moves(s, c);
    return s.empty();
}

bool Board::is_checkmate(Color c)
{
    if (c == Color::WHITE && this->white_king == nullptr) {
        return false;
    } else if (c == Color::BLACK && this->black_king == nullptr) {
        return false;
    }
    // TODO: this should be reviewed
    std::shared_ptr<King> king = nullptr;
    if (c == Color::WHITE) {
        king = std::dynamic_pointer_cast<King>(this->white_king);
    } else {
        king = std::dynamic_pointer_cast<King>(this->black_king);
    }
    bool king_cant_move = king->in_checkmate();
    if (!king_cant_move) {
        return false;
    }
    std::set<std::pair<Coordinate, Coordinate>> s;
    this->checkmate_get_all_valid_moves(s, c);
    for (auto move : s) {
        if (this->checkmate_valid_move(move.first, move.second, c).valid) {
            return false;
        }
    }
    return true;
}

void Board::get_all_valid_moves(
    std::set<std::pair<Coordinate, Coordinate>>& s, Color c)
{
    std::set<std::shared_ptr<Piece>> p;
    if (c == Color::WHITE) {
        p.insert(this->white_pieces.begin(), this->white_pieces.end());
    } else {
        p.insert(this->black_pieces.begin(), this->black_pieces.end());
    }
    for (auto piece : p) {
        std::set<Coordinate> s2;
        piece->get_valid_moves(s2);
        for (auto coord : s2) {
            if (this->is_valid_move(piece->get_coordinate(), coord, c).valid) {
                std::pair<Coordinate, Coordinate> move
                    = std::make_pair(piece->get_coordinate(), coord);
                s.insert(move);
            }
        }
        // add en passasnt
        if (piece->get_piece_type() == PieceType::PAWN) {
            std::shared_ptr<Pawn> pawn = std::dynamic_pointer_cast<Pawn>(piece);
            for (auto square : pawn->get_captures()) {
                std::pair<int, int> end_idx
                    = add_pairs(square, get_grid_indexes(pawn->get_coordinate()));
                Coordinate end = Coordinate(end_idx.first, end_idx.second);
                if (this->is_valid_move(pawn->get_coordinate(), end, c).valid) {
                    std::pair<Coordinate, Coordinate> move
                        = std::make_pair(pawn->get_coordinate(), end);
                    s.insert(move);
                }
            }

        }
        // add castle
        else if (piece->get_piece_type() == PieceType::KING) {
            std::vector<std::pair<int, int>> d = { { 0, 2 }, { 0, -2 } };
            Coordinate cur = piece->get_coordinate();
            std::pair<int, int> cur_idx = get_grid_indexes(cur);
            for (auto i : d) {
                std::pair<int, int> new_idx = add_pairs(cur_idx, i);
                Coordinate new_coord = Coordinate(new_idx.first, new_idx.second);
                if (this->is_valid_move(cur, new_coord, c).valid) {
                    std::pair<Coordinate, Coordinate> move
                        = std::make_pair(cur, new_coord);
                    s.insert(move);
                }
            }
        }
    }
}

void Board::checkmate_get_all_valid_moves(
    std::set<std::pair<Coordinate, Coordinate>>& s, Color c)
{
    std::set<std::shared_ptr<Piece>> p;
    if (c == Color::WHITE) {
        p.insert(this->white_pieces.begin(), this->white_pieces.end());
    } else {
        p.insert(this->black_pieces.begin(), this->black_pieces.end());
    }
    for (auto piece : p) {
        std::set<Coordinate> s2;
        piece->get_valid_moves(s2);
        for (auto coord : s2) {
            if (this->checkmate_valid_move(piece->get_coordinate(), coord, c)
                    .valid) {
                std::pair<Coordinate, Coordinate> move
                    = std::make_pair(piece->get_coordinate(), coord);
                s.insert(move);
            }
        }
        // add en passasnt
        if (piece->get_piece_type() == PieceType::PAWN) {
            std::shared_ptr<Pawn> pawn = std::dynamic_pointer_cast<Pawn>(piece);
            for (auto square : pawn->get_captures()) {
                Coordinate end = Coordinate(square.first, square.second);
                if (this->checkmate_valid_move(pawn->get_coordinate(), end, c)
                        .valid) {
                    std::pair<Coordinate, Coordinate> move
                        = std::make_pair(pawn->get_coordinate(), end);
                    s.insert(move);
                }
            }

        }
        // add castle
        else if (piece->get_piece_type() == PieceType::KING) {
            std::vector<std::pair<int, int>> d = { { 0, 2 }, { 0, -2 } };
            Coordinate cur = piece->get_coordinate();
            std::pair<int, int> cur_idx = get_grid_indexes(cur);
            for (auto i : d) {
                std::pair<int, int> new_idx = add_pairs(cur_idx, i);
                Coordinate new_coord = Coordinate(new_idx.first, new_idx.second);
                if (this->checkmate_valid_move(cur, new_coord, c).valid) {
                    std::pair<Coordinate, Coordinate> move
                        = std::make_pair(cur, new_coord);
                    s.insert(move);
                }
            }
        }
    }
}

void Board::verify_setup()
{
    // no pawns in the last rows
    for (int i = 0; i < 8; i++) {
        if (this->grid[0][i]
            && this->grid[0][i]->get_piece_type() == PieceType::PAWN) {
            throw std::runtime_error("pawn in the outer rows");
        }
        if (this->grid[7][i]
            && this->grid[7][i]->get_piece_type() == PieceType::PAWN) {
            throw std::runtime_error("pawn in the outer rows");
        }
    }
    // one of each king
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (this->grid[i][j] == nullptr) {
                continue;
            }
            std::shared_ptr<Piece> p = this->grid[i][j];
            if (p->get_color() == Color::WHITE) {
                this->white_pieces.insert(p);
                if (p->get_piece_type() == PieceType::KING) {
                    if (this->white_king != nullptr) {
                        throw std::runtime_error("multiple white kings");
                    }
                    this->white_king = p;
                }
            } else {
                this->black_pieces.insert(p);
                if (p->get_piece_type() == PieceType::KING) {
                    if (this->black_king != nullptr) {
                        throw std::runtime_error("multiple black kings");
                    }
                    this->black_king = p;
                }
            }
        }
    }
    // neither king in check
    if (this->is_check(Color::WHITE) || this->is_check(Color::BLACK)) {
        throw std::runtime_error("king in check");
    }
}

std::shared_ptr<Piece> Board::create_piece(
    Color color, Coordinate square, PieceType type)
{
    switch (type) {
    case PieceType::KING:
        return std::make_shared<King>(color, square, type, shared_from_this());
    case PieceType::QUEEN:
        return std::make_shared<Queen>(color, square, type, shared_from_this());
    case PieceType::ROOK:
        return std::make_shared<Rook>(color, square, type, shared_from_this());
    case PieceType::BISHOP:
        return std::make_shared<Bishop>(color, square, type, shared_from_this());
    case PieceType::KNIGHT:
        return std::make_shared<Knight>(color, square, type, shared_from_this());
    case PieceType::PAWN:
        return std::make_shared<Pawn>(color, square, type, shared_from_this());
    default:
        throw std::runtime_error("cant create a piece that doesnt exist");
    }
}

void Board::place_piece(Color color, Coordinate square, PieceType type)
{
    // check if something is there already
    std::pair<int, int> idx = get_grid_indexes(square);
    if (this->grid[idx.first][idx.second] != nullptr) {
        this->remove_piece(square);
    }
    std::shared_ptr<Piece> p = this->create_piece(color, square, type);
    this->grid[idx.first][idx.second] = p;
    /* p->print_attack(); */
    /* p->print_move(); */
}

void Board::remove_piece(Coordinate square)
{
    std::pair<int, int> idx = get_grid_indexes(square);
    if (this->grid[idx.first][idx.second] == nullptr) {
        return;
    }
    this->grid[idx.first][idx.second] = nullptr;
}

std::string Board::serialize()
{
    /*
        uppercase -> white
        lowercase -> black
        top left -> bottom right
    */

    /* board pieces */
    std::string fen = "";

    for (int i = static_cast<int>(this->grid.size()) - 1; i >= 0; --i) {
        std::vector<std::shared_ptr<Piece>> pc = this->grid[i];
        int numEmpty = 0;
        for (std::shared_ptr<Piece> p : pc) {
            if (!p) {
                numEmpty += 1;
                continue;
            }
            if (numEmpty > 0) {
                fen.append(std::to_string(numEmpty));
                numEmpty = 0;
            }
            fen.append(std::string { p->get_piece_character() });
        }
        if (numEmpty > 0) {
            fen.append(std::to_string(numEmpty));
        }
        fen.append("/");
    }
    /* pop the last slash */
    fen.pop_back();

    fen.append(" ");

    /* active color */
    if (this->active_color == Color::WHITE) {
        fen.append("w");
    } else {
        fen.append("b");
    }

    fen.append(" ");

    /* castling rights */
    if (this->castle_rights[Color::WHITE][CastleSide::KINGSIDE]) {
        fen.append("K");
    }
    if (this->castle_rights[Color::WHITE][CastleSide::QUEENSIDE]) {
        fen.append("Q");
    }
    if (this->castle_rights[Color::BLACK][CastleSide::KINGSIDE]) {
        fen.append("k");
    }
    if (this->castle_rights[Color::BLACK][CastleSide::QUEENSIDE]) {
        fen.append("q");
    }

    fen.append(" ");

    /* en passant possible */
    if (this->en_passant_target == nullptr) {
        fen.append(" - ");
    } else {
        fen.append(std::string { this->en_passant_target->column }
            + std::to_string(this->en_passant_target->row));
    }

    fen.append(" ");

    /* half/full moves */
    fen.append(std::to_string(this->halfmove_clock) + " "
        + std::to_string(this->fullmove_clock));

    return fen;
}

std::vector<std::vector<std::shared_ptr<Piece>>>& Board::get_grid()
{
    return this->grid;
}

Color Board::get_active_color() { return this->active_color; }

void Board::destroy_piece(std::shared_ptr<Piece> p)
{
    std::pair<int, int> idx = get_grid_indexes(p->get_coordinate());
    this->grid[idx.first][idx.second] = nullptr;
    if (p->get_color() == Color::WHITE) {
        this->white_pieces.erase(p);
        if (p->get_piece_type() == PieceType::KING) {
            this->white_king = nullptr;
        }
    } else {
        this->black_pieces.erase(p);
        if (p->get_piece_type() == PieceType::KING) {
            this->black_king = nullptr;
        }
    }
}

void Board::add_piece(std::shared_ptr<Piece> p)
{
    // we cannot be adding a king, so we don't check
    std::pair<int, int> idx = get_grid_indexes(p->get_coordinate());
    this->grid[idx.first][idx.second] = p;
    if (p->get_color() == Color::WHITE) {
        this->white_pieces.insert(p);
        if (p->get_piece_type() == PieceType::KING) {
            this->white_king = p;
        }
    } else {
        this->black_pieces.insert(p);
        if (p->get_piece_type() == PieceType::KING) {
            this->black_king = p;
        }
    }
}

bool Board::is_promotion(Coordinate start, Coordinate end)
{
    std::pair<int, int> start_idx = get_grid_indexes(start);
    std::shared_ptr<Piece> p = this->grid[start_idx.first][start_idx.second];
    if (!p) {
        return false;
    }
    if (p->get_piece_type() != PieceType::PAWN) {
        return false;
    }
    if (p->get_color() == Color::WHITE) {
        return end.row == 8 && start.row == 7;
    }
    return end.row == 1 && start.row == 2;
}

Coordinate Board::get_enpassant_coordinate() { return *this->en_passant_target; }

bool Board::is_valid_enpassant(Coordinate start, Coordinate end)
{
    // theres implicitly no piece there
    // TODO: check if theres pieces there
    if (!this->en_passant_enabled) {
        return false;
    }
    if (this->en_passant_target == nullptr) {
        return false;
    }
    if (!(*this->en_passant_target == end)) {
        return false;
    }
    std::pair<int, int> start_idx = get_grid_indexes(start);
    std::shared_ptr<Piece> p = this->grid[start_idx.first][start_idx.second];
    if (!p) {
        return false;
    }
    if (p->get_piece_type() != PieceType::PAWN) {
        return false;
    }
    std::shared_ptr<Pawn> pawn = std::dynamic_pointer_cast<Pawn>(p);
    bool has_move_to_end = false;
    for (auto i : pawn->get_captures()) {
        std::pair<int, int> square = add_pairs(start_idx, i);
        if (Coordinate(square.first, square.second) == end) {
            has_move_to_end = true;
        }
    }
    if (!has_move_to_end) {
        return false;
    }

    Coordinate taken_piece_coord = this->get_enpassant_taken_piece_coordinate();
    std::pair<int, int> taken_piece_idx = get_grid_indexes(taken_piece_coord);
    std::shared_ptr<Piece> taken_piece
        = this->grid[taken_piece_idx.first][taken_piece_idx.second];
    if (taken_piece == nullptr) {
        return false;
    }
    return true;
}

bool Board::is_valid_castle(Coordinate start, Coordinate end)
{
    std::pair<int, int> start_idx = get_grid_indexes(start);
    if (!is_double_king_move(start, end)) {
        return false;
    }
    std::shared_ptr<Piece> p = this->grid[start_idx.first][start_idx.second];
    if (!p) {
        return false;
    }
    if (p->get_piece_type() != PieceType::KING) {
        return false;
    }
    std::shared_ptr<King> king = std::dynamic_pointer_cast<King>(p);
    // check castle rights
    if (king->get_color() == Color::WHITE) {
        if (start.column < end.column) {
            if (!this->castle_rights[Color::WHITE][CastleSide::KINGSIDE]) {
                return false;
            }
        } else {
            if (!this->castle_rights[Color::WHITE][CastleSide::QUEENSIDE]) {
                return false;
            }
        }
    } else {
        if (start.column < end.column) {
            if (!this->castle_rights[Color::BLACK][CastleSide::KINGSIDE]) {
                return false;
            }
        } else {
            if (!this->castle_rights[Color::BLACK][CastleSide::QUEENSIDE]) {
                return false;
            }
        }
    }

    // check if all squares are empty
    std::set<Coordinate> s;
    this->get_threatened_squares_by_color(s, toggle_color(king->get_color()));
    // can't castle when in check
    if (!(s.find(start) == s.end())) {
        return false;
    }
    // check if squares gone to for king is valid
    std::pair<int, int> d;
    if (start.column < end.column) {
        // right
        d = { 0, 1 };
    } else {
        // left
        d = { 0, -1 };
    }
    std::pair<int, int> cur = get_grid_indexes(start);
    for (int i = 0; i < 2; i++) {
        cur = add_pairs(cur, d);
        if (this->grid[cur.first][cur.second] != nullptr) {
            return false;
        }
        Coordinate c = Coordinate(cur.first, cur.second);
        if (!(s.find(c) == s.end())) {
            return false;
        }
    }
    // queenside
    if (start.column > end.column) {
        d = { 0, -1 };
        std::pair<int, int> cur = get_grid_indexes(end);
        cur = add_pairs(cur, d);
        if (this->grid[cur.first][cur.second] != nullptr) {
            return false;
        }
    }
    return true;
}

bool Board::is_double_pawn_move(Coordinate start, Coordinate end)
{
    std::pair<int, int> start_idx = get_grid_indexes(start);
    std::shared_ptr<Piece> p = this->grid[start_idx.first][start_idx.second];
    if (!p) {
        return false;
    }
    if (p->get_piece_type() != PieceType::PAWN) {
        return false;
    }
    if (p->get_color() == Color::WHITE) {
        return start.row == 2 && end.row == 4 && start.column == end.column;
    }
    return start.row == 7 && end.row == 5 && start.column == end.column;
}

bool Board::is_double_king_move(Coordinate start, Coordinate end)
{
    std::pair<int, int> start_idx = get_grid_indexes(start);
    std::shared_ptr<Piece> p = this->grid[start_idx.first][start_idx.second];
    if (!p) {
        return false;
    }
    if (p->get_piece_type() != PieceType::KING) {
        return false;
    }

    bool valid = (end.column == 'g' || end.column == 'c') and (start.row == end.row)
        and (start.column == 'e');
    if (!valid) {
        return false;
    }

    if (p->get_color() == Color::WHITE) {
        return start.row == 1;
    }
    return start.row == 8;
}

void Board::add_enpassant_target(std::shared_ptr<Coordinate> c)
{
    if (!this->en_passant_enabled) {
        return;
    }
    if (this->en_passant_target) {
        this->en_passant_target = nullptr;
    }
    this->en_passant_target = c;
}

Coordinate Board::get_enpassant_taken_piece_coordinate()
{
    if ((*this->en_passant_target).row == 3) {
        return Coordinate(4, (*this->en_passant_target).column);

    } else if ((*this->en_passant_target).row == 6) {
        return Coordinate(5, (*this->en_passant_target).column);
    }
    throw std::runtime_error("enpassant target not on row 3 or 6");
}

std::shared_ptr<Coordinate> Board::get_enpassant_square_coordinate(Coordinate c)
{
    if (c.row == 4) {
        return std::make_shared<Coordinate>(3, c.column);

    } else if (c.row == 5) {
        return std::make_shared<Coordinate>(6, c.column);
    }
    throw std::runtime_error("enpassant move not on row 4 or 5");
}

Coordinate Board::get_castle_rook(Color c, CastleSide cs)
{
    if (c == Color::WHITE) {
        if (cs == CastleSide::KINGSIDE) {
            return Coordinate("h1");
        } else {
            return Coordinate("a1");
        }
    } else {
        if (cs == CastleSide::KINGSIDE) {
            return Coordinate("h8");
        } else {
            return Coordinate("a8");
        }
    }
}

std::set<std::shared_ptr<Piece>> Board::get_pieces(Color c)
{
    if (c == Color::WHITE) {
        return this->white_pieces;
    }
    return this->black_pieces;
}
