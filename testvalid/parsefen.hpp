#include <cctype>
#include <string>

#define NB_SQ 64 // number of squares
#define NB_CASTLE 2 // number of castle types
#define NB_COLOR 2 // number of colors

enum CastleFen { king_side, queen_side };

enum ColorFen { white, black };

class ChessFen {
private:
    ColorFen turn;
    std::string en_passant;
    bool castle_rights[NB_COLOR][NB_CASTLE];

public:
    char board[NB_SQ];
    ChessFen();
    void parse_fen(const std::string&);
};

ChessFen::ChessFen()
{
    for (int i = 0; i < NB_SQ; i++)
        board[i] = '.'; // set all squares to be empty '.'

    for (int i = 0; i < NB_COLOR; i++)
        for (int j = 0; j < NB_CASTLE; j++)
            castle_rights[i][j]
                = false; // all castle_rights are false by default
}

void ChessFen::parse_fen(const std::string& fen)
{
    for (int i = 0; i < NB_SQ; i++)
        board[i] = '.'; // set all squares to be empty '.'

    for (int i = 0; i < NB_COLOR; i++)
        for (int j = 0; j < NB_CASTLE; j++)
            castle_rights[i][j]
                = false; // all castle_rights are false by default
                
    const size_t size = fen.size();
    size_t iter = 0;
    int index = 0;

    // parse the board first
    for (; (iter < size) and (fen[iter] != ' '); iter++) {
        if (fen[iter] == '/')
            continue;

        if (isdigit(fen[iter]))
            index += (fen[iter] - '0'); // converts char digit to int. `5` to 5

        else {
            board[index] = fen[iter];
            ++index;
        }
    }

    turn = fen[iter + 1] == 'w' ? ColorFen::white : ColorFen::black;

    for (iter += 3; (iter < size) and (fen[iter] != ' '); iter++) {

        if (fen[iter] == 'k')
            castle_rights[ColorFen::black][CastleFen::king_side] = true;

        else if (fen[iter] == 'K')
            castle_rights[ColorFen::white][CastleFen::king_side] = true;

        else if (fen[iter] == 'q')
            castle_rights[ColorFen::black][CastleFen::queen_side] = true;

        else if (fen[iter] == 'Q')
            castle_rights[ColorFen::white][CastleFen::queen_side] = true;
    }

    en_passant = fen.substr(iter + 1, 3);
}
