#include "../../include/struct/players.h"

Players::Players(
    std::unique_ptr<Player> white, std::unique_ptr<Player> black)
    : white { std::move(white) }
    , black { std::move(black) }
{
}
