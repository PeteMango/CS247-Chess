#include "../include/chess.h"
#include "../include/util.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

int main()
{
    Chess CHESS;
    std::string line;

    while (true) {
        std::getline(std::cin, line);
        if (std::cin.eof()) {
            std::cout << "CTRL-D detected" << std::endl;
            break;
        }

        std::stringstream ss(line);
        std::string cmd;
        ss >> cmd;
        if (cmd == "game") {
            std::string white;
            std::string black;
            ss >> white >> black;
            if (!validate_player(white)
                || !validate_player(black)) {
                throw std::invalid_argument(
                    "invalid player inputted");
            }
            CHESS.start_game(white, black);
        } else if (cmd == "resign") {
            CHESS.resign();
        } else if (cmd == "move") {
            if (CHESS.is_next_move_human()) {
                std::string start;
                std::string end;
                std::string promotion;
                ss >> start >> end >> promotion;
                if (!validate_coordinate(start)
                    || !validate_coordinate(end)) {
                    throw std::invalid_argument(
                        "invalid coordinates");
                }
                if (!validate_possible_promotion(promotion)) {
                    throw std::invalid_argument(
                        "invalid promotion");
                }
                Coordinate start_coordinate(
                    start[1] - '0', start[0]);
                Coordinate end_coordinate(
                    end[1] - '0', start[0]);
                PromotionType promotion_type
                    = string_to_promotiontype(promotion);
                CHESS.make_move(start_coordinate, end_coordinate,
                    promotion_type);
            } else {
                Coordinate start_coordinate;
                Coordinate end_coordinate;
                PromotionType promotion_type
                    = string_to_promotiontype("");
                CHESS.make_move(start_coordinate, end_coordinate,
                    promotion_type);
            }
        } else if (cmd == "setup") {
            CHESS.setup_board(std::cin);
        } else {
            throw std::invalid_argument("invalid input command");
        }
    }

    return 0;
}

