#ifndef GRAPHICSDISPLAY_H
#define GRAPHICSDISPLAY_H

#include "display.h"

class XWindow;

class GraphicsDisplay : public Display {
    // TODO: add graphics display
    std::shared_ptr<XWindow> display;

public:
    GraphicsDisplay();
    void show() override;
};

#endif
