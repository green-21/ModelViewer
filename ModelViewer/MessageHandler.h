#pragma once
#include <array>
#include <Windows.h>
#include <iostream>

class MessageHandler {
public:
    MessageHandler(bool& quitRequest) : quitRequest(quitRequest), keyMap() {}
    void OnQuit() { quitRequest = 1; }
    void OnKeyDown(int key) { keyMap[key] = true; }
    void OnKeyUp(int key) { keyMap[key] = false; }

private:
    bool& quitRequest;
    std::array<int, 223> keyMap;
};