#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include "display.h"

class TextDisplay : public Display {
public:
    TextDisplay();
    void show() override;
};

#endif
