#include "../lib/thc/thc.h"
#include <string>
#include <iostream>
#include "evaluate.h"

using namespace std;

int depth = 4;

double alphabeta(thc::ChessRules &board, int depth, double alpha, double beta, bool maximizing)
{
    vector<thc::Move> legalMoves;

    board.GenLegalMoveList(legalMoves);

    if (depth <= 0 || legalMoves.size() == 0)
    {
        return evaluate(board);
    }

    if (maximizing)
    {
        double value = INT_MIN;
        thc::Move maxMove;

        for (int i = 0; i < legalMoves.size(); i++)
        {
            thc::ChessRules child = board;
            child.PlayMove(legalMoves.at(i));

            value = max(value, alphabeta(child, depth - 1, alpha, beta, false));

            alpha = max(alpha, value);

            if (alpha >= beta)
            {
                break;
            }
        }

        return value;
    }

    else
    {
        double value = INT_MAX;
        thc::Move minMove;

        for (int i = 0; i < legalMoves.size(); i++)
        {
            thc::ChessRules child = board;
            child.PlayMove(legalMoves.at(i));

            value = min(value, alphabeta(child, depth - 1, alpha, beta, true));

            beta = min(beta, value);

            if (beta <= alpha)
            {
                break;
            }
        }

        return value;
    }
}

thc::Move search(thc::ChessRules board)
{
    thc::Move bestMove;

    vector<thc::Move> legalMoves;

    double bestEvaluation;
    double evaluation;

    board.GenLegalMoveList(legalMoves);

    if (board.white)
    {
        bestEvaluation = INT_MIN;

        for (int i = 0; i < legalMoves.size(); i++)
        {
            thc::ChessRules child = board;
            child.PlayMove(legalMoves.at(i));

            evaluation = alphabeta(child, depth - 1, INT_MIN, INT_MAX, false);

            if (evaluation > bestEvaluation)
            {
                bestMove = legalMoves.at(i);
                bestEvaluation = evaluation;
            }

            cout << "move " << legalMoves.at(i).NaturalOut(&board) << " evaluation: " << evaluation << endl;
        }
    }

    else if (!board.white)
    {
        bestEvaluation = INT_MAX;

        for (int i = 0; i < legalMoves.size(); i++)
        {
            thc::ChessRules child = board;
            child.PlayMove(legalMoves.at(i));

            evaluation = alphabeta(child, depth - 1, INT_MIN, INT_MAX, true);

            if (evaluation < bestEvaluation)
            {
                bestMove = legalMoves.at(i);
                bestEvaluation = evaluation;
            }

            cout << "move " << legalMoves.at(i).NaturalOut(&board) << " evaluation: " << evaluation << endl;
        }
    }

    cout << "info score cp " << (int)bestEvaluation * 100 << " depth " << depth - 1 << endl;

    return bestMove;
}
