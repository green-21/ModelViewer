#include "ApplicationBase.h"

int ApplicationBase::Run() {
    if (init())
        return -1;

    if (load())
        return -1;

    timer.Reset();

    while (!quitRequest) {
        window.PumpMessage();
        timer.Tick();
        Update(timer.DeltaTime());
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

    device = std::make_shared<GraphicsDevice>();
    renderer = device->InitAndCreateRenderer(window.GetHandle(), screenWidth,
                                             screenHeight);

    resourceManager = std::make_unique<ResourceManager>(device);

    if (!device || !renderer || !resourceManager)
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