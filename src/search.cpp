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
    short int mate = 0;

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

void orderMoves(vector<thc::Move> &moveList, vector<bool> &mate, vector<bool> &stalemate, vector<Move> &pv, int &ply)
{
    if (ply >= pv.size())
    {
        return;
    }

    for (int i = 0; i < moveList.size(); i++)
    {
        if (moveList.at(i) == pv.at(ply).move)
        {
            swap(moveList.at(i), moveList.at(0));
            swap(mate.at(i), mate.at(0));
            swap(stalemate.at(i), stalemate.at(0));
        }
    }
}

Move negamax(thc::ChessRules &board, int depth, double alpha, double beta, int color, int ply, vector<Move> &pv, vector<Move> &bestPv)
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

    vector<Move> childPV;

    for (unsigned int i = 0; i < legalMoves.size(); i++)
    {
        if (!searching)
            break;

        if (mate.at(i))
        {
            move = Move(1000000 - ply, legalMoves.at(i));
            move.mate = color;
        }

        else if (stalemate.at(i))
        {
            move = Move(0, legalMoves.at(i));
        }

        else
        {
            board.PlayMove(legalMoves.at(i));

            move = Move(-negamax(board, depth - 1, -beta, -alpha, -color, ply + 1, childPV, bestPv).evaluation, legalMoves.at(i));

            board.PopMove(legalMoves.at(i));
        }

        if (move.evaluation > bestMove.evaluation)
        {
            bestMove = move;
        }

        if (bestMove.evaluation > alpha)
        {
            alpha = bestMove.evaluation;

            if (childPV.size() > 0 && childPV.at(0).mate != 0)
            {
                bestMove.mate = -childPV.at(0).mate + color;
            }

            pv.clear();
            pv.push_back(bestMove);

            copy(childPV.begin(), childPV.end(), back_inserter(pv));
        }

        if (alpha >= beta)
        {
            break;
        }
    }

    nodes++;

    return bestMove;
}

void search(thc::ChessRules board, int maxDepth)
{
    searching = true;

    Move move;

    vector<Move> pv;
    vector<Move> bestPv;

    for (int depth = 1; depth <= maxDepth; depth++)
    {
        if (searching)
        {
            typedef std::chrono::high_resolution_clock Time;
            auto startTime = Time::now();

            bestPv = pv;

            if (board.white)
            {
                move = negamax(board, depth, -1000000, 1000000, 1, 0, pv, pv);
            }

            else
            {
                move = negamax(board, depth, -1000000, 1000000, -1, 0, pv, pv);
            }

            if (move.mate != 0)
            {
                bestPv = pv;

                auto stopTime = Time::now();
                chrono::duration<double> duration = stopTime - startTime;
                double ms = duration.count() * 1000;
                int nps = (int)(nodes / ms * 1000);

                cout << "info depth " << depth
                     << " score cp mate " << ceil(((double)bestPv.at(0).mate) / 2)
                     << " time " << (int)ms
                     << " nodes " << nodes
                     << " nps " << nps
                     << " string pv ";

                for (int i = 0; i < (int)pv.size(); i++)
                {
                    cout << pv.at(i).move.TerseOut() << " ";
                }

                cout << endl;
            }

            else if (searching)
            {
                bestPv = pv;

                auto stopTime = Time::now();
                chrono::duration<double> duration = stopTime - startTime;
                double ms = duration.count() * 1000;
                int nps = (int)(nodes / ms * 1000);

                cout << "info depth " << depth
                     << " score cp " << (int)(move.evaluation * 100)
                     << " time " << (int)ms
                     << " nodes " << nodes
                     << " nps " << nps
                     << " string pv ";

                for (int i = 0; i < (int)pv.size(); i++)
                {
                    cout << pv.at(i).move.TerseOut() << " ";
                }

                cout << endl;
            }
        }
    }

    if (!pondering)
    {
        cout << "bestmove " << bestPv.at(0).move.TerseOut();

        if (bestPv.size() > 1)
            cout << " ponder " << bestPv.at(1).move.TerseOut();

        cout << endl;
    }

    nodes = 0;
    searching = false;
    pondering = false;
}
