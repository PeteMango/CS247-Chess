#include "display/graphicsdisplay.h"
#include "chess.h"
#include "display/xwindow.h"
#include "game.h"
#include "piece/piece.h"
#include <iostream>
#include <unordered_map>

GraphicsDisplay::GraphicsDisplay(std::shared_ptr<Chess> chess)
    : DisplayBoard(chess)
    // , xwin_display { std::make_shared<Xwindow>(10 * 50, 10 * 50) }
    , xwin_display { nullptr }
{
}

const std::unordered_map<Color, std::unordered_map<PieceType, char>> pieces
    = { { Color::WHITE,
            { { PieceType::PAWN, 'P' }, { PieceType::KNIGHT, 'N' },
                { PieceType::BISHOP, 'B' }, { PieceType::ROOK, 'R' },
                { PieceType::QUEEN, 'Q' }, { PieceType::KING, 'K' } } },
          { Color::BLACK,
              { { PieceType::PAWN, 'p' }, { PieceType::KNIGHT, 'n' },
                  { PieceType::BISHOP, 'b' }, { PieceType::ROOK, 'r' },
                  { PieceType::QUEEN, 'q' }, { PieceType::KING, 'k' } } } };

void GraphicsDisplay::show()
{
    xwin_display = std::make_shared<Xwindow>(9 * SQUARE_SIZE, 9 * SQUARE_SIZE);

    if (!this->chess->has_game()) {
        return;
    }
    std::shared_ptr<Game> g = this->chess->get_last_game();
    std::shared_ptr<Board> b = g->get_board();

    std::vector<std::vector<std::shared_ptr<Piece>>>& grid = b->get_grid();

    const int BLACK_SQUARE_COLOR = Xwindow::Black;
    const int WHITE_SQUARE_COLOR = Xwindow::White;
    const int TEXT_COLOR = Xwindow::Red;

    for (int row = 7, color; row >= 0; row--) {
        for (int col = 0; col < 8; col++) {
            color = (row + col) % 2 == 0 ? BLACK_SQUARE_COLOR : WHITE_SQUARE_COLOR;

            xwin_display->fillRectangle((col + 1) * SQUARE_SIZE,
                (7 - row) * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, color);

            if (grid[row][col] != nullptr) {
                Color pieceColor = grid[row][col]->get_color();
                PieceType pieceType = grid[row][col]->get_piece_type();
                char pieceChar = pieces.at(pieceColor).at(pieceType);
                std::string pieceStr(1, pieceChar);

                // Center the piece character in the square
                int textX = col * SQUARE_SIZE + SQUARE_SIZE / 2
                    - 5; // Adjust the offset to center the text
                int textY = (7 - row) * SQUARE_SIZE + SQUARE_SIZE / 2
                    + 5; // Adjust the offset to center the text

                xwin_display->drawString(textX + 50, textY, pieceStr);
            }
        }
    }

    // draw row numbers and column letters
    for (int i = 0; i < 8; ++i) {
        std::string rowNum = std::to_string(8 - i);
        xwin_display->drawString(
            SQUARE_SIZE / 2, i * SQUARE_SIZE + SQUARE_SIZE / 2, rowNum);

        std::string colLetter(1, 'a' + i);
        xwin_display->drawString((i + 1) * SQUARE_SIZE + SQUARE_SIZE / 2,
            8 * SQUARE_SIZE + SQUARE_SIZE / 2, colLetter);
    }

    // for (int row = 7; row >= 0; row--) {
    //     std::cout << row + 1 << " ";
    //     for (int col = 0; col < 8; col++) {
    //         if (grid[row][col] == nullptr) {
    //             // even is black, odd is white
    //             if ((row + col) % 2 == 0) {
    //                 std::cout << "_";
    //             } else {
    //                 std::cout << " ";
    //             }
    //         } else {
    //             Color color = grid[row][col]->get_color();
    //             PieceType type = grid[row][col]->get_piece_type();
    //             std::cout << pieces.at(color).at(type);
    //         }
    //     }
    //     std::cout << std::endl;
    // }

    // std::cout << std::endl;
    // std::cout << "  ";

    // for (int col = 0; col < 8; col++) {
    //     std::cout << (char)(col + 'a');
    // }

    // std::cout << std::endl;
}
