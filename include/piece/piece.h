#ifndef PIECE_H
#define PIECE_H

#include "../enum.h"
#include "../struct/coordinate.h"
#include "../util.h"
#include <iostream>
#include <memory>
#include <set>
#include <vector>

class Board;
class Piece {
protected:
    Color color;
    Coordinate location;
    PieceType type;
    std::shared_ptr<Board> board;
    std::vector<std::pair<int, int>> directions;

public:
    /* constructor and destructor */
    /* set the direction in the constructor */
    Piece(Color color, Coordinate location, PieceType type,
        std::shared_ptr<Board> board);
    // Piece(const char& c); /*TODO */
    virtual ~Piece() = 0;

    /* get the valid moves that the user can make */
    virtual void get_valid_moves(std::set<Coordinate>& s);

    /* get all the coordinate that the square is protecting */
    virtual void get_threatened_squares(std::set<Coordinate>& s) = 0;

    /* move helper, not abstract */
    void single_move(
        std::vector<std::pair<int, int>> directions, std::set<Coordinate>& s);
    void multiple_moves(
        std::vector<std::pair<int, int>> directions, std::set<Coordinate>& s);
    void filter_moves(std::set<Coordinate>& s);

    /* getter/setter */
    Color get_color();
    PieceType get_piece_type();
    Coordinate get_coordinate();

    /* debugging */
    void print_attack();
    void print_move();
};

#endif
