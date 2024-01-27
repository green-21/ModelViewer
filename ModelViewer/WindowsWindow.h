#pragma once

#include <memory>
#include <Windows.h>

#include "MessageHandler.h"

class WindowsWindow {
public:
    WindowsWindow();
    ~WindowsWindow();
    int Init(std::shared_ptr<MessageHandler> handler, int width, int height);
    int Show();
    int Close();
    HWND GetHandle() { return window; };
    LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam,
        LPARAM lParam);

    void PumpMessage();

private:
    HWND window;
    std::shared_ptr<MessageHandler> msgHandler;
};