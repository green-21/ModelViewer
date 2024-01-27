#pragma once

#include "ApplicationBase.h"

class MainApp : public ApplicationBase {
public:
    MainApp();

protected:
    virtual int Init() override;
    virtual int Load() override;
    virtual void Update() override;
    virtual void Draw() override;

private:
    void initBuffer();
    void createPSO();


private:
    GraphicsPipelineStateObject pso;
    GraphicsBuffer triangle;
};