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
    Result result;
    std::shared_ptr<Chess> chess;

public:
    Game(std::shared_ptr<Chess> chess, bool default_board = true);
    void setup_board(std::istream& in, bool& is_eof_given);
    std::string make_move(Coordinate start, Coordinate end, PromotionType promotion);
    void resign();
    // helper functions
    std::shared_ptr<Board> get_board();
    Result get_result();
    void update_start(bool started);
    bool is_game_complete();
    bool is_game_started();
    void end_game(Result result);
};
#endif
