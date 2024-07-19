#ifndef GAME_H
#define GAME_H

#include "enum.h"
#include "struct.h"
#include <vector>

class Board;

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
