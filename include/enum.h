#ifndef ENUM_H
#define ENUM_H

enum class Color { WHITE, BLACK };

enum class PieceType { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };

enum class PromotionType { KNIGHT, BISHOP, ROOK, QUEEN };

enum class CastleSide { KINGSIDE, QUEENSIDE };

enum class PlayerType { HUMAN, LEVEL1, LEVEL2, LEVEL3, LEVEL4 };

enum class Result { WHITE_WIN, BLACK_WIN, DRAW };

enum class DisplayStatus { CHECKMATE, STALEMATE, CHECK, NONE };

#endif
