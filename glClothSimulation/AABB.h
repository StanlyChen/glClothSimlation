#pragma once
#include "RenderingTypes.h"



class AABB
{
public:
    AABB();
    AABB(const vect3 p1, const vect3 p2);
    ~AABB();

    void merge(const vect3& point);
    void merge(const AABB& box);
    vect3 center() const;
    vect3 minP() const;
    vect3 maxP() const;
    float diagonalLength() const;
    bool intersect_segment(const vect3 p1, vect3 p2);
    bool intersect_AABB(const AABB& other);

    unsigned subBoxId(const vect3& point) const;
    AABB subBox(unsigned i) const;
    
    static void generateTesellationData(AABB* pBox, CstPointSequence& vertices, CstInt32Sequence& indices, CstPointSequence& normals ); //triangles
    static void generateTesellationData(AABB* pBox, CstPointSequence& vertices, CstInt32Sequence& indices); //lines

    static void test();
private:
    vect3 m_minPt = glm::vec3();
    vect3 m_maxPt = glm::vec3();
};



