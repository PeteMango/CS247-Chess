#ifndef UTIL_H
#define UTIL_H

#include "enum.h"
#include "struct/coordinate.h"
#include <string>
#include <unordered_map>

extern const std::unordered_map<PromotionType, std::string> PromotionTypeToString;

extern const std::unordered_map<std::string, PromotionType> StringToPromotionType;

extern const std::unordered_map<PieceType, std::string> PieceTypeToString;

extern const std::unordered_map<std::string, PieceType> StringToPieceType;

extern const std::unordered_map<Color, std::string> ColorToString;

extern const std::unordered_map<std::string, Color> StringToColor;

extern const std::unordered_map<Color, std::string> ColorToPrintString;

extern const std::unordered_map<std::string, PlayerType> StringToPlayer;

extern const std::unordered_map<Color, std::unordered_map<PieceType, char>> pieces;

bool validate_player(std::string player);

bool validate_coordinate(std::string coordinate);

bool validate_possible_promotion(std::string promotion);

bool validate_piece(std::string piece);

bool validate_color(std::string color);

bool validate_setup();

PromotionType string_to_promotiontype(std::string promotion);

PieceType string_to_piecetype(std::string piece);

Color string_to_color(std::string color);

PlayerType string_to_playertype(std::string player);

std::pair<int, int> get_grid_indexes(Coordinate c);

std::pair<int, int> get_grid_indexes(std::pair<int, int> a);

std::pair<char, int> get_print_coordinate(std::pair<int, int> c);

std::pair<int, int> add_pairs(std::pair<int, int> a, std::pair<int, int> b);

bool coordinate_in_bounds(std::pair<int, int> a);

PieceType PromotionTypeToPieceType(PromotionType pt);

Color toggle_color(Color c);

void debug(std::string s);

#endif
