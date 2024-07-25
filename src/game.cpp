#include "../include/game.h"
#include "../include/util.h"
#include <iostream>
#include <sstream>

Game::Game(std::shared_ptr<Chess> chess, bool default_board)
    : board { std::make_shared<Board>(default_board) }
    , is_complete { false }
    , is_started { false }
    , chess { chess }
{
}

bool Game::is_game_complete() { return this->is_complete; }

bool Game::is_game_started() { return this->is_started; }

Color Game::get_winner() { }

void Game::resign() { }

bool Game::is_valid_move(Move m) { }

std::string Game::make_move(Move m) { }

void Game::setup_board(std::istream& in, bool& is_eof_given)
{
    std::string line;
    Color color = Color::WHITE;
    while (true) {
        std::getline(in, line);
        if (in.eof()) {
            is_eof_given = true;
            break;
        }

        std::stringstream ss(line);
        std::string cmd;
        ss >> cmd;

        if (cmd == "+") {
            std::string piece;
            std::string coordinate;
            ss >> piece >> coordinate;
            if (!validate_piece(piece)) {
                throw std::invalid_argument("invalid piece");
            }
            if (!validate_coordinate(coordinate)) {
                throw std::invalid_argument(
                    "invalid coordinate");
            }
            Coordinate coord(coordinate);
            PieceType p = string_to_piecetype(piece);
            this->board->place_piece(color, coord, p);
            this->chess->notify_displays();
        } else if (cmd == "-") {
            throw std::logic_error("- unimplemented");
            std::string coordinate;
            ss >> coordinate;
            if (!validate_coordinate(coordinate)) {
                throw std::invalid_argument(
                    "invalid coordinate");
            }
            Coordinate coord(coordinate);
            this->board->remove_piece(coord);
            this->chess->notify_displays();
        } else if (cmd == "=") {
            std::string col;
            ss >> col;
            if (!validate_color(col)) {
                throw std::invalid_argument("invalid color");
            }
            Color c = string_to_color(col);
            color = c;
        } else if (cmd == "done") {
            break;
        } else {
            std::invalid_argument("invalid setup command");
        }
    }
}

std::shared_ptr<Board> Game::get_board() { return this->board; }

