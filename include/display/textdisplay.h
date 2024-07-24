#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include "../enum.h"
#include "display.h"
#include <unordered_map>

extern const std::unordered_map<Color,
    std::unordered_map<PieceType, char>>
    pieces;
class TextDisplay : public Display {
public:
    TextDisplay(std::shared_ptr<Chess> chess);
    void show() override;
};

#endif
