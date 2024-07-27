#include "../../include/player/human.h"
#include "../../include/board.h"
#include "../../include/game.h"
#include "../../include/util.h"

Human::Human(std::shared_ptr<Game> game, Color color)
    : Player(game, true, color)
{
}

void Human::make_move(Coordinate start, Coordinate end, PromotionType promotion)
{
    // promorion, capture, regular, en passant, castle
    std::pair<int, int> starting_idx = get_grid_indexes(start);
    std::pair<int, int> ending_idx = get_grid_indexes(end);
    std::shared_ptr<Board> board = this->game->get_board();
    std::vector<std::vector<std::shared_ptr<Piece>>> grid = board->get_grid();
    std::shared_ptr<Piece> p = grid[starting_idx.first][starting_idx.second];

    // capture
    if (grid[ending_idx.first][ending_idx.second] != nullptr) {
        std::shared_ptr<Piece> taken = grid[ending_idx.first][ending_idx.second];
        board->delete_piece(taken);
    }
    // elif en passant
    // castle???

    board->delete_piece(p);
    std::shared_ptr<Piece> new_p = nullptr;
    if (board->is_promotion(start, end)) {
        new_p = board->create_piece(
            p->get_color(), end, PromotionTypeToPieceType(promotion));
    } else {
        new_p = board->create_piece(p->get_color(), end, p->get_piece_type());
    }
    board->add_piece(new_p);
}
