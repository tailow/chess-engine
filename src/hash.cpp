#include <cstdint>
#include <array>
#include <random>
#include <iostream>

#include "../lib/thc/thc.h"

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

    int previousEnPassantFile = -1;

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

    uint64_t generateHash(thc::ChessRules &board)
    {
        uint64_t hash = 0;

        for (int i = 0; i < 64; i++)
        {
            switch (board.squares[i])
            {
            case 'P':
            {
                hash ^= whitePawn[i];
                break;
            }
            case 'N':
            {
                hash ^= whiteKnight[i];
                break;
            }
            case 'B':
            {
                hash ^= whiteBishop[i];
                break;
            }
            case 'R':
            {
                hash ^= whiteRook[i];
                break;
            }
            case 'Q':
            {
                hash ^= whiteQueen[i];
                break;
            }
            case 'K':
            {
                hash ^= whiteKing[i];
                break;
            }
            case 'p':
            {
                hash ^= blackPawn[i];
                break;
            }
            case 'n':
            {
                hash ^= blackKnight[i];
                break;
            }
            case 'b':
            {
                hash ^= blackBishop[i];
                break;
            }
            case 'r':
            {
                hash ^= blackRook[i];
                break;
            }
            case 'q':
            {
                hash ^= blackQueen[i];
                break;
            }
            case 'k':
            {
                hash ^= blackKing[i];
                break;
            }
            }
        }

        if (board.enpassant_target != 64)
        {
            hash ^= enPassantFile[board.enpassant_target % 8];
        }

        if (board.wking == 1)
        {
            hash ^= whiteShortCastle;
        }

        if (board.wqueen == 1)
        {
            hash ^= whiteLongCastle;
        }

        if (board.bking == 1)
        {
            hash ^= blackShortCastle;
        }

        if (board.bqueen == 1)
        {
            hash ^= blackLongCastle;
        }

        if (!board.white)
        {
            hash ^= blackToMove;
        }

        return hash;
    }

    void updateHash(uint64_t &hash, thc::ChessRules &board, thc::Move &move)
    {
        hash ^= blackToMove;

        if (previousEnPassantFile != -1)
        {
            hash ^= enPassantFile[previousEnPassantFile];
        }

        previousEnPassantFile = -1;

        // castle
        if (move.special == thc::SPECIAL_WK_CASTLING)
        {
            hash ^= whiteRook[63];
            hash ^= whiteRook[61];

            hash ^= whiteShortCastle;
            hash ^= whiteLongCastle;
        }

        else if (move.special == thc::SPECIAL_WQ_CASTLING)
        {
            hash ^= whiteRook[56];
            hash ^= whiteRook[59];

            hash ^= whiteShortCastle;
            hash ^= whiteLongCastle;
        }

        else if (move.special == thc::SPECIAL_BK_CASTLING)
        {
            hash ^= blackRook[7];
            hash ^= blackRook[5];

            hash ^= blackShortCastle;
            hash ^= blackLongCastle;
        }

        else if (move.special == thc::SPECIAL_BQ_CASTLING)
        {
            hash ^= blackRook[0];
            hash ^= blackRook[3];

            hash ^= blackShortCastle;
            hash ^= blackLongCastle;
        }

        // king move
        else if (move.special == thc::SPECIAL_KING_MOVE)
        {
            if (board.white && board.wking_allowed())
            {
                hash ^= whiteShortCastle;
                hash ^= whiteLongCastle;
            }

            else if (!board.white && board.bking_allowed())
            {
                hash ^= blackShortCastle;
                hash ^= blackLongCastle;
            }
        }

        switch (board.squares[move.src])
        {
        case 'P':
        {
            hash ^= whitePawn[move.src];

            if (move.special == thc::NOT_SPECIAL)
            {
                hash ^= whitePawn[move.dst];
            }

            // valid en passant
            else if (move.special == thc::SPECIAL_WPAWN_2SQUARES)
            {
                hash ^= enPassantFile[move.dst % 8];
                hash ^= whitePawn[move.dst];

                previousEnPassantFile = move.dst % 8;
            }

            // en passant
            else if (move.special == thc::SPECIAL_WEN_PASSANT)
            {
                hash ^= blackPawn[move.dst + 8];
                hash ^= whitePawn[move.dst];
            }

            // promotion
            else if (move.special == thc::SPECIAL_PROMOTION_QUEEN)
            {
                hash ^= whiteQueen[move.dst];
            }

            else if (move.special == thc::SPECIAL_PROMOTION_KNIGHT)
            {
                hash ^= whiteKnight[move.dst];
            }

            else if (move.special == thc::SPECIAL_PROMOTION_BISHOP)
            {
                hash ^= whiteBishop[move.dst];
            }

            else if (move.special == thc::SPECIAL_PROMOTION_ROOK)
            {
                hash ^= whiteRook[move.dst];
            }

            break;
        }
        case 'N':
        {
            hash ^= whiteKnight[move.src];
            hash ^= whiteKnight[move.dst];
            break;
        }
        case 'B':
        {
            hash ^= whiteBishop[move.src];
            hash ^= whiteBishop[move.dst];
            break;
        }
        case 'R':
        {
            hash ^= whiteRook[move.src];
            hash ^= whiteRook[move.dst];
            break;
        }
        case 'Q':
        {
            hash ^= whiteQueen[move.src];
            hash ^= whiteQueen[move.dst];
            break;
        }
        case 'K':
        {
            hash ^= whiteKing[move.src];
            hash ^= whiteKing[move.dst];
            break;
        }
        case 'p':
        {
            hash ^= blackPawn[move.src];

            if (move.special == thc::NOT_SPECIAL)
            {
                hash ^= blackPawn[move.dst];
            }

            // valid en passant
            else if (move.special == thc::SPECIAL_BPAWN_2SQUARES)
            {
                hash ^= enPassantFile[move.dst % 8];
                hash ^= blackPawn[move.dst];

                previousEnPassantFile = move.dst % 8;
            }

            // en passant
            else if (move.special == thc::SPECIAL_BEN_PASSANT)
            {
                hash ^= whitePawn[move.dst - 8];
                hash ^= blackPawn[move.dst];
            }

            // promotion
            else if (move.special == thc::SPECIAL_PROMOTION_QUEEN)
            {
                hash ^= blackQueen[move.dst];
            }

            else if (move.special == thc::SPECIAL_PROMOTION_KNIGHT)
            {
                hash ^= blackKnight[move.dst];
            }

            else if (move.special == thc::SPECIAL_PROMOTION_BISHOP)
            {
                hash ^= blackBishop[move.dst];
            }

            else if (move.special == thc::SPECIAL_PROMOTION_ROOK)
            {
                hash ^= blackRook[move.dst];
            }

            break;
        }
        case 'n':
        {
            hash ^= blackKnight[move.src];
            hash ^= blackKnight[move.dst];
            break;
        }
        case 'b':
        {
            hash ^= blackBishop[move.src];
            hash ^= blackBishop[move.dst];
            break;
        }
        case 'r':
        {
            hash ^= blackRook[move.src];
            hash ^= blackRook[move.dst];
            break;
        }
        case 'q':
        {
            hash ^= blackQueen[move.src];
            hash ^= blackQueen[move.dst];
            break;
        }
        case 'k':
        {
            hash ^= blackKing[move.src];
            hash ^= blackKing[move.dst];

            break;
        }
        }

        // capture
        if (board.squares[move.dst] != ' ')
        {
            switch (board.squares[move.dst])
            {
            case 'P':
            {
                hash ^= whitePawn[move.dst];

                break;
            }
            case 'N':
            {
                hash ^= whiteKnight[move.dst];
                break;
            }
            case 'B':
            {
                hash ^= whiteBishop[move.dst];
                break;
            }
            case 'R':
            {
                hash ^= whiteRook[move.dst];
                break;
            }
            case 'Q':
            {
                hash ^= whiteQueen[move.dst];
                break;
            }
            case 'K':
            {
                hash ^= whiteKing[move.dst];
                break;
            }
            case 'p':
            {
                hash ^= blackPawn[move.dst];
                break;
            }
            case 'n':
            {
                hash ^= blackKnight[move.dst];
                break;
            }
            case 'b':
            {
                hash ^= blackBishop[move.dst];
                break;
            }
            case 'r':
            {
                hash ^= blackRook[move.dst];
                break;
            }
            case 'q':
            {
                hash ^= blackQueen[move.dst];
                break;
            }
            case 'k':
            {
                hash ^= blackKing[move.dst];
                break;
            }
            }
        }
    }
}