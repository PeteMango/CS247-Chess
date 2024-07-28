#include "chess.hpp"
#include "json.hpp"
#include "parsefen.hpp"
#include "testutil.h"
#include <algorithm>
#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>

std::vector<std::vector<std::string>> moves;
std::vector<std::string> results;
std::vector<std::string> terminations;
std::vector<int> row_ids;

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
// ---------------------------- HELPER ----------------------------- //
// ----------------------------------------------------------------- //

int multigame_count = 0;
std::string getFileName(std::vector<int> ids, bool isIn)
{
    std::string extension = std::string(isIn ? "in" : "expect");

    std::string folder = "full-games-all-valid-moves-" + extension + "/";

    std::string filename, subfolder;

    if (ids.size() == 1) {
        subfolder = terminations[ids[0]];
        if (terminations[ids[0]] == "FIVEFOLD_REPETITION")
            subfolder = "resign";
        filename = subfolder + std::to_string(row_ids[ids[0]]) + "." + extension;
    } else {
        subfolder = "multigame";
        int id = multigame_count++ / 2;
        filename = subfolder + std::to_string(id) + "." + extension;
    }

    std::string full_filename = folder + subfolder + "/" + filename;

    std::transform(full_filename.begin(), full_filename.end(), full_filename.begin(),
        [](unsigned char c) { return std::tolower(c); });

    std::cout << "writing to " << full_filename << "\n";

    return full_filename;
}

// ----------------------------------------------------------------- //
// --------------------------- Making .in -------------------------- //
// ----------------------------------------------------------------- //

std::string generateIn(int i)
{
    std::string ret = "";
    std::vector<std::string>& move = moves[i];

    ret += "game human human\n";

    for (int j = 0; j < move.size(); j++) {
        ret += "move " + move[j].substr(0, 2) + " " + move[j].substr(2, 2);

        if (move[j].length() == 5)
            ret += " " + std::string(1, (char)std::toupper(move[j][4]));
        ret += '\n';
    }

    if (terminations[i] == "FIVEFOLD_REPETITION")
        ret += "resign\n";

    return ret;
}

// MAIN FUNCTION, ONLY THIS IS CALLED FROM OUTSIDE
void writeTestInput(std::vector<int> ids)
{
    std::string filename = getFileName(ids, true);

    std::ofstream infile;
    infile.open(filename);
    if (!infile) {
        std::cerr << "Error: Could not open the file " << filename << std::endl;
        return;
    }

    for (auto i : ids)
        infile << generateIn(i);

    infile.close();
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
    chess::movegen::legalmoves<chess::movegen::MoveGenType::ALL>(move_list, board);

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

void printBoard(std::string& ret, chess::Board& board)
{
    ChessFen chessfen;

    chessfen.parse_fen(board.getFen(false));

    int offset = -1;
    for (int i = 0; i < 64; i++) {
        if (i % 8 == 0)
            ret += std::string(1, char('8' - i / 8)) + " ", offset++;

        if (chessfen.board[i] == '.') {
            if ((i + offset) % 2 == 0)
                ret += ' ';
            else
                ret += '_';
        } else
            ret += chessfen.board[i];
        if (i > 0 && (i + 1) % 8 == 0)
            ret += '\n';
    }
    ret += "\n  ";

    for (int col = 0; col < 8; col++)
        ret += std::string(1, (char)(col + 'a'));

    ret += "\n";
}

int white_score, black_score;
std::string generateSingleGameExpect(int i)
{
    std::string ret = "";
    std::vector<std::string>& move = moves[i];

    chess::Board board = chess::Board(chess::constants::STARTPOS);
    std::string termination = terminations[i];
    std::string result = results[i];

    chess::Move m;
    chess::Movelist move_list;

    for (int j = 0; j < move.size(); j++) {
        findValidMove(board, move_list, m, move[j], j);
        board.makeMove(m);

        printBoard(ret, board);

        if (termination == "CHECKMATE" && j == move.size() - 1)
            continue;

        if (board.inCheck())
            if (j % 2 == 1)
                ret += "White is in check.\n";
            else
                ret += "Black is in check.\n";
    }

    if (termination == "CHECKMATE") {
        if (result[0] == '1')
            ret += "Checkmate! White wins!\n", white_score += 2;
        else
            ret += "Checkmate! Black wins!\n", black_score += 2;
        return ret;
    }

    if (terminations[i] == "FIVEFOLD_REPETITION") {
        printBoard(ret, board);
        if (board.inCheck())
            if ((move.size() - 1) % 2 == 1)
                ret += "White is in check.\n";
            else
                ret += "Black is in check.\n";

        if (move.size() % 2 != 0)
            ret += "White wins!\n", white_score += 2;
        else
            ret += "Black wins!\n", black_score += 2;
        return ret;
    }

    ret += "Stalemate!\n", white_score++, black_score++;

    return ret;
}

void writeTestExpected(std::vector<int> ids)
{
    std::string filename = getFileName(ids, false);

    std::ofstream outfile;
    outfile.open(filename);
    if (!outfile) {
        std::cerr << "Error: Could not open the file " << filename << std::endl;
        return;
    }

    white_score = black_score = 0;

    for (auto i : ids)
        outfile << generateSingleGameExpect(i);

    outfile << "Final Score:\n";

    outfile << "White: " << white_score / 2;
    if (white_score % 2 != 0)
        outfile << ".5";
    outfile << "\n";

    outfile << "Black: " << black_score / 2;
    if (black_score % 2 != 0)
        outfile << ".5";
    outfile << "\n";

    outfile.close();
}

// ----------------------------------------------------------------- //
// ------------------------------ Main ----------------------------- //
// ----------------------------------------------------------------- //

int main()
{
    std::vector<int> stalemates, checkmates, resign;

    getDataFromDatabase();
    for (int i = 0; i < moves.size(); i++) {
        if (terminations[i] != "STALEMATE" && terminations[i] != "CHECKMATE"
            && terminations[i] != "FIVEFOLD_REPETITION")
            continue;

        if (terminations[i] == "STALEMATE")
            stalemates.push_back(i);
        else if (terminations[i] == "CHECKMATE")
            checkmates.push_back(i);
        else if (terminations[i] == "FIVEFOLD_REPETITION")
            resign.push_back(i);

        writeTestInput({ i });
        writeTestExpected({ i });
    }

    int n = std::min(stalemates.size(), std::min(checkmates.size(), resign.size()));

    std::vector<int> tmp;
    std::random_device rd;
    std::mt19937 g(rd());
    for (int i = 0; i < n; i++) {
        tmp = { checkmates[i], stalemates[i], resign[i] };
        std::shuffle(tmp.begin(), tmp.end(), g);
        writeTestInput(tmp);
        writeTestExpected(tmp);
    }
}
