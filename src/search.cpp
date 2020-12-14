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

// max hash size in mb
const unsigned int MAX_HASH = 256;

struct Move
{
    float score;
    thc::Move move;
    short int mate = 0;

    Move(float p_eval, thc::Move p_move)
    {
        score = p_eval;
        move = p_move;
    }

    Move(float p_eval)
    {
        score = p_eval;
    }

    Move() {}
};

struct Node
{
    unsigned long long int hash;
    Move move;
    short int depth;
};

const unsigned int TT_SIZE = MAX_HASH * 1000000 / sizeof(Node);

Node transpositionTable[TT_SIZE];

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

Move negamax(thc::ChessRules &board, short int depth, double alpha, double beta, int color, int ply, vector<Move> &pv, vector<Move> &bestPv)
{
    if (!searching)
        return Move(0);

    thc::DRAWTYPE drawType;

    if (board.IsDraw(board.white, drawType))
        return Move(0);

    if (depth <= 0)
        return Move(color * evaluate(board));

    vector<thc::Move> legalMoves;
    vector<bool> check;
    vector<bool> mate;
    vector<bool> stalemate;

    board.GenLegalMoveList(legalMoves, check, mate, stalemate);

    orderMoves(legalMoves, mate, stalemate, bestPv, ply);

    Move bestMove(-1000000);
    Move move;

    thc::ChessRules child;

    vector<Move> childPV;

    for (unsigned int i = 0; i < legalMoves.size(); i++)
    {
        if (mate.at(i))
        {
            move = Move(1000000.0f - ply, legalMoves.at(i));
            move.mate = color;
        }

        else if (stalemate.at(i))
        {
            move = Move(0, legalMoves.at(i));
        }

        else
        {
            child = board;
            child.PlayMove(legalMoves.at(i));

            move = Move(-negamax(child, depth - 1, -beta, -alpha, -color, ply + 1, childPV, bestPv).score, legalMoves.at(i));
        }

        if (move.score > bestMove.score)
        {
            bestMove = move;
        }

        if (bestMove.score > alpha)
        {
            alpha = bestMove.score;

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

void search(thc::ChessRules board, short int maxDepth)
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

            if (searching)
            {
                bestPv = pv;
                string score;

                auto stopTime = Time::now();
                chrono::duration<double> duration = stopTime - startTime;
                double ms = duration.count() * 1000;
                int nps = (int)(nodes / ms * 1000);

                if (move.mate != 0)
                    score = "mate " + to_string((int)ceil(((double)pv.at(0).mate) / 2));

                else
                    score = "cp " + to_string((int)(move.score * 100));

                cout << "info depth " << depth
                     << " score " << score
                     << " time " << (int)ms
                     << " nodes " << nodes
                     << " nps " << nps
                     << " string pv ";

                for (unsigned int i = 0; i < pv.size(); i++)
                {
                    cout << pv.at(i).move.TerseOut() << " ";
                }

                cout << endl;
            }
        }
    }

    using namespace chrono_literals;

    while (pondering)
    {
        this_thread::sleep_for(0.1s);
    }

    if (bestPv.size() > 0)
    {
        cout << "bestmove " << bestPv.at(0).move.TerseOut();

        if (bestPv.size() > 1)
            cout << " ponder " << bestPv.at(1).move.TerseOut();

        cout << endl;
    }

    nodes = 0;
    searching = false;
}
