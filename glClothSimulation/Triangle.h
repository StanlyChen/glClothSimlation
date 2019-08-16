#pragma once
#include "PhysicalType.h"
#include "AABB.h"


struct Triangle
{
public:
    Triangle(vect3 _p0,vect3 _p1,vect3 _p2);
    
    vect3 v0() const { return m_v0; }
    vect3 v1() const { return m_v1; }
    vect3 v2() const { return m_v2; }


    bool intersect_segment(const vect3& v0, const vect3& v1, REAL& t) const;

    bool isPointInTriangle(const vect3& intersectPt) const;

    static void test(vect3 t1, vect3 t2, vect3 t3, vect3 p1, vect3 p2);

    bool intersect_traingle(const Triangle& tri) const;


    AABB getBoundingBox() const;

private:
    vect3 m_normal;
    vect3 m_v0;
    vect3 m_v1;
    vect3 m_v2;

};