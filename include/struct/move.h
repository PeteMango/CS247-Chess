#ifndef MOVE_H
#define MOVE_H

#include "../enum.h"
#include "coordinate.h"
#include <memory>

class Piece;

struct MoveFlags {
    bool valid, check, capture, attacked_before, attacked_after;
    int value = 0;

    MoveFlags(bool valid, bool check, bool capture, bool attacked_before,
        bool attacked_after, int value)
        : valid { valid }
        , check { check }
        , capture { capture }
        , attacked_before { attacked_before }
        , attacked_after { attacked_after }
        , value { value }
    {
    }

    MoveFlags(bool invalid)
        : valid { false }
        , check { false }
        , capture { false }
        , attacked_before { false }
        , attacked_after { false }
        , value { -1 }
    {
    }
};

#endif
