#pragma once
#include "RenderingTypes.h"

class RenderSystem;
class StaticModels;

class ViewManipulator
{
public:
    ViewManipulator(RenderSystem* pRenderSystem,  StaticModels* modelsManager);
    ~ViewManipulator();


public:
    void fitview();
    void pan(int x, int y);
    void zoomin();
    void zoomout();
    void rotate(float pitch, float yaw);
    void resize(int w, int h);

private:
    void updateProjMatrix();
    void updateViewMatrix();
    float _getFOV();
public:
    float m_yaw;
    float m_pitch;
    float m_R;

    float m_scale = 1;
    float m_width = 1;
    float m_height = 1;

    float m_panX = 0;
    float m_panY = 0;

    RenderSystem* m_pRenderingWindow = nullptr;
    StaticModels* m_modelsManager = nullptr;

};

