#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include <unordered_map>

#include "enum.h"
#include "display/displayboard.h"

extern const std::unordered_map<Color, std::unordered_map<PieceType, char>> pieces;

class TextDisplay : public DisplayBoard {
public:
    TextDisplay(std::shared_ptr<Chess> chess);
    void show() override;
};

#endif
