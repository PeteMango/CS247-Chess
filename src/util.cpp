#include "../include/util.h"

const std::unordered_map<PromotionType, std::string>
    PromotionTypeToString = {
        { PromotionType::KNIGHT, "N" },
        { PromotionType::BISHOP, "B" },
        { PromotionType::ROOK, "R" },
        { PromotionType::QUEEN, "Q" },
    };

const std::unordered_map<std::string, PromotionType>
    StringToPromotionType {
        { "N", PromotionType::KNIGHT },
        { "B", PromotionType::BISHOP },
        { "R", PromotionType::ROOK },
        { "Q", PromotionType::QUEEN },
    };

const std::unordered_map<PieceType, std::string>
    PieceTypeToString = {
        { PieceType::PAWN, "P" },
        { PieceType::KNIGHT, "N" },
        { PieceType::BISHOP, "B" },
        { PieceType::ROOK, "R" },
        { PieceType::QUEEN, "Q" },
        { PieceType::KING, "K" },
    };

const std::unordered_map<std::string, PieceType>
    StringToPieceType = {
        { "P", PieceType::PAWN },
        { "N", PieceType::KNIGHT },
        { "B", PieceType::BISHOP },
        { "R", PieceType::ROOK },
        { "Q", PieceType::QUEEN },
        { "K", PieceType::KING },
    };

const std::unordered_map<Color, std::string> ColorToString
    = { { Color::WHITE, "white" }, { Color::BLACK, "black" } };

const std::unordered_map<std::string, Color> StringToColor {
    { "white", Color::WHITE }, { "black", Color::BLACK }
};

bool validate_player(std::string player) { }

bool validate_coordinate(std::string coordinate)
{
    if (coordinate.size() != 2) {
        return false;
    }
    char column = coordinate[0];
    char row = coordinate[1];
    return 'a' <= column && column <= 'h' && '1' <= row
        && row <= '8';
}

bool validate_possible_promotion(std::string promotion)
{
    return StringToPromotionType.find(promotion)
        != StringToPromotionType.end();
}

bool validate_piece(std::string piece)
{
    return StringToPieceType.find(piece)
        != StringToPieceType.end();
}

bool validate_color(std::string color)
{
    return StringToColor.find(color) != StringToColor.end();
}

bool validate_setup() { }

PromotionType string_to_promotiontype(std::string promotion)
{
    return StringToPromotionType.at(promotion);
}

PieceType string_to_piecetype(std::string piece)
{
    return StringToPieceType.at(piece);
}

Color string_to_color(std::string color)
{
    return StringToColor.at(color);
}

std::pair<int, int> get_grid_indexes(Coordinate c)
{
    return std::make_pair<int, int>(c.row - 1, c.column - 'a');
}

std::pair<int, int> add_pairs(
    std::pair<int, int> a, std::pair<int, int> b)
{
    a.first += b.first;
    a.second += b.second;
    return a;
}

bool coordinate_in_bounds(std::pair<int, int> a)
{
    return 0 <= a.first && a.first < 8 && 0 <= a.second
        && a.second < 8;
}

