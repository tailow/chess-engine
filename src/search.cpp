#include "../lib/thc/thc.h"
#include <string>
#include <iostream>
#include "evaluate.h"

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
   

/*
       
    for (int i =0; i<legalMoves.size(); i++)
    {
        if (board.AttackedSquare(legalMoves[i].dst, true))
        {
            
        }
        else
        {
            goodmoves.push_back(legalMoves[i]);
        }
    }
    */

    move = at(rand() % legalMoves.size());

    string bestMove = "bestmove " + move.TerseOut() + "\n";

    return bestMove;
}