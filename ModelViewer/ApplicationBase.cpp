#include "ApplicationBase.h"

int ApplicationBase::Run() {
    if (init())
        return -1;

    if (load())
        return -1;

    while (!quitRequest) {
        window.PumpMessage();

        Update();
        Draw();

        renderer->Present();
    }

    if (window.GetHandle())
        window.Close();

    return 0;
}

int ApplicationBase::init() {
    msgHandler = std::make_shared<MessageHandler>(quitRequest);

    if (window.Init(msgHandler, screenWidth, screenHeight))
        return -1;
    window.Show();

    device = std::make_unique<GraphicsDevice>();
    renderer = device->InitAndCreateRenderer(window.GetHandle(), screenWidth,
                                             screenHeight);

    if (!device || !renderer)
        return -1;

    if (Init())
        return -1;

    return 0;
}

int ApplicationBase::load() {
    if (Load())
        return -1;
    return 0;
}