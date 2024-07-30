#ifndef MOVE_H
#define MOVE_H

#include "../enum.h"
#include "coordinate.h"
#include <memory>

class Piece;

struct MoveFlags {
    bool valid, check, capture, attacked_before, attacked_after;
    int score_difference, protected_diff, attacked_diff;

    MoveFlags(bool valid, bool check, bool capture, bool attacked_before,
        bool attacked_after, int score_difference, int protected_diff,
        int attacked_diff)
        : valid { valid }
        , check { check }
        , capture { capture }
        , attacked_before { attacked_before }
        , attacked_after { attacked_after }
        , score_difference { score_difference }
        , protected_diff { protected_diff }
        , attacked_diff { attacked_diff }
    {
    }

    MoveFlags(bool invalid)
        : valid { false }
        , check { false }
        , capture { false }
        , attacked_before { false }
        , attacked_after { false }
        , score_difference { 0 }
        , protected_diff { 0 }
        , attacked_diff { 0 }
    {
    }
};

#endif
