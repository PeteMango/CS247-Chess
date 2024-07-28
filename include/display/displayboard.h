#ifndef DISPLAYBOARD_H
#define DISPLAYBOARD_H

#include "enum.h"
#include <memory>

class Chess;

class DisplayBoard {
protected:
    std::shared_ptr<Chess> chess;

public:
    DisplayBoard(std::shared_ptr<Chess> chess);
    virtual ~DisplayBoard();
    virtual void show() = 0;
    virtual void show_status(DisplayStatus s, Color c) = 0;
};

#endif
