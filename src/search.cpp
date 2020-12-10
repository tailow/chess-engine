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

void orderMoves(vector<thc::Move> &moveList, vector<bool> &mate, vector<bool> &stalemate, vector<thc::Move> &pv, int &ply)
{
    if (ply >= pv.size())
    {
        return;
    }

    for (int i = 0; i < moveList.size(); i++)
    {
        if (moveList.at(i) == pv.at(ply))
        {
            swap(moveList.at(i), moveList.at(0));
            swap(mate.at(i), mate.at(0));
            swap(stalemate.at(i), stalemate.at(0));
        }
    }
}

Move negamax(thc::ChessRules &board, int depth, double alpha, double beta, int color, int ply, vector<thc::Move> &pv, Move &prevBest, vector<thc::Move> bestPv)
{
    if (depth <= 0 || !searching)
        return Move(color * evaluate(board));

    vector<thc::Move> legalMoves;
    vector<bool> check;
    vector<bool> mate;
    vector<bool> stalemate;

    board.GenLegalMoveList(legalMoves, check, mate, stalemate);

    orderMoves(legalMoves, mate, stalemate, bestPv, ply);

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

            move = Move(-negamax(board, depth - 1, -beta, -alpha, -color, ply + 1, childPV, prevBest, bestPv).evaluation, legalMoves.at(i));

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

    vector<thc::Move> bestLine;

    for (int depth = 1; depth <= maxDepth; depth++)
    {
        if (searching)
        {
            typedef std::chrono::high_resolution_clock Time;
            auto startTime = Time::now();

            if (board.white)
            {
                bestMove = negamax(board, depth, -1000000, 1000000, 1, 0, bestLine, bestMove, bestLine);
            }

            else
            {
                bestMove = negamax(board, depth, -1000000, 1000000, -1, 0, bestLine, bestMove, bestLine);
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
    searching = false;
}
