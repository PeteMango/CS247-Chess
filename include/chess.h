#ifndef CHESS_H
#define CHESS_H

#include "display/displayboard.h"
#include "player/player.h"
#include "struct/players.h"
#include <memory>
#include <utility>
#include <vector>

class Game;

class Chess : public std::enable_shared_from_this<Chess> {
    std::vector<std::shared_ptr<Game>> games;
    std::vector<std::shared_ptr<Players>> players;
    std::vector<std::unique_ptr<DisplayBoard>> displays;

    std::shared_ptr<Player> create_player(PlayerType type, Color color);
    bool is_game_not_running();

public:
    Chess();
    void init(bool graphics_mode);
    void notify_displays();
    void notify_status(DisplayStatus s, Color c);
    void notify_results(int white_doubled_results, int black_doubled_results);
    // command functions
    void setup_board(std::istream& in, bool& is_eof_given);
    void start_game(PlayerType white, PlayerType black);
    void make_move(Coordinate start, Coordinate end, PromotionType promotion);
    void resign();
    void get_scores();
    // check command order functions
    bool can_setup_board();
    bool can_start_game();
    bool can_make_move();
    bool can_resign();
    // helper functions
    bool is_next_move_human();
    bool has_game();
    bool has_players();
    std::shared_ptr<Game> get_last_game();
    std::shared_ptr<Players> get_last_game_players();
};

std::shared_ptr<Chess> createChess(bool graphics_mode);
#endif
