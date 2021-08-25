// https://github.com/geaz/emu-chip-8

#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>

enum TimerType { CHIP, TIMER };

class Clock
{
public:
    void Reset()
    {
        lastCycleChip = -1;
        lastCycleTimer = -1;
    }

    int CatchUpCycles(int frequency, TimerType timerType)
    {
        auto now = std::chrono::system_clock::now();
        long long currentClockMs = std::chrono::time_point_cast<std::chrono::milliseconds>(now).time_since_epoch().count();

        long long* lastTimer = timerType == CHIP
            ? &lastCycleChip
            : &lastCycleTimer;

        if (*lastTimer == -1) *lastTimer = currentClockMs;
        int cycles = (int)(currentClockMs - *lastTimer) * frequency / 1000;
        if (cycles > 0) *lastTimer = currentClockMs;
        return cycles;
    }

private:
    long long lastCycleTimer = -1;
    long long lastCycleChip = -1;
};

#endif