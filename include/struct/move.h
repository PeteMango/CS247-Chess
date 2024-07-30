#ifndef MOVE_H
#define MOVE_H

#include "../enum.h"
#include "coordinate.h"
#include <memory>

class Piece;

struct MoveFlags {
    bool valid, check, capture, attacked_before, attacked_after, capture_win,
        good_check;
    int score_difference, protected_diff, attacked_diff, weight_difference,
        capture_value_difference;
    bool is_checkmate;

    MoveFlags(bool valid, bool check, bool capture, bool attacked_before,
        bool attacked_after, int score_difference, int protected_diff,
        int attacked_diff, bool capture_win, int weight_difference,
        int max_value_attacked, int capture_value_difference, bool good_check,
        bool is_checkmate)
        : valid { valid }
        , check { check }
        , capture { capture }
        , attacked_before { attacked_before }
        , attacked_after { attacked_after }
        , capture_win { capture_win }
        , good_check { good_check }
        , score_difference { score_difference }
        , protected_diff { protected_diff }
        , attacked_diff { attacked_diff }
        , weight_difference { weight_difference }
        , capture_value_difference { capture_value_difference }
        , is_checkmate { is_checkmate }
    {
    }

    MoveFlags(bool invalid)
        : valid { false }
        , check { false }
        , capture { false }
        , attacked_before { false }
        , attacked_after { false }
        , capture_win { false }
        , good_check { false }
        , score_difference { 0 }
        , protected_diff { 0 }
        , attacked_diff { 0 }
        , weight_difference { 0 }
        , capture_value_difference { 0 }
        , is_checkmate { false }
    {
    }
};

#endif
