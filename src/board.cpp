#include "../include/board.h"
#include "../include/piece/bishop.h"
#include "../include/piece/king.h"
#include "../include/piece/knight.h"
#include "../include/piece/pawn.h"
#include "../include/piece/queen.h"
#include "../include/piece/rook.h"
#include "../include/util.h"
#include <iostream>
#include <stdexcept>

// empty
Board::Board(bool default_board)
    : grid { 8, std::vector<std::shared_ptr<Piece>>(8, nullptr) }
    , active_color { Color::WHITE }
    , white_king { nullptr }
    , black_king { nullptr }
    , en_passant_targets()
    , halfmove_clock { 0 }
    , fullmove_clock { 0 }
{
    this->white_pieces = std::set<std::shared_ptr<Piece>>();
    this->black_pieces = std::set<std::shared_ptr<Piece>>();
    if (default_board) {
        this->en_passant_enabled = true;
        this->castle_rights[Color::WHITE][CastleSide::KINGSIDE] = true;
        this->castle_rights[Color::WHITE][CastleSide::QUEENSIDE] = true;
        this->castle_rights[Color::BLACK][CastleSide::KINGSIDE] = true;
        this->castle_rights[Color::BLACK][CastleSide::QUEENSIDE] = true;
    } else {
        this->en_passant_enabled = false;
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
    this->place_piece(
        Color::WHITE, Coordinate("e1"), PieceType::KING);
    this->place_piece(
        Color::BLACK, Coordinate("e8"), PieceType::KING);
    // queens
    this->place_piece(
        Color::WHITE, Coordinate("d1"), PieceType::QUEEN);
    this->place_piece(
        Color::BLACK, Coordinate("d8"), PieceType::QUEEN);
    // rooks
    this->place_piece(
        Color::WHITE, Coordinate("a1"), PieceType::ROOK);
    this->place_piece(
        Color::WHITE, Coordinate("h1"), PieceType::ROOK);
    this->place_piece(
        Color::BLACK, Coordinate("a8"), PieceType::ROOK);
    this->place_piece(
        Color::BLACK, Coordinate("h8"), PieceType::ROOK);
    // bishops
    this->place_piece(
        Color::WHITE, Coordinate("c1"), PieceType::BISHOP);
    this->place_piece(
        Color::WHITE, Coordinate("f1"), PieceType::BISHOP);
    this->place_piece(
        Color::BLACK, Coordinate("c8"), PieceType::BISHOP);
    this->place_piece(
        Color::BLACK, Coordinate("f8"), PieceType::BISHOP);
    // knights
    this->place_piece(
        Color::WHITE, Coordinate("b1"), PieceType::KNIGHT);
    this->place_piece(
        Color::WHITE, Coordinate("g1"), PieceType::KNIGHT);
    this->place_piece(
        Color::BLACK, Coordinate("b8"), PieceType::KNIGHT);
    this->place_piece(
        Color::BLACK, Coordinate("g8"), PieceType::KNIGHT);
    // pawns
    for (int i = 0; i < 8; i++) {
        char col = static_cast<char>(i + 'a');
        this->place_piece(
            Color::WHITE, Coordinate(2, col), PieceType::PAWN);
        this->place_piece(
            Color::BLACK, Coordinate(7, col), PieceType::PAWN);
    }
    this->verify_board();
}

Board::Board(const std::string& fen)
    : halfmove_clock { 0 }
    , fullmove_clock { 0 }
    , en_passant_enabled { true }
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
                // this->board[row][col]
                //     = std::make_shared<Piece>(c);
                col++;
            }
        }
        row++;
    }

    /* set active color */
    std::string activeColor;
    fenStream >> activeColor;
    if (activeColor[0] == 'b') {
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
        this->en_passant_targets = std::make_unique<Coordinate>(
            empassant[1] - '0', empassant[0] - 'a' + 1);
    }

    /* half/full move clocks */
    fenStream >> this->halfmove_clock >> this->fullmove_clock;
}

bool Board::is_valid_move(Move m) { return true; }

std::string Board::make_move(Move m) { return ""; }

void Board::setup_board(std::istream& in) { return; }

// returns whether c is in check
bool Board::is_check(Color c)
{
    if (c == Color::WHITE && this->white_king == nullptr) {
        return false;
    } else if (c == Color::BLACK && this->black_king == nullptr) {
        return false;
    }
    std::set<Coordinate> s;

    if (c == Color::WHITE) {
        get_attacked_squares_by_color(s, Color::BLACK);
        return s.find(this->white_king->get_coordinate()) != s.end();
    }
    get_attacked_squares_by_color(s, Color::WHITE);
    return s.find(this->black_king->get_coordinate()) != s.end();
}

bool Board::is_stalemate() { return true; }

bool Board::is_checkmate() { return true; }

void Board::get_attacked_squares_by_color(std::set<Coordinate>& s, Color c)
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

void Board::get_possible_moves_by_color(std::set<Move>& m, Color c) { }

void Board::verify_board()
{
    // no pawns in the last rows
    for (int i = 0; i < 8; i++) {
        if (this->grid[0][i]
            && this->grid[0][i]->get_piece_type() == PieceType::PAWN) {
            throw std::invalid_argument("pawn in the outer rows");
        }
        if (this->grid[7][i]
            && this->grid[7][i]->get_piece_type() == PieceType::PAWN) {
            throw std::invalid_argument("pawn in the outer rows");
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
                        throw std::invalid_argument("multiple white kings");
                    }
                    this->white_king = p;
                }
            } else {
                this->black_pieces.insert(p);
                if (p->get_piece_type() == PieceType::KING) {
                    if (this->black_king != nullptr) {
                        throw std::invalid_argument("multiple black kings");
                    }
                    this->black_king = p;
                }
            }
        }
    }
    // neither king in check
    if (this->is_check(Color::WHITE) || this->is_check(Color::BLACK)) {
        throw std::invalid_argument("king in check");
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
        throw std::logic_error("cant create a piece that doesnt exist");
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
    p->print_attack();
    p->print_move();
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

    for (const std::vector<std::shared_ptr<Piece>>& pc : this->grid) {
        int numEmpty = 0;
        for (std::shared_ptr<Piece> p : pc) {
            if (p == nullptr) {
                numEmpty += 1;
                continue;
            }
            if (p->get_color() == Color::WHITE) {
                PieceType pt = p->get_piece_type();

                /* if there are empty spaces before */
                if (numEmpty > 0) {
                    fen.append(std::to_string(numEmpty));
                    numEmpty = 0;
                }

                /* TODO: replace the piece's enum with the
                 * appropriate character
                 * append the piece type to the string */
                fen.append(std::to_string(int(pt)));
            }
            fen.append("/");
        }
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
    if (this->en_passant_targets == nullptr) {
        fen.append(" - ");
    }

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

void Board::delete_piece(std::shared_ptr<Piece> p, Color color)
{
}

void Board::add_piece(std::shared_ptr<Piece> p, Color color) { }

bool Board::is_promotion(Coordinate start, Coordinate end)
{
    return true;
}
