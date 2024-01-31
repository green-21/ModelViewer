#include <iostream>

#include "WindowsWindow.h"
#include "imgui.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd,
                                                             UINT msg,
                                                             WPARAM wParam,
                                                             LPARAM lParam);
namespace {
WindowsWindow *win = nullptr;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    return win->MsgProc(hWnd, msg, wParam, lParam);
}

WindowsWindow::WindowsWindow() : window(nullptr) { win = this; }

WindowsWindow::~WindowsWindow() {
    if (window)
        ::DestroyWindow(this->window);
}

int WindowsWindow::Init(std::shared_ptr<MessageHandler> handler, int width,
                        int height) {
    if (window) {
        std::cerr << "The window has already been initialized." << std::endl;
        return -1;
    }

    WNDCLASSEX wc{};
    wc.cbClsExtra = 0L;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbWndExtra = 0L;
    wc.hbrBackground = nullptr;
    wc.hCursor = nullptr;
    wc.hIcon = nullptr;
    wc.hIconSm = nullptr;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpfnWndProc = WinProc;
    wc.lpszClassName = L"dxWindow";
    wc.lpszMenuName;
    wc.style = CS_CLASSDC;

    if (!RegisterClassEx(&wc)) {
        std::cerr << "failed to register classEx." << std::endl;
        return -1;
    }
    DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

    RECT wr = {0, 0, width, height};
    AdjustWindowRect(&wr, style, 0);

    this->window = CreateWindow(wc.lpszClassName, L"dxWindow", style, 500, 100,
                                wr.right - wr.left, wr.bottom - wr.top, nullptr,
                                nullptr, wc.hInstance, nullptr);
    if (!window) {
        std::cerr << "failed to create window" << std::endl;
        return -1;
    }

    this->msgHandler = handler;

    return 0;
}

int WindowsWindow::Show() {
    if (!window)
        return -1;

    ShowWindow(this->window, SW_SHOWDEFAULT);
    UpdateWindow(this->window);

    return 0;
}

int WindowsWindow::Close() {
    if (!window)
        return -1;

    DestroyWindow(this->window);
    return 0;
}

LRESULT WindowsWindow::MsgProc(HWND hWnd, UINT msg, WPARAM wParam,
                               LPARAM lParam) {

    // imGUI를 사용하기 위해서는 다음과 같은 방법을 이용해야하는데, 굉장히
    // 마음에 들지 않는다. 해결할 방법을 찾아야 한다.
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
        return true;
    }

    switch (msg) {
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) {
            return 0;
        }
        break;

    case WM_MOUSEMOVE:
        msgHandler->OnMouseMove(LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_LBUTTONDOWN:
        msgHandler->OnLeftMouseDown();
        break;
    case WM_LBUTTONUP:
        msgHandler->OnLeftMouseUp();
        break;

    case WM_RBUTTONDOWN:
        msgHandler->OnRightMouseDown();
        break;
    case WM_RBUTTONUP:
        msgHandler->OnRightMouseUp();
        break;
    case WM_KEYDOWN:
        this->msgHandler->OnKeyDown(static_cast<int>(wParam));
        break;
    case WM_KEYUP:
        this->msgHandler->OnKeyUp(static_cast<int>(wParam));
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        this->msgHandler->OnQuit();
        break;
    case WM_QUIT:
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void WindowsWindow::PumpMessage() {
    MSG msg{};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}