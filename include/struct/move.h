#ifndef MOVE_H
#define MOVE_H

#include "../enum.h"
#include "coordinate.h"
#include <memory>

class Piece;

struct MoveFlags {
    bool valid, check, capture, attacked_before, attacked_after, good_trade,
        is_checkmate;
    int gain;

    MoveFlags(bool valid, bool check, bool capture, bool attacked_before,
        bool attacked_after, bool good_trade, bool is_checkmate, int gain)
        : valid { valid }
        , check { check }
        , capture { capture }
        , attacked_before { attacked_before }
        , attacked_after { attacked_after }
        , good_trade { good_trade }
        , is_checkmate { is_checkmate }
        , gain { gain }
    {
    }

    MoveFlags(bool invalid)
        : valid { false }
        , check { false }
        , capture { false }
        , attacked_before { false }
        , attacked_after { false }
        , good_trade { false }
        , is_checkmate { false }
        , gain { 0 }
    {
    }
};

#endif
