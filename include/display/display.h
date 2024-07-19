#ifndef DISPLAY_H
#define DISPLAY_H

#include <memory>

class Chess;

class Display {
    std::shared_ptr<Chess> chess;

public:
    virtual void show() = 0;
};

#endif
