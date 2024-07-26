#include "chess.h"
#include "../include/player/computer.h"
#include "../include/player/human.h"
#include "display/graphicsdisplay.h"
#include "display/textdisplay.h"
#include "game.h"
#include <iostream>
#include <stdexcept>

// implemented
Chess::Chess() { }

void Chess::init()
{
    auto self = shared_from_this();
    std::unique_ptr<TextDisplay> t = std::make_unique<TextDisplay>(self);

    std::unique_ptr<GraphicsDisplay> g = std::make_unique<GraphicsDisplay>(self);

    this->displays.push_back(std::move(t));
    this->displays.push_back(std::move(g));
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
    std::shared_ptr<Game> game = std::make_shared<Game>(shared_from_this(), false);
    this->games.push_back(game);
    this->get_last_game()->setup_board(in, is_eof_given);
}

std::shared_ptr<Player> Chess::create_player(PlayerType type, Color color)
{
    switch (type) {
    case PlayerType::HUMAN:
        return std::make_shared<Human>(this->get_last_game(), color);
    case PlayerType::LEVEL1:
        return std::make_shared<Computer>(this->get_last_game(), color);
    // TODO: add subclasses to computer
    case PlayerType::LEVEL2:
        throw std::invalid_argument("not implemented");
        return std::make_shared<Computer>(this->get_last_game(), color);
    case PlayerType::LEVEL3:
        throw std::invalid_argument("not implemented");
        return std::make_shared<Computer>(this->get_last_game(), color);
    case PlayerType::LEVEL4:
        throw std::invalid_argument("not implemented");
        return std::make_shared<Computer>(this->get_last_game(), color);
    default:
        throw std::logic_error("cant create a player that doesnt exist");
    }
}

void Chess::start_game(PlayerType white, PlayerType black)
{
    // already validated, need to figure out if we need to create
    // a new game or not
    std::shared_ptr<Game> g = nullptr;
    if (this->has_game()) {
        g = this->get_last_game();
    }

    // create new
    if (!(this->has_game() && !g->is_game_complete() && !g->is_game_started())) {
        g = std::make_shared<Game>(shared_from_this(), true);
        this->games.push_back(g);
    }
    // g now is the current game

    // create players
    std::shared_ptr<Player> w = this->create_player(white, Color::WHITE);
    std::shared_ptr<Player> b = this->create_player(black, Color::BLACK);
    std::shared_ptr<Players> p = std::make_shared<Players>(w, b);
    this->players.push_back(std::move(p));
    g->update_start(true);
}

bool Chess::is_valid_move(Coordinate start, Coordinate end) { }

// TODO: maybe not
void Chess::make_move(Coordinate start, Coordinate end, PromotionType promotion) { }

void Chess::get_scores() { }

void Chess::notify_displays()
{
    for (int i = 0; i < this->displays.size(); i++) {
        displays[i]->show();
    }
}

bool Chess::is_next_move_human()
{
    std::shared_ptr<Game> g = this->get_last_game();
    std::shared_ptr<Board> b = g->get_board();
    std::shared_ptr<Players> p = this->get_last_game_players();
    if (b->get_active_color() == Color::WHITE) {
        return p->white->player_is_human();
    }
    return p->black->player_is_human();
}

Color Chess::get_current_active_color()
{
    std::shared_ptr<Game> g = this->get_last_game();
    std::shared_ptr<Board> b = g->get_board();
    return b->get_active_color();
}

bool Chess::is_game_not_running()
{
    return this->games.empty() || this->get_last_game()->is_game_complete();
}

bool Chess::can_start_game()
{
    std::shared_ptr<Game> g = nullptr;
    if (this->has_game()) {
        g = this->get_last_game();
    }
    // gone through setup
    if (this->has_game() && !g->is_game_complete() && !g->is_game_started()) {
        return true;
    }
    // default configuration
    else if ((this->has_game() && g->is_game_complete() && g->is_game_started())
        || (!this->has_game())) {
        return true;
    }
    // invalid
    return false;
}

bool Chess::has_game() { return !this->games.empty(); }

std::shared_ptr<Game> Chess::get_last_game()
{
    return this->games[this->games.size() - 1];
}

bool Chess::has_players() { return !this->players.empty(); }

std::shared_ptr<Players> Chess::get_last_game_players()
{
    return this->players[this->players.size() - 1];
}

// call has players, etc
bool Chess::can_move() { return true; }

bool Chess::is_promotion(Coordinate start, Coordinate end) { return true; }
