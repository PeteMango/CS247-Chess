#include "display/displayboard.h"

DisplayBoard::DisplayBoard(std::shared_ptr<Chess> chess)
    : chess { chess }
{
}

DisplayBoard::~DisplayBoard() { }
