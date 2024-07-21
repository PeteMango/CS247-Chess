#ifndef GRAPHICSDISPLAY_H
#define GRAPHICSDISPLAY_H

#include "display.h"

class XWindow;

class GraphicsDisplay : public Display {
    // TODO: add graphics display
    std::shared_ptr<XWindow> display;

public:
    GraphicsDisplay(std::shared_ptr<Chess> chess,
        std::shared_ptr<XWindow> display);
    void show() override;
};

#endif
