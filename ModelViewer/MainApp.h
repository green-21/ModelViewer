#pragma once

#include "ApplicationBase.h"
#include "UI.h"
#include "Model.h"
#include "Camera.h"

class MainApp : public ApplicationBase {
public:
    MainApp(int width, int height);

protected:
    virtual int Init() override;
    virtual int Load() override;
    virtual void Update() override;
    virtual void Draw() override;

private:
    void loadBox();
    void createPSO();


private:
    GraphicsPipelineStateObject pso;
    GraphicsBuffer triangle;
    Model model;
    UI ui;
    Camera camera;
};