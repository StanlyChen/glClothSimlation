#include "AABB.h"
#include <algorithm>
#include <assert.h>

AABB::AABB()
{

}


AABB::~AABB()
{
}

AABB::AABB(const vect3 p1, const vect3 p2)
{
    m_maxPt.x = std::max(p1.x, p2.x);
    m_maxPt.y = std::max(p1.y, p2.y);
    m_maxPt.z = std::max(p1.z, p2.z);

    m_minPt.x = std::min(p1.x, p2.x);
    m_minPt.y = std::min(p1.y, p2.y);
    m_minPt.z = std::min(p1.z, p2.z);
}


void AABB::merge(const vect3& point)
{
    m_maxPt.x = std::max(m_maxPt.x, point.x);
    m_maxPt.y = std::max(m_maxPt.y, point.y);
    m_maxPt.z = std::max(m_maxPt.z, point.z);

    m_minPt.x = std::min(m_minPt.x, point.x);
    m_minPt.y = std::min(m_minPt.y, point.y);
    m_minPt.z = std::min(m_minPt.z, point.z);
}

void AABB::merge(const AABB& box)
{
    merge(box.m_minPt);
    merge(box.m_maxPt);
}

vect3 AABB::center() const
{
    return (m_minPt + m_maxPt) / 2.0f;
}
vect3 AABB::minP() const
{
    return m_minPt;
}

vect3 AABB::maxP() const
{
    return m_maxPt;
}

float AABB::diagonalLength() const
{
    return glm::distance(m_minPt, m_maxPt);
}



unsigned AABB::subBoxId(const vect3& point) const
{
    unsigned ret = 0;
    auto midPt = (m_maxPt + m_minPt)*0.5f;

    if (point.x >= midPt.x)
        ret |= 0x1;
    if (point.y >= midPt.y)
        ret |= 0x2;
    if (point.z >= midPt.z)
        ret |= 0x4;
    return ret;
}

AABB AABB::subBox(unsigned i) const
{
    auto midPt = (m_maxPt + m_minPt)*0.5f;
    vect3 p0 = midPt;
    vect3 p1 = m_minPt;

    if (i & 0x1)
    {
        p1.x = m_maxPt.x;
    }
    if (i & 0x2)
    {
        p1.y = m_maxPt.y;
    }
    if (i & 0x4)
    {
        p1.z = m_maxPt.z;
    }

    return AABB(p0, p1);
}

bool AABB::intersect_segment(const vect3 p1, vect3 p2)
{
    if (p1.x > m_maxPt.x && p2.x > m_maxPt.x)
        return false;
    if (p1.y > m_maxPt.y && p2.y > m_maxPt.y)
        return false;
    if (p1.z > m_maxPt.z && p2.z > m_maxPt.z)
        return false;

    if (p1.x < m_minPt.x && p2.x < m_minPt.x)
        return false;
    if (p1.y < m_minPt.y && p2.y < m_minPt.y)
        return false;
    if (p1.z < m_minPt.z && p2.z < m_minPt.z)
        return false;

    return true;

}

bool AABB::intersect_AABB(const AABB& other)
{
    if (m_maxPt.x < other.m_minPt.x || m_minPt.x > other.m_maxPt.x)
        return false;

    if (m_maxPt.y < other.m_minPt.y || m_minPt.y > other.m_maxPt.y)
        return false;

    if (m_maxPt.z < other.m_minPt.z || m_minPt.z > other.m_maxPt.z)
        return false;

    return true;
}

void AABB::generateTesellationData(AABB* pBox, CstPointSequence& vertices, CstInt32Sequence& indices, CstPointSequence& normals)
{
    vertices.clear();
    indices.clear();
    normals.clear();
    
    auto minPt = pBox->m_minPt;
    auto maxPt = pBox->m_maxPt;

    //ccw, bottom to top
    vect3 v0, v1, v2, v3, v4, v5, v6, v7;
    

    v0 = minPt;
    
    v1 = v0; 
    v1.z = maxPt.z;

    v2 = v1;
    v2.x = maxPt.x;

    v3 = v0;
    v3.x = maxPt.x;

    v4 = v0;
    v4.y = maxPt.y;

    v5 = v1;
    v5.y = maxPt.y;

    v6 = v2;
    v6.y = maxPt.y;

    v7 = v3;
    v7.y = maxPt.y;

    vertices.reserve(8);
    vertices.push_back(v0);
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);
    vertices.push_back(v5);
    vertices.push_back(v6);
    vertices.push_back(v7);

    //bottom face
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);

    //front face
    indices.push_back(5);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(5);
    indices.push_back(2);
    indices.push_back(6);

    //left face
    indices.push_back(4);
    indices.push_back(1);
    indices.push_back(5);
    indices.push_back(4);
    indices.push_back(0);
    indices.push_back(1);

    //right face
    indices.push_back(6);
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(6);
    indices.push_back(3);
    indices.push_back(7);

    //top face
    indices.push_back(4);
    indices.push_back(5);
    indices.push_back(6);
    indices.push_back(4);
    indices.push_back(6);
    indices.push_back(7);
}

void AABB::generateTesellationData(AABB* pBox, CstPointSequence& vertices, CstInt32Sequence& indices)
{
    vertices.clear();
    indices.clear();


    auto minPt = pBox->m_minPt;
    auto maxPt = pBox->m_maxPt;

    //ccw, bottom to top
    vect3 v0, v1, v2, v3, v4, v5, v6, v7;


    v0 = minPt;

    v1 = v0;
    v1.z = maxPt.z;

    v2 = v1;
    v2.x = maxPt.x;

    v3 = v0;
    v3.x = maxPt.x;

    v4 = v0;
    v4.y = maxPt.y;

    v5 = v1;
    v5.y = maxPt.y;

    v6 = v2;
    v6.y = maxPt.y;

    v7 = v3;
    v7.y = maxPt.y;

    vertices.reserve(8);
    vertices.push_back(v0);
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);
    vertices.push_back(v5);
    vertices.push_back(v6);
    vertices.push_back(v7);

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(3);
    indices.push_back(0);

    indices.push_back(0);
    indices.push_back(4);
    indices.push_back(1);
    indices.push_back(5);
    indices.push_back(2);
    indices.push_back(6);
    indices.push_back(3);
    indices.push_back(7);

    indices.push_back(4);
    indices.push_back(5);
    indices.push_back(5);
    indices.push_back(6);
    indices.push_back(6);
    indices.push_back(7);
    indices.push_back(7);
    indices.push_back(4);
}

void AABB::test()
{
    const vect3 maxPt = vect3(5, 5, 5);
    const vect3 minPt = vect3(-5, -5, -5);

    AABB const box{ minPt, maxPt };
    assert(box.subBoxId(vect3(1, 1, 1)) == 7);
    assert(box.subBoxId(vect3(-1, 1, 1)) == 6);
    assert(box.subBoxId(vect3(1, -1, 1)) == 5);
    assert(box.subBoxId(vect3(-1, -1, 1)) == 4);
    assert(box.subBoxId(vect3(1, 1, -1)) == 3);
    assert(box.subBoxId(vect3(-1, 1, -1)) == 2);
    assert(box.subBoxId(vect3(1, -1, -1)) == 1);
    assert(box.subBoxId(vect3(-1, -1, -1)) == 0);

    assert(box.subBox(0).maxP() == vect3(0, 0, 0));
    assert(box.subBox(1).maxP() == vect3(5, 0, 0));
    assert(box.subBox(2).maxP() == vect3(0, 5, 0));
    assert(box.subBox(3).maxP() == vect3(5, 5, 0));
    assert(box.subBox(4).maxP() == vect3(0, 0, 5));
    assert(box.subBox(5).maxP() == vect3(5, 0, 5));
    assert(box.subBox(6).maxP() == vect3(0, 5, 5));
    assert(box.subBox(7).maxP() == vect3(5, 5, 5));
}