#include "../lib/thc/thc.h"
#include <string>
#include <iostream>
#include "evaluate.h"
#include <thread>
#include <chrono>
#include "uci.h"

using namespace std;

long nodes;

double alphabeta(thc::ChessRules &board, int depth, double alpha, double beta, bool maximizing)
{
    vector<thc::Move> legalMoves;

    board.GenLegalMoveList(legalMoves);

    if (depth <= 0 || legalMoves.size() == 0 || !searching)
    {
        return evaluate(board);
    }

    if (maximizing)
    {
        double value = -1000000;

        for (unsigned int i = 0; i < legalMoves.size(); i++)
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

        nodes++;

        return value;
    }

    else
    {
        double value = 1000000;

        for (unsigned int i = 0; i < legalMoves.size(); i++)
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

        nodes++;

        return value;
    }
}

void search(thc::ChessRules board, int maxDepth)
{
    searching = true;

    thc::Move bestMove;
    thc::Move currentBestMove;

    for (int depth = 1; depth <= maxDepth; depth++)
    {
        if (searching)
        {
            typedef std::chrono::high_resolution_clock Time;

            auto startTime = Time::now();

            vector<thc::Move> legalMoves;

            double bestEvaluation;
            double evaluation;

            board.GenLegalMoveList(legalMoves);

            if (board.white)
            {
                bestEvaluation = -1000000;

                for (unsigned int i = 0; i < legalMoves.size(); i++)
                {
                    thc::ChessRules child = board;
                    child.PlayMove(legalMoves.at(i));

                    evaluation = alphabeta(child, depth - 1, -1000000, 1000000, false);

                    if (evaluation > bestEvaluation)
                    {
                        currentBestMove = legalMoves.at(i);
                        bestEvaluation = evaluation;
                    }

                    else if (evaluation == bestEvaluation && rand() % 100 < 25)
                    {
                        currentBestMove = legalMoves.at(i);
                        bestEvaluation = evaluation;
                    }

                    //cout << "move " << legalMoves.at(i).NaturalOut(&board) << " evaluation: " << evaluation << endl;
                }
            }

            else if (!board.white)
            {
                bestEvaluation = 1000000;

                for (unsigned int i = 0; i < legalMoves.size(); i++)
                {
                    thc::ChessRules child = board;
                    child.PlayMove(legalMoves.at(i));

                    evaluation = alphabeta(child, depth - 1, -1000000, 1000000, true);

                    if (evaluation < bestEvaluation)
                    {
                        currentBestMove = legalMoves.at(i);
                        bestEvaluation = evaluation;
                    }

                    else if (evaluation == bestEvaluation && rand() % 100 < 25)
                    {
                        currentBestMove = legalMoves.at(i);
                        bestEvaluation = evaluation;
                    }

                    //cout << "move " << legalMoves.at(i).NaturalOut(&board) << " evaluation: " << evaluation << endl;
                }
            }

            if (searching)
            {
                bestMove = currentBestMove;
            }

            auto stopTime = Time::now();

            chrono::duration<double> delta = stopTime - startTime;

            auto duration = chrono::duration_cast<chrono::milliseconds>(delta).count();

            long nps = nodes;

            cout << "info depth " << depth
                 << " score cp " << (int)(bestEvaluation * 100)
                 << " currmove " << bestMove.TerseOut()
                 << " time " << duration
                 << " nodes " << nodes
                 << " nps " << nps
                 << endl;
        }
    }

    cout << "bestmove " << bestMove.TerseOut() << endl;

    searching = false;
}
