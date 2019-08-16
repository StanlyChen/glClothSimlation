#pragma once

#include <boost/circular_buffer.hpp>

    struct MousePosition
    {
        double x;
        double y;
    };

    class MouseStatus
    {
    public:
        bool isLeftDown() { return m_bLeftDown && !m_bMiddleDown && !m_bRightDown; }
        bool isRightDown() { return !m_bLeftDown && !m_bMiddleDown && m_bRightDown; }
        bool isMiddleDown() { return !m_bLeftDown && m_bMiddleDown && !m_bRightDown; }

        bool isLeftRightDown() { return m_bLeftDown && m_bRightDown && !m_bMiddleDown; }

        void onMouseButtonPress( int  btn);
        void onMouseButtonRelease( int  btn);

        void setPosition(double x, double y);
        MousePosition getLastDeltaPosition();

    private:
        bool m_bLeftDown = false;
        bool m_bRightDown = false;
        bool m_bMiddleDown = false;

        boost::circular_buffer<MousePosition> m_mousePosQueue = boost::circular_buffer<MousePosition>(2);
    };


