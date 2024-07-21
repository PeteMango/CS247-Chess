#include "../../include/display/graphicsdisplay.h"

GraphicsDisplay::GraphicsDisplay(std::shared_ptr<Chess> chess,
    std::shared_ptr<XWindow> display)
    : Display(chess)
    , display { display }
{
}

void GraphicsDisplay::show() { }

