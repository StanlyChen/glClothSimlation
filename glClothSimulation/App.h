#pragma once
#include "MouseStatus.h"

struct GLFWwindow;
class RenderSystem;
class ClothPhysical;
class SceneManager;
class ViewManipulator;
class StaticModels;

class App
{
public:
    App(GLFWwindow* pWindow);
    ~App();

    void init();
    void update();
    void destory();
    void resize( int w, int h);

    void handleKeyInput(int button, int scancode, int action, int mods);
    void handleMouseInput(int button, int action, int mods);
    void handleMouseMove(double xpos, double ypos);
    void handleMouseScroll(double x, double y);

private:
    RenderSystem* m_pRenderSystem;
    SceneManager* m_pScenceManager;
    ClothPhysical* m_pSimulator;
    StaticModels*    m_staticModels;

    GLFWwindow*    m_pWindow;
    ViewManipulator* m_pViewManip;
    MouseStatus      m_mouseStatus;

};

