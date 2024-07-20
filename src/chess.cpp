#include "../include/chess.h"

Chess::Chess() { }

void Chess::resign() { }

void Chess::setup_board(std::istream& in) { }

void Chess::start_game(std::string white, std::string black) { }

bool Chess::is_valid_move(Coordinate start, Coordinate end) { }

void Chess::make_move(
    Coordinate start, Coordinate end, PromotionType promotion)
{
}

void Chess::get_scores() { }

void Chess::notify_displays() { }

bool Chess::is_next_move_human() { }

