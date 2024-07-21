#ifndef PLAYERS_H
#define PLAYERS_H

#include "../player/player.h"
#include <memory>

struct Players {
    std::unique_ptr<Player> white;
    std::unique_ptr<Player> black;
    Players(std::unique_ptr<Player> white,
        std::unique_ptr<Player> black);
};

#endif
