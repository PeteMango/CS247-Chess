#include "display/graphicsdisplay.h"
#include "chess.h"
#include "display/xwindow.h"
#include "game.h"
#include "piece/piece.h"
#include "util.h"
#include <unordered_map>

GraphicsDisplay::GraphicsDisplay(std::shared_ptr<Chess> chess)
    : DisplayBoard(chess)
    , xwin_display { nullptr }
{
}

void GraphicsDisplay::setupBoard()
{
    xwin_display = std::make_shared<xwindow>();

    const int BLACK_SQUARE_COLOR = xwindow::Green;
    const int WHITE_SQUARE_COLOR = xwindow::White;

    // draw squares
    for (int row = 7, color; row >= 0; row--)
        for (int col = 0; col < 8; col++) {
            color = (row + col) % 2 == 0 ? BLACK_SQUARE_COLOR : WHITE_SQUARE_COLOR;
            xwin_display->fillGrid(col, row, color);
        }

    // draw row numbers and column letters
    for (int i = 0; i < 8; ++i) {
        xwin_display->drawPiece(-1, i, '1' + i);
        xwin_display->drawPiece(i, -1, 'a' + i);
    }
}

void GraphicsDisplay::show()
{
    if (!this->chess->has_game()) {
        return;
    }
    std::shared_ptr<Game> g = this->chess->get_last_game();
    std::shared_ptr<Board> b = g->get_board();
    std::vector<std::vector<std::shared_ptr<Piece>>>& grid = b->get_grid();

    if (isSetup == false) {
        setupBoard();
        isSetup = true;
    }

    for (int row = 7; row >= 0; row--)
        for (int col = 0; col < 8; col++)
            if (grid[row][col] != nullptr) {
                Color pieceColor = grid[row][col]->get_color();
                PieceType pieceType = grid[row][col]->get_piece_type();

                char piece = pieces.at(pieceColor).at(pieceType);

                xwin_display->drawPiece(col, row, piece);
            }
}

void GraphicsDisplay::show_status() { }
