#include "../../include/struct/players.h"

Players::Players(
    std::shared_ptr<Player> white, std::shared_ptr<Player> black)
    : white { white }
    , black { black }
{
}
