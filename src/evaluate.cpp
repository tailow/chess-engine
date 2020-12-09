#include "../lib/thc/thc.h"
#include <iostream>

using namespace std;

double evaluate(thc::ChessRules board)
{
    volatile double evaluation = 0;

    // Queens alive?
    bool wkingSafety = true;
    bool bkingSafety = true;
    bool wAlive = false;
    bool bAlive = false;

    for (volatile int rank = 0; rank < 8; rank++)
    {
        for (volatile int file = 0; file < 8; file++)
        {
            switch (board.squares[rank * 8 + file])
            {
            case 'P':
            {
                evaluation += 1;

                if (rank < 5 && (file == 4 || file == 3 || file == 2))
                {
                    evaluation += 0.33;
                }

                break;
            }
            case 'p':
            {
                evaluation -= 1;

                if (rank > 2 && (file == 4 || file == 3 || file == 2))
                {
                    evaluation -= 0.33;
                }

                break;
            }
            case 'N':
            {
                evaluation += 3;

                if (rank == 7)
                {
                    evaluation -= 0.33;
                }
                if (file == 0 || file == 7)
                {
                    evaluation -= 0.5;
                }

                break;
            }
            case 'n':
            {
                evaluation -= 3;

                if (rank == 0)
                {
                    evaluation += 0.33;
                }
                if (file == 0 || file == 7)
                {
                    evaluation += 0.5;
                }

                break;
            }
            case 'B':
            {
                evaluation += 3.2;

                if (rank == 7)
                {
                    evaluation -= 0.2;
                }
                if (rank == 6 && (file == 1 || file == 6))
                {
                    evaluation += 0.35;
                }

                break;
            }
            case 'b':
            {
                evaluation -= 3.2;

                if (rank == 0)
                {
                    evaluation += 0.2;
                }
                if (rank == 1 && (file == 1 || file == 6))
                {
                    evaluation -= 0.35;
                }

                break;
            }
            case 'R':
            {
                evaluation += 5;

                if (file == 0 || file == 1 || file == 6 || file == 7)
                {
                    evaluation -= 0.8;
                }
                if (rank == 0 || rank == 1 || rank == 6 || rank == 7)
                {
                    evaluation += 0.22;
                }

                break;
            }
            case 'r':
            {
                evaluation -= 5;

                if (file == 0 || file == 1 || file == 6 || file == 7)
                {
                    evaluation += 0.8;
                }
                if (rank == 0 || rank == 1 || rank == 6 || rank == 7)
                {
                    evaluation -= 0.22;
                }

                break;
            }
            case 'Q':
            {
                evaluation += 9;
                bkingSafety = false;
                break;
            }
            case 'q':
            {
                evaluation -= 9;
                wkingSafety = false;
                break;
            }
            case 'K':
            {
                wAlive = true;
                break;
            }
            case 'k':
            {
                bAlive = true;
                break;
            }
            }
        }
    }

    // try to castle does not work somereason test fen (r1bqk1nr/pppp1ppp/2n5/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4)
    if ((board.squares[58] == 'K' || board.squares[62] == 'K') && wkingSafety == false)
    {
        evaluation += 0.6;
    }

    // arrays start at 0 dummy, also starts at a8, also square is not piece dummy
    if ((board.squares[2] == 'k' || board.squares[6] == 'k') && bkingSafety == false)
    {
        evaluation -= 0.6;
    }

    //checkmate
    if (board.WhiteToPlay() && board.AttackedSquare(board.wking_square, false))
    {
        vector<thc::Move> legalMoves;
        board.GenLegalMoveList(legalMoves);

        if (legalMoves.size() == 0)
        {
            evaluation -= 1000000;
        }
    }

    else if (!board.WhiteToPlay() && board.AttackedSquare(board.bking_square, true))
    {
        vector<thc::Move> legalMoves;
        board.GenLegalMoveList(legalMoves);

        if (legalMoves.size() == 0)
        {
            evaluation += 1000000;
        }
    }

    //stalemate
    /*
    else if ()
    {
        evaluation = 0;
    }
    */

    evaluation += ((rand() % 2) - 1);

    //cout << wkingSafety << "  <-w  b-->  " << bkingSafety;

    return evaluation;
}