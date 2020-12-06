#include <iostream>
#include <string>

void loop()
{
    while (true)
    {
        std::string input;

        std::cin >> input;

        if (input == "uci")
        {
            std::cout << "id name Seggz\nid author markku ja daniel" << std::endl;
            std::cout << "uciok" << std::endl;
        }

        else if (input == "quit")
        {
            break;
        }

        else if (input == "isready")
        {
            std::cout << "readyok" << std::endl;
        }

        else if (input[0] == 'g' && input[1] == 'o')
        {
            std::cout << "bestmove e2e4" << std::endl;
        }
    }
}