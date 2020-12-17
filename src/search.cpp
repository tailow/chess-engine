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

    Move(float p_eval, thc::Move p_move, int8_t p_mate)
    {
        score = p_eval;
        move = p_move;
        mate = p_mate;
    }

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
    Move bestMove;
    uint8_t depth;
    Type type;

    Node(uint64_t p_hash, Move p_bestMove, uint8_t p_depth, float p_score, Type p_type, int8_t p_mate)
    {
        hash = p_hash;
        bestMove = p_bestMove;
        depth = p_depth;
        type = p_type;
    }

    Node()
    {
    }
};

int nodes;
int transpositions;

// max hash size in mb
const unsigned int MAX_HASH = 1024;

const size_t TT_MAX_SIZE = MAX_HASH * 1000000 / sizeof(Node);

array<Node, TT_MAX_SIZE> tTable = {Node()};

void orderMoves(vector<thc::Move> &moveList, vector<bool> &mate, vector<bool> &stalemate, vector<Move> &pv, int &ply, uint64_t &hash, thc::ChessRules &board)
{
    if (ply >= pv.size())
    {
        return;
    }

    vector<float> scores(moveList.size(), -100000);

    // selection sort
    for (int j = 0; j < moveList.size(); j++)
    {
        for (int i = j; i < moveList.size(); i++)
        {
            hash = board.Hash64Update(hash, moveList[i]);

            int ttIndex = hash % TT_MAX_SIZE;

            hash = board.Hash64Update(hash, moveList[i]);

            if (moveList[i] == pv[ply].move)
            {
                scores[j] = pv[ply].score;

                swap(moveList[i], moveList[0]);
                swap(mate[i], mate[0]);
                swap(stalemate[i], stalemate[0]);

                break;
            }

            // if stored move score is better
            else if (tTable[ttIndex].bestMove.score != 0 && -tTable[ttIndex].bestMove.score > scores[j])
            {
                scores[j] = -tTable[ttIndex].bestMove.score;

                swap(moveList[i], moveList[j]);
                swap(mate[i], mate[j]);
                swap(stalemate[i], stalemate[j]);
            }
        }
    }
}

Move negamax(thc::ChessRules &board, uint8_t depth, float alpha, float beta, int color, int ply, vector<Move> &pv, uint64_t &hash)
{
    if (!uci::searching)
        return Move(0);

    nodes++;

    if (depth <= 0)
        return Move(color * evaluate(board));

    double alphaOrig = alpha;

    Node ttNode = tTable[hash % TT_MAX_SIZE];

    // tranposition
    /*
    if (ttNode.hash == hash && ttNode.depth >= depth)
    {
        transpositions++;

        if (ttNode.type == EXACT)
        {
            return ttNode.bestMove;
        }

        else if (ttNode.type == LOWERBOUND)
        {
            alpha = max(alpha, ttNode.bestMove.score);
        }

        else if (ttNode.type == UPPERBOUND)
        {
            beta = min(beta, ttNode.bestMove.score);
        }

        if (alpha >= beta)
        {
            return ttNode.bestMove;
        }
    }
    */

    vector<thc::Move> legalMoves;
    vector<bool> check;
    vector<bool> mate;
    vector<bool> stalemate;

    board.GenLegalMoveList(legalMoves, check, mate, stalemate);

    orderMoves(legalMoves, mate, stalemate, pv, ply, hash, board);

    Move bestMove(-1000000);
    Move childBestMove;
    Move move;

    for (int i = 0; i < legalMoves.size(); i++)
    {
        if (mate[i])
        {
            move = Move(1000000.0f - ply, legalMoves[i]);
            move.mate = color;
        }

        else if (stalemate[i])
        {
            move = Move(0, legalMoves[i]);
        }

        else
        {
            thc::ChessRules child = board;
            child.PlayMove(legalMoves[i]);

            hash = board.Hash64Update(hash, legalMoves[i]);

            childBestMove = negamax(child, depth - 1, -beta, -alpha, -color, ply + 1, pv, hash);

            move = Move(-childBestMove.score, legalMoves[i]);

            hash = board.Hash64Update(hash, legalMoves[i]);
        }

        if (move.score > bestMove.score)
        {
            bestMove = move;

            if (ply <= pv.size() && childBestMove.mate != 0)
            {
                bestMove.mate = -childBestMove.mate + color;
            }

            alpha = max(alpha, bestMove.score);
        }

        if (alpha >= beta)
        {
            break;
        }
    }

    Node node;
    node.hash = hash;
    node.depth = depth;
    node.bestMove = bestMove;

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
    if (tTable[hash % TT_MAX_SIZE].hash == 0 || tTable[hash % TT_MAX_SIZE].depth <= depth)
    {
        tTable[hash % TT_MAX_SIZE] = node;
    }

    return bestMove;
}

vector<Move> getPv(uint64_t hash, uint8_t depth, thc::ChessRules board)
{
    vector<Move> pv;

    for (int i = 0; i < depth; i++)
    {
        Node node = tTable[hash % TT_MAX_SIZE];

        if (node.bestMove.move.TerseOut() != "0000")
        {
            pv.push_back(node.bestMove);

            hash = board.Hash64Update(hash, node.bestMove.move);

            board.PlayMove(node.bestMove.move);
        }
    }

    return pv;
}

void search(thc::ChessRules board, uint8_t maxDepth, uint64_t hash)
{
    uci::searching = true;

    typedef std::chrono::high_resolution_clock Time;
    auto startTime = Time::now();

    Move move;

    vector<Move> pv;
    vector<Move> bestPv;

    for (int depth = 1; depth <= maxDepth; depth++)
    {
        if (uci::searching)
        {
            if (board.white)
            {
                move = negamax(board, depth, -1000000, 1000000, 1, 0, pv, hash);

                pv = getPv(hash, depth, board);
            }

            else
            {
                move = negamax(board, depth, -1000000, 1000000, -1, 0, pv, hash);

                pv = getPv(hash, depth, board);
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

    nodes = 0;

    uci::searching = false;
}
