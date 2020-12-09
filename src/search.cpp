#include "../lib/thc/thc.h"
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <tuple>

#include "uci.h"
#include "evaluate.h"

using namespace std;

static int nodes;

//vector<thc::Move> bestLine(10);

struct Move
{
    double evaluation;
    thc::Move move;

    Move(double p_eval, thc::Move p_move)
    {
        evaluation = p_eval;
        move = p_move;
    }

    Move(double p_eval)
    {
        evaluation = p_eval;
    }

    Move() {}
};

Move negamax(thc::ChessRules &board, int depth, double alpha, double beta, int color, Move prevBest)
{
    if (depth <= 0 || !searching)
        return Move(color * evaluate(board));

    vector<thc::Move> legalMoves;
    board.GenLegalMoveList(legalMoves);

    Move bestMove(-1000000);

    for (unsigned int i = 0; i < legalMoves.size(); i++)
    {
        thc::ChessRules child = board;
        child.PlayMove(legalMoves.at(i));

        Move move(-negamax(child, depth - 1, -beta, -alpha, -color, prevBest).evaluation, legalMoves.at(i));

        if (move.evaluation > bestMove.evaluation)
        {
            bestMove = move;
        }

        alpha = max(alpha, bestMove.evaluation);

        if (alpha >= beta)
        {
            break;
        }
    }

    nodes++;

    /*
    bestLine.at(depth - 1) = bestMove.move;
    cout << bestMove.move.NaturalOut(&board) << " "
         << "\n";
         */

    if (searching)
    {
        return bestMove;
    }

    else
    {
        return prevBest;
    }
}

void search(thc::ChessRules board, int maxDepth)
{
    searching = true;

    Move bestMove;

    for (int depth = 1; depth <= maxDepth; depth++)
    {
        if (searching)
        {
            typedef std::chrono::high_resolution_clock Time;
            auto startTime = Time::now();

            /*
            bestLine.clear();
            bestLine.resize(10);
            */

            if (board.white)
            {
                bestMove = negamax(board, depth, -1000000, 1000000, 1, bestMove);
            }

            else
            {
                bestMove = negamax(board, depth, -1000000, 1000000, -1, bestMove);
                bestMove.evaluation *= -1;
            }

            if (searching)
            {
                auto stopTime = Time::now();
                chrono::duration<double> duration = stopTime - startTime;
                double ms = duration.count() * 1000;
                int nps = (int)(nodes / ms * 1000);

                cout << "info depth " << depth
                     << " score cp " << (int)(bestMove.evaluation * 100)
                     << " currmove " << bestMove.move.TerseOut()
                     << " time " << (int)ms
                     << " nodes " << nodes
                     << " nps " << nps
                     << endl;
            }

            /*
            for (int i = (int)bestLine.size() - 1; i >= 0; i--)
            {
                cout << bestLine.at(i).NaturalOut(&board) << " ";
            }

            cout << "\n";
            */
        }
    }

    cout << "bestmove " << bestMove.move.TerseOut() << endl;

    nodes = 0;
    searching = false;
}
