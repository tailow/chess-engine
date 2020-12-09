#include "../lib/thc/thc.h"
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <tuple>
#include <algorithm>
#include <iterator>

#include "uci.h"
#include "evaluate.h"

using namespace std;

static int nodes;
static int prunings;

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

Move negamax(thc::ChessRules &board, int depth, double alpha, double beta, int color, int ply, vector<thc::Move> &pv, Move prevBest)
{
    if (depth <= 0 || !searching)
        return Move(color * evaluate(board));

    vector<thc::Move> legalMoves;
    vector<bool> check;
    vector<bool> mate;
    vector<bool> stalemate;

    board.GenLegalMoveList(legalMoves, check, mate, stalemate);

    Move bestMove(-1000000);
    Move move;

    for (unsigned int i = 0; i < legalMoves.size(); i++)
    {
        vector<thc::Move> childPV;

        if (mate.at(i))
        {
            move = Move(1000000, legalMoves.at(i));

            childPV.push_back(move.move);
        }

        else if (stalemate.at(i))
        {
            move = Move(0, legalMoves.at(i));

            childPV.push_back(move.move);
        }

        else
        {
            board.PlayMove(legalMoves.at(i));

            move = Move(-negamax(board, depth - 1, -beta, -alpha, -color, ply + 1, childPV, prevBest).evaluation, legalMoves.at(i));

            board.PopMove(legalMoves.at(i));
        }

        if (move.evaluation > bestMove.evaluation)
        {
            bestMove = move;
        }

        if (bestMove.evaluation > alpha)
        {
            alpha = bestMove.evaluation;

            pv.clear();
            pv.push_back(bestMove.move);

            copy(childPV.begin(), childPV.end(), back_inserter(pv));
        }

        if (alpha >= beta)
        {
            prunings++;
            break;
        }
    }

    nodes++;

    if (!searching)
    {
        return prevBest;
    }

    return bestMove;
}

void search(thc::ChessRules board, int maxDepth)
{
    searching = true;

    Move bestMove;

    for (int depth = 1; depth <= maxDepth; depth++)
    {
        vector<thc::Move> bestLine;

        if (searching)
        {
            typedef std::chrono::high_resolution_clock Time;
            auto startTime = Time::now();

            bestLine.clear();

            if (board.white)
            {
                bestMove = negamax(board, depth, -1000000, 1000000, 1, 0, bestLine, bestMove);
            }

            else
            {
                bestMove = negamax(board, depth, -1000000, 1000000, -1, 0, bestLine, bestMove);
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
                     << " string prunings " << prunings
                     << " pv ";

                for (int i = 0; i < (int)bestLine.size(); i++)
                {
                    cout << bestLine.at(i).TerseOut() << " ";
                }

                cout << endl;
            }
        }
    }

    cout << "bestmove " << bestMove.move.TerseOut() << endl;

    nodes = 0;
    prunings = 0;
    searching = false;
}
