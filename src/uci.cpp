#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdint>

#include "../lib/thc/thc.h"
#include "search.h"
#include "evaluate.h"
#include "timeman.h"

using namespace std;

namespace uci
{
    thc::ChessRules board;

    uint8_t maxDepth;
    uint8_t defaultMaxDepth = 20;

    int timeLeft = 60000;
    int timeControl = 0;

    uint64_t hash;

    bool searching = false;
    bool pondering = false;
    bool ponderHit = false;
    bool useTimer = true;

    thread searcher;
    thread timer;

    vector<string> split(const string &str, const string &delim)
    {
        vector<string> tokens;

        size_t last = 0;
        size_t next = 0;

        while ((next = str.find(delim, last)) != string::npos)
        {
            tokens.push_back(str.substr(last, next - last));
            last = next + 1;
        }

        tokens.push_back(str.substr(last, next - last));

        return tokens;
    }

    void stop()
    {
        searching = false;
        pondering = false;
        ponderHit = false;
    }

    void go(vector<string> tokens)
    {
        useTimer = true;
        maxDepth = defaultMaxDepth;

        hash = board.Hash64Calculate();

        for (unsigned int i = 1; i < tokens.size(); i++)
        {
            if (tokens.at(i) == "wtime" && board.white)
            {
                timeLeft = stoi(tokens.at(i + 1));
            }

            else if (tokens.at(i) == "btime" && !board.white)
            {
                timeLeft = stoi(tokens.at(i + 1));
            }

            else if (tokens.at(i) == "infinite")
            {
                maxDepth = 100;
                useTimer = false;
            }

            else if (tokens.at(i) == "depth")
            {
                maxDepth = stoi(tokens.at(i + 1));
                useTimer = false;
            }

            else if (tokens.at(i) == "ponder")
            {
                // start pondering
                useTimer = false;
                pondering = true;
            }
        }

        if (timeControl == 0 || timeLeft > timeControl)
        {
            timeControl = timeLeft;
        }

        if (searcher.joinable() && !ponderHit)
            searcher.join();

        if (timer.joinable())
            timer.join();

        // start new searcher thread if ponder move not played
        if (!ponderHit)
            searcher = thread(search, board, maxDepth, hash);

        if (useTimer)
        {
            timer = thread(timeman, timeControl, timeLeft);
        }

        ponderHit = false;
    }

    void ponderhit()
    {
        ponderHit = true;
        pondering = false;

        vector<string> tokens;

        // resume normal search
        go(tokens);
    }

    void ucinewgame()
    {
        board.Forsyth("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

        timeControl = 0;
        timeLeft = 0;

        searching = false;
        pondering = false;
        ponderHit = false;

        if (searcher.joinable())
            searcher.join();

        if (timer.joinable())
            timer.join();
    }

    void position(vector<string> tokens)
    {
        ponderHit = false;
        searching = false;
        pondering = false;

        if (tokens.at(1) == "startpos")
        {
            board.Forsyth("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

            if (tokens.size() > 3)
            {
                if (tokens.at(2) == "moves")
                {
                    for (unsigned int i = 3; i < tokens.size(); i++)
                    {
                        thc::Move move;
                        move.TerseIn(&board, tokens.at(i).c_str());

                        hash = board.Hash64Update(hash, move);

                        board.PlayMove(move);
                    }
                }
            }
        }

        else if (tokens.at(1) == "fen")
        {
            string fen;

            for (unsigned int i = 2; i < tokens.size(); i++)
            {
                if (tokens.at(i) == "moves")
                    break;

                fen += tokens.at(i) + " ";
            }

            board.Forsyth(fen.c_str());

            if (tokens.size() > 8)
            {
                for (int i = 7; i < tokens.size(); i++)
                {
                    thc::Move move;
                    move.TerseIn(&board, tokens.at(i).c_str());

                    hash = board.Hash64Update(hash, move);

                    board.PlayMove(move);
                }
            }
        }
    }

    void loop()
    {
        string input;

        vector<string> tokens;

        board.Forsyth("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

        while (true)
        {
            getline(cin, input);

            tokens = split(input, " ");

            if (input == "uci")
                cout << "id name Seggz\nid author markku and daniel\nuciok" << endl;

            else if (input == "isready")
                cout << "readyok" << endl;

            else if (input == "quit")
                exit(0);

            else if (input == "stop")
                stop();

            else if (tokens.at(0) == "position")
                position(tokens);

            else if (input == "ucinewgame")
                ucinewgame();

            else if (tokens.at(0) == "go")
                go(tokens);

            else if (tokens.at(0) == "ponderhit")
                ponderhit();
        }
    }
} // namespace uci