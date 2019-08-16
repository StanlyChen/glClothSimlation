#pragma once
#include "RenderingTypes.h"
#include <array>
#include <vector>
#include "AABB.h"
#include "Triangle.h"





class OctreeNode
{
private:
    AABB m_box;
    std::array< OctreeNode*,8> m_children;
    std::vector<Triangle> m_triangles;



public:
    OctreeNode(const AABB& box);
    ~OctreeNode();
    void addTriangle( const Triangle&  tri );
    void divide();

    bool hasChildren() const;
    int maxDeepth();

    TessellationData getTessellationData(unsigned maxDepth);

    bool segment_intersection(const vect3& p1, const vect3& p2, REAL& t);
    bool triangle_intersection(const Triangle& tri);
};


