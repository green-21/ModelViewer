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
#include "ResourceManager.h"
#include "Timer.h"

class ApplicationBase {
public:
    ApplicationBase(const int width, const int height)
        : quitRequest(false), screenWidth(width), screenHeight(height) {}

    int Run();

protected:
    virtual int Init() = 0;
    virtual int Load() = 0;
    virtual void Update(float dt) = 0;
    virtual void Draw() = 0;

    void Exit() { this->quitRequest = true; }

private:
    int init();
    int load();

protected:
    int screenWidth, screenHeight;
    WindowsWindow window;
    std::shared_ptr<MessageHandler> msgHandler;
    std::shared_ptr<GraphicsDevice> device;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<ResourceManager> resourceManager;
    Timer timer;

private:
    bool quitRequest;
};