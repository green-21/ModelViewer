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
    void loadGrid();
    void createPSO();

    void cameraUpdate(float dt);


private:
    GraphicsPipelineStateObject defaultPSO;
    GraphicsPipelineStateObject axisPSO;

    ModelObject gridModel;
    std::vector<ModelObject> models;
    UI ui;
    Camera camera;
};