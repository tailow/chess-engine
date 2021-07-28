#include "uci.h"

#include <iostream>
#include <chrono>

using namespace std;

void timeman(int searchTime)
{
    uci::searching = true;

    typedef std::chrono::high_resolution_clock Time;

    auto startTime = Time::now();

    while (uci::searching)
    {
        auto stopTime = Time::now();

        chrono::duration<float> duration = stopTime - startTime;

        duration *= 1000;

        if (duration.count() > searchTime)
        {
            uci::searching = false;
        }
    }
}