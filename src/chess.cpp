#include "../include/chess.h"

Chess::Chess() { }

void Chess::resign() { }

void Chess::setup_board(std::istream& in) { }

void Chess::start_game(
    std::unique_ptr<Player> white, std::unique_ptr<Player> black)
{
}

bool Chess::is_valid_move(Coordinate start, Coordinate end) { }

void Chess::make_move(
    Coordinate start, Coordinate end, PromotionType promotion)
{
}

void Chess::get_scores() { }

void Chess::notify_displays() { }

