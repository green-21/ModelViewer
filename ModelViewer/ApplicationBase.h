#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "MessageHandler.h"
#include "WindowsWindow.h"

#include "GraphicsDevice.h"
#include "GraphicsPipelineStateObject.h"
#include "Renderer.h"
#include "ShaderCompiler.h"

class ApplicationBase {
public:
    ApplicationBase(const int width, const int height)
        : quitRequest(false), screenWidth(width), screenHeight(height) {}

    int Run();

protected:
    virtual int Init() = 0;
    virtual int Load() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;

    void Exit() { this->quitRequest = true; }

private:
    int init();
    int load();

protected:
    int screenWidth, screenHeight;
    WindowsWindow window;
    std::shared_ptr<MessageHandler> msgHandler;
    std::unique_ptr<GraphicsDevice> device;
    std::unique_ptr<Renderer> renderer;

private:
    bool quitRequest;
};