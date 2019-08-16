#include "PointRenderable.h"
#include <iostream>
#include <string>

void  PointRenderable::initRenderingData(const CstPointSequence& vertices)
{
    CstInt32Sequence indices;
    indices.reserve(vertices.size());
    for (unsigned i = 0; i < vertices.size(); ++i)
        indices.push_back(i);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo_vertices);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vect3)*vertices.size(), vertices.r_address(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*indices.size(), indices.r_address(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    m_vertexCount = (unsigned)indices.size();

    m_pRenderMethod = nullptr;
}

void PointRenderable::setRenderMethod(IRenderMethod* pMethod)
{
    m_pRenderMethod = pMethod;
}

void PointRenderable::updateVertices(const CstPointSequence& vertices)
{
    if (m_vbo_vertices != -1 && vertices.size() == m_vertexCount)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertices);

        vect3* pTarget = (vect3*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        memcpy(pTarget, vertices.r_address(), sizeof(vect3)*vertices.size());

        glUnmapBuffer(GL_ARRAY_BUFFER);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
void PointRenderable::destoryRenderingData()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo_vertices);
    glDeleteBuffers(1, &m_ebo);

    m_pRenderMethod = nullptr;
}

void PointRenderable::render(RenderSystem* pContext )
{
    glBindVertexArray(m_vao);

    m_pRenderMethod->bind();
    m_pRenderMethod->updateParams(pContext);

    glDrawElements(GL_POINTS, m_vertexCount, GL_UNSIGNED_INT, 0);

    m_pRenderMethod->unbind();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

