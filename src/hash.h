#include <cstdint>
#include "../lib/thc/thc.h"

namespace hsh
{
    uint64_t getRandomNumber(uint32_t &seed);

    uint64_t generateHash(thc::ChessRules &board);

    void generateRandomNumbers();

    uint64_t updateHash(uint64_t hash, thc::ChessRules &board, thc::Move &move);
}