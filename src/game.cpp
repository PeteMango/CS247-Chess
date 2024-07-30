#include "../include/game.h"
#include "../include/util.h"
#include "board.h"
#include "globals.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

Game::Game(std::shared_ptr<Chess> chess, bool default_board)
    : is_complete { false }
    , is_started { false }
    , chess { chess }
{
}

void Game::init(bool default_board)
{
    this->board = std::make_shared<Board>(shared_from_this(), default_board);
    if (default_board) {
        this->board->setup_default_board();
    }
}

std::shared_ptr<Game> createGame(std::shared_ptr<Chess> chess, bool default_board)
{
    std::shared_ptr<Game> g = std::make_shared<Game>(chess, default_board);
    g->init(default_board);
    return g;
}

bool Game::is_game_complete() { return this->is_complete; }

bool Game::is_game_started() { return this->is_started; }

Result Game::get_result() { return this->result; }

void Game::resign()
{
    auto chess = this->chess.lock();
    Color winner = toggle_color(this->get_board()->get_active_color());
    this->end_game(color_to_result(winner));
    chess->notify_displays();
    chess->notify_status(DisplayStatus::RESIGN, winner);
}

void Game::make_move(Coordinate start, Coordinate end, PromotionType promotion)
{
    std::string move = this->board->make_move(start, end, promotion);

    this->moves.push_back(move);
}

void Game::setup_board(std::istream& in, bool& is_eof_given)
{
    std::string line;
    Color color = Color::WHITE;
    while (true) {
        try {
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

                auto chess = this->chess.lock();
                chess->notify_displays();
            } else if (cmd == "-") {
                std::string coordinate;
                ss >> coordinate;
                if (!validate_coordinate(coordinate)) {
                    throw std::invalid_argument("invalid coordinate");
                }
                Coordinate coord(coordinate);
                this->board->remove_piece(coord);
                auto chess = this->chess.lock();
                chess->notify_displays();
            } else if (cmd == "=") {
                std::string col;
                ss >> col;
                if (!validate_color(col)) {
                    throw std::invalid_argument("invalid color");
                }
                Color c = string_to_color(col);
                color = c;
            } else if (cmd == "done") {
                this->board->verify_setup();
                break;
            } else {
                throw std::invalid_argument("invalid setup command");
            }
        } catch (const std::invalid_argument& e) {
            if (STRICT) {
                throw;
            } else {
                std::cerr << "Invalid Argument, please try again: " << e.what()
                          << std::endl;
                continue;
            }
        } catch (const std::runtime_error& e) {
            std::cerr << "Runtime Error: " << e.what() << std::endl;
            throw;
        } catch (...) {
            std::cerr << "An unexpected error occurred." << std::endl;
            throw;
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

std::shared_ptr<Chess> Game::get_chess()
{
    auto chess = this->chess.lock();
    return chess;
}
