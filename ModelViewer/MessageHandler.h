#pragma once
#include <Windows.h>
#include <array>
#include <iostream>

#include "KeyCode.h"

class MessageHandler {
public:
    MessageHandler(bool &quitRequest)
        : quitRequest(quitRequest), keyMap(), mouseX(0), mouseY(0),
          isLeftMousePress(false), isRightMousePress(false),
          isLeftMouseDragStart(false) {}

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
            isLeftMouseDragStart = true;
        }
        isLeftMousePress = true;
    }
    void OnLeftMouseUp() {
        isLeftMousePress = false;
        isLeftMouseDragStart = false;
    }

    void OnRightMouseDown() {
        if (!isRightMousePress) {
            isRightMouseDragStart = true;
        }
        isRightMousePress = true;
    }
    void OnRightMouseUp() { isRightMousePress = false; }

    void OnWheelMouse(int wheel) { mouseWheel = wheel; }

    bool IsLeftMousePress() { return isLeftMousePress; }
    bool IsRightMousePress() { return isRightMousePress; }

    bool IsLeftMouseDragStart() { return isLeftMouseDragStart; }
    bool IsRightMouseDragStart() { return isRightMouseDragStart; }

    void OffLeftMouseDragStart() { isLeftMouseDragStart = false; }
    void OffRightMouseDragStart() { isRightMouseDragStart = false; }

    int GetMousePosX() { return mouseX; }
    int GetMousePosY() { return mouseY; }
    
    int GetWheelMouse() { 
        int result = mouseWheel;
        mouseWheel = 0;
        return result; 
    }

    private:
    bool &quitRequest;
    std::array<int, 255> keyMap;

    int mouseX, mouseY;
    int mouseWheel = 0;
    bool isLeftMousePress, isRightMousePress;
    bool isLeftMouseDragStart, isRightMouseDragStart;
};
