#include "MessageHandler.h"
#include "WindowsWindow.h"
#include <iostream>
#include <memory>

#include "GraphicsDevice.h"
#include "Renderer.h"

int main() {
    constexpr int width = 800, height = 600;
    bool quitRequest = 0;
    WindowsWindow window;

    auto msgHandler = std::make_shared<MessageHandler>(quitRequest);
    auto graphicsDevice = std::make_unique<GraphicsDevice>();

    window.Init(msgHandler, width, height);

    auto renderer = graphicsDevice->InitAndCreateRenderer(window.GetHandle(),
                                                          width, height);
    window.Show();
    while (!quitRequest) {
        window.PumpMessage();

        renderer->Present();
    }

    return 0;
}