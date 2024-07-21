#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include "display.h"

class TextDisplay : public Display {
public:
    TextDisplay(std::shared_ptr<Chess> chess);
    void show() override;
};

#endif
