#ifndef PLAYERS_H
#define PLAYERS_H

#include <memory>

class Player;

struct Players {
    std::unique_ptr<Player> white;
    std::unique_ptr<Player> black;
    Players(std::unique_ptr<Player> white,
        std::unique_ptr<Player> black);
};

#endif
