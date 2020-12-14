#include "../lib/thc/thc.h"
#include <iostream>

using namespace std;

float evaluate(thc::ChessRules &board)
{
    float evaluation = 0;

    // Queens alive?

    bool wkingSafety = true;
    bool bkingSafety = true;

    for (int rank = 0; rank < 8; rank++)
    {
        for (int file = 0; file < 8; file++)
        {
            switch (board.squares[rank * 8 + file])
            {
            case 'P':
            {
                evaluation += 1;

                if (rank < 5 && (file == 4 || file == 3))
                {
                    evaluation += 0.4f;
                }
                if (rank < 6 && (board.squares[(rank + 1) * 8 + file + 1] == 'P' || board.squares[(rank + 1 * 8) + file - 1] == 'P'))
                {
                    evaluation += 0.3f;
                }
                if (board.squares[(rank + 1) * 8 + file] == 'P')
                {
                    evaluation -= 0.19f;
                }

                break;
            }
            case 'p':
            {
                evaluation -= 1;

                if (rank > 2 && (file == 4 || file == 3))
                {
                    evaluation -= 0.4f;
                }
                //likes pawns being protected by other pawns maybe?
                if (rank > 1 && (board.squares[(rank - 1) * 8 + file + 1] == 'p' || board.squares[(rank - 1) * 8 + file - 1] == 'p'))
                {
                    evaluation -= 0.3f;
                }
                //doesnt like doubled pawns
                if (board.squares[(rank - 1) * 8 + file] == 'p')
                {
                    evaluation += 0.19f;
                }

                break;
            }
            case 'N':
            {
                evaluation += 3;

                if (rank == 7)
                {
                    evaluation -= 0.33f;
                }
                if (file == 0 || file == 7)
                {
                    evaluation -= 0.5f;
                }

                break;
            }
            case 'n':
            {
                evaluation -= 3;

                if (rank == 0)
                {
                    evaluation += 0.33f;
                }
                if (file == 0 || file == 7)
                {
                    evaluation += 0.5f;
                }

                break;
            }
            case 'B':
            {
                evaluation += 3.2f;

                if (rank == 7)
                {
                    evaluation -= 0.2f;
                }
                if (rank == 6 && (file == 1 || file == 6))
                {
                    evaluation += 0.35f;
                }

                break;
            }
            case 'b':
            {
                evaluation -= 3.2f;

                if (rank == 0)
                {
                    evaluation += 0.2f;
                }
                if (rank == 1 && (file == 1 || file == 6))
                {
                    evaluation -= 0.35f;
                }

                break;
            }
            case 'R':
            {
                evaluation += 5;

                if (file == 0 || file == 1 || file == 6 || file == 7)
                {
                    evaluation -= 0.8f;
                }
                if (!(rank == 0 || rank == 1 || rank == 6 || rank == 7))
                {
                    evaluation -= 0.22f;
                }
                // likes rooks next to each other
                if (!((rank * 8 + file + 1) == 'R' || (rank * 8 + file + -1) == 'R'))
                {
                    evaluation -= 0.3f;
                }

                break;
            }
            case 'r':
            {
                evaluation -= 5;

                if (file == 0 || file == 1 || file == 6 || file == 7)
                {
                    evaluation += 0.8f;
                }
                if (!(rank == 0 || rank == 1 || rank == 6 || rank == 7))
                {
                    evaluation += 0.22f;
                }
                // likes rooks next to each other
                if (!((rank * 8 + file + 1) == 'r' || (rank * 8 + file + -1) == 'r'))
                {
                    evaluation += 0.3f;
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
                //check if the king is safe when castled
                if (((rank - 1) * 8 + file == 'P') && ((rank - 1) * 8 + file + 1 == 'P'))
                {
                    evaluation += 0.3f;
                }
                break;
            }
            case 'k':
            {
                if (((rank + 1) * 8 + file == 'p') && ((rank + 1) * 8 + file + 1 == 'p'))
                {
                    evaluation -= 0.3f;
                }
                break;
            }
            }
        }
    }

    if ((board.squares[58] == 'K' || board.squares[62] == 'K') && wkingSafety == false)
    {
        evaluation += 0.5f;
    }

    if ((board.squares[2] == 'k' || board.squares[6] == 'k') && bkingSafety == false)
    {
        evaluation -= 0.5f;
    }

    evaluation += (rand() % 2 - 1) / 100;

    return evaluation;
}