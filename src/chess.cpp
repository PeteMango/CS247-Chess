#include "../include/chess.h"
#include "../include/game.h"

// implemented
Chess::Chess() { }

void Chess::resign() { }

void Chess::setup_board(std::istream& in, bool& is_eof_given)
{
    // empty game
    std::shared_ptr<Game> game
        = std::make_shared<Game>(shared_from_this());
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

void Chess::notify_displays() { }

bool Chess::is_next_move_human() { }

bool Chess::is_game_not_running()
{
    return this->games.empty()
        || this->games[this->games.size() - 1]
               ->is_game_complete();
}
