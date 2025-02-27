#include "../include/chess.h"
#include "../include/util.h"
#include "game.h"
#include "globals.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

int main(int argc, char* argv[])
{
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--no-graphics" || arg == "-ng") {
            GRAPHICS = false;
        } else if (arg == "--no-strict" || arg == "-ns") {
            STRICT = false;
        }
    }
    if (GRAPHICS) {
        std::cout << "graphics mode enabled" << std::endl;
    } else {
        std::cout << "graphics mode disabled" << std::endl;
    }
    if (STRICT) {
        std::cout << "strict mode enabled" << std::endl;
    } else {
        std::cout << "strict mode disabled" << std::endl;
    }

    std::shared_ptr<Chess> CHESS = createChess(GRAPHICS);
    std::string line;

    bool is_eof_given = false;

    while (true) {
        try {
            std::getline(std::cin, line);
            if (std::cin.eof() || is_eof_given) {
                CHESS->get_scores();
                break;
            }

            std::stringstream ss(line);
            std::string cmd;
            ss >> cmd;
            if (cmd == "game") {
                if (!CHESS->can_start_game()) {
                    throw std::invalid_argument(
                        "game command run when game in progress");
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
                if (!CHESS->can_make_move()) {
                    throw std::invalid_argument("cannot call resign");
                }
                CHESS->resign();
            } else if (cmd == "move") {
                if (!CHESS->can_make_move()) {
                    throw std::invalid_argument("cannot call move");
                }

                std::string start;
                std::string end;
                std::string promotion;
                ss >> start >> end >> promotion;

                std::shared_ptr<Game> g = CHESS->get_last_game();
                std::shared_ptr<Board> b = g->get_board();
                std::shared_ptr<Players> p = CHESS->get_last_game_players();

                if (CHESS->is_next_move_human()) {
                    if (!validate_coordinate(start) || !validate_coordinate(end)) {
                        throw std::invalid_argument("invalid coordinates");
                    }
                    Coordinate start_coordinate(start);
                    Coordinate end_coordinate(end);

                    if (!b->is_valid_move(start_coordinate, end_coordinate,
                              b->get_active_color())
                             .valid) {
                        throw std::invalid_argument("invalid move");
                    }
                    if (b->is_promotion(start_coordinate, end_coordinate)
                        && promotion.empty()) {
                        throw std::invalid_argument(
                            "promotion without specifying promotion "
                            "piece");
                    } else if (!b->is_promotion(start_coordinate, end_coordinate)
                        && !promotion.empty()) {
                        throw std::invalid_argument(
                            "non promotion specifying promotion "
                            "piece");
                    }

                    if (promotion.empty()) {
                        promotion = "Q";
                    }
                    if (!validate_possible_promotion(promotion)) {
                        throw std::invalid_argument("invalid promotion");
                    }
                    PromotionType promotion_type
                        = string_to_promotiontype(promotion);
                    if (b->get_active_color() == Color::WHITE) {
                        p->white->make_move(
                            start_coordinate, end_coordinate, promotion_type);
                    } else {
                        p->black->make_move(
                            start_coordinate, end_coordinate, promotion_type);
                    }
                } else if (start.empty() && end.empty() && promotion.empty()) {
                    if (b->get_active_color() == Color::WHITE) {
                        p->white->move();
                    } else {
                        p->black->move();
                    }

                } else {
                    throw std::invalid_argument(
                        "no other arguments other than move for a "
                        "computer move");
                }
            } else if (cmd == "setup") {
                if (!CHESS->can_setup_board()) {
                    throw std::invalid_argument("setup run when game in progress");
                }
                CHESS->setup_board(std::cin, is_eof_given);
            } else {
                throw std::invalid_argument("invalid input command");
            }
        } catch (const std::invalid_argument& e) {
            if (STRICT) {
                throw;
            } else {
                std::cerr << "Invalid Argument, please try again: " << e.what()
                          << std::endl;
                continue;
            }
        } catch (const std::runtime_error& e) {
            std::cerr << "Runtime Error: " << e.what() << std::endl;
            throw;
        } catch (...) {
            std::cerr << "An unexpected error occurred." << std::endl;
            throw;
        }
    }

    return 0;
}
