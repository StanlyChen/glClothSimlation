#include "Triangle.h"
#include <iostream>

bool isStrictSameSignal(REAL value1, REAL value2)
{
    if (value1 > 0 && value2 > 0)
        return true;
    if (value1 < 0 && value2 < 0)
        return true;
    return false;
}

bool isSameSignal( REAL value1, REAL value2)
{
    return value1 * value2 >= 0;
}

bool isDifferentSignal(REAL value1, REAL value2)
{
    return value1 * value2 <= 0;
}

bool isStrictDifferentSignal(REAL value1, REAL value2)
{
    return value1 * value2 < 0;
}

//return 0 means same-line
//otherwide return 12, 13, 23
int isTwoVectorOnSameLine(const vect3& V0, const vect3& V1)
{
    /* v0.x    v0.y      v0.z
      ------ = ------ = ----- = k
       v1.x    v1.y      v1.z

       to avoid dividing 0 error, we use the following formula:
    */

    if (V0.x*V1.y != V1.x * V0.y  )
        return 12;

    if (V0.x*V1.z != V1.x * V0.z)
        return 13;

    if (V0.y * V1.z != V1.y*V0.z)
        return 23;

    return 0;

}


Triangle::Triangle(vect3 _p0, vect3 _p1, vect3 _p2)
{
    m_v0 = _p0;
    m_v1 = _p1;
    m_v2 = _p2;

    m_normal = glm::normalize( glm::cross(_p1 - _p0, _p2 - _p0) );
}

bool Triangle::intersect_segment(const vect3& p0, const vect3& p1, REAL& t) const
{
    if (p0 == p1)
    {
        if( glm::dot(m_normal, p0 - m_v0) == 0)
        {
            t = 0.0f;
            return isPointInTriangle(p0);
        }
        return false;
    }

    /*
    * first calculate if segment intersect with triangle plane
    */
    auto vV0P0 = p0 - m_v0;
    auto vV0P1 = p1 - m_v0;

    auto proj_V0P0 = glm::dot(vV0P0, m_normal);
    auto proj_V0P1 = glm::dot(vV0P1, m_normal);


    if (isStrictSameSignal(proj_V0P0, proj_V0P1))
        //two points are on the same side on triangle plane.
        return false;

    if (proj_V0P0 == 0 && proj_V0P1 == 0)
    {
        return false;
        //this is the case, the segment is just on the plane

        //TODO handle this case
        //assert(false);
        //return true;
    }
    else
    {
        //the following is safe if p0 or p1 is on the plane.
        t = abs(proj_V0P0) / (abs(proj_V0P0) + abs(proj_V0P1));
        vect3 intersectPt = p0 + (p1 - p0)*t;
        return isPointInTriangle(intersectPt);
    }

}


//the intersectPt must be on the triangle plane
bool Triangle::isPointInTriangle(const vect3& intersectPt) const
{
    /*
    * In the second step we try to test if intersectPt is inside the triangle.
    * for any point p on the triangle plane,we can get three vector( v0-p, v1-p , v2-p), we say it (u, v ,k)
    * If we choose any two vector (if they are not on the same line) as axis of a 2-D coordindte system,
    * there must be some a and b make it happen : k = a*u + b*v.
    * the third vector must be in the third quadrant if p is inside the triangle.
    * So we can test if a < 0 and b < 0
    */

    auto pV0 = m_v0 - intersectPt;
    auto pV1 = m_v1 - intersectPt;
    auto pV2 = m_v2 - intersectPt;

    auto u = pV0;
    auto v = pV1;
    auto k = pV2;

    int choice = isTwoVectorOnSameLine(u, v);
    if (choice == 0)
    {
        std::swap(v, k);
        choice = isTwoVectorOnSameLine(u, v);
    }

    /*
    Ux*a + Vx*b = Kx  ------- formual_1
    Uy*a + Vy*b = Ky  ------- formual_2
    Uz*a + Vz*b = Kz  ------- formual_3
    */
    REAL denominator, numerator_a, numerator_b;
    if (choice == 12)
    {
        denominator = u.x*v.y - u.y*v.x;
        numerator_a = k.x*v.y - k.y*v.x;
        numerator_b = -k.x*u.y + k.y*u.x;
    }
    else if (choice == 13)
    {
        denominator = u.x*v.z - u.z*v.x;
        numerator_a = k.x*v.z - k.z*v.x;
        numerator_b = -k.x*u.z + k.z*u.x;
    }
    else if (choice == 23)
    {
        denominator = u.y*v.z - u.z*v.y;
        numerator_a = k.y*v.z - k.z*v.y;
        numerator_b = -k.y*u.z + k.z*u.y;
    }
    else
    {
        assert(false);
    }

    if (isDifferentSignal(numerator_a, denominator) && isDifferentSignal(numerator_b, denominator))
        return true;
    else
        return false;
}

void Triangle::test(vect3 t1, vect3 t2, vect3 t3, vect3 p1, vect3 p2)
{
    Triangle tri{ t1,t2,t3 };

    float t;
    if (tri.intersect_segment(p1, p2, t))
    {
        vect3 hitPos = p1 + t * (p2 - p1);
        std::cout << "hit:" << hitPos.x << " " << hitPos.y << " " << hitPos.z << std::endl;
    }
    else
    {
        std::cout << "not hit:"  << std::endl;
    }
    return;
}


bool Triangle::intersect_traingle(const Triangle& tri) const
{
    return getBoundingBox().intersect_AABB(tri.getBoundingBox());

}

AABB Triangle::getBoundingBox() const
{
    AABB box(m_v0, m_v1);
    box.merge(m_v2);
    return box;
}