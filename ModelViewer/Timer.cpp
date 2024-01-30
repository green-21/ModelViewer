#include <Windows.h>

#include "Timer.h"

Timer::Timer()
    : countRate(0.0), deltaTime(-1.0), baseTime(0), pauseTime(0), prevTime(0),
      currTime(0), stopped(false) {
    countRate = 1.0 / (double)getFreq();
}

float Timer::GameTime() const { return 0.0f; }

float Timer::DeltaTime() const { return float(deltaTime); }

float Timer::TotalTime() const {
    if (stopped) {
        return float(((stopTime - pauseTime) - baseTime) * countRate);
    }

    return float((((currTime - pauseTime) - baseTime) * countRate));
}

void Timer::Reset() {
    const __int64 curr = getTime();
    baseTime = curr;
    prevTime = curr;
    stopTime = curr;
    stopped = false;
}

void Timer::Start() {
    __int64 startTime = getTime();
    if (stopped) {
        pauseTime += (startTime - stopTime);

        prevTime = startTime;
        stopTime = 0;
        stopped = false;
    }
}

void Timer::Stop() {
    if (!stopped) {
        stopTime = getTime();
        stopped = true;
    }
}

void Timer::Tick() {
    if (stopped) {
        deltaTime = 0.0f;
        return;
    }

    currTime = getTime();
    deltaTime = (currTime - prevTime) * countRate;
    prevTime = currTime;

    if (deltaTime < 0.0) {
        deltaTime = 0.0;
    }
}
