#include "chess.hpp"
#include "json.hpp"
#include "parsefen.hpp"
#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using json = nlohmann::json;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void getData(std::vector<std::map<std::string, std::string>>& dataset)
{
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL,
            "https://datasets-server.huggingface.co/"
            "rows?dataset=laion%2Fstrategic_game_chess&config=default&split="
            "train&offset=0&length=100");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK) {
            // Parse JSON response
            json jsonResponse = json::parse(readBuffer);

            // Define a vector of maps to store the dataset

            // Extract rows from the JSON response
            for (const auto& row : jsonResponse["rows"]) {
                std::map<std::string, std::string> rowData;
                for (auto it = row.begin(); it != row.end(); ++it) {
                    rowData[it.key()] = it.value().dump();
                }
                dataset.push_back(rowData);
            }

        } else
            std::cerr << "curl_easy_perform() failed: "
                      << curl_easy_strerror(res) << std::endl;
    }
}

void parseChessGame(const std::string& input,
    std::vector<std::vector<std::string>>& moves,
    std::vector<std::string>& results, std::vector<std::string>& terminations)
{
    json j = json::parse(input);

    moves.push_back({});

    for (const auto& move : j["Moves"]) {
        moves.back().push_back(move);
    }

    results.push_back(j["Result"]);
    terminations.push_back(j["Termination"]);
}

void get(std::vector<std::vector<std::string>>& moves,
    std::vector<std::string>& results, std::vector<std::string>& terminations)
{
    std::vector<std::map<std::string, std::string>> dataset;

    getData(dataset);

    for (int i = 0; i < dataset.size(); i++) {
        for (const auto& [key, value] : dataset[i]) {
            if (key == "row")
                parseChessGame(value, moves, results, terminations);
            // std::cout << key << ": " << value << "\n";
        }
        // std::cout << "----------\n";
    }
}

void printTest(int i, std::vector<std::vector<std::string>>& moves,
    std::vector<std::string>& results, std::vector<std::string>& terminations)
{
    std::string term = terminations[i];
    std::transform(term.begin(), term.end(), term.begin(),
        [](unsigned char c) { return std::tolower(c); });

    std::string filename = "full-games-all-valid-moves-in/" + term + "/" + term
        + std::to_string(i) + ".in";
    std::cout << "writing to " << filename << "\n";
    std::ofstream outfile;
    outfile.open(filename);
    if (!outfile) {
        std::cerr << "Error: Could not open the file " << filename << std::endl;
        return;
    }

    std::vector<std::string>& move = moves[i];
    std::string result = results[i];
    std::string termination = terminations[i];

    outfile << "game human human\n";

    for (int j = 0; j < move.size(); j++) {
        outfile << "move " << move[j].substr(0, 2) << " "
                << move[j].substr(2, 2);

        if (move[j].length() == 5) {
            // if (j % 2 == 0)
            outfile << " " << (char)toupper(move[j][4]);
            // else
            // outfile << " " << (char)tolower(move[j][4]);
        }
        outfile << "\n";
    }

    outfile.close();
}

using namespace chess;

std::unordered_map<std::string, chess::Square::underlying> posMap
    = { { "a1", chess::Square::underlying::SQ_A1 },
          { "b1", chess::Square::underlying::SQ_B1 },
          { "c1", chess::Square::underlying::SQ_C1 },
          { "d1", chess::Square::underlying::SQ_D1 },
          { "e1", chess::Square::underlying::SQ_E1 },
          { "f1", chess::Square::underlying::SQ_F1 },
          { "g1", chess::Square::underlying::SQ_G1 },
          { "h1", chess::Square::underlying::SQ_H1 },
          { "a2", chess::Square::underlying::SQ_A2 },
          { "b2", chess::Square::underlying::SQ_B2 },
          { "c2", chess::Square::underlying::SQ_C2 },
          { "d2", chess::Square::underlying::SQ_D2 },
          { "e2", chess::Square::underlying::SQ_E2 },
          { "f2", chess::Square::underlying::SQ_F2 },
          { "g2", chess::Square::underlying::SQ_G2 },
          { "h2", chess::Square::underlying::SQ_H2 },
          { "a3", chess::Square::underlying::SQ_A3 },
          { "b3", chess::Square::underlying::SQ_B3 },
          { "c3", chess::Square::underlying::SQ_C3 },
          { "d3", chess::Square::underlying::SQ_D3 },
          { "e3", chess::Square::underlying::SQ_E3 },
          { "f3", chess::Square::underlying::SQ_F3 },
          { "g3", chess::Square::underlying::SQ_G3 },
          { "h3", chess::Square::underlying::SQ_H3 },
          { "a4", chess::Square::underlying::SQ_A4 },
          { "b4", chess::Square::underlying::SQ_B4 },
          { "c4", chess::Square::underlying::SQ_C4 },
          { "d4", chess::Square::underlying::SQ_D4 },
          { "e4", chess::Square::underlying::SQ_E4 },
          { "f4", chess::Square::underlying::SQ_F4 },
          { "g4", chess::Square::underlying::SQ_G4 },
          { "h4", chess::Square::underlying::SQ_H4 },
          { "a5", chess::Square::underlying::SQ_A5 },
          { "b5", chess::Square::underlying::SQ_B5 },
          { "c5", chess::Square::underlying::SQ_C5 },
          { "d5", chess::Square::underlying::SQ_D5 },
          { "e5", chess::Square::underlying::SQ_E5 },
          { "f5", chess::Square::underlying::SQ_F5 },
          { "g5", chess::Square::underlying::SQ_G5 },
          { "h5", chess::Square::underlying::SQ_H5 },
          { "a6", chess::Square::underlying::SQ_A6 },
          { "b6", chess::Square::underlying::SQ_B6 },
          { "c6", chess::Square::underlying::SQ_C6 },
          { "d6", chess::Square::underlying::SQ_D6 },
          { "e6", chess::Square::underlying::SQ_E6 },
          { "f6", chess::Square::underlying::SQ_F6 },
          { "g6", chess::Square::underlying::SQ_G6 },
          { "h6", chess::Square::underlying::SQ_H6 },
          { "a7", chess::Square::underlying::SQ_A7 },
          { "b7", chess::Square::underlying::SQ_B7 },
          { "c7", chess::Square::underlying::SQ_C7 },
          { "d7", chess::Square::underlying::SQ_D7 },
          { "e7", chess::Square::underlying::SQ_E7 },
          { "f7", chess::Square::underlying::SQ_F7 },
          { "g7", chess::Square::underlying::SQ_G7 },
          { "h7", chess::Square::underlying::SQ_H7 },
          { "a8", chess::Square::underlying::SQ_A8 },
          { "b8", chess::Square::underlying::SQ_B8 },
          { "c8", chess::Square::underlying::SQ_C8 },
          { "d8", chess::Square::underlying::SQ_D8 },
          { "e8", chess::Square::underlying::SQ_E8 },
          { "f8", chess::Square::underlying::SQ_F8 },
          { "g8", chess::Square::underlying::SQ_G8 },
          { "h8", chess::Square::underlying::SQ_H8 } };

std::unordered_map<char, chess::Piece::underlying> pieceMap
    = { { 'P', chess::Piece::underlying::WHITEPAWN },
          { 'N', chess::Piece::underlying::WHITEKNIGHT },
          { 'B', chess::Piece::underlying::WHITEBISHOP },
          { 'R', chess::Piece::underlying::WHITEROOK },
          { 'Q', chess::Piece::underlying::WHITEQUEEN },
          { 'K', chess::Piece::underlying::WHITEKING },
          { 'p', chess::Piece::underlying::BLACKPAWN },
          { 'n', chess::Piece::underlying::BLACKKNIGHT },
          { 'b', chess::Piece::underlying::BLACKBISHOP },
          { 'r', chess::Piece::underlying::BLACKROOK },
          { 'q', chess::Piece::underlying::BLACKQUEEN },
          { 'k', chess::Piece::underlying::BLACKKING } };

std::unordered_map<char, chess::PieceType::underlying> simplePieceMap
    = { { 'p', chess::PieceType::underlying::PAWN },
          { 'n', chess::PieceType::underlying::KNIGHT },
          { 'b', chess::PieceType::underlying::BISHOP },
          { 'r', chess::PieceType::underlying::ROOK },
          { 'q', chess::PieceType::underlying::QUEEN },
          { 'k', chess::PieceType::underlying::KING } };

void asd(Movelist& move_list, Move& m, std::string& a, std::string& b)
{
    m = Move::make<Move::NORMAL>(posMap[a], posMap[b]);
    if (move_list.find(m) != -1)
        return;

    m = Move::make<Move::ENPASSANT>(posMap[a], posMap[b]);
    if (move_list.find(m) != -1)
        return;

    if (b[0] > a[0])
        b[0] = (char)((int)b[0] + 1);
    else
        b[0] = (char)((int)b[0] - 2);

    m = Move::make<Move::CASTLING>(posMap[a], posMap[b]);

    if (move_list.find(m) == -1) {
        std::cout << "SOMETHING WENT WRONG";
        return;
    }
}

void printExpect(int i, std::vector<std::vector<std::string>>& moves,
    std::vector<std::string>& results, std::vector<std::string>& terminations)
{
    std::string term = terminations[i];
    std::transform(term.begin(), term.end(), term.begin(),
        [](unsigned char c) { return std::tolower(c); });

    std::string filename = "full-games-all-valid-moves-expect/" + term + "/"
        + term + std::to_string(i) + ".expect";

    std::cout << "writing to " << filename << "\n";
    std::ofstream outfile;
    outfile.open(filename);
    if (!outfile) {
        std::cerr << "Error: Could not open the file " << filename << std::endl;
        return;
    }

    std::vector<std::string>& move = moves[i];
    std::string result = results[i];
    std::string termination = terminations[i];

    Board board = Board(chess::constants::STARTPOS);

    Move m;
    std::string a, b;
    char promo;
    Movelist move_list;
    ChessFen chessfen;

    for (int j = 0; j < move.size(); j++) {
        a = move[j].substr(0, 2);
        b = move[j].substr(2, 2);

        if (move[j].length() == 5) {
            if (j % 2 == 0)
                promo = toupper(move[j][4]);
            else
                promo = tolower(move[j][4]);
            m = Move::make<Move::PROMOTION>(Square { posMap[a] },
                Square { posMap[b] }, simplePieceMap[tolower(promo)]);
        } else {
            movegen::legalmoves<movegen::MoveGenType::ALL>(move_list, board);
            asd(move_list, m, a, b);
        }
        board.makeMove(m);

        chessfen.parse_fen(board.getFen(false));

        for (int i = 0; i < 64; i++) {
            if (i % 8 == 0) {
                outfile << char('8' - i / 8) << " ";
            }
            outfile << chessfen.board[i];
            if (i > 0 && (i + 1) % 8 == 0)
                outfile << "\n";
        }
        outfile << "\n";
        outfile << "  ";

        for (int col = 0; col < 8; col++) {
            outfile << (char)(col + 'a');
        }
        outfile << "\n";

        if (board.inCheck()) {
            if (j % 2 == 1)
                outfile << "Black is in check.\n";
            else
                outfile << "White is in check.\n";
        }

        // outfile << board.getFen(false) << "\n";
    }
    if (termination == "CHECKMATE") {
        if (result[0] == '1')
            outfile << "Checkmate! White wins!\n";
        else
            outfile << "Checkmate! Black wins!\n";
    } else {
        outfile << "Stalemate!\n";
    }
    outfile << "CTRL-D detected\n";
    outfile.close();
}

int main()
{
    std::vector<std::vector<std::string>> moves;
    std::vector<std::string> results;
    std::vector<std::string> terminations;

    int n = moves.size();

    get(moves, results, terminations);

    // for (auto i : terminations) {
    //     std::cout << i << "\n";
    // }

    std::map<std::string, int> ddd;

    for (int i = 0; i < 100; i++) {
        if (ddd[terminations[i]] > 3)
            continue;
        // if (terminations[i] != "STALEMATE")
        //     continue;
        ddd[terminations[i]]++;
        printTest(i, moves, results, terminations);
        printExpect(i, moves, results, terminations);
        // std::cout << terminations[i] << " ASD\n";
    }

    // Board board = Board(chess::constants::STARTPOS);
    // board.makeMove(Move::make<Move::CASTLING>(posMap["e1"], posMap["e3"]));
    // std::cout << "ASD";
}
