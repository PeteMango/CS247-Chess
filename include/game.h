#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "enum.h"
#include "struct/move.h"
#include <vector>

class Game {
    std::unique_ptr<Board> board;
    std::vector<Move> moves;
    bool is_complete;
    Color winner;

public:
    Game();
    bool is_game_complete();
    Color get_winner();
    void resign();
    bool is_valid_move(Move m);
    void make_move(Move m);
    void setup_board(std::istream& in);
};
#endif
