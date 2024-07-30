#ifndef __WINDOW_H__
#define __WINDOW_H__
#include <X11/Xlib.h>
#include <iostream>
#include <string>
#include <vector>

class xwindow {
    Display* d;
    Window w;
    int s;
    GC gc;
    unsigned long colours[10];
    const int SQUARE_SIZE = 120;
    const int STRING_OFFSET = 8;

public:
    xwindow();
    ~xwindow();

    enum { White = 0, Black, Red, Green, Blue };
    std::vector<std::vector<char>> rendered_board;

    void fillGrid(int x, int y, int colour = Black);
    void drawPiece(int x, int y, char piece);
};

#endif
