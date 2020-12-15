#pragma once

#include "../lib/thc/thc.h"
#include <cstdint>

void search(thc::ChessRules board, uint8_t maxDepth, uint64_t hash);