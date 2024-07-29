#include "display/textdisplay.h"
#include "chess.h"
#include "enum.h"
#include "game.h"
#include "piece/piece.h"
#include "util.h"
#include <iomanip>
#include <iostream>

TextDisplay::TextDisplay(std::shared_ptr<Chess> chess)
    : DisplayBoard(chess)
{
}

void TextDisplay::show()
{
    return;
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
    return;
    if (!this->chess->has_game()) {
        return;
    }
    std::shared_ptr<Game> g = this->chess->get_last_game();
    std::shared_ptr<Board> b = g->get_board();
    if (s == DisplayStatus::CHECKMATE) {
        std::cout << "Checkmate! " << ColorToPrintString.at(c) << " wins!"
                  << std::endl;
    } else if (s == DisplayStatus::STALEMATE) {
        std::cout << "Stalemate!" << std::endl;
    } else if (s == DisplayStatus::CHECK) {
        std::cout << ColorToPrintString.at(c) << " is in check." << std::endl;
    } else if (s == DisplayStatus::RESIGN) {
        std::cout << ColorToPrintString.at(c) << " wins!" << std::endl;
    }
}

void TextDisplay::show_results(int white_doubled_results, int black_doubled_results)
{
    std::cout << "Final Score:" << std::endl;
    std::cout << "White: ";
    this->display_doubled_number(white_doubled_results);
    std::cout << std::endl;
    std::cout << "Black: ";
    this->display_doubled_number(black_doubled_results);
    std::cout << std::endl;
}

void TextDisplay::display_doubled_number(int num)
{
    return;
    float res = num / 2.0f;
    if (num % 2 == 0) {
        std::cout << static_cast<int>(res);
    } else {
        std::cout << std::fixed << std::setprecision(1) << res;
    }
}
