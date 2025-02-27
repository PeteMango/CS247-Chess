#include "player/computer.h"

Computer::Computer(std::shared_ptr<Game> game, Color color)
    : Player(game, false, color)
{
}

void Computer::move() { }

void Computer::execute_moves_based_on_priority(
    std::vector<std::set<std::pair<Coordinate, Coordinate>>> moves)
{
    for (auto move : moves) {
        if (!move.empty()) {
            this->execute_move(move);
            return;
        }
    }
    throw std::runtime_error("computer not returning any possible moves");
}
void Computer::execute_move(std::set<std::pair<Coordinate, Coordinate>>& moves)
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
