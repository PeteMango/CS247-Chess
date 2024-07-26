#ifndef PLAYERS_H
#define PLAYERS_H

#include "../player/player.h"
#include <memory>

struct Players {
    std::shared_ptr<Player> white;
    std::shared_ptr<Player> black;
    Players(std::shared_ptr<Player> white,
        std::shared_ptr<Player> black);
};

#endif
