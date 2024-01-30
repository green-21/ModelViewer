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
    virtual void Update(float dt) override;
    virtual void Draw() override;

private:
    void loadBox();
    void loadGrid();
    void loadModel();
    void createPSO();
    void defaultUpdate(Model &model);


private:
    GraphicsPipelineStateObject defaultPSO;
    GraphicsPipelineStateObject axisPSO;
    GraphicsBuffer triangle;
    Model boxModel;
    Model duckModel;
    Model gridModel;
    UI ui;
    Camera camera;
};