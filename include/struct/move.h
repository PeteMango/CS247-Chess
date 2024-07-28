#ifndef MOVE_H
#define MOVE_H

#include "../enum.h"
#include "coordinate.h"
#include <memory>

class Piece;

struct MoveFlags {
    bool valid;
    bool check;
    bool capture;

    MoveFlags(bool valid, bool check, bool capture)
        : valid(valid)
        , check(check)
        , capture(capture)
    {
    }
};

#endif
