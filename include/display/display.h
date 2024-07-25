#ifndef DISPLAY_H
#define DISPLAY_H

#include <memory>

class Chess;

class Display {
protected:
    std::shared_ptr<Chess> chess;

public:
    Display(std::shared_ptr<Chess> chess);
    virtual ~Display();
    virtual void show() = 0;
};

#endif
