#include "../lib/thc/thc.h"
#include <iostream>

using namespace std;

double evaluate(thc::ChessRules board)
{
    double evaluation;

    vector<thc::Move> legalMoves;

    board.GenLegalMoveList(legalMoves);
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            switch (board.squares[i * 8 + j])
            {
            case 'P':
            {
                evaluation += 1;
                break;
            }
            case 'p':
            {
                evaluation -= 1;
                break;
            }
            case 'N':
            {
                evaluation += 3;
                break;
            }
            case 'n':
            {
                evaluation -= 3;
                break;
            }
            case 'B':
            {
                evaluation += 3.5;
                break;
            }
            case 'b':
            {
                evaluation -= 3.5;
                break;
            }
            case 'R':
            {
                evaluation += 5;
                break;
            }
            case 'r':
            {
                evaluation -= 5;
                break;
            }
            case 'Q':
            {
                evaluation += 9;
                break;
            }
            case 'q':
            {
                evaluation -= 9;
                break;
            }
        }
    }
}

// prefer big center (does not really work either)
    if(board.squares[28]=='P' && board.squares[29]=='P')
    {
        evaluation +=0.5;
    }

    if(board.squares[36]=='p' && board.squares[37]=='p')
    {
        evaluation -= 0.5;
    }

// try to castle does not work somereason test fen (r1bqk1nr/pppp1ppp/2n5/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4)
    if(board.wking_square == board.squares[7] || board.wking_square == board.squares[3] )
    {
        evaluation += 1;
    }
    if(board.bking_square == board.squares[59] || board.bking_square == board.squares[63])
    {
        evaluation -= 1;
    }
//checkmate
    if(board.WhiteToPlay() && board.AttackedSquare(board.wking_square, false) && legalMoves.size()==0)
    {
        evaluation -= 10000;
    }
 
    if(!board.WhiteToPlay() && board.AttackedSquare(board.bking_square, true) && legalMoves.size()==0)
    {
        evaluation += 10000;
    }

//stalemate
    if(legalMoves.size()==0 && !(board.AttackedSquare(board.wking_square, false) || board.AttackedSquare(board.bking_square, true)) )
    {
        evaluation = 0;
    }

    return evaluation;
}