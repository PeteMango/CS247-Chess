// ---------------------------- Testing --------------------------- //
#include "chessGOOD.hpp"
#include "json.hpp"
#include "parsefen.hpp"
#include "testutil.h"
// ---------------------------- Project --------------------------- //
#include "board.h"
#include "chess.h"
#include "enum.h"
#include "game.h"
#include "globals.h"
#include "struct/coordinate.h"
#include "struct/move.h"
#include "util.h"

// ----------------------------- Other ---------------------------- //
#include <algorithm>
#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <string>
#include <vector>

// ---------------------------- Globals --------------------------- //
std::vector<std::vector<std::string>> moves;
std::vector<std::string> results;
std::vector<std::string> terminations;
std::vector<int> row_ids;
std::shared_ptr<Chess> NormChess;
std::shared_ptr<Game> NormGame;
std::shared_ptr<Board> NormBoard;

// ----------------------------------------------------------------- //
// ---------------------------- DATABASE --------------------------- //
// ----------------------------------------------------------------- //

using json = nlohmann::json;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void fetchURLchunk(
    std::vector<std::map<std::string, std::string>>& dataset, std::string& offset)
{
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        std::string a = "https://datasets-server.huggingface.co/"
                        "rows?dataset=laion%2Fstrategic_game_chess&config="
                        "default&split=train&offset="
            + offset + "00&length=100";

        curl_easy_setopt(curl, CURLOPT_URL, a.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK) {
            json jsonResponse = json::parse(readBuffer);

            for (const auto& row : jsonResponse["rows"]) {
                std::map<std::string, std::string> rowData;
                for (auto it = row.begin(); it != row.end(); ++it) {
                    rowData[it.key()] = it.value().dump();
                }
                dataset.push_back(rowData);
            }

        } else
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
                      << std::endl;
    }
}

void parseJSONChessGame(const std::string& input)
{
    json j = json::parse(input);

    moves.push_back({});

    for (const auto& move : j["Moves"]) {
        moves.back().push_back(move);
    }

    results.push_back(j["Result"]);
    terminations.push_back(j["Termination"]);
}

// MAIN BOY, everything else is helper
void getDataFromDatabase()
{
    std::vector<std::map<std::string, std::string>> dataset;

    std::string offset_url = "0";

    for (int i = 0; i < 10; i++)
        fetchURLchunk(dataset, offset_url), offset_url[0]++;

    for (int i = 0; i < dataset.size(); i++)
        for (const auto& [key, value] : dataset[i]) {
            if (key == "row")
                parseJSONChessGame(value);
            if (key == "row_idx")
                row_ids.push_back(stoi(value));
        }
}

// ----------------------------------------------------------------- //
// ------------------------- Making .expect ------------------------ //
// ----------------------------------------------------------------- //

void findValidMove(chess::Board& board, chess::Movelist& move_list, chess::Move& m,
    std::string& move, int j)
{
    std::string a, b;
    char promo;

    a = move.substr(0, 2);
    b = move.substr(2, 2);

    if (move.length() == 5) {
        if (j % 2 == 0)
            promo = toupper(move[4]);
        else
            promo = tolower(move[4]);
        m = chess::Move::make<chess::Move::PROMOTION>(chess::Square { posMap[a] },
            chess::Square { posMap[b] }, simplePieceMap[tolower(promo)]);
        return;
    }

    m = chess::Move::make<chess::Move::NORMAL>(posMap[a], posMap[b]);
    if (move_list.find(m) != -1)
        return;

    m = chess::Move::make<chess::Move::ENPASSANT>(posMap[a], posMap[b]);
    if (move_list.find(m) != -1)
        return;

    if (b[0] > a[0])
        b[0] = (char)((int)b[0] + 1);
    else
        b[0] = (char)((int)b[0] - 2);

    m = chess::Move::make<chess::Move::CASTLING>(posMap[a], posMap[b]);

    if (move_list.find(m) == -1) {
        std::cout << "SOMETHING WENT WRONG";
        return;
    }
}

void convertFromNormMove(
    std::pair<Coordinate, Coordinate>& s, std::string& a, std::string& b)
{
    a = s.first.column + std::to_string(s.first.row);
    b = s.second.column + std::to_string(s.first.row);
}

bool findValidNormMove(
    chess::Movelist& move_list, chess::Move& m, std::string a, std::string b)
{
    m = chess::Move::make<chess::Move::NORMAL>(posMap[a], posMap[b]);
    if (move_list.find(m) != -1)
        return true;

    m = chess::Move::make<chess::Move::ENPASSANT>(posMap[a], posMap[b]);
    if (move_list.find(m) != -1)
        return true;

    if (b[0] > a[0])
        b[0] = (char)((int)b[0] + 1);
    else
        b[0] = (char)((int)b[0] - 2);

    m = chess::Move::make<chess::Move::CASTLING>(posMap[a], posMap[b]);
    if (move_list.find(m) != -1)
        return true;

    return false;
}

void generateSingleGameExpect(int i)
{
    std::vector<std::string>& move = moves[i];

    chess::Board boardGOOD = chess::Board(chess::constants::STARTPOS);
    std::string termination = terminations[i];
    std::string result = results[i];

    chess::Move m, normMoveGOOD;
    chess::Movelist move_list;

    std::string a, b, normA, normB;
    char promo;

    std::set<std::pair<Coordinate, Coordinate>> s;

    std::string filename = "ERROR.out";

    std::ofstream outfile;
    outfile.open(filename);
    if (!outfile) {
        std::cerr << "Error: Could not open the file " << filename << std::endl;
        return;
    }

    for (int j = 0; j < move.size(); j++) {
        chess::movegen::legalmoves<chess::movegen::MoveGenType::ALL>(
            move_list, boardGOOD);

        a = move[j].substr(0, 2);
        b = move[j].substr(2, 2);

        promo = 'Q';
        if (move[j].length() == 5)
            promo = toupper(move[j][4]);

        findValidMove(boardGOOD, move_list, m, move[j], j);

        NormBoard->get_all_valid_moves(
            s, (j % 2 == 0 ? Color::WHITE : Color::BLACK));

        if (move[j].length() != 5) {
            for (auto NormMove : s) {
                convertFromNormMove(NormMove, normA, normB);
                bool good = findValidNormMove(move_list, normMoveGOOD, normA, normB);
                if (!good) {
                    if (!NormBoard->is_valid_move(normA, normB).valid)
                        continue;

                    outfile << "\n" << i << " " << j << "\n";
                    outfile << normA << " " << normB << "\n";
                }
            }

            for (auto GOODmove : move_list) {
                chess::Square fromGOOD = GOODmove.from();
                chess::Square toGOOD = GOODmove.to();
                std::string GOODa = reversedPosMap[fromGOOD.sq];
                std::string GOODb = reversedPosMap[toGOOD.sq];

                if (GOODmove.typeOf() == chess::Move::CASTLING)
                    if (GOODb[0] > GOODa[0])
                        GOODb[0] = (char)((int)GOODb[0] - 1);
                    else
                        GOODb[0] = (char)((int)GOODb[0] + 2);

                if (!NormBoard->is_valid_move(GOODa, GOODb).valid) {
                    outfile << "\n" << i << " " << j << "\n";
                    outfile << GOODa << " " << GOODb << "\n";
                }
            }
        }

        NormChess->make_move(a, b, NormPieceMap[promo]);
        boardGOOD.makeMove(m);
    }
    outfile.close();
}

void writeTestExpected(std::vector<int> ids)
{
    for (auto i : ids)
        generateSingleGameExpect(i);
}

// ----------------------------------------------------------------- //
// ------------------------------ Main ----------------------------- //
// ----------------------------------------------------------------- //

int main()
{
    std::vector<int> stalemates, checkmates, resign;
    getDataFromDatabase();

    for (int i = 0; i < moves.size(); i++) {
        // if (terminations[i] != "STALEMATE" && terminations[i] != "CHECKMATE"
        //     && terminations[i] != "FIVEFOLD_REPETITION")
        //     continue;

        // if (terminations[i] == "STALEMATE")
        //     stalemates.push_back(i);
        // else if (terminations[i] == "CHECKMATE")
        //     checkmates.push_back(i);
        // else if (terminations[i] == "FIVEFOLD_REPETITION")
        //     resign.push_back(i);
        NormChess = createChess(false);
        NormChess->start_game(PlayerType::HUMAN, PlayerType::HUMAN);
        NormGame = NormChess->get_last_game();
        NormBoard = NormGame->get_board();
        writeTestExpected({ i });
    }

    // NormChess = createChess(false);
    // NormChess->start_game(PlayerType::HUMAN, PlayerType::HUMAN);
    // NormGame = NormChess->get_last_game();
    // NormBoard = NormGame->get_board();

    // writeTestExpected({ checkmates[0] });
}

