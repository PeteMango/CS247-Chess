#ifndef PIECE_H
#define PIECE_H

#include "../enum.h"
#include "../struct/coordinate.h"
#include "../util.h"
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

public:
    Piece(Color color, Coordinate location, PieceType type,
        std::shared_ptr<Board> board);
    Piece(const char& c);
    virtual ~Piece();
    virtual bool is_valid_move(Coordinate square) = 0;
    Color get_color();
    PieceType get_piece_type();
    Coordinate get_coordinate();
    virtual void get_attacking_squares(std::set<Coordinate>& s)
        = 0;
    void update_location(Coordinate square);
};

#endif
