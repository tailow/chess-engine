#include "../lib/thc/thc.h"
#include <string>
#include <iostream>

using namespace std;

string search(thc::ChessRules board)
{
    thc::Move move;

    vector<thc::Move> legalMoves;

    board.GenLegalMoveList(legalMoves);

    /*
    for (int i = 0; i < legalMoves.size(); i++)
    {
        cout << legalMoves.at(i).NaturalOut(&board) << endl;
    }
    */

    move = legalMoves.at(rand() % legalMoves.size());

    string bestMove = "bestmove " + move.TerseOut() + "\n";

    return bestMove;
}