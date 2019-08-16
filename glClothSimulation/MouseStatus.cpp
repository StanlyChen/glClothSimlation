#include "MouseStatus.h"
#include "GLFW/glfw3.h"

    void MouseStatus::onMouseButtonPress(int btn)
    {
        switch (btn)
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            m_bLeftDown = true;
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            m_bRightDown = true;
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            m_bMiddleDown = true;
            break;
        default:
            break;
        }
    }

    void MouseStatus::onMouseButtonRelease(int btn)
    {
        switch (btn)
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            m_bLeftDown = false;
            break;
        case  GLFW_MOUSE_BUTTON_RIGHT:
            m_bRightDown = false;
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            m_bMiddleDown = false;
            break;
        default:
            break;
        }
    }

    void MouseStatus::setPosition(double x, double y)
    {
        m_mousePosQueue.push_back(MousePosition{ x,y });
    }

    MousePosition MouseStatus::getLastDeltaPosition()
    {
        auto it1 = m_mousePosQueue.rbegin();
        auto it2 = it1+1;
        if (it1 != m_mousePosQueue.rend() && it2 != m_mousePosQueue.rend())
        {
            auto ret = MousePosition{ it1->x - it2->x,
                             it1->y - it2->y };
            return ret;
        }
        return MousePosition{ 0,0 };
    }


