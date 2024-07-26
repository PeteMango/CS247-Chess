#include "display/display.h"

Display::Display(std::shared_ptr<Chess> chess)
    : chess { chess }
{
}

Display::~Display() { }
