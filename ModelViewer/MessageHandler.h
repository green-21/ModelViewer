#pragma once
#include <Windows.h>
#include <array>
#include <iostream>

#include "KeyCode.h"

class MessageHandler {
public:
    MessageHandler(bool &quitRequest)
        : quitRequest(quitRequest), keyMap(), mouseX(0), mouseY(0),
          isLeftMousePress(false), isRightMousePress(false), isDragStart(false) {}

    void OnQuit() { quitRequest = 1; }

    void OnKeyDown(int key) { keyMap[key] = true; }
    void OnKeyUp(int key) { keyMap[key] = false; }
    bool IsKeyPress(KeyCode key) { return keyMap[int(key)]; }

    void OnMouseMove(int x, int y) {
        mouseX = x;
        mouseY = y;
    }

    void OnLeftMouseDown() { 
        if (!isLeftMousePress) {
            isDragStart = true;
        }
        isLeftMousePress = true; 
    }
    void OnLeftMouseUp() { isLeftMousePress = false; }
    
    void OnRightMouseDown() { isRightMousePress = true; }
    void OnRightMouseUp() { isRightMousePress = false; }

    bool IsLeftMousePress() { return isLeftMousePress; }
    bool IsRightMousePress() { return isRightMousePress; }
    bool IsDragStart() { 
        bool drag = isDragStart;
        isDragStart = false;
        return drag;
    }

    int GetMousePosX() { return mouseX; }
    int GetMousePosY() { return mouseY; }

private:
    bool &quitRequest;
    std::array<int, 255> keyMap;

    int mouseX, mouseY;
    bool isLeftMousePress, isRightMousePress, isDragStart;
};
