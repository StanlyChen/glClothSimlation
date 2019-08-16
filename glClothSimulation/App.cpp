#include "App.h"
#include "ClothPhysical.h"
#include "RenderSystem.h"
#include "SceneManager.h"
#include "ViewManipulator.h"
#include "StaticModels.h"

App::App(GLFWwindow* pWindow)
    :m_pWindow(pWindow)
{
}


App::~App()
{
}

void App::init()
{
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    m_pRenderSystem = new RenderSystem(m_pWindow);
    m_pRenderSystem->initializeGL();

    m_pScenceManager = new SceneManager(m_pRenderSystem);
    m_staticModels = new StaticModels(m_pScenceManager);
    m_pSimulator = new ClothPhysical(m_pScenceManager, m_staticModels);


    m_pViewManip = new ViewManipulator(m_pRenderSystem, m_staticModels);
    m_staticModels->loadModel(R"PATH(G:\Projs\gui\glClothSimulation\glClothSimulation\models\Sphere.stl)PATH");
    m_staticModels->buildOctree();
    m_staticModels->drawOctree();

    m_pSimulator->init();

    m_pViewManip->fitview();
}

void App::update()
{
    static double last_time = glfwGetTime();
    double time = glfwGetTime();
    if(m_pSimulator)
        m_pSimulator->update(REAL(time - last_time));
    last_time = time;
    m_pRenderSystem->paintGL();
}

void App::destory()
{

}

void App::resize(int w, int h)
{
    m_pRenderSystem->resizeGL(w, h);
    m_pViewManip->resize(w, h);
}

void App::handleKeyInput(int button, int scancode, int action, int mods)
{
 
}

void App::handleMouseInput(int button, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        m_mouseStatus.onMouseButtonPress(button);

        if (m_mouseStatus.isLeftRightDown())
            m_pViewManip->fitview();
    }
    else if (action == GLFW_RELEASE)
    {
        m_mouseStatus.onMouseButtonRelease(button);
    }
}

void App::handleMouseMove(double xpos, double ypos)
{
    m_mouseStatus.setPosition(xpos, ypos);

    if (m_mouseStatus.isMiddleDown())
    {
        MousePosition delta = m_mouseStatus.getLastDeltaPosition();
        float pitchDegree = 720.0f / m_pRenderSystem->height() * (float)delta.y;
        float yawDegree = 720.0f / m_pRenderSystem->width() * (float)delta.x;
        m_pViewManip->rotate(pitchDegree, yawDegree);
    }
    else if (m_mouseStatus.isRightDown())
    {
        MousePosition delta = m_mouseStatus.getLastDeltaPosition();
        m_pViewManip->pan((int)delta.x, -(int)delta.y);
    }
}

void App::handleMouseScroll(double x, double y)
{
    if (y > 0) // roll up, zoom in
    {
        m_pViewManip->zoomin();
    }
    else if (y < 0) // roll down, zoom out
    {
        m_pViewManip->zoomout();
    }
}
