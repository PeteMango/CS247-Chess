#ifndef CHESS_H
#define CHESS_H

#include "display/display.h"
#include "player/player.h"
#include "struct.h"
#include <utility>
#include <vector>

class Game;

class Chess {
    std::vector<std::shared_ptr<Game>> games;
    std::vector<Players> players;
    std::vector<Display> displays;
    void get_scores();
    void notify_displays();

public:
    Chess();
    void resign();
    void setup_board(std::istream& in);
    void start_game(Player white, Player black);
    bool is_valid_move(Coordinate start, Coordinate end);
    void make_move(Coordinate start, Coordinate end,
        PromotionType promotion);
};

#endif
