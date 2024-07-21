#include "../../include/struct/move.h"

Move::Move(std::shared_ptr<Piece> piece, Coordinate start,
    Coordinate end, PromotionType promotion)
    : piece { piece }
    , start { start }
    , end { end }
    , promotion { promotion }
{
}
