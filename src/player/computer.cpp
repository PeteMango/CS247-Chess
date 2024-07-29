#include "player/computer.h"

Computer::Computer(std::shared_ptr<Game> game, Color color)
    : Player(game, false, color)
{
}

void Computer::move() { }

void Computer::execute_best_move(std::set<std::pair<Coordinate, Coordinate>> moves)
{
    int max_score = -100000;
    std::set<std::pair<Coordinate, Coordinate>> to_move;
    std::pair<Coordinate, Coordinate> best_pair;
    for (const std::pair<Coordinate, Coordinate>& mv : moves) {
        MoveFlags mf = this->game->get_board()->is_valid_move(mv.first, mv.second);
        if (mf.value > max_score) {
            max_score = mf.value;
            best_pair = mv;
        }
    }
    for (const std::pair<Coordinate, Coordinate>& mv : moves) {
        MoveFlags mf = this->game->get_board()->is_valid_move(mv.first, mv.second);
        if (mf.value == max_score) {
            to_move.insert(mv);
        }
    }

    std::pair<Coordinate, Coordinate> best_move;
    int best_score = -1000;
    for (const std::pair<Coordinate, Coordinate>& mv : to_move) {
        std::pair<int, int> start = get_grid_indexes(mv.first),
                            end = get_grid_indexes(mv.second);
        if (this->color == Color::WHITE
            and this->game->get_board()->white_weight[end.first][end.second]
                    - this->game->get_board()
                          ->white_weight[start.first][start.second]
                > best_score) {
            best_score = this->game->get_board()->white_weight[end.first][end.second]
                - this->game->get_board()->white_weight[start.first][start.second];
            best_move = mv;
        } else if (this->color == Color::BLACK
            and this->game->get_board()->black_weight[end.first][end.second]
                    - this->game->get_board()
                          ->black_weight[start.first][start.second]
                > best_score) {
            best_score = this->game->get_board()->black_weight[end.first][end.second]
                - this->game->get_board()->black_weight[start.first][start.second];
            best_move = mv;
        }
    }

    this->game->make_move(best_move.first, best_move.second, PromotionType::QUEEN);
    return;
}

void Computer::execute_move(std::set<std::pair<Coordinate, Coordinate>> moves)
{
    int index = this->random_number(0, moves.size(), moves.size());

    auto it = moves.begin();
    std::advance(it, index);

    this->game->make_move(it->first, it->second, PromotionType::QUEEN);
    return;
}

Computer::~Computer() { }

int Computer::random_number(int lower, int upper, int size)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(lower, upper);
    int num = distrib(gen);

    return num % size;
}
