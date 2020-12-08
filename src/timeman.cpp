#include "uci.h"

#include <iostream>
#include <chrono>

using namespace std;

void timeman(int timeControl, int timeLeft)
{
    typedef std::chrono::high_resolution_clock Time;

    auto startTime = Time::now();

    int maxTime = min(timeControl / 30, timeLeft / 2);

    while (searching)
    {
        auto stopTime = Time::now();

        chrono::duration<float> delta = stopTime - startTime;

        chrono::milliseconds duration = chrono::duration_cast<chrono::milliseconds>(delta);

        if (chrono::duration_cast<chrono::milliseconds>(duration).count() > maxTime)
        {
            searching = false;
        }
    }
}