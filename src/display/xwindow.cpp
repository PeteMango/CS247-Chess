#include "display/xwindow.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdlib>
#include <iostream>
#include <string.h>
#include <string>
#include <unistd.h>
#include <unordered_map>

// Include XBM data for each piece
#include "display/piecebitmaps/black_bishop.XBM"
#include "display/piecebitmaps/black_king.XBM"
#include "display/piecebitmaps/black_knight.XBM"
#include "display/piecebitmaps/black_pawn.XBM"
#include "display/piecebitmaps/black_queen.XBM"
#include "display/piecebitmaps/black_rook.XBM"
#include "display/piecebitmaps/white_bishop.XBM"
#include "display/piecebitmaps/white_king.XBM"
#include "display/piecebitmaps/white_knight.XBM"
#include "display/piecebitmaps/white_pawn.XBM"
#include "display/piecebitmaps/white_queen.XBM"
#include "display/piecebitmaps/white_rook.XBM"

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

    gc = XCreateGC(d, w, 0, (XGCValues*)0);

    XFlush(d);
    XFlush(d);

    // Set up colours.
    XColor xcolour;
    Colormap cmap;
    char color_vals[5][10] = { "white", "black", "red", "#006400", "blue" };

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

    font = XLoadFont(
        d, "-misc-fixed-medium-r-normal--20-200-75-75-c-100-koi8-r");
    XSetFont(d, gc, font);

    usleep(1000);
}

xwindow::~xwindow()
{
    XUnloadFont(d, font);
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

    if (col != -1 && row != -1) {
        int color = (row + col) % 2 == 0 ? xwindow::Green : xwindow::White;
        if (rendered_board[col][row] != ']' && col >= 0 && row >= 0)
            fillGrid(col, row, color);

        rendered_board[col][row] = piece;
    }
    int x, y;

    if (row == -1 || col == -1) {
        x = (col + 1) * SQUARE_SIZE + SQUARE_SIZE / 2 - STRING_OFFSET;
        y = (7 - row) * SQUARE_SIZE + SQUARE_SIZE / 2 + STRING_OFFSET;
    } else {
        x = (col + 1) * SQUARE_SIZE + 10; // + SQUARE_SIZE / 2 - STRING_OFFSET;
        y = (7 - row) * SQUARE_SIZE + 10; // + SQUARE_SIZE / 2 + STRING_OFFSET;
    }

    const char* draw = &piece;

    if (col == -1 || row == -1) {
        XDrawString(d, w, gc, x, y, draw, 1);
        return;
    }

    struct xbm_data {
        unsigned const char* bits;
        unsigned int width;
        unsigned int height;
    };

    std::unordered_map<char, xbm_data> xbmPieces = {
        { 'K', { white_king_bits, white_king_width, white_king_height } },
        { 'Q', { white_queen_bits, white_queen_width, white_queen_height } },
        { 'R', { white_rook_bits, white_rook_width, white_rook_height } },
        { 'B', { white_bishop_bits, white_bishop_width, white_bishop_height } },
        { 'N', { white_knight_bits, white_knight_width, white_knight_height } },
        { 'P', { white_pawn_bits, white_pawn_width, white_pawn_height } },
        { 'k', { black_king_bits, black_king_width, black_king_height } },
        { 'q', { black_queen_bits, black_queen_width, black_queen_height } },
        { 'r', { black_rook_bits, black_rook_width, black_rook_height } },
        { 'b', { black_bishop_bits, black_bishop_width, black_bishop_height } },
        { 'n', { black_knight_bits, black_knight_width, black_knight_height } },
        { 'p', { black_pawn_bits, black_pawn_width, black_pawn_height } }
    };


    xbm_data it = xbmPieces[piece];

    Pixmap pixmap
        = XCreateBitmapFromData(d, w, (char*)it.bits, it.width, it.height);

    if (pixmap == None) {
        // std::cerr << "Failed to create pixmap for piece: " << piece
        //           << std::endl;

        x = (col + 1) * SQUARE_SIZE + SQUARE_SIZE / 2 - STRING_OFFSET;
        y = (7 - row) * SQUARE_SIZE + SQUARE_SIZE / 2 + STRING_OFFSET;
        const char* draw = &piece;
        XDrawString(d, w, gc, x, y, draw, 1);
        return;
    }
    XSetForeground(d, gc, colours[Black]);
    XSetBackground(d, gc, colours[White]);

    // std::cout << "Drawing piece " << piece << " at col: " << col
    //           << ", row: " << row << ", x: " << x << ", y: " << y <<
    //           std::endl;

    XCopyPlane(d, pixmap, w, gc, 0, 0, it.width, it.height, x, y, 1);
    XFreePixmap(d, pixmap);
}

//  valgrind --leak-check=yes ./out/chess
/*
game human human
move e2 e4
*/
