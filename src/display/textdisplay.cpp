#include "display/textdisplay.h"
#include "chess.h"
#include "game.h"
#include "piece/piece.h"
#include <iostream>

TextDisplay::TextDisplay(std::shared_ptr<Chess> chess)
    : Display(chess)
{
}

const std::unordered_map<Color,
    std::unordered_map<PieceType, char>>
    pieces = { { Color::WHITE,
                   { { PieceType::PAWN, 'P' },
                       { PieceType::KNIGHT, 'N' },
                       { PieceType::BISHOP, 'B' },
                       { PieceType::ROOK, 'R' },
                       { PieceType::QUEEN, 'Q' },
                       { PieceType::KING, 'K' } } },
        { Color::BLACK,
            { { PieceType::PAWN, 'p' },
                { PieceType::KNIGHT, 'n' },
                { PieceType::BISHOP, 'b' },
                { PieceType::ROOK, 'r' },
                { PieceType::QUEEN, 'q' },
                { PieceType::KING, 'k' } } } };

void TextDisplay::show()
{
    if (!this->chess->has_game()) {
        return;
    }
    std::shared_ptr<Game> g = this->chess->get_last_game();
    std::shared_ptr<Board> b = g->get_board();
    std::vector<std::vector<std::shared_ptr<Piece>>>& grid
        = b->get_grid();

    for (int row = 7; row >= 0; row--) {
        std::cout << row + 1 << " ";
        for (int col = 0; col < 8; col++) {
            if (grid[row][col] == nullptr) {
                // even is black, odd is white
                if ((row + col) % 2 == 0) {
                    std::cout << "_";
                } else {
                    std::cout << " ";
                }
            } else {
                Color color = grid[row][col]->get_color();
                PieceType type
                    = grid[row][col]->get_piece_type();
                std::cout << pieces.at(color).at(type);
            }
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
    std::cout << "  ";

    for (int col = 0; col < 8; col++) {
        std::cout << (char)(col + 'a');
    }

    std::cout << std::endl;
}
