#include "display/textdisplay.h"
#include "chess.h"
#include "game.h"
#include "piece/piece.h"
#include "util.h"
#include <iostream>

TextDisplay::TextDisplay(std::shared_ptr<Chess> chess)
    : DisplayBoard(chess)
{
}

void TextDisplay::show()
{
    if (!this->chess->has_game()) {
        return;
    }
    std::shared_ptr<Game> g = this->chess->get_last_game();
    std::shared_ptr<Board> b = g->get_board();
    std::vector<std::vector<std::shared_ptr<Piece>>>& grid = b->get_grid();

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
                PieceType type = grid[row][col]->get_piece_type();
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

void TextDisplay::show_status(DisplayStatus s, Color c)
{
    if (!this->chess->has_game()) {
        return;
    }
    std::shared_ptr<Game> g = this->chess->get_last_game();
    std::shared_ptr<Board> b = g->get_board();
    // TODO: resigning
    if (s == DisplayStatus::CHECKMATE) {
        std::cout << "Checkmate! " << ColorToPrintString.at(c) << " wins!"
                  << std::endl;
    } else if (s == DisplayStatus::STALEMATE) {
        std::cout << "Stalemate!" << std::endl;
    } else if (s == DisplayStatus::CHECK) {
        std::cout << ColorToPrintString.at(c) << " is in check." << std::endl;
    }
}
