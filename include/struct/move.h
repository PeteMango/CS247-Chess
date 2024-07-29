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
    bool escapes;

    MoveFlags(bool valid, bool check, bool capture, bool escapes)
        : valid { valid }
        , check { check }
        , capture { capture }
        , escapes { escapes }
    {
    }
};

#endif
