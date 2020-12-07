#include "../lib/thc/thc.h"
#include <string>
#include <iostream>
#include "evaluate.h"

using namespace std;

int depth = 10;

double alphabeta(thc::ChessRules &board, int depth, double alpha, double beta, bool maximizing)
{
    vector<thc::Move> legalMoves;

    board.GenLegalMoveList(legalMoves);

    if (depth == 0 || legalMoves.size() == 0)
    {
        return evaluate(board);
    }

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

        double evaluation;

        if (child.white)
        {
            evaluation = alphabeta(child, depth, -100000, 100000, true);

            if (evaluation < bestEvaluation)
            {
                bestMove = legalMoves.at(i);
                bestEvaluation = evaluation;
            }

            else if (evaluation == bestEvaluation && (rand() % 100) < 25)
            {
                bestMove = legalMoves.at(i);
                bestEvaluation = evaluation;
            }
        }

        else
        {
            evaluation = alphabeta(child, depth, 100000, -100000, false);

            if (evaluation > bestEvaluation)
            {
                bestMove = legalMoves.at(i);
                bestEvaluation = evaluation;
            }

            else if (evaluation == bestEvaluation && (rand() % 100) < 25)
            {
                bestMove = legalMoves.at(i);
                bestEvaluation = evaluation;
            }
        }

        cout << "evaluation for move " << legalMoves.at(i).NaturalOut(&board) << ": " << evaluation << endl;
    }

    return bestMove;
}
