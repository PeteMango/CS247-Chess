#ifndef UTIL_H
#define UTIL_H

#include "enum.h"
#include "struct/coordinate.h"
#include <string>
#include <unordered_map>

extern const std::unordered_map<PromotionType, std::string>
    PromotionTypeToString;

extern const std::unordered_map<std::string, PromotionType>
    StringToPromotionType;

extern const std::unordered_map<PieceType, std::string>
    PieceTypeToString;

extern const std::unordered_map<std::string, PieceType>
    StringToPieceType;

extern const std::unordered_map<Color, std::string>
    ColorToString;

extern const std::unordered_map<std::string, Color>
    StringToColor;

bool validate_player(std::string player);

bool validate_coordinate(std::string coordinate);

bool validate_possible_promotion(std::string promotion);

bool validate_piece(std::string piece);

bool validate_color(std::string color);

bool validate_setup();

PromotionType string_to_promotiontype(std::string promotion);

PieceType string_to_piecetype(std::string piece);

Color string_to_color(std::string color);

std::pair<int, int> get_grid_indexes(Coordinate c);

#endif
