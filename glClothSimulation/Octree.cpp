#include "Octree.h"
#include <algorithm>


const size_t DIVIDE_THREAD_HOLD = 100;

OctreeNode::OctreeNode(const AABB& box)
    :m_box(box)
    , m_children({nullptr,nullptr,nullptr,nullptr,
        nullptr,nullptr,nullptr,nullptr})
{

}

OctreeNode::~OctreeNode()
{
    if (hasChildren())
    {
        for (int i = 0; i < 8; ++i)
        {
            delete m_children[i];
        }
    }
}


bool OctreeNode::hasChildren() const
{
    return m_children[0] != nullptr;
}

void OctreeNode::addTriangle(const Triangle&  tri )
{
    if( hasChildren() )
        return;
    m_triangles.push_back( tri );
}



void OctreeNode::divide()
{
    if (hasChildren() )
        return;

    size_t triangleCount = m_triangles.size();
    if( triangleCount < DIVIDE_THREAD_HOLD )
    {
        return;
    }


    for( int i = 0; i < 8; ++i )
    {
        m_children[i] = new OctreeNode(m_box.subBox(i) );
        m_children[i]->m_triangles.reserve( triangleCount );
    }

    std::vector<Triangle> tmpTris;
    tmpTris.reserve(triangleCount);
    tmpTris.swap(m_triangles);

    for(auto& tri : tmpTris )
    {
        int subPosId0 = m_box.subBoxId( tri.v0() );
        int subPosId1 = m_box.subBoxId( tri.v1() );
        int subPosId2 = m_box.subBoxId( tri.v2() );

        if( subPosId0 == subPosId1 && subPosId0 == subPosId2 )
        {
            m_children[subPosId0]->addTriangle(tri);   
        }   
        else
        {
            m_triangles.push_back(tri);
        }
    }

    for( int i = 0; i < 8; ++i )
    {
        m_children[i]->m_triangles.shrink_to_fit();
        m_children[i]->divide();
    }
}

int OctreeNode::maxDeepth()
{
    if (hasChildren())
    {
        int maxDeepth = 0;
        for (int i = 0; i < 8; ++i)
        {
            maxDeepth = std::max(maxDeepth, m_children[i]->maxDeepth() + 1);
        }
        return maxDeepth;
    }
    else
    {
        return 1;
    }
}

TessellationData OctreeNode::getTessellationData(unsigned  maxDepth)
{
    TessellationData data;
    AABB::generateTesellationData(&m_box, data.m_vertices, data.m_indices);
    data.m_primType = LINE_PRIMITIVE;

    if (maxDepth > 0 && hasChildren())
    {
        for (int i = 0; i < 8; ++i)
        {
            TessellationData child_data = m_children[i]->getTessellationData(maxDepth -1 );

            data.m_indices.reserve( data.m_indices.size() + child_data.m_indices.size());
            for (auto it = child_data.m_indices.r_begin(); it != child_data.m_indices.r_end(); ++it)
            {
                data.m_indices.push_back(*it + (int)data.m_vertices.size());
            }
            data.m_vertices.insert(data.m_vertices.r_end(), child_data.m_vertices.r_begin(), child_data.m_vertices.r_end());
        }
    }

    return data;
}

bool   OctreeNode::triangle_intersection(const Triangle& tri)
{
    bool hited = false;
    if (m_box.intersect_AABB(tri.getBoundingBox()))
    {
        for (const auto& triangle : m_triangles)
        {
            if (triangle.intersect_traingle(tri))
                hited = true;
        }
    }

    if (hasChildren())
    {
        for (int i = 0; i < 8; ++i)
        {
            if ( m_children[i]->triangle_intersection(tri) )
            {
                hited = true;
            }
        }
    }

    return hited;
}

bool OctreeNode::segment_intersection(const vect3& p1, const vect3& p2, REAL& t)
{
    if (m_box.intersect_segment(p1, p2))
    {
        bool hited = false;
        float temp_t = 1;
        float min_t = 1;
        
        for (const auto& triangle : m_triangles)
        {
            if (triangle.intersect_segment(p1, p2, temp_t))
            {
                if (temp_t < min_t)
                {
                    hited = true;
                    min_t = temp_t;
                }
            }
        }

        if (hasChildren())
        {
            for (int i = 0; i < 8; ++i)
            {
                if (m_children[i]->segment_intersection(p1, p2, temp_t))
                {
                    if (temp_t < min_t)
                    {
                        hited = true;
                        min_t = temp_t;
                    }
                }
            }
        }

        if (hited)
            t = min_t;

        return hited;
    }
    else
    {
        return false;
    }
}