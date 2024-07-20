#include "../include/struct.h"

// TODO: error checking
Coordinate::Coordinate(int row, int column) { }

Players::Players(
    std::unique_ptr<Player> white, std::unique_ptr<Player> black)
    : white { std::move(white) }
    , black { std::move(black) }
{
}

Move::Move(std::shared_ptr<Piece> piece, Coordinate start,
    Coordinate end, PromotionType promotion)
    : piece { piece }
    , start { start }
    , end { end }
    , promotion { promotion }
{
}

