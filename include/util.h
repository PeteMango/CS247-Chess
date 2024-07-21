#ifndef UTIL_H
#define UTIL_H

#include "enum.h"
#include <string>

bool validate_player(std::string player);

bool validate_coordinate(std::string coordinate);

bool validate_possible_promotion(std::string promotion);

PromotionType string_to_promotiontype(std::string promotion);

#endif
