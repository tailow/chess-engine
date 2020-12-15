#pragma once

#include "../lib/thc/thc.h"
#include <cstdint>

void search(thc::ChessRules board, short int maxDepth, uint64_t hash);