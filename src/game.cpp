#include "../include/game.h"
#include "../include/util.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

Game::Game(std::shared_ptr<Chess> chess, bool default_board)
    : board { std::make_shared<Board>(chess, default_board) }
    , is_complete { false }
    , is_started { false }
    , chess { chess }
{
    if (default_board) {
        this->board->setup_default_board();
    }
}

bool Game::is_game_complete() { return this->is_complete; }

bool Game::is_game_started() { return this->is_started; }

Result Game::get_result() { return this->result; }

void Game::resign() { throw std::invalid_argument("unimplemented"); }

std::string Game::make_move(
    Coordinate start, Coordinate end, PromotionType promotion)
{
    throw std::invalid_argument("unimplemented");
}

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
                throw std::invalid_argument("invalid coordinate");
            }
            Coordinate coord(coordinate);
            PieceType p = string_to_piecetype(piece);
            this->board->place_piece(color, coord, p);

            this->chess->notify_displays();
        } else if (cmd == "-") {
            std::string coordinate;
            ss >> coordinate;
            if (!validate_coordinate(coordinate)) {
                throw std::invalid_argument("invalid coordinate");
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
            this->board->verify_board();
            break;
        } else {
            std::invalid_argument("invalid setup command");
        }
    }
}

std::shared_ptr<Board> Game::get_board() { return this->board; }

void Game::update_start(bool started) { this->is_started = started; }

void Game::end_game(Result result)
{
    this->is_complete = true;
    this->result = result;
}
