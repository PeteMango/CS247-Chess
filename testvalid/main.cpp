#include "chess.hpp"
#include "json.hpp"
#include "parsefen.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

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

void asd(chess::Movelist& move_list, chess::Move& m, std::string& a, std::string& b)
{
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

void printASDDSA()
{
    chess::Board board = chess::Board(chess::constants::STARTPOS);

    chess::Move m;
    std::string a, b;
    char promo;
    chess::Movelist move_list;

    for (int j = 0; j < move.size(); j++) {
        a = move[j].substr(0, 2);
        b = move[j].substr(2, 2);

        if (move[j].length() == 5) {
            if (j % 2 == 0)
                promo = toupper(move[j][4]);
            else
                promo = tolower(move[j][4]);
            m = chess::Move::make<chess::Move::PROMOTION>(
                chess::Square { posMap[a] }, chess::Square { posMap[b] },
                simplePieceMap[tolower(promo)]);
        } else {
            chess::movegen::legalmoves<chess::movegen::MoveGenType::ALL>(
                move_list, board);
            asd(move_list, m, a, b);
        }
        board.makeMove(m);


        if (board.inCheck()) {
            
        }

        // outfile << board.getFen(false) << "\n";
    }
   
}

int main() { }
