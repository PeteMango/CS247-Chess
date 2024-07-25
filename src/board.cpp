#include "../include/board.h"

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
    this->castle_rights[Color::WHITE][CastleSide::KINGSIDE]
        = true;
    this->castle_rights[Color::WHITE][CastleSide::QUEENSIDE]
        = true;
    this->castle_rights[Color::BLACK][CastleSide::KINGSIDE]
        = true;
    this->castle_rights[Color::BLACK][CastleSide::QUEENSIDE]
        = true;

    if (default_board) {
        // TODO:
    }
}

Board::Board(const std::string& fen)
    : halfmove_clock { 0 }
    , fullmove_clock { 0 }
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
        this->castle_rights[Color::WHITE][CastleSide::KINGSIDE]
            = true;
    }
    if (castleRights.find('Q') != std::string::npos) {
        this->castle_rights[Color::WHITE][CastleSide::QUEENSIDE]
            = true;
    }
    if (castleRights.find('j') != std::string::npos) {
        this->castle_rights[Color::BLACK][CastleSide::KINGSIDE]
            = true;
    }
    if (castleRights.find('q') != std::string::npos) {
        this->castle_rights[Color::BLACK][CastleSide::QUEENSIDE]
            = true;
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

bool Board::is_check() { return true; }

bool Board::is_stalemate() { return true; }

bool Board::is_checkmate() { return true; }

void Board::get_attacked_squares_by_color(
    std::set<Coordinate>& s, Color c)
{
}

void Board::get_possible_moves_by_color(
    std::set<Move>& m, Color c)
{
}

bool Board::verify_board() { return true; }

void Board::place_piece(
    Color color, Coordinate square, PieceType type)
{
    return;
}

void Board::remove_piece(Coordinate square) { return; }

std::string Board::serialize()
{
    /*
        uppercase -> white
        lowercase -> black
        top left -> bottom right
    */

    /* board pieces */
    std::string fen = "";

    for (const std::vector<std::shared_ptr<Piece>>& pc :
        this->grid) {
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
    if (this->castle_rights[Color::WHITE]
                           [CastleSide::KINGSIDE]) {
        fen.append("K");
    }
    if (this->castle_rights[Color::WHITE]
                           [CastleSide::QUEENSIDE]) {
        fen.append("Q");
    }
    if (this->castle_rights[Color::BLACK]
                           [CastleSide::KINGSIDE]) {
        fen.append("k");
    }
    if (this->castle_rights[Color::BLACK]
                           [CastleSide::QUEENSIDE]) {
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

std::vector<std::vector<std::shared_ptr<Piece>>>&
Board::get_grid()
{
    return this->grid;
}
