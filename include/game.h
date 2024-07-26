#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "chess.h"
#include "enum.h"
#include "struct/move.h"
#include <string>
#include <vector>

class Game {
    std::shared_ptr<Board> board;
    std::vector<std::string> moves;
    bool is_complete;
    bool is_started;
    Color winner;
    std::shared_ptr<Chess> chess;

public:
    Game(std::shared_ptr<Chess> chess, bool default_board = true);
    void setup_board(std::istream& in, bool& is_eof_given);
    bool is_valid_move(Coordinate start, Coordinate end);
    std::string make_move(Coordinate start, Coordinate end, PromotionType promotion);
    void resign();
    // helper functions
    std::shared_ptr<Board> get_board();
    Color get_winner();
    void update_start(bool started);
    bool is_game_complete();
    bool is_game_started();
};
#endif
