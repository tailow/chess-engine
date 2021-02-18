#include "../lib/thc/thc.h"
#include <cstdint>
#include <array>
#include <random>

using namespace std;

namespace hsh
{
    array<uint64_t, 64> whitePawn;
    array<uint64_t, 64> whiteKnight;
    array<uint64_t, 64> whiteBishop;
    array<uint64_t, 64> whiteRook;
    array<uint64_t, 64> whiteQueen;
    array<uint64_t, 64> whiteKing;

    array<uint64_t, 64> blackPawn;
    array<uint64_t, 64> blackKnight;
    array<uint64_t, 64> blackBishop;
    array<uint64_t, 64> blackRook;
    array<uint64_t, 64> blackQueen;
    array<uint64_t, 64> blackKing;

    array<uint64_t, 8> enPassantFile;

    uint64_t whiteShortCastle;
    uint64_t whiteLongCastle;
    uint64_t blackShortCastle;
    uint64_t blackLongCastle;

    uint64_t blackToMove;

    uint64_t getRandomNumber(uint32_t seed)
    {
        mt19937_64 gen(seed);

        uint64_t randomNumber = gen();

        return randomNumber;
    }

    void generateRandomNumbers()
    {
        for (int i = 0; i < 64; i++)
        {
            whitePawn[i] = getRandomNumber(i);
            whiteKnight[i] = getRandomNumber(i + 64);
            whiteBishop[i] = getRandomNumber(i + 64 * 2);
            whiteRook[i] = getRandomNumber(i + 64 * 3);
            whiteQueen[i] = getRandomNumber(i + 64 * 4);
            whiteKing[i] = getRandomNumber(i + 64 * 5);

            blackPawn[i] = getRandomNumber(i + 64 * 6);
            blackKnight[i] = getRandomNumber(i + 64 * 7);
            blackBishop[i] = getRandomNumber(i + 64 * 8);
            blackRook[i] = getRandomNumber(i + 64 * 9);
            blackQueen[i] = getRandomNumber(i + 64 * 10);
            blackKing[i] = getRandomNumber(i + 64 * 11);
        }

        for (int i = 0; i < 8; i++)
        {
            enPassantFile[i] = getRandomNumber(i + 1000);
        }

        whiteShortCastle = getRandomNumber(2000);
        whiteLongCastle = getRandomNumber(2001);
        blackShortCastle = getRandomNumber(2002);
        blackLongCastle = getRandomNumber(2003);

        blackToMove = getRandomNumber(2004);
    }

    uint64_t generateHash(thc::ChessRules board)
    {
        return 0;
    }

    void updateHash(uint64_t *hash, thc::ChessRules board, thc::Move move)
    {
    }
}