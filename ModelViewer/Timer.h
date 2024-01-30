#pragma once

#include <Windows.h>

// Reference: Luna 12 4.4
class Timer {
public:
    Timer();

    float GameTime() const;
    float DeltaTime() const;
    float TotalTime() const;

    void Reset();
    void Start();
    void Stop();
    void Tick();

private:
    __int64 getTime() {
        __int64 curr;
        QueryPerformanceCounter((LARGE_INTEGER *)&curr);
        return curr;
    }
    __int64 getFreq() {
        __int64 curr;
        QueryPerformanceFrequency((LARGE_INTEGER *)&curr);
        return curr;
    }

private:
    double countRate;
    double deltaTime;

    __int64 baseTime;
    __int64 pauseTime;
    __int64 stopTime;
    __int64 prevTime;
    __int64 currTime;

    bool stopped;
};