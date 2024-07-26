#ifndef GRAPHICSDISPLAY_H
#define GRAPHICSDISPLAY_H

#include "display/displayboard.h"

class xwindow;

class GraphicsDisplay : public DisplayBoard {
    std::shared_ptr<xwindow> xwin_display;
    bool isSetup = false;
    void setupBoard();

public:
    GraphicsDisplay(std::shared_ptr<Chess> chess);
    void show() override;
};

#endif
