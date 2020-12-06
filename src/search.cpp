#include "../lib/thc/thc.h"
#include <string>
#include <iostream>
#include "evaluate.h"

using namespace std;

int depth = 1;

double alphabeta(thc::ChessRules &board, int depth, double alpha, double beta, bool maximizing)
{
    if (depth == 0)
    {
        return evaluate(board);
    }

    thc::Move move;

    vector<thc::Move> legalMoves;

    board.GenLegalMoveList(legalMoves);

    if (maximizing)
    {
        double value = -100000;

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
        double value = 100000;

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

    if (board.white)
    {
        bestEvaluation = -1000000;
    }

    else
    {
        bestEvaluation = 1000000;
    }

    board.GenLegalMoveList(legalMoves);

    for (int i = 0; i < legalMoves.size(); i++)
    {
        thc::ChessRules child = board;
        child.PlayMove(legalMoves.at(i));

        if (board.white)
        {
            double evaluation = alphabeta(child, depth, 100000, -100000, false);

            cout << "evaluation for move " << legalMoves.at(i).NaturalOut(&board) << ": " << evaluation << endl;

            if (evaluation > bestEvaluation)
            {
                bestMove = legalMoves.at(i);
                bestEvaluation = evaluation;
            }
        }

        else
        {
            double evaluation = alphabeta(child, depth, -100000, 100000, true);

            if (evaluation < bestEvaluation)
            {
                bestMove = legalMoves.at(i);
                bestEvaluation = evaluation;
            }
        }
    }

    //cout << "info score " << bestEvaluation << endl;

    return bestMove;
}
