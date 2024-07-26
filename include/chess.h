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
    std::vector<Players> players;
    std::vector<std::unique_ptr<DisplayBoard>> displays;
    void get_scores();

public:
    Chess();
    void resign();
    void setup_board(std::istream& in, bool& is_eof_given);
    void start_game(std::string white, std::string black);
    bool is_valid_move(Coordinate start, Coordinate end);
    void make_move(Coordinate start, Coordinate end,
        PromotionType promotion);
    bool is_next_move_human();
    void notify_displays();
    bool is_game_not_running();
    bool has_game();
    std::shared_ptr<Game> get_last_game();
    void init();
};

std::shared_ptr<Chess> createChess();
#endif
