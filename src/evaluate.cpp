#include "../lib/thc/thc.h"
#include <iostream>

using namespace std;

double evaluate(thc::ChessRules board)
{
    double evaluation = 0;
    double score;
    double bscore;

    /*
    for (square = a8; square <= h1; ++square)
    {

        char piece = squares[square];

        if ((white && IsWhite(piece)) || (!white && IsBlack(piece)))
        {
            switch (piece)
            {
            case 'P':
            {
                score = score + 1 break;
            }
            case 'p':
            {
                bscore = bscore + 1 break;
            }
            case 'N':
            {
                score = score + 3
            }
            case 'n':
            {
                bscore = bscore + 3
            }
            case 'B':
            {
                score = score + 3.5
            }
            case 'b':
            {
                bscore = bscore + 3.5 break;
            }
            case 'R':
            {
                score = score + 5
            }
            case 'r':
            {
                bscore = bscore + 5 break;
            }
            case 'Q':
            {
                score = score + 9
            }
            case 'q':
            {
                bscore = bscore + 9 break;
            }
            }
        }
        */

    //evaluation = score - bscore;


    for(int i = 0; i<8; i++)
    {
        for(int j =0; j<8; j++)
        {
            switch(board.squares[i,j])
            {
            case 'P':
            {
                score = score + 1;
                break;
            }
            case 'p':
            {
                bscore = bscore + 1;
                break;
            }
            case 'N':
            {
                score = score + 3;
                break;
            }
            case 'n':
            {
                bscore = bscore + 3;
                break;
            }
            case 'B':
            {
                score = score + 3.5;
                break;
            }
            case 'b':
            {
                bscore = bscore + 3.5 ;
                break;
            }
            case 'R':
            {
                score = score + 5;
                break;
            }
            case 'r':
            {
                bscore = bscore + 5 ;
                break;
            }
            case 'Q':
            {
                score = score + 9;
                break;
            }
            case 'q':
            {
                bscore = bscore + 9 ;
                break;
            }
            case ' ':
            {
                break;
            }
            }
        }
    }

    
    evaluation = score-bscore;
    cout << evaluation;
    return evaluation;
}