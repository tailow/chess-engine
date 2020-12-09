#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>

#include "../lib/thc/thc.h"
#include "search.h"
#include "evaluate.h"
#include "timeman.h"

using namespace std;

thc::ChessRules board;

int maxDepth = 20;

int timeLeft = 60000;
int timeControl = 0;

bool searching = false;
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
}

void go(vector<string> tokens)
{
    useTimer = true;

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
            maxDepth = 1000000;
            useTimer = false;
        }
    }

    if (timeControl == 0)
    {
        timeControl = timeLeft;
    }

    if (searcher.joinable())
        searcher.join();

    if (timer.joinable())
        timer.join();

    searcher = thread(search, board, maxDepth);

    if (useTimer)
    {
        timer = thread(timeman, timeControl, timeLeft);
    }
}

void ucinewgame()
{
    board.Forsyth("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    timeControl = 0;
    timeLeft = 0;
}

void position(vector<string> tokens)
{
    if (tokens.at(1) == "startpos")
    {
        board.Forsyth("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }

    if (tokens.size() > 3)
    {
        if (tokens.at(2) == "moves")
        {
            for (unsigned int i = 3; i < tokens.size(); i++)
            {
                thc::Move move;
                move.TerseIn(&board, tokens.at(i).c_str());

                board.PlayMove(move);
            }
        }

        else
        {
            string fen;

            for (unsigned int i = 1; i < tokens.size(); i++)
            {
                fen += tokens.at(i) + " ";
            }

            board.Forsyth(fen.c_str());
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
            cout << "id name Seggz\nid author markku ja daniel\nuciok" << endl;

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
    }
}
