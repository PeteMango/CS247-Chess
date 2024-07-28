#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include <unordered_map>

#include "display/displayboard.h"
#include "enum.h"

extern const std::unordered_map<Color, std::unordered_map<PieceType, char>> pieces;

class TextDisplay : public DisplayBoard {
    void display_doubled_number(int num);

public:
    TextDisplay(std::shared_ptr<Chess> chess);
    void show() override;
    void show_status(DisplayStatus s, Color c) override;
    void show_results(int white_doubled_results, int black_doubled_results) override;
};

#endif
