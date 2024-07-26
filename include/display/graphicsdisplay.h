#ifndef GRAPHICSDISPLAY_H
#define GRAPHICSDISPLAY_H

#include "display/displayboard.h"

class Xwindow;

class GraphicsDisplay : public DisplayBoard {
    std::shared_ptr<Xwindow> xwin_display;
    const int SQUARE_SIZE = 50;

public:
    GraphicsDisplay(std::shared_ptr<Chess> chess);
    void show() override;
};

#endif
