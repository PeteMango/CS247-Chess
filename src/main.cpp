#include "../include/chess.h"
#include "../include/util.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

int main()
{
    std::shared_ptr<Chess> CHESS = createChess();
    std::string line;

    bool is_eof_given = false;

    while (true) {
        std::getline(std::cin, line);
        if (std::cin.eof() || is_eof_given) {
            std::cout << "CTRL-D detected" << std::endl;
            break;
        }

        std::stringstream ss(line);
        std::string cmd;
        ss >> cmd;
        if (cmd == "game") {
            if (!CHESS->can_start_game()) {
                throw std::invalid_argument("game run when game in progress");
            }
            std::string white;
            std::string black;
            ss >> white >> black;
            if (!validate_player(white) || !validate_player(black)) {
                throw std::invalid_argument("invalid player inputted");
            }

            CHESS->start_game(
                string_to_playertype(white), string_to_playertype(black));
        } else if (cmd == "resign") {
            throw std::logic_error("unimplemented");
            CHESS->resign();
        } else if (cmd == "move") {
            throw std::logic_error("unimplemented");
            if (CHESS->is_next_move_human()) {
                std::string start;
                std::string end;
                std::string promotion;
                ss >> start >> end >> promotion;
                if (!validate_coordinate(start) || !validate_coordinate(end)) {
                    throw std::invalid_argument("invalid coordinates");
                }
                if (!validate_possible_promotion(promotion)) {
                    throw std::invalid_argument("invalid promotion");
                }
                Coordinate start_coordinate(start);
                Coordinate end_coordinate(end);
                PromotionType promotion_type = string_to_promotiontype(promotion);
                CHESS->make_move(start_coordinate, end_coordinate, promotion_type);
            } else {
                Coordinate start_coordinate;
                Coordinate end_coordinate;
                PromotionType promotion_type = string_to_promotiontype("");
                CHESS->make_move(start_coordinate, end_coordinate, promotion_type);
            }
        } else if (cmd == "setup") {
            if (!CHESS->is_game_not_running()) {
                throw std::invalid_argument("setup run when game in progress");
            }
            CHESS->setup_board(std::cin, is_eof_given);
        } else {
            throw std::invalid_argument("invalid input command");
        }
    }

    return 0;
}

