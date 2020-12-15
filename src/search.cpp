#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <tuple>
#include <algorithm>
#include <iterator>
#include <cstdint>
#include <array>

#include "uci.h"
#include "evaluate.h"
#include "../lib/thc/thc.h"

using namespace std;

struct Move
{
    float score;
    thc::Move move;
    int8_t mate = 0;

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

enum Type
{
    EXACT,
    UPPERBOUND,
    LOWERBOUND
};

struct Node
{
    uint64_t hash;
    thc::Move bestMove;
    uint8_t depth;
    float score;
    Type type;

    Node(uint64_t p_hash, thc::Move p_bestMove, uint8_t p_depth, float p_score, Type p_type)
    {
        hash = p_hash;
        bestMove = p_bestMove;
        depth = p_depth;
        score = p_score;
        type = p_type;
    }

    Node()
    {
    }
};

int nodes;
int transpositions;
int ttCutoffs;
int ttSize;

// max hash size in mb
const unsigned int MAX_HASH = 256;

const size_t TT_MAX_SIZE = MAX_HASH * 1000000 / sizeof(Node);

array<Node, TT_MAX_SIZE> tTable = {Node()};

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

Move negamax(thc::ChessRules &board, uint8_t depth, float alpha, float beta, int color, int ply, vector<Move> &pv, vector<Move> &bestPv, uint64_t &hash)
{
    if (!uci::searching)
        return Move(0);

    nodes++;

    Node node;
    node.hash = hash;
    node.depth = depth;

    double alphaOrig = alpha;

    // check for transposition
    for (int i = 0; i < ttSize; i++)
    {
        // transposition
        if (tTable.at(i).hash == hash)
        {
            Node ttHit = tTable.at(i);

            transpositions++;

            if (ttHit.depth >= depth)
            {
                if (ttHit.type == EXACT)
                {
                    ttCutoffs++;
                    return Move(ttHit.score, ttHit.bestMove);
                }

                else if (ttHit.type == LOWERBOUND)
                {
                    alpha = max(alpha, ttHit.score);
                }

                else if (ttHit.type == UPPERBOUND)
                {
                    beta = min(beta, ttHit.score);
                }

                if (alpha >= beta)
                {
                    ttCutoffs++;
                    return Move(ttHit.score, ttHit.bestMove);
                }
            }

            break;
        }
    }

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

    vector<Move> childPV;

    for (int i = 0; i < legalMoves.size(); i++)
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
            thc::ChessRules child = board;
            child.PlayMove(legalMoves.at(i));

            hash = board.Hash64Update(hash, legalMoves.at(i));

            move = Move(-negamax(child, depth - 1, -beta, -alpha, -color, ply + 1, childPV, bestPv, hash).score, legalMoves.at(i));

            hash = board.Hash64Update(hash, legalMoves.at(i));
        }

        if (move.score > bestMove.score)
        {
            bestMove = move;

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
        }

        if (alpha >= beta)
        {
            break;
        }
    }

    node.bestMove = bestMove.move;
    node.score = bestMove.score;

    if (bestMove.score <= alphaOrig)
    {
        node.type = UPPERBOUND;
    }

    else if (bestMove.score >= beta)
    {
        node.type = LOWERBOUND;
    }

    else
    {
        node.type = EXACT;
    }

    // store table entry
    if (ttSize < TT_MAX_SIZE)
    {
        tTable.at(ttSize) = node;
        ttSize++;
    }

    else
    {
        tTable.at(0) = node;
    }

    return bestMove;
}

void search(thc::ChessRules board, short int maxDepth, uint64_t hash)
{
    uci::searching = true;

    Move move;

    vector<Move> pv;
    vector<Move> bestPv;

    for (int depth = 1; depth <= maxDepth; depth++)
    {
        if (uci::searching)
        {
            typedef std::chrono::high_resolution_clock Time;
            auto startTime = Time::now();

            bestPv = pv;

            if (board.white)
            {
                move = negamax(board, depth, -1000000, 1000000, 1, 0, pv, pv, hash);
            }

            else
            {
                move = negamax(board, depth, -1000000, 1000000, -1, 0, pv, pv, hash);
            }

            if (uci::searching)
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

    while (uci::pondering)
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

    cout << "transpositions: " << transpositions << endl;
    cout << "cutoffs: " << ttCutoffs << endl;

    /*
    for (int i = 0; i < TT_MAX_SIZE; i++)
    {
        if (tTable.at(i).hash != 0)
        {
            cout << "\nhash: " << tTable.at(i).hash
                 << "\nbestmove: " << tTable.at(i).bestMove.TerseOut()
                 << "\nscore: " << tTable.at(i).score
                 << "\ndepth: " << to_string(tTable.at(i).depth);

            cout << endl;
        }
    }
    */

    nodes = 0;
    transpositions = 0;
    ttCutoffs = 0;

    uci::searching = false;
}
