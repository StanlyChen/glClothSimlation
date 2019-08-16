#include "ViewManipulator.h"
#include <gtc/matrix_transform.hpp>

#include "RenderSystem.h"
#include "StaticModels.h"


ViewManipulator::ViewManipulator(RenderSystem* pRenderSystem, StaticModels* modelsManager)
    : m_pRenderingWindow(pRenderSystem)
    , m_modelsManager(modelsManager)
{
    m_width = (float)pRenderSystem->width();
    m_height = (float)pRenderSystem->height();
}


ViewManipulator::~ViewManipulator()
{
}


void ViewManipulator::fitview()
{
    m_scale = 1;
    m_panX = 0;
    m_panY = 0;

    AABB box = m_modelsManager->getBoundingBox();
    float diagonal = box.diagonalLength();
    m_R = diagonal == 0 ? 1 : diagonal;

    m_pitch = glm::radians(45.0f);
    m_yaw = 0;

  
    updateViewMatrix();
    updateProjMatrix();
}

void ViewManipulator::rotate(float pitch, float yaw)
{
    float yawDegree = yaw * 360.0f / m_width;
    float pitchDegree = pitch * 180.0f / m_height;

    m_pitch += glm::radians(pitchDegree);
    m_yaw += glm::radians(yawDegree);

    //limit pitch in [0, 89], use the above semisphere
    if (m_pitch < 0)
        m_pitch = 0;
    if (m_pitch > glm::radians(89.f))
        m_pitch = glm::radians(89.f);

    updateViewMatrix();
}

//x,y are screen coorindate
void ViewManipulator::pan(int x, int y)
{
    m_panX += x / m_width * 2 * m_R *tan(_getFOV() / 2);
    m_panY += y / m_height * 2 * m_R *tan(_getFOV() / 2);

    updateViewMatrix();
}

void ViewManipulator::zoomin()
{
    m_scale *= 1.1f;
    updateProjMatrix();

}

void ViewManipulator::zoomout()
{
    m_scale /= 1.1f;
    updateProjMatrix();

}

void ViewManipulator::updateViewMatrix()
{

    vect3 position { m_R*cos(m_pitch)*cos(m_yaw),  m_R*sin(m_pitch), m_R*cos(m_pitch)*sin(m_yaw) };

    matrix4 matrix = glm::lookAt(position, vect3{ 0,0,0 }, vect3{ 0, 1, 0 });
    m_pRenderingWindow->setViewMatrix(matrix);

    //glm::mat4 projMatrix = m_pRenderingWindow->getProjMatrix();
    //glm::vec3 localTrans = glm::vec3(m_panX / projMatrix[0][0], m_panY / projMatrix[1][1], 0);
    //glm::mat4 viewMatrix = glm::translate(glm::mat4(), localTrans);

    //m_pRenderingWindow->setViewMatrix(viewMatrix*matrix);
}

void ViewManipulator::updateProjMatrix()
{
    //keep the object-camera distance not change, we just act as a magnifier
    float fov = _getFOV();
    auto projectMatrix = glm::perspectiveFov(fov, m_width, m_height, 0.05f, 1000.f);
   
    m_pRenderingWindow->setProjMatrix(projectMatrix);
}

float ViewManipulator::_getFOV()
{
    float fov = glm::radians(45.0f*m_scale);
    if (fov > glm::radians(179.f))
        fov = glm::radians(179.f);
    return fov;
}

void ViewManipulator::resize(int w, int h)
{
    m_width = float(w);
    m_height = float(h);
    updateProjMatrix();
}