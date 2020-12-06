#include <iostream>
#include <string>
#include "../lib/thc/thc.h"

using namespace std;

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

void loop()
{
    thc::ChessRules rules;

    while (true)
    {
        string input;

        getline(cin, input);

        if (input == "uci")
        {
            cout << "id name Seggz\nid author markku ja daniel" << endl;
            cout << "uciok" << endl;
        }

        else if (input == "quit")
        {
            break;
        }

        else if (input == "isready")
        {
            cout << "readyok" << endl;
        }

        else if (split(input, " ").at(0) == "go")
        {
            thc::Move move;

            vector<thc::Move> legalMoves;

            rules.GenLegalMoveList(legalMoves);

            for (int i = 0; i < legalMoves.size(); i++)
            {
                cout << legalMoves.at(i).NaturalOut(&rules) << endl;
            }

            move = legalMoves.at(rand() % legalMoves.size() + 0);

            printf(("bestmove " + move.TerseOut() + "\n").c_str());
        }

        else if (split(input, " ").at(0) == "position")
        {
            vector<string> strings = split(input, " ");

            if (strings.at(1) == "startpos")
            {
                rules.Forsyth("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

                if (strings.size() > 3)
                {
                    if (strings.at(2) == "moves")
                    {
                        for (int i = 3; i < strings.size(); i++)
                        {
                            thc::Move move;
                            move.TerseIn(&rules, strings.at(i).c_str());

                            rules.PlayMove(move);
                        }
                    }

                    else
                    {
                        string fen;

                        for (int i = 1; i < strings.size(); i++)
                        {
                            fen += strings.at(i) + " ";
                        }

                        rules.Forsyth(fen.c_str());
                    }
                }
            }
        }
    }
}
