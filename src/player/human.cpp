#include "../../include/player/human.h"
#include "../../include/board.h"
#include "../../include/game.h"
#include "../../include/util.h"

Human::Human(std::shared_ptr<Game> game, Color color)
    : Player(game, true, color)
{
}

void Human::make_move(
    Coordinate start, Coordinate end, PromotionType promotion)
{
    // promorion, capture, regular, en passant, castle
    std::pair<int, int> starting_idx = get_grid_indexes(start);
    std::pair<int, int> ending_idx = get_grid_indexes(end);
    std::shared_ptr<Board> board = this->game->get_board();
    std::vector<std::vector<std::shared_ptr<Piece>>> grid
        = board->get_grid();
    std::shared_ptr<Piece> p
        = grid[starting_idx.first][starting_idx.second];

    if (grid[ending_idx.first][ending_idx.second] != nullptr) {
        std::shared_ptr<Piece> taken
            = grid[ending_idx.first][ending_idx.second];
        /* // this can't be a king  shold be in validate */
        if (taken->get_color() == Color::WHITE) {
            board->delete_piece(taken, Color::WHITE);
        } else {
            board->delete_piece(taken, Color::BLACK);
        }
        grid[ending_idx.first][ending_idx.second] = nullptr;
    }
    // elif en passant
    // castle???

    if (board->is_promotion(start, end)) {
        std::shared_ptr<Piece> new_p
            = board->create_piece(p->get_color(), end,
                PromotionTypeToPieceType(promotion));
        grid[ending_idx.first][ending_idx.second] = new_p;
        // add it to the piece set
        board->add_piece(new_p, new_p->get_color());
        // delete it from the piece set
        board->delete_piece(p, p->get_color());
    } else {
        grid[ending_idx.first][ending_idx.second] = p;
        grid[starting_idx.first][starting_idx.second] = nullptr;
        p->update_location(end);
    }
}
