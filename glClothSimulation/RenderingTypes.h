#pragma once
#include "PhysicalType.h"

using vect2 = glm::vec2;
using matrix4 = glm::mat4x4;



enum PrimitiveType
{
    POINT_PRIMITVIE,
    LINE_PRIMITIVE,
    TRIANGLE_PRIMITIVE,
};

struct TessellationData
{
    PrimitiveType m_primType;
    CstPointSequence m_vertices;
    CstInt32Sequence m_indices;
    CstPointSequence m_normals;
};