#include "../lib/thc/thc.h"
using namespace std;
double evaluate(thc::ChessRules board)
{
    double evaluation;
    double score;
    double bscore;


    for( square=a8; square<=h1; ++square )
    {

    char piece=squares[square];

    if( (white&&IsWhite(piece)) || (!white&&IsBlack(piece)) )
    {
        switch( piece )
            {
                case 'P':
                {
                    score = score + 1
                    break;
                }
                case 'p':
                {
                    bscore = bscore + 1
                    break;
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
                    bscore = bscore + 3.5
                    break;
                }
                case 'R':
                {
                    score = score + 5
                }
                case 'r':
                {
                    bscore = bscore + 5
                    break;
                }
                case 'Q':
                {
                    score = score + 9
                }
                case 'q':
                {
                    bscore = bscore + 9
                    break;
                }
        }
    }

    evaluation = score - bscore;
    
    return evaluation;
}