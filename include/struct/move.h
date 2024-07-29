#ifndef MOVE_H
#define MOVE_H

#include "../enum.h"
#include "coordinate.h"
#include <memory>

class Piece;

struct MoveFlags {
    bool valid, check, capture, attacked_before, attacked_after;

    MoveFlags(bool valid, bool check, bool capture, bool attacked_before,
        bool attacked_after)
        : valid { valid }
        , check { check }
        , capture { capture }
        , attacked_before { attacked_before }
        , attacked_after { attacked_after }
    {
    }

    MoveFlags(bool invalid)
        : valid { false }
        , check { false }
        , capture { false }
        , attacked_before { false }
        , attacked_after { false }
    {
    }
};

#endif
