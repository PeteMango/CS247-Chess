#include "chess.h"
#include "../include/player/computer.h"
#include "../include/player/human.h"
#include "display/graphicsdisplay.h"
#include "display/textdisplay.h"
#include "game.h"
#include "player/computer/levelfour.h"
#include "player/computer/levelone.h"
#include "player/computer/levelthree.h"
#include "player/computer/leveltwo.h"
#include "util.h"
#include <iostream>
#include <stdexcept>

Chess::Chess() { }

void Chess::init(bool graphics_mode)
{
    auto self = shared_from_this();
    std::unique_ptr<TextDisplay> t = std::make_unique<TextDisplay>(self);
    this->displays.push_back(std::move(t));
    if (graphics_mode) {
        std::unique_ptr<GraphicsDisplay> g = std::make_unique<GraphicsDisplay>(self);
        this->displays.push_back(std::move(g));
    }
}

std::shared_ptr<Chess> createChess(bool graphics_mode)
{
    std::shared_ptr<Chess> c = std::make_shared<Chess>();
    c->init(graphics_mode);
    return c;
}

void Chess::resign()
{
    std::shared_ptr<Game> g = this->get_last_game();
    g->resign();
}

void Chess::setup_board(std::istream& in, bool& is_eof_given)
{
    // empty game
    std::shared_ptr<Game> game = createGame(shared_from_this(), false);
    this->games.push_back(game);
    this->get_last_game()->setup_board(in, is_eof_given);
}

std::shared_ptr<Player> Chess::create_player(PlayerType type, Color color)
{
    switch (type) {
    case PlayerType::HUMAN:
        return std::make_shared<Human>(this->get_last_game(), color);
    case PlayerType::LEVEL1:
        return std::make_shared<LevelOne>(this->get_last_game(), color);
    case PlayerType::LEVEL2:
        return std::make_shared<LevelTwo>(this->get_last_game(), color);
    case PlayerType::LEVEL3:
        return std::make_shared<LevelThree>(this->get_last_game(), color);
    case PlayerType::LEVEL4:
        return std::make_shared<LevelFour>(this->get_last_game(), color);
    default:
        throw std::runtime_error("cant create a player that doesnt exist");
    }
}

void Chess::start_game(PlayerType white, PlayerType black)
{
    std::shared_ptr<Game> g = nullptr;
    if (this->has_game()) {
        g = this->get_last_game();
    }

    // create new
    if (!(this->has_game() && !g->is_game_complete() && !g->is_game_started())) {
        g = createGame(shared_from_this(), true);
        this->games.push_back(g);
    }

    // create players
    std::shared_ptr<Player> w = this->create_player(white, Color::WHITE);
    std::shared_ptr<Player> b = this->create_player(black, Color::BLACK);
    std::shared_ptr<Players> p = std::make_shared<Players>(w, b);
    this->players.push_back(p);
    g->update_start(true);
}

void Chess::make_move(Coordinate start, Coordinate end, PromotionType promotion)
{
    std::shared_ptr<Game> g = this->get_last_game();
    g->make_move(start, end, promotion);
}

void Chess::get_scores()
{
    int white = 0;
    int black = 0;
    for (long unsigned int i = 0; i < this->games.size(); i++) {
        if (!games[i]->is_game_complete()) {
            continue;
        }
        Result r = games[i]->get_result();
        if (r == Result::WHITE_WIN) {
            white += 2;
        } else if (r == Result::BLACK_WIN) {
            black += 2;
        } else if (r == Result::DRAW) {
            white += 1;
            black += 1;
        }
    }
    this->notify_results(white, black);
}

void Chess::notify_displays()
{
    for (long unsigned int i = 0; i < this->displays.size(); i++) {
        displays[i]->show();
    }
}

void Chess::notify_status(DisplayStatus s, Color c)
{
    for (long unsigned int i = 0; i < this->displays.size(); i++) {
        displays[i]->show_status(s, c);
    }
}

void Chess::notify_results(int white_doubled_results, int black_doubled_results)
{
    for (long unsigned int i = 0; i < this->displays.size(); i++) {
        displays[i]->show_results(white_doubled_results, black_doubled_results);
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
bool Chess::can_make_move()
{
    if (!this->has_game()) {
        return false;
    }
    std::shared_ptr<Game> g = this->get_last_game();
    if (g->is_game_started() && !g->is_game_complete()) {
        return true;
    }
    return false;
}

bool Chess::can_setup_board() { return this->is_game_not_running(); }
