#include "../include/chess.h"
#include "../include/display/textdisplay.h"
#include "../include/game.h"
#include <iostream>

// implemented
Chess::Chess() { }

void Chess::init()
{
    auto self = shared_from_this();
    std::unique_ptr<TextDisplay> t
        = std::make_unique<TextDisplay>(self);
    this->displays.push_back(std::move(t));
}

std::shared_ptr<Chess> createChess()
{
    std::shared_ptr<Chess> c = std::make_shared<Chess>();
    c->init();
    return c;
}

void Chess::resign() { }

void Chess::setup_board(std::istream& in, bool& is_eof_given)
{
    // empty game
    std::shared_ptr<Game> game
        = std::make_shared<Game>(shared_from_this(), false);
    this->games.push_back(game);
    this->games[this->games.size() - 1]->setup_board(
        in, is_eof_given);
}

void Chess::start_game(std::string white, std::string black) { }

bool Chess::is_valid_move(Coordinate start, Coordinate end) { }

void Chess::make_move(
    Coordinate start, Coordinate end, PromotionType promotion)
{
}

void Chess::get_scores() { }

void Chess::notify_displays()
{
    for (int i = 0; i < this->displays.size(); i++) {
        displays[i]->show();
    }
}

bool Chess::is_next_move_human() { }

bool Chess::is_game_not_running()
{
    return this->games.empty()
        || this->games[this->games.size() - 1]
               ->is_game_complete();
}

bool Chess::has_game() { return !this->games.empty(); }

std::shared_ptr<Game> Chess::get_last_game()
{
    return this->games[this->games.size() - 1];
}

