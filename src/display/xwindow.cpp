#include "display/xwindow.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

xwindow::xwindow()
    : rendered_board { 8, std::vector<char>(8, ']') }
{
    int width = SQUARE_SIZE * 9;
    int height = SQUARE_SIZE * 9;

    d = XOpenDisplay(NULL);
    if (d == NULL) {
        cerr << "Cannot open display" << endl;
        exit(1);
    }
    s = DefaultScreen(d);
    w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, width, height, 1,
        BlackPixel(d, s), WhitePixel(d, s));
    XSelectInput(d, w, ExposureMask | KeyPressMask);
    XMapRaised(d, w);

    Pixmap pix
        = XCreatePixmap(d, w, width, height, DefaultDepth(d, DefaultScreen(d)));
    gc = XCreateGC(d, pix, 0, (XGCValues*)0);

    XFlush(d);
    XFlush(d);

    // Set up colours.
    XColor xcolour;
    Colormap cmap;
    char color_vals[7][10] = { "white", "black", "red", "#006400", "blue" };

    cmap = DefaultColormap(d, DefaultScreen(d));
    for (int i = 0; i < 5; ++i) {
        XParseColor(d, cmap, color_vals[i], &xcolour);
        XAllocColor(d, cmap, &xcolour);
        colours[i] = xcolour.pixel;
    }

    XSetForeground(d, gc, colours[Black]);

    // Make window non-resizeable.
    XSizeHints hints;
    hints.flags = (USPosition | PSize | PMinSize | PMaxSize);
    hints.height = hints.base_height = hints.min_height = hints.max_height
        = height;
    hints.width = hints.base_width = hints.min_width = hints.max_width = width;
    XSetNormalHints(d, w, &hints);

    XSynchronize(d, True);

    usleep(1000);
}

xwindow::~xwindow()
{
    XFreeGC(d, gc);
    XCloseDisplay(d);
}

void xwindow::fillGrid(int col, int row, int colour)
{
    col = (col + 1) * SQUARE_SIZE;
    row = (7 - row) * SQUARE_SIZE;

    XSetForeground(d, gc, colours[colour]);
    XFillRectangle(d, w, gc, col, row, SQUARE_SIZE, SQUARE_SIZE);
    XSetForeground(d, gc, colours[Black]);
}

void xwindow::drawPiece(int col, int row, char piece)
{
    if (col >= 0 && row >= 0 && rendered_board[col][row] == piece)
        return;
    rendered_board[col][row] = piece;

    int x = (col + 1) * SQUARE_SIZE + SQUARE_SIZE / 2 - STRING_OFFSET;
    int y = (7 - row) * SQUARE_SIZE + SQUARE_SIZE / 2 + STRING_OFFSET;

    const char* draw = &piece;

    Font font;

    if (col == -1 || row == -1)
        font = XLoadFont(d, "-*-helvetica-*-r-*-*-24-*-*-*-*-*-*-*");
    else if (islower(piece)) {
        font = XLoadFont(d, "-*-courier-*-r-*-*-24-*-*-*-*-*-*-*");
    } else {
        font = XLoadFont(d, "-*-lucida-*-r-*-*-24-*-*-*-*-*-*-*");
    }

    XSetFont(d, gc, font);
    XDrawString(d, w, gc, x, y, draw, 1);
    XUnloadFont(d, font);
}
